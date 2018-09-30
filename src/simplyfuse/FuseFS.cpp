#include "FuseFS.h"

#define FUSE_USE_VERSION 31
#include <fuse.h>
#include <fuse/fuse_lowlevel.h>

#include <stdexcept>
#include <array>
#include <map>
#include <mutex>
#include <cstring>
#include <iostream>
#include <vector>

#include <filesystem>

namespace simplyfuse {
namespace {

struct Node {
	Node(std::string const& _name) : name(_name) {}
	Node* parent {nullptr};

	std::map<std::string, std::unique_ptr<Node>> children;
	FuseFile* file {nullptr};
	std::string name;

	Node* getOrCreateChild(std::string const& name) {
		auto& ptr = children[name];
		if (not ptr) {
			ptr = std::make_unique<Node>(name);
			ptr->parent = this;
		}
		return ptr.get();
	}
};

void destroyNode(Node* node) {
	Node* parent = node->parent;
	parent->children.erase(node->name);
	if (parent->children.empty() and parent->parent) {
		destroyNode(parent);
	}
}

static int getattr_callback(const char *path, struct stat *stbuf);
static int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi);
static int open_callback(const char *path, struct fuse_file_info *fi);
static int read_callback(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
static int write_callback(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
static int truncate_callback(const char *path, off_t offset);

}

struct FuseFS::Pimpl {
	struct fuse_chan* channel {nullptr};
	struct fuse* fuse {nullptr};
	std::filesystem::path mountPoint;
	int fuseFD {0};

	std::recursive_mutex mutex;
	std::map<Node*, FuseFile*> files;
	std::multimap<FuseFile*, Node*> filesInvMap;

	Node root{"/"};

	Node* getNode(std::filesystem::path const& path) {
		if (not path.is_absolute()) {
			throw InvalidPathError("path must be absolute");
		}
		std::lock_guard lock{mutex};
		Node* node = &root;
		for (auto it = std::next(path.begin()); it != path.end(); ++it) {
			auto childit = node->children.find(*it);
			if (childit == node->children.end()) {
				return nullptr;
			}
			node = childit->second.get();
		}
		return node;
	}

	bool tearDownMountPoint = false;
};

FuseFS::FuseFS(std::filesystem::path const& mountPoint) :
		pimpl { std::make_unique<Pimpl>() } {
	pimpl->mountPoint = mountPoint;
	// try to create the mountpoint
	std::filesystem::create_directory(mountPoint);

	std::vector<std::string> argStrings {"", "-o", "auto_unmount"};
	char* args[] = {
			argStrings[0].data(), argStrings[1].data(), argStrings[2].data(), nullptr
	};
	struct fuse_args fuse_args {3, args, false};
	pimpl->channel = fuse_mount(pimpl->mountPoint.c_str(), &fuse_args);
	if (not pimpl->channel) {
		throw MountError("cannot mount");
	}
	struct fuse_operations fuse_operations = { };
	fuse_operations.getattr = getattr_callback;
	fuse_operations.open = open_callback;
	fuse_operations.read = read_callback;
	fuse_operations.write = write_callback;
	fuse_operations.readdir = readdir_callback;
	fuse_operations.truncate = truncate_callback;
	pimpl->fuse = fuse_new(pimpl->channel, nullptr, &fuse_operations,
			sizeof(fuse_operations), this);
	pimpl->fuseFD = fuse_chan_fd(pimpl->channel);
}

FuseFS::~FuseFS() {
	fuse_unmount(pimpl->mountPoint.c_str(), pimpl->channel);
	fuse_destroy(pimpl->fuse);

	if (pimpl->tearDownMountPoint) {
		std::filesystem::remove(pimpl->mountPoint);
	}
}
int FuseFS::getFD() const {
	return pimpl->fuseFD;
}

void FuseFS::loop() {
	struct fuse_cmd *cmd = fuse_read_cmd(pimpl->fuse);
	if (cmd) {
		fuse_process_cmd(pimpl->fuse, cmd);
	}
}


void FuseFS::registerFile(std::filesystem::path const& _path, FuseFile& file) {
	std::filesystem::path path = _path.lexically_normal();
	if (not path.is_absolute()) {
		throw InvalidPathError("path must be absolute");
	}
	std::lock_guard lock{pimpl->mutex};
	Node* node = &pimpl->root;
	for (auto it = std::next(path.begin()); it != path.end(); ++it) {
		node = node->getOrCreateChild(*it);
		if (node->file) {
			throw InvalidPathError("path is invalid");
		}
	}
	node->file = &file;
	if (not node->children.empty()) {
		throw InvalidPathError("file already exists");
	}
	pimpl->files[node] = &file;
	pimpl->filesInvMap.emplace(&file, node);
}

void FuseFS::unregisterFile(FuseFile& file) {
	std::lock_guard lock{pimpl->mutex};

	auto range = pimpl->filesInvMap.equal_range(&file);
	for (auto it = range.first; it != range.second; ++it) {
		Node* node = it->second; // destroy this node
		pimpl->files.erase(node);
		destroyNode(node);
	}
}

void FuseFS::unregisterFile(std::filesystem::path const& path, FuseFile& file) {
	std::lock_guard lock{pimpl->mutex};
	Node* node = pimpl->getNode(path);
	if (not node or node->file != &file) {
		throw InvalidPathError("cannot unregister a wrong file");
	}
	pimpl->files.erase(node);
	destroyNode(node);
}

namespace {

Node* getNode(std::string const& path) {
	struct fuse_context* context = fuse_get_context();
	FuseFS* fusefs = reinterpret_cast<FuseFS*>(context->private_data);
	return fusefs->pimpl->getNode(path);
}

int getattr_callback(const char *path, struct stat *stbuf) {
	memset(stbuf, 0, sizeof(*stbuf));

	Node* node = getNode(path);
	if (not node) {
		return -ENOENT;
	}
	clock_gettime(CLOCK_REALTIME, &stbuf->st_mtim);
	if (node->file) {
		stbuf->st_mode = S_IFREG | 0666;
		stbuf->st_nlink = 1;
		stbuf->st_size = node->file->getSize();
		return 0;
	} else {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
		return 0;
	}
	return -ENOENT;
}

int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler, off_t, struct fuse_file_info *) {
	Node* node = getNode(path);
	if (not node) {
		return -ENOENT;
	}
	for (auto& child : node->children) {
		filler(buf, child.first.c_str(), NULL, 0);
	}

	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);
	return 0;
}

int open_callback(const char *path, struct fuse_file_info *) {
	Node* node = getNode(path);
	if (not node or not node->file) {
		return -ENOENT;
	}
	return node->file->onOpen();
}

int read_callback(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *) {
	Node* node = getNode(path);
	if (not node or not node->file) {
		return -ENOENT;
	}
	return node->file->onRead(buf, size, offset);
}

int write_callback(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *) {
	Node* node = getNode(path);
	if (not node or not node->file) {
		return -ENOENT;
	}
	return node->file->onWrite(buf, size, offset);
}

int truncate_callback(const char *path, off_t offset) {
	Node* node = getNode(path);
	if (not node or not node->file) {
		return -ENOENT;
	}
	return node->file->onTruncate(offset);
}


}

}

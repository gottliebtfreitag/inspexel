#include "FuseFile.h"
#include "FuseFS.h"

#include <cstring>

namespace simplyfuse {

FuseFile::~FuseFile() {
	if (fuseFS) {
		fuseFS->unregisterFile(*this);
	}
}
int FuseFile::onOpen()  { return 0; }
int FuseFile::onClose() { return 0; }
int FuseFile::onRead(char*, std::size_t, off_t)  { return -ENOENT; }
int FuseFile::onWrite(const char*, std::size_t, off_t) { return -ENOENT; }
std::size_t FuseFile::getSize() { return 4096; }
int FuseFile::onTruncate(off_t) { return -ENOENT; }

int FuseFile::getFilePermissions() {
	return 0666;
}

int SimpleROFile::onRead(char* buf, std::size_t size, off_t) {
	size = std::min(size, content.size()+1);
	std::memcpy(buf, content.data(), size);
	return size;
}

int SimpleROFile::getFilePermissions() {
	return 0444;
}

int SimpleRWFile::onWrite(const char* buf, std::size_t size, off_t offset) {
	content.resize(content.size() + offset + size);
	std::memcpy(content.data() + offset, buf, size);
	return size;
}

int SimpleRWFile::onTruncate(off_t offset) {
	content.resize(offset);
	return 0;
}

int SimpleRWFile::getFilePermissions() {
	return 0666;
}


}

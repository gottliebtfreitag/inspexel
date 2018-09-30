#pragma once

#include <string>
#include <memory>
#include <type_traits>
#include <stdexcept>
#include <filesystem>

#include "FuseFile.h"

namespace simplyfuse {

struct FileExistsError : std::logic_error {
	FileExistsError(std::string const& msg) : std::logic_error(msg) {}
};
struct InvalidPathError : std::logic_error {
	InvalidPathError(std::string const& msg) : std::logic_error(msg) {}
};
struct MountError : std::logic_error {
	MountError(std::string const& msg) : std::logic_error(msg) {}
};

struct FuseFS {
	friend class FuseFile;
	FuseFS(std::filesystem::path const& mountPoint);
	virtual ~FuseFS();

	void loop();

	int getFD() const;

	// register a file in the file system (a file can be registered multiple times)
	void registerFile(std::filesystem::path const& path, FuseFile& file);

	// unregister a file from the file system
	void unregisterFile(std::filesystem::path const& path, FuseFile& file);

	// unregister all instances of this file from the file system
	void unregisterFile(FuseFile& file);

	// create a directory (will create intermediate directories, too)
	void mkdir(std::filesystem::path const& path);

	// delete a directory and all its contents.
	// files registered within the deleted directory will simply be deregistered
	void rmdir(std::filesystem::path const& path);

	struct Pimpl;
	std::unique_ptr<Pimpl> pimpl;

};

}

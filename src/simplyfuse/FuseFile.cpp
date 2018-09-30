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


int SimpleROFile::onRead(char* buf, std::size_t size, off_t) {
	size = std::min(size, content.size()+1);
	std::memcpy(buf, content.data(), size);
	return size;
}

}

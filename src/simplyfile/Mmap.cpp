#include <simplyfile/Mmap.h>

#include <sys/stat.h>

namespace simplyfile
{

Mmap::Mmap(FileDescriptor&& fd, int protocol, int flags, std::size_t size, int offset)
	: FileDescriptor(std::move(fd))
	, _size(size)
{
	if (_size == 0) {
		struct stat st;
		fstat(*this, &st);
		_size = st.st_size;
	}
	_data = mmap(nullptr, size, protocol, flags,
	                  *this, offset);
}

Mmap::~Mmap()
{
	munmap(_data, _size);
}

} /* namespace simplyfile */

#pragma once

#include "simplyfile/FileDescriptor.h"
#include "sys/mman.h"

namespace simplyfile
{

struct Mmap : FileDescriptor
{
private:
	std::size_t _size {0};
	void* _data{nullptr};

public:
	Mmap() = default;
	Mmap(FileDescriptor&& fd, int protocol=PROT_READ|PROT_WRITE, int flags=MAP_SHARED, std::size_t size=0, int offset=0);

	Mmap(Mmap&&) = default;
	Mmap& operator=(Mmap&&) = default;

	virtual ~Mmap();

	auto data() -> void* {
		return _data;
	}
	auto data() const -> void const* {
		return _data;
	}
	auto size() const -> std::size_t {
		return _size;
	}

	template<typename T>
	auto as() -> T* {
		if (_size < sizeof(T)) {
			throw std::runtime_error("cannot access an mmaped file as a bigger type than the file");
		}
		return reinterpret_cast<T*>(_data);
	}

	template<typename T>
	auto as() const -> T const* {
		if (_size < sizeof(T)) {
			throw std::runtime_error("cannot access an mmaped file as a bigger type than the file");
		}
		return reinterpret_cast<T*>(_data);
	}
};

} /* namespace simplyfile */

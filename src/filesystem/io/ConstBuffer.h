#pragma once 

#include <filesystem/compatibility/Compatibility.h>


__FILESYSTEM_IO_NAMESPACE_BEGIN

class const_buffer {
public:
	const_buffer(const void* __mem, sizetype __size) noexcept :
		_mem(__mem), _size(__size)
	{}

	sizetype size() const noexcept {
		return _size;
	}
	
	const void* data() noexcept {
		return _mem;
	}

	operator const void* () noexcept {
		return _mem;
	}

	const_buffer operator+(sizetype __offset) const noexcept {
		return const_buffer(static_cast<const char*>(_mem) + __offset, _size - __offset);
	}
private:
	const void* _mem = nullptr;
	sizetype _size = 0;
};

__FILESYSTEM_IO_NAMESPACE_END

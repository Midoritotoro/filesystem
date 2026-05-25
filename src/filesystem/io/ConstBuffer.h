#pragma once 

#include <filesystem/compatibility/Compatibility.h>


__FILESYSTEM_IO_NAMESPACE_BEGIN

class const_buffer {
public:
	using buffer_type = const void*;
	using size_type = sizetype;

	const_buffer(buffer_type mem, size_type size) noexcept :
		_mem(mem), _size(size)
	{}

	size_type size() const noexcept {
		return _size;
	}
	
	buffer_type data() const noexcept {
		return _mem;
	}

	operator buffer_type() const noexcept {
		return _mem;
	}

	const_buffer operator+(size_type offset) const noexcept {
		return const_buffer(static_cast<const char*>(_mem) + offset, _size - offset);
	}
private:
	buffer_type _mem = nullptr;
	size_type _size = 0;
};

__FILESYSTEM_IO_NAMESPACE_END

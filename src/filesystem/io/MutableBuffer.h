#pragma once 

#include <filesystem/compatibility/Compatibility.h>


__FILESYSTEM_IO_NAMESPACE_BEGIN

class mutable_buffer {
public:
	mutable_buffer(void* __mem, sizetype __size) noexcept :
		_mem(__mem), _size(__size)
	{}
	
	mutable_buffer(sizetype __size) noexcept :
		_mem(::operator new(__size)), _size(__size)
	{}

	sizetype size() const noexcept {
		return _size;
	}
	
	void* data() noexcept {
		return _mem;
	}

	operator void* () noexcept {
		return _mem;
	}

	mutable_buffer operator+(sizetype __offset) const noexcept {
		return mutable_buffer(static_cast<char*>(_mem) + __offset, _size - __offset);
	}
private:
	void* _mem = nullptr;
	sizetype _size = 0;
};

__FILESYSTEM_IO_NAMESPACE_END

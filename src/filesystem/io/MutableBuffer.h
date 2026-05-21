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
private:
	void* _mem = nullptr;
	sizetype _size = 0;
};

__FILESYSTEM_IO_NAMESPACE_END

#pragma once 

#include <filesystem/compatibility/Compatibility.h>


__FILESYSTEM_IO_NAMESPACE_BEGIN

class consuming_buffer {
public:
	consuming_buffer(const void* __mem, sizetype __size) noexcept :
		_mem(__mem), _size(__size)
	{}

	sizetype size() const noexcept {
		return _size;
	}
	
	const void* data() noexcept {
		return _mem;
	}
private:
	const void* _mem = nullptr;
	sizetype _size = 0;
	sizetype _consumed = 0;
};

__FILESYSTEM_IO_NAMESPACE_END

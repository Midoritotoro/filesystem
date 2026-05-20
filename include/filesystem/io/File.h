#pragma once 

#include <src/filesystem/io/FileData.h>
#include <src/filesystem/system/Handle.h>
#include <filesystem/io/Path.h>


__FILESYSTEM_IO_NAMESPACE_BEGIN

class file {
public: 
	file(system::handle __handle) noexcept: _handle(__handle) {}
	~file() {

	}
private:
	system::handle _handle;
};

__FILESYSTEM_IO_NAMESPACE_END

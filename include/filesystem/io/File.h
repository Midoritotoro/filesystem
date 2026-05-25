#pragma once 

#include <src/filesystem/io/FileData.h>
#include <src/filesystem/system/Handle.h>
#include <src/filesystem/io/Exceptions.h>


__FILESYSTEM_IO_NAMESPACE_BEGIN

class file {
public:
    file() noexcept {};
    explicit file(const path& p, system::handle h) {
        _handle = h;
        _path = p;

        filesystem_assert(_handle.available());
    }

    ~file() {
        _handle.destroy();
    }

    file(file&& other) noexcept {
		_handle = std::move(other._handle);
		_path = std::move(other._path);
    }

    file& operator=(file&& other) noexcept {
        if (this != &other) {
            _handle = std::move(other._handle);
            _path = std::move(other._path);
        }
        return *this;
    }

    file(const file&) = delete;
    file& operator=(const file&) = delete;

    bool is_open() const noexcept { 
        return _handle.available(); 
    }

    system::handle handle() noexcept { 
        return _handle; 
    }

    void reset() noexcept { 
        _handle.destroy(); 
    }
private:
    path _path;
    system::handle _handle;
};

__FILESYSTEM_IO_NAMESPACE_END
#pragma once 

#include <src/filesystem/io/FileData.h>
#include <src/filesystem/system/Handle.h>
#include <src/filesystem/io/Exceptions.h>


__FILESYSTEM_IO_NAMESPACE_BEGIN

class file {
public:
    file() noexcept = default;
    explicit file(const path& __path, system::handle __h) {
        _handle = __h;
        _path = __path;

        if (!_handle.available()) throw filesystem_error("Invalid handle", _path, __get_last_error_code());
    }

    ~file() {
        _handle.destroy();
    }

    file(file&& __other) noexcept {
		_handle = std::move(__other._handle);
		_path = std::move(__other._path);
    }

    file& operator=(file&& __other) noexcept {
        if (this != &__other) {
            _handle = std::move(__other._handle);
            _path = std::move(__other._path);
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
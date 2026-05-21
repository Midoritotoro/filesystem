#pragma once 

#include <src/filesystem/io/FileData.h>
#include <src/filesystem/system/Handle.h>
#include <src/filesystem/io/Exceptions.h>


__FILESYSTEM_IO_NAMESPACE_BEGIN

class file {
public:
    file() noexcept = default;
    explicit file(const path& __path, system::handle __h) throw() {
        if (!__h.available()) throw filesystem_error("Invalid handle", _path, __make_std_error_code(__get_last_error_code()));
        else {
            _handle = __h;
            _path = __path;
        }
    }

    ~file() {}

    file(file&&) noexcept = default;
    file& operator=(file&&) noexcept = default;
    file(const file&) = delete;
    file& operator=(const file&) = delete;

    bool is_open() const noexcept { 
        return _handle.available(); 
    }

    system::handle native_handle() noexcept { 
        return _handle; 
    }

    void reset() noexcept { 
        if (_handle.available()) _handle.destroy(); 
    }
private:
    path _path;
    system::handle _handle;
};

__FILESYSTEM_IO_NAMESPACE_END
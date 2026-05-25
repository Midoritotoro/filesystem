#pragma once 

#include <filesystem/io/Path.h>
#include <src/filesystem/system/IoError.h>
#include <string_view>
#include <exception>

__FILESYSTEM_IO_NAMESPACE_BEGIN

inline system::__fs_win_error __get_last_win_error() noexcept {
    return system::__fs_win_error(GetLastError());
}

inline system::io_error __get_last_io_error() noexcept {
	return __from_win32_error(__get_last_win_error());
}

class filesystem_error : public std::system_error {
public:
    filesystem_error(const std::string& __message, const std::error_code __error_code):
        system_error(__error_code, __message), __what(std::system_error::what())
    {}

    filesystem_error(const std::string& __message, const io::path& __path, const std::error_code __error_code):
        system_error(__error_code, __message), _path(__path),
        __what(std::system_error::what())
    {}

    const io::path& path() const noexcept {
        return _path;
    }

    const char* what() const noexcept override {
        return __what.c_str();
    }
private:
    io::path _path;
    std::string __what;
};


__FILESYSTEM_IO_NAMESPACE_END

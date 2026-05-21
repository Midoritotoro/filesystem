#pragma once 

#include <filesystem/io/Path.h>
#include <src/filesystem/system/WindowsErrors.h>
#include <string_view>
#include <exception>

__FILESYSTEM_IO_NAMESPACE_BEGIN

system::__fs_win_error __get_last_win_error() noexcept {
    return system::__fs_win_error(GetLastError());
}

std::error_code __make_std_error_code(system::__fs_win_error __error) noexcept {
    return std::error_code(static_cast<i32>(__error), std::system_category());
}


std::error_code __get_last_error_code() noexcept {
	return __make_std_error_code(__get_last_win_error());
}

class filesystem_error : public std::system_error {
public:
    filesystem_error(const std::string& __message, const std::error_code __error_code):
        system_error(__error_code, __message), __what(std::runtime_error::what()) 
    {}

    filesystem_error(const std::string& __message, const io::path& __path, const std::error_code __error_code):
        system_error(__error_code, __message), _path(__path),
        __what(std::runtime_error::what()) 
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

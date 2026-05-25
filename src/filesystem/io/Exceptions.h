#pragma once 

#include <filesystem/io/Path.h>
#include <src/filesystem/system/IoError.h>
#include <string_view>
#include <exception>

__FILESYSTEM_IO_NAMESPACE_BEGIN

class filesystem_error : public std::system_error {
public:
    filesystem_error(const std::string& message, const std::error_code error_code):
        system_error(error_code, message), _what(std::system_error::what())
    {}

    filesystem_error(const std::string& message, const io::path& path, const std::error_code error_code):
        system_error(error_code, message), _path(path),
        _what(std::system_error::what())
    {}

    const io::path& path() const noexcept {
        return _path;
    }

    const char* what() const noexcept override {
        return _what.c_str();
    }
private:
    io::path _path;
    std::string _what;
};


__FILESYSTEM_IO_NAMESPACE_END

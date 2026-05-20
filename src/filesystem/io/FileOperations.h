#pragma once 

#include <src/filesystem/Options.h>
#include <src/filesystem/io/Async.h>
#include <filesystem/io/File.h>

__FILESYSTEM_IO_NAMESPACE_BEGIN

struct for_read_open_mode {};
constexpr inline auto for_read = fs::options::flag(for_read_open_mode{});
struct for_read_option : fs::options::exact_option<for_read> {};

struct for_write_open_mode {};
constexpr inline auto for_write = fs::options::flag(for_write_open_mode{});
struct for_write_option : fs::options::exact_option<for_write> {};

struct for_append_open_mode {};
constexpr inline auto for_append = fs::options::flag(for_append_open_mode{});
struct for_append_option : fs::options::exact_option<for_append> {};

struct binary_open_mode {};
constexpr inline auto as_binary = fs::options::flag(binary_open_mode{});
struct binary_open_option : fs::options::exact_option<as_binary> {};

template <class _Options_>
struct _Configurable_open : fs::options::strict_elementwise_callable<_Configurable_open, _Options_, for_read_option,
	for_write_option, for_append_option, binary_open_option> 
{

};

template <class _Options_>
struct _Configurable_read : fs::options::strict_elementwise_callable<_Configurable_read, _Options_> {};

__FILESYSTEM_IO_NAMESPACE_END

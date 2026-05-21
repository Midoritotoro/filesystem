#pragma once 

#include <src/filesystem/Options.h>
#include <src/filesystem/io/FileOperationsImpl.h>

__FILESYSTEM_IO_NAMESPACE_BEGIN

struct for_read_open_mode {};
struct for_write_open_mode {};
struct share_delete_mode {};
struct share_read_mode {};
struct share_write_mode {};
struct open_always_mode {};
struct binary_open_mode {};
struct async_open_mode {};
struct hidden_open_mode {};
struct directory_open_mode {};
struct symlink_open_mode {};
struct junction_open_mode {};

constexpr inline auto for_read = fs::options::flag(for_read_open_mode{});
constexpr inline auto for_write = fs::options::flag(for_write_open_mode{});
constexpr inline auto share_delete = fs::options::flag(share_delete_mode{});
constexpr inline auto share_read = fs::options::flag(share_read_mode{});
constexpr inline auto share_write = fs::options::flag(share_write_mode{});
constexpr inline auto open_always = fs::options::flag(open_always_mode{});
constexpr inline auto as_binary = fs::options::flag(binary_open_mode{});
constexpr inline auto async = fs::options::flag(async_open_mode{});
constexpr inline auto hidden = fs::options::flag(hidden_open_mode{});
constexpr inline auto directory = fs::options::flag(directory_open_mode{});
constexpr inline auto symlink = fs::options::flag(symlink_open_mode{});
constexpr inline auto junction = fs::options::flag(junction_open_mode{});

struct async_open_option : fs::options::exact_option<async> {};
struct binary_open_option : fs::options::exact_option<as_binary> {};
struct for_read_option : fs::options::exact_option<for_read> {};
struct for_write_option : fs::options::exact_option<for_write> {};
struct share_delete_option : fs::options::exact_option<share_delete> {};
struct share_read_option : fs::options::exact_option<share_read> {};
struct share_write_option : fs::options::exact_option<share_write> {};
struct open_always_option : fs::options::exact_option<open_always> {};
struct hidden_open_option : fs::options::exact_option<hidden> {};
struct directory_open_option : fs::options::exact_option<directory> {};
struct symlink_open_option : fs::options::exact_option<symlink> {};
struct junction_open_option : fs::options::exact_option<junction> {};

template <class _Options_>
filesystem_nodiscard constexpr filesystem_always_inline auto __fs_access_flags_from_options() noexcept {
	auto __result = __fs_win_file_access_flags(__fs_win_file_access_mode::__read);
	if constexpr (_Options_::contains(for_write)) __result |= __fs_win_file_access_mode::__write;
	return __result;
}

template <class _Options_>
filesystem_nodiscard constexpr filesystem_always_inline auto __fs_share_flags_from_options() noexcept {
	auto __result = __fs_share_mode_flags(__fs_win_share_mode::__none);

	if constexpr (_Options_::contains(share_delete)) __result |= __fs_win_share_mode::__delete;
	if constexpr (_Options_::contains(share_read)) __result |= __fs_win_share_mode::__read;
	if constexpr (_Options_::contains(share_write)) __result |= __fs_win_share_mode::__write;

	return __result;
}

template <class _Options_>
filesystem_nodiscard constexpr filesystem_always_inline auto __fs_creation_disposition_from_options() noexcept {
	if constexpr (_Options_::contains(open_always)) return __fs_win_file_creation_disposition::__open_always;
	else return __fs_win_file_creation_disposition::__open_existing;
}

template <class _Options_>
filesystem_nodiscard constexpr filesystem_always_inline auto __fs_file_attributes_from_options() noexcept {
	auto __attrs = __fs_win_file_attributes(__fs_win_file_attributes_data::__normal) | __fs_win_file_attributes_data::__overlapped;


	return __attrs;
}

template <class _Options_>
struct _Configurable_open : fs::options::strict_elementwise_callable<_Configurable_open, 
	_Options_, for_read_option, for_write_option, binary_open_option, share_delete_option, 
	share_read_option, share_write_option, open_always_option, async_open_option>
{
	filesystem_nodiscard filesystem_always_inline file operator()(const path& __path) {
		return fs::options::__dispatch_call(*this, __path);
	}

	static filesystem_always_inline auto deferred_call(const path& __path) {
		return __fs_open_file(__path, __fs_access_flags_from_options<_Options_>(), __fs_share_flags_from_options<_Options_>(), 
			__fs_creation_disposition_from_options<_Options_>(), __fs_file_attributes_from_options<_Options_>());
	}

	using callable_tag_type = _Configurable_open;
};

template <class _Options_>
struct _Configurable_read : fs::options::strict_elementwise_callable<_Configurable_read, _Options_> {
	filesystem_nodiscard filesystem_always_inline sizetype operator()(
		file& __file, mutable_buffer __buffer, dword_t __bytes_count, dword_t __offset) const noexcept
	{
		return fs::options::__dispatch_call(*this, __file, __buffer, __bytes_count, __offset);
	}

	static filesystem_always_inline auto deferred_call(
		file& __file, mutable_buffer __buffer, dword_t __bytes_count, dword_t __offset) noexcept
	{
		return __fs_read_file(__file, __buffer, __bytes_count, __offset);
	}

	using callable_tag_type = _Configurable_read;
};

template <class _Options_>
struct _Configurable_write : fs::options::strict_elementwise_callable<_Configurable_write, _Options_> {
	using callable_tag_type = _Configurable_write;
};


template <class _Options_>
struct _Configurable_create : fs::options::strict_elementwise_callable<_Configurable_create, _Options_> {
	using callable_tag_type = _Configurable_create;
};

template <class _Options_>
struct _Configurable_copy : fs::options::strict_elementwise_callable<_Configurable_copy, _Options_> {
	using callable_tag_type = _Configurable_copy;
};

template <class _Options_>
struct _Configurable_move : fs::options::strict_elementwise_callable<_Configurable_move, _Options_> {
	using callable_tag_type = _Configurable_move;
};

template <class _Options_>
struct _Configurable_rename : fs::options::strict_elementwise_callable<_Configurable_rename, _Options_> {
	using callable_tag_type = _Configurable_rename;
};

template <class _Options_>
struct _Configurable_delete : fs::options::strict_elementwise_callable<_Configurable_delete, _Options_> {
	using callable_tag_type = _Configurable_delete;
};

__FILESYSTEM_IO_NAMESPACE_END

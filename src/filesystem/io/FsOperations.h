#pragma once 

#include <src/filesystem/Options.h>
#include <src/filesystem/io/FileOperationsImpl.h>
#include <ranges>

__FILESYSTEM_IO_NAMESPACE_BEGIN

template <class _It_>
filesystem_always_inline auto buffer(_It_ __first, _It_ __last) noexcept {
	constexpr auto __size = sizeof(std::iter_value_t<_It_>);

	if constexpr (std::is_const_v<std::remove_reference_t<std::iter_reference_t<_It_>>>)
		return const_buffer(std::to_address(__first), static_cast<sizetype>(__last - __first) * __size);
	else return mutable_buffer(std::to_address(__first), static_cast<sizetype>(__last - __first) * __size);
}

template <std::ranges::contiguous_range _Range_>
filesystem_always_inline auto buffer(_Range_& __r) noexcept {
    if constexpr (std::is_const_v<std::remove_reference_t<_Range_>>)
        return const_buffer(std::ranges::data(__r), std::ranges::size(__r) * sizeof(std::ranges::range_value_t<_Range_>));
    else return mutable_buffer(std::ranges::data(__r), std::ranges::size(__r) * sizeof(std::ranges::range_value_t<_Range_>));
}

struct for_read_mode {};
struct for_write_mode {};
struct always_mode {};
struct share_delete_mode {};
struct share_read_mode {};
struct share_write_mode {};
struct async_mode {};
struct hidden_mode {};
struct as_directory_mode {};
struct as_symlink_mode {};
struct as_junction_mode {};
struct if_not_exists_mode {};

constexpr inline auto for_read = fs::options::flag(for_read_mode{});
constexpr inline auto for_write = fs::options::flag(for_write_mode{});
constexpr inline auto share_delete = fs::options::flag(share_delete_mode{});
constexpr inline auto share_read = fs::options::flag(share_read_mode{});
constexpr inline auto share_write = fs::options::flag(share_write_mode{});
constexpr inline auto always = fs::options::flag(always_mode{});
constexpr inline auto async = fs::options::flag(async_mode{});
constexpr inline auto hidden = fs::options::flag(hidden_mode{});
constexpr inline auto as_directory = fs::options::flag(as_directory_mode{});
constexpr inline auto as_symlink = fs::options::flag(as_symlink_mode{});
constexpr inline auto as_junction = fs::options::flag(as_junction_mode{});
constexpr inline auto if_not_exists = fs::options::flag(if_not_exists_mode{});

struct async_option : fs::options::exact_option<async> {};
struct for_read_option : fs::options::exact_option<for_read> {};
struct for_write_option : fs::options::exact_option<for_write> {};
struct share_delete_option : fs::options::exact_option<share_delete> {};
struct share_read_option : fs::options::exact_option<share_read> {};
struct share_write_option : fs::options::exact_option<share_write> {};
struct always_option : fs::options::exact_option<always> {};
struct hidden_option : fs::options::exact_option<hidden> {};
struct as_directory_option : fs::options::exact_option<as_directory> {};
struct as_symlink_option : fs::options::exact_option<as_symlink> {};
struct as_junction_option : fs::options::exact_option<as_junction> {};
struct if_not_exists_option : fs::options::exact_option<if_not_exists> {};

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
filesystem_nodiscard constexpr filesystem_always_inline auto __fs_file_attributes_from_options() noexcept {
	auto __attrs = __fs_win_file_attributes(__fs_win_file_attributes_data::__normal) | __fs_win_file_attributes_data::__overlapped;
	if constexpr (_Options_::contains(hidden)) __attrs |= __fs_win_file_attributes_data::__hidden;
	return __attrs;
}

template <class _Options_>
struct _Configurable_open : fs::options::strict_elementwise_callable<_Configurable_open, 
	_Options_, for_read_option, for_write_option, share_delete_option, 
	share_read_option, share_write_option, open_always_option, as_directory_option>
{
private:
	static filesystem_nodiscard constexpr filesystem_always_inline auto __open_disposition() noexcept {
		if constexpr (_Options_::contains(always)) return __fs_win_file_creation_disposition::__open_always;
		else return __fs_win_file_creation_disposition::__open_existing;
	}
public:
	using callable_tag_type = _Configurable_open;

	filesystem_nodiscard filesystem_always_inline file operator()(const path& __path) const {
		return fs::options::__dispatch_call(*this, __path);
	}

	static filesystem_always_inline auto deferred_call(auto __options, const path& __path) {
		return __fs_create_file(__path, __fs_access_flags_from_options<_Options_>(),
			__fs_share_flags_from_options<_Options_>(), __open_disposition(),
			__fs_file_attributes_from_options<_Options_>());
	}
};

template <class _Options_>
struct _Configurable_read : fs::options::strict_elementwise_callable<_Configurable_read, _Options_, async_option> {
	using callable_tag_type = _Configurable_read;

	filesystem_nodiscard filesystem_always_inline sizetype operator()(
		file& __file, mutable_buffer __buffer, sizetype __offset) const
	{
		return fs::options::__dispatch_call(*this, __file, __buffer, __offset);
	}

	static filesystem_always_inline auto deferred_call(
		auto __options, file& __file, mutable_buffer __buffer, sizetype __offset)
	{
		//if constexpr (_Options_::contains(async)) return __fs_read_file_async(__file, __buffer, __offset);
		//else 
		return __fs_read_file(__file, __buffer, __offset);
	}
};

template <class _Options_>
struct _Configurable_write : fs::options::strict_elementwise_callable<_Configurable_write, _Options_, async_option> {
	using callable_tag_type = _Configurable_write;

	filesystem_nodiscard filesystem_always_inline sizetype operator()(
		file& __file, const_buffer __buffer, sizetype __offset) const
	{
		return fs::options::__dispatch_call(*this, __file, __buffer, __offset);
	}

	static filesystem_always_inline auto deferred_call(
		auto __options, file& __file, const_buffer __buffer, sizetype __offset)
	{
		return __fs_write_file(__file, __buffer, __offset);
	}
};

template <class _Options_>
struct _Configurable_append : fs::options::strict_elementwise_callable<_Configurable_append, _Options_, async_option> {
	using callable_tag_type = _Configurable_append;

	filesystem_nodiscard filesystem_always_inline sizetype operator()(file& __file, const_buffer __buffer) const {
		return fs::options::__dispatch_call(*this, __file, __buffer);
	}

	static filesystem_always_inline auto deferred_call(auto __options, file& __file, const_buffer __buffer) {
		return __fs_append_file(__file, __buffer);
	}
};


template <class _Options_>
struct _Configurable_create : fs::options::strict_elementwise_callable<_Configurable_create, _Options_,
	for_read_option, for_write_option, share_delete_option, share_read_option, share_write_option, 
	always_option, if_not_exists_option, as_directory_option>
{
private:
	static filesystem_nodiscard constexpr filesystem_always_inline auto __disposition() noexcept {
		if constexpr (_Options_::contains(always)) return __fs_win_file_creation_disposition::__create_always;
		else return __fs_win_file_creation_disposition::__create_new;
	}
public:
	using callable_tag_type = _Configurable_create;
	using return_type = file;

	filesystem_nodiscard filesystem_always_inline return_type operator()(const path& __path) const {
		return fs::options::__dispatch_call(*this, __path);
	}

	static filesystem_always_inline auto deferred_call(auto __options, const path& __path) {
		return __fs_create_file(__path, __fs_access_flags_from_options<_Options_>(),
			__fs_share_flags_from_options<_Options_>(), __disposition(),
			__fs_file_attributes_from_options<_Options_>());
	}
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
struct _Configurable_remove : fs::options::strict_elementwise_callable<_Configurable_remove, _Options_> {
	using callable_tag_type = _Configurable_remove;

	filesystem_nodiscard filesystem_always_inline bool operator()(const path& __path) const {
		return fs::options::__dispatch_call(*this, __path);
	}

	static filesystem_always_inline auto deferred_call(auto __options, const path& __path) {
		return __fs_remove_file(__path, __fs_access_flags_from_options<_Options_>());
	}
};

constexpr inline auto open = fs::options::functor<_Configurable_open>;
constexpr inline auto read = fs::options::functor<_Configurable_read>;
constexpr inline auto write = fs::options::functor<_Configurable_write>;
constexpr inline auto append = fs::options::functor<_Configurable_append>;
constexpr inline auto create = fs::options::functor<_Configurable_create>;
constexpr inline auto copy = fs::options::functor<_Configurable_copy>;
constexpr inline auto move = fs::options::functor<_Configurable_move>;
constexpr inline auto remove = fs::options::functor<_Configurable_remove>;
constexpr inline auto rename = fs::options::functor<_Configurable_rename>;

__FILESYSTEM_IO_NAMESPACE_END

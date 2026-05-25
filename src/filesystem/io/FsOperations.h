#pragma once 

#include <src/filesystem/Options.h>
#include <src/filesystem/io/FileOperationsImpl.h>
#include <src/filesystem/system/IoError.h>
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
struct directory_mode {};
struct symlink_mode {};
struct junction_mode {};
struct if_not_exists_mode {};

constexpr inline auto for_read = fs::options::flag(for_read_mode{});
constexpr inline auto for_write = fs::options::flag(for_write_mode{});
constexpr inline auto share_delete = fs::options::flag(share_delete_mode{});
constexpr inline auto share_read = fs::options::flag(share_read_mode{});
constexpr inline auto share_write = fs::options::flag(share_write_mode{});
constexpr inline auto always = fs::options::flag(always_mode{});
constexpr inline auto async = fs::options::flag(async_mode{});
constexpr inline auto hidden = fs::options::flag(hidden_mode{});
constexpr inline auto dir = fs::options::flag(directory_mode{});
constexpr inline auto link = fs::options::flag(symlink_mode{});
constexpr inline auto junct = fs::options::flag(junction_mode{});
constexpr inline auto if_not_exists = fs::options::flag(if_not_exists_mode{});

struct async_option : fs::options::exact_option<async> {};
struct for_read_option : fs::options::exact_option<for_read> {};
struct for_write_option : fs::options::exact_option<for_write> {};
struct share_delete_option : fs::options::exact_option<share_delete> {};
struct share_read_option : fs::options::exact_option<share_read> {};
struct share_write_option : fs::options::exact_option<share_write> {};
struct always_option : fs::options::exact_option<always> {};
struct hidden_option : fs::options::exact_option<hidden> {};
struct directory_option : fs::options::exact_option<dir> {};
struct symlink_option : fs::options::exact_option<link> {};
struct junction_option : fs::options::exact_option<junct> {};
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
	auto __attrs = __fs_win_file_attributes(__fs_win_file_attributes_data::__normal);
	if constexpr (_Options_::contains(hidden)) __attrs |= __fs_win_file_attributes_data::__hidden;
	return __attrs;
}

template <class _Options_>
filesystem_nodiscard constexpr filesystem_always_inline auto __fs_file_flags_from_options() noexcept {
	auto __attrs = __fs_win_file_flags(__fs_win_file_flags_data::__overlapped);
	return __attrs;
}

template <class _Options_>
struct _Configurable_open : fs::options::notifiable<_Configurable_open,
	_Options_, for_read_option, for_write_option, share_delete_option,
	share_read_option, share_write_option, always_option, directory_option>
{
private:
	static filesystem_nodiscard constexpr filesystem_always_inline auto __open_disposition() noexcept {
		if constexpr (_Options_::contains(always)) return __fs_win_file_creation_disposition::__open_always;
		else return __fs_win_file_creation_disposition::__open_existing;
	}
public:
	using on_complete_callback_type = fs::options::fetch_t<fs::options::completion_callback_key, _Options_>;
	static constexpr auto __has_callback = !options::concepts::same_as<on_complete_callback_type, fs::options::unknown_key>;

	using callable_tag_type = _Configurable_open;
	using return_type = std::conditional_t<__has_callback, void, std::pair<file, system::io_error>>;

	filesystem_always_inline return_type operator()(const path& __path) const {
		return fs::options::__dispatch_call(*this, __path);
	}

	static filesystem_always_inline return_type deferred_call(auto __options, const path& __path) {
		auto [__file, __err] = __fs_create_file(__path, __fs_access_flags_from_options<_Options_>(),
			__fs_share_flags_from_options<_Options_>(), __open_disposition(),
			__fs_file_attributes_from_options<_Options_>(), __fs_file_flags_from_options<_Options_>());

		if constexpr (__has_callback) 
			std::invoke(__options[fs::options::completion_callback_key].callback(), __err, std::move(__file));
		else 
			return { std::move(__file), __err };
	}
};

template <class _Options_>
struct _Configurable_read : fs::options::notifiable<_Configurable_read, _Options_, async_option> {
	using on_complete_callback_type = fs::options::fetch_t<fs::options::completion_callback_key, _Options_>;
	static constexpr auto __has_callback = !options::concepts::same_as<on_complete_callback_type, fs::options::unknown_key>;

	using return_type = std::conditional_t<__has_callback, void, std::pair<sizetype, system::io_error>>;
	using callable_tag_type = _Configurable_read;

	filesystem_always_inline return_type operator()(
		file& __file, mutable_buffer __buffer, sizetype __offset) const
	{
		return fs::options::__dispatch_call(*this, __file, __buffer, __offset);
	}

	static filesystem_always_inline return_type deferred_call(
		auto __options, file& __file, mutable_buffer __buffer, sizetype __offset)
	{
		auto [__size, __err] = __fs_read_file(__file, __buffer, __offset);

		if constexpr (__has_callback)
			std::invoke(__options[fs::options::completion_callback_key].callback(), __err, __size);
		else
			return { __size, __err };
	}
};

template <class _Options_>
struct _Configurable_write : fs::options::notifiable<_Configurable_write, _Options_, async_option> {
	using callable_tag_type = _Configurable_write;

	filesystem_always_inline sizetype operator()(
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
struct _Configurable_append : fs::options::notifiable<_Configurable_append, _Options_, async_option> {
	using callable_tag_type = _Configurable_append;

	filesystem_nodiscard filesystem_always_inline sizetype operator()(file& __file, const_buffer __buffer) const {
		return fs::options::__dispatch_call(*this, __file, __buffer);
	}

	static filesystem_always_inline auto deferred_call(auto __options, file& __file, const_buffer __buffer) {
		return __fs_append_file(__file, __buffer);
	}
};


template <class _Options_>
struct _Configurable_create : fs::options::notifiable<_Configurable_create, _Options_,
	for_read_option, for_write_option, share_delete_option, share_read_option, share_write_option,
	always_option, if_not_exists_option, directory_option>
{
private:
	static filesystem_nodiscard constexpr filesystem_always_inline auto __disposition() noexcept {
		if constexpr (_Options_::contains(always)) return __fs_win_file_creation_disposition::__create_always;
		else return __fs_win_file_creation_disposition::__create_new;
	}
public:
	using on_complete_callback_type = fs::options::fetch_t<fs::options::completion_callback_key, _Options_>;
	static constexpr auto __has_callback = !options::concepts::same_as<on_complete_callback_type, fs::options::unknown_key>;

	using callable_tag_type = _Configurable_create;
	using return_type = std::conditional_t<__has_callback, void, std::pair<file, system::io_error>>;

	filesystem_nodiscard filesystem_always_inline return_type operator()(const path& __path) const {
		return fs::options::__dispatch_call(*this, __path);
	}

	static filesystem_always_inline return_type deferred_call(auto __options, const path& __path) {
		auto [__file, __err] = __fs_create_file(__path, __fs_access_flags_from_options<_Options_>(),
			__fs_share_flags_from_options<_Options_>(), __disposition(),
			__fs_file_attributes_from_options<_Options_>(), __fs_file_flags_from_options<_Options_>());

		if constexpr (__has_callback)
			std::invoke(__options[fs::options::completion_callback_key].callback(), __err, std::move(__file));
		else
			return { std::move(__file), __err };
	}
};

template <class _Options_>
struct _Configurable_copy : fs::options::notifiable<_Configurable_copy, _Options_> {
	using callable_tag_type = _Configurable_copy;
};

template <class _Options_>
struct _Configurable_move : fs::options::notifiable<_Configurable_move, _Options_> {
	using callable_tag_type = _Configurable_move;
};

template <class _Options_>
struct _Configurable_rename : fs::options::notifiable<_Configurable_rename, _Options_> {
	using callable_tag_type = _Configurable_rename;
};

template <class _Options_>
struct _Configurable_remove : fs::options::notifiable<_Configurable_remove, _Options_> {
	using callable_tag_type = _Configurable_remove;

	filesystem_nodiscard filesystem_always_inline bool operator()(const path& __path) const {
		return fs::options::__dispatch_call(*this, __path);
	}

	static filesystem_always_inline auto deferred_call(auto __options, const path& __path) {
		return __fs_remove_file(__path, __fs_access_flags_from_options<_Options_>());
	}
};

template <class _Options_>
struct _Configurable_exists : fs::options::notifiable<_Configurable_exists, _Options_> {
	using callable_tag_type = _Configurable_exists;

	filesystem_nodiscard filesystem_always_inline bool operator()(const path& __path) const {
		return fs::options::__dispatch_call(*this, __path);
	}

	static filesystem_always_inline auto deferred_call(auto __options, const path& __path) {
		return __fs_file_exists(__path);
	}
};

constexpr inline auto open = fs::options::functor<_Configurable_open>;
constexpr inline auto read = fs::options::functor<_Configurable_read>;
constexpr inline auto write = fs::options::functor<_Configurable_write>;
constexpr inline auto append = fs::options::functor<_Configurable_append>;
constexpr inline auto create = fs::options::functor<_Configurable_create>;
constexpr inline auto exists = fs::options::functor<_Configurable_exists>;
constexpr inline auto copy = fs::options::functor<_Configurable_copy>;
constexpr inline auto move = fs::options::functor<_Configurable_move>;
constexpr inline auto remove = fs::options::functor<_Configurable_remove>;
constexpr inline auto rename = fs::options::functor<_Configurable_rename>;

__FILESYSTEM_IO_NAMESPACE_END

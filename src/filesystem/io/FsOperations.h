#pragma once 

#include <src/filesystem/Options.h>
#include <src/filesystem/io/FileOperationsImpl.h>
#include <src/filesystem/system/IoError.h>
#include <ranges>

__FILESYSTEM_IO_NAMESPACE_BEGIN

template <class _It_>
filesystem_always_inline auto buffer(_It_ first, _It_ last) noexcept {
	constexpr auto size = sizeof(std::iter_value_t<_It_>);

	if constexpr (std::is_const_v<std::remove_reference_t<std::iter_reference_t<_It_>>>)
		return const_buffer(std::to_address(first), static_cast<sizetype>(last - first) * size);
	else return mutable_buffer(std::to_address(first), static_cast<sizetype>(last - first) * size);
}

template <std::ranges::contiguous_range _Range_>
filesystem_always_inline auto buffer(_Range_& r) noexcept {
    if constexpr (std::is_const_v<std::remove_reference_t<_Range_>>)
        return const_buffer(std::ranges::data(r), std::ranges::size(r) * sizeof(std::ranges::range_value_t<_Range_>));
    else return mutable_buffer(std::ranges::data(r), std::ranges::size(r) * sizeof(std::ranges::range_value_t<_Range_>));
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

namespace detail {
	template <class _Options_>
	filesystem_nodiscard constexpr filesystem_always_inline auto access_flags_from_options() noexcept {
		auto result = win_file_access_flags(win_file_access_mode::read);
		if constexpr (_Options_::contains(for_write)) result |= win_file_access_mode::write;
		return result;
	}

	template <class _Options_>
	filesystem_nodiscard constexpr filesystem_always_inline auto share_flags_from_options() noexcept {
		auto result = share_mode_flags(win_share_mode::none);

		if constexpr (_Options_::contains(share_delete)) result |= win_share_mode::delete_;
		if constexpr (_Options_::contains(share_read)) result |= win_share_mode::read;
		if constexpr (_Options_::contains(share_write)) result |= win_share_mode::write;

		return result;
	}

	template <class _Options_>
	filesystem_nodiscard constexpr filesystem_always_inline auto file_attributes_from_options() noexcept {
		auto attrs = win_file_attributes(win_file_attributes_data::normal);
		if constexpr (_Options_::contains(hidden)) attrs |= win_file_attributes_data::hidden;
		return attrs;
	}

	template <class _Options_>
	filesystem_nodiscard constexpr filesystem_always_inline auto file_flags_from_options() noexcept {
		auto attrs = win_file_flags(win_file_flags_data::overlapped);
		return attrs;
	}

	template <class _Options_>
	struct configurable_open_t : fs::options::strict_elementwise_callable<configurable_open_t,
		_Options_, for_read_option, for_write_option, share_delete_option,
		share_read_option, share_write_option, always_option, directory_option>
	{
	private:
		static filesystem_nodiscard constexpr filesystem_always_inline auto open_disposition() noexcept {
			if constexpr (_Options_::contains(always)) return win_file_creation_disposition::open_always;
			else return win_file_creation_disposition::open_existing;
		}
	public:
		using callable_tag_type = configurable_open_t;

		filesystem_always_inline std::pair<file, system::io_error> operator()(const path& path) const {
			return fs::options::dispatch_call(*this, path);
		}
		
		static filesystem_always_inline std::pair<file, system::io_error> deferred_call(auto options, const path& path) {
			return create_file(path, access_flags_from_options<_Options_>(),
				share_flags_from_options<_Options_>(), open_disposition(),
				file_attributes_from_options<_Options_>(), file_flags_from_options<_Options_>());
		}
	};

	template <class _Options_>
	struct configurable_read_t : fs::options::notifiable<configurable_read_t, _Options_, async_option> {
		using on_complete_callback_type = fs::options::fetch_t<fs::options::completion_callback_key, _Options_>;
		static constexpr auto has_callback = !options::concepts::same_as<on_complete_callback_type, fs::options::unknown_key>;

		using return_type = std::conditional_t<has_callback, void, std::pair<sizetype, system::io_error>>;
		using callable_tag_type = configurable_read_t;

		template <mutable_buffer_sequence _BufferSequence_>
		filesystem_always_inline return_type operator()(
			file& file, _BufferSequence_ buffer, sizetype offset) const
		{
			return fs::options::dispatch_call(*this, file, buffer, offset);
		}

		template <mutable_buffer_sequence _BufferSequence_>
		static filesystem_always_inline return_type deferred_call(
			auto options, file& file, _BufferSequence_ buffer, sizetype offset)
		{
			auto [size, err] = read_file(file, buffer, offset);

			if constexpr (has_callback)
				std::invoke(options[fs::options::completion_callback_key].callback(), err, size);
			else
				return { size, err };
		}
	};

	template <class _Options_>
	struct configurable_write_t : fs::options::notifiable<configurable_write_t, _Options_, async_option> {
		using on_complete_callback_type = fs::options::fetch_t<fs::options::completion_callback_key, _Options_>;
		static constexpr auto has_callback = !options::concepts::same_as<on_complete_callback_type, fs::options::unknown_key>;

		using return_type = std::conditional_t<has_callback, void, std::pair<sizetype, system::io_error>>;
		using callable_tag_type = configurable_write_t;

		template <buffer_sequence _BufferSequence_>
		filesystem_always_inline return_type operator()(
			file& file, _BufferSequence_ buffer, sizetype offset) const
		{
			return fs::options::dispatch_call(*this, file, buffer, offset);
		}

		template <buffer_sequence _BufferSequence_>
		static filesystem_always_inline return_type deferred_call(
			auto options, file& file, _BufferSequence_ buffer, sizetype offset)
		{
			auto [size, err] = write_file(file, buffer, offset);

			if constexpr (has_callback)
				std::invoke(options[fs::options::completion_callback_key].callback(), err, size);
			else
				return { size, err };
		}
	};

	template <class _Options_>
	struct configurable_append_t : fs::options::notifiable<configurable_append_t, _Options_, async_option> {
		using on_complete_callback_type = fs::options::fetch_t<fs::options::completion_callback_key, _Options_>;
		static constexpr auto has_callback = !options::concepts::same_as<on_complete_callback_type, fs::options::unknown_key>;

		using return_type = std::conditional_t<has_callback, void, std::pair<sizetype, system::io_error>>;
		using callable_tag_type = configurable_append_t;

		template <buffer_sequence _BufferSequence_>
		filesystem_nodiscard filesystem_always_inline return_type operator()(file& file, _BufferSequence_ buffer) const {
			return fs::options::dispatch_call(*this, file, buffer);
		}

		template <buffer_sequence _BufferSequence_>
		static filesystem_always_inline return_type deferred_call(auto options, file& file, _BufferSequence_ buffer) {
			auto [size, err] = append_file(file, buffer);

			if constexpr (has_callback)
				std::invoke(options[fs::options::completion_callback_key].callback(), err, size);
			else
				return { size, err };
		}
	};

	template <class _Options_>
	struct configurable_create_t : fs::options::strict_elementwise_callable<configurable_create_t, _Options_,
		for_read_option, for_write_option, share_delete_option, share_read_option, share_write_option,
		always_option, if_not_exists_option, directory_option>
	{
	private:
		static filesystem_nodiscard constexpr filesystem_always_inline auto disposition() noexcept {
			if constexpr (_Options_::contains(always)) return win_file_creation_disposition::create_always;
			else if constexpr (_Options_::contains(if_not_exists)) return win_file_creation_disposition::open_always;
			else return win_file_creation_disposition::create_new;
		}
	public:
		using callable_tag_type = configurable_create_t;

		filesystem_nodiscard filesystem_always_inline std::pair<file, system::io_error> operator()(const path& path) const {
			return fs::options::dispatch_call(*this, path);
		}

		static filesystem_always_inline std::pair<file, system::io_error> deferred_call(auto options, const path& path) {
			return create_file(path, access_flags_from_options<_Options_>(),
				share_flags_from_options<_Options_>(), disposition(),
				file_attributes_from_options<_Options_>(), file_flags_from_options<_Options_>());
		}
	};

	template <class _Options_>
	struct configurable_copy_t : fs::options::strict_elementwise_callable<configurable_copy_t, _Options_> {
		using callable_tag_type = configurable_copy_t;
	};

	template <class _Options_>
	struct configurable_move_t : fs::options::strict_elementwise_callable<configurable_move_t, _Options_> {
		using callable_tag_type = configurable_move_t;


	};

	template <class _Options_>
	struct configurable_rename_t : fs::options::strict_elementwise_callable<configurable_rename_t, _Options_> {
		using callable_tag_type = configurable_rename_t;

		filesystem_nodiscard filesystem_always_inline system::io_error operator()(const path& file_path, const path& new_name) const {
			return fs::options::dispatch_call(*this, file_path, new_name);
		}

		filesystem_nodiscard filesystem_always_inline static auto deferred_call(auto options, const path& file_path, const path& new_name) {
			return rename_file(file_path, new_name);
		}
	};

	template <class _Options_>
	struct configurable_remove_t : fs::options::strict_elementwise_callable<configurable_remove_t, _Options_> {
		using callable_tag_type = configurable_remove_t;

		filesystem_nodiscard filesystem_always_inline system::io_error operator()(const path& path) const {
			return fs::options::dispatch_call(*this, path);
		}

		static filesystem_always_inline auto deferred_call(auto options, const path& path) {
			return remove_file(path);
		}
	};

	template <class _Options_>
	struct configurable_exists_t : fs::options::strict_elementwise_callable<configurable_exists_t, _Options_> {
		using callable_tag_type = configurable_exists_t;

		filesystem_nodiscard filesystem_always_inline bool operator()(const path& path) const {
			return fs::options::dispatch_call(*this, path);
		}

		static filesystem_always_inline auto deferred_call(auto options, const path& path) {
			return file_exists(path);
		}
	};
}

constexpr inline auto open = fs::options::functor<detail::configurable_open_t>;
constexpr inline auto read = fs::options::functor<detail::configurable_read_t>;
constexpr inline auto write = fs::options::functor<detail::configurable_write_t>;
constexpr inline auto append = fs::options::functor<detail::configurable_append_t>;
constexpr inline auto create = fs::options::functor<detail::configurable_create_t>;
constexpr inline auto exists = fs::options::functor<detail::configurable_exists_t>;
constexpr inline auto copy = fs::options::functor<detail::configurable_copy_t>;
constexpr inline auto move = fs::options::functor<detail::configurable_move_t>;
constexpr inline auto remove = fs::options::functor<detail::configurable_remove_t>;
constexpr inline auto rename = fs::options::functor<detail::configurable_rename_t>;

__FILESYSTEM_IO_NAMESPACE_END

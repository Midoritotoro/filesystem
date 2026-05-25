#pragma once 

#include <filesystem/compatibility/Compatibility.h>
#include <src/filesystem/Assert.h>
#include <ranges>
#include <algorithm>
#include <src/filesystem/io/PathParser.h>
#include <src/filesystem/IsCharacter.h>
#include <src/filesystem/string/StringConversions.h>
#include <string>

__FILESYSTEM_IO_NAMESPACE_BEGIN

class path {
	using traits = Path_traits;
	using parser = Path_parser;
public:
	using value_type = traits::value_type;
	using string_type = traits::string_type;

	enum format {
		native_format,
		generic_format,
		auto_format
	};

	path() noexcept {}
	path(const string_type& p) : _path(p) {}

	path(const path& p) {
		_path = p._path;
	}

	path(path&& p) noexcept {
		_path = std::move(p._path);
		p.clear();
	}

	template <str_type _Source_>
	path(const _Source_& source, format fmt = auto_format) {
		str_cvt(_path, source);
	}

	template <class _InputIterator_>
	path(_InputIterator_ first, _InputIterator_ last) {
		static_assert(is_character_v<std::iter_value_t<_InputIterator_>>, "invalid value_type");
		str_cvt(_path, first, last);
	}

	path& operator=(const path& p) {
		_path = p._path;
		return *this;
	}

	path& operator=(path&& p) {
		_path = std::move(p._path);
		p.clear();
		return *this;
	}

	path& operator=(string_type&& p) {
		_path = std::move(p);
		return *this;
	}

	void clear() noexcept {
		_path.clear();
	}

	operator string_type() const noexcept {
		return _path;
	}

	const string_type& native() const noexcept {
		return _path;
	}

	const value_type* c_str() const noexcept {
		return _path.c_str();
	}

	path& make_preferred() noexcept {
		if constexpr (windows) std::ranges::replace(_path, L'/', traits::preferred_separator);
		return *this;
	}

	path& remove_filename() noexcept {
		parser::remove_filename(_path);
		return *this;
	}

	path& replace_filename(const path& replacement) noexcept {
		remove_filename();
		return *this += replacement._path;
	}

	path& replace_extension(const path& replacement = path()) noexcept {
		parser::remove_extension(_path);
		return *this += (replacement._path[0] == traits::dot ? replacement._path : traits::dot + replacement._path);
	}

	path& operator+=(const path& p) {
		return operator+=(p._path);
	}

	path& operator+=(const string_type& str) {
		_path.append(str);
		return *this;
	}

	path& operator+=(std::basic_string_view<value_type> str) {
		return operator+=(string_type(str));
	}

	path& operator+=(const value_type* ptr) {
		_path.append(ptr);
	}

	path& operator+=(value_type x) {
		_path.push_back(x);
	}

	template <class _CharType_>
	path& operator+=(_CharType_ x) {
		return *this;
	}

	template <class _Source_>
	path& operator+=(const _Source_& source) {
		return *this;
	}

	template <class _Source_>
	path& concat(const _Source_& source) {
		return *this;
	}

	template <class _InputIterator_>
	path& concat(_InputIterator_ first, _InputIterator_ last) {
		return *this;
	}
	
	path root_name() const noexcept {
		return path(_path.data(), parser::find_root_name_end(_path.data(), _path.data() + _path.size()));
	}

	path root_directory() const noexcept {
		if (has_root_directory())
			return path(parser::find_root_name_end(_path.data(), _path.data() + _path.size()), _path.data() + _path.size());

		return path();
	}

	path root_path() const noexcept {
		return *this;
	}

	path relative_path() const noexcept {
		return *this;
	}

	path parent_path() const noexcept {
		return *this;
	}

	path filename() const noexcept {
		return *this;
	}

	path stem() const noexcept {
		return *this;
	}

	path extension() const noexcept {
		return path(parser::find_extension(_path), _path.end());
	}

	bool empty() const noexcept {
		return _path.empty();
	}

	bool has_root_name() const noexcept {
		return false;
	}

	bool has_root_directory() const noexcept {
		return false;// return parser::has_root_directory(__path);
	}

	bool has_root_path() const noexcept {
		return false;
	}

	bool has_relative_path() const noexcept {
		return false;
	}

	bool has_parent_path() const noexcept {
		return false;
	}

	bool has_filename() const noexcept {
		return false;
	}

	bool has_stem() const noexcept {
		return false;
	}

	bool has_extension() const noexcept {
		return false;
	}

	bool is_absolute() const noexcept {
		return !is_relative();
	}

	bool is_relative() const noexcept {
		return !any_separator(_path[0]) && !(_path[1] == traits::colon);
	}

	path lexically_normal() const noexcept {
		return *this;
	}

	path lexically_relative(const path& base) const noexcept {
		return *this;
	}

	path lexically_proximate(const path& base) const noexcept {
		return *this;
	}
private:
	string_type _path;
};

__FILESYSTEM_IO_NAMESPACE_END

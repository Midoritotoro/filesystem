#pragma once 

#include <filesystem/compatibility/Compatibility.h>
#include <src/filesystem/Assert.h>
#include <ranges>
#include <algorithm>
#include <src/filesystem/io/PathParser.h>
#include <string>

__FILESYSTEM_IO_NAMESPACE_BEGIN

class path {
	using _Traits = _Path_traits;
	using _Parser = _Path_parser;
public:
	using value_type = _Traits::__value_type;
	using string_type = _Traits::__string_type;

	path() noexcept {}
	path(const string_type& __path) : _path(__path) {}

	path(const path& __path) {
		_path = __path._path;
	}

	path(path&& __path) noexcept {
		_path = std::move(__path._path);
		__path.clear();
	}

	path& operator=(string_type&& __path) {
		_path = std::move(__path);
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
		if constexpr (windows) std::ranges::replace(_path, L'/', _Traits::__preferred_separator);
		return *this;
	}

	path& remove_filename() noexcept {
		_Path_parser::__remove_filename(_path);
		return *this;
	}

	path& replace_filename(const path& __replacement) noexcept {
		remove_filename();
		return *this += __replacement._path;
	}

	path& replace_extension(const path& __replacement = path()) noexcept {
		_Path_parser::__remove_extension(_path);
		return *this += (_Traits::__dot + __replacement._path);
	}

	path& operator+=(const path& __path) {
		return operator+=(__path._path);
	}

	path& operator+=(const string_type& __str) {
		_path.append(__str);
		return *this;
	}

	path& operator+=(std::basic_string_view<value_type> __str) {
		return operator+=(string_type(__str));
	}

	path& operator+=(const value_type* ptr) {
		_path.append(ptr);
	}

	path& operator+=(value_type x) {
		_path.push_back(x);
	}

	template <class _CharType_>
	path& operator+=(_CharType_ __x) {
		return *this;
	}

	template <class _Source_>
	path& operator+=(const _Source_& __source) {
		return *this;
	}

	template <class _Source_>
	path& concat(const _Source_& __source) {
		return *this;
	}

	template <class _InputIterator_>
	path& concat(_InputIterator_ __first, _InputIterator_ __last) {
		return *this;
	}
	
	path root_name() const noexcept {
		return *this;
	}

	path root_directory() const noexcept {
		return *this;
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
		return *this;
	}

	bool empty() const noexcept {
		return _path.empty();
	}

	//bool has_root_name() const noexcept {

	//}

	//bool has_root_directory() const noexcept {

	//}

	//bool has_root_path() const noexcept {

	//}

	//bool has_relative_path() const noexcept {

	//}

	//bool has_parent_path() const noexcept {

	//}

	//bool has_filename() const noexcept {

	//}

	//bool has_stem() const noexcept {
	//
	//}

	//bool has_extension() const noexcept {

	//}

	//bool is_absolute() const noexcept {

	//}

	//bool is_relative() const noexcept {
	//	
	//}

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

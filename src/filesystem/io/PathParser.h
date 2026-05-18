#pragma once 

#include <src/filesystem/io/PathTraits.h>
#include <ranges>
#include <string>


__FILESYSTEM_IO_NAMESPACE_BEGIN

struct {
	filesystem_static_operator bool operator()(_Path_traits::__value_type __c) filesystem_const_operator noexcept {
		if constexpr (windows) return __c == _Path_traits::__preferred_separator || __c == _Path_traits::__slash;
		else return __c == _Path_traits::__slash;
	}
} __any_separator;

class _Path_parser {
	using _Traits = _Path_traits;

	using __str_type = _Traits::__string_type;
	using __value_type = _Traits::__value_type;
public:
	static void __remove_filename(__str_type& __path) {
		const auto __filename = __find_filename(__path);
		(__filename == __path.end()) ? __path.clear() : __path.resize((__filename + 1) - __path.begin());
	}
	
	static void __remove_extension(__str_type& __path) {
		filesystem_debug_assert(!__path.empty());
		__path.erase(__find_extension(__path), __path.end());
	}

	static __str_type::const_iterator __find_extension(const __str_type& __path) {
		const auto __filename = __find_filename(__path);
		return std::ranges::find(__filename, __path.end(), _Traits::__dot);
	}

	static __str_type::const_iterator __find_filename(const __str_type& __path) {
		const auto __found = std::ranges::begin(std::ranges::find_last_if(__path, __any_separator));
		return __found == __path.end() ? __path.begin() : __found;
	}
};

__FILESYSTEM_IO_NAMESPACE_END

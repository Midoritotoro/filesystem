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
		(__filename.begin() == __path.end()) ? __path.clear() : __path.resize((__filename.begin() + 1) - __path.begin());
	}
	
	static void __remove_extension(__str_type& __path) {
		if (__path.empty()) return;
		 
		const auto __filename = __find_filename(__path);
		const auto __extension = std::ranges::find(__filename.begin(), __filename.end(), _Traits::__dot);
		\

	}

	static auto __find_filename(const __str_type& __path) {
		return std::ranges::find_last_if(__path, __any_separator);
	}
};

__FILESYSTEM_IO_NAMESPACE_END

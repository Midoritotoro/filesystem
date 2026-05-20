#pragma once 

#include <src/filesystem/io/PathTraits.h>
#include <ranges>
#include <string>
#include <string_view>

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
    using view_type = std::basic_string_view<__value_type, std::char_traits<__value_type>>;

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

    static bool __is_drive_prefix(const __value_type* __first) {
        u32 __v;
        std::memcpy(&__v, __first, sizeof(__v));

        __v &= 0xFFFF'FFDFu;
        __v -= (static_cast<unsigned int>(L':') << (sizeof(wchar_t) * CHAR_BIT)) | L'A';

        return __v < 26;
    }

    static bool __has_drive_letter_prefix(const __value_type* __first, const __value_type* __last) {
        return __last - __first >= 2 && __is_drive_prefix(__first);
    }

    static const __value_type* __find_root_name_end(const __value_type* __first, const __value_type* __last) {
        if (__last - __first < 2) return __first;
        if (__has_drive_letter_prefix(__first, __last)) return __first + 2;
        if (!__any_separator(__first[0])) return __first;

        if (__last - __first >= 4 && __any_separator(__first[3]) && (__last - __first == 4 || !__any_separator(__first[4]))
            && ((__any_separator(__first[1]) && (__first[2] == L'?' || __first[2] == L'.')) || (__first[1] == L'?' && __first[2] == L'?')))
            return __first + 3;

        if (__last - __first >= 3 && __any_separator(__first[1]) && !__any_separator(__first[2])) 
            return std::find_if(__first + 3, __last, __any_separator);

        return __first;
    }

    static const __value_type* __find_ext(const __value_type* __fname, const __value_type* __ads) {
        auto __ext = __ads;

        if (__fname == __ext) return __ads;
        if (__fname == --__ext) return __ads;

        if (*__ext == L'.') {
            if (__fname == __ext - 1 && __ext[-1] == L'.') return __ads;
            else return __ext;
        }

        while (__fname != --__ext) if (*__ext == L'.') return __ext;
        return __ads;
    }
};

__FILESYSTEM_IO_NAMESPACE_END

#pragma once 

#include <src/filesystem/io/PathTraits.h>
#include <src/filesystem/Assert.h>
#include <ranges>
#include <string>
#include <string_view>

__FILESYSTEM_IO_NAMESPACE_BEGIN

struct {
	filesystem_static_operator bool operator()(Path_traits::value_type c) filesystem_const_operator noexcept {
		if constexpr (windows) return c == Path_traits::preferred_separator || c == Path_traits::slash;
		else return c == Path_traits::slash;
	}
} any_separator;

class Path_parser {
	using traits = Path_traits;

	using str_type = traits::string_type;
	using value_type = traits::value_type;
public:
    using view_type = std::basic_string_view<value_type, std::char_traits<value_type>>;

	static void remove_filename(str_type& path) {
		const auto filename = find_filename(path);
		(filename == path.end()) ? path.clear() : path.resize((filename + 1) - path.begin());
	}
	
	static void remove_extension(str_type& path) {
		filesystem_debug_assert(!path.empty());
		path.erase(find_extension(path), path.end());
	}

	static str_type::const_iterator find_extension(const str_type& path) {
		const auto filename = find_filename(path);
		return std::ranges::find(filename, path.end(), traits::dot);
	}

	static str_type::const_iterator find_filename(const str_type& path) {
		const auto found = std::ranges::begin(std::ranges::find_last_if(path, any_separator));
		return found == path.end() ? path.begin() : found;
	}

    static bool is_drive_prefix(const value_type* first) {
        u32 v;
        std::memcpy(&v, first, sizeof(v));

        v &= 0xFFFF'FFDFu;
        v -= (static_cast<unsigned int>(L':') << (sizeof(wchar_t) * CHAR_BIT)) | L'A';

        return v < 26;
    }

    static bool has_drive_letter_prefix(const value_type* first, const value_type* last) {
        return last - first >= 2 && is_drive_prefix(first);
    }

    static const value_type* find_root_name_end(const value_type* first, const value_type* last) {
        if (last - first < 2) return first;
        if (has_drive_letter_prefix(first, last)) return first + 2;
        if (!any_separator(first[0])) return first;

        if (last - first >= 4 && any_separator(first[3]) && (last - first == 4 || !any_separator(first[4]))
            && ((any_separator(first[1]) && (first[2] == L'?' || first[2] == L'.')) || (first[1] == L'?' && first[2] == L'?')))
            return first + 3;

        if (last - first >= 3 && any_separator(first[1]) && !any_separator(first[2])) 
            return std::find_if(first + 3, last, any_separator);

        return first;
    }

    static const value_type* find_ext(const value_type* fname, const value_type* ads) {
        auto ext = ads;

        if (fname == ext) return ads;
        if (fname == --ext) return ads;

        if (*ext == L'.') {
            if (fname == ext - 1 && ext[-1] == L'.') return ads;
            else return ext;
        }

        while (fname != --ext) if (*ext == L'.') return ext;
        return ads;
    }
};

__FILESYSTEM_IO_NAMESPACE_END

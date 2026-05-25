#pragma once

#include <filesystem/compatibility/Compatibility.h>
#include <Windows.h>

__FILESYSTEM_SYSTEM_NAMESPACE_BEGIN

enum class code_page : u32 {
	acp = 0,
	utf8 = 65001,
	utf7 = 65000
};

inline code_page current_thread_code_page() noexcept {
	return code_page(___lc_codepage_func());
}

__FILESYSTEM_SYSTEM_NAMESPACE_END

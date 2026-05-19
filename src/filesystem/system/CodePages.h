#pragma once

#include <filesystem/compatibility/Compatibility.h>
#include <Windows.h>

__FILESYSTEM_SYSTEM_NAMESPACE_BEGIN

enum class __code_page : u32 {
	__acp = 0,
	__utf8 = 65001,
	__utf7 = 65000
};

__code_page __current_thread_code_page() noexcept {
	return __code_page(___lc_codepage_func());
}

__FILESYSTEM_SYSTEM_NAMESPACE_END

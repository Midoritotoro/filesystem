#pragma once  

#include <filesystem/FilesystemNamespace.h>

#include <filesystem/compatibility/Warnings.h>
#include <filesystem/compatibility/BranchPrediction.h>

#include <cstdlib>
#include <cassert>

#include <iostream>


__FILESYSTEM_NAMESPACE_BEGIN


filesystem_disable_warning_msvc(6011);

struct static_locale {
	static_locale() noexcept { setlocale(LC_ALL, ""); }
};

static const static_locale lc;

inline void fail(const char* message, const char* file, int line) noexcept {
	printf("Error: %s in File \"%s\", Line: %d\n", message, file, line);

	volatile auto nullptr_value = (int*)nullptr;
	*nullptr_value = 0;
	
	std::abort();
	std::terminate();
}

inline const char* extract_basename(const char* path, size_t size) noexcept {
	while (size != 0 && path[size - 1] != '/' && path[size - 1] != '\\')
		--size;

	return path + size;
}

#define return_on_failure(message, file, line, return_value) \
	do { \
		printf("Error: %s in File \"%s\", Line: %d\n", message, file, line); \
		return return_value; \
	} while (0)
	

#define assert_validation_condition(condition, message, file, line)\
	((filesystem_unlikely(!((condition))))\
		? fs::fail(message, file, line) : void(0))

#define assert_validation_condition_with_ret(condition, message, file, line, return_value)\
	if ((filesystem_unlikely(!(condition)))) return_on_failure(message, file, line, return_value)

#define source_file_basename (fs::extract_basename(__FILE__, sizeof(__FILE__)))

#define filesystem_assert_log(condition, message) (assert_validation_condition(\
	condition, message, source_file_basename, __LINE__))

#define filesystem_assert_return(condition, message, return_value) assert_validation_condition_with_ret(\
	condition, message, source_file_basename, __LINE__, return_value)

#define filesystem_assert(condition) filesystem_assert_log((condition), "\"" #condition "\"")
#define filesystem_assert_unreachable() filesystem_assert(false)

#if !defined(NDEBUG)

#define filesystem_debug_assert_return	filesystem_assert_return
#define filesystem_debug_assert			filesystem_assert

#define filesystem_debug_assert_log		filesystem_assert_log

#else 

#define filesystem_debug_assert_return(condition, message, return_value)
#define filesystem_debug_assert(condition)

#define filesystem_debug_assert_log(condition, message)

#endif // !defined(NDEBUG)

__FILESYSTEM_NAMESPACE_END
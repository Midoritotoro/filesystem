#pragma once 

#include <filesystem/Types.h>
#include <filesystem/compatibility/CompilerDetection.h>
#include <filesystem/compatibility/AlignmentMacros.h>
#include <filesystem/compatibility/BranchPrediction.h>
#include <filesystem/compatibility/CallingConventions.h>
#include <filesystem/compatibility/Inline.h>
#include <filesystem/compatibility/CxxVersionDetection.h>
#include <filesystem/compatibility/FunctionAttributes.h>
#include <filesystem/compatibility/LanguageFeatures.h>
#include <filesystem/compatibility/MemoryMacros.h>
#include <filesystem/compatibility/Nodiscard.h>
#include <filesystem/compatibility/SystemDetection.h>
#include <filesystem/compatibility/UnreachableCode.h>
#include <filesystem/compatibility/Warnings.h>
#include <filesystem/compatibility/StaticOperators.h>
#include <filesystem/compatibility/StackProtection.h>
#include <filesystem/compatibility/ProcessorDetection.h>
#include <cstddef>

filesystem_disable_warning_msvc(4067)

__FILESYSTEM_NAMESPACE_BEGIN

#if defined(filesystem_os_windows)
  constexpr inline auto windows = true;
#else
  constexpr inline auto windows = false;
#endif

#if defined(filesystem_os_win64)
  constexpr inline auto windows_x86_64 = true;
#else
  constexpr inline auto windows_x86_64 = false;
#endif

#if defined(filesystem_os_linux)
  constexpr inline auto linux = true;
#else
  constexpr inline auto linux = false;
#endif

__FILESYSTEM_NAMESPACE_END

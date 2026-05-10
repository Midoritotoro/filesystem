#pragma once 

// Clang attributes
// https://clang.llvm.org/docs/AttributeReference.html#always-inline-force-inline
// Clang builtins
// https://clang.llvm.org/docs/LanguageExtensions.html

// Msvc attributes
// https://learn.microsoft.com/en-us/cpp/cpp/declspec?view=msvc-170
// Msvc SAL
// https://learn.microsoft.com/en-us/cpp/code-quality/using-sal-annotations-to-reduce-c-cpp-code-defects?view=msvc-170

// Gcc attributes
// https://ohse.de/uwe/articles/gcc-attributes.html and https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html

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
#include <filesystem/compatibility/SimdCompatibility.h>
#include <filesystem/compatibility/SystemDetection.h>
#include <filesystem/compatibility/UnreachableCode.h>
#include <filesystem/compatibility/Warnings.h>
#include <filesystem/compatibility/StaticOperators.h>
#include <filesystem/compatibility/StackProtection.h>
#include <filesystem/compatibility/ProcessorDetection.h>
#include <cstddef>

filesystem_disable_warning_msvc(4067)

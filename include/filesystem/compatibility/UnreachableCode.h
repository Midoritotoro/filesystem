#pragma once 

#include <filesystem/compatibility/CompilerDetection.h>


#if !defined(filesystem_maybe_unused_attribute)
#  if defined(__has_cpp_attribute) && __has_cpp_attribute(maybe_unused) >= 201603L
#    define filesystem_maybe_unused_attribute     [[maybe_unused]]
#  elif defined(filesystem_cpp_gnu) || defined(filesystem_cpp_clang)
#    define filesystem_maybe_unused_attribute     __attribute__(unused)
#  else
#    define filesystem_maybe_unused_attribute		
#  endif // defined(__has_cpp_attribute) && __has_cpp_attribute(maybe_unused) >= 201603L
#endif // !defined(filesystem_maybe_unused_attribute)


#if !defined(filesystem_unused)
#  define filesystem_unused(variable) ((void)(variable))
#endif // !defined(filesystem_unused)

#if !defined(filesystem_unreachable)
#  if defined(filesystem_cpp_gnu) || defined(filesystem_cpp_clang)
#    define filesystem_unreachable() __builtin_unreachable()
#  elif defined(filesystem_cpp_msvc)
#    define filesystem_unreachable() (__assume(0))
#  else
#    define filesystem_unreachable() ((void)0)
#  endif // defined(filesystem_cpp_gnu) || defined(filesystem_cpp_clang) || defined(filesystem_cpp_msvc)
#endif // !defined(filesystem_unreachable)


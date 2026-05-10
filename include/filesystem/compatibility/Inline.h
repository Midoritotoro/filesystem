#pragma once 

#include <filesystem/compatibility/CxxVersionDetection.h>
#include <filesystem/compatibility/CompilerDetection.h>


#if !defined(filesystem_never_inline)
#  if defined(filesystem_cpp_msvc) || defined(filesystem_cpp_clang)
#     define filesystem_never_inline __declspec(noinline)
#  elif defined(filesystem_cpp_gnu) 
#    define filesystem_never_inline __attribute__((noinline))
#  else 
#    define filesystem_never_inline 
#  endif // defined(filesystem_cpp_msvc) || defined(filesystem_cpp_clang) || defined(filesystem_cpp_gnu)
#endif // !defined(filesystem_never_inline)


#if !defined(filesystem_always_inline)
#  if defined(filesystem_cpp_msvc)
#    define filesystem_always_inline __forceinline
#  elif defined(filesystem_cpp_gnu) || defined(filesystem_cpp_clang)
#    define filesystem_always_inline inline __attribute__((__always_inline__))
#  else 
#    define filesystem_always_inline inline
#  endif // defined(filesystem_cpp_msvc) || defined(filesystem_cpp_clang) || defined(filesystem_cpp_gnu)
#endif // !defined(filesystem_always_inline)


#if !defined(filesystem_always_inline_lambda)
#  if defined(filesystem_cpp_msvc)
#    define filesystem_always_inline_lambda [[msvc::forceinline]]
#  elif defined(filesystem_cpp_gnu) || defined(filesystem_cpp_clang)
#    define filesystem_always_inline_lambda __attribute__((__always_inline__))
#  else 
#    define filesystem_always_inline_lambda
#  endif // defined(filesystem_cpp_msvc) || defined(filesystem_cpp_clang) || defined(filesystem_cpp_gnu)
#endif // !defined(filesystem_always_inline_lambda)


#if !defined(filesystem_constexpr_cxx20)
#  if filesystem_has_cxx20
#    define filesystem_constexpr_cxx20 constexpr
#  else
#    define filesystem_constexpr_cxx20
#  endif // filesystem_has_cxx20
#endif // !defined(filesystem_constexpr_cxx20)


#if !defined(filesystem_clang_constexpr_cxx20)
#  if defined(filesystem_cpp_clang)
#    define filesystem_clang_constexpr_cxx20 filesystem_constexpr_cxx20
#  else
#    define filesystem_clang_constexpr_cxx20 
#  endif // defined(filesystem_cpp_clang)
#endif // !defined(filesystem_clang_constexpr_cxx20)
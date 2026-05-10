#pragma once

#include <filesystem/compatibility/CompilerDetection.h>


#if defined(filesystem_cpp_msvc) || defined(filesystem_cpp_clang)
#  include <stdnoreturn.h> 
#endif // defined(filesystem_cpp_msvc) || defined(filesystem_cpp_clang)


#if defined(__cpp_conditional_explicit)
#  if !defined(filesystem_implicit)
#    define filesystem_implicit explicit(false)
#  else
#    define filesystem_implicit
#  endif // filesystem_implicit
#endif // defined(__cpp_conditional_explicit)


#if !defined(filesystem_noreturn)
#  if defined(filesystem_cpp_gnu)
#    define filesystem_noreturn           __attribute__((__noreturn__))
#  elif defined(filesystem_cpp_msvc) || defined(filesystem_cpp_clang)
#    define filesystem_noreturn           __declspec(noreturn)
#  else 
#    define filesystem_noreturn       
#  endif // defined(filesystem_cpp_gnu) || defined(filesystem_cpp_msvc) || defined(filesystem_cpp_clang)
#endif // !defined(filesystem_noreturn)


#if !defined(filesystem_declare_pure_function)
#  if defined(filesystem_cpp_gnu) || defined(filesystem_cpp_clang)
#    define filesystem_declare_pure_function __attribute__((pure))
#  else
#    define filesystem_declare_pure_function
#  endif // defined(filesystem_cpp_gnu) || defined(filesystem_cpp_clang)
#endif // !defined(filesystem_declare_pure_function)


#if !defined(filesystem_declare_const_function)
#  if defined(filesystem_cpp_msvc)
#    define filesystem_declare_const_function __declspec(noalias)
#  elif defined(filesystem_cpp_gnu) || defined(filesystem_cpp_clang)
#    define filesystem_declare_const_function __attribute__((const))
#  else
#    define filesystem_declare_const_function
#  endif // defined(filesystem_cpp_gnu) || defined(filesystem_cpp_clang) || defined(filesystem_cpp_msvc)
#endif // !defined(filesystem_declare_const_function)


#if !defined(filesystem_declare_cold_function)
#  if defined(filesystem_cpp_gnu) || defined(filesystem_cpp_clang)
#    define filesystem_declare_cold_function __attribute__((cold))
#  else 
#    define filesystem_declare_cold_function
#  endif // defined(filesystem_cpp_gnu) || defined(filesystem_cpp_clang)
#endif // !defined(filesystem_declare_cold_function)

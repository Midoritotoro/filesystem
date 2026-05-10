#pragma once 

#include <filesystem/compatibility/CompilerDetection.h>

#if (defined(filesystem_cpp_clang) && filesystem_cpp_clang < 1600) || defined(filesystem_cpp_gnu)

#  if !defined(filesystem_static_operator)
#    define filesystem_static_operator
#  endif // !defined(filesystem_static_operator)

#  if !defined(filesystem_const_operator)
#    define filesystem_const_operator const
#  endif // !defined(filesystem_const_operator)

#  if !defined(filesystem_static_labmda)
#    define filesystem_static_labmda
#  endif // !defined(filesystem_static_labmda)

#else

#  if !defined(filesystem_static_operator)
#    define filesystem_static_operator static
#  endif // !defined(filesystem_static_operator)

#  if !defined(filesystem_const_operator)
#    define filesystem_const_operator
#  endif // !defined(filesystem_const_operator)

#  if !defined(filesystem_static_labmda)
#    define filesystem_static_labmda static
#  endif // !defined(filesystem_static_labmda)

#endif // (defined(filesystem_cpp_clang) && filesystem_cpp_clang < 1600) || defined(filesystem_cpp_gnu)
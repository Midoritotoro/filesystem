#pragma once 

#include <filesystem/compatibility/CompilerDetection.h>
#include <filesystem/compatibility/Nodiscard.h>


#if !defined(filesystem_restrict)
#  if defined(filesystem_cpp_msvc) || defined (filesystem_cpp_clang)
#    define filesystem_restrict   __declspec(restrict)
#  elif defined(filesystem_cpp_gnu)
#    define filesystem_restrict   __restrict  
#  endif // defined(filesystem_cpp_msvc) || defined (filesystem_cpp_clang) || defined(filesystem_cpp_gnu)
#endif // !defined(filesystem_restrict)

#if !defined(filesystem_sizeof_in_bits)
#  define filesystem_sizeof_in_bits(type) (sizeof(type) * 8)
#endif // !defined(filesystem_sizeof_in_bits)
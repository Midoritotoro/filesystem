#pragma once 

#include <filesystem/compatibility/CompilerDetection.h>
#include <filesystem/arch/ProcessorDetection.h>


#if !defined(filesystem_unaligned)
#  if defined(filesystem_processor_x86_64) || defined(filesystem_processor_arm) || (filesystem_processor_arm == 8) // x64 ARM
#    if defined(filesystem_os_windows) && defined(filesystem_cpp_msvc)
#      define filesystem_unaligned __unaligned
#    else
#      define filesystem_unaligned
#    endif // defined(filesystem_os_windows) && defined(filesystem_cpp_msvc)
#  else 
#    define filesystem_unaligned
#  endif // defined(filesystem_processor_x86_64) || defined(filesystem_processor_arm) || (filesystem_processor_arm == 8)
#endif // !defined(filesystem_unaligned)


#if !defined(filesystem_aligned_type)
#  if defined(filesystem_cpp_gnu)
#    define filesystem_aligned_type(alignment) __attribute__((aligned(alignment)))
#  elif defined(filesystem_cpp_msvc) || defined(filesystem_cpp_clang) 
#    define filesystem_aligned_type(alignment) __declspec(align(alignment))
#  elif filesystem_has_cxx11
#    define filesystem_aligned_type(alignment) alignas(alignment)
#  endif // defined(filesystem_cpp_gnu) || defined(filesystem_cpp_msvc) || defined(filesystem_cpp_clang) || filesystem_has_cxx11
#endif // !defined(filesystem_aligned_type)
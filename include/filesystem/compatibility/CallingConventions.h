#pragma once 

#include <filesystem/compatibility/CompilerDetection.h>
#include <filesystem/compatibility/ProcessorDetection.h>

#if !defined(filesystem_fastcall)
#  if defined(filesystem_processor_x86_32)
#    if defined(filesystem_cpp_gnu) || defined(filesystem_cpp_clang)
#      define filesystem_fastcall __attribute__((regparm(3)))
#    elif defined(filesystem_cpp_msvc)
#      define filesystem_fastcall __fastcall
#    else
#      define filesystem_fastcall
#    endif // defined(filesystem_cpp_gnu) || defined(filesystem_cpp_msvc) || defined(filesystem_cpp_clang)
#  else
#    define filesystem_fastcall
#  endif // defined(filesystem_processor_x86_32)
#endif // !defined(filesystem_fastcall)


#if !defined(filesystem_stdcall)
#  if defined(filesystem_cpp_msvc) || defined(filesystem_cpp_clang)
#    define filesystem_stdcall            __stdcall
#  elif defined(filesystem_cpp_gnu)
#    define filesystem_stdcall            __attribute__((__stdcall__))
#  else
#    define filesystem_stdcall        
#  endif // defined(filesystem_cpp_msvc) || defined(filesystem_cpp_clang) || defined(filesystem_cpp_gnu)
#endif // !defined(filesystem_stdcall)


#if !defined(filesystem_cdecl)
#  if defined(filesystem_cpp_msvc) || defined(filesystem_cpp_clang)
#    define filesystem_cdecl          __cdecl
#  elif defined(filesystem_cpp_gnu)
#    define filesystem_cdecl          __attribute__((__cdecl__))
#  else
#    define filesystem_cdecl        
#  endif // defined(filesystem_cpp_msvc) || defined(filesystem_cpp_clang) || defined(filesystem_cpp_gnu)
#endif // !defined(filesystem_cdecl)

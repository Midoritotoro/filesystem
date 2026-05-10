#pragma once

#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
#  define filesystem_os_apple
#  if defined(TARGET_OS_MAC) && TARGET_OS_MAC
#    define filesystem_os_darwin
#    define filesystem_os_bsd4
#    if defined(OS_IPHONE) && TARGET_OS_IPHONE
#    else
#      define filesystem_os_mac
#    endif
#  endif
#elif defined(__CYGWIN__)
#  define filesystem_os_cygwin
#elif !defined(SAG_COM) && (!defined(WINAPI_FAMILY) || WINAPI_FAMILY==WINAPI_FAMILY_DESKTOP_APP) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#  define filesystem_os_win32
#  define filesystem_os_win64
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#  define filesystem_os_win32
#elif defined(__linux__) || defined(__linux)
#  define filesystem_os_linux

#elif defined(__Lynx__)
#  define filesystem_os_lynx

#elif defined(__GNU__)
#  define filesystem_os_hurd

#elif defined(__FreeBSD__)
#  define filesystem_os_freebsd

#elif defined(__NetBSD__)
#  define filesystem_os_netbsd

#elif defined(__OpenBSD__)
#  define filesystem_os_openbsd

#elif defined(__DragonFly__)
#  define filesystem_os_dragonfly

#elif defined(__linux__)
#  define filesystem_os_linux

#elif defined(__native_client__)
#  define filesystem_os_nacl

#elif defined(__EMSCRIPTEN__)
#  define filesystem_os_emscripten

#elif defined(__rtems__)
#  define filesystem_os_rtems

#elif defined(__Fuchsia__)
#  define filesystem_os_fuchsia

#elif defined (__SVR4) && defined (__sun)
#  define filesystem_os_solaris

#elif defined(__QNX__)
#  define filesystem_os_qnx

#elif defined(__MVS__)
#  define filesystem_os_zos

#elif defined(__hexagon__)
#  define filesystem_os_qurt

#else
#  error ""
#endif

#if defined(filesystem_os_win32) || defined(filesystem_os_win64)
#  define filesystem_os_windows
#  define filesystem_os_win
#endif

#if defined(filesystem_os_windows)
#  undef filesystem_os_unix
#elif !defined(filesystem_os_unix)
#  define filesystem_os_unix
#endif

#if defined(filesystem_os_win)
#  define NOMINMAX
#endif // defined(filesystem_os_win)

#if defined(filesystem_os_windows)
#  include <windows.h>
#  include <stdnoreturn.h>
#endif // defined(filesystem_os_windows)

#if defined(max) 
#  undef max
#endif

#if defined(min) 
#  undef min
#endif
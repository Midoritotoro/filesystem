#pragma once 

#if defined(__cplusplus)
    #if defined(_MSVC_LANG) && _MSVC_LANG > __cplusplus
        #define LANGUAGE_VERSION _MSVC_LANG
    #else
        #define LANGUAGE_VERSION __cplusplus
    #endif
#else
    #define LANGUAGE_VERSION 0L
#endif


#if !defined(filesystem_has_cxx11)
#  if LANGUAGE_VERSION >= 201103L
#    define filesystem_has_cxx11 1
#  else
#    define filesystem_has_cxx11 0
#  endif // LANGUAGE_VERSION >= 201103L
#endif // !defined(filesystem_has_cxx11)


#if !defined(filesystem_has_cxx14)
#  if LANGUAGE_VERSION >= 201402L
#    define filesystem_has_cxx14 1
#  else
#    define filesystem_has_cxx14 0
#  endif // LANGUAGE_VERSION >= 201402L
#endif // !defined(filesystem_has_cxx14)


#if !defined(filesystem_has_cxx17)
#  if LANGUAGE_VERSION >= 201703L
#    define filesystem_has_cxx17 1
#  else
#    define filesystem_has_cxx17 0
#  endif // LANGUAGE_VERSION >= 201703L
#endif // !defined(filesystem_has_cxx17)


#if !defined(filesystem_has_cxx20)
#  if filesystem_has_cxx17 && LANGUAGE_VERSION >= 202002L
#    define filesystem_has_cxx20 1
#  else
#    define filesystem_has_cxx20 0
#  endif // filesystem_has_cxx17 && LANGUAGE_VERSION >= 202002L
#endif // !defined(filesystem_has_cxx20)


#if !defined(filesystem_has_cxx23)
#  if filesystem_has_cxx20 && LANGUAGE_VERSION > 202002L
#    define filesystem_has_cxx23 1
#  else
#    define filesystem_has_cxx23 0
#  endif // filesystem_has_cxx20 && LANGUAGE_VERSION > 202002L
#endif // !defined(filesystem_has_cxx23)
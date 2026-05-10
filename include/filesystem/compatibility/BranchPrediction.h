#pragma once 

#include <filesystem/compatibility/CompilerDetection.h>
#include <filesystem/compatibility/LanguageFeatures.h>


#if !defined(filesystem_likely)
#  if defined(filesystem_cpp_gnu) || defined(filesystem_cpp_clang)
#    define filesystem_likely(expression) __builtin_expect(!!(expression), true)
#  elif defined(filesystem_cpp_msvc) && defined(__has_cpp_attribute) && __has_cpp_attribute(likely) >= 201803L
#    define filesystem_likely(expression)                     \
       (                                                \
         ([](bool value){                               \
           switch (value) {                             \
             [[unlikely]] case true: return true;       \
             [[likely]] case false: return false;       \
         }                                              \
       })(expression))
#  else
#    define filesystem_likely(expression) (!!(expression))
#  endif // defined(filesystem_cpp_gnu) || defined(filesystem_cpp_clang)
#endif // !defined(filesystem_likely)


#if !defined(filesystem_unlikely)
#  if defined(filesystem_cpp_gnu) || defined(filesystem_cpp_clang)
#    define filesystem_unlikely(expression) __builtin_expect(!!(expression), false)
#  elif defined(filesystem_cpp_msvc) && defined(__has_cpp_attribute) && __has_cpp_attribute(unlikely) >= 201803L
#    define filesystem_unlikely(expression)                   \
       (                                                \
         ([](bool value){                               \
           switch (value) {                             \
             [[likely]] case true: return true;         \
             [[unlikely]] case false: return false;     \
         }                                              \
       })(expression))
#  else
#    define filesystem_unlikely(expression) (!!(expression))
#  endif // defined(filesystem_cpp_gnu) || defined(filesystem_cpp_clang)
#endif // !defined(filesystem_unlikely)


#if !defined(filesystem_likely_attribute)
#  if defined(__has_cpp_attribute) && __has_cpp_attribute(likely) >= 201803L
#    define filesystem_likely_attribute [[likely]]
#  else
#    define filesystem_likely_attribute
#  endif
#endif // !defined(filesystem_likely_attribute)


#if !defined(filesystem_unlikely_attribute)
#  if defined(__has_cpp_attribute) && __has_cpp_attribute(unlikely) >= 201803L
#    define filesystem_unlikely_attribute [[unlikely]]
#  else
#    define filesystem_unlikely_attribute
#  endif
#endif // !defined(filesystem_unlikely_attribute)

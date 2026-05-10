#pragma once 

#include <filesystem/compatibility/CompilerDetection.h>
#include <filesystem/compatibility/LanguageFeatures.h>


#if !defined(filesystem_has_nodiscard)
#  if !defined(__has_cpp_attribute)
#    define filesystem_has_nodiscard 0
#  elif __has_cpp_attribute(nodiscard) >= 201603L
#    define filesystem_has_nodiscard 1
#  else
#    define filesystem_has_nodiscard 0
#  endif
#endif // !defined(filesystem_has_nodiscard)


#if !defined(filesystem_nodiscard)
#  if filesystem_has_nodiscard
#    define filesystem_nodiscard  [[nodiscard]]
#  elif defined(filesystem_cpp_gnu)
#    define filesystem_nodiscard  __attribute__((__warn_unused_result__))
#  elif defined(filesystem_cpp_clang)
#    define filesystem_nodiscard  __attribute__(warn_unused_result)
#  elif defined(filesystem_cpp_msvc)
#    define filesystem_nodiscard  _Check_return_
#  else
#    define filesystem_nodiscard
#  endif // filesystem_has_nodiscard || defined(filesystem_cpp_gnu) || defined(filesystem_cpp_clang) || defined(filesystem_cpp_msvc)
#endif // !defined(filesystem_nodiscard)


#if !defined(filesystem_nodiscard_with_warning)
#  if defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L
#    define filesystem_nodiscard_with_warning(message)    [[nodiscard(message)]]
#  elif defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201603L
#    define filesystem_nodiscard_with_warning(message)    filesystem_nodiscard
#  else
#    define filesystem_nodiscard_with_warning(message)
#  endif //     defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L 
//  || defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201603L
#endif // !defined(filesystem_nodiscard_with_warning)


#if !defined(filesystem_nodiscard_constructor)
// https://open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1771r1.pdf
#  if defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L
#    define filesystem_nodiscard_constructor filesystem_nodiscard
#  else
#    define filesystem_nodiscard_constructor
#  endif // defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L
#endif // !defined(filesystem_nodiscard_constructor)


#if !defined(filesystem_nodiscard_constructor_with_warning)
#  if defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L
#    define filesystem_nodiscard_constructor_with_warning(message) filesystem_nodiscard_with_warning(message)
#  else
#    define filesystem_nodiscard_constructor_with_warning(message) 
#  endif // defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L
#endif // !defined(filesystem_nodiscard_constructor_with_warning)
#pragma once 

#include <filesystem/compatibility/Nodiscard.h>

// Warnings

# if !defined(filesystem_do_pragma)
#  if defined(filesystem_cpp_clang) || defined(filesystem_cpp_gnu)
#    define filesystem_do_pragma(__text) _Pragma(#__text)
#  elif defined(filesystem_cpp_msvc)
#    define filesystem_do_pragma(__text) __pragma(#__text)
#  else
#    define filesystem_do_pragma(__text)
#  endif
#endif // !defined(filesystem_do_pragma)

#if !defined(filesystem_warning_push)
#  if defined(filesystem_cpp_msvc) && !defined(filesystem_cpp_clang)
#    define filesystem_warning_push __pragma(warning(push))
#  elif defined(filesystem_cpp_clang)
#    define filesystem_warning_push filesystem_do_pragma(clang diagnostic push)
#  elif defined(filesystem_cpp_gnu) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#    define filesystem_warning_push filesystem_do_pragma(GCC diagnostic push)
#  else 
#    define filesystem_warning_push
#  endif // (defined(filesystem_cpp_msvc) && !defined(filesystem_cpp_clang)) || defined(filesystem_cpp_clang)
         // || defined(filesystem_cpp_gnu) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#endif // !defined(filesystem_warning_push)


#if !defined(filesystem_warning_pop)
#  if defined(filesystem_cpp_msvc) && !defined(filesystem_cpp_clang)
#    define filesystem_warning_pop __pragma(warning(pop))
#  elif defined(filesystem_cpp_clang)
#    define filesystem_warning_pop filesystem_do_pragma(clang diagnostic pop)
#  elif defined(filesystem_cpp_gnu) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#    define filesystem_warning_pop filesystem_do_pragma(GCC diagnostic pop)
#  else
#    define filesystem_warning_pop
#  endif // (defined(filesystem_cpp_msvc) && !defined(filesystem_cpp_clang)) || defined(filesystem_cpp_clang)
         // || defined(filesystem_cpp_gnu) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#endif // !defined(filesystem_warning_pop)


#if !defined(filesystem_disable_warning_msvc)
#  if defined(filesystem_cpp_msvc) && !defined(filesystem_cpp_clang)
#    define filesystem_disable_warning_msvc(number) __pragma(warning(disable: number))
#  else
#    define filesystem_disable_warning_msvc(number)
#  endif // defined(filesystem_cpp_msvc) && !defined(filesystem_cpp_clang)
#endif // !defined(filesystem_disable_warning_msvc)


#if !defined(filesystem_disable_warning_clang)
#  if defined(filesystem_cpp_clang)
#    define filesystem_disable_warning_clang(text) filesystem_do_pragma(clang diagnostic ignored text)
#  else
#    define filesystem_disable_warning_clang(text)
#  endif // defined(filesystem_cpp_msvc) && !defined(filesystem_cpp_clang)
#endif // !defined(filesystem_disable_warning_clang)


#if !defined(filesystem_disable_warning_gcc)
#  if defined(filesystem_cpp_gnu) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#    define filesystem_disable_warning_gcc(text) filesystem_do_pragma(GCC diagnostic ignored text)
#  else
#    define filesystem_disable_warning_gcc(text) 
#  endif // defined(filesystem_cpp_msvc) && !defined(filesystem_cpp_clang)
#endif // !defined(filesystem_disable_warning_gcc)


#if !defined(filesystem_disable_warning_deprecated)
#  if defined(filesystem_cpp_msvc) && !defined(filesystem_cpp_clang)
#    define filesystem_disable_warning_deprecated filesystem_disable_warning_msvc(4996)
#  elif defined(filesystem_cpp_clang)
#    define filesystem_disable_warning_deprecated filesystem_disable_warning_clang("-Wdeprecated-declarations")
#  elif defined(filesystem_cpp_gnu) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#    define filesystem_disable_warning_deprecated filesystem_warning_disable_gcc("-Wdeprecated-declarations")
#  else
#    define filesystem_disable_warning_deprecated
#  endif // (defined(filesystem_cpp_msvc) && !defined(filesystem_cpp_clang)) || defined(filesystem_cpp_clang)
         // || defined(filesystem_cpp_gnu) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#endif // !defined(filesystem_disable_warning_deprecated)


#if !defined(filesystem_cpp_warnings)
#  define filesystem_no_warnings
#endif // !defined(filesystem_cpp_warnings)


#if defined(filesystem_no_warnings)
#  if defined(filesystem_cpp_msvc)
     filesystem_disable_warning_msvc(4828)
     filesystem_disable_warning_msvc(4251)
     filesystem_disable_warning_msvc(4244)
     filesystem_disable_warning_msvc(4275)
     filesystem_disable_warning_msvc(4514)
     filesystem_disable_warning_msvc(4800)
     filesystem_disable_warning_msvc(4097)
     filesystem_disable_warning_msvc(4706)
     filesystem_disable_warning_msvc(4355)
     filesystem_disable_warning_msvc(4710)
     filesystem_disable_warning_msvc(4530)
     filesystem_disable_warning_msvc(4006)
     filesystem_disable_warning_msvc(4715)
     filesystem_disable_warning_msvc(4267)
#  endif // defined(filesystem_cpp_msvc)

#endif // defined(filesystem_no_warnings)

#if !defined(filesystem_nodiscard_return_raw_ptr)
#  define filesystem_nodiscard_return_raw_ptr \
        filesystem_nodiscard_with_warning("This function allocates memory and returns a raw pointer. " \
            "Discarding the return value will cause a memory leak.")
#endif // !defined(filesystem_nodiscard_return_raw_ptr)


#if !defined(filesystem_nodiscard_thread_constructor)
#  define filesystem_nodiscard_thread_constructor \
    filesystem_nodiscard_constructor_with_warning("Creating a thread object without assigning it to a variable " \
        "may lead to unexpected behavior and resource leaks. Ensure " \
        "the thread is properly managed.")
#endif // !defined(filesystem_nodiscard_thread_constructor)


#if !defined(filesystem_nodiscard_remove_algorithm)
#  define filesystem_nodiscard_remove_algorithm \
        filesystem_nodiscard_with_warning("The 'remove' and 'remove_if' algorithms return the iterator past the last element " \
            "that should be kept. You need to call container.erase(result, container.end()) afterwards. " \
            "In C++20, 'std::erase' and 'std::erase_if' are simpler replacements for these two steps.")
#endif // !defined(filesystem_nodiscard_remove_algorithm)


#if !defined(filesystem_deprecated_warning)
#  if defined(filesystem_cpp_msvc)
#    define filesystem_deprecated_warning(message)                                           \
       __pragma(warning(push))                                                         \
       __pragma(warning(disable: 4996))                                                \
       __pragma(message (__FILE__ "(" __LINE__ ") : warning C4996: " message))         \
       __pragma(warning(pop))
#  elif defined(filesystem_cpp_clang)
#    define filesystem_deprecated_warning(message)                                           \
       filesystem_do_pragma("clang diagnostic push")                                         \
       filesystem_do_pragma("clang diagnostic warning \"-Wdeprecated-declarations\"")        \
       filesystem_do_pragma("clang diagnostic ignored \"-Wunused-but-set-variable\"")        \
       filesystem_do_pragma("message \"" __FILE__ "(" __LINE__ ") : warning: " message "\"") \
       filesystem_do_pragma("clang diagnostic pop")
#  elif defined(filesystem_cpp_gnu)
#    define filesystem_deprecated_warning(message)                                           \
       filesystem_do_pragma("GCC diagnostic push")                                           \
       filesystem_do_pragma("GCC diagnostic warning \"-Wdeprecated-declarations\"")          \
       filesystem_do_pragma("message \"" __FILE__ "(" __LINE__ ") : warning: " message "\"") \ 
       filesystem_do_pragma("GCC diagnostic pop")
#  else
#    define filesystem_deprecated_warning(message)
#  endif // defined(filesystem_cpp_msvc) || defined(filesystem_cpp_clang) || defined(filesystem_cpp_gnu)
#endif // !defined(filesystem_deprecated_warning)
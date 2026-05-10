#pragma once 

#if !defined(filesystem_no_stack_protector)
#  if defined(filesystem_cpp_msvc)
#    define filesystem_no_stack_protector __declspec(safebuffers)
#  else
#    define filesystem_no_stack_protector
#  endif
#endif // !defined(filesystem_no_stack_protector)

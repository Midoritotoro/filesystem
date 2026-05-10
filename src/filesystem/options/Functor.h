#pragma once 


#include <src/filesystem/options/Options.h>


__FILESYSTEM_OPTIONS_NAMESPACE_BEGIN

template <template <class> class _Function_>
constexpr inline auto functor = _Function_<filesystem::options::options<>>{};

__FILESYSTEM_OPTIONS_NAMESPACE_END

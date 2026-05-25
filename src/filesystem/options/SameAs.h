#pragma once 

#include <filesystem/compatibility/Compatibility.h>


__FILESYSTEM_OPTIONS_CONCEPTS_NAMESPACE_BEGIN

template <class _Type1_, class _Type2_>
concept is_same = std::is_same_v<_Type1_, _Type2_>;

template <class _Type1_, class _Type2_>
concept same_as = is_same<_Type1_, _Type2_> && is_same<_Type2_, _Type1_>;

__FILESYSTEM_OPTIONS_CONCEPTS_NAMESPACE_END


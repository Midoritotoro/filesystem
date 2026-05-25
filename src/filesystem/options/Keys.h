#pragma once 

#include <filesystem/compatibility/Compatibility.h>
#include <src/filesystem/options/SameAs.h>


__FILESYSTEM_OPTIONS_NAMESPACE_BEGIN

template <class ... _Keys_> 
struct keys 
{};

template <class _Key_, class _Keys_> 
struct contains_;

template <class ... _Keys_, class _Key_>
struct contains_<_Key_, keys<_Keys_...>>: 
    std::bool_constant<(concepts::same_as<_Key_, _Keys_> || ...)>
{};

template <class _Key_, class _Keys_, bool> 
struct append_if_;

template <class ... _Keys_, class _Key_> 
struct append_if_<_Key_, keys<_Keys_...>, true> {
    using type = keys<_Keys_...>;
};

template <class ... _Keys_, class _Key_> 
struct append_if_<_Key_, keys<_Keys_...>, false> {
    using type = keys<_Keys_..., _Key_>;
};

template <class _Key_, class _Keys_> 
struct append_if;

template <class _Key_, class _Keys_> 
struct append_if :
    append_if_<_Key_, _Keys_, contains_<_Key_, _Keys_>::value>
{};

template <class _Key1_, class _Key2_> 
struct uniques_;

template <class _Keys1_,  class _Key2_, class ... _Keys2_>
struct uniques_<_Keys1_, keys<_Key2_, _Keys2_...>>: 
    uniques_<typename append_if<_Key2_, _Keys1_>::type, keys<_Keys2_...>>
{};

template <class _Keys1_> 
struct uniques_<_Keys1_, keys<>> {
    using type = _Keys1_; 
};

template <class _Key1_, class _Key2_> 
struct contain_all_;

template <class _Key1_, class _Key2_>
struct contain_all_;

template <class _Keys1_, class ... _Keys2_>
struct contain_all_<_Keys1_, keys<_Keys2_...>>: 
    std::bool_constant<(contains_<_Keys2_,_Keys1_>::value && ...)>
{};

template <class _Keys1_> 
struct contain_all_<_Keys1_, keys<>>: 
    std::false_type 
{};

template <class _Keys2_>
struct contain_all_<keys<>, _Keys2_>:
    std::false_type 
{};

template <> 
struct contain_all_<keys<>, keys<>>:
    std::true_type  
{};

template <class _Key1_, class _Key2_>
struct is_equivalent: 
    std::bool_constant<contain_all_<_Key2_, _Key1_>::value && contain_all_<_Key2_, _Key1_>::value> 
{};

__FILESYSTEM_OPTIONS_NAMESPACE_END

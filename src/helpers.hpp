/**
 * @file helpers.hpp
 * 
 * @brief Header only helper functions for accessing data relative to a base pointer
 * 
 * @author SavariaS
 */
#ifndef ER_SPELL_SHUFFLER_HELPERS_HPP
#define ER_SPELL_SHUFFLER_HELPERS_HPP

#include <type_traits>

template <typename T>
inline unsigned char* p(T* base, int offset)
{
    return reinterpret_cast<unsigned char*>(
        *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(base) + offset));
}

template <typename T, typename... Offsets>
inline unsigned char* dereference_chain(T base, Offsets... offsets)
{
    static_assert((std::is_same_v<Offsets, int> && ...), "All offsets must be of type int!");
	
    ((base = p(base, offsets)), ...);
    return reinterpret_cast<unsigned char* >(base);
}

#endif
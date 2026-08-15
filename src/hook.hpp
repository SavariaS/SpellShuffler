/**
 * @file hook.hpp
 * 
 * @brief Logic of the mod
 * @details Contains the hook and the logic of the mod
 * 
 * @author SavariaS
 */
#ifndef ER_SPELL_SHUFFLER_HOOK_HPP
#define ER_SPELL_SHUFFLER_HOOK_HPP

#include <iostream>

// Global variables used by hook.cpp but initialized by dllmain.cpp
extern bool g_allowSpellSwitching;
extern bool g_equippedSpellsOnly;
extern bool g_sorceriesOnly;
extern bool g_incantsOnly;
extern std::ostream* g_logFile;

void spellShufflerMain(void* playerIns);

#endif
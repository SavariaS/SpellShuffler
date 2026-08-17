/**
 * @file dllmain.cpp
 * 
 * @brief Entry point of the DLL. Sets up the mod
 * @details Load the configuration, attach the hook and scan for the relevant structures
 * 
 * @author SavariaS
 */
#include "hook.hpp"
#include "PlayerIns.hpp"
#include "GameDataMan.hpp"
#include "MagicParam.hpp"

#include "RTTIHook/VFTHook.h"
#include "Pattern16/Pattern16.h"
#include "mINI/ini.h"

#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <fstream>
#include <string>

VFTHook* g_hook = nullptr; //< The main hook

/**
 * @brief Gets the directory the DLL resides in
 * @param hModule The handle to the current module
 * @return The path to the directory
 */
std::string getDllDirectory(HMODULE hModule)
{
	char path[MAX_PATH];
	GetModuleFileNameA(hModule, path, sizeof(path));
	
	std::string dllDirectory = std::string(path);
	dllDirectory = dllDirectory.substr(0, dllDirectory.find_last_of('\\'));
	return dllDirectory;
}

/**
 * @brief Resolves the address of a structure from a signature
 * @param signature The AOB to scan
 * @return A pointer to a pointer to the structure
 */
static void* resolve_rip_relative_static(const std::string& signature)
{
    HMODULE mainModule = GetModuleHandle(nullptr);
    MODULEINFO moduleInfo{};
    GetModuleInformation(GetCurrentProcess(), mainModule, &moduleInfo, sizeof(moduleInfo));
 
    void* instrAddr = Pattern16::scan(moduleInfo.lpBaseOfDll, moduleInfo.SizeOfImage, signature);
    if (!instrAddr) {
        return nullptr;
    }
 
    int32_t displacement = *reinterpret_cast<int32_t*>(
        reinterpret_cast<uint8_t*>(instrAddr) + 3);
    uint8_t* nextInstr = reinterpret_cast<uint8_t*>(instrAddr) + 7;
 
    return nextInstr + displacement;
}

/**
 * @brief Attach the hook to the game
 * @param hModule The handle to the current module
 */
void attachHook(HMODULE hModule)
{
	std::string dllDirectory = getDllDirectory(hModule);
	
    //=======================================================================//
    //                          Logging                                      //
    //=======================================================================//
#ifdef NDEBUG
    // If compiling a release build, log to a file
	g_logFile = new std::ofstream(dllDirectory + "\\SpellShuffler.log");
#endif
#ifndef NDEBUG
    // If compiling a debug build, log to a console directly
    AllocConsole();
    FILE* dummy;
    freopen_s(&dummy, "CONOUT$", "w", stdout);
    freopen_s(&dummy, "CONOUT$", "w", stderr);
	g_logFile = &std::cout;
#endif
	*g_logFile << "[SpellShuffler] dll attached" << std::endl;

    //=======================================================================//
    //                       Configuration                                   //
    //=======================================================================//
    *g_logFile << "[SpellShuffler] Loading config..." << std::endl;
    mINI::INIStructure ini;
    mINI::INIFile file(dllDirectory + "\\SpellShuffler.ini");
    file.read(ini);

    g_allowSpellSwitching = (ini["Configuration"]["allowSpellSwitching"] == "true") ? true : false;
	g_equippedSpellsOnly = (ini["Configuration"]["equippedSpellsOnly"] == "true") ? true : false;
	g_sorceriesOnly = (ini["Configuration"]["sorceriesOnly"] == "true") ? true : false;
	g_incantsOnly = (ini["Configuration"]["incantsOnly"] == "true") ? true : false;

    *g_logFile << std::boolalpha;
    *g_logFile << "[SpellShuffler] allowSpellSwitching is " << g_allowSpellSwitching << std::endl;
    *g_logFile << "[SpellShuffler] equippedSpellsOnly is " << g_equippedSpellsOnly << std::endl;
    *g_logFile << "[SpellShuffler] sorceriesOnly is " << g_sorceriesOnly << std::endl;
    *g_logFile << "[SpellShuffler] incantsOnly is " << g_incantsOnly << std::endl;

    //=======================================================================//
    //                            Hook                                       //
    //=======================================================================//
	*g_logFile << "[SpellShuffler] Scanning RTTI..." << std::endl;
	RTTIScanner scanner;
    if (!scanner.scan())
	{
        *g_logFile << "[SpellShuffler] RTTI scan failed - aborting." << std::endl; 
		*g_logFile << "[SpellShuffler] Is the game fully loaded into the main menu or a save yet?" << std::endl;
        return;
    }
    g_hook = new VFTHook("CS::PlayerIns", 20, spellShufflerMain);
	
    //=======================================================================//
    //                            AOB scans                                  //
    //=======================================================================//
    *g_logFile << "[SpellShuffler] Scanning for WorldChrMan..." << std::endl;
	void** worldChrManPtr = reinterpret_cast<void**>(resolve_rip_relative_static("48 8B 05 ?? ?? ?? ?? 48 85 C0 74 0F 48 39 88"));
	PlayerIns::getInstance().setPointer(worldChrManPtr);
    if (!worldChrManPtr) 
	{
        *g_logFile << "[SpellShuffler] WorldChrMan signature scan failed - aborting" << std::endl;
        abort();
    }

	*g_logFile << "[SpellShuffler] Scanning for GameDataMan..." << std::endl;
	void** gameDataManPtr = reinterpret_cast<void**>(resolve_rip_relative_static("48 8B 05 ?? ?? ?? ?? 48 85 C0 74 05 48 8B 40 58 C3 C3"));
	GameDataMan::getInstance().setPointer(gameDataManPtr);
    if (!gameDataManPtr) 
	{
        *g_logFile << "[SpellShuffler] GameDataMan signature scan failed - aborting" << std::endl;
        abort();
    }
	
	
	*g_logFile << "[SpellShuffler] Scanning for CSRegulationManager..." << std::endl;
	void** regulationManagerPtr = reinterpret_cast<void**>(resolve_rip_relative_static("48 8B 0D ?? ?? ?? ?? 48 85 C9 74 0B 4C 8B C0 48 8B D7"));
	MagicParam::getInstance().setPointer(regulationManagerPtr);
	if(!regulationManagerPtr)
	{
		*g_logFile << "[SpellShuffler] CSRegulationManager signature scan failed - aborting" << std::endl;
        abort();
	}

	*g_logFile << "[SpellShuffler] Mod has been loaded successfully" << std::endl;
}

/**
 * @brief Deletes the hook
 */
void detachHook()
{
	if(g_hook) delete g_hook;
	g_hook = nullptr;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        attachHook(hModule);
        break;
    case DLL_PROCESS_DETACH:
        detachHook();
        break;
    default:
        break;
    }
    return TRUE;
}
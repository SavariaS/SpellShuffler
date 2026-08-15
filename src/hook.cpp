/**
 * @file hook.cpp
 * 
 * @brief Logic of the mod
 * @details Contains the hook and the logic of the mod
 * 
 * @author SavariaS
 */
#include "hook.hpp"
#include "PlayerIns.hpp"
#include "GameDataMan.hpp"
#include "MagicParam.hpp"

#include <iostream>
#include <random>

bool g_allowSpellSwitching = false; //< Is changing the selected spell index allowed?
bool g_equippedSpellsOnly = false; //< If true, randomizes to a random equipped spell. If false, randomizes to any spell
bool g_sorceriesOnly = false; //< Only select sorceries
bool g_incantsOnly = false; //< Only select incantations

bool g_isCasting = false; //< Tracks if the player is currently casting
int32_t g_selectedIndex = 0; //< Tracks the currently equipped spell index
std::ostream* g_logFile = nullptr; //< The output stream to use for logging
std::vector<Magic> g_spells; //< The list of all spells

std::random_device g_seed; //< Random device for seeding the random number engine
std::mt19937 g_randomGenerator(g_seed()); //< Random number engine

/**
 * @brief Checks if an animation is a spell casting animation
 * @details The animation ID of each spell starts at 400,045,100 + (spell.refType * 1,000,000)
 *          The maximum refType currently in the game is Renalla's Twin Moons at 161
 *          Therefore, checking refTypes between 0 and 199 seems reasonable
 * 
 * @param animationId The ID of the animation currently playing
 * @return True if the player is in a spell casting animation, False otherwise
 */
bool isMagicAnimation(int32_t animationId)
{
	return 400000000 < animationId && animationId < 600000000;
}

/**
 * @brief Gets the list of all player spells from the Magic param table
 * @details Discards NPC spells, SpEffects spells and certain non-intended spells
 *          Also discards sorceries when g_incantsOnly is set and incantations
 *          when g_sorceriesOnly is set
 */
void generateSpellList()
{
	std::vector<Magic> allMagic = MagicParam::getInstance().getAllMagic();
	for(Magic& magic : allMagic)
	{
		if(g_incantsOnly && !magic.isIncantation) continue;
		if(g_sorceriesOnly && magic.isIncantation) continue;
		
		// Exceptions
		if(magic.fp == 0 || magic.fp == 1) continue; // FP cost of 0 or 1 means a NPC spell
		if(magic.refCategory == 2) continue; // Ignore SpEffects, only keep offensive spells (Bullet or Attack)
		if(magic.id == 4641) continue; // Carian Retaliation (Unused 1)
		if(magic.id == 4642) continue; // Carian Retaliation (Unused 2)
		if(magic.id == 8000) continue; // Incantation version of Briars of Sin
		if(magic.id == 8001) continue; // Incantation version of Briars of Punishment
		if(magic.id == 999999999) continue; // [NPC: Incantation] Golden Lightning Fortification (NPC spell with real FP cost)
		
		g_spells.push_back(magic);
	}
}

/**
 * @brief Selects a random spell slot
 * @details Only spells that the player meets the stat requirements for and has
 *          enough FP to cast are considered.
 */
void selectRandomSlot()
{
	uint32_t fp = PlayerIns::getInstance().getFP();
	int32_t intelligence = GameDataMan::getInstance().getIntelligence();
	int32_t faith = GameDataMan::getInstance().getFaith();
	int32_t arcane = GameDataMan::getInstance().getArcane();
	
	std::vector<int32_t> availableIndices;
	int32_t currentIndex = GameDataMan::getInstance().getSelectedSpellIndex();
	int32_t spellCount = GameDataMan::getInstance().getEquippedSpellCount();
	for(int32_t i = 0; i < spellCount; ++i)
	{
		if(i == currentIndex) continue; // Skip current index
		
		int32_t magicId = GameDataMan::getInstance().getEquippedSpellId(i);
		Magic magic = MagicParam::getInstance().getMagicById(magicId);
		
		if(magic.fp <= fp &&
		   magic.intelligence <= intelligence &&
		   magic.faith <= faith &&
		   magic.arcane <= arcane &&
		   !(g_sorceriesOnly && magic.isIncantation) && // If sorceriesOnly is active and the spell is an incantation, reject it
		   !(g_incantsOnly && !magic.isIncantation)) // If incantsOnly is active and the spell is a sorcery, reject it
		{
			availableIndices.push_back(i);
		}
	}
	
	if(availableIndices.size() != 0)
	{
		std::uniform_int_distribution<int> distribution(0, availableIndices.size() - 1);
		g_selectedIndex = availableIndices.at(distribution(g_randomGenerator));
		GameDataMan::getInstance().setSelectedSpellIndex(g_selectedIndex);
		*g_logFile << "[SpellShuffler] New selected spell index: " << g_selectedIndex << std::endl;
	}
}

/**
 * @brief Selects a random spell
 * @details Only spells that the player meets the stat requirements for and has
 *          enough FP to cast are considered.
 */
void selectRandomSpell()
{
	if(g_spells.empty())
	{
		generateSpellList();
	}
	
	uint32_t fp = PlayerIns::getInstance().getFP();
	int32_t intelligence = GameDataMan::getInstance().getIntelligence();
	int32_t faith = GameDataMan::getInstance().getFaith();
	int32_t arcane = GameDataMan::getInstance().getArcane();
	
	std::vector<const Magic*> availableSpells;
	for(Magic& spell : g_spells)
	{
		if(spell.fp <= fp &&
		   spell.intelligence <= intelligence &&
		   spell.faith <= faith &&
		   spell.arcane <= arcane)
		{
		   availableSpells.push_back(&spell);
		}
	}
	
	if(availableSpells.size() != 0)
	{
		std::uniform_int_distribution<int> distribution(0, availableSpells.size() - 1);
		int32_t newSpellId = availableSpells.at(distribution(g_randomGenerator))->id;
		int32_t selectedIndex = (g_allowSpellSwitching) ? GameDataMan::getInstance().getSelectedSpellIndex()
		                                                : g_selectedIndex;
		GameDataMan::getInstance().setEquippedSpell(selectedIndex, newSpellId);
		*g_logFile << "[SpellShuffler] New spell: " << newSpellId << std::endl;
	}
}

/**
 * @brief Main loop of the mod
 */
void spellShufflerMain(void* playerIns)
{
	PlayerIns::getInstance().setPointer(playerIns);
	
	if(!g_allowSpellSwitching)
	{
		GameDataMan::getInstance().setSelectedSpellIndex(g_selectedIndex);
	}
	
	int32_t animationId = PlayerIns::getInstance().getAnimationId();
	if(!g_isCasting && isMagicAnimation(animationId))
	{
		g_isCasting = true;
	}
	else if(g_isCasting && !isMagicAnimation(animationId))
	{
		g_isCasting = false;
		
		if(g_equippedSpellsOnly)
		{
			selectRandomSlot();
		}
		else
		{
			selectRandomSpell();
		}
	}
}
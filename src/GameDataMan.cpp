/**
 * @file GameDataMan.cpp
 * 
 * @brief Singleton for accessing attributes of the GameDataMan structure
 * @details Contains getter functions for getting the PC's intelligence, faith
 *          and arcane levels as well as the equipped spell ID and equipped slot ID
 * 
 *          Also contains setter functions for setting the equipped spell ID or the
 *          equipped slot ID
 * 
 * @author SavariaS
 */
#include "GameDataMan.hpp"
#include "helpers.hpp"

#define SPELL_NONE 0xFFFFFFFF

GameDataMan::GameDataMan() {}

GameDataMan& GameDataMan::getInstance()
{
	if(singleton == nullptr)
	{
		singleton = new GameDataMan();
	}
	
	return *singleton;
}

void GameDataMan::setPointer(void** pointer)
{
	m_gameDataManPtr = pointer;
}

int32_t GameDataMan::getIntelligence()
{
	void* gameDataMan = *reinterpret_cast<void**>(m_gameDataManPtr);
	unsigned char* base = dereference_chain(gameDataMan, 0x08);
	return *reinterpret_cast<int32_t*>(base + 0x50);
}

int32_t GameDataMan::getFaith()
{
	void* gameDataMan = *reinterpret_cast<void**>(m_gameDataManPtr);
	unsigned char* base = dereference_chain(gameDataMan, 0x08);
	return *reinterpret_cast<int32_t*>(base + 0x54);
}

int32_t GameDataMan::getArcane()
{
	void* gameDataMan = *reinterpret_cast<void**>(m_gameDataManPtr);
	unsigned char* base = dereference_chain(gameDataMan, 0x08);
	return *reinterpret_cast<int32_t*>(base + 0x58);
}

int32_t GameDataMan::getSelectedSpellIndex()
{
	void* gameDataMan = *reinterpret_cast<void**>(m_gameDataManPtr);
	unsigned char* base = dereference_chain(gameDataMan, 0x08, 0x530);
	return *reinterpret_cast<int32_t*>(base + 0x80);
}

int32_t GameDataMan::getEquippedSpellCount()
{
	int32_t index = 0;
	while(static_cast<uint32_t>(getEquippedSpellId(index)) != SPELL_NONE) ++index;
	return index;
}

int32_t GameDataMan::getEquippedSpellId(uint8_t index)
{
	void* gameDataMan = *reinterpret_cast<void**>(m_gameDataManPtr);
	unsigned char* base = dereference_chain(gameDataMan, 0x08, 0x530);
	return *reinterpret_cast<int32_t*>(base + 0x10 + (0x08 * index));
}

void GameDataMan::setSelectedSpellIndex(int32_t index)
{
	void* gameDataMan = *reinterpret_cast<void**>(m_gameDataManPtr);
	unsigned char* base = dereference_chain(gameDataMan, 0x08, 0x530);
	*reinterpret_cast<int32_t*>(base + 0x80) = index;
}

void GameDataMan::setEquippedSpell(uint8_t index, int32_t spellId)
{
	void* gameDataMan = *reinterpret_cast<void**>(m_gameDataManPtr);
	unsigned char* base = dereference_chain(gameDataMan, 0x08, 0x530);
	*reinterpret_cast<int32_t*>(base + 0x10 + (0x08 * index)) = spellId;
}
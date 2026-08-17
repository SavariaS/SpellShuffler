/**
 * @file PlayerIns.cpp
 * 
 * @brief Singleton for accessing attributes of the PlayerIns structure
 * @details Contains getter function for the animation ID currently playing
 *          and the PC's remaining FP
 * 
 * @author SavariaS
 */
#include "PlayerIns.hpp"
#include "helpers.hpp"

PlayerIns::PlayerIns() {}

PlayerIns& PlayerIns::getInstance()
{
	if(singleton == nullptr)
	{
		singleton = new PlayerIns();
	}
	
	return *singleton;
}

void PlayerIns::setPointer(void** pointer)
{
	m_worldChrManPtr = pointer;
}

void* PlayerIns::getPointer(uint8_t index)
{
	void* worldChrMan = *m_worldChrManPtr;
	unsigned char* base = dereference_chain(worldChrMan, 0x10EF8, index * 0x10);
	return reinterpret_cast<void*>(base);
}

int32_t PlayerIns::getAnimationId(uint8_t index)
{
	void* playerIns = getPointer(index);
	unsigned char* base = dereference_chain(playerIns, 0x190, 0x18);
	return *reinterpret_cast<int32_t*>(base + 0x20);
}

uint32_t PlayerIns::getFP(uint8_t index)
{
	void* playerIns = getPointer(index);
	unsigned char* base = dereference_chain(playerIns, 0x190, 0x00); 
	return *reinterpret_cast<uint32_t*>(base + 0x148);
}
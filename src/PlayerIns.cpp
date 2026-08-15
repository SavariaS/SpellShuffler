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

void PlayerIns::setPointer(void* pointer)
{
	m_playerIns = pointer;
}

int32_t PlayerIns::getAnimationId()
{
	unsigned char* base = dereference_chain(m_playerIns, 0x190, 0x18);
	return *reinterpret_cast<int32_t*>(base + 0x20);
}

uint32_t PlayerIns::getFP()
{
	unsigned char* base = dereference_chain(m_playerIns, 0x190, 0x00); 
	return *reinterpret_cast<uint32_t*>(base + 0x148);
}
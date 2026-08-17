/**
 * @file PlayerIns.hpp
 * 
 * @brief Singleton for accessing attributes of the PlayerIns structure
 * @details Contains getter function for the animation ID currently playing
 *          and the character's remaining FP
 * 
 * @author SavariaS
 */
#ifndef ER_SPELL_SHUFFLER_PLAYERINS_HPP
#define ER_SPELL_SHUFFLER_PLAYERINS_HPP

#include <cstdint>

/**
 * @brief Singleton for the PlayerIns structure
 */
class PlayerIns
{
public:
	/**
	 * @brief Get the singleton instance
	 * @return The PlayerIns singleton
	 */
	static PlayerIns& getInstance();

	/**
	 * @brief Set the level 2 pointer used to access WorldChrMan
	 * @param pointer The level 2 pointer to WorldChrMan
	 */
	void setPointer(void** pointer);

	/**
	 * @brief Get the PlayerIns pointer of a character
	 * @param index Index of the character
	 * @return The pointer to the PlayerIns object of that character
	 */
	void* getPointer(uint8_t index);
	
	/**
	 * @brief Get the ID of the character's current animation
	 * @param index Index of the character
	 * @return The animation ID
	 */
	int32_t getAnimationId(uint8_t index);

	/**
	 * @brief Get the character's remaining FP
	 * @param index Index of the character
	 * @return Remaining FP
	 */
	uint32_t getFP(uint8_t index);
	
private:
	/**
	 * @brief Private ctor for the singleton design pattern
	 */
	PlayerIns();
	
	// PlayerIns may change location so a level 2 pointer is used every access
	// to get a valid pointer
	void** m_worldChrManPtr = nullptr;
	static inline PlayerIns* singleton = nullptr;
};

#endif
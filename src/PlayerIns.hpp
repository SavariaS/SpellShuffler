/**
 * @file PlayerIns.hpp
 * 
 * @brief Singleton for accessing attributes of the PlayerIns structure
 * @details Contains getter function for the animation ID currently playing
 *          and the PC's remaining FP
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
	 * @brief Set the pointer used to access PlayerIns
	 * @param pointer The pointer to PlayerIns
	 */
	void setPointer(void* pointer);
	
	/**
	 * @brief Get the ID of the current animation
	 * @return The animation ID
	 */
	int32_t getAnimationId();

	/**
	 * @brief Get the PC's remaining FP
	 * @return Remaining FP
	 */
	uint32_t getFP();
	
private:
	/**
	 * @brief Private ctor for the singleton design pattern
	 */
	PlayerIns();
	
	// The PlayerIns pointer comes from the hook so it is always valid
	// No need for level 2 pointer in this case
	void* m_playerIns = nullptr;
	static inline PlayerIns* singleton = nullptr;
};

#endif
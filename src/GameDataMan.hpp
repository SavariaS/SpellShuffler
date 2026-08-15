/**
 * @file GameDataMan.hpp
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
#ifndef ER_SPELL_SHUFFLER_GAMEDATAMAN_HPP
#define ER_SPELL_SHUFFLER_GAMEDATAMAN_HPP

#include <cstdint>

/**
 * @brief Singleton for the GameDataMan structure
 */
class GameDataMan
{
public:
	/**
	 * @brief Get the singleton instance
	 * @return The GameDataMan singleton
	 */
	static GameDataMan& getInstance();

	/**
	 * @brief Set the level 2 pointer used to access the GameDataMan struct
	 * @param pointer Level 2 pointer to the GameDataMan struct
	 */
	void setPointer(void** pointer);
	
	/**
	 * @brief Get the intelligence level of the PC
	 * @return The intelligence level
	 */
	int32_t getIntelligence();

	/**
	 * @brief Get the faith level of the PC
	 * @return The faith level
	 */
	int32_t getFaith();

	/**
	 * @brief Get the arcane level of the PC
	 * @return The arcane level
	 */
	int32_t getArcane();
	
	/**
	 * @brief Get the index of the selected spell slot
	 * @return The index of the selected spell slot
	 */
	int32_t getSelectedSpellIndex();

	/**
	 * @brief Get the number of equipped spells
	 * @return The number of equipped spells
	 */
	int32_t getEquippedSpellCount();

	/**
	 * @brief Get the ID of the spell equipped in a slot
	 * @param index The spell slot
	 * @return The spell ID
	 */
	int32_t getEquippedSpellId(uint8_t index);
	
	/**
	 * @brief Set the selected spell slot
	 * @param index The spell slot to select
	 */
	void setSelectedSpellIndex(int32_t index);

	/**
	 * @brief Set the spell equipped in a slot
	 * @param index The spell slot to read
	 * @param spellId The spell to equip
	 */
	void setEquippedSpell(uint8_t index, int32_t spellId);
	
private:
	/**
	 * @brief Private ctor for the singleton design pattern
	 */
	GameDataMan();
	
	// GameDataMan may change location so a level 2 pointer is used every access
	// to get a valid pointer
	void** m_gameDataManPtr = nullptr;
	static inline GameDataMan* singleton = nullptr;
};

#endif
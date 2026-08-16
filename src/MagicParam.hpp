/**
 * @file MagicParam.hpp
 * 
 * @brief Singleton for accessing the Magic param table
 * @details Contains getters for getting all rows or
 *          only getting 1 row by ID
 * 
 * @author SavariaS
 */
#ifndef ER_SPELL_SHUFFLER_MAGIC_PARAM_HPP
#define ER_SPELL_SHUFFLER_MAGIC_PARAM_HPP

#include <vector>
#include <cstdint>
#include "TGA/param_containers.h"

/**
 * @brief POD structure containing the relevant fields of a row
*/
struct Magic
{
	int32_t id;
	uint16_t fp;
	uint8_t intelligence;
	uint8_t faith;
	uint8_t arcane;
	bool isOffensiveMagic;
	bool isIncantation;
};

/**
 * @brief Singleton for accessing the Magic param table
 */
class MagicParam
{
public:
	/**
	 * @brief Get the singleton instance
	 * @return The MagicParam singleton
	 */
	static MagicParam& getInstance();

	/**
	 * @brief Set the level 2 pointer used to access the param tables
	 * @param pointer Level 2 pointer to param tables
	 */
	void setPointer(void** pointer);
	
	/**
	 * @brief Get a row by ID
	 * @param magicId The header ID of the row
	 * @return The relevant fields packed in a struct
	 */
	Magic getMagicById(int32_t magicId);

	/**
	 * @brief Get every row of the Magic param table
	 * @return A list of every row
	 */
	std::vector<Magic> getAllMagic();

private:
	/**
	 * @brief Private ctor for the singleton design pattern
	 */
	MagicParam();
	
	/**
	 * @brief Helper function for getting a pointer to the Magic param table
	 */
	void getMagicTable();

	/**
	 * @brief Helper function for loading the relevant fields of a row in the Magic struct
	 * @param row The row to load
	 * @param id The header ID of the row
	 * @return A Magic struct populated by the row
	 */
	Magic getMagicByRow(uint8_t* row, int32_t id);
	
	// The RegulationManager pointer is not known when the mod starts loading
	// A level 2 pointer is used to access it once a save has been loaded
	void** m_regulationManagerPtr = nullptr;
	_ParamTable* m_magicTable = nullptr; //< Reference to the Magic table that will be reused
	static inline MagicParam* singleton = nullptr;
};

#endif
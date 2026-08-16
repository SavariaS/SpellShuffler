/**
 * @file MagicParam.cpp
 * 
 * @brief Singleton for accessing the Magic param table
 * @details Contains getters for getting all rows or
 *          only getting 1 row by ID
 * 
 * @author SavariaS
 */
#include "MagicParam.hpp"

MagicParam::MagicParam() {}

MagicParam& MagicParam::getInstance()
{
	if(singleton == nullptr)
	{
		singleton = new MagicParam();
	}
	
	return *singleton;
}

void MagicParam::setPointer(void** pointer)
{
	m_regulationManagerPtr = pointer;
}

void MagicParam::getMagicTable()
{
	_CSRegulationManagerImp* regulationManager = *reinterpret_cast<_CSRegulationManagerImp**>(m_regulationManagerPtr);
	for(_ParamResCap** it = regulationManager->param_list_begin; it != regulationManager->param_list_end; ++it)
    {
        _ParamResCap* resCap = *it;
        if (wcscmp(dlw_c_str(&resCap->param_name), L"Magic") == 0) {
            m_magicTable = resCap->param_header->param_table;
        }
    }
}

Magic MagicParam::getMagicByRow(uint8_t* row, int32_t id)
{
	Magic magic;
	magic.id = id;
	magic.isOffensiveMagic = !(*reinterpret_cast<uint8_t*>(row + 0x32) & (1<<6)); // Flag for if the spell can be used when attacking is disabled
	magic.isIncantation = *reinterpret_cast<uint8_t*>(row + 0x26);
	magic.fp = *reinterpret_cast<uint16_t*>(row + 0x10);
	magic.intelligence = *reinterpret_cast<uint8_t*>(row + 0x22);
	magic.faith = *reinterpret_cast<uint8_t*>(row + 0x23);
	magic.arcane = *reinterpret_cast<uint8_t*>(row + 0xe);

	return magic;
}

Magic MagicParam::getMagicById(int32_t magicId)
{
	if(m_magicTable == nullptr) getMagicTable();
	
	for (uint16_t i = 0; i < m_magicTable->num_rows; ++i)
	{
		if(m_magicTable->rows[i].row_id == magicId)
		{
			uint8_t* row = reinterpret_cast<uint8_t*>(m_magicTable) + m_magicTable->rows[i].param_offset;
			return getMagicByRow(row, magicId);
		}
	}
	
	// If no magic by that name was found, return the ID for NONE
	Magic magic;
	magic.id = 0xFFFFFFFF;
	return magic;
}

std::vector<Magic> MagicParam::getAllMagic()
{
	if(m_magicTable == nullptr) getMagicTable();
	
	std::vector<Magic> magicList;
	for (uint16_t i = 0; i < m_magicTable->num_rows; ++i)
	{
		uint8_t* row = reinterpret_cast<uint8_t*>(m_magicTable) + m_magicTable->rows[i].param_offset;
		magicList.push_back(getMagicByRow(row, m_magicTable->rows[i].row_id));
	}
	
	return magicList;
}
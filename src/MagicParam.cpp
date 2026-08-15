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

Magic MagicParam::getMagicById(int32_t magicId)
{
	if(m_magicTable == nullptr) getMagicTable();
	
	Magic magic;
	for (uint16_t i = 0; i < m_magicTable->num_rows; ++i)
	{
		if(m_magicTable->rows[i].row_id == magicId)
		{
			uint8_t* row = reinterpret_cast<uint8_t*>(m_magicTable) + m_magicTable->rows[i].param_offset;
		
			magic.id = m_magicTable->rows[i].row_id;
			magic.refCategory = *reinterpret_cast<uint8_t*>(row + 0x1e);
			magic.isIncantation = *reinterpret_cast<uint8_t*>(row + 0x26);
			magic.fp = *reinterpret_cast<uint16_t*>(row + 0x10);
			magic.intelligence = *reinterpret_cast<uint8_t*>(row + 0x22);
			magic.faith = *reinterpret_cast<uint8_t*>(row + 0x23);
			magic.arcane = *reinterpret_cast<uint8_t*>(row + 0xe);
			
			return magic;
		}
	}
	
	// If no magic by that name was found, return the ID for NONE
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
		
		Magic magic;
		magic.id = m_magicTable->rows[i].row_id;
		magic.refCategory = *reinterpret_cast<uint8_t*>(row + 0x1e);
		magic.isIncantation = *reinterpret_cast<uint8_t*>(row + 0x26);
		magic.fp = *reinterpret_cast<uint16_t*>(row + 0x10);
		magic.intelligence = *reinterpret_cast<uint8_t*>(row + 0x22);
		magic.faith = *reinterpret_cast<uint8_t*>(row + 0x23);
		magic.arcane = *reinterpret_cast<uint8_t*>(row + 0xe);
		
		/*if(spell.fp == 0 || spell.fp == 1) continue; // FP cost of 0 or 1 means a NPC spell
		if(refCategory == 2) continue; // Ignore SpEffects, only keep offensive spells (Bullet or Attack)
		if(isIncantation) continue;
	
		// Exceptions
		if(spell.id == 4641) continue; // Carian Retaliation (Unused 1)
		if(spell.id == 4642) continue; // Carian Retaliation (Unused 2)
		if(spell.id == 8000) continue; // Incantation version of Briars of Sin
		if(spell.id == 8001) continue; // Incantation version of Briars of Punishment*/
		
		magicList.push_back(magic);
	}
	
	return magicList;
}
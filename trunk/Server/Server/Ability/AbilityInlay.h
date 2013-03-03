/********************************************************************************
 *	�ļ�����	AbilityInlay.h
 *	ȫ·����	d:\Prj\Server\Server\Ability\AbilityInlay.h
 *	�����ˣ�	����
 *	����ʱ�䣺	2006 �� 3 �� 23 ��	15:27
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef __ABILITYINLAY_H__
#define __ABILITYINLAY_H__

#include "AbilityCompound.h"

class AbilityInlay : public AbilityCompound
{
	enum EmbedResult
	{
		EMBED_SUCCESS = 0,
		GEM_GONE,
		EQUIP_GONE,
		BOTH_GONE,
		ENERGY_NOT_ENOUGH,
		GEM_DEGRADE_1,
		GEM_DEGRADE_2,
		GEM_DEGRADE_3,
	};

public:
	AbilityInlay(const Ability& a);

	virtual
		ORESULT					CanUseAbility(Obj_Human* pHuman);

	virtual
		ORESULT					OnProcOver(Obj_Human* pHuman);

	BOOL						IsGemConflict(Obj_Human* pHuman, BYTE GemIndex, BYTE EquipIndex);

protected:
	virtual
		ORESULT					OnProcSuccess(Obj_Human* pHuman, const _PRESCRIPTION_DATA* pPrescrData);

private:
	ORESULT						OnProcSuccess(Obj_Human* pHuman);
};

#endif // __ABILITYINLAY_H__

// AbilityMachining.h

#ifndef __ABILITYMACHINING_H__
#define __ABILITYMACHINING_H__

#include "Ability.h"
#include "AbilityCompound.h"

class AbilityMachining : public AbilityCompound
{
	
public:
	enum MachiningType
	{
		NORMAL_TYPE = 0,		// ��ͨ�ӹ�
		GEM_COMPOUND,			// ��ʯ�ϳ�
		GEM_EMBED,				// ��ʯ��Ƕ
		TYPE_SIZE,				// �ӹ���������
	};

public:
	AbilityMachining();
	AbilityMachining(const Ability& a);
	~AbilityMachining();

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

#endif // __ABILITYMACHINING_H__
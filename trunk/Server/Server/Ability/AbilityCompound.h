// AbilityCompound.h

#ifndef __ABILITYCOMPOUND_H__
#define __ABILITYCOMPOUND_H__

#include "Ability.h"

class AbilityCompound : public Ability
{
public:
	AbilityCompound(const Ability& a) : Ability(a) {}

	/*
	 * �ж��Ƿ���Ժϳ�
	 */
	virtual
		ORESULT					CanUseAbility(Obj_Human* pHuman);

	virtual
		VOID					OnProcInterrupt(Obj_Human* pHuman) { Ability::OnProcInterrupt(pHuman); }

	virtual
		ORESULT					OnProcOver(Obj_Human* pHuman);

protected:
	/*
	 * ���䷽�ĺϳɳɹ����
	 */
	virtual
		ORESULT					OnProcSuccess(Obj_Human* pHuman, const _PRESCRIPTION_DATA* pPrescrData);

	virtual
		ORESULT					OnProcFailure(Obj_Human* pHuman) { return OR_OK; }

	virtual
		ORESULT					OnProcFailure(Obj_Human* pHuman, const _PRESCRIPTION_DATA* pPrescrData);

	/*
	 * �ж��Ƿ������䷽Ҫ��
	 */
	virtual
		ORESULT					IsFitPrescr(Obj_Human* pHuman, const _PRESCRIPTION_DATA* pPrescrData);

	virtual
		ORESULT					AbilityConsume(Obj_Human* pHuman, const _PRESCRIPTION_DATA* pPrescrData);

	virtual
		BOOL					CreateItemToHuman(Obj_Human* pHuman, UINT uItemSN, INT nCount, INT nQuality = 1);

private:
};

#endif // __ABILITYCOMPOUND_H__

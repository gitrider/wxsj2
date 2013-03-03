
/** ����������-���＼��
*/

#pragma once


#include "GMActionSystem.h"
#include "../DataPool/GMDP_Struct_Pet.h"



class CActionItem_PetSkill : public CActionItem
{
public:

	//�õ���������
	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_PET_SKILL;	}
	//�����ַ���
	virtual	LPCTSTR			GetType_String(VOID)	{ return NAMETYPE_PETSKILL;	}
	//�õ�����ID
	virtual INT				GetDefineID(VOID);
	//�õ�����
	virtual INT				GetNum(VOID)			{ return -1;	}
	//�õ��ڲ�����
	virtual VOID*			GetImpl(VOID)			{ return (VOID*)GetPetSkillImpl();	}
	//�õ�����
	virtual LPCTSTR			GetDesc(VOID);
	//�õ���ȴ״��ID
	virtual INT				GetCoolDownID(VOID);
	//�õ���������������
	//	����			- �ڼ�������
	virtual INT				GetPosIndex(VOID);
	//�Ƿ��ܹ��Զ���������
	virtual BOOL			AutoKeepOn(VOID) { return FALSE; }
	//�����
	virtual VOID			DoAction(VOID);
	//�Ƿ���Ч
	virtual BOOL			IsValidate(VOID) { return TRUE; }
	//�����ȴ�Ƿ����
	virtual BOOL			CoolDownIsOver(VOID);
	//�϶�����
	virtual VOID			NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName);

	virtual VOID			UpdateCoolDown(VOID);

public:

	CActionItem_PetSkill(INT nID);
	virtual ~CActionItem_PetSkill();

	//���ݼ��ܸ���
	VOID	Update_PetSkill(const PET_GUID_t& PetGUID, const PET_SKILL* pPetSkill, INT nSkillIndex);
	const PET_GUID_t&	GetMountGUID(){ return m_GUID; }

protected:

	//�õ���������
	PET_SKILL*	GetPetSkillImpl(VOID);

protected:

	//���ڼ��ܲ���ʱ����������
	INT			m_nSkillID;
	INT			m_nSkillIndex;
	PET_GUID_t	m_GUID;

};

/** ����������-�����
 */


#pragma once


#include "GMActionSystem.h"


class CActionItem_LifeAbility : public CActionItem
{
public:

	//�õ���������
	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_SKILL_LIFEABILITY;	}
	//�����ַ���
	virtual	LPCTSTR			GetType_String(VOID)	{ return NAMETYPE_LIFESKILL; }
	//�õ�����ID
	virtual INT				GetDefineID(VOID);
	//�õ�����
	virtual INT				GetNum(VOID)			{ return -1;	}
	//�õ��ڲ�����
	virtual VOID*			GetImpl(VOID)			{ return (VOID*)GetLifeAbilityImpl();	}
	//�õ�����
	virtual LPCTSTR			GetDesc(VOID);
	//�õ���ȴ״��ID
	virtual INT				GetCoolDownID(VOID)		{ return -1; }
	//�õ���������������
	virtual INT				GetPosIndex(VOID);
	//�Ƿ��ܹ��Զ���������
	virtual BOOL			AutoKeepOn(VOID)		{ return FALSE; }
	//�����
	virtual VOID			DoAction(VOID);
	//�Ƿ���Ч
	virtual BOOL			IsValidate(VOID)		{ return TRUE; }
	//ˢ��
	virtual VOID			Update(tActionReference* pRef) {}
	//�����ȴ�Ƿ����
	virtual BOOL			CoolDownIsOver(VOID)	{ return TRUE; }
	//�϶�����
	virtual VOID			NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName);

	VOID			Set_LifeStudy( BOOL isStudy ) {m_bStudy = isStudy; };
	BOOL		Get_LifeStudy(){ return m_bStudy; }

	VOID			Set_LifeLevel( INT nLevel ){ m_iLevel = nLevel; }
	INT			Get_LifeLevel(){ return m_iLevel; }

	VOID			Set_Sclient_lifeability( SCLIENT_LIFEABILITY* pSclient_lifeability ){ m_pSclient_lifeability = pSclient_lifeability; }
	SCLIENT_LIFEABILITY*			Get_Sclient_lifeability() { return m_pSclient_lifeability; };

public:

	CActionItem_LifeAbility(INT nID);
	virtual ~CActionItem_LifeAbility();

	//���������
	VOID	Update_LifeAbility(const SCLIENT_LIFEABILITY* pAbility);

protected:

	//���������
	const SCLIENT_LIFEABILITY*	GetLifeAbilityImpl(VOID);

protected:

	//�����ID
	INT		m_idLifeAbilityImpl;
	BOOL	m_bStudy;
	INT		m_iLevel;
	SCLIENT_LIFEABILITY* m_pSclient_lifeability;
};


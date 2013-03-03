
/** ����������-���춯��
 */

#pragma once


#include "GMActionSystem.h"



struct TALK_ACT_STRUCT;

class CActionItem_ChatMood : public CActionItem
{
public:

	//�õ���������
	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_CHATMOOD;	}
	//�����ַ���
	virtual	LPCTSTR			GetType_String(VOID)	{ return NAMETYPE_CHATMOOD;	}
	//�õ�����ID
	virtual INT				GetDefineID(VOID)		{ return -1; }
	//�õ�����
	virtual INT				GetNum(VOID)			{ return -1; }
	//�õ��ڲ�����
	virtual VOID*			GetImpl(VOID)			{ return (VOID*)GetChatMoodImpl();	}
	//�õ�����
	virtual LPCTSTR			GetDesc(VOID);
	//�õ���ȴ״��ID
	virtual INT				GetCoolDownID(VOID)		{ return -1; }
	//�õ���������������
	//	��������			- �ڼ�������
	virtual INT				GetPosIndex(VOID)		{ return (m_pActStruct)?m_pActStruct->m_actIdx:-1;}
	//�Ƿ��ܹ��Զ���������
	virtual BOOL			AutoKeepOn(VOID)		{ return FALSE; }
	//�����
	virtual VOID			DoAction(VOID);
	//�Ƿ���Ч
	virtual BOOL			IsValidate(VOID) { return TRUE; }
	//ˢ��
	virtual VOID			Update(tActionReference* pRef) { }
	//�����ȴ�Ƿ����
	virtual BOOL			CoolDownIsOver(VOID) { return TRUE; }
	//�϶�����
	virtual VOID			NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName);
	//��ʾtooltips
	virtual VOID			NotifyTooltipsShow(VOID){};
	//����tooltips
	virtual VOID			NotifyTooltipsHide(VOID){};

	//������鶯����ͼ��
	virtual LPCTSTR			GetIconName(VOID) const;

public:

	CActionItem_ChatMood(INT nID);
	virtual ~CActionItem_ChatMood();

	//�������춯������
	VOID	Update_ChatMood(const TALK_ACT_STRUCT* pChatMoodStruct);

protected:

	//�������춯��
	const TALK_ACT_STRUCT*	GetChatMoodImpl(VOID)	{ return m_pActStruct; }

protected:

	//���춯���ṹָ��
	TALK_ACT_STRUCT*		m_pActStruct;

};

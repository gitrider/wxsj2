
/** ����������-�ķ�
 */

#pragma once


#include "GMActionSystem.h"

//// $$
//class CActionItem_XinFa : public CActionItem
//{
//public:
//
//	//�õ���������
//	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_XINFA;	}
//	//�����ַ���
//	virtual	LPCTSTR			GetType_String(VOID)	{ return NAMETYPE_XINFA;	}
//	//�õ�����ID
//	virtual INT				GetDefineID(VOID);
//	//�õ�����
//	virtual INT				GetNum(VOID)			{ return -1;	}
//	//�õ��ڲ�����
//	virtual VOID*			GetImpl(VOID)			{ return (VOID*)GetXinFaImpl();	}
//	//�õ�����
//	virtual LPCTSTR			GetDesc(VOID);
//	//�õ���ȴ״��ID
//	virtual INT				GetCoolDownID(VOID)		{ return -1; }
//	//�õ���������������
//	//	����			- �ڼ�������
//	virtual INT				GetPosIndex(VOID);
//	//�Ƿ��ܹ��Զ���������
//	virtual BOOL			AutoKeepOn(VOID)		{ return FALSE; }
//	//�����
//	virtual VOID			DoAction(VOID)			{ }
//	//�Ƿ���Ч
//	virtual BOOL			IsValidate(VOID) { return TRUE; }
//	//ˢ��
//	virtual VOID			Update(tActionReference* pRef) { }
//	//�����ȴ�Ƿ����
//	virtual BOOL			CoolDownIsOver(VOID) { return TRUE; }
//	//�϶�����
//	virtual VOID			NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName);
//	// �õ��ķ��ȼ�
//	virtual INT				GetXinfaLevel();
//
//public:
//
//	CActionItem_XinFa(INT nID);
//	virtual ~CActionItem_XinFa();
//	
//	//�����ķ�����
////$$	VOID	Update_XinFa(const SCLIENT_XINFA* pXinfaDef);
//
//protected:
//
//	//�����ķ�
//	const SCLIENT_XINFA*	GetXinFaImpl(VOID);
//
//protected:
//
//	//�ķ�ID
//	INT		m_idXinfa;
//
//};
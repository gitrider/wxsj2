
/** GMActionSystem_MouseCmd.h
 *	
 *	���ܣ�	����������-���ָ��
 *			���ڴ������������������Ϊ���ѵ��������״̬
 *
 *	�޸ļ�¼��
 *			080421	CActionItem_MouseCmd_Friend
 *					CActionItem_MouseCmd_Exchange
 *			
 *			
 */

#pragma once


#include "GMActionSystem.h"


//-------------------------------------
//����
class CActionItem_MouseCmd_Repair : public CActionItem
{
public:

	//�õ���������
	virtual ACTION_OPTYPE	GetType(VOID) const	{ return AOT_MOUSECMD_REPARE; }
	//�õ��ڲ�����(DONT USE THIS DATA!)
	virtual VOID*			GetImpl(VOID)		{ return (VOID*)(DWORD_PTR)AOT_MOUSECMD_REPARE;}

protected:

	static CActionItem_MouseCmd_Repair*	s_pMe;

public:

	CActionItem_MouseCmd_Repair(INT id);
	virtual ~CActionItem_MouseCmd_Repair();

	static CActionItem_MouseCmd_Repair*	GetMe(VOID) { return s_pMe; }

};

//-------------------------------------
//����
class CActionItem_MouseCmd_Identify : public CActionItem
{
public:

	//�õ���������
	virtual ACTION_OPTYPE	GetType(VOID) const	{ return AOT_MOUSECMD_IDENTIFY; }
	//�õ��ڲ�����(DONT USE THIS DATA!)
	virtual VOID*			GetImpl(VOID)		{ return (VOID*)(DWORD_PTR)AOT_MOUSECMD_IDENTIFY;}
	//����Default״̬(ֻ��ͨ��ActionSystem����)
	virtual VOID			SetDefaultState(BOOL bDefault);

protected:

	static CActionItem_MouseCmd_Identify*	s_pMe;

public:

	CActionItem_MouseCmd_Identify(INT id);
	virtual ~CActionItem_MouseCmd_Identify();

	static CActionItem_MouseCmd_Identify*	GetMe(VOID) { return s_pMe; }
};


////////////////////////////////////////////////////////////////////////////////////
// CActionItem_ChangeSuit
// һ����װ��ͼ����Ҫ��ק�������
////////////////////////////////////////////////////////////////////////////////////

class CActionItem_ChangeSuit : public CActionItem
{
public:
	CActionItem_ChangeSuit(INT nID);
	virtual ~CActionItem_ChangeSuit();

	//�õ���������
	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_CHANGE_SUIT;	}
	//�����ַ���
	virtual	LPCTSTR			GetType_String(VOID)	{ return NAMETYPE_CHANGE_SUIT; }
	//�õ�����ID
	virtual INT				GetDefineID(VOID);
	//�õ�����
	virtual INT				GetNum(VOID)			{ return -1;	}
	//�õ��ڲ�����
	virtual VOID*			GetImpl(VOID)			{ return (VOID*)&m_idSuit;	}
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
	
	virtual VOID			UpdateSuit(STRING& iconName, INT suitId);
	//�����ȴ�Ƿ����
	virtual BOOL			CoolDownIsOver(VOID)	{ return TRUE; }
	//�϶�����
	virtual VOID			NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName);

	virtual void			SpecialOperation(void* pData);

protected:

	INT		m_idSuit;	// �ڼ�����װ
};


//-------------------------------------
// ��Ӻ����������
class CActionItem_MouseCmd_Friend : public CActionItem
{
public:

	//�õ���������
	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_MOUSECMD_ADDFRIEND; }
	//�õ��ڲ�����(DONT USE THIS DATA!)
	virtual VOID*			GetImpl(VOID)		{ return (VOID*)(DWORD_PTR)AOT_MOUSECMD_ADDFRIEND;}

protected:

	static CActionItem_MouseCmd_Friend*	s_pMe;

public:

	CActionItem_MouseCmd_Friend(INT id);
	virtual ~CActionItem_MouseCmd_Friend();

	static CActionItem_MouseCmd_Friend*	GetMe(VOID) { return s_pMe; }

};

//-------------------------------------
// ����
class CActionItem_MouseCmd_Exchange : public CActionItem
{
public:

	//�õ���������
	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_MOUSECMD_EXCHANGE; }
	//�õ��ڲ�����(DONT USE THIS DATA!)
	virtual VOID*			GetImpl(VOID)		{ return (VOID*)(DWORD_PTR)AOT_MOUSECMD_EXCHANGE;}

protected:

	static CActionItem_MouseCmd_Exchange*	s_pMe;

public:

	CActionItem_MouseCmd_Exchange(INT id);
	virtual ~CActionItem_MouseCmd_Exchange();

	static CActionItem_MouseCmd_Exchange*	GetMe(VOID) { return s_pMe; }
};


//-------------------------------------


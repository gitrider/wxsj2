#pragma once
#include "..\GameCommand.h"
#include "GIMath.h"
#include "GICursorSystem.h"

class CObject;
class tActionItem;

class CCursorMng : public tCursorSystem
{
public:
	virtual  VOID		Initial(VOID *);
	
	//--------------------------------------------------
	//ǿ�����������
	virtual VOID		SetCursor( ENUM_CURSOR_TYPE nType );
	virtual HCURSOR		GetCursor(VOID);
	//��ӦWM_SETCURSOR
	virtual VOID		OnSetCursor(VOID);
	//����UI����ģʽ
	virtual VOID		EnterUICursor(HCURSOR hCursor) { m_hUICursor = hCursor; }
	virtual VOID		LeaveUICursor(VOID) { m_hUICursor = NULL; } 
	//��ʾ/�������
	virtual VOID		ShowCursor(BOOL bShow);
	//ȡ���״̬
	INT	GetCursor_State(VOID);

	//--------------------------------------------------
	//������ϴ洢����
	virtual VOID					MouseCommand_Set(BOOL bHoverInUI, CObject* pObj, const fVector3& fvPos, tActionItem* pActiveSkill);
	//�õ�����
	virtual SCommand_Mouse&			MouseCommand_GetLeft(VOID) { return m_cmdCurrent_Left; }
	virtual SCommand_Mouse&			MouseCommand_GetRight(VOID) { return m_cmdCurrent_Right; }
	//�����������
	virtual VOID					MouseCommand_Active(SCommand_Mouse& cmd);

	//���
	virtual VOID					MouseCommand_Clear(VOID) { 	m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_NULL; }

public:
	CCursorMng(VOID);
	virtual ~CCursorMng(VOID);
	static CCursorMng* GetMe(VOID)	{ return s_pMe; }

protected:
	static CCursorMng*		s_pMe;

protected:
	BOOL		m_bShow;							// �Ƿ���ʾ���
	INT			m_nCursorState;						// ���״̬
	HCURSOR		m_hCursor[ CURSOR_NUMBER ];			// ���
	HCURSOR		m_hUICursor;						//UI���ƹ��

	//����ϴ��������
	SCommand_Mouse	m_cmdCurrent_Left;				// ������ָ��_���
	SCommand_Mouse	m_cmdCurrent_Right;				// ������ָ��_�Ҽ�

	KL_DECLARE_DYNAMIC( CCursorMng );
};

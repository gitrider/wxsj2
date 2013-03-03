#pragma once

#include "GMTripperObj.h"


/**
	���͵�
*/

class CTripperObject_Transport :	public CTripperObject
{
public:
	//Tripper ����
	virtual TRIPPER_OBJECT_TYPE		Tripper_GetType(VOID) const	{ return TOT_TRANSPORT; }
	//�ܷ�������
	virtual BOOL					Tripper_CanOperation(VOID) const { return FALSE; }
	//�������
	virtual ENUM_CURSOR_TYPE		Tripper_GetCursor(VOID) const { return CURSOR_NORMAL; }
	//���뼤��
	virtual VOID					Tripper_Active(VOID) {}

public:
	//-----------------------------------------------------
	///���ݳ�ʼ�����壬��ͬ������Ⱦ��
	virtual	VOID				Initial(VOID*);
	virtual VOID				Release( VOID );
	virtual VOID				SetPosition(const fVector3& fvPosition);

public:
	CTripperObject_Transport();
	virtual ~CTripperObject_Transport();

protected:
	tEntityNode*			m_pRenderInterface2;

protected:
	KL_DECLARE_DYNAMIC(CTripperObject_Transport);
};

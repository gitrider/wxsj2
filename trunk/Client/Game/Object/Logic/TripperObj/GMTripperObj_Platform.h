#pragma once

#include "GMTripperObj.h"
struct _DBC_LIFEABILITY_PLATFORM;

/**
	�����
	����ƽ̨
*/

class CTripperObject_Platform :	public CTripperObject
{
public:
	//Tripper ����
	virtual TRIPPER_OBJECT_TYPE		Tripper_GetType(VOID) const	{ return TOT_PLATFORM; }
	//�ܷ�������
	virtual BOOL					Tripper_CanOperation(VOID) const;
	//�������
	virtual ENUM_CURSOR_TYPE		Tripper_GetCursor(VOID) const { return CURSOR_SPEAK; }
	//���뼤��
	virtual VOID					Tripper_Active(VOID);
	//����ƽ̨������
	BOOL							SetPlatformID(INT nID);
	//ȡ��ƽ̨������
	INT								GetPlatformID(VOID);

public:
	//-----------------------------------------------------
	///���ݳ�ʼ�����壬��ͬ������Ⱦ��
	virtual	VOID				Initial(VOID*);
	virtual VOID				Release( VOID );
	virtual VOID				SetPosition(const fVector3& fvPosition);

public:
	CTripperObject_Platform();
	virtual ~CTripperObject_Platform();

protected:
	tEntityNode*			m_pRenderInterface2;

protected:
	//ƽ̨����
	const _DBC_LIFEABILITY_PLATFORM*	m_pPlatformDef;

protected:
	KL_DECLARE_DYNAMIC(CTripperObject_Platform);
};

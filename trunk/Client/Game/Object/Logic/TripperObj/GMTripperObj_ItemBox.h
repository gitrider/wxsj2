#pragma once

#include "GMTripperObj.h"


/**
	���ڵ��ϵı���
*/

class CTripperObject_ItemBox :	public CTripperObject
{
public:
	//Tripper ����
	virtual TRIPPER_OBJECT_TYPE		Tripper_GetType(VOID) const	{ return TOT_ITEMBOX; }
	//�ܷ�������
	virtual BOOL					Tripper_CanOperation(VOID) const;
	//�������
	virtual ENUM_CURSOR_TYPE		Tripper_GetCursor(VOID) const { return CURSOR_PICKUP; }
	//���뼤��
	virtual VOID					Tripper_Active(VOID);

public:
	//-----------------------------------------------------
	///���ݳ�ʼ�����壬��ͬ������Ⱦ��
	virtual	VOID				Initial(VOID*);
	virtual VOID				Release( VOID );

	//���õ�����Ĺ���
	VOID						SetOwnerGUID(UINT nID) { m_idOwner = nID; }

public:
	CTripperObject_ItemBox();
	virtual ~CTripperObject_ItemBox();

protected:
	UINT			m_idOwner;		//��Ʒ���˵�ObjID,������ӵ�ID

protected:
	KL_DECLARE_DYNAMIC(CTripperObject_ItemBox);
};

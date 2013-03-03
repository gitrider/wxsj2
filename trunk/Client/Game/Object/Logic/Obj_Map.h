
#pragma once


#include "Obj_Phy.h"



class CZone;

/** ���ڵر������壬���������ܳ���������Ҫע�ᵽĳ���ر�������
	�����Զ�������ʱ�ᱻ����
 */
class CObject_Map : public CObject_Phy
{
public:

	/** ����λ�� */
	virtual VOID		SetPosition(const fVector3& fvPosition);
	const iVector2&		GetMapPosition(VOID) const							{ return m_ivGridPos; }

public:

	CObject_Map();
	virtual ~CObject_Map();

protected:

	/// ���������ڵ�����λ��
	iVector2		m_ivGridPos;
	/// ���������ڵ�Zone
	CZone*			m_pZone;

protected:

	KL_DECLARE_DYNAMIC(CObject_Map);

	friend class CZone;

};

#pragma once

#include "GMTripperObj.h"
struct _DBC_LIFEABILITY_GROWPOINT;

/**
	�����_������Դ
*/
//����ܶ���
#define LIFE_ABILITY_MINE		(1)
#define LIFE_ABILITY_HERBS		(8)
#define LIFE_ABILITY_FISH		(9)
#define LIFE_ABILITY_INTERACT	(10)
#define LIFE_ABILITY_SUBDUE		(21)

class CTripperObject_Resource :	public CTripperObject
{
public:
	//Tripper ����
	virtual TRIPPER_OBJECT_TYPE		Tripper_GetType(VOID) const	{ return TOT_RESOURCE; }
	//�ܷ�������
	virtual BOOL					Tripper_CanOperation(VOID) const { return true; }
	//�������
	virtual ENUM_CURSOR_TYPE		Tripper_GetCursor(VOID) const;
	//���뼤��
	virtual VOID					Tripper_Active(VOID);


	// ���ÿ�����ԴID Ref[DBC_LIFEABILITY_GROWPOINT]
	// �����Ƿ��ǺϷ�����Դ
	BOOL							SetResourceID(INT nID);
	// �鿴��Դ������
	INT								GetResourceID(VOID);
	// �Ƿ��ǵ�����Ҫ����Ⱥ
	BOOL							Resource_IsFish(VOID);
	// �ܷ���Կ�����Դ
	BOOL							Resource_CanOperation(VOID);
	// �Ƿ�ɵ���
	BOOL							Resource_CanFish(FLOAT distance);
	// �鿴��Դ����Ҫ�Ĳ�������
	FLOAT							Resource_GetOperationDistance(VOID);

public:
	//-----------------------------------------------------
	///���ݳ�ʼ�����壬��ͬ������Ⱦ��
	virtual	VOID				Initial(VOID*);
	virtual VOID				Release( VOID );

	const _DBC_LIFEABILITY_GROWPOINT*  GetLifeAbility(){ return m_pResourceDef; };

public:
	CTripperObject_Resource();
	virtual ~CTripperObject_Resource();

protected:
	//��Դ����
	const _DBC_LIFEABILITY_GROWPOINT*	m_pResourceDef;

protected:
	KL_DECLARE_DYNAMIC(CTripperObject_Resource);
};

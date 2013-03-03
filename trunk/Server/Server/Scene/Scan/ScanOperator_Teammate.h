/////////////////////////////////////////////////////////////////////////////////
//�ļ�����ScanOperator_Enemy.h
//�����ߣ�frank
//����������������Χ�ĵ���obj
//�޸ļ�¼��2005-11-9����
/////////////////////////////////////////////////////////////////////////////////


#ifndef __SCANOPERATOR_TEAMMATE_H__
#define __SCANOPERATOR_TEAMMATE_H__

#include "Type.h"
#include "ScanOperator.h"
#include "AI_Monster_Def.h"

class Obj_Monster ;

struct SCANOPERATOR_TEAMMATE_INIT : public SCANOPERATOR_INIT
{
	Obj_Monster*				m_pMonster ;
	FLOAT						m_fRadius ;
	INT							m_nCount;
	INT							m_Type;
	BOOL						m_bOnlyNoEnemy; //ֻ��û�е��˵�ͬ��
	BOOL						m_bScanAllMonster; //ɨ���������͵Ĺ�

	SCANOPERATOR_TEAMMATE_INIT( )
	{
		m_pMonster = NULL ;
		m_fRadius = 0.0f ;
		m_nCount = 0;
		m_Type = MONSTERAI_SUMMON_REGION;
		m_bOnlyNoEnemy = TRUE;
		m_bScanAllMonster = FALSE;
	}
};



class ScanOperator_Teammate : public ScanOperator
{
public :
	ScanOperator_Teammate( ) ;
	virtual ~ScanOperator_Teammate( ) ;

	//��ʼ��ɨ�������
	virtual BOOL				Init( SCANOPERATOR_TEAMMATE_INIT* pInit ) ;
	//ɨ�������ʼǰ�ص�
	//	virtual BOOL				On_BeforeScan( ) ;
	//�жϲ����е�Zone�Ƿ���Ҫɨ��
	//	virtual BOOL				On_IsNeedScan( ZoneID_t ZoneID ) ;
	//������һ��Obj, ����ֵ�� SCANRETURN
	virtual UINT				On_FindObj( Obj* pObj ) ;
	//ɨ�������ɺ�ص�
	//	virtual VOID				On_AfterScan( ) ;


public :
	Obj_Monster*				m_pMonster ;
	FLOAT						m_fRadius ;
	INT							m_nCount;
	INT							m_Type;
	BOOL						m_bOnlyNoEnemy; //ֻ��û�е��˵�ͬ��
	BOOL						m_bScanAllMonster; //ɨ���������͵Ĺ�

};




#endif

/////////////////////////////////////////////////////////////////////////////////
//�ļ�����ScanOperator_Enemy.h
//�����ߣ�jack
//����������������Χ�ĵ���obj
//�޸ļ�¼��2005-11-8����
/////////////////////////////////////////////////////////////////////////////////


#ifndef __SCANOPERATOR_ACTIVETEAMMATES_H__
#define __SCANOPERATOR_ACTIVETEAMMATES_H__

#include "Type.h"
#include "ScanOperator.h"

class Obj_Human ;

struct SCANOPERATOR_ACTIVETEAMMATES_INIT : public SCANOPERATOR_INIT
{
	WORLD_POS					m_Position ;
	TeamID_t					m_TeamID ;
	GUID_t						m_MemberGUID ;
	FLOAT						m_fRadius ;

	SCANOPERATOR_ACTIVETEAMMATES_INIT( )
	{
		m_TeamID = INVALID_ID ;
		m_MemberGUID = INVALID_ID ;
		m_fRadius = 0.0f ;
	}
};



class ScanOperator_ActiveTeammates : public ScanOperator
{
public :
	ScanOperator_ActiveTeammates( ) ;
	virtual ~ScanOperator_ActiveTeammates( ) ;

	//��ʼ��ɨ�������
	virtual BOOL				Init( SCANOPERATOR_ACTIVETEAMMATES_INIT* pInit ) ;
	//ɨ�������ʼǰ�ص�
//	virtual BOOL				On_BeforeScan( ) ;
	//�жϲ����е�Zone�Ƿ���Ҫɨ��
//	virtual BOOL				On_IsNeedScan( ZoneID_t ZoneID ) ;
	//������һ��Obj, ����ֵ�� SCANRETURN
	virtual UINT				On_FindObj( Obj* pObj ) ;
	//ɨ�������ɺ�ص�
//	virtual VOID				On_AfterScan( ) ;


public :
	WORLD_POS					m_Position ;
	TeamID_t					m_TeamID ;
	GUID_t						m_MemberGUID ;
	FLOAT						m_fRadius ;

	Obj_Human*					m_aActiveTeammate[MAX_TEAM_MEMBER] ;
	INT							m_nActiveTeammateCount ;

};




#endif

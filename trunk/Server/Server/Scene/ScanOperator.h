
/////////////////////////////////////////////////////////////////////////////////
//�ļ�����ScanOperator.h
//�����ߣ�jack
//�����������ṩ������obj����human�����Ķ��ƹ���
//�޸ļ�¼��2005-11-8 ����
/////////////////////////////////////////////////////////////////////////////////

#ifndef __SCANOPERATOR_H__
#define __SCANOPERATOR_H__


#include "Type.h"


enum SCANRETURN
{
	SCANRETURN_CONTINUE = 0 ,//����ɨ����һ��obj
	SCANRETURN_BREAK ,//ֹͣ�ڵ�ǰZone���ɨ�裬ת����һ��Zone��ȥ
	SCANRETURN_RETURN ,//�ж�ɨ�����

	SCANRETURN_NUMBER ,
};


class Scene ;
class Obj ;

struct SCANOPERATOR_INIT
{
	//����ָ��
	Scene*						m_pScene ;
	//����������ZoneID
	ZoneID_t					m_ZoneID ;
	//�����İ뾶����Zone����Ϊ��λ��
	INT							m_nZoneRadius ;
	//�Ƿ�����HumanList
	//TRUE����HumanList,FALSE����ObjList
	BOOL						m_bScanHuman ;

	SCANOPERATOR_INIT( )
	{
		m_pScene = NULL ;
		m_ZoneID = INVALID_ID ;
		m_nZoneRadius = 0 ;
		m_bScanHuman = FALSE ;
	};
};

class ScanOperator
{
friend class Scene ;

public :
	ScanOperator( ) ;
	virtual ~ScanOperator( ) ;

	//��ʼ��ɨ�������
	virtual BOOL				Init( SCANOPERATOR_INIT* pInit ) ;
	//ɨ�������ʼǰ�ص�
	virtual BOOL				On_BeforeScan( ){ return TRUE ; }
	//�жϲ����е�Zone�Ƿ���Ҫɨ��
	virtual BOOL				On_IsNeedScan( ZoneID_t ZoneID ){ return TRUE ; }
	//������һ��Obj, ����ֵ�� SCANRETURN
	virtual UINT				On_FindObj( Obj* pObj ){ return SCANRETURN_CONTINUE ; }
	//ɨ�������ɺ�ص�
	virtual VOID				On_AfterScan( ){ ; } ;

	//��ȡ����ָ��
	Scene*						GetScene( ){ return m_pScene ; }

protected :
	//����ָ��
	Scene*						m_pScene ;
	//����������ZoneID
	ZoneID_t					m_ZoneID ;
	//�����İ뾶����Zone����Ϊ��λ��
	INT							m_nZoneRadius ;
	//�Ƿ�����HumanList
	//TRUE����HumanList,FALSE����ObjList
	BOOL						m_bScanHuman ;
};











#endif

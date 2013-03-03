/////////////////////////////////////////////////////////////////////////////////
//�ļ�����City.h
//�����������������ݺ��߼�����
//�޸ļ�¼��2005-11-28 ���� 
//			2006-3-11	
/////////////////////////////////////////////////////////////////////////////////


#ifndef __CITY_H__
#define __CITY_H__


#include "Type.h"


class City
{

public :
	//-------------------------------------------------------------------------------------------------
	//��������ʱ�ṹ
	struct CityRT_t
	{
		SceneID_t		m_SceneID;			//���еĳ���ID, ����ʱ��̬����
		ID_t			m_ServerID;			//��������ServerID, ����ʱ��̬����
		BOOL			m_bLock;			//����ʱ�������ⲻͬ�������Ĳ���
		GUID_t			m_ApplicantGuid;	//�����ߵ�guid��ʱ��¼������������ʹ��
	};

public :
	//-------------------------------------------------------------------------------------------------
	//�ڴ�ķ��䣬�ͷ�
	City( ) ;
	~City( ) ;

	//-------------------------------------------------------------------------------------------------
	//��ʼ��,��մ˳��У���������ʱ�ṹ����ʵ���ݽṹ
	BOOL		Init( CityDB_t* pCityDB ) ;
	VOID		CleanUp( ) ;

public:
	//-------------------------------------------------------------------------------------------------
	//���е�ΨһID
	_CITY_GUID	GetGuid(){return m_pCityDB->m_Guid;}
	VOID		SetGuid(_CITY_GUID CityGuid)
	{
		m_pCityDB->m_Guid	= CityGuid;
	}

	//-------------------------------------------------------------------------------------------------
	//���е���ڳ���ID
	SceneID_t	GetPortSceneID(){return m_pCityDB->m_PortSceneID;}
	VOID		SetPortSceneID(SceneID_t PortScene){m_pCityDB->m_PortSceneID = PortScene;}

	//-------------------------------------------------------------------------------------------------
	//���еļ���
	BYTE		GetCityLevel(){return m_pCityDB->m_Level;}
	VOID		SetCityLevel(BYTE SceneLevel){m_pCityDB->m_Level = SceneLevel;}

	//-------------------------------------------------------------------------------------------------
	//����������ĳ���ID
	SceneID_t	GetSceneID(){return m_CityRT.m_SceneID;}
	VOID		SetSceneID(SceneID_t SceneID){m_CityRT.m_SceneID = SceneID;}

	//-------------------------------------------------------------------------------------------------
	//���������䵽��ServerID
	ID_t		GetServerID(){return m_CityRT.m_ServerID;}
	VOID		SetServerID(ID_t ServerID){m_CityRT.m_ServerID = ServerID;}

	//-------------------------------------------------------------------------------------------------
	//�˳����Ƿ��Ѿ���ס
	BOOL		IsLock(){return m_CityRT.m_bLock;}
	VOID		SetLock(BOOL bLock){m_CityRT.m_bLock = bLock;}

	//-------------------------------------------------------------------------------------------------
	//��������ʱGuid
	GUID_t		GetApplicantGuid(){return m_CityRT.m_ApplicantGuid;}
	VOID		SetApplicantGuid(GUID_t	ApplicantGuid){m_CityRT.m_ApplicantGuid = ApplicantGuid;}

	//-------------------------------------------------------------------------------------------------
	//������
	CHAR*		GetCityName(){return m_pCityDB->m_szCityName;}
	VOID		SetCityName(CHAR* pszCityName)
	{
		memcpy(m_pCityDB->m_szCityName, pszCityName, MAX_CITY_NAME_SIZE);
	}

	//-------------------------------------------------------------------------------------------------
	//���ID
	GuildID_t	GetGuildID(){return m_pCityDB->m_GuildID;}
	VOID		SetGuildID(GuildID_t nGuildID)
	{
		m_pCityDB->m_GuildID = nGuildID;
	}

	//-------------------------------------------------------------------------------------------------
	//���뽨����Ϣ
	VOID		LoadBuildingInfo(SceneID_t nSceneID);

	//-------------------------------------------------------------------------------------------------
	//���DB
	CityDB_t*	GetCityDB() const {return m_pCityDB;}

protected:
	//-------------------------------------------------------------------------------------------------
	//��Ҫ�洢���ݿ����Ϣ
	CityDB_t*		m_pCityDB;
	//-------------------------------------------------------------------------------------------------
	//����ʱ��Ϣ
	CityRT_t		m_CityRT;
};

#endif

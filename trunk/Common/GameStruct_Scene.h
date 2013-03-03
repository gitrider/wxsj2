/////////////////////////////////////////////////////////////////////////////////
//�ļ�����GameStruct_Scene.h
//�����ߣ�jack
//�������������峡���еĳ�ʼ����Ϣ���ݽṹ
//�޸ļ�¼��2005��11��10����
/////////////////////////////////////////////////////////////////////////////////
#include "GameUtil.h"
#include "GameStruct_City.h"

#ifndef __GAMESTRUCT_SCENE_H__
#define __GAMESTRUCT_SCENE_H__

//Load�����е���Ϣ���У�������ͼ�����NPC���ֲ�������̨�ֲ�����������Ϣ
//Ѳ��·����Ϣ���¼�������Ϣ������ֲ���Ϣ
struct SCENE_LOAD
{
	CHAR		m_szMap[_MAX_PATH] ;
	CHAR		m_szMonster[_MAX_PATH] ;
	CHAR		m_szPlatform[_MAX_PATH] ;
	CHAR		m_szGrowPointData[_MAX_PATH] ;
	CHAR		m_szGrowPointSetup[_MAX_PATH] ;
	CHAR		m_szPatrolPointData[_MAX_PATH] ;
	CHAR		m_szArea[_MAX_PATH] ;
	CHAR		m_szPet[_MAX_PATH] ;
	CHAR		m_szStallInfo[_MAX_PATH] ;
	CHAR		m_szBus[_MAX_PATH];
	BOOL		m_IsPlatformExist;
	BOOL		m_IsGrowPointExist;
	BOOL		m_IsStallInfoExist;
	BOOL		m_IsBusExist;


	SCENE_LOAD( )
	{
		CleanUp( ) ;
	};
	VOID		CleanUp( ) 
	{
		memset( this, 0, sizeof(SCENE_LOAD) ) ;
	}
};

//��ʼ�����������ݣ����磺�����еĽ�����Ϣ�������е�������Ϣ�������еļҾ���Ϣ
//ͥԺ�еĲ�����Ϣ��

struct SCENE_INIT_DATA
{
	INT					m_nDataType;//�������� ��SCENE_TYPE��
	union
	{
		CityDB_t			m_CityData;//���г�ʼ������
	};
	
	SCENE_INIT_DATA( )
	{
		CleanUp( );
	}
	VOID		CleanUp( )
	{
		if(m_nDataType == SCENE_TYPE_CIT)
		{
			m_CityData.CleanUp();
		}
		m_nDataType = SCENE_TYPE_GAMELOGIC ;
	}
};

//Ϊ�˴���������Ҫ���������
//
#define MAX_COPY_MEMBER 30
#define MAX_COPYSCENE_DATA_PARAM 32
struct COPYSCENE_DATA
{
	SceneID_t				m_SourSceneID ;//���������ĳ���
	GUID_t					m_TeamLeader;//�����Ĵ�����
	TeamID_t				m_TeamID;//����ID
	UINT					m_NoUserCloseTime;//������û���˺��Զ��رյ�ʱ��
	UINT					m_SceneTimer;//�����ж�ʱ���ļ��ʱ��,0��ʾû�ж�ʱ��
	INT						m_Param[MAX_COPYSCENE_DATA_PARAM] ;
	GUID_t					m_MemberGUID[MAX_COPY_MEMBER] ;
	
	COPYSCENE_DATA( )
	{
		CleanUp() ;
	}
	VOID CleanUp()
	{
		m_SourSceneID=INVALID_ID ;
		m_TeamLeader=INVALID_ID ;
		m_TeamID=INVALID_ID ;
		m_NoUserCloseTime=10000 ;
		m_SceneTimer=0 ;
		memset( m_Param, 0, sizeof(INT)*MAX_COPYSCENE_DATA_PARAM ) ;
		for( INT i=0; i<MAX_COPY_MEMBER; i++ )
		{
			m_MemberGUID[i]=INVALID_ID ;
		}
	}
	VOID AddMemberGUID(GUID_t guid )
	{
		if( IsHaveMember(guid) )
			return ;

		for( INT i=0; i<MAX_COPY_MEMBER; i++ )
		{
			if( m_MemberGUID[i]==INVALID_ID )
			{
				m_MemberGUID[i] = guid ;
				return;
			}
		}
	}
	BOOL IsHaveMember( GUID_t guid )
	{
		for( INT i=0; i<MAX_COPY_MEMBER; i++ )
		{
			if( m_MemberGUID[i]==guid )
				return TRUE ;
		}
		return FALSE ;
	}
};


//���ڴ˳����ϵĳ�������Ҫ����Ϣ
struct	CITYSCENE_DATA
{
	SceneID_t	m_ScenePortList[MAX_CITY_PORT];
	GUID_t		m_Creator;
	CMyTimer	m_DynamicSceneQuitTimer;
	BOOL		m_bClose;

	CITYSCENE_DATA()
	{
		CleanUp();
	}
	BOOL AddPort(SceneID_t ScenePort)
	{
		for(UINT i = 0; i<MAX_CITY_PORT; i++)
		{
			if(m_ScenePortList[i] == INVALID_ID)
			{
				m_ScenePortList[i] = ScenePort;
				return TRUE;
			}
		}
		return FALSE;
	}

	BOOL RemovePort(SceneID_t ScenePort)
	{
		for(UINT i = 0; i<MAX_CITY_PORT; i++)
		{
			if(m_ScenePortList[i] == ScenePort)
			{
				m_ScenePortList[i] = INVALID_ID;
				return TRUE;
			}
		}
		return FALSE;
	}

	INT	GetPortNum()
	{
		INT a= 0;
		for(UINT i = 0; i<MAX_CITY_PORT; i++)
		{
			if(m_ScenePortList[i] != INVALID_ID)
			{
				a++;			
			}
		}
		return a;
	}

	VOID CleanUp()
	{
		for(UINT i = 0; i<MAX_CITY_PORT; i++)
		{
			m_ScenePortList[i] = INVALID_ID;
		}
		m_Creator	=	INVALID_GUID;
		m_DynamicSceneQuitTimer.CleanUp();
		m_bClose	=	FALSE;
	}
};

#endif


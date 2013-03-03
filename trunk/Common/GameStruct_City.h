/********************************************************************************
 *	�ļ�����	GameStruct_City.h
 *	ȫ·����	d:\Prj\Common\GameStruct_City.h
 *	�����ˣ�	����
 *	����ʱ�䣺	2006-3-11
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef __GAMESTRUCT_CITY_H__
#define __GAMESTRUCT_CITY_H__

#include "Type.h"


//���е�ΨһID
struct _CITY_GUID
{
	ID_t		m_World ;		//�����: 
	SHORT		m_PoolPos ;		//���ݳ�λ��

	BOOL	operator ==(_CITY_GUID& Ref)	const
	{
		return (Ref.m_PoolPos==m_PoolPos)&&(Ref.m_World==m_World);
	}

	BOOL	isNull() const
	{
		return (m_World ==INVALID_ID)&&(m_PoolPos==-1);
	}

	VOID	Reset()
	{
		m_PoolPos	= -1;
		m_World	= INVALID_ID;	
	}
};

//��������
struct CityBuilding_t
{
	INT		m_BuildingID;
	BYTE	m_BuildingLevel;
	VOID	CleanUp()
	{
		m_BuildingID	= INVALID_ID;
		m_BuildingLevel = 0;
	}
};

//���д����ݿ�ṹ
struct CityDB_t
{
	_CITY_GUID		m_Guid;										//һ�����е�Ψһ��ʶ
	SceneID_t		m_PortSceneID;								//������ڳ���ID
	GuildID_t		m_GuildID;									//���ID
	BYTE			m_Level;									//���м���
	CHAR			m_szCityName[MAX_CITY_NAME_SIZE];			//��������
	BYTE			m_BuildingNum;								//����������
	CityBuilding_t	m_Builds[MAX_CITY_BUILDING_NUM];			//����������
	//...
	//...
	VOID	CleanUp()
	{
		m_Guid.Reset();
		m_PortSceneID	=	INVALID_ID;
		m_GuildID		=	INVALID_ID;
		m_Level			=	0;
		memset(m_szCityName, 0, MAX_CITY_NAME_SIZE);
		m_BuildingNum	=	0;
		for(INT i = 0; i<MAX_CITY_BUILDING_NUM; i++)
		{
			m_Builds[i].CleanUp();
		}
	}
};

//���д����ݿ�ṹ
struct CityManagerDB_t
{
	//-------------------------------------------------------------------------------------------------
	//����λ�ñ��
	UINT			m_nFreePosition;
	//-------------------------------------------------------------------------------------------------
	//���г�Id
	INT				m_pFreeCities[MAX_CITY_PER_WORLD];
	//-------------------------------------------------------------------------------------------------
	//�����������
	CityDB_t		m_CityList[MAX_CITY_PER_WORLD];

	CityManagerDB_t()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		m_nFreePosition = 0;
		for(INT i = 0; i<MAX_CITY_PER_WORLD; i++)
		{
			m_pFreeCities[i] = i;
			m_CityList[i].CleanUp();
		}
	}

};

#endif // __GAMESTRUCT_CITY_H__

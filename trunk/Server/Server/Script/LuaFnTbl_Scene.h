
//-----------------------------------------------------------------------------
// �ļ��� : LuaFnTbl_Scene.h
// ģ��	:	Script
// ����	 :  ������ص�������
// �޸���ʷ:
//	jack����
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_SCENE_H__
#define __LUAFNTBL_SCENE_H__

#include "LuaFnMacro.h"
#include "Scene.h"
#include "SceneManager.h"
#include "CopySceneManager.h"
#include "GameTable.h"

namespace LuaFnTbl
{
/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnGetSceneType
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*				 ���ص�ǰ���������ͣ������Config/SceneInfo.ini ��Type�ֶ�����
	*/
	INT LuaFnGetSceneType( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
	
		sceneId = Lua_ValueToNumber(L,1);

		{
			Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	
			AssertEx(pScene,"����ID���� LuaFnGetSceneType");	
			if(pScene) 
			{
				//Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
				INT nType = pScene->GetSceneType() ;
				Lua_PushNumber(L,nType);
				return 1;
			}
		}

		LUA_LEAVE_FUNCTION
		Lua_PushNumber(L,SCENE_TYPE_INVALID);
		return 1 ;
	}

/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnCreateCopyScene
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*				 ����ֵ���������-1����ʾ����ʧ�ܣ����򷵻ش����ɹ��ĳ�����
	*				 �����Ѿ����úõ����ݴ�������, ����ѡ������ò�����
	*				 ����*SetSceneLoad_*����*SetCopySceneData_*�����͵ĺ���
	*/
	INT LuaFnCreateCopyScene( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
	
		sceneId = Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnCreateCopyScene")
			COPYSCENE_SELECT SelectScene ;
			SelectScene.m_Load = pScene->m_SceneLoad ;
			SelectScene.m_SceneDataType = 0 ;
			SelectScene.m_CopyData = pScene->m_CopyData ;
			SelectScene.m_CopyData.m_SourSceneID = sceneId ;//����Ҫ���ã��Զ����

			INT nRet = g_pCopySceneManager->SelectScene( &SelectScene ) ;
			Lua_PushNumber(L,nRet);
			return 1 ;

		ENDSCENEDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : �¾�
	*     ��дʱ�� : 2010-07-21
	*     �������� : LuaFnGetSceneStatue
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*				 �õ�������״̬
	*/
	INT LuaFnGetSceneStatue( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;

		sceneId = Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnGetSceneStatue")
		INT nStaus = pScene->GetSceneStatus();
		Lua_PushNumber(L,nStaus);
		return 1 ;

		ENDSCENEDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}


/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetSceneLoad_Map
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 mapName
	*					����ͼ�ļ���,�ַ�������
	*     ����˵�� :
	*				 ���ø�������ѡ����ĵ���ͼ�ļ�������"*.nav", ����Ҫ��·��
	*				 ע�⣺�ڴ�������ʱ�򣬱������õ�ͼ�ļ��������ң��˵�ͼ�Ѿ�
	*					   ���õ�SceneInfo.ini��
	*/
	INT LuaFnSetSceneLoad_Map( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		const CHAR* mapName;
	
		sceneId = Lua_ValueToNumber(L,1);
		mapName = Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnSetSceneLoad_Map")
			pScene->SetSceneLoad_Map( mapName ) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetSceneLoad_Monster
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 monsterName
	*					����ֲ��ļ���,�ַ�������
	*     ����˵�� :
	*				 ���ø�������ѡ����Ĺ���ֲ��ļ�,����"*_monster.ini",
	*				 ����Ҫ��·��
	*/
	INT LuaFnSetSceneLoad_Monster( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		const CHAR* monsterName;
	
		sceneId = Lua_ValueToNumber(L,1);
		monsterName = Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnSetSceneLoad_Monster")
			pScene->SetSceneLoad_Monster( monsterName ) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetSceneLoad_Platform
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 platformName
	*					����̨�ֲ��ļ���,�ַ�������
	*     ����˵�� :
	*				 ���ø�������ѡ����Ĳ���̨�ֲ��ļ�,����"*_platform.ini",
	*				 ����Ҫ��·��
	*/
	INT LuaFnSetSceneLoad_Platform( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		const CHAR* platformName;
	
		sceneId = Lua_ValueToNumber(L,1);
		platformName = Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnSetSceneLoad_Platform")
			pScene->SetSceneLoad_Platform( platformName ) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetSceneLoad_GrowPointData
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 growpointdataName
	*					������ֲ��ļ���,�ַ�������
	*     ����˵�� :
	*				 ���ø�������ѡ�����������ֲ��ļ�,����"*_growpoint.ini",
	*				 ����Ҫ��·��
	*/
	INT LuaFnSetSceneLoad_GrowPointData( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		const CHAR* growpointdataName;
	
		sceneId = Lua_ValueToNumber(L,1);
		growpointdataName = Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnSetSceneLoad_GrowPointData")
			pScene->SetSceneLoad_GrowPointData( growpointdataName ) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetSceneLoad_GrowPointSetup
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 growpointsetupName
	*					���������������ɵ���Ʒ�ֲ��ļ���,�ַ�������
	*     ����˵�� :
	*				 ���ø�������ѡ����Ĵ��������������ɵ���Ʒ�ֲ��ļ�,����"*_growpointsetup.ini",
	*				 ����Ҫ��·��
	*/
	INT LuaFnSetSceneLoad_GrowPointSetup( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		const CHAR* growpointsetupName;
	
		sceneId = Lua_ValueToNumber(L,1);
		growpointsetupName = Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnSetSceneLoad_GrowPointSetup")
			pScene->SetSceneLoad_GrowPointData( growpointsetupName ) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetSceneLoad_Area
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 areaName
	*					�¼����ֲ��ļ���,�ַ�������
	*     ����˵�� :
	*				 ���ø�������ѡ������¼����ֲ��ļ�,����"*_area.ini",
	*				 ����Ҫ��·��
	*/
	INT LuaFnSetSceneLoad_Area( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		const CHAR* areaName;
	
		sceneId = Lua_ValueToNumber(L,1);
		areaName = Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnSetSceneLoad_Area")
			pScene->SetSceneLoad_GrowPointData( areaName ) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetSceneLoad_Pet
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 petName
	*					����ֲ��ļ���,�ַ�������
	*     ����˵�� :
	*				 ���ø�������ѡ����ĳ���ֲ��ļ�,����"*_pet.ini",
	*				 ����Ҫ��·��
	*/
	INT LuaFnSetSceneLoad_Pet( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		const CHAR* petName;
	
		sceneId = Lua_ValueToNumber(L,1);
		petName = Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnSetSceneLoad_Pet")
			pScene->SetSceneLoad_GrowPointData( petName ) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetSceneLoad_PatrolPointData
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 patrolpointdataName
	*					Ѳ��·���ֲ��ļ���,�ַ�������
	*     ����˵�� :
	*				 ���ø�������ѡ�����Ѳ��·���ֲ��ļ�,����"*_patrolpoint.ini",
	*				 ����Ҫ��·��
	*/
	INT LuaFnSetSceneLoad_PatrolPointData( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		const CHAR* patrolpointdataName;
	
		sceneId = Lua_ValueToNumber(L,1);
		patrolpointdataName = Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnSetSceneLoad_PatrolPointData")
			pScene->SetSceneLoad_GrowPointData( patrolpointdataName ) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetCopySceneData_TeamLeader
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 guidLeader
	*					�ӳ���GUID,uint
	*     ����˵�� :
	*				 ���ø�������������Ķӳ�GUID
	*/
	INT LuaFnSetCopySceneData_TeamLeader( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		GUID_t guidLeader;
	
		sceneId = Lua_ValueToNumber(L,1);
		guidLeader = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnSetCopySceneData_TeamLeader")
			pScene->SetCopySceneData_TeamLeader( guidLeader ) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetCopySceneData_NoUserCloseTime
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 closeTime
	*					ʱ�䣨���룩,uint
	*     ����˵�� :
	*				 ���ø�������������ģ�û���˺󸱱������Զ��ر������ʱ�䡣
	*/
	INT LuaFnSetCopySceneData_NoUserCloseTime( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		UINT closeTime;
	
		sceneId = Lua_ValueToNumber(L,1);
		closeTime = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnSetCopySceneData_NoUserCloseTime")
			pScene->SetCopySceneData_NoUserCloseTime( closeTime ) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetCopySceneData_Timer
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 uTime
	*					ʱ�䣨���룩,uint
	*     ����˵�� :
	*				 ���ø�������������ģ�������ʱ����ʱ���������Ϊ0��ʾû�����á�
	*/
	INT LuaFnSetCopySceneData_Timer( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		UINT uTime;
	
		sceneId = Lua_ValueToNumber(L,1);
		uTime = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnSetCopySceneData_Timer")
			pScene->SetCopySceneData_Timer( uTime ) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnSetCopySceneData_Param
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 uIndex
	*					����,uint �����Ե�32
	*				 nValue
	*					ֵ,uint
	*     ����˵�� :
	*				 ���ø�������������ģ�����������Ϣ
	*/
	INT LuaFnSetCopySceneData_Param( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		UINT uIndex;
		INT nValue;
	
		sceneId = Lua_ValueToNumber(L,1);
		uIndex = Lua_ValueToNumber(L,2);
		nValue = Lua_ValueToNumber(L,3);

		Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	
		AssertEx(pScene,"����ID���� LuaFnSetCopySceneData_Param");	
		if(pScene) 
		{
			//Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
			pScene->SetCopySceneData_Param( uIndex, nValue ) ;
		}

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnGetCopySceneData_TeamLeader
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*				 ���ظ�������������Ķӳ�GUID��ʧ�ܣ�1 
	*/
	INT LuaFnGetCopySceneData_TeamLeader( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
	
		sceneId = Lua_ValueToNumber(L,1);

		Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	
		AssertEx(pScene,"����ID���� LuaFnGetCopySceneData_TeamLeader");	
		if(pScene) 
		{
			//Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
			GUID_t guidLeader = pScene->GetCopySceneData_TeamLeader( ) ;
			Lua_PushNumber(L,guidLeader);
			return 1;
		}

		LUA_LEAVE_FUNCTION
		Lua_PushNumber(L,INVALID_ID);
		return 1 ;
	}

/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnGetCopySceneData_NoUserCloseTime
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*				 ����û���˺󸱱������Զ��ر������ʱ�䣨���룩��
	*/
	INT LuaFnGetCopySceneData_NoUserCloseTime( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
	
		sceneId = Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnGetCopySceneData_NoUserCloseTime")
			UINT closeTime = pScene->GetCopySceneData_NoUserCloseTime( ) ;
			Lua_PushNumber(L,closeTime);
			return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnGetCopySceneData_Timer
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*				 ��ȡ������ʱ����ʱ���������Ϊ0��ʾû�����á�
	*/
	INT LuaFnGetCopySceneData_Timer( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
	
		sceneId = Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnGetCopySceneData_Timer")
			UINT uTime = pScene->GetCopySceneData_Timer( ) ;
			Lua_PushNumber(L,uTime);
			return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-10-16
	*     �������� : LuaFnGetCopySceneData_Param
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 uIndex
	*					����,uint �����Ե�32
	*     ����˵�� :
	*				 ��ȡ����������Ϣ
	*/
	INT LuaFnGetCopySceneData_Param( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		UINT uIndex;
	
		sceneId = Lua_ValueToNumber(L,1);
		uIndex = Lua_ValueToNumber(L,2);
		{
			Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	
			AssertEx(pScene,"����ID���� LuaFnGetCopySceneData_Param");	
			if(pScene) 
			{
				//Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
				INT nValue = pScene->GetCopySceneData_Param( uIndex ) ;
				Lua_PushNumber(L,nValue);
				return 1;
			}
		}

		LUA_LEAVE_FUNCTION
		Lua_PushNumber(L,0);
		return 1;
	}

/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-11-18
	*     �������� : LuaFnGetCopyScene_HumanCount
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*				 ���ص�ǰ����������������
	*/
	INT LuaFnGetCopyScene_HumanCount( Lua_State* L )	
	{
		LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  
		sceneId = Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnGetCopyScene_HumanCount")
			INT nHumanCount = pScene->GetCopyScene_HumanCount() ;
			Lua_PushNumber(L,nHumanCount);
			return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		Lua_PushNumber(L,0);
		return 1;
	}

/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-11-18
	*     �������� : LuaFnGetCopyScene_HumanObjId
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 uIndex
	*					����,uint �����Ե�32
	*     ����˵�� :
	*				 ��ȡ����������Ϣ
	*/
	INT LuaFnGetCopyScene_HumanObjId( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		UINT uIndex;
	
		sceneId = Lua_ValueToNumber(L,1);
		uIndex = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetCopyScene_HumanObjId")
			ObjID_t HumanObjId = pScene->GetCopyScene_HumanObjID( uIndex ) ;
			Lua_PushNumber(L,HumanObjId);
			return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-11-24
	*     �������� : LuaFnIsObjValid
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 objId
	*					objId ��
	*     ����˵�� :
	*				 �ж��Ƿ�ӵ��objId�ŵ�Obj
	*/
	INT LuaFnIsObjValid( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t objId;
	
		sceneId = Lua_ValueToNumber(L,1);
		objId = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnIsObjValid")
			Obj* pObj = pScene->GetObjManager()->GetObj(objId) ;
			if( pObj==NULL )
			{
				Lua_PushNumber(L,FALSE);
			}
			else
			{
				Lua_PushNumber(L,TRUE);
			}
			return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2005-11-24
	*     �������� : LuaFnIsCharacterLiving
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 objId
	*					objId ��
	*     ����˵�� :
	*				 �ж�objId�ŵ�Obj�Ƿ��ǻ��
	*/
	INT LuaFnIsCharacterLiving( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t objId;
	
		sceneId = Lua_ValueToNumber(L,1);
		objId = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnIsCharacterLiving")
			Obj* pObj = pScene->GetObjManager()->GetObj(objId) ;
			if( pObj && IsCharacterObj(pObj->GetObjType()) )
			{
				Obj_Character* pCharacter = (Obj_Character*)pObj ;
				if( pCharacter->IsActiveObj() && pCharacter->IsAlive() )
				{
					Lua_PushNumber(L,TRUE);
				}
				else
				{
					Lua_PushNumber(L,FALSE);
				}
			}
			else
			{
				Lua_PushNumber(L,FALSE);
			}
			return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-2-28	10:28
	 *	�������ƣ�	LuaFnSendNormalMail
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				objId
	 *					������ ID���޷�������
	 *				szName
	 *					�ռ�������
	 *				szMail
	 *					�ʼ�����
	 *
	 *	����˵����	������ͨ�ʼ�
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnSendNormalMail( Lua_State* L )
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t objId;
	  const CHAR* szName;
	  const CHAR* szMail;

		sceneId = Lua_ValueToNumber(L, 1);
		objId = Lua_ValueToNumber(L, 2);
		szName = Lua_ValueToString(L, 3);
		szMail = Lua_ValueToString(L, 4);

		BEGINSCENEDEFINE("LuaFnSendNormalMail")
			Assert( szName );
			Assert( szMail );

			Obj_Human* pHuman = pScene->GetHumanManager()->GetHuman( objId );
			if( pHuman == NULL )
			{
				Assert( pHuman );
				return 0;
			}

			pScene->SendNormalMail( pHuman, szName, szMail );

		ENDSCENEDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-2-28	13:30
	 *	�������ƣ�	LuaFnSendScriptMail
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				szName
	 *					�ռ�������
	 *				uParam0 ~ uParam3
	 *					��ִ���ʼ��Դ��� 4 ������
	 *
	 *	����˵����	���Ϳ�ִ���ʼ�
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnSendScriptMail( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  const CHAR* szName;
	  UINT uParam0;
	  UINT uParam1;
	  UINT uParam2;
	  UINT uParam3 ;
	  

		sceneId = Lua_ValueToNumber(L, 1);
		szName = Lua_ValueToString(L, 2);
		uParam0 = Lua_ValueToNumber(L, 3);
		uParam1 = Lua_ValueToNumber(L, 4);
		uParam2 = Lua_ValueToNumber(L, 5);
		uParam3 = Lua_ValueToNumber(L, 6);

		BEGINSCENEDEFINE("LuaFnSendScriptMail")
			Assert( szName );

			pScene->SendScriptMail( szName, uParam0, uParam1, uParam2, uParam3 );

		ENDSCENEDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-4	17:46
	 *	�������ƣ�	LuaFnSendMailToAllFriend
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				szMail
	 *					�ʼ����� 
	 *				SpouseId����żID��Ĭ��Ϊʱ��ʾ������ż
	 *
	 *	����˵����	�����к��ѷ�����ͨ�ʼ�
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnSendMailToAllFriend( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t objId;
	  const CHAR* szMail;
	  ObjID_t SpouseId;

		sceneId = Lua_ValueToNumber(L, 1);
		objId = Lua_ValueToNumber(L, 2);
		szMail = Lua_ValueToString(L, 3);
		SpouseId =  Lua_ValueToNumber(L, 4);


		BEGINSCENEDEFINE("LuaFnSendMailToAllFriend")
			Assert( szMail );

			Obj_Human* pHuman = pScene->GetHumanManager()->GetHuman( objId );
			if( pHuman == NULL )
			{
				Assert( pHuman );
				return 0;
			}
			
			if ( pHuman->GetID() != SpouseId )
				pHuman->GetHumanRelation()->SendMailToAllFriend(szMail);

		ENDSCENEDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-3-23
	*     �������� : LuaFnGetMonsterCount
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*				 ȡ�ô˳����еĹ�������
	*/
	INT LuaFnGetMonsterCount( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
	
		sceneId = Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnGetMonsterCount")
			UINT uRet = pScene->GetMonsterManager()->GetCount() ;
			pScene->m_ObjIDList.m_nCount = uRet ;
			for( UINT i=0; i<uRet; i++ )
			{
				pScene->m_ObjIDList.m_aIDs[i] = pScene->GetMonsterManager()->GetObjByIndex(i)->GetID() ;
			}
			Lua_PushNumber(L,uRet);
			return 1;
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-3-23
	*     �������� : LuaFnGetMonsterObjID
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 nIndex
	*					����������
	*     ����˵�� :
	*				 ����������ȡ�ù����ObjID
	*/
	INT LuaFnGetMonsterObjID( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT nIndex;
	
		sceneId = Lua_ValueToNumber(L,1);
		nIndex = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetMonsterObjID")
			if( nIndex>=pScene->m_ObjIDList.m_nCount )
			{
				Assert(FALSE) ;
				RETURNFALSE
			}
			Obj* pObj = (Obj*)(pScene->GetObjManager()->GetObj(pScene->m_ObjIDList.m_aIDs[nIndex])) ;
			if( pObj && pObj->GetObjType()==Obj::OBJ_TYPE_MONSTER)
			{				
				UINT uRet = (UINT)pObj->GetID() ;
				Lua_PushNumber(L,uRet);
				return 1;
			}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	
	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-05-15
	*     �������� : LuaFnIsCanEnterCopyScene
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 guid
	*					���GUID
	*     ����˵�� :
	*				 �ж�����Ƿ���Խ��볡��sceneId, 
	*					����1 ��ʾ�ܹ���������0��ʾ���ܽ�, -1��ʾ����
	*/
	INT LuaFnIsCanEnterCopyScene( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		GUID_t guid;
	
		sceneId = Lua_ValueToNumber(L,1);
		guid = Lua_ValueToNumber(L,2);

		Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	
		AssertEx(pScene,"����ID���� LuaFnGetSceneType");	
		if(pScene) 
		{
			BOOL bRet = pScene->m_CopyData.IsHaveMember(guid) ;
			Lua_PushNumber(L,bRet);
			return 1;
		}
		Lua_PushNumber(L,-1);
		return 1;

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	

	/** -----------------------------------------------------------------------
	*     ������Ա : �����
	*     ��дʱ�� : 2008-04-17
	*     �������� : LuaFnGetNewSceneInfoByIndex
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 index
	*					�ɽ���ĳ�����������
	*     ����˵�� :
	*				 ���ݿ���ת���������±꣬ �õ���ҽ���ó���������С�ȼ���λ�á�ID��Ϣ 
	*					
	*/
	INT LuaFnGetNewSceneInfoByIndex( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT index;
	
		sceneId = Lua_ValueToNumber(L,1);
		index = Lua_ValueToNumber(L,2);

		
		Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	
		AssertEx(pScene,"����ID���� LuaFnGetNewSceneInfoByIndex");	

		//����ó���������С�ȼ�
		Lua_PushNumber(L,g_MapInfoAttrTbl.m_TableExt[index].m_MinUserLevel);	

		//Ŀ�ĳ���ID
		Lua_PushNumber(L,g_MapInfoAttrTbl.m_TableExt[index].m_SceneId);		

		//λ��
		Lua_PushNumber(L,g_MapInfoAttrTbl.m_TableExt[index].m_DestPosX);			
		Lua_PushNumber(L,g_MapInfoAttrTbl.m_TableExt[index].m_DestPosZ);			
		return 4;

		LUA_LEAVE_FUNCTION
		return 0;
	}


}

#endif

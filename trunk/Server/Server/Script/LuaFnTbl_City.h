//-----------------------------------------------------------------------------
// �ļ��� : LuaFnTbl_City.h
// ģ��	:	Script
// ����	 :  ���нű��ӿ�
// �޸���ʷ:
//				sunyu ����
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_CITY_H__
#define __LUAFNTBL_CITY_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "Obj_Human.h"
#include "ServerManager.h"
#include "PacketFactoryManager.h"
namespace LuaFnTbl
{
	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-3-30
	*     �������� : LuaFnCityCreate
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*	  ����˵�� :
	*				������������ڱ������ĳ���
	*/
	INT	LuaFnCityCreate(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
	
		BEGINSCENEDEFINE("LuaFnCityCreate")
		Obj* pSelf = (Obj*) (pScene->GetObjManager()->GetObj(selfId));
		Assert(pSelf);
		if(pSelf && IsCharacterObj( pSelf->GetObjType() ))
		{
			Obj_Human* pHuman = (Obj_Human*)pSelf;
			Assert(pHuman);
			pHuman->CreateCity( sceneId );
		}
		ENDSCENEDEFINE
		return	0;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-3-30
	*     �������� : LuaFnCityDelete
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				CityIndex
	*					���б��
	*     ����˵�� :
	*				ɾ���˳���
	*/
	INT	LuaFnCityDelete(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT CityIndex ;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		CityIndex		=	Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnCityDelete")
			Obj* pSelf = (Obj*) (pScene->GetObjManager()->GetObj(selfId));
			Assert(pSelf);
			if(pSelf && IsCharacterObj( pSelf->GetObjType() ))
			{
				Obj_Human* pHuman = (Obj_Human*)pSelf;
				Assert(pHuman);
				pHuman->DeleteCity( sceneId, CityIndex );		
			}
		ENDSCENEDEFINE
			return	0;

		LUA_LEAVE_FUNCTION
			return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-3-30
	*     �������� : LuaFnCityBuildingChange
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				BuildingID
	*					������
	*				BuildingLevel
	*					��������
	*     ����˵�� :
	*				���ĳ����н�������
	*/
	INT	LuaFnCityBuildingChange(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT BuildingID;
		INT BuildingLevel;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		BuildingID		=	Lua_ValueToNumber(L,3);
		BuildingLevel	=	Lua_ValueToNumber(L,4);

		BEGINSCENEDEFINE("LuaFnCityBuildingChange")
		Obj* pSelf = (Obj*) (pScene->GetObjManager()->GetObj(selfId));
		Assert(pSelf);
		if(pSelf && IsCharacterObj( pSelf->GetObjType() ))
		{
			Obj_Human* pHuman = (Obj_Human*)pSelf;
			Assert(pHuman);
			pHuman->ChangeCityBuilding( sceneId, BuildingID, BuildingLevel);
		}
		ENDSCENEDEFINE
			return	0;

		LUA_LEAVE_FUNCTION
			return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-3-30
	*     �������� : LuaFnCityMoveTo
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				CityIndex
	*					��������
	*     ����˵�� :
	*				���ĳ����н�������
	*/
	INT	LuaFnCityMoveTo(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT CityIndex;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		CityIndex		=	Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnCityMoveTo")
		Obj* pSelf = (Obj*) (pScene->GetObjManager()->GetObj(selfId));
		Assert(pSelf);
		if(pSelf && IsCharacterObj( pSelf->GetObjType() ))
		{
			Obj_Human* pHuman = (Obj_Human*)pSelf;
			Assert(pHuman);
			Scene* pPortScene = g_pSceneManager->GetScene(sceneId);
			if(pPortScene->m_CityData.m_ScenePortList[CityIndex] != INVALID_ID)
			{//���ڴ˳���
				Scene* pCityScene = g_pSceneManager->GetScene(pPortScene->m_CityData.m_ScenePortList[CityIndex]);
				if(pCityScene)
				{
					if(pCityScene->GetSceneType() == SCENE_TYPE_CIT)
					{
						WORLD_POS pos(22,22);
						pHuman->ChangeScene( sceneId, pCityScene->SceneID(), pos, 9 ) ;
					}
				}
			}
		}
		ENDSCENEDEFINE
			return	0;

		LUA_LEAVE_FUNCTION
			return 0 ;
	}
}
#endif
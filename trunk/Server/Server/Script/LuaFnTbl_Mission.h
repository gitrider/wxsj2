//-----------------------------------------------------------------------------
// �ļ��� : LuaFnTbl_Mission.h
// ģ��	:	Script
// ����	 :	������ص�������
// �޸���ʷ:2008-2-1 �����
//	yangfei����
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_MISSION_H__
#define __LUAFNTBL_MISSION_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "GCScriptCommand.h"
#include "SceneManager.h"
#include "ItemHelper.h"
#include "ItemManager.h"
#include "HumanItemLogic.h"
#include "MonsterManager.h"
#include "Obj_Monster.h"
#include "GCMissionResult.h"
#include "GCUICommand.h"
#include "GameTable.h"
#include "LogDefine.h"
#include "DayTips.h"


namespace LuaFnTbl
{


	enum
	{
		EA_Number,
		EA_State,
		EA_Index,
		EA_Count
	};

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnBeginEvent
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*				��EndEvent���ʹ��
	*				Ϊ��һ������ز�����׼��
	*/
	INT LuaFnBeginEvent(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;

		sceneId = Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnBeginEvent")
		pScene->mMisBuf.ResetMisBuf();

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0;
	}

	
	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddText
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				msg
	*					��ʾ����
	*     ����˵�� :
	*				����ַ�����ʾ
	*/
	INT LuaFnAddText(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		const CHAR* msg;

		sceneId = Lua_ValueToNumber(L,1);		
		msg = Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnAddText")
		//size_t len = min(_MisBuf::MISSIONCHARBUFLEN -1,strlen(msg));	
		//strncpy(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf,msg,len);
		Q_strncpyz(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf,msg,_MisBuf::MISSIONCHARBUFLEN);//�Ż�
		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_Str;		

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddNumber
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				id
	*					��Ŀ,INT
	*     ����˵�� :
	*				�����Ŀ��ʾ
	*/
	INT LuaFnAddNumber(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT id;

		sceneId = Lua_ValueToNumber(L,1);
		id = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnAddNumber")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf);
		*pNumber = id;
		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_Int;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddNumText
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				id
	*					�ڼ�����ʾ,INT
	*				msg
	*					��ʾ����
	*				state 
	*					�Ƿ���Խ�����ͽ�����
	*				index
	*					�߻���������
	*     ����˵�� :
	*				�����Ŀ���ַ�����ʾ
	*				state��index���Բ���
	*/
	INT LuaFnAddNumText(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		INT id;
		const CHAR* msg;
		INT state;
		INT index;

		sceneId = Lua_ValueToNumber(L,1);
		id = Lua_ValueToNumber(L,2);
		msg = Lua_ValueToString(L,3);
		
		//
		if(Lua_GetTopIndex(L) < 4)
			state = -1;//���Խӵ�����
		else
			state = Lua_ValueToNumber(L,4);
		
		if(Lua_GetTopIndex(L) < 5)
			index = -1;//���Խӵ�����
		else
			index = Lua_ValueToNumber(L,5);

		BEGINSCENEDEFINE("LuaFnAddNumText")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_Number * sizeof(INT));
		*pNumber = id;
		INT* pState  = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_State * sizeof(INT));
		*pState = state;
		INT* pIndex  = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_Index * sizeof(INT));
		*pIndex = index;
		
		//size_t len = min(_MisBuf::MISSIONCHARBUFLEN -1,strlen(msg));	
		//strncpy(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + sizeof(INT),msg,strlen(msg));
		Q_strncpyz(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_Count * sizeof(INT),msg,_MisBuf::MISSIONCHARBUFLEN - EA_Count * sizeof(INT));
		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_IntStr;	

		return 0;
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : yf
	*     ��дʱ�� : 2005-12-9
	*     �������� : LuaFnSetNumText
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				number
	*					�����õ�ֵ,short
	*     ����˵�� :
	*				���ýű�����������
	*/
	INT LuaFnSetNumText(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		INT number;

		number = Lua_ValueToNumber(L,1);
		lua_setglobal(L,"global_numtext");

		return 0;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : yf
	*     ��дʱ�� : 2005-12-9
	*     �������� : LuaFnGetNumText
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				number
	*					�����õ�ֵ,short
	*     ����˵�� :
	*				ȡ���ű�����������
	*/
	INT LuaFnGetNumText(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		lua_getglobal(L,"global_numtext");
		INT number = Lua_ValueToNumber(L,1);
		Lua_PushNumber(L,number);
		return 1;

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     ������Ա :�����
	*     ��дʱ�� : 2008-1-31
	*     �������� : LuaFnGetOneMissionInfo
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				nPhase
	*					���������׶�
	*				MissionType
	*					�������� 0��ɱ��  1��Ѱ��  2������	
	*				MisIndex
	*					�������� Ϊ-1ʱ��Ҫ�����������Ϊ-1ʱMisIndexΪ��һ�������
	*     ����˵�� :
	*				�õ�ѭ��������Ŀ��NPC��ITEM����Ϣ��ID��NAME��COUNT�����ڳ�������ID��λ�á�����
	*				�õ�ѭ��������������Ҫ�������ű�ʹ��
	*/

	INT LuaFnGetOneMissionInfo(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		INT nPhase;
		INT MissionType;
		INT MisIndex;

		nPhase = Lua_ValueToNumber(L,1);
		MissionType = Lua_ValueToNumber(L,2);
		MisIndex = Lua_ValueToNumber(L,3);

		if ( MisIndex == -1 )
		{
			MisIndex = rand()%g_XunHuanMissionAttrTbl.m_SubMissionInfo[nPhase][MissionType].m_SubMissionCnt ;
			AssertEx(MisIndex>=0 && MisIndex<g_XunHuanMissionAttrTbl.m_SubMissionInfo[nPhase][MissionType].m_SubMissionCnt,"������ID���� ");
			MisIndex += g_XunHuanMissionAttrTbl.m_SubMissionInfo[nPhase][MissionType].M_SubMissionPos;	
		}

		Lua_PushNumber(L,g_XunHuanMissionAttrTbl.m_TableExt[nPhase][MisIndex].m_ObjId) ;	
		Lua_PushString(L,g_XunHuanMissionAttrTbl.m_TableExt[nPhase][MisIndex].m_ObjName);
		Lua_PushNumber(L,g_XunHuanMissionAttrTbl.m_TableExt[nPhase][MisIndex].m_ObjCount) ;	

		Lua_PushString(L, g_XunHuanMissionAttrTbl.m_TableExt[nPhase][MisIndex].m_SceneName);
		Lua_PushNumber(L,g_XunHuanMissionAttrTbl.m_TableExt[nPhase][MisIndex].m_ScendId);
		Lua_PushNumber(L,g_XunHuanMissionAttrTbl.m_TableExt[nPhase][MisIndex].m_PosX);			//λ��
		Lua_PushNumber(L,g_XunHuanMissionAttrTbl.m_TableExt[nPhase][MisIndex].m_PosZ);			//λ��
		Lua_PushString(L,g_XunHuanMissionAttrTbl.m_TableExt[nPhase][MisIndex].m_DialogText);
		Lua_PushNumber(L,MisIndex);
		return 9;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}



	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddMoneyBonus
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				count
	*					������Ǯ��Ŀ
	*     ����˵�� :
	*				���Money
	*/
	INT LuaFnAddMoneyBonus(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT count;

		sceneId = Lua_ValueToNumber(L,1);
		count = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnAddMoneyBonus")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf);
		*pNumber = count;
		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_Money;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddItemBonus
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				itemTypeSn
	*					������Ʒ��ItemType��9λ���
	*				count
	*					������Ŀ
	*     ����˵�� :
	*				���count�����ΪitemType����
	*/
	INT LuaFnAddItemBonus(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT itemTypeSn;
		INT count;
			
		sceneId = Lua_ValueToNumber(L,1);
		itemTypeSn = Lua_ValueToNumber(L,2);
		count = Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnAddItemBonus")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf);
		*pNumber++ = itemTypeSn;
		*pNumber = count;

		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_Item;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddItemDemand
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				itemTypeSn
	*					������Ʒ��ItemType��9λ���
	*				count
	*					������Ŀ

	*     ����˵�� :
	*				����count�����ΪitemType����
	*/
	INT LuaFnAddItemDemand(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT itemTypeSn;
		INT count;
			
		sceneId = Lua_ValueToNumber(L,1);;
		itemTypeSn = Lua_ValueToNumber(L,2);
		count = Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnAddItemDemand")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf);
		
		*pNumber++ = itemTypeSn ;
		*pNumber = count;

		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_Item;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddRandItemBonus
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*					��ʾ�����������
	*/
	INT LuaFnAddRandItemBonus(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;

		sceneId = Lua_ValueToNumber(L,1);
			
		BEGINSCENEDEFINE("LuaFnAddRandItemBonus")

		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_RandItem;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddRadioItemBonus
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				itemTypeSn
	*					��Ʒ��ItemType9λ���
	*				count
	*					������ѡһ��Ʒ����Ŀ
	*     ����˵�� :
	*				��Ӷ�ѡһ����
	*/
	INT LuaFnAddRadioItemBonus(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT itemTypeSn;
		INT count;
		
		sceneId = Lua_ValueToNumber(L,1);
		itemTypeSn = Lua_ValueToNumber(L,2);
		count = Lua_ValueToNumber(L,3);
	
		BEGINSCENEDEFINE("LuaFnAddRadioItemBonus")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf);

		*pNumber++ = itemTypeSn;
		*pNumber = count;

		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_RadioItem;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnEndEvent
	*     ����˵�� : 
	*     ����˵�� :
	*				��BeginEvent���ʹ��
	*				֮�������ĳ��Dispatch***����
	*/
	INT LuaFnEndEvent(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
			return 0;

		LUA_LEAVE_FUNCTION
	}
	
	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDispatchEventList
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*     ����˵�� :
	*				��ͻ��˷��ظ�Npc�ɴ����¼��б�
	*				�ɴ����¼��ɸ�npc�Ĺҵ��¼�OnEnumerate�����ж�
	*				������������������,ͨ��AddNumText���
	*				BeginEvent
	*				AddText
	*				����: AddNumText
	*				EndEvent
	*/
	INT LuaFnDispatchEventList(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		targetId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDispatchEventList")
		ScriptParam_EventList paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;

		for(INT i= 0;i < pScene->mMisBuf.mUseNum;i++)
		{
			if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Str)
			{
				ScriptEventItem itemEvent;
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf;
				itemEvent.SetText( &scriptStr);
				paramEventList.AddItem( &itemEvent );
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Int)
			{
				//ScriptString scriptStr;
				//scriptStr = pScene->mMisBuf.Buff[i].buf;
				//INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				//ScriptEventItem itemEvent;
				//itemEvent.SetScriptID(*number,&scriptStr);
				//paramEventList.AddItem( &itemEvent );
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_IntStr)
			{
				INT number = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_Number * sizeof(INT));
				INT state = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_State * sizeof(INT));
				INT index = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_Index * sizeof(INT));
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf + EA_Count * sizeof(INT);
				ScriptEventItem itemEvent;
				itemEvent.SetScriptID(index,state,number,&scriptStr);
				paramEventList.AddItem( &itemEvent );
			}

		}

		Packets::GCScriptCommand Msg;
		Msg.SetEventListResponseCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;	
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDispatchRegieEventList
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*     ����˵�� :
	*				��ͻ��˷��ظ�Npc�ɴ����¼��б�
	*				�ɴ����¼��ɸ�npc�Ĺҵ��¼�OnEnumerate�����ж�
	*				������������������,ͨ��AddNumText���
	*				BeginEvent
	*				AddText
	*				����: AddNumText
	*				EndEvent
	*/
	INT LuaFnDispatchRegieEventList(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		targetId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDispatchRegieEventList")
		ScriptParam_EventList paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;

		for(INT i= 0;i < pScene->mMisBuf.mUseNum;i++)
		{
			if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Str)
			{
				ScriptEventItem itemEvent;
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf;
				itemEvent.SetText( &scriptStr);
				paramEventList.AddItem( &itemEvent );
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Int)
			{
				//ScriptString scriptStr;
				//scriptStr = pScene->mMisBuf.Buff[i].buf;
				//INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				//ScriptEventItem itemEvent;
				//itemEvent.SetScriptID(*number,&scriptStr);
				//paramEventList.AddItem( &itemEvent );
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_IntStr)
			{
				INT number = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_Number * sizeof(INT));
				INT state = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_State * sizeof(INT));
				INT index = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_Index * sizeof(INT));
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf + EA_Count * sizeof(INT);
				ScriptEventItem itemEvent;
				itemEvent.SetScriptID(index,state,number,&scriptStr);
				paramEventList.AddItem( &itemEvent );
			}

		}

		Packets::GCScriptCommand Msg;
		Msg.SetMissionRegieCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;	
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDispatchMissionInfo
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				scriptId
	*					�ű����
	*				misId
	*					������
	*     ����˵�� :
	*				��ͻ��˷���������Ϣ����
	*				
	*/
	INT LuaFnDispatchMissionInfo(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;
		ScriptID_t scriptId;
		MissionID_t misId;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		targetId = Lua_ValueToNumber(L,3);
		scriptId = Lua_ValueToNumber(L,4);
		misId = Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnDispatchMissionInfo")
		ScriptParam_MissionInfo paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;
		paramEventList.m_idMission = misId;
		paramEventList.m_idScript = scriptId;

		for(INT i= 0;i < pScene->mMisBuf.mUseNum;i++)
		{
			if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Str)
			{
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf;
				paramEventList.AddText( &scriptStr);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Item)
			{
				SMissionBonus bonus;

				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetItem(number[0],number[1]);
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_RandItem)
			{
				SMissionBonus bonus;
				bonus.SetItemRand();
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_RadioItem)
			{
				SMissionBonus bonus;
				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetItemRadio(number[0],number[1]);
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Money)
			{
				SMissionBonus bonus;
				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetMoney(*number);
				paramEventList.AddBonus(&bonus);
			}
		}

		Packets::GCScriptCommand Msg;
		Msg.SetMissionInfoResponseCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDispatchMissionDemandInfo
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				scriptId
	*					�ű����
	*				misId
	*					������
	*				done
	*					�Ƿ����
	*     ����˵�� :
	*				��ͻ��˷�������������
	*				done���ΪTRUE,�ͻ��˽���������������ɰ�ť
	*				֮ǰ��ʹ�õĺ�������Ϊ
	*				BeginEvent
	*				AddText,�������ַ���
	*				AddItemDemand,������Ʒ
	*				EndEvent
	*/
	INT LuaFnDispatchMissionDemandInfo(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;
		ScriptID_t scriptId;
		MissionID_t misId;
		INT done;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		targetId = Lua_ValueToNumber(L,3);
		scriptId = Lua_ValueToNumber(L,4);
		misId = Lua_ValueToNumber(L,5);
		done = (Lua_ValueToNumber(L,6));

		BEGINHUMANDEFINE("LuaFnDispatchMissionDemandInfo")
		ScriptParam_MissionDemandInfo paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;
		paramEventList.m_idScript = scriptId;
		paramEventList.m_idMission = misId;
		paramEventList.m_bDone = done;

		for(INT i= 0;i < pScene->mMisBuf.mUseNum;i++)
		{
			if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Str)
			{
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf;
				paramEventList.AddText( &scriptStr);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Item)
			{
				SMissionDemandItem demandItem;

				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				demandItem.m_uItemID = number[0];
				demandItem.m_yCount = (BYTE)(number[1]);
				paramEventList.AddDemandItem(&demandItem);
			}
		}


		Packets::GCScriptCommand Msg;
		Msg.SetMissionDemandInfoResponseCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDispatchMissionContinueInfo
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				scriptId
	*					�ű����
	*				misId
	*					������
	*     ����˵�� :
	*				��ͻ��˷��������������
	*/
	INT LuaFnDispatchMissionContinueInfo(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;
		ScriptID_t scriptId;
		MissionID_t misId;
			
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		targetId = Lua_ValueToNumber(L,3);
		scriptId = Lua_ValueToNumber(L,4);
		misId = Lua_ValueToNumber(L,5);
		
		BEGINHUMANDEFINE("LuaFnDispatchMissionContinueInfo")
		ScriptParam_MissionContinueInfo paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;
		paramEventList.m_idScript = scriptId;
		paramEventList.m_idMission = misId;

		for(INT i= 0;i < pScene->mMisBuf.mUseNum;i++)
		{
			if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Str)
			{
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf;
				paramEventList.AddText( &scriptStr);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Item)
			{
				SMissionBonus bonus;

				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetItem(number[0],number[1]);
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_RandItem)
			{
				SMissionBonus bonus;
				bonus.SetItemRand();
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_RadioItem)
			{
				SMissionBonus bonus;
				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetItemRadio(number[0],number[1]);
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Money)
			{
				SMissionBonus bonus;
				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetMoney(*number);
				paramEventList.AddBonus(&bonus);
			}
		}

		Packets::GCScriptCommand Msg;
		Msg.SetMissionContinueInfoResponseCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDispatchMissionTips
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��ͻ��˷������������ʾ��Ϣ
	*				
	*/
	INT LuaFnDispatchMissionTips(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnDispatchMissionTips")
		ScriptParam_MissionTips paramMissionTips;
		paramMissionTips.Reset();
		if(pScene->mMisBuf.Buff[0].mType == _MisBuf::_MisItemBuf::BufType_Str)
		{
			paramMissionTips.m_strText = pScene->mMisBuf.Buff[0].buf;
		}

		Packets::GCScriptCommand Msg;
		Msg.SetMissionTipsCmd( &paramMissionTips );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;	
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-28
	*     �������� : LuaFnDispatchMissionResult
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��ͻ��˷���������֤���
	*				
	*/
	INT LuaFnDispatchMissionResult(Lua_State* L)
	{

		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT IsFinished;

		sceneId		= Lua_ValueToNumber(L,1);
		selfId		= Lua_ValueToNumber(L,2);
		IsFinished	= Lua_ValueToNumber(L,3);
		BEGINHUMANDEFINE("LuaFnDispatchMissionResult")

		Packets::GCMissionResult Msg;
		Msg.setIsFinished(IsFinished);
		pHuman->GetPlayer()->SendPacket(&Msg);

		return 0;	

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnMissionCom
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*     ����˵�� :
	*				�������
	*				֮����DelMission
	*/
	INT LuaFnMissionCom(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		MissionID_t misId;

		sceneId = Lua_ValueToNumber(L,1);	
		selfId = Lua_ValueToNumber(L,2);
		misId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnMissionCom")
		pHuman->SetMissionHaveDone(misId,TRUE);

		return 0;
		ENDHUMANDEFINE
		
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMission
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*     ����˵�� :
	*				��õ�ǰ���ĳһ����ŵ�ֵ	//����,������,δ��
	*/
	INT LuaFnGetMission(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		MissionID_t misId;

		sceneId = Lua_ValueToNumber(L,1);	
		selfId = Lua_ValueToNumber(L,2);
		misId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetMission")
		Lua_PushNumber(L,pHuman->IsHaveMission(misId));
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMissionIndexByID
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*     ����˵�� :
	*				���õ�ǰ���ĳһ����ŵ�ֵ
	*/
	INT LuaFnGetMissionIndexByID(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		MissionID_t misId;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		misId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetMissionIndexByID")
		Lua_PushNumber(L,pHuman->GetMissionIndexByID(misId));
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION	
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMissionParam
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misIndex
	*					����λ������
	*				paramIndex
	*					�������ֶα��
	*     ����˵�� :
	*				 ȡ�������
	*/
	INT LuaFnGetMissionParam(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t selfId;
		INT misIndex;
		INT paramIndex;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		misIndex = Lua_ValueToNumber(L,3);
		paramIndex = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnGetMissionParam")
		if( misIndex>=0 )
		{
			Lua_PushNumber( L, pHuman->GetMissionParam( misIndex, paramIndex ) );
			return 1;
		}
		else
		{
			Lua_PushNumber( L, 0 );
			return 1;
		}
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetMissionByIndex
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*				paramIndex
	*					�������ֶα��
	*				value
	*					��paramIndex�ֶε�ֵ
	*     ����˵�� :
	*				 �����������
	*/
	INT LuaFnSetMissionByIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t selfId;
		INT misIndex;
		INT paramIndex;
		INT value;
			
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		misIndex = Lua_ValueToNumber(L,3);
		paramIndex = Lua_ValueToNumber(L,4);
		value= Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnSetMissionByIndex")
		if( misIndex>=0 )
		{
			pHuman->SetMissionParam(misIndex,paramIndex,value);
			return 0;
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-1-18
	*     �������� : LuaFnGetMissionCount
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				 �õ��ѽ����������
	*				 û������ʱ����0
	*/
	INT LuaFnGetMissionCount(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnGetMissionCount")
			Lua_PushNumber(L,pHuman->GetConstMissionList()->m_Count);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : yf
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddMission
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*				scriptId
	*					���ýű����
	*				killObjEvent
	*					�Ƿ�ص�OnKillObject
	*				enterAreaEvent
	*					�Ƿ�ص�OnEnterArea
	*				itemChangeEvent
	*					�Ƿ�ص�OnItemChange
	*     ����˵�� :
	*				������������misId��ŵ�����
	*				�ɹ�����1
	*/
	INT LuaFnAddMission( Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t selfId;
		MissionID_t misId;
		ScriptID_t scriptId;
		BOOL killObjEvent;
		BOOL enterAreaEvent;
		BOOL itemChangedEvent;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		misId = Lua_ValueToNumber(L,3);
		scriptId = Lua_ValueToNumber(L,4);
		killObjEvent = Lua_ValueToNumber(L,5) > 0 ? 1 : 0;
		enterAreaEvent = Lua_ValueToNumber(L,6)> 0 ? 1 : 0;
		itemChangedEvent = Lua_ValueToNumber(L,7)> 0 ? 1 : 0;

		BEGINHUMANDEFINE("LuaFnAddMission")
		INT ret = pHuman->AddMission( misId, scriptId, killObjEvent, enterAreaEvent, itemChangedEvent );
		ret = ( (ret==OR_OK) ? TRUE : FALSE ) ;

		Lua_PushNumber( L, ret );
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-3-13
	*     �������� : LuaFnAddMissionEx
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*				scriptId
	*					���ýű����
	*     ����˵�� :
	*				������������misId��ŵ�����
	*				�ɹ�����1
	*/
	INT LuaFnAddMissionEx( Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t selfId;
		MissionID_t misId;
		ScriptID_t scriptId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		misId = Lua_ValueToNumber(L,3);
		scriptId = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnAddMissionEx")
		INT ret = pHuman->AddMissionEx( misId, scriptId );
		ret = ( (ret==OR_OK) ? TRUE : FALSE ) ;

		Lua_PushNumber( L, ret );
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-3-13
	*     �������� : LuaFnSetMissionEvent
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*				eventId
	*					���ýű���ţ�0 KillObj, 1 EnterArea, 2 ItemChanged, 3 PetChanged
	*     ����˵�� :
	*				�޸��������misId��ŵ�������Ӧ�¼�
	*				�ɹ�����1
	*/
	INT LuaFnSetMissionEvent( Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t selfId;
		MissionID_t misId;
		INT eventId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		misId = Lua_ValueToNumber(L,3);
		eventId = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnSetMissionEvent")
		INT ret = pHuman->SetMissionEvent( misId, eventId );
		ret = ( (ret==OR_OK) ? TRUE : FALSE ) ;

		Lua_PushNumber( L, ret );
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDelMission
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*     ����˵�� :
	*				ɾ������
	*/
	INT LuaFnDelMission(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		MissionID_t misId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		misId = Lua_ValueToNumber(L,3);
		
		BEGINHUMANDEFINE("LuaFnDelMission")
		pHuman->DelMission(misId);

		Lua_PushNumber( L, 1 );
		return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : yf
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnIsMissionHaveDone
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*     ����˵�� :
	*				�Ƿ�����������
	*				����ֵ1Ϊ�ɹ�
	*				����ֵ0Ϊʧ��
	*/
	INT LuaFnIsMissionHaveDone(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		MissionID_t misId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		misId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnIsMissionHaveDone")
		Lua_PushNumber(L,pHuman->IsMissionHaveDone(misId));
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : IsHaveMission
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*     ����˵�� :
	*				����Ƿ�������������
	*				����ֵ����0Ϊ�ɹ�
	*				
	*/
	INT LuaFnIsHaveMission(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		MissionID_t misId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		misId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnIsHaveMission")
		Lua_PushNumber(L,pHuman->IsHaveMission(misId));
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnBeginAddItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*				��ʼ��ӵ��߲���
	*				��EndAddItem���ʹ��
	*/
	INT LuaFnBeginAddItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;

			sceneId = Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnBeginAddItem")
			pScene->mItemList.Init();
			return 0;
		ENDSCENEDEFINE
		
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				itemTypeSn
	*					ItemType��9λ����(ʵ��Ϊ8λ)	
	*				count
	*					�����Ŀ
	*     ����˵�� :
	*				�������Ʒ��������count��itemType��ŵĵ���
	*/
	INT LuaFnAddItem( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT itemTypeSn;
		INT count;

		sceneId = Lua_ValueToNumber(L,1);
		itemTypeSn = Lua_ValueToNumber(L,2);
		count = Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnAddItem")
		TSerialHelper help(itemTypeSn);
		_ITEM_TYPE	itemType =	help.GetItemTypeStruct();


		if(!itemType.isNull())
		{
			pScene->mItemList.AddType(itemType,count);
			
			return 0;
		}
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : yf
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnEndAddItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				������ӵ��߲���
	*				��BeginAddItem���ʹ��
	*				����1��ʾ�пռ���������Ʒ
	*/
	INT LuaFnEndAddItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnEndAddItem")
			Lua_PushNumber(L,HumanItemLogic::CanBagReceiveItemList(pHuman,pScene->mItemList));
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddItemListToHuman
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				����Ʒ�б��е�������Ʒ���õ���Ұ���
	*				�ڵ���BeginAddItem��EndAddItem֮��ʹ��
	*
	*	  ��ע:
	*				�˺�����ص���ӦOnItemChanged�¼��Ľű�
	*/
	INT LuaFnAddItemListToHuman(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnAddItemListToHuman")
			//pHuman->ReceiveItemList(pScene->mItemList);

		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType		= ITEM_CREATE_FROM_SCRIPT;
		ItemLogParam.CharGUID	= pHuman->GetGUID();
		ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
		ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
		ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
	
		BOOL bRet =	HumanItemLogic::ReceiveBagItemList(&ItemLogParam,pHuman,pScene->mItemList);

		if(bRet) SaveItemLog(&ItemLogParam);

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-11-16
	*     �������� : LuaFnGetMissionData
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				nIndex
	*					���ݿ�����
	*     ����˵�� :
	*				�õ�Humanȫ�����ݿ��е�nIndex������ֵ
	*				GetMissionData(L,18,12,3)
	*/
	INT LuaFnGetMissionData(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		nIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetMissionData")

		if( nIndex<0 || nIndex>=MAX_CHAR_MISSION_DATA_NUM )
		{
			Lua_PushNumber(L,-1);
			return 1;
		}
		else
		{
			Lua_PushNumber(L,pHuman->GetMissionData(nIndex));
			return 1;
		}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		
		RETURNFALSE
	}
	
	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-11-16
	*     �������� : LuaFnSetMissionData
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				nIndex
	*					���ݿ�����
	*				nData
	*					����ֵ
	*     ����˵�� :
	*				����Human��nIndex�����ݿ��ֵΪnData
	*				�ڵ���BeginAddItem��EndAddItem֮��ʹ��
	*/
	INT LuaFnSetMissionData(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		INT nData;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		nIndex = Lua_ValueToNumber(L,3);
		nData = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnSetMissionData")
			if( nIndex<0 || nIndex>=MAX_CHAR_MISSION_DATA_NUM )
			{
				return 0;
			}
			pHuman->SetMissionData(nIndex,nData);
		ENDHUMANDEFINE
		
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/** ---------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-11-16
	*     �������� : LuaFnAddMonsterDropItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				MonsterID
	*					������
	*				HumanID
	*					��ɫ���
	*				ItemIndex
	*					�������Ʒ����
	*				
	*     ����˵�� :
	*			�ڹ����������һ������������Ʒ,������������
	*/
	INT LuaFnAddMonsterDropItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t MonsterID;
		ObjID_t HumanID;
		UINT ItemIndex;

		sceneId			=	Lua_ValueToNumber(L,1);
		MonsterID		=	Lua_ValueToNumber(L,2);
		HumanID			=	Lua_ValueToNumber(L,3);
		ItemIndex		=	Lua_ValueToNumber(L,4);
		
		BEGINSCENEDEFINE("LuaFnAddMonsterDropItem")
		Obj_Monster*	pMonster	=	
			pScene->GetMonsterManager()->GetMonster(MonsterID);
		if(!pMonster)
		{
			Assert(FALSE);
			return 0;
		}
		Assert(ItemIndex>0);
		Assert(ItemIndex!=INVALID_ID);
		//pMonster->GetOwnerList().AddItem(ItemIndex,HumanID);

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}

	/** ---------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-11-16
	*     �������� : LuaFnGetMonsterOwnerCount
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				MonsterID
	*					������
	*				
	*     ����˵�� :
	*			��ȡ��������������
	*/
	INT LuaFnGetMonsterOwnerCount(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t MonsterID;

		sceneId			=	Lua_ValueToNumber(L,1);
		MonsterID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetMonsterOwnerCount")
		Obj_Monster*	pMonster	=	
			pScene->GetMonsterManager()->GetMonster(MonsterID);
		if(!pMonster)
		{
			Assert(FALSE);
			return 0;
		}
		
		Lua_PushNumber(L,pMonster->GetOwnerList().OwnerCount);
		return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** ---------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-11-16
	*     �������� : LuaFnGetMonsterOwnerID
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				MonsterID
	*					������
	*				OwnerOffSet
	*					������ƫ����
	*     ����˵�� :
	*			��ȡ���������߱��
	*			����������id
	*/
	INT LuaFnGetMonsterOwnerID(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t MonsterID;
		UINT OwnerOffSet;

		sceneId			=	Lua_ValueToNumber(L,1);
		MonsterID		=	Lua_ValueToNumber(L,2);
		OwnerOffSet		=	Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnGetMonsterOwnerID")
		Obj_Monster*	pMonster	=	
			pScene->GetMonsterManager()->GetMonster(MonsterID);
		if(!pMonster)
		{
			Assert(FALSE);
			RETURNFALSE
		}	
		
		Assert(OwnerOffSet<pMonster->GetOwnerList().OwnerCount);
		//Lua_PushNumber(L,pMonster->GetOwnerList().OwnerDropList[OwnerOffSet].HumanID);
		return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** ---------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-2-8
	*     �������� : LuaFnBeginUICommand
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*			��ʼ����UICommand������
	*			�޷���
	*/
	INT LuaFnBeginUICommand(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;

		sceneId			=	Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnBeginUICommand")
			pScene->m_XParam.CleanUp() ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}

	/** ---------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-2-8
	*     �������� : LuaFnEndUICommand
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*			��������UICommand������
	*			�޷���
	*/
	INT LuaFnEndUICommand(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;

		sceneId			=	Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnEndUICommand")
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}	

	/** ---------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-2-8
	*     �������� : LuaFnDispatchUICommand
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				nUIIndex
	*					��������ֵ
	*     ����˵�� :
	*			��������UICommand������
	*			�޷���
	*/
	INT LuaFnDispatchUICommand(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nUIIndex;

		sceneId	= Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		nUIIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDispatchUICommand")
			GCUICommand Msg ;
			Msg.SetParam(&pScene->m_XParam) ;
			Msg.SetUIIndex(nUIIndex) ;
			pHuman->GetPlayer()->SendPacket(&Msg) ;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}	
	
	/** ---------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-2-8
	*     �������� : LuaFnUICommand_AddInt
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				nValue
	*					����������ӵ�����,����
	*     ����˵�� :
	*			��������UICommand������
	*			�޷���
	*/
	INT LuaFnUICommand_AddInt(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT nValue;

		sceneId			=	Lua_ValueToNumber(L,1);
		nValue			=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnUICommand_AddInt")
			pScene->m_XParam.AddIntValue(nValue) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}	

	/** ---------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-2-8
	*     �������� : LuaFnUICommand_AddString
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				szValue
	*					����������ӵ�����,�ַ���
	*     ����˵�� :
	*			��������UICommand������
	*			�޷���
	*/
	INT LuaFnUICommand_AddString(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		const CHAR* szValue;

		sceneId			=	Lua_ValueToNumber(L,1);
		szValue			=	Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnUICommand_AddString")
			pScene->m_XParam.AddStrValue(szValue) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}	

	/** ---------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-2-10
	*     �������� : LuaFnGetQuestionsRecord
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				nIndex
	*					����ţ�����
	*     ����˵�� :
	*			��ȡ�����Ϣ
	*			����ֵ����������(�ַ���),
	*					ѡ��0(�ַ���),
	*					ѡ��1(�ַ���),
	*					ѡ��2(�ַ���),
	*					ѡ��3(�ַ���),
	*					ѡ��4(�ַ���),
	*					ѡ��5(�ַ���),
	*					��0(����),
	*					��1(����),
	*					��2(����),
	*					��3(����),
	*					��4(����),
	*					��5(����),
	*/
	INT LuaFnGetQuestionsRecord(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		INT nIndex;

		nIndex			=	Lua_ValueToNumber(L,1);

		INT i ;
		for( i=0; i<MAX_QUESTIONS; i++ )
		{
			if( nIndex==g_QuestionLib.m_aQuestions[i].m_nIndex )
				break ;
		}
		if( i>=MAX_QUESTIONS )
		{
			AssertEx(FALSE,"������û��������з��֣����飡") ;
			return 0 ;
		}

		_QUESTION_DATA* pData = &(g_QuestionLib.m_aQuestions[i]) ;

		Lua_PushString(L,pData->m_szContex) ;
		Lua_PushString(L,pData->m_szOption0) ;
		Lua_PushString(L,pData->m_szOption1) ;
		Lua_PushString(L,pData->m_szOption2) ;
		Lua_PushString(L,pData->m_szOption3) ;
		Lua_PushString(L,pData->m_szOption4) ;
		Lua_PushString(L,pData->m_szOption5) ;
		Lua_PushNumber(L,pData->m_Key0);
		Lua_PushNumber(L,pData->m_Key1);
		Lua_PushNumber(L,pData->m_Key2);
		Lua_PushNumber(L,pData->m_Key3);
		Lua_PushNumber(L,pData->m_Key4);
		Lua_PushNumber(L,pData->m_Key5);
		return 13;

		LUA_LEAVE_FUNCTION

		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		return 13;
	}	

	/** ---------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-4-19
	*     �������� : LuaFnSetMissionDataBit
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				nIndex
	*					MissionData�ı������
	*				nBits
	*					ƫ�Ƶ�λ����0��31��
	*				nValue
	*					����, 0���0
	*     ����˵�� :
	*			�޷���
	*/
	INT LuaFnSetMissionDataBit(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		INT nBits;
		INT nValue;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		nIndex			=	Lua_ValueToNumber(L,3);
		nBits			=	Lua_ValueToNumber(L,4);
		nValue			=	Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnSetMissionDataBit")
			INT nDataValue = pHuman->GetMissionData(nIndex) ;
			if( nValue )
			{
				nDataValue |= (1<<nBits) ;
			}
			else
			{
				nDataValue &= (~(1<<nBits)) ;
			}
			pHuman->SetMissionData(nIndex,nDataValue) ;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}	

	/** ---------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-4-19
	*     �������� : LuaFnGetMissionDataBit
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				nIndex
	*					MissionData�ı������
	*				nBits
	*					ƫ�Ƶ�λ����0��31��
	*     ����˵�� :
	*			0 ���� 1
	*/
	INT LuaFnGetMissionDataBit(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		INT nBits;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		nIndex			=	Lua_ValueToNumber(L,3);
		nBits			=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnGetMissionDataBit")
			INT nDataValue = pHuman->GetMissionData(nIndex) ;
			INT nRet=nDataValue&(1<<nBits) ;
			//nRet = nRet >> nBits; // ����nRetΪ1��0
			Lua_PushNumber(L,nRet);
			return 1 ;
		ENDHUMANDEFINE

		Lua_PushNumber(L,0);
		return 1 ;

		LUA_LEAVE_FUNCTION
	}	

	/** ---------------------------------------------------
	*     ������Ա : liulu
	*     ��дʱ�� : 2010-07-22
	*     �������� : LuaFnIsMissionInTime
	*					
	*     ����˵�� :
	*			����ÿ�ջ�����Ƿ��ڻ����ʱ����
	*			��ÿ�ջʱ�ô˺����ж�
	*	
	*/
	INT LuaFnIsMissionInTime(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
	
		INT nMissionID;

		nMissionID = Lua_ValueToNumber( L, 1 );	

		INT nRet;

		nRet = DayTips::GetMe()->IsMissionInTime( nMissionID );

		Lua_PushNumber( L, nRet );
		return 1 ;

		LUA_LEAVE_FUNCTION
	}

	/** ---------------------------------------------------
	*     ������Ա : liulu
	*     ��дʱ�� : 2010-07-22
	*     �������� : LuaFnMissionCommonCheck
	*					
	*     ����˵�� :
	*			��������Ƿ������ÿ�ջ�Ļ�������
	*			��ÿ�ջʱ�ô˺����ж�
	*
	*/
	INT LuaFnMissionCommonCheck(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		INT  nMissionID;
		BOOL bRet;

		SceneID_t sceneId;
		ObjID_t selfId;

		nMissionID = Lua_ValueToNumber( L, 1 );	
		sceneId    = Lua_ValueToNumber( L, 2 );	
		selfId     = Lua_ValueToNumber( L, 3 );	

		BEGINHUMANDEFINE( "LuaFnMissionCommonCheck" )		

		bRet = DayTips::GetMe()->CommonCheck( nMissionID, pHuman );

		ENDHUMANDEFINE

		if ( bRet == TRUE )
		{
			Lua_PushNumber( L, 1 );
		}
		else
			Lua_PushNumber( L, 0 );
			
		return 1;

		LUA_LEAVE_FUNCTION
	}

	/** ---------------------------------------------------
	*     ������Ա : liulu
	*     ��дʱ�� : 2010-07-23
	*     �������� : LuaFnIsTodayMission
	*					
	*     ����˵�� :
	*			����ÿ�ջ�Ƿ��ǽ��տɼ�����
	*/
	INT LuaFnIsTodayMission(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		INT  nMissionID;
		BOOL bRet;

		nMissionID = Lua_ValueToNumber( L, 1 );	

		bRet = DayTips::GetMe()->IsTodayMission( nMissionID );		

		if ( bRet == TRUE )
		{
			Lua_PushNumber( L, 1 );
		}
		else
			Lua_PushNumber( L, 0 );

		return 1;

		LUA_LEAVE_FUNCTION		
	}

	/** ---------------------------------------------------
	*     ������Ա : liulu
	*     ��дʱ�� : 2010-07-23
	*     �������� : LuaFnSetTimeParam								
	*					
	*     ����˵�� :
	*			�������һ����ɻ��ʱ��
	*			�±��ڽű�ScriptGlobal.lua������
	*
	*/
	INT LuaFnSetTimeParam(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t   selfId;

		INT  nIndex;

		nIndex     = Lua_ValueToNumber( L, 1 );	
		sceneId    = Lua_ValueToNumber( L, 2 );
		selfId     = Lua_ValueToNumber( L, 3 );

		BEGINHUMANDEFINE("LuaFnSetTimeParam")

			if( nIndex >= 0 )
			{
				INT nValue;

				nValue = DayTips::GetMe()->GetTodayFormatDate();

				pHuman->SetMissionData( nIndex, nValue );
				return 0;
			}

		ENDHUMANDEFINE
		 LUA_LEAVE_FUNCTION
	}

	/** ---------------------------------------------------
	*     ������Ա : liulu
	*     ��дʱ�� : 2010-07-30
	*     �������� : LuaFnGetTodayDate								
	*					
	*     ����˵�� :
	*			��ȡ��ǰ���ڣ���������ת����ĸ�ʽ
	*			
	*/
	INT LuaFnGetTodayDate(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		INT nValue;

		nValue = DayTips::GetMe()->GetTodayFormatDate();

		Lua_PushNumber( L, nValue );

		return 1;

		LUA_LEAVE_FUNCTION
	}


}

#endif
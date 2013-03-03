//-----------------------------------------------------------------------------
// �ļ��� : LuaFnTbl_DoAction.h
// ģ��	:	Script
// ����	 :  ��ָ����ɫ��ָ����Ϊ����غ���
// �޸���ʷ:
//	Gaoqi����
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_DOACTION_H__
#define __LUAFNTBL_DOACTION_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "SkillInfo.h"
#include "GameStruct_Skill.h"
#include "StdImpact000.h"
#include "ScriptLogic.h"
#include "Impact_Core.h"
#include "ActionDelegator.h"

using namespace Combat_Module;
using namespace Impact_Module;
using namespace Script_Module;
using namespace Action_Module;

namespace LuaFnTbl
{

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-5-17	11:29
	 *	�������ƣ�	LuaFnDoAction
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *				nActionID
	 *					�������
	 *				TargetID
	 *					Ŀ���ţ�û��Ŀ���� -1 �Ϳ����ˣ�
	 *	����˵����	��Ŀ��������
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT	LuaFnDoAction(Lua_State* L) 
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		SkillID_t nActionID;
		ObjID_t TargetID;

		sceneId		=	Lua_ValueToNumber(L,1);
		selfId		=	Lua_ValueToNumber(L,2);
		nActionID	=	Lua_ValueToNumber(L,3);
		TargetID	=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnDoAction")

			pHuman->DoAction( nActionID, TargetID );
			return 0;

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-1-10
	*     �������� : LuaFnUnitDoEmoteAction
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*				nSkillID
	*					���ܱ��
	*				nTargetID
	*					Ŀ����
	*     ����˵�� :
	*/
	INT	LuaFnUnitDoEmoteAction(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;
		SkillID_t nSkillID;
		ObjID_t nTargetID;

		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);
		nSkillID	=	Lua_ValueToNumber(L,3);
		nTargetID	=	Lua_ValueToNumber(L,4);

		BEGINSCENEDEFINE("LuaFnUnitDoEmoteAction")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			GetGlobalActionDelegator().RegisterEmoteAction(rMe, nSkillID);
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-1-10
	*     �������� : LuaFnUnitUseSkill
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*				nSkillID
	*					���ܱ��
	*				nTargetID
	*					Ŀ����
	*				fPos_X
	*					Ŀ��λ�õ�X����
	*				fPos_Z
	*					Ŀ��λ�õ�Z����
	*				fDir
	*					Ŀ�귽��
	*				bPassSkillCheck
	*					�Ƿ���Լ��ܵ�����������Ʒ����
	*     ����˵�� :
	*/
	INT	LuaFnUnitUseSkill(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;
		SkillID_t nSkillID;
		ObjID_t nTargetID;
		FLOAT fPos_X;
		FLOAT fPos_Z;
		FLOAT fDir;
		BOOL bPassSkillCheck;

		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);
		nSkillID	=	Lua_ValueToNumber(L,3);
		nTargetID	=	Lua_ValueToNumber(L,4);
		fPos_X		=	Lua_ValueToNumber(L,5);
		fPos_Z		=	Lua_ValueToNumber(L,6);
		fDir		=	Lua_ValueToNumber(L,7);
		bPassSkillCheck = 	((0==Lua_ValueToNumber(L,8))?FALSE:TRUE);

		BEGINSCENEDEFINE("LuaFnUnitUseSkill")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			WORLD_POS pos;
			pos.m_fX = fPos_X;
			pos.m_fZ = fPos_Z;
			rMe.Skill_ExecFromScript(nSkillID, nTargetID, pos, fDir, bPassSkillCheck);			
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnSendSpecificImpactToUnit
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*				nSenderID
	*					Ч��������ID,�޷�������
	*				nTargetID
	*					Ŀ���ɫID,�޷�������
	*				nImpactDataIndex
	*					Ч����������,�޷�������
	*				nDelayTime
	*					�ӳ�ʱ��
	*     ����˵�� :
	*				
	*/
	INT	LuaFnSendSpecificImpactToUnit(Lua_State* L)
	{
		__ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;
		ObjID_t nSenderID;
		ObjID_t nTargetID;
		INT nImpactDataIndex;
		INT nDelayTime;

		sceneId				=	Lua_ValueToNumber(L,1);
		nSelfID				=	Lua_ValueToNumber(L,2);
		nSenderID			=	Lua_ValueToNumber(L,3);
		nTargetID			=	Lua_ValueToNumber(L,4);
		nImpactDataIndex	=	Lua_ValueToNumber(L,5);
		nDelayTime			=	Lua_ValueToNumber(L,6);

		BEGINSCENEDEFINE("LuaFnSendSpecificImpactToUnit")
			Obj* pSelf = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pSelf)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pSelf->GetObjType()))
			{
				RETURNFALSE
			}
			Obj* pSender = (Obj*) (pScene->GetObjManager()->GetObj(nSenderID));
			if(NULL==pSender)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pSender->GetObjType()))
			{
				RETURNFALSE
			}
			Obj* pTarget = (Obj*) (pScene->GetObjManager()->GetObj(nTargetID));
			if(NULL==pTarget)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pTarget->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rTar = *(Obj_Character*)pTarget;
			BOOL bRet = g_ImpactCore.SendImpactToUnit(rTar, nImpactDataIndex, nSenderID, nDelayTime);
			Lua_PushNumber(L, bRet);
			return 1;
		ENDSCENEDEFINE
		RETURNFALSE

		__LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnCancelSpecificImpact
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*				nImpactDataIndex
	*					Ч����������, �޷������ͣ��ڱ�׼Ч�������ѯ
	*     ����˵�� :
	*				
	*/
	INT	LuaFnCancelSpecificImpact(Lua_State* L)
	{
		__ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;
		INT nImpactDataIndex;

		sceneId				=	Lua_ValueToNumber(L,1);
		nSelfID				=	Lua_ValueToNumber(L,2);
		nImpactDataIndex	=	Lua_ValueToNumber(L,3);
		
		BEGINSCENEDEFINE("LuaFnCancelSpecificImpact")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			INT nRet = rMe.Impact_DispelSpecificImpactByDataIndex(nImpactDataIndex, 1);
			Lua_PushNumber(L, nRet);
			return 1;

		ENDSCENEDEFINE
		RETURNFALSE

		__LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnVerifyUsedItem
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	INT	LuaFnVerifyUsedItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;

		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnVerifyUsedItem")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			if(Obj::OBJ_TYPE_HUMAN!=pObj->GetObjType())
			{
				RETURNFALSE
			}
			Obj_Human& rMe = *(static_cast<Obj_Human*>(pObj));
			BOOL bRet = rMe.VerifyItem();
			//return the result
			Lua_PushNumber(L, bRet);
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnDepletingUsedItem
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	INT	LuaFnDepletingUsedItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;

		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnDepletingUsedItem")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			BOOL bRet = rMe.DepletingUsedItem();
			//return the result
			Lua_PushNumber(L, bRet);
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGetTargetObjID
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	INT	LuaFnGetTargetObjID(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;

		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetTargetObjID")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetObj());
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGetTargetPosition_X
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	INT	LuaFnGetTargetPosition_X(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;

		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetTargetPosition_X")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetPosition().m_fX);
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGetTargetPosition_Z
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	INT	LuaFnGetTargetPosition_Z(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;

		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetTargetPosition_Z")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetPosition().m_fZ);
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	
	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGetTargetBagSlot
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	INT	LuaFnGetTargetBagSlot(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;

		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetTargetBagSlot")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetBagSlot());
			return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-4-11	19:29
	 *	�������ƣ�	LuaFnGetBagIndexOfUsedItem
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	?
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnGetBagIndexOfUsedItem( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnGetBagIndexOfUsedItem")
			TargetingAndDepletingParams_T& rParams = pHuman->GetTargetingAndDepletingParams();
			
			Lua_PushNumber(L, rParams.GetBagIndexOfDepletedItem());
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-8-16	12:23
	 *	�������ƣ�	LuaFnGetItemIndexOfUsedItem
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	ȡ��ʹ�õ���Ʒ����Ʒ��
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnGetItemIndexOfUsedItem( Lua_State* L )
	{
	LUA_ENTER_FUNCTION
		//Steven.Han start 2006-9-18 16:16
		SceneID_t sceneId;
		ObjID_t selfId;
	    //Steven.Han end   2006-9-18 16:16
		//CHECKPARAM_RET0(2)
		if( FALSE == LuaVerifyAllNumberParam( L, 1, 2, "LuaFnGetItemIndexOfUsedItem" ) )
		{
			RETURNFALSE
		}

		sceneId = Lua_ValueToNumber( L, 1 );
		selfId = Lua_ValueToNumber( L, 2 );

		BEGINHUMANDEFINE("LuaFnGetItemIndexOfUsedItem")
			TargetingAndDepletingParams_T& rParams = pHuman->GetTargetingAndDepletingParams();
			
			Lua_PushNumber( L, rParams.GetItemIndexOfDepletedItem() );
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGetLowSectionOfTargetPetGuid
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	INT	LuaFnGetLowSectionOfTargetPetGuid(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;

		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetLowSectionOfTargetPetGuid")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetPetGuid().GetLowSection());
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGetHighSectionOfTargetPetGuid
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	INT	LuaFnGetHighSectionOfTargetPetGuid(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;

		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetHighSectionOfTargetPetGuid")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetPetGuid().GetHighSection());
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	
	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnIsScriptDynamicParamIndexLegal
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*				nIndex
	*					��������
	*     ����˵�� :
	*				
	*/
	INT	LuaFnIsScriptDynamicParamIndexLegal(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;
		INT nIndex;

		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);
		nIndex		=	Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnIsScriptDynamicParamIndexLegal")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.IsDynamicParamIndexLegal(nIndex));
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGetScriptDynamicParamByIndex
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*				nIndex
	*					��������
	*     ����˵�� :
	*				
	*/
	INT	LuaFnGetScriptDynamicParamByIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;
		INT nIndex;
		
		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);
		nIndex		=	Lua_ValueToNumber(L,3);
		INT nRet = 0;
		BEGINSCENEDEFINE("LuaFnGetScriptDynamicParamByIndex")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			if(TRUE==rParams.IsDynamicParamIndexLegal(nIndex))
			{
				//return the specific param
				Lua_PushNumber(L, rParams.GetDynamicParamByIndex(nIndex));
				return 1 ;
			}
			else
			{
				Lua_PushNumber(L, -1);
				return 1 ;
			}
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : ����
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnSetScriptDynamicParamByIndex
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*				nIndex
	*					��������
	*				nValue
	*					����ֵ
	*     ����˵�� :
	*				
	*/
	INT	LuaFnSetScriptDynamicParamByIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;
		INT nIndex;
		INT nValue;

		sceneId	=	Lua_ValueToNumber(L,1);
		nSelfID	=	Lua_ValueToNumber(L,2);
		nIndex	=	Lua_ValueToNumber(L,3);
		nValue	=	Lua_ValueToNumber(L,4);
		BEGINSCENEDEFINE("LuaFnSetScriptDynamicParamByIndex")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			rParams.SetDynamicParamByIndex(nIndex, nValue);
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	
}

#endif

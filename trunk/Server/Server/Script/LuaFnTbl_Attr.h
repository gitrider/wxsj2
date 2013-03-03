//-----------------------------------------------------------------------------
// �ļ��� : LuaFnTbl_Attr.h
// ģ��	:	Script
// ����	 :  ������غ���
// �޸���ʷ:
//	yangfei����
//-----------------------------------------------------------------------------

#ifndef __LUAFNTBL_ATTR_H__
#define __LUAFNTBL_ATTR_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "GCNotifyChangeScene.h"
#include "GCDetailAttrib_Pet.h"
#include "GCChat.h"
#include "AI_Human.h"
#include "HumanItemLogic.h"
#include "PetManager.h"
#include "Obj_Pet.h"
#include "ItemOperator.h"
#include "AI_Monster.h"

#include "GCDetailSkillList.h"
#include "GamePlayer.h"

namespace LuaFnTbl
{
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetPos
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
    *					���øú�������ID,short����
	*				selfId
	*					������ID,�޷�������
	*				x
	*					��Ҫwarp������λ�õ�X����ֵ
	*				z
	*					��Ҫwarp������λ�õ�Z����ֵ
	*     ����˵�� :
	*				��ɫ��ͬ����ת��
	*				���ý�ɫ(��Һ͹���)��ת����ǰ��ͼ�ϵ�ĳ��
	*/
	INT LuaFnSetPos( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		FLOAT x;
		FLOAT z;
		
		if(!Lua_IsNumber(L,1) || !Lua_IsNumber(L,2) || !Lua_IsNumber(L,3) || !Lua_IsNumber(L,4))
		{
			RETURNFALSE
		}

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		x = Lua_ValueToNumber(L,3);
		z = Lua_ValueToNumber(L,4);

		BEGINCHARCTERDEFINE("LuaFnSetPos")

		WORLD_POS Old = *pCharacter->getWorldPos() ;
		WORLD_POS New(x,z);

		pScene->GetMap()->VerifyPos( &New ) ;
		if( !pScene->GetMap()->IsCanGo( New ) )
		{
			return 0;
		}

		((Obj_Human*)(pCharacter))->GetHumanAI()->PushCommand_Idle( ) ;
		pCharacter->Teleport( &New ) ;

		return 0;
		ENDCHARCTERDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetRevPos
	*     ����˵�� : 
	*				L
	*				LuaStateִ�л���ָ��
	*				sceneId
	*				���øú�������ID,short
	*				selfId
	*				������ID,�޷�������
	*				x
	*					λ��x����
	*				z
	*					λ��z����
	*     ����˵�� :
	*				������ҵ������㣨�ٴε��뵱ǰ�����λ�ã�
	*				��������д�ϸ��
	*/
	INT LuaFnSetRevPos( Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		FLOAT x;
		FLOAT z;
	
		sceneId = Lua_ValueToNumber(L,1);
		selfId	= Lua_ValueToNumber(L,2);
		x		= Lua_ValueToNumber(L,3);
		z		= Lua_ValueToNumber(L,4);

		BEGINCHARCTERDEFINE("LuaFnSetRevPos")
			if( pCharacter->GetObjType()!=Obj::OBJ_TYPE_MONSTER )
				return 0 ;
			Obj_Monster* pMonster = (Obj_Monster*)pCharacter ;
			WORLD_POS pos(x,z) ;
			pMonster->SetRespawnPos(&pos) ;
		ENDCHARCTERDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnNewWorld
	*     ����˵�� : 
	*				L
	*				LuaStateִ�л���ָ��
	*				sceneId
	*				���øú�������ID,short
	*				selfId
	*				������ID,�޷�������
	*				targetSceneId
	*				��Ҫ��ת����ID,short
	*				x
	*					λ��x����
	*				z
	*					λ��z����
	*     ����˵�� :
	*				�����л�����
	*				��selfId����Ҵ�sceneId��־�ĳ�����ת��targetSceneId��־�ĳ�����(x,z)λ��
	*				ʾ��:NewWorld(sceneId,selfId,4,100,102)
	*/
	INT LuaFnNewWorld(Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		SceneID_t targetSceneId;
		FLOAT x;
		FLOAT z;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		targetSceneId = Lua_ValueToNumber(L,3);
		x = Lua_ValueToNumber(L,4);
		z = Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnNewWorld")
		if( pHuman->getScene()->SceneID() == targetSceneId )
		{
			return 0 ;
		}

		WORLD_POS New(x,z);
		pHuman->ChangeScene( pHuman->getScene()->SceneID(), targetSceneId, New, 9 ) ;

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetWorldPos
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
    *				�õ���ɫ(character)��ǰ����
	*				����x��z��FLOAT����ֵ
	*				ʾ��: x,z = GetWorldPos(sceneId,selfId) 
	*/
	INT LuaFnGetWorldPos(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetWorldPos")
		Lua_PushNumber(L,(int)(pCharacter->getWorldPos()->m_fX));
		Lua_PushNumber(L,(int)(pCharacter->getWorldPos()->m_fZ));

		return 2;
		ENDCHARCTERDEFINE

		Lua_PushNumber(L,-1);
		Lua_PushNumber(L,-1);
		return 2;

		LUA_LEAVE_FUNCTION

		Lua_PushNumber(L,-1);
		Lua_PushNumber(L,-1);
		return 2;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : �¾�
	*     ��дʱ�� : 2010-6-7
	*     �������� : LuaFnGetSceneNPCPos
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				uDataID
	*					�޷�������
	*     ����˵�� :
    *				�õ���ɫ(character)��ǰ����
	*				����x��z��FLOAT����ֵ
	*				ʾ��: x,z = GetWorldPos(sceneId,npcId) 
	*/
	INT LuaFnGetSceneNPCPos(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		UINT uDataID;

		sceneId = Lua_ValueToNumber(L,1);
		uDataID = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetSceneNPCPos")

		ObjID_t	MonsterID = pScene->GetMonsterManager()->FindMonsterByDataID(uDataID);

		if(MonsterID != INVALID_ID)
		{
			Obj_Monster* pMonster = pScene->GetMonsterManager()->GetMonster(MonsterID);
			if(pMonster)
			{
				Lua_PushNumber(L,(int)pMonster->getWorldPos()->m_fX);
				Lua_PushNumber(L,(int)pMonster->getWorldPos()->m_fZ);
				return 2;
			}
		}
		
		ENDSCENEDEFINE

		Lua_PushNumber(L,-1);
		Lua_PushNumber(L,-1);
		return 2;

		LUA_LEAVE_FUNCTION

		Lua_PushNumber(L,-1);
		Lua_PushNumber(L,-1);
		return 2;
	}


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetPatrolId
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				����Obj_Monster��Ѳ��·��
	*				ʾ��:SetPatrolId(sceneId,selfId,0)
	*				patrolId����Чֵ�� 
	*				0 �� ��sceneId��ָ����_patrolpoint.ini�ļ�PATROLNUMBER(������)֮���ֵ
	*/
	INT LuaFnSetPatrolId(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT patrolId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		patrolId = Lua_ValueToNumber(L,3);

		BEGINMONSTERDEFINE("LuaFnSetPatrolId")
		if(patrolId >= 0)
		{
			pMonster->SetPatrolId(patrolId);
			pMonster->GetMonsterAI()->StartPatrol(TRUE);
		}
		else
		{
			pMonster->GetMonsterAI()->StopPatrol();
			pMonster->setDir( pMonster->GetRespawnDir() );
			pMonster->SetPatrolId(INVALID_ID);
		}
		return 0;
		ENDMONSTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnIsCaptain
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�жϵ�ǰ����Ƿ�Ϊ�ӳ�������ֵΪ0�����Ƕӳ�������ֵΪ1���ӳ���
	*				û�в���
	*/
	INT LuaFnIsCaptain( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnIsCaptain")
		if(pHuman->GetTeamInfo()->Leader()->m_GUID == pHuman->GetGUID())
		{
			Lua_PushNumber(L,1);
		}
		else
		{
			Lua_PushNumber(L,0);
		}
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE
		
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddXinFa
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				xinfaId
	*					�ķ����
	*     ����˵�� :
	*				����ǰ��Ҽ�ĳ���ķ�
	*				û�в���
	*/
	INT LuaFnAddXinFa( Lua_State* L)	
	{
		
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT xinfaId;
	
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		xinfaId = Lua_ValueToNumber(L,3);

		BEGINCHARCTERDEFINE("LuaFnAddXinFa")

		BOOL bRet = ((Obj_Human*)pCharacter)->Skill_SetXinFaLevel(xinfaId,1);
		Lua_PushNumber(L,bRet);
		if(bRet)
		{
			Obj_Human* pHuman	=	(Obj_Human*)pCharacter;
			Assert(pHuman);
			
				XINFA_LOG_PARAM XinFaLogParam;
				XinFaLogParam.CharGUID	=	pHuman->GetGUID();
				XinFaLogParam.SceneID	=	pHuman->getScene()->SceneID();
				XinFaLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				XinFaLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				XinFaLogParam.OPType	=	XINFA_OP_SCRIPT_STUDY;
				XinFaLogParam.Count		=	pHuman->Skill_GetXinFaList().m_Count;
				XinFaLogParam.XinFaID	=	xinfaId;
				XinFaLogParam.Level		=	1;
				SaveXinFaLog(&XinFaLogParam);
		}
		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddSkill
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				skillId
	*					���ܱ��
	*     ����˵�� :
	*				����ǰ��Ҽ�ĳ������
	*				û�в���
	*/
	INT LuaFnAddSkill( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT skillId;
			
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		skillId = Lua_ValueToNumber(L,3);

		BEGINCHARCTERDEFINE("LuaFnAddSkill")	
		BOOL bRet = ((Obj_Human*)pCharacter)->Skill_ModifySkill(skillId,1);
		Lua_PushNumber(L,bRet);
		
		if(bRet)
		{
			Obj_Human* pHuman = (Obj_Human*)pCharacter;
			Assert(pHuman);

			SKILL_LOG_PARAM SkillLogParam;
			SkillLogParam.CharGUID	=	pHuman->GetGUID();
			SkillLogParam.SceneID	=	pHuman->getScene()->SceneID();
			SkillLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			SkillLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SkillLogParam.OPType	=	SKILL_OP_SCRIPT_STUDY;
			SkillLogParam.Count		=	pHuman->Skill_GetSkillList().m_Count;
			SkillLogParam.SkillID	=	skillId;
			SkillLogParam.Level		=	1;

			SaveSkillLog(&SkillLogParam);
		}

		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDelXinFa
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				xinfaId
	*					�ķ����
	*     ����˵�� :
	*				ɾ����ǰ��ҵ�ĳ���ķ�
	*				û�в���
	*/
	INT LuaFnDelXinFa( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT xinfaId;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		xinfaId = Lua_ValueToNumber(L,3);

		BEGINCHARCTERDEFINE("LuaFnDelXinFa")	

			BOOL bRet  =((Obj_Human*)pCharacter)->Skill_SetXinFaLevel(xinfaId,-1);	
			Lua_PushNumber(L,bRet);
			if(bRet)
			{
				Obj_Human* pHuman	=	(Obj_Human*)pCharacter;
				Assert(pHuman);
				XINFA_LOG_PARAM XinFaLogParam;
				XinFaLogParam.CharGUID	=	pHuman->GetGUID();
				XinFaLogParam.SceneID	=	pHuman->getScene()->SceneID();
				XinFaLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				XinFaLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				XinFaLogParam.OPType	=	XINFA_OP_SCRIPT_ABANDON;
				XinFaLogParam.Count		=	pHuman->Skill_GetXinFaList().m_Count;
				XinFaLogParam.XinFaID	=	xinfaId;
				XinFaLogParam.Level		=	-1;
				SaveXinFaLog(&XinFaLogParam);
			}

			return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDelSkill
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				skillId
	*					���ܱ��				
	*     ����˵�� :
	*				ɾ����ǰ��ҵ�ĳ������
	*				û�в���
	*/
	INT LuaFnDelSkill( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT skillId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		skillId = Lua_ValueToNumber(L,3);

		BEGINCHARCTERDEFINE("LuaFnDelSkill")
		
		BOOL bRet = ((Obj_Human*)pCharacter)->Skill_ModifySkill(skillId,-1);
		Lua_PushNumber(L,bRet);
		
		if(bRet)
		{
			Obj_Human* pHuman = (Obj_Human*)pCharacter;
			Assert(pHuman);

			SKILL_LOG_PARAM SkillLogParam;
			SkillLogParam.CharGUID	=	pHuman->GetGUID();
			SkillLogParam.SceneID	=	pHuman->getScene()->SceneID();
			SkillLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			SkillLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SkillLogParam.OPType	=	SKILL_OP_SCRIPT_STUDY;
			SkillLogParam.Count		=	pHuman->Skill_GetSkillList().m_Count;
			SkillLogParam.SkillID	=	skillId;
			SkillLogParam.Level		=	-1;
			SaveSkillLog(&SkillLogParam);
		}

		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnHaveXinFa
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				xinfaId
	*					�ķ����				
	*     ����˵�� :
	*				�жϵ�ǰ����Ƿ���ĳ���ķ���δѧ���ü��ܷ���-1�����򷵻��ķ��ȼ�
	*				û�в���
	*/
	INT LuaFnHaveXinFa( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT xinfaId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		xinfaId = Lua_ValueToNumber(L,3);

		BEGINCHARCTERDEFINE("LuaFnHaveXinFa")
		if(FALSE==((Obj_Human*)pCharacter)->Skill_GetXinFaLevel(xinfaId))
		{
			Lua_PushNumber(L,((Obj_Human*)pCharacter)->Skill_GetXinFaLevel(xinfaId));//gaoqi: �����ǲ���������?
			return	1;
		}
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnHaveSkill
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				skillId
	*					���ܱ��				
	*     ����˵�� :
	*				�жϵ�ǰ����Ƿ���ĳ�����ܡ�δѧ���ü��ܷ���-1�����򷵻ؼ��ܵȼ���//gaoqi? ����û�еȼ�
	*/
	INT LuaFnHaveSkill( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT skillId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		skillId = Lua_ValueToNumber(L,3);

		BEGINCHARCTERDEFINE("LuaFnHaveSkill")
		if(pCharacter->Skill_HaveSkill(skillId))
		{
			Lua_PushNumber(L,1);//gaoqi: �����ǲ���������?
			return 1;
		}
		else
		{
			Lua_PushNumber(L,-1);
			return 1;
		}
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnUseSkill
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				skillId
	*					���ܱ��				
	*     ����˵�� :
	*				�д���߻���ȶ
	*/
	INT LuaFnUseSkill( Lua_State* L)	
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		INT skillId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		skillId = Lua_ValueToNumber(L,3);

		BEGINCHARCTERDEFINE("LuaFnUseSkill")
			AssertEx(FALSE,"�˺���������") ;
			if(1)
			{
				Lua_PushNumber(L,1);//gaoqi: �����ǲ���������?
				return 1;
			}
			ENDCHARCTERDEFINE
				RETURNFALSE

	LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : �����
	*     ��дʱ�� : 2008-3-20
	*     �������� : LuaFnGetCurCountry
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�����ҵĵ�ǰ����
	*				û�в���
	*/
	INT LuaFnGetCurCountry( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetCurCountry")
			Lua_PushNumber(L,pHuman->GetCountry());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}



	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetCurCamp
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�����ҵĵ�ǰ��Ӫ
	*				û�в���
	*/
	INT LuaFnGetCurCamp( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetCurCamp")
		Lua_PushNumber(L,pHuman->GetCampID());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetCurCamp
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				campIndex
	*					��Ӫ���,INT����
	*     ����˵�� :
	*				�޸������Ӫ,��������δ��д
	*/
	INT LuaFnSetCurCamp ( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT campIndex ;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		campIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetCurCamp")
		pHuman->SetBaseCampID(campIndex);//���룺���������һ��
		return 0;
		ENDHUMANDEFINE
		
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-10	19:42
	 *	�������ƣ�	LuaFnGetGUID
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�õ������ߵ� GUID
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnGetGUID( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetGUID")	
		Lua_PushNumber(L, ((Obj_Human*)pCharacter)->GetGUID());
		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetExp
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�����ҵĵ�ǰս������ֵ
	*/
	INT LuaFnGetExp( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetExp")	
		Lua_PushNumber(L, ((Obj_Human*)pCharacter)->GetExp());
		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddExp
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				exp
	*					����ֵ����,INT
	*     ����˵�� :
	*				������ҵ�ս������ֵ
	*/
	INT LuaFnAddExp( Lua_State* L)	
	{	
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT exp;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		exp = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnAddExp")
		pHuman->SetAlterExp( exp ) ;
		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetHp
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�����ҵ�����ֵ
	*				������ҵ�HPֵ
	*/
	INT LuaFnGetHp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetHp")	
		Lua_PushNumber(L, pCharacter->GetHP());
		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE
		
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnRestoreHp
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*		
	*				percent
	*					�ָ��İٷֱ�,Ϊ0ʱ��ԭ�����߼�����0ʱֱ��SetMP
	*     ����˵�� :
	*				�ָ���ɫ������ȫ��
	*				�ѽ�ɫ������ֵ��ΪMaxHP*percent/100ֵ
	*/
	INT LuaFnRestoreHp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT percent;
			
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		percent	= Lua_ValueToNumber(L,3);

		BEGINCHARCTERDEFINE("LuaFnRestoreHp")

		// 20100414 AddCodeBegin ��Ѫʱ����Ҫ�ָ���ֱ�ӷ��أ����غ��������Ҫ�ָ�����ʾ
		if ( pCharacter->GetHP() == pCharacter->GetMaxHP() )
		{
			return 1;
		}
		// 20100414 AddCodeEnd

		if ( percent == 0 )
			pCharacter->HealthIncrement(pCharacter->GetMaxHP());
		else
		{
			INT Increment = ( pCharacter->GetMaxHP()*percent ) / 100;
			pCharacter->HealthIncrement( Increment-pCharacter->GetHP() );
		}

		return 0;
		ENDCHARCTERDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMp
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��ý�ɫ������ֵ
	*				���ؽ�ɫ��MPֵ
	*/
	INT LuaFnGetMp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetMp")
		Lua_PushNumber(L, pCharacter->GetMP());
		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnRestoreMp
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				percent
	*					�ָ��İٷֱ�
	*     ����˵�� :
	*				�ָ���ҵ�����ΪMaxMP*percent/100
	*/
	INT LuaFnRestoreMp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT percent;
			
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		percent =  Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnRestoreMp")
		
		// 20100414 AddCodeBegin ��ħʱ����Ҫ�ָ���ֱ�ӷ��أ����غ��������Ҫ�ָ�����ʾ
		if ( pCharacter->GetMP() == pCharacter->GetMaxHP() )
		{
			return 1;
		}
		// 20100414 AddCodeEnd

		if ( percent == 0 )
			percent = 100;
		pHuman->SetMP( (pHuman->GetMaxMP() *percent)/100 );

		GCDetailHealsAndDamages msg;
		msg.SetReceiverID(pHuman->GetID());
		msg.SetSenderID(INVALID_ID);
		msg.SetSenderLogicCount(0);

		INT Increment = ( pCharacter->GetMaxMP()*percent ) / 100;
		msg.SetMpModification(Increment-pCharacter->GetMP());
		pHuman->GetPlayer()->SendPacket(&msg ) ;

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetCon
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�����ҵ�����ֵ
	*/
	INT LuaFnGetCon(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetCon")
		Lua_PushNumber(L,pHuman->GetCon());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetSex
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�����ҵ��Ա�
	*				����ֵΪ0��Ů
	*				����ֵΪ1����
	*/
	INT LuaFnGetSex(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnGetSex")
		Lua_PushNumber(L,pHuman->GetSex());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetName
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��ý�ɫ(Obj_Character)���֣�����һ���ַ���
	*				��ɫ������Һ�Obj_Monster
	*				ʾ��: npcName = GetName(sceneId,npcId)
	*/
	INT LuaFnGetName(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		
		BEGINCHARCTERDEFINE("LuaFnGetName")
		Lua_PushString(L,pCharacter->GetName());
		return 1;
		ENDCHARCTERDEFINE
		Lua_PushString(L,"");
		return 1;
		
		LUA_LEAVE_FUNCTION

		Lua_PushString(L,"");
		return 1;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMoney
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�����ҵ��ֽ�
	*				�������ӵ�еĽ�Ǯ��
	*/
	INT LuaFnGetMoney(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetMoney")
		Lua_PushNumber(L,pHuman->GetMoney());	
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnCostMoney
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				payment
	*					��Ǯ����
	*     ����˵�� :
	*				�۳���ҽ�Ǯ
	*				�ɹ�����1
	*				�Ժ��Ƿ��Ƿ����޸ĺ����ҽ�Ǯ��?
	*				ĿǰpaymentӦ��Ϊ��ֵ
	*				ʧ�ܷ���-1
	*/
	INT LuaFnCostMoney(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT payment;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		payment = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnCostMoney")
		if( pHuman->GetMoney() >= payment)
		{
			pHuman->SetMoney( pHuman->GetMoney() - payment);
			

			MONEY_LOG_PARAM	MoneyLogParam;
			MoneyLogParam.CharGUID	=	pHuman->GetGUID();
			MoneyLogParam.OPType	=	MONEY_SCRIPT_COST;	
			MoneyLogParam.Count		=	payment;
			MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
			MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SaveMoneyLog(&MoneyLogParam);

			Lua_PushNumber(L,1);	
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
	*     �������� : LuaFnAddMoney
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				profit
	*					��Ǯ������Ŀ
	*     ����˵�� :
	*				������ҽ�Ǯ
	*				����1,�ɹ�
	*				�Ժ��Ƿ��Ƿ����޸ĺ����ҽ�Ǯ��?
	*				����-1,ʧ��
	*/
	INT LuaFnAddMoney(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT profit;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		profit = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnAddMoney")

		//������Ǯ��á����ߵ�Ӱ��
		_ITEM_EFFECT* pIE = pHuman->ItemEffect(IATTRIBUTE_MONEY_GET);
		Assert(pIE);
		if(pIE->IsActive())
		{
			profit = Float2Int(profit * (100 + pIE->m_Attr.m_Value) /100.0f);
		}

		pHuman->SetMoney( pHuman->GetMoney() + profit);

		MONEY_LOG_PARAM	MoneyLogParam;
		MoneyLogParam.CharGUID	=	pHuman->GetGUID();
		MoneyLogParam.OPType	=	MONEY_SCRIPT_COST;	
		MoneyLogParam.Count		=	profit;
		MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
		MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
		MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
		SaveMoneyLog(&MoneyLogParam);

		Lua_PushNumber(L,1);	
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetLevel
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				ȡ����ҵ�ǰ�ȼ�
	*				Level�ĺϷ���Χ��?-?
	*				����-1,ʧ��
	*/
	INT LuaFnGetLevel(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetLevel")	
		Lua_PushNumber(L,pCharacter->GetLevel());
		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	

	/** -----------------------------------------------------------------------
	*     ������Ա : ��
	*     ��дʱ�� : 2005-10-20
	*     �������� : LuaFnSetLevel
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				level
	*					��ɫ����,INT
	*     ����˵�� :
	*				���������ҵȼ�
	*				Level�ĺϷ���Χ��1-60
	*				����-1,ʧ��
	*/
	INT LuaFnSetLevel(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT level;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		level = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetLevel")	
		
		AssertEx(level > 0 && level <= DEFAULT_WASHPOINT_LEVEL,"level wrong");
		if( level <=0 || level > DEFAULT_WASHPOINT_LEVEL)
		{
			RETURNFALSE
		}
		
		pHuman->SetLevel(level);

		Lua_PushNumber(L,1);
		return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		
		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-7	16:36
	 *	�������ƣ�	LuaFnCanLevelUp
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�жϸ�����Ƿ��������
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnCanLevelUp(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnCanLevelUp")
			if( pHuman->GetExp() >= (UINT)g_LevelExpTbl.Get(pHuman->GetLevel() - 1) )
			{
				Lua_PushNumber(L, TRUE);
			}
			else
			{
				Lua_PushNumber(L, FALSE);
			}

			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-28
	*     �������� : LuaFnGetItemTableIndexByIndex
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				itemIndex
	*					��Ʒ�����ϵ�λ������
	*     ����˵�� :
	*				ȡ��ǰλ�õ���Ʒ����
	*				
	*/
	INT LuaFnGetItemTableIndexByIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT itemIndex;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		itemIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetItemTableIndexByIndex")
			
		Item*	pItem = 	HumanItemLogic::GetBagItem(pHuman, itemIndex);
		INT itemType = pItem->GetItemTableIndex();

		Lua_PushNumber(L, itemType);
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-30	14:47
	 *	�������ƣ�	LuaFnGetMaterialStartBagPos
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�õ������в��ϵ���ʼλ��
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnGetMaterialStartBagPos(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;

		/*sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnGetMaterialStartBagPos")
			Lua_PushNumber(L, MAT_CONTAINER_OFFSET);
			return 1;
		ENDHUMANDEFINE*/
		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-30	14:47
	 *	�������ƣ�	LuaFnGetMaterialEndBagPos
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�õ������в��ϵĽ���λ��
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnGetMaterialEndBagPos(Lua_State* L)	
	{
	/*LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnGetMaterialEndBagPos")
			Lua_PushNumber(L, TASK_CONTAINER_OFFSET-1);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

	LUA_LEAVE_FUNCTION*/

		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-3-30	19:30
	 *	�������ƣ�	LuaFnGetItemCountInBagPos
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				uBagPos
	 *					������λ��
	 *
	 *	����˵����	�õ�����ĳ������Ʒ������
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnGetItemCountInBagPos(Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT uBagPos;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		uBagPos = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnGetItemCountInBagPos")

			Item* pItem = HumanItemLogic::GetBagItem(pHuman, uBagPos);
			if (pItem != NULL)
			{
				Lua_PushNumber(L, pItem->GetLayedNum());
				return 1;
			}
			return 0;

		ENDHUMANDEFINE
			RETURNFALSE

			LUA_LEAVE_FUNCTION

			RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetItemCount
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				itemTypeSn
	*					ItemType��9λ����(ʵ��Ϊ8λ)
	*     ����˵�� :
	*				�жϵ�ǰ����ж��ٸ�����
	*				ֻ����ҵ�װ������Ʒ���в���
	*/
	INT LuaFnGetItemCount(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT itemTypeSn;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		itemTypeSn = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetItemCount")
		INT count =0;
				
		count = HumanItemLogic::CalcEquipItemCount(pHuman,itemTypeSn);
		
		count += HumanItemLogic::CalcBagItemCount(pHuman,itemTypeSn);

		Lua_PushNumber(L,count);
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}















	/** -----------------------------------------------------------------------
	*     ������Ա : �����
	*     ��дʱ�� : 2008-4-22
	*     �������� : LuaFnGetPKValue
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�õ���ҵ�ǰPKֵ
	*/
	INT LuaFnGetPKValue(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetPKValue")
		Lua_PushNumber(L,pHuman->GetPKValue());
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}




	/** -----------------------------------------------------------------------
	*     ������Ա : �����
	*     ��дʱ�� : 2008-4-22
	*     �������� : LuaFnSetPKValue
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				Val:
	*					����µ�PKֵ
	*     ����˵�� :
	*				������ҵ�ǰPKֵ
	*/
	INT LuaFnSetPKValue(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT Val;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		Val    = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetPKValue")
		
		pHuman->SetPKValue(Val);

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0;
	}















	/** -----------------------------------------------------------------------
	*     ������Ա : yf
	*     ��дʱ�� : 2005-1-22
	*     �������� : LuaFnHaveItemInBag
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				itemTypeSn
	*					ItemType��9λ����(ʵ��Ϊ8λ)
	*     ����˵�� :
	*				�жϵ�ǰ��Ұ����Ƿ�ӵ��ĳ������
	*				����ɹ�,���ش���0
	*				���ʧ��,����-1
	*/
	INT LuaFnHaveItemInBag ( Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT itemTypeSn;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		itemTypeSn = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnHaveItemInBag")
		if(HumanItemLogic::CalcBagItemCount(pHuman,itemTypeSn))
		{
			Lua_PushNumber(L,1);
			return 1;			
		}
		else
		{
			Lua_PushNumber(L,-1);
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
	*     �������� : LuaFnHaveItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				itemTypeSn
	*					ItemType��9λ����(ʵ��Ϊ8λ)
	*     ����˵�� :
	*				�жϵ�ǰ����Ƿ�ӵ��ĳ������
	*				����ɹ�,���ش���0
	*				���ʧ��,����-1
	*/
	INT LuaFnHaveItem ( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		INT itemTypeSn;
	
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		itemTypeSn = Lua_ValueToNumber(L,3);
		
		BEGINHUMANDEFINE("LuaFnHaveItem")
		//INT count = 0;
	
		//�ȴ�װ��������
		for(INT i=0;i<HEQUIP_NUMBER;i++)
		{
			if( pHuman->GetDB()->GetEquipItem((HUMAN_EQUIP)i)->m_ItemIndex == itemTypeSn)
			{
				Lua_PushNumber(L,1);
				return 1;	
			}
		}
	
		if(HumanItemLogic::CalcBagItemCount(pHuman,itemTypeSn))
		{
			Lua_PushNumber(L,1);
			return 1;			
		}
		
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2008-4-28
	*     �������� : LuaFnUseItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				itemTypeSn
	*					ItemType��9λ����(ʵ��Ϊ8λ)
	*     ����˵�� :
	*				���ʹ��ĳ������
	*				����ɹ�,���ش���0
	*				���ʧ��,����-1
	*/
	INT LuaFnUseItem ( Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT itemTypeSn;

			sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		itemTypeSn = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnUseItem")
			//INT count = 0;
		if (GetSerialClass(itemTypeSn) == ICLASS_EQUIP)
		{
			INT BagIndex = HumanItemLogic::GetBagItemPosByType(pHuman, itemTypeSn);
			if (BagIndex == INVALID_INDEX)
			{
				Lua_PushNumber(L,0);
				return 1;
			}
			
			Item* pUseItem = HumanItemLogic::GetBagItem(pHuman, BagIndex);
			Assert(pUseItem);
			if (pUseItem->IsEmpty())
			{
				Lua_PushNumber(L,0);
				return 1;
			}

			INT nPos = pHuman->UseEquip(BagIndex);
			if (nPos == INVALID_INDEX)
			{
				Lua_PushNumber(L,0);
				return 1;
			}
		}
			
		ENDHUMANDEFINE
			RETURNFALSE

			LUA_LEAVE_FUNCTION
			RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDelItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				itemTypeSn
	*					ItemType��9λ����(ʵ��Ϊ8λ)
	*				delCount
	*					ɾ����Ŀ
	*     ����˵�� :
	*				ɾ����ǰ��ҵ�ĳ������	
	*				������ָ�������	
	*				������װ���ĵ���ɾ��
	*				ɾ���ɹ�,����1
	*				ɾ��ʧ��,����-1
	*/
	INT LuaFnDelItem ( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT itemIndex;
		INT delCount;
			
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		itemIndex = Lua_ValueToNumber(L,3);
		delCount = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnDelItem")
		//TSerialHelper help(itemIndex);
		//_ITEM_TYPE	itemType =	help.GetItemTypeStruct();

		//pHuman->EraseItem(itemType,delCount);
		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType	=	ITEM_SCRIPT_DELETE;
		if( HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,itemIndex,delCount)==TRUE )
		{
			Lua_PushNumber(L,1);
			return 1;
		}
		else
		{
			Lua_PushNumber(L,0);
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
	*     �������� : LuaFnEraseItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				bagIndex
	*					�����ĸ���
	*     ����˵�� :
	*				ɾ����ǰ��ұ���ĳһ��ĵ���
	*				ɾ���ɹ�,����1
	*				ɾ��ʧ��,����-1
	*/
	INT LuaFnEraseItem ( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		uint bagIndex;
			
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		bagIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnEraseItem")
		
		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType	=	ITEM_SCRIPT_DELETE;
		if( HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,bagIndex) == TRUE )
		{
			SaveItemLog(&ItemLogParam);
			Lua_PushNumber(L,1);
			return 1;
		}
		else
		{
			Lua_PushNumber(L,0);
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
	*     �������� : LuaFnGetMenPai
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��ȡ��ҵ�ǰ��������ID
	*/
	INT LuaFnGetMenPai(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetMenPai")
		Lua_PushNumber(L,pHuman->GetMenPai());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-10-13
	*     �������� : LuaFnSetMenPai
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				menpaiId
	*					����ID,INT
	*     ����˵�� :
	*				������ҵ�ǰ��������ΪmenpaiId
	*/
	INT LuaFnSetMenPai(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT menpaiId;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		menpaiId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetMenPai")
		
		AssertEx(menpaiId < MATTRIBUTE_NUMBER && menpaiId >= MATTRIBUTE_SHAOLIN_GWS ,"��������Խ��");
		if ( menpaiId >= MATTRIBUTE_NUMBER && menpaiId < MATTRIBUTE_SHAOLIN_GWS)
		{
			RETURNFALSE
		}

		pHuman->SetMenPai(menpaiId);
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : yangei
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMenPaiMaster
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��ȡ������ڰ��İ������ƣ��ַ�����
	*				������û��ʵ��
	*/
	INT LuaFnGetMenPaiMaster(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetMenPaiMaster")
		//pHuman->GetMenPaiName();
		//Lua_PushNumber(L,number);
		//Lua_PushString(L,str);
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMenPaiFigure
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��������ڰ���еĽ׼���0���ڣ�1�ӳ���2���ϣ�4����
	*				������û��ʵ��
	*/
	INT LuaFnGetMenPaiFigure(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnGetMenPaiFigure")
		//pHuman->GetMenPaiName();
		//Lua_PushNumber(L,number);
		//Lua_PushString(L,str);
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMenPaiTitle
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��������ڰ���е�ͷ�Σ�����ͷ���в����������
	*				������û��ʵ��
	*/
	INT LuaFnGetMenPaiTitle(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnGetMenPaiTitle")
		//pHuman->GetMenPaiName();
		//Lua_PushNumber(L,number);
		//Lua_PushString(L,str);
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDestroyMenPai
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				menpaiId
	*					�����
	*     ����˵�� :
	*				��ɢ���
	*				������û��ʵ��
	*/
	INT LuaFnDestroyMenPai(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT menpaiId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		menpaiId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDestroyMenPai")
		//INT value = pHuman->GetTask(misId); 
		//Lua_PushNumber(L,value);
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetNpcCurCamp
	*     ����˵�� : 
	*     ����˵�� :
	*				�ı�NPC����Ӫ,��������δ��д
	*/
	INT LuaFnSetNpcCurCamp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		INT npcIndex;
		INT campId ;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnRestoreNpcCamp
	*     ����˵�� : 
	*     ����˵�� :
	*				�ָ�NPCԭ�е���Ӫ,��������δ��д
	*/
	INT LuaFnRestoreNpcCamp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		INT npcindex;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-1-23
	*     �������� : LuaFnSetDamage
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					�Լ���ObjID
	*				targetId
	*					Ŀ���ObjID
	*				damage
	*					��Ҫ���õ��˺�
	*     ����˵�� :
	*				����ĳ��obj���˺�
	*/
	INT LuaFnSetDamage(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;
		INT damage;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		targetId = Lua_ValueToNumber(L,3);
		damage = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnSetDamage")
			Obj* pTarObj = pScene->GetObjManager()->GetObj(targetId) ;
			if( pTarObj && IsCharacterObj(pTarObj->GetObjType()) )
			{
				Obj_Character* pTarChar = (Obj_Character*)pTarObj ;	
				pTarChar->HealthIncrement(-damage, INVALID_ID, pHuman);
			}
		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : gaoqi
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGmKillObj
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					�Լ���ObjID
	*				targetId
	*					Ŀ���ObjID
	*     ����˵�� :
	*				ֱ��ɱ��ָ����obj;
	*/
	INT LuaFnGmKillObj(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		targetId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGmKillObj")
			Obj* pTarObj = pScene->GetObjManager()->GetObj(targetId) ;
			if( pTarObj && IsCharacterObj(pTarObj->GetObjType()) )
			{
				Obj_Character* pTarChar = (Obj_Character*)pTarObj ;	
				pTarChar->GM_ForceDie(pHuman);
			}
		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : jack
	*     ��дʱ�� : 2006-1-23
	*     �������� : LuaFnFindMonsterByGUID
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				guid
	*					��ҪѰ�ҵĹ���GUID
	*     ����˵�� :
	*				Ѱ�ҳ����е�ĳ������,����GUID
	*/
	INT LuaFnFindMonsterByGUID(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		GUID_t guid;

		sceneId = Lua_ValueToNumber(L,1);
		guid = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnFindMonsterByGUID")
			ObjID_t objid = pScene->GetMonsterManager()->FindMonsterByGUID(guid) ;
			Lua_PushNumber(L,objid);
		return 1;
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanMaxVigor
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*     ����˵�� :
	*				�õ���һ������ֵ
	*/
	INT LuaFnGetHumanMaxVigor(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetHumanMaxVigor")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			Lua_PushNumber(L,pHuman->GetMaxVigor());
			return 1;
		}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanMaxEnergy
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*     ����˵�� :
	*				�õ���Ҿ������ֵ
	*/
	INT LuaFnGetHumanMaxEnergy(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetHumanMaxEnergy")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			Lua_PushNumber(L,pHuman->GetMaxEnergy());
			return 1;
		}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanVigor
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*     ����˵�� :
	*				�õ���һ���ֵ
	*/
	INT LuaFnGetHumanVigor(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetHumanVigor")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			Lua_PushNumber(L,pHuman->GetVigor());
			return 1;
		}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanEnergy
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*     ����˵�� :
	*				�õ���Ҿ���ֵ
	*/
	INT LuaFnGetHumanEnergy(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetHumanEnergy")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			Lua_PushNumber(L,pHuman->GetEnergy());
			return 1;
		}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanMaxVigor
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				������һ������ֵ
	*/
	INT LuaFnSetHumanMaxVigor(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;
		INT value;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);
		value	= Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnSetHumanMaxVigor")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			pHuman->SetMaxVigor(value);
			return 0;
		}
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0;
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanMaxEnergy
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				������Ҿ������ֵ
	*/
	INT LuaFnSetHumanMaxEnergy(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;
		INT value;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);
		value	= Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnSetHumanMaxEnergy")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			pHuman->SetMaxEnergy(value);
			return 0;
		}
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanVigor
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				������һ���
	*/
	INT LuaFnSetHumanVigor(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;
		INT value;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);
		value	= Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnSetHumanVigor")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			pHuman->SetVigor(value);
			return 0;
		}
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanEnergy
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				������Ҿ���
	*/
	INT LuaFnSetHumanEnergy(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;
		INT value;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);
		value	= Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnSetHumanEnergy")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			pHuman->SetEnergy(value);
			return 0;
		}
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanVigorRegeneRate
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*     ����˵�� :
	*				�õ���һ����ָ��ٶ�
	*/
	INT LuaFnGetHumanVigorRegeneRate(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetHumanVigorRegeneRate")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			Lua_PushNumber(L,pHuman->GetVigorRegeneRate());
			return 1;
		}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanEnergyRegeneRate
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*     ����˵�� :
	*				�õ���Ҿ����ָ��ٶ�
	*/
	INT LuaFnGetHumanEnergyRegeneRate(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetHumanEnergyRegeneRate")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			Lua_PushNumber(L,pHuman->GetEnergyRegeneRate());
			return 1;
		}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanVigorRegeneRate
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				������һ����ָ��ٶ�
	*/
	INT LuaFnSetHumanVigorRegeneRate(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;
		INT value;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);
		value	= Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnSetHumanVigorRegeneRate")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			pHuman->SetVigorRegeneRate(value);
			return 0;
		}
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanEnergyRegeneRate
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				������Ҿ����ָ��ٶ�
	*/
	INT LuaFnSetHumanEnergyRegeneRate(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;
		INT value;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);
		value	= Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnSetHumanEnergyRegeneRate")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			pHuman->SetEnergyRegeneRate(value);
			return 0;
		}
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanHairColor
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				�������ͷ����ɫ
	*/
	INT LuaFnSetHumanHairColor(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT value;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		value	= Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetHumanHairColor")
			pHuman->__SetHairColor(value);
			return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanHairColor
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				�������ͷ����ɫ
	*/
	INT LuaFnGetHumanHairColor(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetHumanHairColor")
			UINT uValue = pHuman->__GetHairColor();
			Lua_PushNumber(L,uValue);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanHairModel
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				uHairStyle
	*					��ֵ��ֵ
	*     ����˵�� :
	*				�������ͷ��ģ��
	*/
	INT LuaFnSetHumanHairModel(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT uHairStyle;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		uHairStyle = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnSetHumanHairModel")
			pHuman->__SetHairModel(uHairStyle);
			Lua_PushNumber(L,1);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-5-15	14:36
	 *	�������ƣ�	LuaFnChangeHumanHairModel
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				uHairStyle
	 *					����ֵ
	 *
	 *	����˵����	����ҵķ��ͻ��� uHairStyle
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnChangeHumanHairModel(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT uHairStyle;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		uHairStyle = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnChangeHumanHairModel")
			if ( uHairStyle >= g_HairStyleTbl.m_uCount )
			{
				Assert( uHairStyle < g_HairStyleTbl.m_uCount );
				Lua_PushNumber(L, FALSE);
				return 1;
			}

			_HAIR_STYLE_TYPE_* pHairStyleType;
			pHairStyleType = &(g_HairStyleTbl.m_HairStyle[uHairStyle]);
			if ( pHairStyleType->m_nIndex != uHairStyle )
			{
				AssertEx( pHairStyleType->m_nIndex == uHairStyle, "Invalid Hair Style, Please Check the Table!" );
				Lua_PushNumber(L, FALSE);
				return 1;
			}

			if ( pHairStyleType->m_nUseScope < 2 )
			{
				AssertEx( pHairStyleType->m_nUseScope >= 2, "This Hair Style Can't be Used in this Scenario!" );
				Lua_PushNumber(L, FALSE);
				return 1;
			}

			if ( pHairStyleType->m_nRaceID != pHuman->GetSex() )
			{
				AssertEx( pHairStyleType->m_nRaceID == pHuman->GetSex(), "This Hair Style is NOT Suit this Player's Gender!" );
				Lua_PushNumber(L, FALSE);
				return 1;
			}

			INT nCostCount;
			nCostCount = pHairStyleType->m_nItemCostCount;
			if ( nCostCount > 0 )
			{
				if ( nCostCount > HumanItemLogic::CalcBagItemCount(pHuman, pHairStyleType->m_uItemSerial) )
				{
					Lua_PushNumber(L, FALSE);
					return 1;
				}
				
				ITEM_LOG_PARAM ItemLogParam;
				ItemLogParam.OpType	= ITEM_SCRIPT_DELETE;
				HumanItemLogic::EraseBagItem(&ItemLogParam, pHuman, pHairStyleType->m_uItemSerial, nCostCount);
			}

			pHuman->__SetHairModel(uHairStyle);
			Lua_PushNumber(L, TRUE);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

	LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanHairModel
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				�������ͷ��ģ��
	*/
	INT LuaFnGetHumanHairModel(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetHumanHairModel")
			UINT uValue = pHuman->__GetHairModel();
			Lua_PushNumber(L,uValue);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-5-15	14:26
	 *	�������ƣ�	LuaFnGetHumanGoodBadValue
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�õ� selfId ���ƶ�ֵ
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnGetHumanGoodBadValue(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnGetHumanGoodBadValue")
			INT nValue = pHuman->GetGoodBadValue();
			Lua_PushNumber(L, nValue);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

	LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-5-15	14:26
	 *	�������ƣ�	LuaFnSetHumanGoodBadValue
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				value
	 *					�ƶ�ֵ
	 *
	 *	����˵����	���� selfId ���ƶ�ֵ
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnSetHumanGoodBadValue(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;
	  INT value;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		value = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnSetHumanGoodBadValue")
			pHuman->SetGoodBadValue(value);
			return 0;
		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

    
    /****************************************************************************
    *	������Ա��	amos
    *	��дʱ�䣺	2008-6-23	14:26
    *	�������ƣ�	LuaFnSetExchangStatus
    *	����˵����
    *				sceneId
    *					�������
    *				selfId
    *					������ ID���޷�������
    *				
    *					
    *
    *	����˵����	��ս���״̬
    *	�޸ļ�¼��
    *****************************************************************************/
    INT LuaFnSetExchangStatus(Lua_State* L)	
    {
        LUA_ENTER_FUNCTION
        
        SceneID_t sceneId;
        ObjID_t selfId;

        sceneId = Lua_ValueToNumber(L, 1);
        selfId = Lua_ValueToNumber(L, 2);

        BEGINHUMANDEFINE("LuaFnSetExchangStatus")
            pHuman->m_ExchangBox.CleanUp();
        return 0;
        ENDHUMANDEFINE

            LUA_LEAVE_FUNCTION

            return 0;
    }


}

#endif

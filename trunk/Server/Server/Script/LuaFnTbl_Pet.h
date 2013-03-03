//-----------------------------------------------------------------------------
// �ļ��� : LuaFnTbl_Pet.h
// ģ��	:	Script
// ����	 :  ������غ���
// �޸���ʷ:
//	wangpojin����
//-----------------------------------------------------------------------------

#ifndef __LUAFNTBL_PET_H__
#define __LUAFNTBL_PET_H__

#include "LuaFnMacro.h"
#include "Obj_Human.h"

#include "GCDetailAttrib_Pet.h"

extern INT CalcPetDomesticationMoney( INT nLevel, INT nHP, INT nHPMax, INT nHappiness );

namespace LuaFnTbl
{
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpj
	*     ��дʱ�� : 2006-021-20
	*     �������� : LuaFnPetStudySkill
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*				skillId
	*					����ID
	*     ����˵�� :
	*				����ѧϰ����
	*/
	INT LuaFnPetStudySkill(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;
		UINT petGUID_H;
		UINT petGUID_L;
		INT skillId;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);
		petGUID_H = Lua_ValueToNumber(L,3);
		petGUID_L = Lua_ValueToNumber(L,4);
		skillId = Lua_ValueToNumber(L,5);

		BEGINSCENEDEFINE("LuaFnPetStudySkill")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			PET_GUID_t guidPet;
			guidPet.m_uHighSection = petGUID_H;
			guidPet.m_uLowSection = petGUID_L;
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				INT nSkillIndex = -1;
				BOOL bResult = pHuman->Pet_Skill_Modify_Study(guidPet, skillId, &nSkillIndex);
				if ( bResult )
				{
					GCDetailAttrib_Pet msgDetailPet;
					msgDetailPet.SetGUID(guidPet);

					_PET_SKILL skillOwn;
					skillOwn.m_nSkillID = pPetItem->GetSkill(nSkillIndex).m_nSkillID;
					msgDetailPet.SetSkill(nSkillIndex, &skillOwn);

					pHuman->GetPlayer()->SendPacket(&msgDetailPet);

					return 0;
				}
			}
		}
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpj
	*     ��дʱ�� : 2006-021-20
	*     �������� : LuaFnPetStudySkill
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*				skillId
	*					����ID
	*     ����˵�� :
	*				����ѧϰ���ɼ���
	*/
	INT LuaFnPetStudySkill_MenPai(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;
		UINT petGUID_H;
		UINT petGUID_L;
		INT skillId;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);
		petGUID_H = Lua_ValueToNumber(L,3);
		petGUID_L = Lua_ValueToNumber(L,4);
		skillId = Lua_ValueToNumber(L,5);

		BEGINSCENEDEFINE("LuaFnPetStudySkill_MenPai")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			PET_GUID_t guidPet;
			guidPet.m_uHighSection = petGUID_H;
			guidPet.m_uLowSection = petGUID_L;
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				BOOL bResult = pHuman->Pet_Skill_Modify_MenPai(guidPet, skillId);
				if ( bResult )
				{
					GCDetailAttrib_Pet msgDetailPet;
					msgDetailPet.SetGUID(guidPet);

					_PET_SKILL skillOwn;
					skillOwn.m_nSkillID = pPetItem->GetSkill(PET_SKILL_INDEX_MENPAI).m_nSkillID;
					msgDetailPet.SetSkill(PET_SKILL_INDEX_MENPAI, &skillOwn);

					pHuman->GetPlayer()->SendPacket(&msgDetailPet);

					return 0;
				}
			}
		}
		ENDSCENEDEFINE
		

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpj
	*     ��дʱ�� : 2006-021-20
	*     �������� : LuaFnCalcPetDomesticationMoney
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					����ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*     ����˵�� :
	*				�õ�ѵ�������������
	*/
	INT LuaFnCalcPetDomesticationMoney(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;
		UINT petGUID_H;
		UINT petGUID_L;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);
		petGUID_H = Lua_ValueToNumber(L,3);
		petGUID_L = Lua_ValueToNumber(L,4);

		BEGINSCENEDEFINE("LuaFnCalcPetDomesticationMoney")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			PET_GUID_t guidPet;
			guidPet.m_uHighSection = petGUID_H;
			guidPet.m_uLowSection = petGUID_L;
			if(!guidPet.IsNull())
			{
				ItemContainer *pPetContainer = pHuman->GetPetContain();
				Item *pPetItem = pHuman->GetHorseItem(guidPet);
				if(pPetItem != NULL)
				{
					INT nMoney = CalcPetDomesticationMoney(pPetItem->GetLevel(), pHuman->GetPetHP(guidPet), pHuman->GetPetMaxHP(guidPet), pHuman->GetHorseHappiness(guidPet));
					Lua_PushNumber(L,nMoney);
					return 1;
				}
			}
		}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-021-20
	*     �������� : LuaFnPetDomestication
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					����ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*     ����˵�� :
	*				ѵ������
	*/
	INT LuaFnPetDomestication(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;
		UINT petGUID_H;
		UINT petGUID_L;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);
		petGUID_H = Lua_ValueToNumber(L,3);
		petGUID_L = Lua_ValueToNumber(L,4);

		BEGINSCENEDEFINE("LuaFnPetDomestication")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			PET_GUID_t guidPet;
			guidPet.m_uHighSection = petGUID_H;
			guidPet.m_uLowSection = petGUID_L;
			if(!guidPet.IsNull())
			{
				ItemContainer *pPetContainer = pHuman->GetPetContain();
				Item *pPetItem = pHuman->GetHorseItem(guidPet);
				if(pPetItem != NULL)
				{
					INT nMoney = CalcPetDomesticationMoney(pPetItem->GetLevel(), pHuman->GetPetHP(guidPet), pHuman->GetPetMaxHP(guidPet), pHuman->GetHorseHappiness(guidPet));
					if((UINT)nMoney < pHuman->GetMoney())
					{
						INT petIndex = pHuman->GetHorseIndexByGUID(guidPet);
						pHuman->SetMoney(pHuman->GetMoney() - (UINT)nMoney);
						
						MONEY_LOG_PARAM	MoneyLogParam;
						MoneyLogParam.CharGUID	=	pHuman->GetGUID();
						MoneyLogParam.OPType	=	MONEY_PET_COST;	
						MoneyLogParam.Count		=	nMoney;
						MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
						MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
						MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
						SaveMoneyLog(&MoneyLogParam);

						pHuman->SetPetHP(guidPet, pHuman->GetPetMaxHP(guidPet));
						pHuman->SetPetHappiness(guidPet, 100);

						GCDetailAttrib_Pet msgDetailPet;
						msgDetailPet.SetGUID(guidPet);
						msgDetailPet.SetHP(pHuman->GetPetHP(guidPet));
						msgDetailPet.SetHappiness(pHuman->GetHorseHappiness(guidPet));

						pHuman->GetPlayer()->SendPacket(&msgDetailPet);

						Lua_PushNumber(L,1);
						return 1;
					}
				}
			}
		}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-17
	*     �������� : LuaFnGetPetHP
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*     ����˵�� :
	*				���ó���HP
	*/
	INT LuaFnGetPetHP(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT petGUID_H;
		UINT petGUID_L;

		sceneId		= Lua_ValueToNumber(L,1);
		selfId		= Lua_ValueToNumber(L,2);
		petGUID_H	= (UINT)(Lua_ValueToNumber(L,3));
		petGUID_L	= (UINT)(Lua_ValueToNumber(L,4));

		BEGINHUMANDEFINE("LuaFnGetPetHP")
			PET_GUID_t guidPet;
			guidPet.m_uHighSection	= petGUID_H;
			guidPet.m_uLowSection	= petGUID_L;
			INT nHP = pHuman->GetPetHP(guidPet);

			Lua_PushNumber(L,nHP);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-17
	*     �������� : LuaFnGetPetMaxHP
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*     ����˵�� :
	*				���ó���HP���ֵ
	*/
	INT LuaFnGetPetMaxHP(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT petGUID_H;
		UINT petGUID_L;

		sceneId		= Lua_ValueToNumber(L,1);
		selfId		= Lua_ValueToNumber(L,2);
		petGUID_H	= (UINT)(Lua_ValueToNumber(L,3));
		petGUID_L	= (UINT)(Lua_ValueToNumber(L,4));

		BEGINHUMANDEFINE("LuaFnGetPetMaxHP")
			PET_GUID_t guidPet;
			guidPet.m_uHighSection	= petGUID_H;
			guidPet.m_uLowSection	= petGUID_L;
			INT nHP = pHuman->GetPetMaxHP(guidPet);

			Lua_PushNumber(L,nHP);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-17
	*     �������� : LuaFnGetPetLife
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*     ����˵�� :
	*				���ó�������
	*/
	INT LuaFnGetPetLife(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT petGUID_H;
		UINT petGUID_L;

		sceneId		= Lua_ValueToNumber(L,1);
		selfId		= Lua_ValueToNumber(L,2);
		petGUID_H	= (UINT)(Lua_ValueToNumber(L,3));
		petGUID_L	= (UINT)(Lua_ValueToNumber(L,4));

		BEGINHUMANDEFINE("LuaFnGetPetLife")
			PET_GUID_t guidPet;
			guidPet.m_uHighSection	= petGUID_H;
			guidPet.m_uLowSection	= petGUID_L;
			INT nLife = pHuman->GetPetLife(guidPet);

			Lua_PushNumber(L,nLife);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-17
	*     �������� : LuaFnGetPetHappiness
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*     ����˵�� :
	*				���ó�����ֶ�
	*/
	INT LuaFnGetPetHappiness(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT petGUID_H;
		UINT petGUID_L;

		sceneId		= Lua_ValueToNumber(L,1);
		selfId		= Lua_ValueToNumber(L,2);
		petGUID_H	= (UINT)(Lua_ValueToNumber(L,3));
		petGUID_L	= (UINT)(Lua_ValueToNumber(L,4));

		BEGINHUMANDEFINE("LuaFnGetPetHappiness")
			PET_GUID_t guidPet;
			guidPet.m_uHighSection	= petGUID_H;
			guidPet.m_uLowSection	= petGUID_L;
			INT nHappiness = pHuman->GetHorseHappiness(guidPet);

			Lua_PushNumber(L,nHappiness);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-17
	*     �������� : LuaFnSetPetHP
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*				hp
	*					Ҫ���õ�HPֵ	
	*     ����˵�� :
	*				���ó���HP
	*/
	INT LuaFnSetPetHP(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT petGUID_H;
		UINT petGUID_L;
		INT hp;

		sceneId		= Lua_ValueToNumber(L,1);
		selfId		= Lua_ValueToNumber(L,2);
		petGUID_H	= (UINT)(Lua_ValueToNumber(L,3));
		petGUID_L	= (UINT)(Lua_ValueToNumber(L,4));
		hp			= Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnSetPetHP")
			PET_GUID_t guidPet;
			guidPet.m_uHighSection	= petGUID_H;
			guidPet.m_uLowSection	= petGUID_L;
			pHuman->SetPetHP(guidPet,hp);

			GCDetailAttrib_Pet msgDetailPet;
			msgDetailPet.SetGUID(guidPet);
			msgDetailPet.SetHP(pHuman->GetPetHP(guidPet));
			pHuman->GetPlayer()->SendPacket(&msgDetailPet);

			return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-17
	*     �������� : LuaFnSetPetMaxHP
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*				maxHP
	*					Ҫ���õ�HP���ֵ	
	*     ����˵�� :
	*				���ó���HP���ֵ
	*/
	INT LuaFnSetPetMaxHP(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT petGUID_H;
		UINT petGUID_L;
		INT maxHP;

		sceneId		= Lua_ValueToNumber(L,1);
		selfId		= Lua_ValueToNumber(L,2);
		petGUID_H	= (UINT)(Lua_ValueToNumber(L,3));
		petGUID_L	= (UINT)(Lua_ValueToNumber(L,4));
		maxHP		= Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnSetPetMaxHP")

			return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-17
	*     �������� : LuaFnSetPetLife
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*				life
	*					Ҫ���õ�����ֵ	
	*     ����˵�� :
	*				���ó�������ֵ	
	*/
	INT LuaFnSetPetLife(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT petGUID_H;
		UINT petGUID_L;
		INT life;

		sceneId		= Lua_ValueToNumber(L,1);
		selfId		= Lua_ValueToNumber(L,2);
		petGUID_H	= (UINT)(Lua_ValueToNumber(L,3));
		petGUID_L	= (UINT)(Lua_ValueToNumber(L,4));
		life		= Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnSetPetLife")
			PET_GUID_t guidPet;
			guidPet.m_uHighSection	= petGUID_H;
			guidPet.m_uLowSection	= petGUID_L;
			pHuman->SetPetLife(guidPet, life);

			GCDetailAttrib_Pet msgDetailPet;
			msgDetailPet.SetGUID(guidPet);
			msgDetailPet.SetLife(pHuman->GetPetLife(guidPet));
			pHuman->GetPlayer()->SendPacket(&msgDetailPet);

			return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-17
	*     �������� : LuaFnSetPetHappiness
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*				happiness
	*					Ҫ���õĿ��ֶ�
	*     ����˵�� :
	*				���ó�����ֶ�
	*/
	INT LuaFnSetPetHappiness(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT petGUID_H;
		UINT petGUID_L;
		INT happiness;

		sceneId		= Lua_ValueToNumber(L,1);
		selfId		= Lua_ValueToNumber(L,2);
		petGUID_H	= (UINT)(Lua_ValueToNumber(L,3));
		petGUID_L	= (UINT)(Lua_ValueToNumber(L,4));
		happiness	= Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnSetPetHappiness")
			PET_GUID_t guidPet;
			guidPet.m_uHighSection	= petGUID_H;
			guidPet.m_uLowSection	= petGUID_L;
			pHuman->SetPetHappiness(guidPet, happiness);

			GCDetailAttrib_Pet msgDetailPet;
			msgDetailPet.SetGUID(guidPet);
			msgDetailPet.SetHappiness(pHuman->GetHorseHappiness(guidPet));
			pHuman->GetPlayer()->SendPacket(&msgDetailPet);

			return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}

	/****************************************************************************
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-4-11	16:31
	 *	�������ƣ�	LuaFnPetCanUseFood
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				petGUID_H
	 *					����GUID��λ
	 *				petGUID_L
	 *					����GUID��λ
	 *				nIndex
	 *					��Ʒ����λ��
	 *
	 *	����˵����	�ж���Ʒ�Ƿ��ʺϳ���ʳ��
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnPetCanUseFood(Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;
	  UINT petGUID_H;
	  UINT petGUID_L;
	  INT nIndex;

		sceneId			= Lua_ValueToNumber(L, 1);
		selfId			= Lua_ValueToNumber(L, 2);
		petGUID_H		= (UINT)(Lua_ValueToNumber(L, 3));
		petGUID_L		= (UINT)(Lua_ValueToNumber(L, 4));
		nIndex			= Lua_ValueToNumber(L, 5);

		BEGINHUMANDEFINE("LuaFnPetCanUseFood")
			PET_GUID_t guidPet;
			guidPet.m_uHighSection	= petGUID_H;
			guidPet.m_uLowSection	= petGUID_L;
			INT nPetLevel;
			INT nFoodLevel;

			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if( pPetItem == NULL )
			{
				Lua_PushNumber(L, FALSE);
				return 1;
			}
			else
			{
				nPetLevel = pPetItem->GetLevel();
			}

			Item *pFood = HumanItemLogic::GetBagItem(pHuman, nIndex);
			if( pFood == NULL )
			{
				Lua_PushNumber(L, FALSE);
				return 1;
			}
			else
			{
				nFoodLevel = pFood->GetItemLevel();
			}

			if( nFoodLevel > nPetLevel )
			{ // ���Ｖ�𲻹�
				Lua_PushNumber(L, FALSE);
				return 1;
			}

			PET_ATTR *pPetAttr = g_PetAttrTbl.GetAttr( pPetItem->GetDataID() );
			if( pPetAttr == NULL )
			{
				Assert(FALSE);
				Lua_PushNumber(L, FALSE);
				return 1;
			}

			INT nRet;
			switch( pFood->GetItemType() )
			{ // �ж�ʳ�������Ƿ�ƥ�䣬���ʳ�ĳ���ʳ�������� 1111
			case 2: // PET_FOOD_TYPE_MEAT
				nRet = (pPetAttr->m_FoodType / 1000) & 1;
				break;
			case 3: // PET_FOOD_TYPE_GRASS
				nRet = (pPetAttr->m_FoodType / 100) & 1;
				break;
			case 4: // PET_FOOD_TYPE_WORM
				nRet = (pPetAttr->m_FoodType / 10) & 1;
				break;
			case 5: // PET_FOOD_TYPE_PADDY
				nRet = pPetAttr->m_FoodType & 1;
				break;
			default:
				AssertEx(FALSE, "Invalid Pet Food.");
				Lua_PushNumber(L, FALSE);
				return 1;
			}

			Lua_PushNumber(L, nRet);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : wuwenbin
	*     ��дʱ�� : 2006-04-19
	*     �������� : LuaFnPetReturnToChild
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*     ����˵�� :
	*				���ﻹͯLua�ӿ�
	*/
	INT LuaFnPetReturnToChild(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT petGUID_H;
		UINT petGUID_L;

		sceneId		= Lua_ValueToNumber(L,1);
		selfId		= Lua_ValueToNumber(L,2);
		petGUID_H	= (UINT)(Lua_ValueToNumber(L,3));
		petGUID_L	= (UINT)(Lua_ValueToNumber(L,4));
		
		BEGINHUMANDEFINE("LuaFnPetReturnToChild")
			PET_GUID_t guidPet;
			guidPet.m_uHighSection	= petGUID_H;
			guidPet.m_uLowSection	= petGUID_L;

			Lua_PushNumber(L, pHuman->HorseReturnToChild(guidPet));
			return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : wuwenbin
	*     ��дʱ�� : 2006-04-19
	*     �������� : LuaFnPetReturnToChild
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					���ID
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*     ����˵�� :
	*				���ﻹͯLua�ӿ�
	*/
	INT LuaFnPetCanReturnToChild(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT petGUID_H;
		UINT petGUID_L;

		sceneId		= Lua_ValueToNumber(L,1);
		selfId		= Lua_ValueToNumber(L,2);
		petGUID_H	= (UINT)(Lua_ValueToNumber(L,3));
		petGUID_L	= (UINT)(Lua_ValueToNumber(L,4));
		
		BEGINHUMANDEFINE("LuaFnPetCanReturnToChild")
			PET_GUID_t guidPet;
			guidPet.m_uHighSection	= petGUID_H;
			guidPet.m_uLowSection	= petGUID_L;

			Lua_PushNumber(L, pHuman->CanHorseReturnToChild(guidPet));
			return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-05-12
	*     �������� : LuaFnGetPetGUID
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					���ID
	*				petIndex
	*					��������
	*     ����˵�� :
	*				ȡ�ó����GUID
	*/
	INT LuaFnGetPetGUID(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT petIndex;

		sceneId		= Lua_ValueToNumber(L,1);
		selfId		= Lua_ValueToNumber(L,2);
		petIndex	= Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetPetGUID")

			PET_GUID_t guidPet = pHuman->GetPetGUIDByIndex(petIndex);

			Lua_PushNumber(L,guidPet.m_uHighSection);
			Lua_PushNumber(L,guidPet.m_uLowSection);
			return 2;

		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}
	;
}

#endif // __LUAFNTBL_PET_H__

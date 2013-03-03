//-----------------------------------------------------------------------------
// �ļ��� : LuaFnTbl_Battle.h
// ģ��	:	Script
// ����	 :  ս����غ���
// �޸���ʷ:
//	yangfei����
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_BATTLE_H__
#define __LUAFNTBL_BATTLE_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"

namespace LuaFnTbl
{
	/*
	 *	��ö�ӦObjID ����ҵĳ�������
	 *     ������Ա : jack
	 *     ��дʱ�� : 2006-2-27
	 *     �������� : LuaFnGetPetCount
	 *     ����˵�� :  
	 *			SceneID_t	sceneID   �������
	 *          ObjID_t		selfID	  ��ɫ���
     *
	 *	   ����  INT ��ɫ�ĳ�������
	 *     ����˵�� :
	 *
	 */
	INT	LuaFnGetPetCount(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		INT	nCount;


		BEGINHUMANDEFINE("LuaFnGetPetCount")
			nCount = pHuman->GetHorseCount() ;
		Lua_PushNumber(L,nCount);
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	��ö�ӦObjID ����ҵĳ����DataID(�� ������еĺ�)
	 *     ������Ա : jack
	 *     ��дʱ�� : 2006-2-27
	 *     �������� : LuaFnGetPet_DataID
	 *     ����˵�� :  
	 *			SceneID_t	sceneID   �������
	 *          ObjID_t		selfID	  ��ɫ���
	 *			INT			nIndex	  ����λ��
     *
	 *	   ����  INT ����ȼ�
	 *     ����˵�� :
	 *
	 */
	INT	LuaFnGetPet_DataID(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nDataID;


		BEGINHUMANDEFINE("LuaFnGetPet_DataID")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nDataID = pPetItem->GetDataID();
				Lua_PushNumber(L,nDataID);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	��ö�ӦObjID ����ҵĳ���ĵȼ�
	 *     ������Ա : jack
	 *     ��дʱ�� : 2006-2-27
	 *     �������� : LuaFnGetPet_Level
	 *     ����˵�� :  
	 *			SceneID_t	sceneID   �������
	 *          ObjID_t		selfID	  ��ɫ���
	 *			INT			nIndex	  ����λ��
     *
	 *	   ����  INT ����ȼ�
	 *     ����˵�� :
	 *
	 */
	INT	LuaFnGetPet_Level(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nLevel;


		BEGINHUMANDEFINE("LuaFnGetPet_Level")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nLevel = pPetItem->GetLevel();
				Lua_PushNumber(L,nLevel);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	��ö�ӦObjID ����ҵĳ����Я���ȼ�
	 *     ������Ա : jack
	 *     ��дʱ�� : 2006-2-27
	 *     �������� : LuaFnGetPet_TakeLevel
	 *     ����˵�� :  
	 *			SceneID_t	sceneID   �������
	 *          ObjID_t		selfID	  ��ɫ���
	 *			INT			nIndex	  ����λ��
     *
	 *	   ����  INT �����Я���ȼ�
	 *     ����˵�� :
	 *
	 */
	INT	LuaFnGetPet_TakeLevel(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nTakeLevel;


		BEGINHUMANDEFINE("LuaFnGetPet_TakeLevel")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nTakeLevel = pPetItem->GetTakeLevel();
				Lua_PushNumber(L,nTakeLevel);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	��ö�ӦObjID ����ҵĳ����HP
	 *     ������Ա : jack
	 *     ��дʱ�� : 2006-2-27
	 *     �������� : LuaFnGetPet_HP
	 *     ����˵�� :  
	 *			SceneID_t	sceneID   �������
	 *          ObjID_t		selfID	  ��ɫ���
	 *			INT			nIndex	  ����λ��
     *
	 *	   ����  INT ����HP
	 *     ����˵�� :
	 *
	 */
	INT	LuaFnGetPet_HP(Lua_State* L)
	{
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		Assert( FALSE && "��û��HP, �˽ӿ��ѷ���" );
		//LUA_ENTER_FUNCTION
		//sceneId			=	Lua_ValueToNumber(L,1);
		//selfId			=	Lua_ValueToNumber(L,2);	
		//nIndex			=	Lua_ValueToNumber(L,3);	
		//INT	nHP;


		//BEGINHUMANDEFINE("LuaFnGetPet_HP")
		//	PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
		//	Item *pPetItem = pHuman->GetPetItem(guidPet);
		//	if(pPetItem != NULL)
		//	{
		//		nHP = pPetItem->GetHP();
		//		Lua_PushNumber(L,nHP);
		//		return 1;
		//	}
		//ENDHUMANDEFINE
		//RETURNFALSE
		//LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	��ö�ӦObjID ����ҵĳ����Max HP
	 *     ������Ա : jack
	 *     ��дʱ�� : 2006-2-27
	 *     �������� : LuaFnGetPet_MaxHP
	 *     ����˵�� :  
	 *			SceneID_t	sceneID   �������
	 *          ObjID_t		selfID	  ��ɫ���
	 *			INT			nIndex	  ����λ��
     *
	 *	   ����  INT ����Max HP
	 *     ����˵�� :
	 *
	 */
	INT	LuaFnGetPet_MaxHP(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nMaxHP;


		BEGINHUMANDEFINE("LuaFnGetPet_MaxHP")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			nMaxHP = pHuman->GetPetMaxHP(guidPet);
			Lua_PushNumber(L,nMaxHP);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	��ö�ӦObjID ����ҵĳ��������
	 *     ������Ա : jack
	 *     ��дʱ�� : 2006-2-27
	 *     �������� : LuaFnGetPet_Life
	 *     ����˵�� :  
	 *			SceneID_t	sceneID   �������
	 *          ObjID_t		selfID	  ��ɫ���
	 *			INT			nIndex	  ����λ��
     *
	 *	   ����  INT ��������
	 *     ����˵�� :
	 *
	 */
	INT	LuaFnGetPet_Life(Lua_State* L)
	{
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		Assert( FALSE && "��û���������ڣ��˽ӿڷ���" );
		//LUA_ENTER_FUNCTION
		//
		//sceneId			=	Lua_ValueToNumber(L,1);
		//selfId			=	Lua_ValueToNumber(L,2);	
		//nIndex			=	Lua_ValueToNumber(L,3);	
		//INT	nLife;


		//BEGINHUMANDEFINE("LuaFnGetPet_Life")
		//	PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
		//	Item *pPetItem = pHuman->GetPetItem(guidPet);
		//	if(pPetItem != NULL)
		//	{
		//		nLife = pPetItem->GetLife();
		//		Lua_PushNumber(L,nLife);
		//		return 1;
		//	}
		//ENDHUMANDEFINE
		//RETURNFALSE
		//LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	��ö�ӦObjID ����ҵĳ�������ͣ����� 0������ 1��Ұ�� 2��
	 *     ������Ա : jack
	 *     ��дʱ�� : 2006-2-27
	 *     �������� : LuaFnGetPet_Type
	 *     ����˵�� :  
	 *			SceneID_t	sceneID   �������
	 *          ObjID_t		selfID	  ��ɫ���
	 *			INT			nIndex	  ����λ��
     *
	 *	   ����  INT �������� ������ 0������ 1��Ұ�� 2��
	 *     ����˵�� :
	 *
	 */
	INT	LuaFnGetPet_Type(Lua_State* L)
	{
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		Assert( FALSE && "��û�����ͣ��ӿڷ���" );
		//LUA_ENTER_FUNCTION
		//
		//sceneId			=	Lua_ValueToNumber(L,1);
		//selfId			=	Lua_ValueToNumber(L,2);	
		//nIndex			=	Lua_ValueToNumber(L,3);	
		//INT	nType;


		//BEGINHUMANDEFINE("LuaFnGetPet_Type")
		//	PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
		//	Item *pPetItem = pHuman->GetPetItem(guidPet);
		//	if(pPetItem != NULL)
		//	{
		//		nType = pPetItem->GetPetType();
		//		Lua_PushNumber(L,nType);
		//		return 1;
		//	}
		//ENDHUMANDEFINE
		//RETURNFALSE
		//LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	��ö�ӦObjID ����ҵĳ���Ĵ���
	 *     ������Ա : jack
	 *     ��дʱ�� : 2006-2-27
	 *     �������� : LuaFnGetPet_Generation
	 *     ����˵�� :  
	 *			SceneID_t	sceneID   �������
	 *          ObjID_t		selfID	  ��ɫ���
	 *			INT			nIndex	  ����λ��
     *
	 *	   ����  INT �������
	 *     ����˵�� :
	 *
	 */
	INT	LuaFnGetPet_Generation(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nGeneration;


		BEGINHUMANDEFINE("LuaFnGetPet_Generation")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nGeneration = pPetItem->GetGeneration();
				Lua_PushNumber(L,nGeneration);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	��ö�ӦObjID ����ҵĳ���Ŀ��ֶ�
	 *     ������Ա : jack
	 *     ��дʱ�� : 2006-2-27
	 *     �������� : LuaFnGetPet_Happyness
	 *     ����˵�� :  
	 *			SceneID_t	sceneID   �������
	 *          ObjID_t		selfID	  ��ɫ���
	 *			INT			nIndex	  ����λ��
     *
	 *	   ����  INT ���ﻶ�ֶ�
	 *     ����˵�� :
	 *
	 */
	INT	LuaFnGetPet_Happyness(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nHappyness;


		BEGINHUMANDEFINE("LuaFnGetPet_Happyness")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nHappyness = pPetItem->GetHappiness();
				Lua_PushNumber(L,nHappyness);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	��ö�ӦObjID ����ҵĳ������������
	 *     ������Ա : jack
	 *     ��дʱ�� : 2006-2-27
	 *     �������� : LuaFnGetPet_StrPerception
	 *     ����˵�� :  
	 *			SceneID_t	sceneID   �������
	 *          ObjID_t		selfID	  ��ɫ���
	 *			INT			nIndex	  ����λ��
     *
	 *	   ����  INT ������������
	 *     ����˵�� :
	 *
	 */
	INT	LuaFnGetPet_StrPerception(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nStrPerception;

		BEGINHUMANDEFINE("LuaFnGetPet_StrPerception")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nStrPerception = pPetItem->GetStrengthPer();
				Lua_PushNumber(L,nStrPerception);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	��ö�ӦObjID ����ҵĳ������������
	 *     ������Ա : jack
	 *     ��дʱ�� : 2006-2-27
	 *     �������� : LuaFnGetPet_ConPerception
	 *     ����˵�� :  
	 *			SceneID_t	sceneID   �������
	 *          ObjID_t		selfID	  ��ɫ���
	 *			INT			nIndex	  ����λ��
     *
	 *	   ����  INT ������������
	 *     ����˵�� :
	 *
	 */
	INT	LuaFnGetPet_SmartnessPerception(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nConPerception;


		BEGINHUMANDEFINE("LuaFnGetPet_ConPerception")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nConPerception = pPetItem->GetSmartnessPer();
				Lua_PushNumber(L,nConPerception);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	��ö�ӦObjID ����ҵĳ������������
	 *     ������Ա : jack
	 *     ��дʱ�� : 2006-2-27
	 *     �������� : LuaFnGetPet_DexPerception
	 *     ����˵�� :  
	 *			SceneID_t	sceneID   �������
	 *          ObjID_t		selfID	  ��ɫ���
	 *			INT			nIndex	  ����λ��
     *
	 *	   ����  INT ������������
	 *     ����˵�� :
	 *
	 */
	INT	LuaFnGetPet_MindPerception(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nDexPerception;


		BEGINHUMANDEFINE("LuaFnGetPet_DexPerception")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nDexPerception = pPetItem->GetMindPer();
				Lua_PushNumber(L,nDexPerception);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	��ö�ӦObjID ����ҵĳ������������
	 *     ������Ա : jack
	 *     ��дʱ�� : 2006-2-27
	 *     �������� : LuaFnGetPet_IntPerception
	 *     ����˵�� :  
	 *			SceneID_t	sceneID   �������
	 *          ObjID_t		selfID	  ��ɫ���
	 *			INT			nIndex	  ����λ��
     *
	 *	   ����  INT ������������
	 *     ����˵�� :
	 *
	 */
	INT	LuaFnGetPet_ConstitutionPerception(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nIntPerception;


		BEGINHUMANDEFINE("LuaFnGetPet_IntPerception")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nIntPerception = pPetItem->GetConstitutionPer();
				Lua_PushNumber(L,nIntPerception);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	��ö�ӦObjID ����ҵĳ���ĸ���
	 *     ������Ա : jack
	 *     ��дʱ�� : 2006-2-27
	 *     �������� : LuaFnGetPet_GenGu
	 *     ����˵�� :  
	 *			SceneID_t	sceneID   �������
	 *          ObjID_t		selfID	  ��ɫ���
	 *			INT			nIndex	  ����λ��
     *
	 *	   ����  INT �������
	 *     ����˵�� :
	 *
	 */
	INT	LuaFnGetPet_GenGu(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nGenGu;


		BEGINHUMANDEFINE("LuaFnGetPet_GenGu")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nGenGu = pPetItem->GetGenGu();
				Lua_PushNumber(L,nGenGu);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	��ö�ӦObjID ����ҵĳ����һ������ʣ�����
	 *     ������Ա : jack
	 *     ��дʱ�� : 2006-2-27
	 *     �������� : LuaFnGetPet_RemainPoint
	 *     ����˵�� :  
	 *			SceneID_t	sceneID   �������
	 *          ObjID_t		selfID	  ��ɫ���
	 *			INT			nIndex	  ����λ��
     *
	 *	   ����  INT ����һ������ʣ�����
	 *     ����˵�� :
	 *
	 */
	INT	LuaFnGetPet_RemainPoint(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nRemainPoint;


		BEGINHUMANDEFINE("LuaFnGetPet_RemainPoint")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nRemainPoint = pPetItem->GetRemainPoint();
				Lua_PushNumber(L,nRemainPoint);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	��ö�ӦObjID ����ҵĳ���ľ���ֵ
	 *     ������Ա : jack
	 *     ��дʱ�� : 2006-2-27
	 *     �������� : LuaFnGetPet_Exp
	 *     ����˵�� :  
	 *			SceneID_t	sceneID   �������
	 *          ObjID_t		selfID	  ��ɫ���
	 *			INT			nIndex	  ����λ��
     *
	 *	   ����  INT ���ﾭ��ֵ
	 *     ����˵�� :
	 *
	 */
	INT	LuaFnGetPet_Exp(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nExp;


		BEGINHUMANDEFINE("LuaFnGetPet_Exp")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nExp = pPetItem->GetExp();
				Lua_PushNumber(L,nExp);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	��ö�ӦObjID ����ҵĳ���Ļ���һ��ս������
	 *     ������Ա : jack
	 *     ��дʱ�� : 2006-2-27
	 *     �������� : LuaFnGetPet_AttrLevel1
	 *     ����˵�� :  
	 *			SceneID_t	sceneID   �������
	 *          ObjID_t		selfID	  ��ɫ���
	 *			INT			nIndex	  ����λ��
	 *			INT			nAttr	  ����(0 ����, 1 ����, 2 ����, 3 ����, 4 ��)
     *
	 *	   ����  INT �������һ��ս������
	 *     ����˵�� :
	 *
	 */
	INT	LuaFnGetPet_AttrLevel1(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		INT nAttr;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		nAttr			=	Lua_ValueToNumber(L,4);	
		INT	nValue;


		BEGINHUMANDEFINE("LuaFnGetPet_AttrLevel1")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nValue = pPetItem->GetLvl1Attr((CHAR_ATTR_LEVEL1)nAttr);
				Lua_PushNumber(L,nValue);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	��ö�ӦObjID ����ҵĳ��＼��
	 *     ������Ա : jack
	 *     ��дʱ�� : 2006-2-27
	 *     �������� : LuaFnGetPet_Skill
	 *     ����˵�� :  
	 *			SceneID_t	sceneID   �������
	 *          ObjID_t		selfID	  ��ɫ���
	 *			INT			nIndex	  ����λ��
	 *			INT			nSkillIndex ���ܲ�����(0-5),���е�0�����������ܲ�
     *
	 *	   ����  INT ���＼��
	 *     ����˵�� :
	 *
	 */
	INT	LuaFnGetPet_Skill(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		INT nSkillIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		nSkillIndex		=	Lua_ValueToNumber(L,4);	
		INT	nSkill;


		BEGINHUMANDEFINE("LuaFnGetPet_Skill")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nSkill = pPetItem->GetSkill(nSkillIndex).m_nSkillID;
				Lua_PushNumber(L,nSkill);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *     ������Ա : jack
	 *     ��дʱ�� : 2006-3-27
	 *     �������� : LuaFnCreatePet
	 *     ����˵�� :  
	 *			SceneID_t	sceneID   �������
	 *          ObjID_t		selfID	  ��ɫ���
	 *			INT			nPetDataID ������
     *
	 *	   ����  1 �ɹ���0 ʧ��
	 *     ����˵�� :
	 *			�ڸ��������һ������
	 */
	INT	LuaFnCreatePet(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nPetDataID;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nPetDataID		=	Lua_ValueToNumber(L,3);	

		BEGINHUMANDEFINE("LuaFnCreatePet")
			PET_LOG_PARAM	PetLogParam;
			ORESULT eRet = pHuman->AddPetByDataID(&PetLogParam,nPetDataID ) ;
			if( eRet==OR_OK )
			{
				PetLogParam.CharGUID	=	pHuman->GetGUID();
				PetLogParam.OPType		=	PET_OP_SCRIPT_CREATE;
				PetLogParam.SceneID		=	pHuman->getScene()->SceneID();
				PetLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				PetLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				SavePetLog(&PetLogParam);

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

		return 0;
	}

	/*
	 *	ɾ����ӦObjID ����ҵĳ���
	 *     ������Ա : jack
	 *     ��дʱ�� : 2006-3-27
	 *     �������� : LuaFnDeletePet
	 *     ����˵�� :  
	 *			SceneID_t	sceneID   �������
	 *          ObjID_t		selfID	  ��ɫ���
	 *			INT			nIndex	  ����λ��
     *
	 *	   ����  1 �ɹ��� 0 ʧ��
	 *     ����˵�� :
	 *
	 */
	INT	LuaFnDeletePet(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	


		BEGINHUMANDEFINE("LuaFnDeletePet")
			PET_LOG_PARAM	PetLogParam;
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			ORESULT eRet = pHuman->FreePetToNature(&PetLogParam,guidPet) ;
			if( eRet==OR_OK )
			{
			
				PetLogParam.CharGUID	=	pHuman->GetGUID();
				
				PetLogParam.OPType		=	PET_OP_SCRIPT_DELETE;
				PetLogParam.SceneID		=	pHuman->getScene()->SceneID();
				PetLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				PetLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				SavePetLog(&PetLogParam);
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
    *     ������Ա : amos
    *     ��дʱ�� : 2008-05-22
    *     �������� : LuaFnGetPKLevel
    *     ����˵�� :
    *				L
    *					LuaStateִ�л���ָ��
    *				sceneId
    *					������
    *				selfId
    *					���ID
    *				deltaLevel
    *					�ȼ���
    *     ����˵�� :
    *				���ݵȼ����õȼ�����
    */
    INT LuaFnGetPKLevel(Lua_State* L)	
    {
        LUA_ENTER_FUNCTION
        
        SceneID_t sceneId;
        ObjID_t selfId;
        INT deltaLevel;

        sceneId		= Lua_ValueToNumber(L,1);
        selfId		= Lua_ValueToNumber(L,2);
        deltaLevel	= Lua_ValueToNumber(L,3);

        BEGINHUMANDEFINE("LuaFnGetPKLevel")

        for (INT i = 0; i < MAX_PK_NUM; ++i)
        {
            if ( deltaLevel >= g_PKLevelTbl.m_aPKPointInfo[i].m_nMin &&
                 deltaLevel < g_PKLevelTbl.m_aPKPointInfo[i].m_nMax   )
            {
                INT n = g_PKLevelTbl.m_aPKPointInfo[i].m_nValue;

                Lua_PushNumber(L,n);
                break;
            }
        }
        return 1;

        ENDHUMANDEFINE

        LUA_LEAVE_FUNCTION

        return 0 ;
    }

    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-05-22
    *     �������� : LuaFnGetPKPoint
    *     ����˵�� :
    *				L
    *					LuaStateִ�л���ָ��
    *				sceneId
    *					������
    *				selfId
    *					���ID
    *				PKValue
    *					PKֵ
    *     ����˵�� :
    *				����PKֵ����PK����
    */
    INT LuaFnGetPKPoint(Lua_State* L)	
    {
        LUA_ENTER_FUNCTION
        
        SceneID_t sceneId;
        ObjID_t selfId;
        INT PKValue;

        sceneId		= Lua_ValueToNumber(L,1);
        selfId		= Lua_ValueToNumber(L,2);
        PKValue	    = Lua_ValueToNumber(L,3);

        BEGINHUMANDEFINE("LuaFnGetPKPoint")

            for (INT i = 0; i < MAX_PK_NUM; ++i)
            {
                if ( PKValue >= g_PKPointTbl.m_aPKPointInfo[i].m_nMin &&
                    PKValue < g_PKPointTbl.m_aPKPointInfo[i].m_nMax   )
                {
                    INT n = g_PKPointTbl.m_aPKPointInfo[i].m_nValue;

                    Lua_PushNumber(L,n);
                    break;
                }
            }
            return 1;

            ENDHUMANDEFINE

            LUA_LEAVE_FUNCTION

            return 0 ;
    }


    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-05-22
    *     �������� : LuaFnGetPKPunish
    *     ����˵�� :
    *				L
    *					LuaStateִ�л���ָ��
    *				sceneId
    *					������
    *				selfId
    *					���ID
    *				PunishValue
    *					�ͷ�ֵ
    *     ����˵�� :
    *				���ݳͷ�ֵ���óͷ�ֵ����
    */
    INT LuaFnGetPKPunish(Lua_State* L)	
    {
        LUA_ENTER_FUNCTION
        
        SceneID_t sceneId;
        ObjID_t selfId;
        INT PunishValue;

        sceneId		= Lua_ValueToNumber(L,1);
        selfId		= Lua_ValueToNumber(L,2);
        PunishValue	    = Lua_ValueToNumber(L,3);

        BEGINHUMANDEFINE("LuaFnGetPKPunish")

            for (INT i = 0; i < MAX_PK_NUM; ++i)
            {
                if ( PunishValue >= g_PKPunishTbl.m_aPKPointInfo[i].m_nMin &&
                    PunishValue < g_PKPunishTbl.m_aPKPointInfo[i].m_nMax   )
                {
                    INT n = g_PKPunishTbl.m_aPKPointInfo[i].m_nValue;

                    Lua_PushNumber(L,n);
                    break;
                }
            }
            return 1;

            ENDHUMANDEFINE

            LUA_LEAVE_FUNCTION

            return 0 ;
    }


    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-21
    *     �������� : LuaAddHonor
    *     ����˵�� : 
    *     ����˵�� :
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
    *				nHonor 
    *					��������
    *				
    */
    INT	LuaAddHonor(Lua_State* L) 
    {
        LUA_ENTER_FUNCTION
        
        SceneID_t sceneId;
        ObjID_t selfId;
        INT nHonor ;

        sceneId			=	Lua_ValueToNumber(L,1);
        selfId			=	Lua_ValueToNumber(L,2);
        nHonor     		=	Lua_ValueToNumber(L,3);

        BEGINHUMANDEFINE("LuaAddHonor")

        pHuman->SetHonor(pHuman->GetHonor() + nHonor);


        ENDHUMANDEFINE
        return	0;

        LUA_LEAVE_FUNCTION
        return  0;
    }

    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-21
    *     �������� : LuaSubHonor
    *     ����˵�� : 
    *     ����˵�� :
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
    *				nHonor 
    *					����
    *				
    */
    INT	LuaSubHonor(Lua_State* L) 
    {
        LUA_ENTER_FUNCTION
        
        SceneID_t sceneId;
        ObjID_t selfId;
        INT nHonor ;

        sceneId			=	Lua_ValueToNumber(L,1);
        selfId			=	Lua_ValueToNumber(L,2);
        nHonor     		=	Lua_ValueToNumber(L,3);

        BEGINHUMANDEFINE("LuaSubHonor")
            
            INT nTotal  = pHuman->GetHonor() - nHonor;
            if (nTotal < 0)
                nTotal = 0;

            pHuman->SetHonor(nTotal);


        ENDHUMANDEFINE
            return	0;

        LUA_LEAVE_FUNCTION
            return  0;
    }
}

#endif
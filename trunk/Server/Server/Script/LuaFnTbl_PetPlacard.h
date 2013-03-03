// LuaFnTbl_PetPlacard.h
// 
// ���﹫������LUA�����ӿ�
// 
//////////////////////////////////////////////////////////

#ifndef __LUAFNTBL_PETPLACARD_H__
#define __LUAFNTBL_PETPLACARD_H__

#include "LuaFnMacro.h"
#include "Obj_Monster.h"
#include "PetPlacard.h"

#include "GCPetPlacardList.h"
#include "GCOperateResult.h"

namespace LuaFnTbl
{
	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-07
	*     �������� : LuaFnIssuePetPlacardByIndex
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*				pszMessage
	*					����������
	*				
	*     ����˵�� :
	*				�������﹫��
	*				
	*/
	INT	LuaFnIssuePetPlacardByIndex(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;
		INT nPetIndex;
		const CHAR *pszMessage;

		sceneId				=	Lua_ValueToNumber(L,1);
		selfId				=	Lua_ValueToNumber(L,2);
		targetId			=	Lua_ValueToNumber(L,3);
		nPetIndex			=	Lua_ValueToNumber(L,4);
		pszMessage			=	Lua_ValueToString(L,5);

		BEGINSCENEDEFINE("LuaFnIssuePetPlacardByIndex")
		{
			Assert(selfId != INVALID_ID && targetId != INVALID_ID && "LuaFnIssuePetPlacardByIndex");
			if(selfId != INVALID_ID && targetId != INVALID_ID)
			{
				Obj_Monster *pNpc = (Obj_Monster*)(pScene->GetObjManager()->GetObj(targetId));
				Obj_Human* pHuman = (Obj_Human*)(pScene->GetObjManager()->GetObj(selfId));
				Assert(pNpc != NULL && pHuman != NULL && "LuaFnIssuePetPlacardByIndex");
				if(pNpc != NULL
					&& pNpc->GetObjType() == Obj::OBJ_TYPE_MONSTER
					&& pHuman != NULL
					&& pHuman->GetObjType() == Obj::OBJ_TYPE_HUMAN)
				{
					PetPlacardSystem *pPetPlacardSystem = pNpc->GetPetPlacardSystem();
					if(pPetPlacardSystem == NULL)
					{
						pNpc->CreatePetPlacardSystem();
						pPetPlacardSystem = pNpc->GetPetPlacardSystem();
					}

					if(pPetPlacardSystem != NULL)
					{
						PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nPetIndex);
						ORESULT oResult = pPetPlacardSystem->IssuePlacard(pHuman, guidPet, pszMessage);
						if(OR_SUCCEEDED(oResult))
						{
							Lua_PushNumber(L,1);
							return 1;
						}
						else
						{
							pHuman->SendOperateResultMsg(oResult);
						}
					}
				}
			}

		}
		ENDSCENEDEFINE

		Lua_PushNumber(L,0);
		return	1;

		LUA_LEAVE_FUNCTION

		Lua_PushNumber(L,0);
		return 1 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-07
	*     �������� : LuaFnIssuePetPlacard
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				petGUID_H
	*					����GUID��λ
	*				petGUID_L
	*					����GUID��λ
	*				pszMessage
	*					����������
	*				
	*     ����˵�� :
	*				�������﹫��
	*				
	*/
	INT	LuaFnIssuePetPlacard(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;
		UINT petGUID_H;
		UINT petGUID_L;
		const CHAR *pszMessage;

		sceneId				=	Lua_ValueToNumber(L,1);
		selfId				=	Lua_ValueToNumber(L,2);
		targetId			=	Lua_ValueToNumber(L,3);
		petGUID_H			=	(UINT)(Lua_ValueToNumber(L,4));
		petGUID_L			=	(UINT)(Lua_ValueToNumber(L,5));
		pszMessage			=	Lua_ValueToString(L,6);

		BEGINSCENEDEFINE("LuaFnIssuePetPlacard")
		{
			Assert(selfId != INVALID_ID && targetId != INVALID_ID && "LuaFnIssuePetPlacard");
			if(selfId != INVALID_ID && targetId != INVALID_ID)
			{
				Obj_Monster *pNpc = (Obj_Monster*)(pScene->GetObjManager()->GetObj(targetId));
				Obj_Human* pHuman = (Obj_Human*)(pScene->GetObjManager()->GetObj(selfId));
				Assert(pNpc != NULL && pHuman != NULL && "LuaFnIssuePetPlacard");
				if(pNpc != NULL
					&& pNpc->GetObjType() == Obj::OBJ_TYPE_MONSTER
					&& pHuman != NULL
					&& pHuman->GetObjType() == Obj::OBJ_TYPE_HUMAN)
				{
					PetPlacardSystem *pPetPlacardSystem = pNpc->GetPetPlacardSystem();
					if(pPetPlacardSystem == NULL)
					{
						pNpc->CreatePetPlacardSystem();
						pPetPlacardSystem = pNpc->GetPetPlacardSystem();
					}

					if(pPetPlacardSystem != NULL)
					{
						PET_GUID_t guidPet;
						guidPet.m_uHighSection	= petGUID_H;
						guidPet.m_uLowSection	= petGUID_L;
						ORESULT oResult = pPetPlacardSystem->IssuePlacard(pHuman, guidPet, pszMessage);
						if(OR_SUCCEEDED(oResult))
						{
							Lua_PushNumber(L,1);
							return 1;
						}
						else
						{
							pHuman->SendOperateResultMsg(oResult);
						}
					}
				}
			}

		}
		ENDSCENEDEFINE

		Lua_PushNumber(L,0);
		return	1;

		LUA_LEAVE_FUNCTION

		Lua_PushNumber(L,0);
		return 1 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-07
	*     �������� : LuaFnIssuePetPlacard
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				
	*     ����˵�� :
	*				ȡ�����﹫��
	*				
	*/
	INT	LuaFnCancelPetPlacard(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;

		sceneId				=	Lua_ValueToNumber(L,1);
		selfId				=	Lua_ValueToNumber(L,2);
		targetId			=	Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnCancelPetPlacard")
		{
			Assert(selfId != INVALID_ID && targetId != INVALID_ID && "LuaFnCancelPetPlacard");
			if(selfId != INVALID_ID && targetId != INVALID_ID)
			{
				Obj_Monster *pNpc = (Obj_Monster*)(pScene->GetObjManager()->GetObj(targetId));
				Obj_Human* pHuman = (Obj_Human*)(pScene->GetObjManager()->GetObj(selfId));
				Assert(pNpc != NULL && pHuman != NULL && "LuaFnCancelPetPlacard");
				if(pNpc != NULL
					&& pNpc->GetObjType() == Obj::OBJ_TYPE_MONSTER
					&& pHuman != NULL
					&& pHuman->GetObjType() == Obj::OBJ_TYPE_HUMAN)
				{
					PetPlacardSystem *pPetPlacardSystem = pNpc->GetPetPlacardSystem();
					if(pPetPlacardSystem != NULL)
					{
						pPetPlacardSystem->CancelPlacard(pHuman->GetGUID());
						Lua_PushNumber(L,1);
						return 1;
					}
				}
			}

		}
		ENDSCENEDEFINE

		Lua_PushNumber(L,0);
		return	1;

		LUA_LEAVE_FUNCTION

		Lua_PushNumber(L,0);
		return 1 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2006-03-07
	*     �������� : LuaFnDispatchPetPlacardList
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				firstCheckId
	*					�����ҵ����Ӧ�ô���һ����Ϣ��ʼ��������ˢ��
	*					˳��=���ϴ�ˢ�µĵ�����һ��ID��
	*					����=���ϴ�ˢ�µĵ�һ��ID��
	*				secondCheckId
	*					˳��=���ϴ�ˢ�µĵ����ڶ���ID)
	*					����=���ϴ�ˢ�µĵڶ���ID��
	*				nextPageOrPrevPage
	*					!=0Ϊ˳��
	*				
	*     ����˵�� :
	*				�������Ĺ����б�
	*				
	*/
	INT	LuaFnDispatchPetPlacardList(Lua_State* L) 
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;
	  ObjID_t targetId;
	  GUID_t firstCheckId;
	  GUID_t secondCheckId;
	  INT nextPageOrPrevPage;

		sceneId				=	Lua_ValueToNumber(L,1);
		selfId				=	Lua_ValueToNumber(L,2);
		targetId			=	Lua_ValueToNumber(L,3);
		firstCheckId		=	Lua_ValueToNumber(L,4);
		secondCheckId		=	Lua_ValueToNumber(L,5);
		nextPageOrPrevPage	=	Lua_ValueToNumber(L,6);

		BEGINSCENEDEFINE("LuaFnDispatchPetPlacardList")
		{
			Assert(selfId != INVALID_ID && targetId != INVALID_ID && "LuaFnDispatchPetPlacardList");
			if(selfId != INVALID_ID && targetId != INVALID_ID)
			{
				Obj_Monster *pNpc = (Obj_Monster*)(pScene->GetObjManager()->GetObj(targetId));
				Obj_Human* pHuman = (Obj_Human*)(pScene->GetObjManager()->GetObj(selfId));
				Assert(pNpc != NULL && pHuman != NULL && "LuaFnDispatchPetPlacardList");
				if(pNpc != NULL
					&& pNpc->GetObjType() == Obj::OBJ_TYPE_MONSTER
					&& pHuman != NULL
					&& pHuman->GetObjType() == Obj::OBJ_TYPE_HUMAN)
				{
					PetPlacardSystem *pPetPlacardSystem = pNpc->GetPetPlacardSystem();
					if(pPetPlacardSystem != NULL)
					{
						BOOL bNextPage = (nextPageOrPrevPage != 0)?(TRUE):(FALSE);

						const PetPlacard *pPetPlacard = pPetPlacardSystem->GetPetPlacard();
						INT nIndex = -1;
						const _PET_PLACARD_ITEM *pItem;
						if(nIndex == -1 && firstCheckId != INVALID_ID)
						{
							nIndex = pPetPlacard->GetIndexByHumanGUID(firstCheckId);
						}
						if(nIndex == -1 && secondCheckId != INVALID_ID)
						{
							nIndex = pPetPlacard->GetIndexByHumanGUID(secondCheckId);
						}

						INT nMaxSendCount = MAX_PETPLACARD_LIST_ITEM_NUM;
						if(nMaxSendCount > 0)
						{
							if(nextPageOrPrevPage == 0 && nIndex != -1)
							{
								INT nTempCount = nMaxSendCount;
								pItem = pPetPlacard->PrevValidItem(&nIndex);
								if(pItem == NULL)
								{
									nIndex = -1;
								}
								else
								{
									while(pItem != NULL && nTempCount > 0)
									{
										pItem = pPetPlacard->PrevValidItem(&nIndex);
										nTempCount--;
									}
								}
							}

							GCPetPlacardList msgPetPlacardList;

							if(nIndex != -1)
								pItem = pPetPlacard->NextValidItem(&nIndex);
							else
								pItem = pPetPlacard->FirstValidItem(&nIndex);
							while(pItem != NULL && nMaxSendCount > 0)
							{
								msgPetPlacardList.AddItem(pItem);

								pItem = pPetPlacard->NextValidItem(&nIndex);
								nMaxSendCount--;
							}

							if(msgPetPlacardList.GetItemCount() > 0)
							{
								pHuman->GetPlayer()->SendPacket(&msgPetPlacardList);
							}
							Lua_PushNumber(L,1);
							return 1;
						}
					}
				}
			}

		}
		ENDSCENEDEFINE

		Lua_PushNumber(L,0);
		return 1;

	LUA_LEAVE_FUNCTION

		Lua_PushNumber(L,0);
		return 1;
	}
}

#endif // __LUAFNTBL_PETPLACARD_H__

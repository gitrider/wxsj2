//-----------------------------------------------------------------------------
// 文件名 : LuaFnTbl_Ability.h
// 模块	:	Script
// 功能	 :  生活技能相关函数
// 修改历史:
//	yangfei创建
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_ABILITY_H__
#define __LUAFNTBL_ABILITY_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "Obj_ItemBox.h"
#include "GameTable.h"
#include "ItemManager.h"
#include "GCNewItemBox.h"
#include "GCAbilityLevel.h"
#include "Ability.h"
#include "GCPrescription.h"
#include "GCAbilityExp.h"
#include "AbilityManager.h"
#include "Obj_Monster.h"
#include "StoreManager.h"
#include "GCNotifyEquip.h"
#include "MonsterManager.h"
#include "Obj_Human.h"
#include "HumanItemLogic.h"
#include "GCItemInfo.h"
#include "ItemBoxManager.h"
#include "GCAbilitySucc.h"
#include "ItemOperator.h"
#include "GameDefine_Item.h"
#include "Obj_Item.h"
#include "Obj_ItemManager.h"
#include "GCDiscardItemResult.h"
#include "LogDefine.h"
namespace LuaFnTbl
{

	/*
	 *	获得对应ObjID 的玩家的全局ID(整个世界唯一)
	 *     开发人员 :
	 *     编写时间 : 2006-1-20
	 *     函数名称 : LuaFnGetHumanGUID
	 *     参数说明 :  
	 *			SceneID_t	SceneID   场景编号
	 *          ObjID_t		HumanID	  角色编号
     *
	 *	   返回  角色的全局ID
	 *     功能说明 :
	 *
	 */

	INT	LuaFnGetHumanGUID(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		GUID_t	guid;


		BEGINHUMANDEFINE("LuaFnGetHumanGUID")
		guid = pHuman->GetGUID();
		Lua_PushNumber(L,guid);
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetGemEmbededCount
	*     参数说明 : 
	*     功能说明 :
	*/
	INT LuaFnGetGemEmbededCount(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT BagPos;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		BagPos			=	Lua_ValueToNumber(L,3);
	
		BEGINHUMANDEFINE("LuaFnGetGemEmbededCount")
		Item*	pEquipItem	=	HumanItemLogic::GetBagItem(pHuman,BagPos);
		if(pEquipItem)
		{
			Lua_PushNumber(L,(INT)pEquipItem->GetEquipGemCount());
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
	*     开发人员 :
	*     编写时间 : 2006-1-17
	*     函数名称 : LuaFnGetBagItemParam
	*     参数说明 : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*				UINT				StartPos
	*				INT					Type
	*     功能说明 :
	*		获取背包中物品的参数
	*/
	INT LuaFnGetBagItemParam(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t selfId;
		UINT BagPos;
		UINT StartPos;
		INT Type;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		BagPos			=	Lua_ValueToNumber(L,3);
		StartPos		=	Lua_ValueToNumber(L,4);
		Type			=   Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnGetBagItemParam")
			Item*	pBagItem	=	HumanItemLogic::GetBagItem(pHuman,BagPos);
		if(pBagItem)
		{

			if(Type<IPV_CHAR || Type>IPV_INT)
				Assert(FALSE);
			ItemParamValue ipv = (ItemParamValue)Type;
			Lua_PushNumber(L,(INT)pBagItem->GetItemParam(StartPos,ipv));
			return 1;
		}
		else
		{
			Lua_PushNumber(L,-1);
			return 1;
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2006-1-17
	*     函数名称 : LuaFnSetBagItemParam
	*     参数说明 : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*				UINT				StartPos
	*				INT					Type
	*				INT					Value
	*     功能说明 :
	*		设置背包中物品的参数
	*/
	INT LuaFnSetBagItemParam(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t selfId;
		UINT BagPos;
		UINT StartPos;
		INT Type;
		INT value;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		BagPos			=	Lua_ValueToNumber(L,3);
		StartPos		=	Lua_ValueToNumber(L,4);
		Type			=	Lua_ValueToNumber(L,5);
		value			=	Lua_ValueToNumber(L,6);


		BEGINHUMANDEFINE("LuaFnSetBagItemParam")
		Item*	pBagItem	=	HumanItemLogic::GetBagItem(pHuman,BagPos);
		Assert(pBagItem);

		HumanItemLogic::SetBagItemParam(pHuman,BagPos,StartPos,Type,value);
	
		ENDHUMANDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2006-1-17
	*     函数名称 : LuaFnGetBagItemDur
	*     参数说明 : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*     功能说明 :
	*		获取背包中物品的耐久度
	*/
	INT LuaFnGetBagItemDur(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT BagPos;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		BagPos			=	Lua_ValueToNumber(L,3);


		BEGINHUMANDEFINE("LuaFnGetBagItemDur")
		Item*	pBagItem	=	HumanItemLogic::GetBagItem(pHuman,BagPos);
		if(pBagItem)
		{
			Lua_PushNumber(L,(INT)pBagItem->GetDurPoints());
			return 1;
		}
		else
		{
			Lua_PushNumber(L,-1);
			return 1;
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2006-1-17
	*     函数名称 : LuaFnSetBagItemDur
	*     参数说明 : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*				INT					Dur
	*     功能说明 :
	*		设置背包中物品的耐久度
	*/
	INT LuaFnSetBagItemDur(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t selfId;
		UINT BagPos;
		INT Dur;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		BagPos			=	Lua_ValueToNumber(L,3);
		Dur				=	Lua_ValueToNumber(L,4);


		BEGINHUMANDEFINE("LuaFnSetBagItemDur")
		Item*	pBagItem	=	HumanItemLogic::GetBagItem(pHuman,BagPos);
		Assert(pBagItem);
		
		HumanItemLogic::SetBagItemDur(pHuman,BagPos,Dur);
		
		GCItemInfo Msg;
		pBagItem->SaveValueTo(Msg.getItem());
		Msg.setID(BagPos);
		Msg.setIsNull(FALSE);
		pHuman->GetPlayer()->SendPacket(&Msg);
		ENDHUMANDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     开发人员 :王勇鸷
	*     编写时间 : 2008-4-21
	*     函数名称 : LuaFnSetBodyItemDur
	*     参数说明 : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				Pos
	*				INT					Dur
	*     功能说明 :
	*		设置玩家身上物品的耐久度
	*/
	INT LuaFnSetBodyItemDur(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t selfId;
		UINT Pos;
		INT Dur;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		Pos				=	Lua_ValueToNumber(L,3);
		Dur				=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnSetEquipItemDur")
		pHuman->GetDB()->SetEquipDur((HUMAN_EQUIP)Pos,Dur);
	
		Item*	pItem	=	HumanItemLogic::GetEquip(pHuman,(HUMAN_EQUIP)Pos);
		Assert(pItem);

		GCItemInfo Msg;
		Msg.setOpType(GCItemInfo::OPERATOR_BODY);
		pItem->SaveValueTo(Msg.getItem());
		Msg.setID(Pos);
		Msg.setIsNull(FALSE);
		pHuman->GetPlayer()->SendPacket(&Msg);
		ENDHUMANDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     开发人员 :王勇鸷
	*     编写时间 : 2008-4-21
	*     函数名称 : LuaFnGetBodyItemDur
	*     参数说明 : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				Pos
	*				INT					Dur
	*     功能说明 :
	*		得到玩家身上物品的耐久度
	*/
	INT LuaFnGetBodyItemDur(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT Pos;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		Pos				=	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetBodyItemDur")

		Item*	pItem	=	HumanItemLogic::GetEquip(pHuman,(HUMAN_EQUIP)Pos);
	
		if(pItem)
		{
			Lua_PushNumber(L,(INT)pItem->GetDurPoints());
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
	*     开发人员 :王能鸷
	*     编写时间 : 2008-4-21
	*     函数名称 : LuaFnGetNotBindItemCountInBody
	*     参数说明 : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*     功能说明 :
	*		得到玩家身上未绑定物品数
	*/
	INT LuaFnGetNotBindItemCountInBody(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	

		BEGINHUMANDEFINE("LuaFnGetNotBindItemCountInBody")

		INT nCount = 0;

		for ( INT Index = 0; Index < HEQUIP_NUMBER; Index++ )
		{
			Item* pItem = HumanItemLogic::GetEquip( pHuman, (HUMAN_EQUIP)Index );
			if ( pItem != NULL && !pItem->IsEmpty() && pItem->GetItemBind() == FALSE  )
			{
				nCount++;
			}
		}

		Lua_PushNumber(L,nCount);
		return 1;

		ENDHUMANDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     开发人员 :王能鸷
	*     编写时间 : 2008-4-21
	*     函数名称 : LuaFnGetNotBindItemCountInBag
	*     参数说明 : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*     功能说明 :
	*		得到玩家背包中未绑定物品数
	*/
	INT LuaFnGetNotBindItemCountInBag(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	

		BEGINHUMANDEFINE("LuaFnGetNotBindItemCountInBag")

		INT nCount = 0;

		INT nMaxBagSize = pHuman->GetBaseContain()->GetContainerSize();
		for ( INT i = 0; i < nMaxBagSize; i++ )
		{
			Item* pItem = HumanItemLogic::GetBagItem( pHuman, i + BASE_CONTAINER_OFFSET );
			if ( pItem != NULL && !pItem->IsEmpty() && pItem->GetItemBind() == FALSE )
			{
				nCount++;
			}
		}

		Lua_PushNumber(L,nCount);
		return 1;

		ENDHUMANDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 王能鸷
	*     编写时间 : 2008-4-21
	*     函数名称 : LuaFnDropNotBindItemInBag
	*     参数说明 : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				DropIndex			掉落位置
	*     功能说明 :
	*		玩家背包中未绑定物品掉落
	*/
	INT LuaFnDropNotBindItemInBag(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT DropIndex;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		DropIndex		=	Lua_ValueToNumber(L,3);	

		BEGINHUMANDEFINE("LuaFnDropNotBindItemInBag")

		INT nMaxBagSize = pHuman->GetBaseContain()->GetContainerSize();
		for ( INT i = 0; i < nMaxBagSize; i++ )
		{
			Item* pItem = HumanItemLogic::GetBagItem( pHuman, i + BASE_CONTAINER_OFFSET );
			UINT itemIndex = 
				HumanItemLogic::GetBagItem(pHuman,i + BASE_CONTAINER_OFFSET)->GetItemTableIndex();

			if ( pItem != NULL && !pItem->IsEmpty() && pItem->GetItemBind() == FALSE )
			{
				if ( DropIndex == i+1 )
				{
					//创建物品到地图
					WORLD_POS	Pos	=	*pHuman->getWorldPos() ;
					Pos.m_fX +=  ITEM_DROP_DISTANCE;
				
					ITEM_LOG_PARAM	ItemLogParam;
					ItemLogParam.OpType		= ITEM_DIE_LOST;
					ItemLogParam.SceneID	= pScene->SceneID();
					ItemLogParam.NpcType	= pHuman->GetDataID();
					ItemLogParam.XPos		= Pos.m_fX;
					ItemLogParam.ZPos		= Pos.m_fZ;

					Obj_ItemManager*	pIBManager = pScene->GetObj_ItemManager();
					Assert(pIBManager);
					Obj_Item* pObj_Item = (Obj_Item*)pIBManager->CreatePersonDropObj_Item(&Pos,pItem);
					pObj_Item->SetDropObjID(pHuman->GetID());
					pObj_Item->SetActiveFlag(TRUE);
					SaveItemLog(&ItemLogParam);

					//该物品掉落
					BOOL bRet = HumanItemLogic::EraseBagItem(&ItemLogParam, pHuman, i + BASE_CONTAINER_OFFSET );

					//返回该玩家销毁物品消息
					GCDiscardItemResult Msg;
					Msg.setResult( DISCARDITEM_SUCCESS );

					if(bRet) 
						SaveItemLog(&ItemLogParam);
					Msg.setItemTableIndex( itemIndex );
					Msg.setBagIndex( i + BASE_CONTAINER_OFFSET );
					pHuman->GetPlayer()->SendPacket( &Msg );

					break;
				}
			}
		}

		return 0;

		ENDHUMANDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE

	}//LuaFnDropNotBindItemInBag end


	/** -----------------------------------------------------------------------
	*     开发人员 : 王能鸷
	*     编写时间 : 2008-4-21
	*     函数名称 : LuaFnDropNotBindItemInBody
	*     参数说明 : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*     功能说明 :
	*		玩家身上未绑定物品掉落
	*/
	INT LuaFnDropNotBindItemInBody(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT DropIndex;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		DropIndex		=	Lua_ValueToNumber(L,3);	

		BEGINHUMANDEFINE("LuaFnDropNotBindItemInBody")

		for ( INT i = 0; i < HEQUIP_NUMBER; i++ )
		{
			Item* pItem = HumanItemLogic::GetEquip( pHuman, (HUMAN_EQUIP)i );
			UINT itemIndex = 
				HumanItemLogic::GetEquip(pHuman,(HUMAN_EQUIP)i)->GetItemTableIndex();

			if ( pItem != NULL && !pItem->IsEmpty() && pItem->GetItemBind() == FALSE  )
			{
				if ( DropIndex == i+1 )
				{
					//创建物品到地图
					WORLD_POS	Pos	=	*pHuman->getWorldPos() ;
					Pos.m_fX +=  ITEM_DROP_DISTANCE;
				
					ITEM_LOG_PARAM	ItemLogParam;
					ItemLogParam.OpType		= ITEM_DIE_LOST;
					ItemLogParam.SceneID	= pScene->SceneID();
					ItemLogParam.NpcType	= pHuman->GetDataID();
					ItemLogParam.XPos		= Pos.m_fX;
					ItemLogParam.ZPos		= Pos.m_fZ;

					Obj_ItemManager*	pIBManager = pScene->GetObj_ItemManager();
					Assert(pIBManager);
					Obj_Item* pObj_Item = (Obj_Item*)pIBManager->CreatePersonDropObj_Item(&Pos,pItem);
					pObj_Item->SetDropObjID(pHuman->GetID());
					pObj_Item->SetActiveFlag(TRUE);
					SaveItemLog(&ItemLogParam);

					//该物品掉落
					BOOL bRet = HumanItemLogic::EraseEquip( pHuman, (HUMAN_EQUIP)(i) );

					//返回该玩家销毁物品消息
					GCDiscardItemResult Msg;
					Msg.setResult( DISCARDITEM_SUCCESS );

					if(bRet) 
						SaveItemLog(&ItemLogParam);
					Msg.setItemTableIndex( itemIndex );
					Msg.setBagIndex( i );
					Msg.setOpt(GCDiscardItemResult::FromBody);
					pHuman->GetPlayer()->SendPacket( &Msg );


					break;
				}
			}
		}

		return 0;

		ENDHUMANDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2006-3-27
	*     函数名称 : LuaFnGetBagItemIdent
	*     参数说明 : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*     功能说明 :
	*		获取背包中物品的鉴定属性
	*/
	INT LuaFnGetBagItemIdent(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT BagPos;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		BagPos			=	Lua_ValueToNumber(L,3);


		BEGINHUMANDEFINE("LuaFnGetBagItemIdent")
		Item*	pBagItem	=	HumanItemLogic::GetBagItem(pHuman,BagPos);
		if(pBagItem)
		{
			Lua_PushNumber(L,(INT)pBagItem->GetItemIdent());
			return 1;
		}
		else
		{
			Lua_PushNumber(L,-1);
			return 1;
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2006-3-27
	*     函数名称 : LuaFnSetBagItemDur
	*     参数说明 : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*     功能说明 :
	*		设置背包中物品的鉴定属性
	*/
	INT LuaFnSetBagItemIdent(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT BagPos;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		BagPos			=	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetBagItemIdent")
		Item*	pBagItem	=	HumanItemLogic::GetBagItem(pHuman,BagPos);
		Assert(pBagItem);

		HumanItemLogic::SetBagItemIdent(pHuman,BagPos);

		GCItemInfo Msg;
		pBagItem->SaveValueTo(Msg.getItem());
		Msg.setID(BagPos);
		Msg.setIsNull(FALSE);
		pHuman->GetPlayer()->SendPacket(&Msg);
		ENDHUMANDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
    
	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetItemQuality
	*     参数说明 : 
	*     功能说明 :
	*/
	INT LuaFnGetItemQuality(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		UINT itemIndex;

			itemIndex = Lua_ValueToNumber(L, 1);

		TSerialHelper itemHelper(itemIndex);
		//Lua_PushNumber(L, itemHelper.GetItemQual());
		return 1;

		LUA_LEAVE_FUNCTION
			RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	胡繁
	 *	编写时间：	2006-4-6	15:04
	 *	函数名称：	LuaFnGetItemType
	 *	参数说明：
	 *				uSerialNumber
	 *					物品序列号
	 *
	 *	功能说明：	根据物品序列号得到物品的 Type
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnGetItemType(Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
		UINT uSerialNumber;

		uSerialNumber = Lua_ValueToNumber(L, 1);

		Lua_PushNumber(L, GetSerialType(uSerialNumber));
		return 1;

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	胡繁
	 *	编写时间：	2006-4-6	14:00
	 *	函数名称：	LuaFnGetBagEquipType
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					角色编号
	 *				nBagPos
	 *					背包格子
	 *
	 *	功能说明：	通过背包格子查到该物品是哪种装备（如果不是装备，则返回 -1）
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnGetBagEquipType(Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nBagPos;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		nBagPos = Lua_ValueToNumber(L,3);
		
		BEGINHUMANDEFINE("LuaFnGetBagEquipType")
			Item* pItem = HumanItemLogic::GetBagItem(pHuman, nBagPos);
			if( pItem == NULL
			 || pItem->GetItemClass() != ICLASS_EQUIP
			 )
			{
				Lua_PushNumber(L, -1);
			}
			else
			{
				Lua_PushNumber(L, pItem->GetEquipPoint());
			}

			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

	LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :王勇鸷
	*     编写时间 : 2008-3-20
	*     函数名称 : LuaFnGetItemIsEmpty
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :判断背包中是否有空位 ,返回：INVALID_INDEX(-1) 背包满了
	*/
	INT LuaFnGetItemIsEmpty(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetItemIsEmpty")
			Lua_PushNumber(L,pHuman->GetBaseContain()->GetEmptyItemIndex());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetItemIndex
	*     参数说明 : 
	*     功能说明 :
	*/
	INT LuaFnGetItemIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		UINT itemIndex;

		itemIndex = Lua_ValueToNumber(L, 1);
			
		TSerialHelper itemHelper(itemIndex);
		Lua_PushNumber(L, itemHelper.GetItemIndex());
		return 1;

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetItemBoxRequireAbilityID
	*     参数说明 : 
	*     功能说明 :
	*				获得Obj_ItemBox需要的生活技能编号
	*		SceneID
	*				场景编号
	*		ItemBoxID
	*				Obj_ItemBox编号
	*
	*		返回
	*			AbilityID
	*				需要生活技能的编号
	*/
	INT	LuaFnGetItemBoxRequireAbilityID(Lua_State* L) 
	{												 
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t ItemBoxID;

		sceneId			=	Lua_ValueToNumber(L,1);
		ItemBoxID		=	Lua_ValueToNumber(L,2);

		INT			ItemBoxType;

		BEGINSCENEDEFINE("LuaFnGetItemBoxRequireAbilityID")

		Obj_ItemBox* pIB = pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
		if(!pIB)
		{
			RETURNFALSE
		}

		ItemBoxType	=	pIB->GetType();

		_GROW_POINT_INFO*	pResult =	g_GrowPointInfoTbl.Get(ItemBoxType);
		
		if(!pResult)
		{
			RETURNFALSE
		}

		Lua_PushNumber(L,pResult->m_OpenAbility);
		return 1;
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddItemToBox
	*     参数说明 : 
	*     功能说明 :
	*				添加物品到Obj_ItemBox
	*
	*
	*	  SceneID
	*			场景ID
	*	  ItemBoxID
	*			Obj_ItemBox编号
	*	  ItemCount
	*			放入Obj_ItemBox的物品个数,总共最大10个
	*	  Item0
	*			0号物品
	*	  ...
	*
	*     Item9 
	*			9号物品
	*     
	*/
	INT LuaFnAddItemToBox(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t ItemBoxID;
		INT iBoxItemCount;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		ItemBoxID		=	Lua_ValueToNumber(L,2);
		iBoxItemCount	=	Lua_ValueToNumber(L,3);


		if(iBoxItemCount>0)
		{
			BEGINSCENEDEFINE("LuaFnAddItemToBox")
			Obj_ItemBox* pIB = pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
			if(pIB)
			{
				ITEM_LOG_PARAM	ItemLogParam;
				ItemLogParam.OpType		= ITEM_CREATE_SCRIPT_TO_BOX;
				ItemLogParam.SceneID	= pScene->SceneID();
				ItemLogParam.XPos		= pIB->getWorldPos()->m_fX;
				ItemLogParam.ZPos		= pIB->getWorldPos()->m_fZ;
			
				for(INT iIndex=0;iIndex<iBoxItemCount;iIndex++)
				{
					INT iItemType = Lua_ValueToNumber(L,4+iIndex);
					INT iQuality = 9;

					_MY_TRY
					{
						pIB->CreateItem(&ItemLogParam,iItemType,iQuality);
						SaveItemLog(&ItemLogParam);
					}
					_MY_CATCH
					{
						SaveCodeLog( ) ;
					}
				}
			}
			ENDSCENEDEFINE
		}

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnItemBoxEnterScene
	*     参数说明 : 
	*     功能说明 :
	*				Obj_ItemBox进入场景
	*  
	*	  fx
	*				Obj_ItemBox进入场景X坐标
	*	  fz	
	*				Obj_ItemBox进入场景Z坐标
	*	  iBoxType  
	*				Obj_ItemBox类型
	*	  SceneID	
	*				场景编号
	*	  
	*	  ItemCount  
	*				放入的物品个数 最大10个
	*	  Item0    
	*				放入的0号物品
	*	  。。。
	*	  Item9		放入的9号物品
	*
	*	  ItemBoxID 
	*               返回的ItemBoxID
	*/
	INT LuaFnItemBoxEnterScene(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		FLOAT m_fX;
		FLOAT m_fZ;
		INT iBoxType;
		SceneID_t sceneId;
		INT iBoxItemCount;

		WORLD_POS pos;
		pos.m_fX		= Lua_ValueToNumber(L,1);
		pos.m_fZ		= Lua_ValueToNumber(L,2);
		iBoxType		= Lua_ValueToNumber(L,3);
		sceneId			= Lua_ValueToNumber(L,4);
		iBoxItemCount	= Lua_ValueToNumber(L,5);
		
		
		BEGINSCENEDEFINE("LuaFnItemBoxEnterScene")
		Obj_ItemBox* pIB = pScene->GetItemBoxManager()->CreateGrowPointItemBox(iBoxType);
		if(pIB)
		{
			ITEM_LOG_PARAM	ItemLogParam;
			ItemLogParam.OpType		= ITEM_CREATE_SCRIPT_TO_BOX;
			ItemLogParam.SceneID	= pScene->SceneID();
			ItemLogParam.XPos		= pos.m_fX;
			ItemLogParam.ZPos		= pos.m_fZ;

			for(INT iIndex=0;iIndex<iBoxItemCount;iIndex++)
			{
				INT iItemType = Lua_ValueToNumber(L,6+iIndex);
				Assert(iItemType>0);
				INT iQuality = 9;
				_MY_TRY
				{
					pIB->CreateItem(&ItemLogParam,iItemType,iQuality);
					SaveItemLog(&ItemLogParam);
				}
				_MY_CATCH
				{
					SaveCodeLog( ) ;
				}
			}
			pScene->ObjectEnterScene(pIB );
			pIB->setWorldPos( &pos );
			pIB->SetActiveFlag( TRUE );
			Lua_PushNumber(L,pIB->GetID());
			return 1;
		}
		else
		{
			RETURNFALSE
		}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnQueryHumanAbility
	*     参数说明 : 
	*     功能说明 :
	*				查询human Ability是否学习
	*	
	*		sceneId
	*					场景编号
	*		selfId
	*					角色编号
	*		abilityID
	*					生活技能ID
	*
	*/
	INT LuaFnQueryHumanAbility(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
			SceneID_t sceneId;
			ObjID_t selfId;
			AbilityID_t abilityId;

			sceneId			=	Lua_ValueToNumber(L,1);
			selfId			=	Lua_ValueToNumber(L,2);	
			abilityId       =	Lua_ValueToNumber(L,3);

			BEGINHUMANDEFINE("LuaFnQueryHumanAbility")
			if(pHuman->__IsAbilityHaveLearned(abilityId))
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

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnQueryHumanAbilityLevel
	*     参数说明 : 
	*     功能说明 :
	*				查询Obj_Human生活技能等级 SceneID,HumanID,AbilityID
	*/
	INT LuaFnQueryHumanAbilityLevel(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		AbilityID_t AbilityID;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		AbilityID       =	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnQueryHumanAbility")
		INT AbilityLevel =  pHuman->__GetAbilityLevel(AbilityID);
		Lua_PushNumber(L,AbilityLevel);
		return 1;
		ENDHUMANDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetHumanAbilityLevel
	*     参数说明 : 
	*     功能说明 :
	*				设置人物生活技能级别,SceneID,HumanID,AbilityID,AbilityLevel
	*/
	INT LuaFnSetHumanAbilityLevel(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t selfId;
		AbilityID_t abilityId;
		INT abilityLvl;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		abilityId       =	Lua_ValueToNumber(L,3);
		abilityLvl		=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnSetHumanAbilityLevel")

		Ability* pAbility = g_pAbilityManager->GetAbility(abilityId);
		if ( abilityLvl>pAbility->AbilityLevelLimit() )
		{
			Assert(FALSE && "技能等级超过上限了");
			return 0;
		}

		pHuman->__SetAbilityLevel(abilityId, abilityLvl);
		GCAbilityLevel Msg;
		Msg.SetAbilityId(abilityId);
		Msg.SetLevel(abilityLvl);
		//Msg.SetResult()
		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg );



		ABILITY_LOG_PARAM	AbilityLogParam;

		AbilityLogParam.CharGUID	=	pHuman->GetGUID();
		AbilityLogParam.AbilityID	=	abilityId;
		AbilityLogParam.Level		=	abilityLvl;
		AbilityLogParam.OPType		=	ABILITY_OP_LEVELUP;
		AbilityLogParam.SceneID		=	pHuman->getScene()->SceneID();
		AbilityLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
		AbilityLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
		SaveAbilityLog(&AbilityLogParam);

		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}

	/** ----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-10-28
	*     函数名称 : LuaFnSetAbilityOperaTime
	*     参数说明 : 
	*     功能说明 :
	*				设置生活技能操作时间
	*
	*	  SceneID
	*				场景编号
	*	  HumanID
	*				角色编号
	*	  OperaTime
	*				操作时间
	*
	*
	*
	*/
	INT LuaFnSetAbilityOperaTime(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
			SceneID_t	sceneId;
			ObjID_t selfId;
			UINT OperaTime;

			sceneId  = Lua_ValueToNumber(L,1);
			selfId	 = Lua_ValueToNumber(L,2);
			OperaTime	=	Lua_ValueToNumber(L,3);
			
			BEGINHUMANDEFINE("LuaFnSetAbilityOperaTime")

			AbilityOpera*	pAbilityOpera;
			pAbilityOpera = pHuman->GetAbilityOpera();
			Assert( pAbilityOpera );
			pAbilityOpera->m_nMaxTime	=	OperaTime;

			ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;

	}


	/** ----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-10-28
	*     函数名称 : LuaFnGetAbilityOperaRobotTime
	*     参数说明 : 
	*     功能说明 :
	*				获得生活技能挂机器时间
	*
	*	  SceneID
	*				场景编号
	*	  HumanID
	*				角色编号
	*    返回
	*	  RobotTime
	*				操作时间
	*
	*
	*
	*/
	INT LuaFnGetAbilityOperaRobotTime(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId  = Lua_ValueToNumber(L,1);
		selfId	 = Lua_ValueToNumber(L,2);
		UINT		RobotTime;

		BEGINHUMANDEFINE("LuaFnGetAbilityOperaRobotTime")
			AbilityOpera*	pAbilityOpera;
			pAbilityOpera = pHuman->GetAbilityOpera();
			Assert( pAbilityOpera );
			RobotTime	=	 pAbilityOpera->m_nRobotTime;
			Lua_PushNumber(L,RobotTime);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** ----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-10-28
	*     函数名称 : LuaFnSetAbilityOperaRobotTime
	*     参数说明 : 
	*     功能说明 :
	*				设置生活技能挂机器时间
	*
	*	  SceneID
	*				场景编号
	*	  HumanID
	*				角色编号
	*	  
	*	  RobotTime
	*				操作时间
	*
	*
	*
	*/
	INT LuaFnSetAbilityOperaRobotTime(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT RobotTime;

		sceneId  = Lua_ValueToNumber(L,1);
		selfId	 = Lua_ValueToNumber(L,2);
		RobotTime	= Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetAbilityOperaRobotTime")
			AbilityOpera*	pAbilityOpera;
			pAbilityOpera = pHuman->GetAbilityOpera();
			Assert( pAbilityOpera );
			pAbilityOpera->m_nRobotTime	=	RobotTime;
		ENDHUMANDEFINE
		
		LUA_LEAVE_FUNCTION
		return 0 ;

	}

	/****************************************************************************
	 *	开发人员：	胡繁
	 *	编写时间：	2006-3-29	15:54
	 *	函数名称：	LuaFnSetAbilityExp
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				abilityId
	 *					生活技能编号
	 *				abilityExp
	 *					生活技能经验
	 *
	 *	功能说明：	设置生活技能经验，发送的时候 / 100 是为了整型运算方便
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnSetAbilityExp(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t selfId;
		AbilityID_t abilityId;
		INT abilityExp;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		abilityId       =	Lua_ValueToNumber(L,3);
		abilityExp		=	Lua_ValueToNumber(L,4);
		
		BEGINHUMANDEFINE("LuaFnSetAbilityExp")
			INT oldAbilityExp = pHuman->__GetAbilityExp(abilityId);
			
			abilityExp = (abilityExp+1000)+oldAbilityExp;
			//abilityExp<=上限 	验证数值是否超越该级别上限
			const AbilityLevelUpConfigManager *pConfigMgr = g_pAbilityManager->GetLevelUpConfigManager();
			Assert(pConfigMgr);

			Ability* pAbility = g_pAbilityManager->GetAbility(abilityId);
			Assert(pAbility);
			if( pAbility == NULL )
			{ // 不存在该技能
				return PACKET_EXE_CONTINUE;
			}
			const AbilityLevelUpConfig *pLevelUpConfig = pConfigMgr->GetLevelUpConfig(pAbility->LevelUpConfigIndex());
			Assert(pLevelUpConfig);

			const AbilityLevelUpInfo *pLevelUpInfo = pLevelUpConfig->GetLevelUpInfo(pHuman->__GetAbilityLevel(abilityId));
			Assert(pLevelUpInfo);
			if (abilityExp > pLevelUpInfo->GetCurrentLevelAbilityExpTop())
			{
				abilityExp = pLevelUpInfo->GetCurrentLevelAbilityExpTop();
			}

			pHuman->__SetAbilityExp(abilityId, abilityExp);

			if( abilityExp/100 - oldAbilityExp/100 >= 1 )
			{
				GCAbilityExp Msg;
				Msg.setAbilityId(abilityId);
				Msg.setExp(abilityExp);

				Player* pPlayer = pHuman->GetPlayer();
				pPlayer->SendPacket( &Msg );
			}

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetAbilityExpPlus
	*     参数说明 : 		
	*		SceneID
	*				场景编号
	*		HumanID
	*				角色编号
	*		AbilityID
	*				生活技能编号
	*     功能说明 :
	*				获得生活技能经验
	*     计算公式
	*	            1如果PlayerAbilityLevel <= C  玩家获得技能经验 =B
	*				2如果 B-(PlayerAbilityLevel-C)*D <0 则玩家获得技能经验=0
	*				3否则 玩家获得技能经验= B-(PlayerAbilityLevel-C)*D 之后取整
	*/
	INT	LuaFnGetAbilityExpPlus(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT itemTypeSn;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		itemTypeSn      =   Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetAbilityExpPlus")
		
		/*for(INT i=0;g_pAbilityManager->GetAbilityExpTblRowCount();i++)
		{*/	
			_AbilityExpTable* pAbilityExpTbl = g_pAbilityManager->GetAbilityExpTbl();
			//if(itemTypeSn == pAbilityExpTbl->m_AbilityItemTypeSn)取消对象的验证,因为还没搞明白对象
			//{
				INT abilityId	 =  pAbilityExpTbl->m_AbilityId;
				INT abilityLevel =  pHuman->__GetAbilityLevel(abilityId);
				INT abilityExp;

				if( abilityLevel < pAbilityExpTbl->m_AbilityLevel)
					abilityExp = pAbilityExpTbl->m_AbilityBaseExp;
				else 
				{	
					abilityExp = pAbilityExpTbl->m_AbilityBaseExp  - (abilityLevel - pAbilityExpTbl->m_AbilityLevel) * pAbilityExpTbl->m_AbilityCostPercent;
					if( abilityExp < 0)
						abilityExp = 0;
				}
				abilityExp = abilityExp+1000;//数值太小改大点
				Lua_PushNumber(L,abilityExp);
				return 1;
			//}
		//}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetAbilityExp
	*     参数说明 : 		
	*		SceneID
	*				场景编号
	*		HumanID
	*				角色编号
	*		AbilityID
	*				生活技能编号
	*     功能说明 :
	*				获得生活技能经验
	*/
	INT	LuaFnGetAbilityExp(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		AbilityID_t abilityId;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		abilityId       =	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetAbilityExp")
			INT				abilityExp;
		abilityExp = pHuman->__GetAbilityExp(abilityId);
		Lua_PushNumber(L,abilityExp);
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-10-17
	*     函数名称 : LuaFnSetPrescription
	*     参数说明 : 
	*     功能说明 :
	*				学会某项合成配方（只能操作学会，不能操作放弃具体某项配方，具体
	*				的配方放弃功能由技能放弃功能提供）
	*/
	INT LuaFnSetPrescription(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t selfId;
		PrescriptionID_t PrescrID;
		BOOL LearnOrAbandon;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		PrescrID		=	Lua_ValueToNumber(L,3);
		LearnOrAbandon	=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnSetPrescription")

		// TRUE to learn and FALSE to abandon
		pHuman->__SetPrescrLearnedFlag(PrescrID, LearnOrAbandon);

		GCPrescription Msg;
		Msg.setPrescription(PrescrID);
		Msg.setLearnOrAbandon(LearnOrAbandon);

		pHuman->GetPlayer()->SendPacket( &Msg );
		return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-10-17
	*     函数名称 : LuaFnIsPrescrLearned
	*     参数说明 : 
	*     功能说明 :
	*				判断是否已经学会某项配方
	*/
	INT LuaFnIsPrescrLearned(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		PrescriptionID_t PrescrID;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		PrescrID		=	Lua_ValueToNumber(L,3);
		BOOL 			bFlag;

		BEGINHUMANDEFINE("LuaFnIsPrescrLearned")

		bFlag = pHuman->__IsPrescrHaveLearned(PrescrID);
		Lua_PushNumber(L,bFlag);
		return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetAbilityObjIndex
	*     参数说明 : 
	*     功能说明 :
	*
	*/
	INT LuaFnGetAbilityObjIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT Index ;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		Index			=	Lua_ValueToNumber(L,3);
		INT				ObjIndex;

		BEGINHUMANDEFINE("LuaFnGetAbilityObjIndex")
		if( Index >= MAX_ABILITY_ITEM )
		{
			RETURNFALSE
		}
		ObjIndex = pHuman->GetAbilityOpera()->m_BagPos[Index];
		Lua_PushNumber(L, ObjIndex);
		return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetItemBoxRecycleTimes
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*				调用该函数场景ID,short
	*				ItemBoxID
	*					Obj_ItemBox 编号
	*				RecycleTimes
	*					回收次数
	*				
	*				获得itemBox回收次数
	*/
	INT		LuaFnSetItemBoxRecycleTimes(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t ItemBoxID;
		INT RecycleTimes;

		sceneId			=	Lua_ValueToNumber(L,1);
		ItemBoxID		=	Lua_ValueToNumber(L,2);	
		RecycleTimes    =	Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnSetItemBoxRecycleTimes")
			Obj_ItemBox* pItemBox = pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
			Assert(pItemBox);
			pItemBox->SetRecycleTimes(RecycleTimes);
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetItemBoxRecycleTimes
	*     参数说明 : 
	*     功能说明 :
	*			   SceneID	
	*			   调用该函数场景ID
	*				ItemBoxID
	*					Obj_ItemBox 编号
	*			   返回
	*				RecycleTimes
	*					回收次数
	*
	*				设置Obj_ItemBox回收次数
	*/
	INT		LuaFnGetItemBoxRecycleTimes(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			ItemBoxID		=	Lua_ValueToNumber(L,2);	
		INT				RecycleTimes;

		INT				Result	=	0;
		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);
		Obj_ItemBox*	pItemBox	=	pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
		if(pItemBox)
		{
			RecycleTimes = pItemBox->GetRecycleTimes();
			Lua_PushNumber(L,RecycleTimes);
			return 1;
		}
		else
		{
			RETURNFALSE
		}

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetItemBoxOwner
	*     参数说明 : 
	*     功能说明 :
	*			   SceneID	
	*			       调用该函数场景ID
	*				ItemBoxID
	*					Obj_ItemBox 编号
	*				OwnerID
	*					所有者ID			
	*				设置Obj_ItemBox所有者
	*/
	INT		LuaFnSetItemBoxOwner(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			ItemBoxID		=	Lua_ValueToNumber(L,2);	
		GUID_t			OwnerID			=	Lua_ValueToNumber(L,3);

		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);
		Obj_ItemBox*	pItemBox	=	pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
		Assert(pItemBox);
		pItemBox->SetOwner(OwnerID);

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetItemBoxOpenFlag
	*     参数说明 : 
	*     功能说明 :
	*			   SceneID	
	*			       调用该函数场景ID
	*				ItemBoxID
	*					Obj_ItemBox 编号
	*/
	INT		LuaFnGetItemBoxOpenFlag(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			ItemBoxID		=	Lua_ValueToNumber(L,2);	
		BOOL			bFlag;

		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);
		Obj_ItemBox*	pItemBox	=	pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
		if(pItemBox)
		{
			bFlag	= pItemBox->GetOpenFlag();
			Lua_PushNumber(L,bFlag);
			return 1;
		}
		else
		{
			Lua_PushNumber(L,0);
			return 1;
		}
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetItemBoxOwner
	*     参数说明 : 
	*     功能说明 :
	*			   SceneID	
	*			       调用该函数场景ID
	*				ItemBoxID
	*					Obj_ItemBox 编号
	*			返回
	*				OwnerID
	*					所有者ID	
	*
	*				获得Obj_ItemBox所有者
	*/
	INT		LuaFnGetItemBoxOwner(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			ItemBoxID		=	Lua_ValueToNumber(L,2);	
		GUID_t			OwnerID;

		INT				Result	=	0;
		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);
		Obj_ItemBox*	pItemBox	=	pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
		if(pItemBox)
		{
			OwnerID = pItemBox->GetOwner();
			Lua_PushNumber(L,OwnerID);
			return 1;
		}
		else
		{
			RETURNFALSE
		}
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2006-1-20
	*     函数名称 : LuaFnAddGPOwner
	*     参数说明 : 
	*     功能说明 :
	*			   SceneID	
	*			       调用该函数场景ID
	*			   OwnerID
	*					所有者ID	
	*
	*				
	*/
	INT		LuaFnAddGPOwner(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			OwnerID			=	Lua_ValueToNumber(L,2);	
		
		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);

		GrowPointManager* pGPManager = 	pScene->GetGrowPointManager();
		Assert(pGPManager);

		pGPManager->AddGrowPointOwner(OwnerID);
		

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/****************************************************************************
	 *	开发人员：	胡繁
	 *	编写时间：	2006-5-17	10:56
	 *	函数名称：	LuaFnSetItemCreator
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				nBagPos
	 *					物品在背包中的位置
	 *				szCreatorName
	 *					创建者名称
	 *
	 *	功能说明：	给背包里的某个物品增加创建者名称
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnSetItemCreator( Lua_State* L )
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nBagPos;
		const CHAR* szCreatorName;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		nBagPos = Lua_ValueToNumber(L, 3);
		szCreatorName = Lua_ValueToString(L, 4);

		BEGINHUMANDEFINE("LuaFnSetItemCreator")
			Assert( szCreatorName );
			ItemContainer* pContainer = HumanItemLogic::GetBagContainer(pHuman, nBagPos);
			if (pContainer == NULL)
			{
				return 0;
			}
			INT nIndex = pContainer->BagIndex2ConIndex( nBagPos );
			Item* pItem = pContainer->GetItem(nIndex);
			if ( pItem->IsEmpty() != TRUE )
			{
				// 加上制造者名称
				g_ItemOperator.SetItemCreator( pContainer, nIndex, szCreatorName );

				GCNotifyEquip Msg;
				Msg.SetBagIndex( nBagPos );
				pItem->SaveValueTo(Msg.GetItem());
				pHuman->GetPlayer()->SendPacket(&Msg);
			}

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2006-1-20
	*     函数名称 : LuaFnDelGPOwner
	*     参数说明 : 
	*     功能说明 :
	*			   SceneID	
	*			       调用该函数场景ID
	*			   OwnerID
	*					所有者ID	
	*
	*				
	*/
	INT		LuaFnDelGPOwner(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			OwnerID			=	Lua_ValueToNumber(L,2);	

		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);

		GrowPointManager* pGPManager = 	pScene->GetGrowPointManager();
		Assert(pGPManager);

		pGPManager->DelGrowPointOwner(OwnerID);


		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2006-1-20
	*     函数名称 : LuaFnGetGPCountByOwner
	*     参数说明 : 
	*     功能说明 :
	*			   SceneID	
	*			       调用该函数场景ID
	*			   OwnerID
	*					所有者ID	
	*			  返回对应OwnerID 所有的gpCount
	*				
	*/
	INT		LuaFnGetGPCountByOwner(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			OwnerID			=	Lua_ValueToNumber(L,2);	

		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);

		GrowPointManager* pGPManager = 	pScene->GetGrowPointManager();
		if(pGPManager)
		{
			INT Count = 	pGPManager->GetGrowPointCountByOwner(OwnerID);
			Lua_PushNumber(L,Count);
			return 1;
		}
		else
		{
			Lua_PushNumber(L,0);
			return 1;
		}

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetItemBoxWorldPosX
	*     参数说明 : 
	*     功能说明 :
	*			   SceneID	
	*			       调用该函数场景ID
	*				ItemBoxID
	*					Obj_ItemBox 编号
	*			返回
	*				fX
	*					Obj_ItemBox世界坐标X
	*
	*				获得Obj_ItemBox世界坐标X
	*/
	INT		LuaFnGetItemBoxWorldPosX(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			ItemBoxID		=	Lua_ValueToNumber(L,2);	
		FLOAT			fX;

		INT				Result	=	0;
		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);
		Obj_ItemBox*	pItemBox	=	pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
		if(pItemBox)
		{
			fX = pItemBox->getWorldPos()->m_fX;
			Lua_PushNumber(L,fX);
			return 1;
		}
		else
		{
			Lua_PushNumber(L,0);
			return 1;
		}
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetItemBoxWorldPosZ
	*     参数说明 : 
	*     功能说明 :
	*			   SceneID	
	*			       调用该函数场景ID
	*				ItemBoxID
	*					Obj_ItemBox 编号
	*			返回
	*				fZ
	*					Obj_ItemBox世界坐标Z
	*				获得Obj_ItemBox世界坐标Z
	*/
	INT		LuaFnGetItemBoxWorldPosZ(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			ItemBoxID		=	Lua_ValueToNumber(L,2);	
		FLOAT			fZ;

		INT				Result	=	0;
		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);
		Obj_ItemBox*	pItemBox	=	pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
		if(pItemBox)
		{
			fZ = pItemBox->getWorldPos()->m_fZ;
			Lua_PushNumber(L,fZ);
			return 1;
		}
		else
		{
			Lua_PushNumber(L,0);
			return 1;
		}
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetItemBoxMaxGrowTime
	*     参数说明 : 
	*     功能说明 :
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				ItemBoxID
	*					Obj_ItemBox编号
	*			返回
	*				maxGrowTime
	*					Obj_ItemBox最大生长时间
	*
	*					获得Obj_ItemBox最大生长时间
	*/
	INT		LuaFnGetItemBoxMaxGrowTime(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			ItemBoxID		=	Lua_ValueToNumber(L,2);	
		UINT			maxGrowTime;

		INT				Result	=	0;
		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);
		Obj_ItemBox*	pItemBox	=	pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
		if(pItemBox)
		{
			maxGrowTime = pItemBox->GetMaxGrowTime();
			Lua_PushNumber(L,maxGrowTime);
			return 1;
		}
		else
		{
			Lua_PushNumber(L,0);
			return 1;
		}
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetItemBoxMaxGrowTime
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				ItemBoxID
	*					Obj_ItemBox编号
	*				maxGrowTime
	*					Obj_ItemBox最大生长时间
	*				设置Obj_ItemBox最大生长时间
	*/
	INT		LuaFnSetItemBoxMaxGrowTime(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			ItemBoxID		=	Lua_ValueToNumber(L,2);	
		UINT			maxGrowTime    =	Lua_ValueToNumber(L,3);

		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);
		Obj_ItemBox*	pItemBox	=	pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
		Assert(pItemBox);
		pItemBox->SetMaxGrowTime(maxGrowTime);

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetItemBoxPickOwnerTime
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				ItemBoxID
	*					Obj_ItemBox编号
	*				PickProtTime
	*					设置保护时间长度
	*				设置Obj_ItemBox所有者保护时间
	*/
	INT		LuaFnSetItemBoxPickOwnerTime(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			ItemBoxID		=	Lua_ValueToNumber(L,2);	
		UINT			PickProtTime	=	Lua_ValueToNumber(L,3);

		INT				Result	=	0;
		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);
		Obj_ItemBox*	pItemBox	=	pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
		Assert(pItemBox);

		pItemBox->SetPickOwnerTime(PickProtTime);
		
		return 0;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnEnableItemBoxPickOwnerTime
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				ItemBoxID
	*					Obj_ItemBox编号
	*				保护时间开始计时
	*/
	INT		LuaFnEnableItemBoxPickOwnerTime(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			ItemBoxID		=	Lua_ValueToNumber(L,2);	
		INT				Result	=	0;
		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);
		Obj_ItemBox*	pItemBox	=	pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
		Assert(pItemBox);

		pItemBox->EnablePickOwnerTime();

		return 0;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnCreateMonsterOnScene
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				MonsterType
	*					怪物类型
	*				fx
	*					位置x
	*				fz
	*					位置z
	*				AIType
	*					怪物AI类型
	*				AIScriptID
	*					怪物AIScriptID
	*
	*				在场景创建怪物 SceneID,MonsterType,x,z,AIType
	*/
	INT	  LuaFnCreateMonsterOnScene(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		AssertEx( FALSE, "此接口已经废弃，请使用“LuaFnCreateMonster”" ) ;
		return 0 ;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : jack
	*     编写时间 : 2005-11-24
	*     函数名称 : LuaFnCreateMonster
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				MonsterType
	*					怪物类型
	*				fx
	*					位置x
	*				fz
	*					位置z
	*				AIType
	*					怪物AI类型
	*				AIScriptID
	*					怪物AIScriptID
	*				ScriptID
	*					事件脚本ID
	*
	*				在场景创建怪物 SceneID,MonsterType,x,z,AIType
	*/
	INT	  LuaFnCreateMonster(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		//怪物生成所在的场景ID
		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		//怪物类型
		INT				MonsterType		=	Lua_ValueToNumber(L,2);	
		WORLD_POS pos;
		//生成的位置
		pos.m_fX						=	Lua_ValueToNumber(L,3);
		pos.m_fZ						=	Lua_ValueToNumber(L,4);

		INT				iAIType			=	Lua_ValueToNumber(L,5);	
		INT				iAIScript		=	Lua_ValueToNumber(L,6);	
		INT				ScriptID		=	Lua_ValueToNumber(L,7);	

		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);
		
		_OBJ_MONSTER_INIT	init;

		init.m_uDataID			=	MonsterType ;
		init.m_Pos.m_fX			=	pos.m_fX ;
		init.m_Pos.m_fZ			=	pos.m_fZ ;
		init.m_RespawnTime		=	-1;
		init.m_BaseAI			=	iAIType ;
		init.m_ExtAIScript		=   iAIScript;
		init.m_idScript			=	ScriptID ;

		ObjID_t ObjID = pScene->CreateTempMonster((const _OBJ_MONSTER_INIT*)&init) ;

		Lua_PushNumber(L,ObjID);
		return 1;


		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : jack
	*     编写时间 : 2005-11-24
	*     函数名称 : LuaFnDeleteMonster
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				ObjID
	*					怪物号
	*				
	*				删除动态创建出来的怪(或NPC)，注意：场景里摆的怪不能被删除
	*/
	INT	  LuaFnDeleteMonster(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		INT				ObjID			=	Lua_ValueToNumber(L,2);	

		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);

		Obj* pObj = pScene->GetObjManager()->GetObj(ObjID) ;
		if( pObj ==NULL )
			return 0 ;

		if( pObj->GetObjType() != Obj::OBJ_TYPE_MONSTER )
			return 0 ;

		Obj_Monster* pMonster = (Obj_Monster*)pObj ;
        		
		BOOL ret = pScene->DeleteTempMonster(pMonster);
		
		if(ret)
		{
			Lua_PushNumber(L,TRUE);
			return 1;
		}

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddStorePointOnScene
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*						场景编号
	*				StorePointType
	*						藏宝点类型
	*				fx
	*						藏宝点x
	*				fz		
	*						藏宝点z
	*				
	*				在场景里添加一个藏宝点
	*/
	INT	LuaFnAddStorePointOnScene(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		INT				StorePointType	=	Lua_ValueToNumber(L,2);	
		WORLD_POS		pos;
		pos.m_fX						=	Lua_ValueToNumber(L,3);	
		pos.m_fZ						=	Lua_ValueToNumber(L,4);	

		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);

		STORE_POINT	point;
		point.m_Pos		=	pos;
		point.m_Type	=	StorePointType;
		pScene->GetStoreManager()->AddStorePoint(&point);

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : DelStorePointOnScene
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*						场景编号
	*				StorePointType
	*						藏宝点类型
	*				fx
	*						藏宝点x
	*				fz		
	*						藏宝点z
	*				
	*				在场景里删除一个藏宝点
	*/
	INT	LuaFnDelStorePointOnScene(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT StorePointType;
		INT index;

		sceneId			=	Lua_ValueToNumber(L,1);
		StorePointType	=	Lua_ValueToNumber(L,2);	
		index			=	Lua_ValueToNumber(L,3);	

		BEGINSCENEDEFINE("LuaFnCheckTimer")

		pScene->GetStoreManager()->DelStorePoint(index);

		WORLD_POS pos = pScene->GetStoreManager()->GetStorePointPos(index);

		pScene->GetGrowPointManager()->DecGrowPointTypeCount(StorePointType,pos.m_fX,pos.m_fZ);
		
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnFindStorePointOnScene
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*						场景编号
	*				StorePointType
	*						藏宝点类型
	*				fx
	*						藏宝点x
	*				fz		
	*						藏宝点z
	*				
	*				在场景里查找一个藏宝点
	*/
	INT	LuaFnFindStorePointOnScene(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT StorePointType;

		sceneId			=	Lua_ValueToNumber(L,1);
		StorePointType	=	Lua_ValueToNumber(L,2);	
		WORLD_POS		pos;
		pos.m_fX						=	Lua_ValueToNumber(L,3);	
		pos.m_fZ						=	Lua_ValueToNumber(L,4);	

		BEGINSCENEDEFINE("LuaFnFindStorePointOnScene")

		STORE_POINT	point;
		point.m_Pos		=	pos;
		point.m_Type	=	StorePointType;

		INT	index;

		pScene->GetStoreManager()->FindStorePointByPos(pos,index);
		
		Lua_PushNumber(L,index);
		return 1;
		ENDSCENEDEFINE
		

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetStorePointType
	*     参数说明 : 
	*				SceneID
	*						场景编号
	*				StorePointType
	*						藏宝点类型
	*				fx
	*						藏宝点x
	*				fz		
	*						藏宝点z
	*	
	*     功能说明 :				
	*				在场景里获得一个藏宝点类型
	*/
	INT	LuaFnGetStorePointType(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT index;

		sceneId			=	Lua_ValueToNumber(L,1);
		index			=	Lua_ValueToNumber(L,2);	
		
		BEGINSCENEDEFINE("LuaFnGetStorePointType")
		INT GrowPointType = pScene->GetStoreManager()->GetStorePointType(index);
		Lua_PushNumber(L,GrowPointType);
		return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnTryRecieveItem
	*     参数说明 : 
	*				SceneID
	*						场景编号
	*				HumanID
	*						人物编号
    *				ItemType
	*						物品9位标示号
	*				Quality
	*						蓝色装备品质段,其他装备随便添一个正数
	*					
	*	
	*     功能说明 :
	*				尝试接受物品
	*/
	INT	LuaFnTryRecieveItem(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t selfId;
		INT iItemId;
		INT iQuality;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		iItemId			=	Lua_ValueToNumber(L,3);	
		iQuality		=	Lua_ValueToNumber(L,4);	
		
		Assert(iItemId>0);

		BEGINHUMANDEFINE("LuaFnTryRecieveItem")

		Assert(iQuality>INVALID_ID);
		TSerialHelper	help(iItemId);

		_ITEM_TYPE		itemType = help.GetItemTypeStruct();

		uint iBagIndex=INVALID_INDEX;
		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType		= ITEM_CREATE_FROM_ABILITY_SCRIPT;
		ItemLogParam.CharGUID	= pHuman->GetGUID();
		ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
		ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
		ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
	
		_MY_TRY
		{
			HumanItemLogic::CreateItemToBag(&ItemLogParam,pHuman,itemType.ToSerial(),iBagIndex);

			if(iBagIndex==INVALID_INDEX) //创建失败
			{
				Lua_PushNumber(L,INVALID_INDEX);
				return 1;
			}

			SaveItemLog(&ItemLogParam);
			Item*	pDestItem = HumanItemLogic::GetBagItem(pHuman,iBagIndex);
			Assert(pDestItem);
			GCNotifyEquip	msg;
			msg.SetBagIndex(iBagIndex);
			pDestItem->SaveValueTo(msg.GetItem());
			pHuman->GetPlayer()->SendPacket(&msg);
		}
		_MY_CATCH
		{
			SaveCodeLog( ) ;
		}

		Lua_PushNumber(L,iBagIndex);
		return 1;

		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetHumanWorldX
	*     参数说明 : 
	*     功能说明 :
	*				获得Obj_Human的世界坐标X 参数SceneID,HumanID,返回WorldX
	*/
	INT	LuaFnGetHumanWorldX(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		
		FLOAT			WorldX;

		BEGINHUMANDEFINE("LuaFnGetHumanWorldX")
		WorldX	=	pHuman->getWorldPos()->m_fX;

		Lua_PushNumber(L,WorldX);
		return	1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetHumanWorldZ
	*     参数说明 : 
	*     功能说明 :
	*				获得Obj_Human的世界坐标Z 参数SceneID,HumanID,返回WorldZ
	*/
	INT	LuaFnGetHumanWorldZ(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);

		FLOAT			WorldZ;

		BEGINHUMANDEFINE("LuaFnGetHumanWorldZ")
		WorldZ	=	pHuman->getWorldPos()->m_fZ;
		Lua_PushNumber(L,WorldZ);
		return	1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetStoreMapWorldX
	*     参数说明 : 

	*				SceneID
	*					场景编号
	*				HumanID
	*					人物编号
	*				BagIndex
	*					背包位置
	*				fx
	*					藏宝图x
	*     功能说明 :
	*			获得藏宝图位置X
	*/
	INT	LuaFnGetStoreMapX(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		uint bagIndex;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		bagIndex		=	Lua_ValueToNumber(L,3);

		FLOAT			MapX;

		BEGINHUMANDEFINE("LuaFnGetStoreMapX")
		
		Item*	pStoreMapItem = 	HumanItemLogic::GetBagItem(pHuman,bagIndex);
		Assert(pStoreMapItem);

		if(pStoreMapItem->GetItemClass()==ICLASS_STOREMAP)
		{
			MapX	=	pStoreMapItem->GetXPos();
			Lua_PushNumber(L,MapX);
			return	1;
		}
		else
		{
			AssertEx(FALSE,"此物品不是藏宝图");
			RETURNFALSE
		}
		ENDHUMANDEFINE


		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetStoreMapWorldZ
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				HumanID
	*					人物编号
	*				BagIndex
	*					背包位置
	*				fz
	*					藏宝图z
	*			获得藏宝图位置Z
	*/
	INT	LuaFnGetStoreMapZ(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		uint bagIndex;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		bagIndex		=	Lua_ValueToNumber(L,3);
		FLOAT			MapZ;

		BEGINHUMANDEFINE("LuaFnGetStoreMapZ")

		Item*	pStoreMapItem = 	HumanItemLogic::GetBagItem(pHuman,bagIndex);
		Assert(pStoreMapItem);

		if(pStoreMapItem->GetItemClass()==ICLASS_STOREMAP)
		{
			MapZ	=	pStoreMapItem->GetZPos();
			Lua_PushNumber(L,MapZ);
			return	1;
		}
		else
		{
			AssertEx(FALSE,"此物品不是藏宝图");
			RETURNFALSE
		}
		ENDHUMANDEFINE


		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetStoreMapSceneID
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				HumanID
	*					人物编号
	*				BagIndex
	*					背包位置
	*				MapSceneID
	*					藏宝图场景
	*     功能说明 :
	*			获得藏宝图场景ID
	*/
	INT	LuaFnGetStoreMapSceneID(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		uint bagIndex;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		bagIndex		=	Lua_ValueToNumber(L,3);

		SceneID_t		MapSceneID;

		BEGINHUMANDEFINE("LuaFnGetStoreMapSceneID")

		Item*	pStoreMapItem = 	HumanItemLogic::GetBagItem(pHuman,bagIndex);
		Assert(pStoreMapItem);

		if(pStoreMapItem->GetItemClass()==ICLASS_STOREMAP)
		{
			MapSceneID	=	pStoreMapItem->GetSceneID();
			Lua_PushNumber(L,MapSceneID);
			return	1;
		}
		else
		{
			AssertEx(FALSE,"此物品不是藏宝图");
			RETURNFALSE
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	*	开发人员：	胡繁
	*	编写时间：	2005-11-1	10:15
	*	函数名称：	LuaFnQueryAreaStandingTime
	*	参数说明：
	*				sceneId
	*					场景编号
	*				selfId
	*					调用者 ID，无符号整型
	*
	*	功能说明：	查询玩家在事件区域停留的时间
	*	修改记录：
	*****************************************************************************/
	INT LuaFnQueryAreaStandingTime(Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId			=	Lua_ValueToNumber(L, 1);
		selfId			=	Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnQueryAreaStandingTime")

		UINT uStandingTime = pHuman->NowTime();
		uStandingTime -= pHuman->GetAreaTimeStamp();

		Lua_PushNumber(L, uStandingTime);
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

	LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	*	开发人员：	胡繁
	*	编写时间：	2005-11-1	11:02
	*	函数名称：	LuaFnResetAreaStandingTime
	*	参数说明：
	*				sceneId
	*					场景编号
	*				selfId
	*					调用者 ID，无符号整型
	*				nTimeStampOffset
	*					新的时间戳偏移量（毫秒）
	*
	*	功能说明：	重新设置玩家进入事件区域的时间（作为下一次触发事件的时间戳）
	*	修改记录：
	*****************************************************************************/
	INT LuaFnResetAreaStandingTime(Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nTimeStampOffset;

		sceneId			=	Lua_ValueToNumber(L, 1);
		selfId			=	Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("ResetAreaStandingTime")

		UINT uNewTimeStamp;
		uNewTimeStamp = pHuman->NowTime() + nTimeStampOffset;
		pHuman->ResetAreaTimeStamp(uNewTimeStamp);

		return 0;
		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/****************************************************************************
	 *	开发人员：	胡繁
	 *	编写时间：	2006-4-10	15:06
	 *	函数名称：	LuaFnSendAbilitySuccessMsg
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				AbilityId
	 *					生活技能 ID 号
	 *				RecipeId
	 *					配方 ID 号
	 *				ItemSerialNum
	 *					物品序列号
	 *
	 *	功能说明：	向客户端发送物品制造成功的消息
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnSendAbilitySuccessMsg(Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		INT AbilityId;
		INT RecipeId;
		UINT ItemSerialNum;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		AbilityId = Lua_ValueToNumber(L, 3);
		RecipeId = Lua_ValueToNumber(L, 4);
		ItemSerialNum = Lua_ValueToNumber(L, 5);

		BEGINHUMANDEFINE("LuaFnSendAbilitySuccessMsg")

			GCAbilitySucc Msg;
			Msg.SetAbilityID( AbilityId );
			Msg.SetPrescriptionID( RecipeId );
			Msg.SetItemSerial( ItemSerialNum );
			pHuman->GetPlayer()->SendPacket( &Msg );

		return 0;
		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION
		return 0 ;
	}

}
//add by gh 2010/07/22
/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2006-6-7
	*     函数名称 : LuaFnIsEquipItem
	*     参数说明 : 
	*				UINT			ItemTableIndex
	*				
	*     功能说明 :
	*		是否是装备
	*/
	INT LuaFnIsEquipItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		//Steven.Han start 2006-9-18 16:16
		UINT ItemTableIndex;
		//Steven.Han end   2006-9-18 16:16

		//CHECKPARAM_RET1(1)
		if(FALSE==LuaVerifyAllNumberParam(L, 1, 1, "LuaFnIsEquipItem"))
		{
			RETURNFALSE;
		}
		ItemTableIndex = Lua_ValueToNumber(L,1);

		Assert(ItemTableIndex>0);

		TSerialHelper	help(ItemTableIndex);

		_ITEM_TYPE it = help.GetItemTypeStruct();

		if (ICLASS_EQUIP==it.m_Class)
		{
			Lua_PushNumber(L, 1);
		}
		else
		{
			Lua_PushNumber(L, 0);
		}

		return 1;

		LUA_LEAVE_FUNCTION
		RETURNFALSE
		
	}

	/****************************************************************************
	*	开发人员：	
	*	编写时间：	2006-8-17	14:00
	*	函数名称：	LuaFnGetItemEquipPoint
	*	参数说明：
	*
	*	功能说明：	查找装备的装配点
	*	修改记录：
	*****************************************************************************/
	INT LuaFnGetItemEquipPoint(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		//Steven.Han start 2006-9-18 16:16
		UINT ItemIndex;
		//Steven.Han end   2006-9-18 16:16

		//CHECKPARAM_RET1(1)

		if(FALSE==LuaVerifyAllNumberParam(L, 1, 1, "LuaFnGetItemEquipPoint"))
		{
				RETURNFALSE;
		}

		ItemIndex = Lua_ValueToNumber(L, 1);
		
		BYTE	bClass =	GetSerialClass(ItemIndex);
		
		switch(bClass)
		{
		case ICLASS_EQUIP:
			{
				EQUIP_TB* pGET	=	g_ItemTable.GetEquipTB(ItemIndex);
				Assert(pGET);
				Lua_PushNumber(L,pGET->m_EquipPoint);

				return 1;
			}
			break;		
		default:
			{
				RETURNFALSE;
			}
		}
		
		return 1;

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}


/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2006-6-7
	*     函数名称 : LuaFnGeEquipReqLevel
	*     参数说明 : 
	*
	*				INT				ItemTableIndex
	*				
	*     功能说明 :
	*		返回物品级别
	*/
	INT LuaFnGeEquipReqLevel(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		//Steven.Han start 2006-9-18 16:16
		INT ItemTableIndex;
		//Steven.Han end   2006-9-18 16:16

		//CHECKPARAM_RET1(3)

			if(FALSE==LuaVerifyAllNumberParam(L, 1, 1, "LuaFnGeEquipReqLevel"))
			{
				RETURNFALSE;
			}

			ItemTableIndex = Lua_ValueToNumber(L,1);
			Assert(ItemTableIndex>0);

			TSerialHelper	help(ItemTableIndex);

			_ITEM_TYPE it = help.GetItemTypeStruct();

			switch(it.m_Class)
			{
			case ICLASS_EQUIP:
				{
					EQUIP_TB*	pTB = 	g_ItemTable.GetEquipTB(ItemTableIndex);
						if(pTB)
						{
							Lua_PushNumber(L,(INT)(pTB->m_ReqLevel));
							return 1;
						}
				}
				break;
			default:
				RETURNFALSE
			}

			LUA_LEAVE_FUNCTION
			RETURNFALSE

	}
/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-12-21
	 *	函数名称：	LuaFnGetAbilityLevelUpConfig
	 *	参数说明：
	 *				abilityId
	 *					生活技能ID
	 *				level
	 *					所要取得的级别
	 *
	 *	功能说明：	得生活技能升级所对应的数据
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnGetAbilityLevelUpConfig( Lua_State* L )
	{
		LUA_ENTER_FUNCTION
		//Steven.Han start 2006-9-18 16:16
		AbilityID_t abilityId;
		INT level;
		//Steven.Han end   2006-9-18 16:16

		//CHECKPARAM_RET1(2)

		if(FALSE==LuaVerifyAllNumberParam(L, 1, 2, "LuaFnGetAbilityLevelUpConfig"))
		{
			RETURNFALSE;
		}

		abilityId = Lua_ValueToNumber(L, 1);
		level = Lua_ValueToNumber(L, 2);

		const AbilityLevelUpConfigManager *pConfigMgr = g_pAbilityManager->GetLevelUpConfigManager();
		Assert(pConfigMgr);
		Ability *pAbility = g_pAbilityManager->GetAbility(abilityId);
		Assert(pAbility);
		if(NULL != pConfigMgr && NULL != pAbility)
		{
			const AbilityLevelUpConfig *pLevelUpConfig = pConfigMgr->GetLevelUpConfig(pAbility->LevelUpConfigIndex());
			if(NULL != pLevelUpConfig)
			{
				const AbilityLevelUpInfo *pLevelUpInfo = pLevelUpConfig->GetLevelUpInfo(level);
				if(NULL != pLevelUpInfo)
				{
					Lua_PushNumber(L, TRUE);
					Lua_PushNumber(L, pLevelUpInfo->GetDemandMoney());
					Lua_PushNumber(L, pLevelUpInfo->GetDemandExp());
					Lua_PushNumber(L, pLevelUpInfo->GetLimitAbilityExp());
					Lua_PushNumber(L, pLevelUpInfo->GetLimitAbilityExpShow());
					Lua_PushNumber(L, pLevelUpInfo->GetCurrentLevelAbilityExpTop());
					Lua_PushNumber(L, pLevelUpInfo->GetLimitLevel());
					return 7;
				}
			}
		}
		Lua_PushNumber(L, FALSE);
		return 1;

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	*	开发人员：	
	*	编写时间：	2006-12-21
	*	函数名称：	LuaFnGetAbilityLevelUpConfig2
	*	参数说明：
	*				abilityId
	*					生活技能ID
	*				level
	*					所要取得的级别
	*
	*	功能说明：	得生活技能升级所对应的数据
	*	修改记录：
	*****************************************************************************/
	INT LuaFnGetAbilityLevelUpConfig2( Lua_State* L )
	{
		LUA_ENTER_FUNCTION
		//Steven.Han start 2006-9-18 16:16
		AbilityID_t abilityId;
		INT level;
		//Steven.Han end   2006-9-18 16:16

		//CHECKPARAM_RET1(2)

			if(FALSE==LuaVerifyAllNumberParam(L, 1, 2, "LuaFnGetAbilityLevelUpConfig2"))
			{
				RETURNFALSE;
			}

			abilityId = Lua_ValueToNumber(L, 1);
			level = Lua_ValueToNumber(L, 2);

			const AbilityLevelUpConfigManager *pConfigMgr = g_pAbilityManager->GetLevelUpConfigManager();
			Assert(pConfigMgr);
			Ability *pAbility = g_pAbilityManager->GetAbility(abilityId);
			Assert(pAbility);
			if(NULL != pConfigMgr && NULL != pAbility)
			{
				const AbilityLevelUpConfig *pLevelUpConfig = pConfigMgr->GetLevelUpConfig(pAbility->LevelUpConfigIndex());
				if(NULL != pLevelUpConfig)
				{
					const AbilityLevelUpInfo *pLevelUpInfo = pLevelUpConfig->GetLevelUpInfo(level);
					if(NULL != pLevelUpInfo)
					{
						Lua_PushNumber(L, TRUE);
						Lua_PushNumber(L, pLevelUpInfo->GetDemandMoney());
						Lua_PushNumber(L, pLevelUpInfo->GetDemandExp());
						Lua_PushNumber(L, pLevelUpInfo->GetLimitAbilityExp());
						Lua_PushNumber(L, pLevelUpInfo->GetLimitAbilityExpShow());
						Lua_PushNumber(L, pLevelUpInfo->GetCurrentLevelAbilityExpTop());
						Lua_PushNumber(L, pLevelUpInfo->GetLimitLevel());
						Lua_PushNumber(L, pLevelUpInfo->GetExtraNeedMoney());
						Lua_PushNumber(L, pLevelUpInfo->GetExtraExp());
						return 9;
					}
				}
			}
			Lua_PushNumber(L, FALSE);
			return 1;

			LUA_LEAVE_FUNCTION
			RETURNFALSE
	}



#endif
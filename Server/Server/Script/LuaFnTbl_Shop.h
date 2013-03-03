#ifndef __LUAFNTBL_SHOP_H__
#define __LUAFNTBL_SHOP_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "GCShopMerchandiseList.h"
#include "GCShopSoldList.h"
#include "ShopManager.h"
#include "GCMenpaiInfo.h"
#include "Obj_Monster.h"
#include "ShopManager.h"
#include "GCXinfaStudyInfo.h"
#include "GCXinfaStudyInfo.h"
#include "GCAbilityTeacherInfo.h"
#include "GCPlayerShopError.h"
#include "GCPlayerShopApply.h"
#include "GCPlayerShopAcquireShopList.h"
#include "GCItemList.h"
#include "GCDetailAttrib_Pet.h"
#include "GameTable.h"
#include "RecordOpt.h"
#include "CGPlayerShopAcquireItemList.h"
#include "GCPlayerShopUpdatePartners.h"
using namespace ITEM_SYNCH_STRUCT;

namespace LuaFnTbl
{
	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-10-12
	*     函数名称 : LuaFnDispatchShopItem
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				ShopId
	*					商店编号
	*				
	*/
	INT	LuaFnDispatchShopItem(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;
		ObjID_t ShopId;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		targetId		=	Lua_ValueToNumber(L,3);
		ShopId			=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnDispatchShopItem")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(targetId));
			Assert(pObj);
			if(pObj && IsCharacterObj( pObj->GetObjType() ))
			{
				Obj_Monster* pNpc = (Obj_Monster*)pObj;
				Assert(pNpc);
				if(pNpc)
				{//	17:03 2005-10-12 先去写个商店管理器

					_SHOP* pShop =	NULL;

					//先查自己身上挂没挂商店
					if(pNpc->GetShopManager() == NULL)
					{//没有，直接在静态表中查找这个商店
                        pShop	=	g_pStaticShopManager->GetShopByID(ShopId);
					}
					else
					{//自己身上有动态表
						//先在自己身上找
						pShop	=	pNpc->GetShopManager()->GetShopByID(ShopId);
						if(!pShop)
						{//没有，回到静态表中找
							 pShop	=	g_pStaticShopManager->GetShopByID(ShopId);
						}
					}

					//还没有，一定是shopid出错了
					if(!pShop)
						Assert(0);


                    //填充消息
                    GCShopMerchandiseList::_MERCHANDISE_ITEM	MerchandiseList[MAX_BOOTH_NUMBER];

                    INT k = 0;
                    for(INT i = 0;i<pShop->m_ItemList->m_ListCount;i++)
                    {
                        MerchandiseList[k].idTable		    =	pShop->m_ItemList->m_ListTypeIndex[i];
                        if(pShop->m_IsDyShop == TRUE)
                            MerchandiseList[k].MaxNumber	=	pShop->m_ItemList->m_TypeMaxNum[i];
                        else
                            MerchandiseList[k].MaxNumber	=	-1;

                        switch (pShop->m_IsDyShop)
                        {
                        case FALSE://静态商店
                            {
                                //价格的计算，
                                uint BaseMoney = ShopMgr::ConvertItemType2Money(pShop->m_ItemList->m_ListType[i]);
                                if(BaseMoney<0) BaseMoney = 0;
                                BaseMoney *= static_cast<INT>(pShop->m_scale);
                                MerchandiseList[k].nPrice		=	BaseMoney;
                            }
                            break;
                        case TRUE://动态商店
                            {
                                MerchandiseList[k].nPrice		=	pShop->m_ItemList->m_Price[i];
                                MerchandiseList[k].nPriceType	=	pShop->m_ItemList->m_PriceType[i];
                            }
                            break;
                        }
                        MerchandiseList[k++].byNumber	=	pShop->m_ItemList->m_TypeCount[i];
                    }

					GCShopMerchandiseList	Msg;
					Msg.SetMerchadiseNum(k);//一定要先设置数量再设置List
					Msg.SetMerchadiseList(MerchandiseList);

					Msg.SetBuyType(pShop->m_nBuyType);
					Msg.SetBuyLevel(pShop->m_nBuyLevel);
					Msg.SetRepairLevel(pShop->m_nRepairLevel);
					Msg.SetRepairType(pShop->m_nRepairType);
					Msg.SetUniqueID(pNpc->GetUniqueID());
					Msg.SetScale(pShop->m_scale);
                    Msg.SetDyShop(pShop->m_IsDyShop);
                    Msg.SetVersion(pShop->m_nVersion);

					//设置商人ID
					Msg.SetObjID(pNpc->GetID());

					//此人开始与商人交易
					pHuman->Shop(pShop);//切记!!!交易完成后一定要把这个清成NULL!!!
					//pShop->Custom(pHuman);//切记!!!交易完成后一定要把这个清成NULL!!!
					Player* pPlayer = pHuman->GetPlayer();
					pPlayer->SendPacket( &Msg ) ;


                    //刷新整个回购列表商品到界面
                    GCShopSoldList::_MERCHANDISE_ITEM	SoldItem[MAX_BOOTH_SOLD_NUMBER];
                    pHuman->GetSlodListInfo(&SoldItem[0]);
                    GCShopSoldList	MsgSold;
                    MsgSold.SetMerchadiseNum(MAX_BOOTH_SOLD_NUMBER);
                    for (INT i=0; i<MAX_BOOTH_SOLD_NUMBER; ++i)
                    {
                        MsgSold.SetMerchadiseData(&SoldItem[i], i);
                    }
                    pHuman->GetPlayer()->SendPacket( &MsgSold );
				}
			}
		ENDHUMANDEFINE
		return	0;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	

	/** -----------------------------------------------------------------------
	*     开发人员 : yu
	*     编写时间 : 2005-10-20
	*     函数名称 : LuaFnDispatchXinfaInfo
	*     参数说明 : 
	*     功能说明 : 门派心法导师使用
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				MenpaiId
	*					这个老师的ID（门派ID）
	*/
	INT LuaFnDispatchXinfaLevelInfo( Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;
		ObjID_t MenpaiId;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		targetId		=	Lua_ValueToNumber(L,3);
		MenpaiId		=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnDispatchXinfaLevelInfo")
		{
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(targetId));
			Assert(pObj);
			//向客户端发送门派心法教师的门派消息
			GCXinfaStudyInfo msg;
			msg.SetMenpaiInfo_ID(MenpaiId);
			msg.SetTeacherId(targetId);

			if(pObj && IsCharacterObj( pObj->GetObjType() ))
			{
				Player* pPlayer = pHuman->GetPlayer();
				pPlayer->SendPacket( &msg ) ;
			}
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : yu
	*     编写时间 : 2005-10-20
	*     函数名称 : LuaFnDispatchMenpaiInfo
	*     参数说明 : 
	*     功能说明 : 门派导师使用
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				MenpaiId
	*					这个老师的ID（门派ID）
	*/				
	INT LuaFnDispatchMenpaiInfo( Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;
		ObjID_t MenpaiId;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		targetId		=	Lua_ValueToNumber(L,3);
		MenpaiId		=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnDispatchMenpaiInfo")
		{
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(targetId));
			Assert(pObj);

			//向客户端发送门派教师的门派消息
			GCMenpaiInfo Msg;
			Msg.SetMenpaiInfo_ID((short)MenpaiId);

			if(pObj && IsCharacterObj( pObj->GetObjType() ))
			{
				Player* pPlayer = pHuman->GetPlayer();
				pPlayer->SendPacket( &Msg ) ;
			}
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : yu
	*     编写时间 : 2005-10-20
	*     函数名称 : LuaFnDispatchAbilityInfo
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				AbilityId
	*					这个老师所教的生活技能的ID
	*/
	INT LuaFnDispatchAbilityInfo( Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t	sceneId;
    ObjID_t	selfId;
		ObjID_t	targetId;
		UINT		scriptId; 
		ObjID_t	AbilityId;
		UINT		nNeedMoney;
		UINT		nNeedExp;
		UINT		nNeedSkillExp;
		INT		nLevelLimit;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		targetId		=	Lua_ValueToNumber(L,3);
		scriptId		=	Lua_ValueToNumber(L,4);
		AbilityId		=	Lua_ValueToNumber(L,5);
		nNeedMoney		=	Lua_ValueToNumber(L,6);
		nNeedExp		=	Lua_ValueToNumber(L,7);
		nNeedSkillExp	=	Lua_ValueToNumber(L,8);
		nLevelLimit		=	Lua_ValueToNumber(L,9);


		BEGINHUMANDEFINE("LuaFnDispatchAbilityInfo")
		{
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(targetId));
			Assert(pObj);

			//向客户端发送教师的生活技能消息
			GCAbilityTeacherInfo Msg;
			GCAbilityTeacherInfo::_AbilityTeacherInfo StudyInfo;
			StudyInfo.m_nAbilityID	= AbilityId;
			StudyInfo.m_uSpendMoney	= nNeedMoney;
			StudyInfo.m_uSpendExp	= nNeedExp;
			StudyInfo.m_uSkillExp	= nNeedSkillExp;
			StudyInfo.m_uTeacherId	= targetId;
			StudyInfo.m_uScriptId	= scriptId;
			StudyInfo.m_nLevelLimit = nLevelLimit;

			Msg.SetAbilityTeacherInfo(StudyInfo);

			if(pObj && IsCharacterObj( pObj->GetObjType() ))
			{
				Player* pPlayer = pHuman->GetPlayer();
				pPlayer->SendPacket( &Msg ) ;
			}
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : yu
	*     编写时间 : 2006-01-09
	*     函数名称 : LuaFnJoinMenpai
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				AbilityId
	*					这个老师所教的生活技能的ID
	*/
	INT LuaFnJoinMenpai( Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t	sceneId;
		ObjID_t	selfId;
	
		ObjID_t	targetId;
		ObjID_t	MenpaiId;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		targetId		=	Lua_ValueToNumber(L,3);
		MenpaiId		=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnJoinMenpai")
		{
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(targetId));
			Assert(pObj);

			//1、更改门派心法技能
			if(pObj && IsCharacterObj( pObj->GetObjType() ))
			{
				//刷新门派心法和技能到客户端
				SKILL_LOG_PARAM SkillLogParam;
				SkillLogParam.CharGUID	=	pHuman->GetGUID();
				SkillLogParam.SceneID	=	pHuman->getScene()->SceneID();
				SkillLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				SkillLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;

				SkillLogParam.OPType	=	SKILL_OP_SCRIPT_CHANGEMENPAI;

				XINFA_LOG_PARAM XinFaLogParam;
				XinFaLogParam.CharGUID	=	pHuman->GetGUID();
				XinFaLogParam.SceneID	=	pHuman->getScene()->SceneID();
				XinFaLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				XinFaLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				XinFaLogParam.OPType	=	XINFA_OP_SCRIPT_CHANGEMENPAI;	

				pHuman->ChangeMenpai(&SkillLogParam,&XinFaLogParam,MenpaiId);

				//刷新人物属性点数到客户端
				pHuman->ChangeMenpaiPoints();
			}
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	
}

#endif
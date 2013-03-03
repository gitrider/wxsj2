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
	*     ������Ա :
	*     ��дʱ�� : 2005-10-12
	*     �������� : LuaFnDispatchShopItem
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				ShopId
	*					�̵���
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
				{//	17:03 2005-10-12 ��ȥд���̵������

					_SHOP* pShop =	NULL;

					//�Ȳ��Լ����Ϲ�û���̵�
					if(pNpc->GetShopManager() == NULL)
					{//û�У�ֱ���ھ�̬���в�������̵�
                        pShop	=	g_pStaticShopManager->GetShopByID(ShopId);
					}
					else
					{//�Լ������ж�̬��
						//�����Լ�������
						pShop	=	pNpc->GetShopManager()->GetShopByID(ShopId);
						if(!pShop)
						{//û�У��ص���̬������
							 pShop	=	g_pStaticShopManager->GetShopByID(ShopId);
						}
					}

					//��û�У�һ����shopid������
					if(!pShop)
						Assert(0);


                    //�����Ϣ
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
                        case FALSE://��̬�̵�
                            {
                                //�۸�ļ��㣬
                                uint BaseMoney = ShopMgr::ConvertItemType2Money(pShop->m_ItemList->m_ListType[i]);
                                if(BaseMoney<0) BaseMoney = 0;
                                BaseMoney *= static_cast<INT>(pShop->m_scale);
                                MerchandiseList[k].nPrice		=	BaseMoney;
                            }
                            break;
                        case TRUE://��̬�̵�
                            {
                                MerchandiseList[k].nPrice		=	pShop->m_ItemList->m_Price[i];
                                MerchandiseList[k].nPriceType	=	pShop->m_ItemList->m_PriceType[i];
                            }
                            break;
                        }
                        MerchandiseList[k++].byNumber	=	pShop->m_ItemList->m_TypeCount[i];
                    }

					GCShopMerchandiseList	Msg;
					Msg.SetMerchadiseNum(k);//һ��Ҫ����������������List
					Msg.SetMerchadiseList(MerchandiseList);

					Msg.SetBuyType(pShop->m_nBuyType);
					Msg.SetBuyLevel(pShop->m_nBuyLevel);
					Msg.SetRepairLevel(pShop->m_nRepairLevel);
					Msg.SetRepairType(pShop->m_nRepairType);
					Msg.SetUniqueID(pNpc->GetUniqueID());
					Msg.SetScale(pShop->m_scale);
                    Msg.SetDyShop(pShop->m_IsDyShop);
                    Msg.SetVersion(pShop->m_nVersion);

					//��������ID
					Msg.SetObjID(pNpc->GetID());

					//���˿�ʼ�����˽���
					pHuman->Shop(pShop);//�м�!!!������ɺ�һ��Ҫ��������NULL!!!
					//pShop->Custom(pHuman);//�м�!!!������ɺ�һ��Ҫ��������NULL!!!
					Player* pPlayer = pHuman->GetPlayer();
					pPlayer->SendPacket( &Msg ) ;


                    //ˢ�������ع��б���Ʒ������
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
	*     ������Ա : yu
	*     ��дʱ�� : 2005-10-20
	*     �������� : LuaFnDispatchXinfaInfo
	*     ����˵�� : 
	*     ����˵�� : �����ķ���ʦʹ��
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				MenpaiId
	*					�����ʦ��ID������ID��
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
			//��ͻ��˷��������ķ���ʦ��������Ϣ
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
	*     ������Ա : yu
	*     ��дʱ�� : 2005-10-20
	*     �������� : LuaFnDispatchMenpaiInfo
	*     ����˵�� : 
	*     ����˵�� : ���ɵ�ʦʹ��
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				MenpaiId
	*					�����ʦ��ID������ID��
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

			//��ͻ��˷������ɽ�ʦ��������Ϣ
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
	*     ������Ա : yu
	*     ��дʱ�� : 2005-10-20
	*     �������� : LuaFnDispatchAbilityInfo
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				AbilityId
	*					�����ʦ���̵�����ܵ�ID
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

			//��ͻ��˷��ͽ�ʦ���������Ϣ
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
	*     ������Ա : yu
	*     ��дʱ�� : 2006-01-09
	*     �������� : LuaFnJoinMenpai
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				AbilityId
	*					�����ʦ���̵�����ܵ�ID
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

			//1�����������ķ�����
			if(pObj && IsCharacterObj( pObj->GetObjType() ))
			{
				//ˢ�������ķ��ͼ��ܵ��ͻ���
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

				//ˢ���������Ե������ͻ���
				pHuman->ChangeMenpaiPoints();
			}
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	
}

#endif
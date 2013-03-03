// AbilityInlay.cpp
#include "stdafx.h"
#include "AbilityInlay.h"
#include "Scene.h"
#include "GCDiscardItemResult.h"
#include "GamePlayer.h"
#include "HumanItemLogic.h"
#include "GCNotifyEquip.h"
#include "GCAbilitySucc.h"
#include "ScriptDef.h"

AbilityInlay::AbilityInlay(const Ability& a) : AbilityCompound(a)
{
}

ORESULT AbilityInlay::CanUseAbility(Obj_Human* pHuman)
{
__ENTER_FUNCTION

	Assert( pHuman );

	AbilityOpera* pAbilityOpera;

	pAbilityOpera = pHuman->GetAbilityOpera();
	Assert( pAbilityOpera );

	ORESULT res;
	res = Ability::CanUseAbility(pHuman);
	if( res != OR_OK )
	{
		return res;
	}

	CHAR MaterialBagIndex1 = (CHAR)pAbilityOpera->m_BagPos[2];
	CHAR MaterialBagIndex2 = (CHAR)pAbilityOpera->m_BagPos[3];
	if( (pAbilityOpera->m_BagPos[0] < 0)  || (pAbilityOpera->m_BagPos[0] >= MAX_BAG_SIZE)
	 || (pAbilityOpera->m_BagPos[1] < 0)  || (pAbilityOpera->m_BagPos[1] >= MAX_BAG_SIZE)
	 || (MaterialBagIndex1>= MAX_BAG_SIZE)|| (MaterialBagIndex2>= MAX_BAG_SIZE)
	 )
	{
		Assert(FALSE);
		return OR_ERROR;
	}

	Item* pGemItem = HumanItemLogic::GetBagItem( pHuman, pAbilityOpera->m_BagPos[0] );
	Item* pEquipItem = HumanItemLogic::GetBagItem( pHuman, pAbilityOpera->m_BagPos[1] );
	Item* pMaterialItem1 = NULL;
	Item* pMaterialItem2 = NULL;
	if( MaterialBagIndex1 != -1 )
	{
		pMaterialItem1 = HumanItemLogic::GetBagItem( pHuman, pAbilityOpera->m_BagPos[2] );
		Assert( pMaterialItem1 != NULL );
	}
	if( MaterialBagIndex2 != -1 )
	{
		pMaterialItem2 = HumanItemLogic::GetBagItem( pHuman, pAbilityOpera->m_BagPos[3] );
		Assert( pMaterialItem2 != NULL );
	}
	Assert( (pGemItem != NULL) && (pEquipItem != NULL) );

	if( (pGemItem->IsLock())
	 || (pEquipItem->IsLock())
	 || (pGemItem->IsPWLock())
	 || (pEquipItem->IsPWLock())
	 || (pGemItem->GetItemClass() != ICLASS_GEM)
	 || (pEquipItem->GetItemClass() != ICLASS_EQUIP)
	 )
	{
		return OR_STUFF_LACK;
	}

	//if( pHuman->__GetAbilityLevel(pAbilityOpera->m_AbilityID) < pGemItem->GetItemQual() )
	//{
	//	return OR_NO_LEVEL;
	//}

	if( IsGemConflict(pHuman, pAbilityOpera->m_BagPos[0], pAbilityOpera->m_BagPos[1]) == FALSE )
	{
		return OR_GEM_CONFLICT;
	}

	LuaInterface* pLuaInterface;
	pLuaInterface = pHuman->getScene()->GetLuaInterface();

	if( pEquipItem->GetGemSlotMax() <= pEquipItem->GetEquipGemCount()
	 || pLuaInterface->ExeScript_DDD(	ABILITY_LOGIC_SCRIPT, DEF_GEM_COUNT_CHECK,
										(INT)pHuman->getScene()->SceneID(),
										(INT)pHuman->GetID(),
										(INT)pAbilityOpera->m_BagPos[1] )
	 )
	{
		return OR_GEM_SLOT_LACK;
	}

	if( pLuaInterface->ExeScript_DDDD(	ABILITY_LOGIC_SCRIPT, DEF_IS_GEM_FIT_EQUIP,
										(INT)pHuman->getScene()->SceneID(),
										(INT)pHuman->GetID(),
										pGemItem->GetItemTableIndex(),
										pAbilityOpera->m_BagPos[1] ) == 0
	 )
	{ // 判断宝石和装备是否匹配
		return OR_GEM_NOT_FIT_EQUIP;
	}

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

BOOL AbilityInlay::IsGemConflict(Obj_Human* pHuman, BYTE GemIndex, BYTE EquipIndex)
{
__ENTER_FUNCTION

	Assert( pHuman );

	Item*	pGemItem	 = HumanItemLogic::GetBagItem(pHuman,GemIndex);
	Item*	pEquipItem	 = HumanItemLogic::GetBagItem(pHuman,EquipIndex);

	UINT uGemSerialNumber = pGemItem->GetItemTableIndex();

	LuaInterface* pLuaInterface;
	pLuaInterface = pHuman->getScene()->GetLuaInterface();

	for( INT i=0; i<pEquipItem->GetGemSlotMax(); ++i )
	{
		UINT idx;

		idx = pEquipItem->GetEquipGemInfo(i).m_GemIndex;
		if( idx < 1 )
			continue;

		if( pLuaInterface->ExeScript_DDD(	ABILITY_LOGIC_SCRIPT, DEF_GEM_CONFLICT_CHECK,
											(INT)pHuman->getScene()->SceneID(),
											(INT)uGemSerialNumber,
											(INT)idx )
			)
		{
			return FALSE;
		}
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

ORESULT AbilityInlay::OnProcOver(Obj_Human* pHuman)
{
__ENTER_FUNCTION

	Assert( pHuman );

	ORESULT res;

	res = CanUseAbility(pHuman); // 以免过程中某些状态发生变化
	if( res != OR_OK )
	{
		return res;
	}

	AbilityOpera* pAbilityOpera;

	pAbilityOpera = pHuman->GetAbilityOpera();
	Assert( pAbilityOpera );

	BYTE GemBagIndex = pAbilityOpera->m_BagPos[0];
	BYTE EquipBagIndex = pAbilityOpera->m_BagPos[1];
	CHAR MaterialBagIndex1 = (CHAR)pAbilityOpera->m_BagPos[2];
	CHAR MaterialBagIndex2 = (CHAR)pAbilityOpera->m_BagPos[3];

	Item* pGemItem = HumanItemLogic::GetBagItem( pHuman, GemBagIndex );
	Item* pEquipItem = HumanItemLogic::GetBagItem( pHuman, EquipBagIndex );
	Item* pMaterialItem1 = NULL;
	Item* pMaterialItem2 = NULL;
	if( MaterialBagIndex1 != -1 )
	{
		pMaterialItem1 = HumanItemLogic::GetBagItem( pHuman, MaterialBagIndex1 );
		Assert(pMaterialItem1 != NULL);
		
		
	}
	if( MaterialBagIndex2 != -1 )
	{
		pMaterialItem2 = HumanItemLogic::GetBagItem( pHuman, MaterialBagIndex2 );
		Assert(pMaterialItem1 != NULL);

		
	}
	Assert( (pGemItem != NULL) && (pEquipItem != NULL) );

	UINT GemIndex = pGemItem->GetItemTableIndex();
	UINT EquipIndex = pEquipItem->GetItemTableIndex();
	INT MaterialIndex1 = -1;
	INT MaterialIndex2 = -1;
	if( pMaterialItem1 )
	{
		MaterialIndex1 = pMaterialItem1->GetItemTableIndex();
		//if(pMaterialItem1)
		//	pMaterialItem1->SetItemParam( 0, IPV_INT, INT( EquipBagIndex ) );//modi:lby20071114记录当前装备的id
	}
	if( pMaterialItem2 )
	{
		MaterialIndex2 = pMaterialItem2->GetItemTableIndex();

		//if(pMaterialItem2)
			//pMaterialItem2->SetItemParam( 0, IPV_INT, INT( EquipBagIndex ) );//modi:lby20071114记录当前装备的id
	}

	if( (pGemItem->IsLock())
	 || (pEquipItem->IsLock())
	 || (pGemItem->IsPWLock())
	 || (pEquipItem->IsPWLock())
	 )
	{ // 这里判断是否锁定，下面消耗时就不再判断了
		return OR_STUFF_LACK;
	}
	if( pMaterialItem1 )
	{
		if( pMaterialItem1->IsLock()
			|| pMaterialItem1->IsPWLock()
			)
			return OR_STUFF_LACK;
	}
	if( pMaterialItem2 )
	{
		if( pMaterialItem2->IsLock()
			|| pMaterialItem2->IsPWLock()
			)
			return OR_STUFF_LACK;
	}

	EmbedResult flag;
	LuaInterface* pLuaInterface;

	pLuaInterface = pHuman->getScene()->GetLuaInterface();
	// 脚本判断是否镶嵌成功，如果成功则主动加上熟练度
	// 否则返回失败情况
	flag = (EmbedResult)pLuaInterface->ExeScript_DDDDDD(ABILITY_LOGIC_SCRIPT, DEF_GEM_EMBED_PROCESS, 
														(INT)pHuman->getScene()->SceneID(),
														(INT)pHuman->GetID(), 
														(INT)EquipBagIndex,
														(INT)GemIndex,
														(INT)MaterialIndex1,
														(INT)MaterialIndex2 ) ;

	GCDiscardItemResult Msg;
	Msg.setResult( DISCARDITEM_SUCCESS );
	GamePlayer* pGamePlayer = (GamePlayer*)(pHuman->GetPlayer());
	ITEM_LOG_PARAM	ItemLogParam;
	ItemLogParam.OpType	= ITEM_INLAY_LOST;
	
	BOOL bRet = FALSE;
	switch(flag)
	{
	case EMBED_SUCCESS:
		{
			if( MaterialIndex1 != -1 )
			{
				bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,MaterialBagIndex1);
				if(bRet)	SaveItemLog(&ItemLogParam);
			}
			if( MaterialIndex2 != -1 )
			{
				bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,MaterialBagIndex2);
				if(bRet)	SaveItemLog(&ItemLogParam);
			}

			INT nRet ;//= pHuman->UseGem(GemBagIndex, EquipBagIndex);
			if(USEGEM_SUCCESS == nRet)
			{
				g_ImpactCore.SendImpactToUnit(*pHuman, 49, pHuman->GetID());
			}
			return OnProcSuccess(pHuman);
		}
	case GEM_GONE:
		
		if( MaterialIndex1 != -1 )
		{
		bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,GemBagIndex);
			if(bRet)	SaveItemLog(&ItemLogParam);
		}
		if( MaterialIndex2 != -1 )
		{
			bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,MaterialBagIndex2);
			if(bRet)	SaveItemLog(&ItemLogParam);
		}

		bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,GemBagIndex);
		if(bRet)	SaveItemLog(&ItemLogParam);

		Msg.setItemTableIndex( GemIndex );
		Msg.setBagIndex( GemBagIndex );
		pGamePlayer->SendPacket( &Msg );
		return OR_FAILURE;
	case EQUIP_GONE:
		if( MaterialIndex1 != -1 )
		{
		bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,EquipBagIndex);
			if(bRet)	SaveItemLog(&ItemLogParam);
		}
		if( MaterialIndex2 != -1 )
		{
			bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,MaterialBagIndex2);
			if(bRet)	SaveItemLog(&ItemLogParam);
		}

		bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,EquipBagIndex);
		if(bRet)	SaveItemLog(&ItemLogParam);

		Msg.setItemTableIndex( EquipIndex );
		Msg.setBagIndex( EquipBagIndex );
		pGamePlayer->SendPacket( &Msg );
		return OR_FAILURE;
	case BOTH_GONE:

		if( MaterialIndex1 != -1 )
		{
		bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,EquipBagIndex);
			if(bRet)	SaveItemLog(&ItemLogParam);
		}
		if( MaterialIndex2 != -1 )
		{
		bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,GemBagIndex);
			if(bRet)	SaveItemLog(&ItemLogParam);
		}

		bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,EquipBagIndex);
		if(bRet)	SaveItemLog(&ItemLogParam);
		bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,GemBagIndex);
		if(bRet)	SaveItemLog(&ItemLogParam);

		Msg.setItemTableIndex( GemIndex );
		Msg.setBagIndex( GemBagIndex );
		pGamePlayer->SendPacket( &Msg );

		Msg.setItemTableIndex( EquipIndex );
		Msg.setBagIndex( EquipBagIndex );
		pGamePlayer->SendPacket( &Msg );
		return OR_FAILURE;
	case ENERGY_NOT_ENOUGH:
		return OR_NOT_ENOUGH_ENERGY;
	case GEM_DEGRADE_1:
		{
			if( MaterialIndex1 != -1 )
			{
				bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,MaterialBagIndex1);
				if(bRet)	SaveItemLog(&ItemLogParam);
			}
			if( MaterialIndex2 != -1 )
			{
				bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,MaterialBagIndex2);
				if(bRet)	SaveItemLog(&ItemLogParam);
			}

			bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,GemBagIndex);
			if(bRet)	SaveItemLog(&ItemLogParam);

			_ITEM_TYPE	 type = ConvertSerial2ItemType(GemIndex);
		/*	if(type.m_Quality>1)
			{
				type.m_Quality--;
				ITEM_LOG_PARAM	ItemLogParam;
				ItemLogParam.OpType		= ITEM_CREATE_REMOVE_GEM;
				ItemLogParam.CharGUID	= pHuman->GetGUID();
				ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
				ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
				ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
				UINT GemBagIndex_Temp = GemBagIndex;
				HumanItemLogic::CreateItemToBag(&ItemLogParam,pHuman,type.ToSerial(),GemBagIndex_Temp,ITEM_DROP_MONSTER);
			}*/

			Msg.setItemTableIndex( GemIndex );
			Msg.setBagIndex( GemBagIndex );
			pGamePlayer->SendPacket( &Msg );
		}
		return OR_FAILURE;
	case GEM_DEGRADE_2:
		{
			if( MaterialIndex1 != -1 )
			{
				bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,MaterialBagIndex1);
				if(bRet)	SaveItemLog(&ItemLogParam);
			}
			if( MaterialIndex2 != -1 )
			{
				bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,MaterialBagIndex2);
				if(bRet)	SaveItemLog(&ItemLogParam);
			}

			bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,GemBagIndex);
			if(bRet)	SaveItemLog(&ItemLogParam);

			_ITEM_TYPE	 type = ConvertSerial2ItemType(GemIndex);
			/*if(type.m_Quality>2)
			{
				type.m_Quality -= 2;
				ITEM_LOG_PARAM	ItemLogParam;
				ItemLogParam.OpType		= ITEM_CREATE_REMOVE_GEM;
				ItemLogParam.CharGUID	= pHuman->GetGUID();
				ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
				ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
				ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
				UINT GemBagIndex_Temp = GemBagIndex;
				HumanItemLogic::CreateItemToBag(&ItemLogParam,pHuman,type.ToSerial(),GemBagIndex_Temp,ITEM_DROP_MONSTER);
			}*/

			Msg.setItemTableIndex( GemIndex );
			Msg.setBagIndex( GemBagIndex );
			pGamePlayer->SendPacket( &Msg );
		}
		return OR_FAILURE;
	case GEM_DEGRADE_3:
		{
			if( MaterialIndex1 != -1 )
			{
				bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,MaterialBagIndex1);
				if(bRet)	SaveItemLog(&ItemLogParam);
			}
			if( MaterialIndex2 != -1 )
			{
				bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,MaterialBagIndex2);
				if(bRet)	SaveItemLog(&ItemLogParam);
			}

			bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,GemBagIndex);
			if(bRet)	SaveItemLog(&ItemLogParam);

			_ITEM_TYPE	 type = ConvertSerial2ItemType(GemIndex);
			/*if(type.m_Quality>3)
			{
				type.m_Quality -= 3;
				ITEM_LOG_PARAM	ItemLogParam;
				ItemLogParam.OpType		= ITEM_CREATE_REMOVE_GEM;
				ItemLogParam.CharGUID	= pHuman->GetGUID();
				ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
				ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
				ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
				UINT GemBagIndex_Temp = GemBagIndex;
				HumanItemLogic::CreateItemToBag(&ItemLogParam,pHuman,type.ToSerial(),GemBagIndex_Temp,ITEM_DROP_MONSTER);
			}*/

			Msg.setItemTableIndex( GemIndex );
			Msg.setBagIndex( GemBagIndex );
			pGamePlayer->SendPacket( &Msg );
		}
		return OR_FAILURE;
	default:
		Assert(FALSE);
		return OR_ERROR;
	}

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AbilityInlay::OnProcSuccess(Obj_Human* pHuman, const _PRESCRIPTION_DATA* pPrescrData)
{
	return OR_WARNING;
}

ORESULT AbilityInlay::OnProcSuccess(Obj_Human* pHuman)
{
__ENTER_FUNCTION

	Assert( pHuman );

	AbilityOpera* pAbilityOpera;

	pAbilityOpera = pHuman->GetAbilityOpera();
	Assert( pAbilityOpera );

	LuaInterface* pLuaInterface;
	pLuaInterface = pHuman->getScene()->GetLuaInterface();

	GamePlayer* pGamePlayer;
	pGamePlayer = (GamePlayer*)pHuman->GetPlayer();
	Assert( pGamePlayer != NULL );

	// 发送成功消息以及物品和状态变化
	Item* pItem = HumanItemLogic::GetBagItem(pHuman, pAbilityOpera->m_BagPos[1]);
	Assert( pItem );
	GCAbilitySucc SuccMsg;
	SuccMsg.SetAbilityID( pAbilityOpera->m_AbilityID );
	SuccMsg.SetPrescriptionID( INVALID_ID );
	SuccMsg.SetItemSerial( pItem->GetItemTableIndex() );
	//SuccMsg.SetSuccFlag( TRUE );
	pGamePlayer->SendPacket( &SuccMsg );

	return OR_OK;

__LEAVE_FUNCTION
	return OR_ERROR;
}

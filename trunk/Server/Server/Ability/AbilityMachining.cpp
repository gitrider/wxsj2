// AbilityMachining.cpp

#include "stdafx.h"
#include "AbilityMachining.h"
#include "GameTable.h"
#include "Scene.h"
#include "GCDiscardItemResult.h"
#include "GamePlayer.h"
#include "HumanItemLogic.h"
#include "GCNotifyEquip.h"
#include "GCAbilitySucc.h"
#include "LogDefine.h"

#define ABILITY_LOGIC_SCRIPT			701601

AbilityMachining::AbilityMachining(const Ability& a) : AbilityCompound(a)
{
}

ORESULT AbilityMachining::CanUseAbility(Obj_Human* pHuman)
{
__ENTER_FUNCTION

	Assert( pHuman );

	AbilityOpera* pAbilityOpera;

	pAbilityOpera = pHuman->GetAbilityOpera();
	Assert( pAbilityOpera );

	switch( pAbilityOpera->m_SubType )
	{
	case NORMAL_TYPE:
		return AbilityCompound::CanUseAbility(pHuman);
	case GEM_COMPOUND:
		{
			ORESULT res;
			res = Ability::CanUseAbility(pHuman);
			if( res != OR_OK )
			{
				return res;
			}

			if( (pAbilityOpera->m_BagPos[0] < 0) || (pAbilityOpera->m_BagPos[0] >= MAX_BAG_SIZE)
			 || (pAbilityOpera->m_BagPos[1] < 0) || (pAbilityOpera->m_BagPos[1] >= MAX_BAG_SIZE)
			 )
			{
				Assert(FALSE);
				return OR_ERROR;
			}

			//const	_ITEM* pGem1 = pHuman->GetItem(pAbilityOpera->m_BagPos[0]);
			//const	_ITEM* pGem2 = pHuman->GetItem(pAbilityOpera->m_BagPos[1]);
			
			Item*	pGem1Item = HumanItemLogic::GetBagItem(pHuman,pAbilityOpera->m_BagPos[0]);
			Item*	pGem2Item = HumanItemLogic::GetBagItem(pHuman,pAbilityOpera->m_BagPos[1]);

			Assert( (pGem1Item != NULL) && (pGem2Item != NULL) );

			if( (pGem1Item->GetItemClass() != ICLASS_GEM) || (pGem2Item->GetItemClass() != ICLASS_GEM) )
			{
				return OR_STUFF_LACK;
			}

			if((pGem1Item->GetItemIndex() != pGem2Item->GetItemIndex())// (pGem1Item->GetItemQual() != pGem2Item->GetItemQual())
			 )
			{
				return OR_STUFF_LACK;
			}

			//if(  >= MAX_GEM_QUALITY ) // pGem1Item->GetItemQual()û��������
			//{
			//	return OR_CANNOT_UPGRADE;
			//}

			return OR_OK;
		}
		break;
	case GEM_EMBED:
		{
			ORESULT res;
			res = Ability::CanUseAbility(pHuman);
			if( res != OR_OK )
			{
				return res;
			}

			if( (pAbilityOpera->m_BagPos[0] < 0) || (pAbilityOpera->m_BagPos[0] >= MAX_BAG_SIZE)
			 || (pAbilityOpera->m_BagPos[1] < 0) || (pAbilityOpera->m_BagPos[1] >= MAX_BAG_SIZE)
			 )
			{
				Assert(FALSE);
				return OR_ERROR;
			}

			//const	_ITEM* pGem = pHuman->GetItem(pAbilityOpera->m_BagPos[0]);
			//const	_ITEM* pEquip = pHuman->GetItem(pAbilityOpera->m_BagPos[1]);

			Item*		pGemItem	=	HumanItemLogic::GetBagItem(pHuman,pAbilityOpera->m_BagPos[0]);
			Item*		pEquipItem	=	HumanItemLogic::GetBagItem(pHuman,pAbilityOpera->m_BagPos[1]);
			Assert( (pGemItem != NULL) && (pEquipItem != NULL) );

			if( (pGemItem->GetItemClass() != ICLASS_GEM) || (pEquipItem->GetItemClass() != ICLASS_EQUIP) )
			{
				return OR_STUFF_LACK;
			}

			if( IsGemConflict(pHuman, pAbilityOpera->m_BagPos[0], pAbilityOpera->m_BagPos[1]) == FALSE )
			{
				return OR_GEM_CONFLICT;
			}

			LuaInterface* pLuaInterface;
			pLuaInterface = pHuman->getScene()->GetLuaInterface();

			if( pEquipItem->GetGemSlotMax() <= pEquipItem->GetEquipGemCount()
				|| pLuaInterface->ExeScript_DDD(	ABILITY_LOGIC_SCRIPT, 
													"TooManyGems",
													(INT)pHuman->getScene()->SceneID(),
													(INT)pHuman->GetID(),
													(INT)pAbilityOpera->m_BagPos[1] )
			 )
			{
				return OR_GEM_SLOT_LACK;
			}

			return OR_OK;
		}
		break;
	default:
		Assert(FALSE);
		return OR_ERROR;
	}

	return OR_WARNING;

__LEAVE_FUNCTION

	return OR_ERROR;
}

BOOL AbilityMachining::IsGemConflict(Obj_Human* pHuman, BYTE GemIndex, BYTE EquipIndex)
{
__ENTER_FUNCTION

	Assert( pHuman );

	//const	_ITEM* pGem = pHuman->GetItem(GemIndex);
	//const	_ITEM* pEquip = pHuman->GetItem(EquipIndex);

	Item*	pGemItem	 = HumanItemLogic::GetBagItem(pHuman,GemIndex);
	Item*	pEquipItem	 = HumanItemLogic::GetBagItem(pHuman,EquipIndex);

	UINT uGemSerialNumber = pGemItem->GetItemTableIndex();

	LuaInterface* pLuaInterface;
	pLuaInterface = pHuman->getScene()->GetLuaInterface();
	_ITEM tmpGem;
	for( INT i=0; i<pEquipItem->GetGemSlotMax(); ++i )
	{
		UINT idx;

		idx = pEquipItem->GetEquipGemInfo(i).m_GemIndex;
		if( idx < 1 )
			continue;

		if( pLuaInterface->ExeScript_DDD(	ABILITY_LOGIC_SCRIPT, 
											"IsGemConflict",
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

ORESULT AbilityMachining::OnProcOver(Obj_Human* pHuman)
{
__ENTER_FUNCTION

	Assert( pHuman );

	AbilityOpera* pAbilityOpera;

	pAbilityOpera = pHuman->GetAbilityOpera();
	Assert( pAbilityOpera );

	switch( pAbilityOpera->m_SubType )
	{
	case NORMAL_TYPE:
		return AbilityCompound::OnProcOver(pHuman);
	case GEM_COMPOUND:
		{
			ORESULT res;
			BOOL flag = FALSE;
			LuaInterface* pLuaInterface;
			UINT itemIndex;

			res = CanUseAbility(pHuman); // ���������ĳЩ״̬�����仯
			if( res != OR_OK )
			{
				return res;
			}

			pLuaInterface = pHuman->getScene()->GetLuaInterface();
			
			

			itemIndex = HumanItemLogic::GetBagItem(pHuman,
												pAbilityOpera->m_BagPos[0])->GetItemTableIndex();

			// ���ܳɹ�ʧ�ܣ���ʯ��û����
			GCDiscardItemResult Msg;
			GamePlayer* pGamePlayer;
			pGamePlayer = (GamePlayer*)pHuman->GetPlayer();
			Assert( pGamePlayer != NULL );
			ITEM_LOG_PARAM	ItemLogParam;
			ItemLogParam.OpType	= ITEM_GEM_COMPOUND_LOST;
			BOOL bRet	=	HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,pAbilityOpera->m_BagPos[0]);
			if(bRet)
			{
				SaveItemLog(&ItemLogParam);
			}
			Msg.setItemTableIndex( itemIndex );
			Msg.setResult( DISCARDITEM_SUCCESS );
			Msg.setBagIndex( pAbilityOpera->m_BagPos[0] );
			pGamePlayer->SendPacket( &Msg );
			

			bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,pAbilityOpera->m_BagPos[1]);
			if(bRet)
			{
				SaveItemLog(&ItemLogParam);
			}
			Msg.setBagIndex( pAbilityOpera->m_BagPos[1] );
			pGamePlayer->SendPacket( &Msg );

			if( pLuaInterface->ExeScript_DDD(	ABILITY_LOGIC_SCRIPT, 
												"CompoundProc", 
												(INT)pHuman->getScene()->SceneID(),
												(INT)pHuman->GetID(), 
												(INT)itemIndex ) != FALSE ) // ������Ȼ��Ҫ���� lua script
			{ // �ű��ж��Ƿ�ϳɳɹ����ɹ����Զ�����������
				flag = TRUE;
				pAbilityOpera->m_uItemIndex = itemIndex;
				res = OnProcSuccess( pHuman );
			}

			if( flag == TRUE )
			{
				return res;
			}
			else
			{
				return OR_FAILURE;
			}
		}
		break;
	case GEM_EMBED:
		{
			ORESULT res;

			enum EmbedResult
			{
				EMBED_SUCCESS = 0,
				GEM_GONE,
				EQUIP_GONE,
				BOTH_GONE,
			};
			
			EmbedResult flag;
			LuaInterface* pLuaInterface;

			BYTE GemBagIndex = pAbilityOpera->m_BagPos[0];
			BYTE EquipBagIndex = pAbilityOpera->m_BagPos[1];

			UINT GemIndex = 
			HumanItemLogic::GetBagItem(pHuman,GemBagIndex)->GetItemTableIndex();

			UINT EquipIndex = 
			HumanItemLogic::GetBagItem(pHuman,EquipBagIndex)->GetItemTableIndex();
			res = CanUseAbility(pHuman); // ���������ĳЩ״̬�����仯
			if( res != OR_OK )
			{
				return res;
			}

			pLuaInterface = pHuman->getScene()->GetLuaInterface();
			// �ű��ж��Ƿ���Ƕ�ɹ�������ɹ�����������������
			// ���򷵻�ʧ�����
			flag = (EmbedResult)pLuaInterface->ExeScript_DDD(	ABILITY_LOGIC_SCRIPT, 
																"EmbedProc", 
																(INT)pHuman->getScene()->SceneID(),
																(INT)pHuman->GetID(), 
																(INT)GemIndex ) ;

			GCDiscardItemResult Msg;
			Msg.setResult( DISCARDITEM_SUCCESS );
			GamePlayer* pGamePlayer = (GamePlayer*)(pHuman->GetPlayer());
			ITEM_LOG_PARAM	ItemLogParam;
			ItemLogParam.OpType	=	ITEM_MACHINE_LOST;
			BOOL bRet	= FALSE;
			switch(flag)
			{
			case EMBED_SUCCESS:
				{
					INT nRet;// = pHuman->UseGem(GemBagIndex, EquipBagIndex);
					if(USEGEM_SUCCESS == nRet)
					{
						g_ImpactCore.SendImpactToUnit(*pHuman, 49, pHuman->GetID());
					}
				}
				return OnProcSuccess(pHuman);
			case GEM_GONE:
				bRet =	HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,GemBagIndex);
				if(bRet) SaveItemLog(&ItemLogParam);
				//pHuman->EraseItem(GemBagIndex);
				Msg.setItemTableIndex( GemIndex );
				Msg.setBagIndex( GemBagIndex );
				pGamePlayer->SendPacket( &Msg );
				return OR_FAILURE;
			case EQUIP_GONE:
				bRet	=	HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,EquipBagIndex);
				if(bRet)	SaveItemLog(&ItemLogParam);
				//pHuman->EraseItem(EquipBagIndex);
				Msg.setItemTableIndex( EquipIndex );
				Msg.setBagIndex( EquipBagIndex );
				pGamePlayer->SendPacket( &Msg );
				return OR_FAILURE;
			case BOTH_GONE:

				bRet	=	HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,EquipBagIndex);
				if(bRet)	SaveItemLog(&ItemLogParam);
				bRet	=	HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,GemBagIndex);
				if(bRet)	SaveItemLog(&ItemLogParam);
				Msg.setItemTableIndex( GemIndex );
				Msg.setBagIndex( GemBagIndex );
				pGamePlayer->SendPacket( &Msg );

				Msg.setItemTableIndex( EquipIndex );
				Msg.setBagIndex( EquipBagIndex );
				pGamePlayer->SendPacket( &Msg );
				return OR_FAILURE;
			default:
				Assert(FALSE);
				return OR_ERROR;
			}
		}
		break;
	default:
		Assert(FALSE);
		return OR_ERROR;
	}

	return OR_WARNING;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AbilityMachining::OnProcSuccess(Obj_Human* pHuman, const _PRESCRIPTION_DATA* pPrescrData)
{
__ENTER_FUNCTION

	Assert( pHuman );
	Assert( pPrescrData );

	ORESULT res;
	res = AbilityCompound::OnProcSuccess(pHuman, pPrescrData);
	if( res != OR_OK )
	{
		return res;
	}

	OnProcSuccess( pHuman );

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AbilityMachining::OnProcSuccess(Obj_Human* pHuman)
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

	switch( pAbilityOpera->m_SubType )
	{
	case NORMAL_TYPE:
		break;
	case GEM_COMPOUND:
		{
			_ITEM_TYPE GemType;
			UINT bi;
			
			ITEM_LOG_PARAM	ItemLogParam;
			ItemLogParam.OpType		= ITEM_CREATE_ABILITY_COMPOUD;
			ItemLogParam.CharGUID	= pHuman->GetGUID();
			ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
			ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
			ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;

			GemType = ConvertSerial2ItemType( pAbilityOpera->m_uItemIndex );

			//GemType.m_Quality++;
			HumanItemLogic::CreateItemToBag(&ItemLogParam,pHuman,GemType.ToSerial(),bi);
			if(bi==INVALID_INDEX) //����ʧ��
			{
				return OR_ERROR;
			}

			SaveItemLog(&ItemLogParam);

			GCNotifyEquip Msg;
			Msg.SetBagIndex( bi );
			Item* pItem = HumanItemLogic::GetBagItem(pHuman,bi);
			Assert(pItem);
			pItem->SaveValueTo(Msg.GetItem());
			pGamePlayer->SendPacket(&Msg);

			// ���ͳɹ���Ϣ�Լ���Ʒ��״̬�仯
			GCAbilitySucc SuccMsg;
			SuccMsg.SetAbilityID( EAN_GEM_COMPOUNDED );
			SuccMsg.SetPrescriptionID( INVALID_ID );
			SuccMsg.SetItemSerial( GemType.ToSerial() );
			//SuccMsg.SetSuccFlag( TRUE );
			pGamePlayer->SendPacket( &SuccMsg );
		}
		break;
	case GEM_EMBED: // ����Ҫ����Ĵ�����
		{
			// ���ͳɹ���Ϣ�Լ���Ʒ��״̬�仯
			Item* pItem = HumanItemLogic::GetBagItem(pHuman, pAbilityOpera->m_BagPos[1]);
			Assert( pItem );
			GCAbilitySucc SuccMsg;
			SuccMsg.SetAbilityID( EAN_GEM_EMBEDDED );
			SuccMsg.SetPrescriptionID( INVALID_ID );
			SuccMsg.SetItemSerial( pItem->GetItemTableIndex() );
			//SuccMsg.SetSuccFlag( TRUE );
			pGamePlayer->SendPacket( &SuccMsg );
		}
		break;
	default:
		Assert(FALSE);
		return OR_ERROR;
	}

	// �ж�һ���������Ƿ��㹻������
	pLuaInterface->ExeScript_DDD(	ABILITY_LOGIC_SCRIPT, 
									"CheckAbilityLevel", 
									(INT)pHuman->getScene()->SceneID(),
									(INT)pHuman->GetID(), 
									(INT)pAbilityOpera->m_AbilityID ) ;

	return OR_OK;

__LEAVE_FUNCTION
	return OR_ERROR;
}

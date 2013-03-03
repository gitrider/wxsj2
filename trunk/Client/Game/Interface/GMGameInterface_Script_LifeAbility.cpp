
#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "LuaPlus.h"
#include "Global.h"
#include "Object/Manager/ObjectManager.h"
#include "Object/Logic/Object.h"
#include "Object/Logic/Character/Obj_Character.h"
#include "Object/Logic/Character/AI/GMAI_Base.h"
#include "Object/Logic/Character/AI/GMAI_MySelf.h"
#include "Object/Logic/Character/Obj_Character.h"
#include "Object/Logic/Item\Obj_Item_Equip.h"
#include "Object/Logic/Item\Obj_Item_Gem.h"
#include "Event/GMEventSystem.h"
#include "Network/NetManager.h"
#include "DataPool/GMDataPool.h"
#include "object/Logic/character/obj_playermyself.h"
#include "DataPool\GMDP_CharacterData.h"
#include "Action\GMActionSystem.h"
#include "DBC\GMDataBase.h"
#include "Procedure/GameProcedure.h"
#include "CGUseGem.h"
#include "CGGemCompound.h"
#include "GMInterface_Script_Talk.h"
#include "GMGameInterface_Script_SouXia.h"



namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	LifeAbility LifeAbility::s_LifeAbility;
	LuaPlus::LuaObject* LifeAbility::s_pMetaTable = NULL;

	// 得到生活技能的数量
/*	INT LifeAbility::GetLifeAbility_Count(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();

		const SLIFEABILITY_MAP& My_LifeAbility = pCharData->Get_LifeAbility();
		if( My_LifeAbility.size() > 0 )
		{
			state->PushInteger((INT)My_LifeAbility.size());
			return 1;
		}
		return 0;
	}
*/	

	INT LifeAbility::GetLifeAbility_Data(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if( !args[2].IsInteger() ) return 0;
		if( !args[3].IsInteger() ) return 0;
		if( !args[4].IsString() ) return 0;


		std::map<STRING, INT> dbc_file;
		dbc_file.clear();
		dbc_file.insert( make_pair( "caikuang.tab",DBC_CAIKUANG_LEVELINFO ) );
		dbc_file.insert( make_pair( "shouge.tab",DBC_SHOUGE_LEVELINFO ) );
		dbc_file.insert( make_pair( "buzhuo.tab",DBC_BUZHUO_LEVELINFO ) );
		dbc_file.insert( make_pair( "zhuzao.tab",DBC_ZHUZAO_LEVELINFO ) );
		dbc_file.insert( make_pair( "zhiyi.tab",DBC_ZHIYI_LEVELINFO ) );
		dbc_file.insert( make_pair( "gongjiang.tab",DBC_GONGJIANG_LEVELINFO ) );
		dbc_file.insert( make_pair( "pengren.tab",DBC_PENGREN_LEVELINFO ) );

		INT nAbilityID = args[2].GetInteger();		//生活技能ID
		INT nLevel = args[3].GetInteger();			//生活技能的级别

		DBC_DEFINEHANDLE(s_pAbility, DBC_LIFEABILITY_DEFINE);
		_DBC_LIFEABILITY_DEFINE* pData = (_DBC_LIFEABILITY_DEFINE*)s_pAbility->Search_Index_EQU(nAbilityID);
		if( !pData ) return 0;

		INT idDataBase= dbc_file[pData->szLevelupTable];
		if( idDataBase == 0 ) return 0;


		//DBC_DEFINEHANDLE(s_pAbilityLevel, idDataBase);
		const tDataBase* pAbilityLevel = CDataBaseSystem::GetMe()->GetDataBase( idDataBase );
		if ( strcmp( args[ 4 ].GetString(), "LevelNum" ) == 0  )	//级别的总数
		{
			state->PushInteger( pAbilityLevel->GetRecordsNum() );
			return 1;
		} 
		else if ( strcmp( args[ 4 ].GetString(), "AbilityName" ) == 0  )	//生活技能的名字
		{
			state->PushString( pData->szName );
			return 1;
		}
		else if ( strcmp( args[ 4 ].GetString(), "AbilityDesc" ) == 0  )		//技能描述
		{
			state->PushString(pData->szExplain);
			return 1;
		}

		_DBC_LIFEABILITY_LEVELINFO *pLevel = (_DBC_LIFEABILITY_LEVELINFO*)pAbilityLevel->Search_Index_EQU(nLevel);
		if( !pLevel ) return 0;

		if ( strcmp( args[ 4 ].GetString(), "LevelName" ) == 0  )	//不同等级的名称
		{
			state->PushString( pLevel->szName );
			return 1;
		}
		else if ( strcmp( args[ 4 ].GetString(), "NeedExp" ) == 0  )	//所需要的经验
		{
			state->PushInteger( pLevel->nExpNeed );
			return 1;
		}
		else if ( strcmp( args[ 4 ].GetString(), "NeedMoney" ) == 0  )		//所需要的金钱
		{
			state->PushInteger(pLevel->nMoneyNeed);
			return 1;
		}
		else if ( strcmp( args[ 4 ].GetString(), "SkillExp" ) == 0  )		//当前级别所需要的熟练度
		{
			INT skillPro = pLevel->nSkillProClient;
			state->PushInteger(skillPro);
			return 1;
		}
		else if ( strcmp( args[ 4 ].GetString(), "SkillMaxExp" ) == 0 )	//当前级别的熟练度上限
		{
			state->PushInteger( pLevel->nSkillMaxProLevel );
			return 1;
		}

		return 0;
	}

	INT LifeAbility::GetActionID_ByAbilityID(LuaPlus::LuaState*state)
	{
		LuaStack args(state);
		if( !args[2].IsInteger() ) return 0;
		if( !args[3].IsInteger() ) return 0;

		INT nAbilityID = args[2].GetInteger();
		INT nLevel = args[3].GetInteger();

		//tActionItem* pActionItem = CActionSystem::GetMe()->GetAction_LifeAbilityID(nAbilityID);
		//if( !pActionItem ) return 0;

		//INT nActionID = pActionItem->GetID();

		tActionItem* pActionItem = CActionSystem::GetMe()->UserLifeAbilityStudy_Get(nAbilityID,nLevel);

		if( !pActionItem )
		{
			state->PushInteger(-1);
			return 1;
		}

		INT nActionID = pActionItem->GetID();
		state->PushInteger(nActionID);
		return 1;
	}


	INT LifeAbility::GetLifeAbility_Number(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( !args[2].IsInteger() ) return 0;

		INT	nActionID	= args[2].GetInteger();

		CActionItem* pAction = (CActionItem*)CActionSystem::GetMe()->GetAction(nActionID);

		if(!pAction) return 0;
	
		if(pAction->GetDefineID() != INVALID_ID)
		{
			state->PushInteger(pAction->GetDefineID());
			return 1;
		}
		return 0;
	}
	INT LifeAbility::GetPrescr_Material_Number(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( !args[2].IsInteger() ) return 0;

		INT	nPrescrID	= args[2].GetInteger();

		CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();

		const SCLIENT_PRESCR* The_Prescr = pCharData->Get_Prescr(nPrescrID);

		INT ret;
		if(The_Prescr->m_pDefine->nStuff1ID == INVALID_ID) ret = 0;
		else if (The_Prescr->m_pDefine->nStuff2ID == INVALID_ID) ret = 1;
		else if (The_Prescr->m_pDefine->nStuff3ID == INVALID_ID) ret = 2;
		else if (The_Prescr->m_pDefine->nStuff4ID == INVALID_ID) ret = 3;
		else if (The_Prescr->m_pDefine->nStuff5ID == INVALID_ID) ret = 4;
		else ret = 5;

		state->PushInteger(ret);
		return 1;

	}
	//remove
	INT LifeAbility::GetPrescr_Material(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( !args[2].IsInteger() ) return 0;

		INT	nItemID	= args[2].GetInteger();

				BYTE    nItemClass = GetSerialClass(nItemID);
				switch( nItemClass )
				{

					//武器WEAPON、防具DEFENCE、饰物ADORN   1
				case ICLASS_EQUIP: 
					{

						//if (ISCommonEquip(nItemID))
						//{  
						//	// 打开数据表
						//	DBC_DEFINEHANDLE(s_pItem_EquipVisual, DBC_ITEM_COMMON_EQUIP_VISUAL);
						//	// 搜索纪录

						//	const _DBC_ITEM_COMMON_EQUIP_VISUAL* pEquipVisual = \
						//		(const _DBC_ITEM_COMMON_EQUIP_VISUAL*)s_pItem_EquipVisual->Search_Index_EQU((UINT)nItemID);
						//	if(!pEquipVisual) return NULL;
						//	state->PushString(pEquipVisual->szName);
						//	state->PushString(pEquipVisual->szIcon);
						//}
						//else

						//{	
						//	// 打开数据表
						//	DBC_DEFINEHANDLE(s_pItem_EquipVisual, DBC_ITEM_ADVANCED_EQUIP_VISUAL);
						//	// 搜索纪录
						//	const _DBC_ITEM_ADVANCED_EQUIP_VISUAL* pEquipVisual = \
						//		(const _DBC_ITEM_ADVANCED_EQUIP_VISUAL*)s_pItem_EquipVisual->Search_Index_EQU((UINT)nItemID);
						//	if(!pEquipVisual) return NULL;
						//	state->PushString(pEquipVisual->szName);
						//	state->PushString(pEquipVisual->szIcon);
						//}


							// 打开数据表
							DBC_DEFINEHANDLE(s_pItem_EquipVisual, DBC_ITEM_EQUIP_VISUAL);
							// 搜索纪录
							const _DBC_ITEM_EQUIP_VISUAL* pEquipVisual = \
								(const _DBC_ITEM_EQUIP_VISUAL*)s_pItem_EquipVisual->Search_Index_EQU((UINT)nItemID);
							if(!pEquipVisual) return NULL;
							state->PushString(pEquipVisual->szName);
							state->PushString(pEquipVisual->szIcon);


						
						return 2;
					}
					break;

				case ICLASS_MATERIAL: //原料 2

				case ICLASS_COMITEM:  //药品 3 

				case ICLASS_TASKITEM: //任务物品 4

				case ICLASS_IDENT:
					{
						//打开数据表
						DBC_DEFINEHANDLE(s_pItem_Medic, DBC_ITEM_MEDIC);
						//搜索纪录
						const _DBC_ITEM_MEDIC* pMedic = (const _DBC_ITEM_MEDIC*)s_pItem_Medic->Search_Index_EQU((UINT)nItemID);
						if(!pMedic) return 0;

						state->PushString(pMedic->szName);
						state->PushString(pMedic->szIcon);
						return 2;
						
					}

					break;
				case ICLASS_GEM: //宝石 5
					{
					//打开数据表
					DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);
					//搜索纪录
					const _DBC_ITEM_GEM* pGem = (const _DBC_ITEM_GEM*)s_pItem_Gem->Search_Index_EQU((UINT)nItemID);
					if(!pGem) return 0;

					state->PushString(pGem->szName);
					state->PushString(pGem->szIcon);
					return 2;

					}

					break;

				case ICLASS_STOREMAP:   //藏宝图  6
					{

					//打开数据表
					DBC_DEFINEHANDLE(s_pItem_StoreMap, DBC_ITEM_STOREMAP);
					//搜索纪录
					const _DBC_ITEM_STOREMAP* pStoreMap = (const _DBC_ITEM_STOREMAP*)s_pItem_StoreMap->Search_Index_EQU((UINT)nItemID);
					if(!pStoreMap) return 0;

					state->PushString(pStoreMap->szName);
					state->PushString(pStoreMap->szIcon);
					return 2;
					}
					break;

				case ICLASS_TALISMAN : //法宝  7

					break;

				case ICLASS_GUILDITEM : //帮会物品 8

					break;

				default:
					break;
				}	

		return 0;
	}



	INT LifeAbility::GetPrescrList_Item_FromNum(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:LifeAbility:GetPrescrList_Item_FromNum args error");
		}	

		INT nIndex = args[2].GetInteger();

//		const SCLIENT_PRESCR* pPrescr = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr(nIndex);
		const SPRESCR_MAP& mapPrescr = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr();
		SPRESCR_MAP::const_iterator it;
		INT count = 0;

		for(it=mapPrescr.begin(); it!=mapPrescr.end(); it++)
		{
			if(count == nIndex) break;
			count++;
		}

		state->PushInteger( (&(it->second))->m_pDefine->nID);


		return 1;
	}
	INT LifeAbility::GetPrescr_Item_Maximum(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:LifeAbility:GetPrescrList_Item_FromNum args error");
		}	

		INT nIndex = args[2].GetInteger();

		const SCLIENT_PRESCR* pPrescr = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr(nIndex);
		INT max = INVALID_ID,count,need_count,need_guid;

		if( (need_guid = pPrescr->m_pDefine->nStuff1ID) != INVALID_ID)
		{
			count = CDataPool::GetMe()->UserBag_CountItemByIDTable(need_guid);
			need_count = pPrescr->m_pDefine->nStuff1num;
	//		max = ( max > (count / need_count) ) ? max : (count / need_count);
			if( (max < 0) || max > (count / need_count) ) 
				max = count / need_count;
		}

		if( (need_guid = pPrescr->m_pDefine->nStuff2ID) != INVALID_ID)
		{
			count = CDataPool::GetMe()->UserBag_CountItemByIDTable(need_guid);
			need_count = pPrescr->m_pDefine->nStuff2num;
			if( (max < 0) || max > (count / need_count) ) 
				max = count / need_count;
		}
		if( (need_guid = pPrescr->m_pDefine->nStuff3ID) != INVALID_ID)
		{
			count = CDataPool::GetMe()->UserBag_CountItemByIDTable(need_guid);
			need_count = pPrescr->m_pDefine->nStuff3num;
			if( (max < 0) || max > (count / need_count) ) 
				max = count / need_count;
		}

		if( (need_guid = pPrescr->m_pDefine->nStuff4ID) != INVALID_ID)
		{
			count = CDataPool::GetMe()->UserBag_CountItemByIDTable(need_guid);
			need_count = pPrescr->m_pDefine->nStuff4num;
			if( (max < 0) || max > (count / need_count) ) 
				max = count / need_count;
		}
		
		if( (need_guid = pPrescr->m_pDefine->nStuff5ID) != INVALID_ID)
		{
			count = CDataPool::GetMe()->UserBag_CountItemByIDTable(need_guid);
			need_count = pPrescr->m_pDefine->nStuff5num;
			if( (max < 0) || max > (count / need_count) ) 
				max = count / need_count;
		}
		
		if(max>0)
			max /= pPrescr->m_pDefine->nResultNum;
		state->PushInteger( max );

		return 1;
	}
	INT LifeAbility::GetPrescr_Material_Hold_Count(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()) || !(args[3].IsInteger()))
		{
			KLThrow("LUA:LifeAbility:GetPrescr_Material_Hold_Count args error");
		}	

		INT nIndex = args[2].GetInteger();
		INT nMaterial = args[3].GetInteger();

		const SCLIENT_PRESCR* pPrescr = 
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr(nIndex);
		if(!pPrescr) 
		{
			KLThrow("LUA:LifeAbility:GetPrescrList_Item_Requirement NULL Item");
		}

		INT count = -1,need_guid;
		switch (nMaterial)
		{
		case 1:
			need_guid = pPrescr->m_pDefine->nStuff1ID;
			if(need_guid !=INVALID_ID)
				count = CDataPool::GetMe()->UserBag_CountItemByIDTable(need_guid);
			state->PushInteger( count );
			return 1;
			break;
		case 2:
			need_guid = pPrescr->m_pDefine->nStuff2ID;
			if(need_guid !=INVALID_ID)
				count = CDataPool::GetMe()->UserBag_CountItemByIDTable(need_guid);
			state->PushInteger( count );
			return 1;
			break;
		case 3:
			need_guid = pPrescr->m_pDefine->nStuff3ID;
			if(need_guid !=INVALID_ID)
				count = CDataPool::GetMe()->UserBag_CountItemByIDTable(need_guid);
			state->PushInteger( count );
			return 1;
			break;
		case 4:
			need_guid = pPrescr->m_pDefine->nStuff4ID;
			if(need_guid !=INVALID_ID)
				count = CDataPool::GetMe()->UserBag_CountItemByIDTable(need_guid);
			state->PushInteger( count );
			return 1;
			break;
		case 5:
			need_guid = pPrescr->m_pDefine->nStuff5ID;
			if(need_guid !=INVALID_ID)
				count = CDataPool::GetMe()->UserBag_CountItemByIDTable(need_guid);
			state->PushInteger( count );
			return 1;
			break;
		default :
			return 0;
			break;
		}
	}
	
	INT LifeAbility::GetPrescr_Explain(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()) )
		{
			KLThrow("LUA:LifeAbility:GetPrescr_Explain args error");
		}	

		INT nIndex = args[2].GetInteger();

		const SCLIENT_PRESCR* pPrescr = 
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr(nIndex);

		if(!pPrescr) 
		{
			KLThrow("LUA:LifeAbility:GetPrescr_Explain NULL Item");
		}

		if(!pPrescr->m_pDefine)
		{
			KLThrow("表没填吧");
		}

		state->PushString(pPrescr->m_pDefine->szExplain);
		return 1;
	}

	//刷新合成界面界面
	INT LifeAbility::Update_Synthesize( LuaPlus::LuaState* state) 
	{
		LuaStack args(state);
		if (! (args[2].IsInteger()) )
		{
			KLThrow("LUA: LifeAbility:Update_Synthesize Wrong Param");
		}
		INT	nAbility	= args[2].GetInteger();
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_COMPOSE_ITEM,nAbility);

		return 0;
	}

	INT LifeAbility::Open_Compose_Gem_Page( LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (! (args[2].IsInteger()) )
		{
			KLThrow("LUA: LifeAbility:Open_Compose_Gem_Page Wrong Param");
		}
		INT	nFrom = args[2].GetInteger();
		INT	nIdx = args[3].GetInteger();

		char a[10];
		itoa(nIdx, a, 10);	
		
		// 背包里物品镶嵌
		if( nFrom == 0 )
		{
			CObject_Item_Equip* pItem = (CObject_Item_Equip*)CDataPool::GetMe()->UserBag_GetItem( nIdx );

			if( !pItem ) return 0;
			if( pItem->GetItemClass() == ICLASS_EQUIP ) 
			{
				int hole = pItem->GetEquipHoleNum();

				if( hole > 0 )
				{
					std::vector< STRING > vParam;
					char szTemp[100];

					sprintf(szTemp, "0");
					vParam.push_back( szTemp );

					vParam.push_back( a );

					sprintf(szTemp, "%d", hole);
					vParam.push_back( szTemp );		

					tActionItem* pAction = CActionSystem::GetMe()->GetAction_ItemID(pItem->GetID());
					INT nActionId = -1;
					if (pAction)
					{
						nActionId = pAction->GetID();
					}
					sprintf(szTemp, "%d", nActionId );
					vParam.push_back( szTemp );

					CEventSystem::GetMe()->PushEvent( GE_OPEN_COMPOSE_GEM, vParam );

					for( int i = 0; i < hole; i++ )
					{
						STRING icon;
						pItem->GetGemIcon(i,icon);
						if( !icon.empty() )
							CEventSystem::GetMe()->PushEvent(GE_UPDATE_COMPOSE_GEM, "-2",icon.c_str(), i);
					}
				}
				else
				{
					STRING strTemp = "该装备没有孔";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
				}
			}
			else
			{
				STRING strTemp = "该物品不能镶嵌";
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
			}
		}
		// 装备里物品镶嵌
		else if( nFrom == 1 )
		{
			CObject_Item_Equip* pItem = (CObject_Item_Equip*)CDataPool::GetMe()->UserEquip_GetItem( (HUMAN_EQUIP)nIdx );
			if( pItem )
			{
				if( pItem->GetItemClass() == ICLASS_EQUIP ) 
				{
					int hole = pItem->GetEquipHoleNum();
					if( hole > 0 )
					{
						std::vector< STRING > vParam;
						char szTemp[100];

						sprintf(szTemp, "1");
						vParam.push_back( szTemp );

						vParam.push_back( a );

						sprintf(szTemp, "%d", hole);
						vParam.push_back( szTemp );
						tActionItem* pAction = CActionSystem::GetMe()->GetAction_ItemID(pItem->GetID());
						INT nActionId = -1;
						if (pAction)
						{
							nActionId = pAction->GetID();
						}
						sprintf(szTemp, "%d", nActionId );
						vParam.push_back( szTemp );

						CEventSystem::GetMe()->PushEvent( GE_OPEN_COMPOSE_GEM, vParam );
						for( int i = 0; i < hole; i++ )
						{
							STRING icon;
							pItem->GetGemIcon(i,icon);
							if( !icon.empty() )
							CEventSystem::GetMe()->PushEvent(GE_UPDATE_COMPOSE_GEM, "-2",icon.c_str(), i);
						}
					}
					else
					{
						STRING strTemp = "该装备没有孔";
						CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					}
				}
				else
				{
					STRING strTemp = "该物品不能镶嵌";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
				}
			}
		}

		return 0;
	}

	INT LifeAbility::Do_Enchase( LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if ( !args[2].IsInteger() || !args[3].IsInteger())
		{
			KLThrow("LUA:LifeAbility Do_Enchase parameter error");
		}

		INT itemType = args[2].GetInteger();
		INT itemPos = args[3].GetInteger();
	
		_ITEM_GUID itemGuid;

		// 背包
		if( itemType == 0 )
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem( itemPos );
			if( pItem )
				pItem->GetGUID( (USHORT&)itemGuid.m_World, (USHORT&)itemGuid.m_Server, (UINT&)itemGuid.m_Serial );
		}
		// 装备
		else if( itemType == 1 )
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem( (HUMAN_EQUIP)itemPos );
			if( pItem )
				pItem->GetGUID( (USHORT&)itemGuid.m_World, (USHORT&)itemGuid.m_Server, (UINT&)itemGuid.m_Serial );
		}

		int validNum = 0;
		HOLE_GEM_INFO aGemInfo[MAX_ITEM_GEM];
	
		CGUseGem msg;

		// 取得有效宝石消息
		for( int i = 0; i < MAX_ITEM_GEM; i++ )
		{
			INT itemId = args[i + 4].GetInteger();
			if( itemId > -1 )
			{
				++validNum;

				aGemInfo[i].m_nGemHolePos = i;
				aGemInfo[i].m_GemBagIndex = itemId;
				msg.SetHoleGemInfo( aGemInfo[i] );
			}
		}

		if( validNum <= 0 )
			return 0;

		msg.SetEquipPosType( (BYTE)itemType );
		msg.SetPos( (BYTE)itemPos );
		msg.setItemID( itemGuid );
		CNetManager::GetMe()->SendPacket(&msg);

		return 0;
	}

	INT LifeAbility::Do_Combine( LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if ( !args[2].IsInteger() || !args[3].IsInteger())
		{
			KLThrow("LUA:LifeAbility Do_Combine parameter error");
		}

		INT nIndexGem1 = args[2].GetInteger();
		INT nIndexGem2 = args[3].GetInteger();
/*		INT nAbility = 3;
///////////////////////////////////
		CAI_Base * pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();
		const _DBC_LIFEABILITY_DEFINE* m_pAbilityDef = (const _DBC_LIFEABILITY_DEFINE*)CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(nAbility);

		INT guidPlatform = INVALID_ID;
		if(m_pAbilityDef->nPlatformID>0)
		{
			guidPlatform = ((CAI_MySelf*)pMySelfAI)->Find_Platform(nAbility);
			///////////////////////////////////
			if(guidPlatform == INVALID_ID)
				return 0;
		}

		if( CObjectManager::GetMe()->GetMySelf()->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_ABILITY_ACTION )
			return 0;
*/		//向服务器发送

		INT guidPlatform = INVALID_ID;
		CGGemCompound Msg;

		Msg.SetGemBagIndex1(nIndexGem1);
		Msg.SetGemBagIndex2(nIndexGem2);
		Msg.SetPlatformGUID(guidPlatform);
		CNetManager::GetMe()->SendPacket(&Msg);
		return 0;
	}
	INT LifeAbility::GetEquip_Gem( LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if ( !args[2].IsInteger() || !args[3].IsInteger())
		{
			KLThrow("LUA:LifeAbility GetEquip_Gem parameter error");
		}

		INT nIndexEquip = args[2].GetInteger();
		UINT nIndexGem = args[3].GetInteger();

		tObject_Item* My_Equip = CDataPool::GetMe()->UserBag_GetItem(nIndexEquip);

		if(My_Equip->GetItemClass() != ICLASS_EQUIP) 
			return 0;
		CObject_Item_Equip::EXTRA_DEFINE* Extra = (CObject_Item_Equip::EXTRA_DEFINE*)((CObject_Item_Equip*)(CObject_Item*)My_Equip)->GetExtraInfo();
		
		CObject_Item_Equip::EQUIP_ATTACH_GEM Gems = Extra->m_vEquipAttachGem;
//		if( !Gems[nIndexGem]) 
//			return 0;
		if( nIndexGem >= Gems.size() ) return 0;
		UINT uGemType = Gems[nIndexGem].m_GemIndex;
		// 从数据库中查找到这个Gem的信息
		DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);
		//搜索纪录
		const _DBC_ITEM_GEM* pGem = (const _DBC_ITEM_GEM*)s_pItem_Gem->Search_Index_EQU(uGemType);
		if(!pGem) return 0;

		state->PushString(pGem->szIcon);
		return 1;
		
	}
	INT LifeAbility::GetEquip_GemCount( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if ( !args[2].IsInteger() )
		{
			KLThrow("LUA:LifeAbility GetEquip_GemCount parameter error");
		}

		INT nIndexEquip = args[2].GetInteger();

		tObject_Item* My_Equip = CDataPool::GetMe()->UserBag_GetItem(nIndexEquip);

		if(My_Equip->GetItemClass() != ICLASS_EQUIP) 
			return 0;
		CObject_Item_Equip::EXTRA_DEFINE* Extra = (CObject_Item_Equip::EXTRA_DEFINE*)((CObject_Item_Equip*)(CObject_Item*)My_Equip)->GetExtraInfo();
		
		CObject_Item_Equip::EQUIP_ATTACH_GEM Gems = Extra->m_vEquipAttachGem;

		state->PushInteger((int)Gems.size() );
		return 1;
	}

	INT LifeAbility::Enchase_Preparation( LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if ( !args[2].IsInteger() || !args[3].IsInteger())
		{
			KLThrow("LUA:LifeAbility Enchase_Preparation parameter error");
		}

		INT nIndexInterface = args[2].GetInteger();
		INT nIndexPacket = args[3].GetInteger();

		if(nIndexInterface == 1){
			tObject_Item* My_Equip = CDataPool::GetMe()->UserBag_GetItem(nIndexPacket);

			if(My_Equip->GetItemClass() != ICLASS_EQUIP)
			{
			//	CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,GetOResultText(OR_STUFF_LACK));
				ADDTALKMSG(GetOResultText(OR_STUFF_LACK));
				state->PushBoolean(FALSE);
				return 1;
			}

			INT ret = ((CObject_Item_Equip*)(CObject_Item*)My_Equip)->GetGemMaxCount();
			CObject_Item_Equip::EXTRA_DEFINE* Extra = (CObject_Item_Equip::EXTRA_DEFINE*)((CObject_Item_Equip*)(CObject_Item*)My_Equip)->GetExtraInfo();
			
			CObject_Item_Equip::EQUIP_ATTACH_GEM Gems = Extra->m_vEquipAttachGem;
			if(ret <= (INT)Gems.size() || ret > MAX_ITEM_GEM || (INT)Gems.size() > MAX_ITEM_GEM){
			//	CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,GetOResultText(OR_GEM_SLOT_LACK));
			//	SCRIPT_SANDBOX::Talk::s_Talk.AddSelfMessage(GetOResultText(OR_GEM_SLOT_LACK));			
				ADDTALKMSG(GetOResultText(OR_GEM_SLOT_LACK));
				state->PushBoolean(FALSE);
				return 1;
			}
			state->PushBoolean(true);
			return 1;
		}
		else if(nIndexInterface == 2)
		{
			tObject_Item* My_Gem = CDataPool::GetMe()->UserBag_GetItem(nIndexPacket);

			if(My_Gem->GetItemClass() != ICLASS_GEM)
			{
			
			//	CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,GetOResultText(OR_STUFF_LACK));
			//	SCRIPT_SANDBOX::Talk::s_Talk.AddSelfMessage(GetOResultText(OR_STUFF_LACK));			
				ADDTALKMSG(GetOResultText(OR_STUFF_LACK));
				state->PushBoolean(FALSE);
				return 1;
			}
		}else
			state->PushBoolean(FALSE);

		return 1;
	}
	INT LifeAbility::Compound_Preparation( LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if ( !args[2].IsInteger() )
		{
			KLThrow("LUA:LifeAbility Compound_Preparation parameter error");
		}

		INT nIndexPacket = args[2].GetInteger();

		tObject_Item* My_Gem = CDataPool::GetMe()->UserBag_GetItem(nIndexPacket);

		if(My_Gem->GetItemClass() != ICLASS_GEM)
		{
		//	CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,GetOResultText(OR_STUFF_LACK));
			//SCRIPT_SANDBOX::Talk::s_Talk.AddSelfMessage(GetOResultText(OR_STUFF_LACK));
			ADDTALKMSG(GetOResultText(OR_STUFF_LACK));
			state->PushBoolean(FALSE);
			return 1;
		}

		//INT Gem_Quality = ((CObject_Item_Gem*)(CObject_Item*)My_Gem)->GetQuality();
		//if(Gem_Quality >= MAX_GEM_QUALITY)
		//{
		////	CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,GetOResultText(OR_CANNOT_UPGRADE));
		//	//SCRIPT_SANDBOX::Talk::s_Talk.AddSelfMessage(GetOResultText(OR_CANNOT_UPGRADE));		
		//	ADDTALKMSG(GetOResultText(OR_CANNOT_UPGRADE));
		//	state->PushBoolean(FALSE);
		//	return 1;
		//}   //remove
		state->PushBoolean(true);
		return 1;
	}

	INT LifeAbility::Can_Enchase( LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if ( !args[2].IsInteger() || !args[3].IsInteger())
		{
			KLThrow("LUA:LifeAbility Can_Enchase parameter error");
		}

		INT nIndexEquip = args[2].GetInteger();
		INT nIndexGem = args[3].GetInteger();

		tObject_Item* My_Equip = CDataPool::GetMe()->UserBag_GetItem(nIndexEquip);
		tObject_Item* My_Gem = CDataPool::GetMe()->UserBag_GetItem(nIndexGem);

		if( (My_Equip->GetItemClass() != ICLASS_EQUIP) || (My_Gem->GetItemClass() != ICLASS_GEM) )
			return 0;
		CObject_Item_Equip::EXTRA_DEFINE* Extra = (CObject_Item_Equip::EXTRA_DEFINE*)((CObject_Item_Equip*)(CObject_Item*)My_Equip)->GetExtraInfo();
		
		CObject_Item_Equip::EQUIP_ATTACH_GEM Gems = Extra->m_vEquipAttachGem;
//		if( !Gems[nIndexGem]) 
//			return 0;
		
		INT ret = ((CObject_Item_Equip*)(CObject_Item*)My_Equip)->GetGemMaxCount();
		
		if(ret <= (INT)Gems.size() || ret > MAX_ITEM_GEM || (INT)Gems.size() > MAX_ITEM_GEM){
			//	CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,GetOResultText(OR_GEM_SLOT_LACK));
			//	SCRIPT_SANDBOX::Talk::s_Talk.AddSelfMessage(GetOResultText(OR_GEM_SLOT_LACK));			
			ADDTALKMSG(GetOResultText(OR_GEM_SLOT_LACK));
			state->PushBoolean(FALSE);
			return 1;
		}

		size_t nGemCount;
		INT find=0;
		UINT uGemType;
		if( (nGemCount= Gems.size()) > 0 ) {
			for( size_t i=0; i < nGemCount; i++)
			{
				uGemType = Gems[i].m_GemIndex;
				DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);
				//搜索纪录
				const _DBC_ITEM_GEM* pGem = (const _DBC_ITEM_GEM*)s_pItem_Gem->Search_Index_EQU(uGemType);
				if(!pGem) continue;
				if(pGem->nType == ((CObject_Item_Gem*)(CObject_Item*)My_Gem)->GetGemType() )
				{
					find = 1;
					break;
				}
			}
		}

		if(find)
		{
		//	CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,GetOResultText(OR_GEM_CONFLICT));		
			//SCRIPT_SANDBOX::Talk::s_Talk.AddSelfMessage(GetOResultText(OR_GEM_CONFLICT));		
			ADDTALKMSG(GetOResultText(OR_GEM_CONFLICT));
			state->PushBoolean(FALSE);
		}
		else
			state->PushBoolean(true);

		return 1;
	}

	INT LifeAbility::Can_Combine( LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if ( !args[2].IsInteger() || !args[3].IsInteger())
		{
			KLThrow("LUA:LifeAbility Can_Combine parameter error");
		}

		INT nIndexGem1 = args[2].GetInteger();
		INT nIndexGem2 = args[3].GetInteger();

		tObject_Item* My_Gem1 = CDataPool::GetMe()->UserBag_GetItem(nIndexGem1);
		tObject_Item* My_Gem2 = CDataPool::GetMe()->UserBag_GetItem(nIndexGem2);

		if( (My_Gem1->GetItemClass() != ICLASS_GEM)  || (My_Gem2->GetItemClass() != ICLASS_GEM) )
			return 0;

		if(((CObject_Item_Gem*)(CObject_Item*)My_Gem1)->GetIdTable() == ((CObject_Item_Gem*)(CObject_Item*)My_Gem2)->GetIdTable())
		{
			state->PushBoolean(true);
		}
		else
		{
		//	CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,GetOResultText(OR_STUFF_LACK));		
			//SCRIPT_SANDBOX::Talk::s_Talk.AddSelfMessage(GetOResultText(OR_STUFF_LACK));		
			ADDTALKMSG(GetOResultText(OR_STUFF_LACK));
			state->PushBoolean(FALSE);
		}
		return 1;
	}

	INT LifeAbility::Lock_Packet_Item( LuaPlus::LuaState* state )
	{

		LuaStack args(state);

		//检查参数
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA: Lock_Packet_Item Wrong Param1");
		}
		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA: Lock_Packet_Item Wrong Param2");
		}

		INT nIndex = args[2].GetInteger();
		
		if(nIndex >= 0)
		{
			tObject_Item* pObj = CDataPool::GetMe()->UserBag_GetItem(nIndex);
			if(pObj)
			{
				if(args[3].GetInteger() == 1)
					pObj->SetLock(true);
				else
					pObj->SetLock(false);
			}
		}
		CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED,-1);

		return 0;
	}
	// 生活技能界面关心的Npc
	INT LifeAbility::GetNpcId( LuaPlus::LuaState* state )
	{
		INT nServerId = CDataPool::GetMe()->StudyAbility_GetNpcId();

		CObject* pObj = (CObject*)CObjectManager::GetMe()->FindServerObject(nServerId);
		if(!pObj)
		{
			state->PushInteger(-1);
			return 1;
		}
		state->PushInteger(pObj->GetID());

		return 1;
	}
	
	INT LifeAbility::GetGemActionInfo( LuaPlus::LuaState* state )
	{
		state->PushInteger(0);
		return 1;
	}
	INT LifeAbility::SetPrecalcInfo( LuaPlus::LuaState* state )
	{
		return 1;
	}

	INT LifeAbility::GetPrescrList_Item_RequirementData(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()) )
		{
			KLThrow("LUA:GetPrescrList_Item_RequirementData Param1");
		}	
		INT nIdTable = args[2].GetInteger();

		CObject_Item* pItem = m_Requirement[nIdTable];
		tActionItem* pAction;
		if ( !pItem )
		{
			pItem = CObject_Item::NewItem( nIdTable );
			//pItem->SetGUID(0,0,0);
			pItem->SetTypeOwner( tObject_Item::IO_QUESTVIRTUALITEM );
			pItem->SetNumber(1);
			CActionSystem::GetMe()->CreateObjectItemByTableId(pItem);
			m_Requirement[nIdTable] = pItem;
		}
		pAction = CActionSystem::GetMe()->GetAction_ItemID( pItem->GetID() );

		if( !pAction)
		{
			state->PushInteger(-1);
			state->PushString("");
		}
		else
		{
			state->PushInteger( pAction->GetID() );
			state->PushString(pAction->GetName() );
		}
		
		return 2;
	}

	INT LifeAbility::GetRequirementNum_ById(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()) )
		{
			KLThrow("LUA:GetRequirementNum_ById Param1");
		}	
		INT nIdTable = args[2].GetInteger();

		INT num = SouXiaLu::s_SouXiaLu.GetItemNum_ByIdTable(nIdTable);

		state->PushInteger(num);

		return 1;
	}
	
};
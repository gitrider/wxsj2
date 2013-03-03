
/**	GMGameInterface_Script_SouXia.cpp
 *	
 *	功能：	搜侠录相关接口
 *			
 */

#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "GMGameInterface_Script_SouXia.h"

#include "Procedure/GamePro_Login.h"
#include "NetWork/NetManager.h"
#include "LuaPlus.h"
#include "Global.h"

#include "Event/GMEventSystem.h"
#include "GIException.h"
#include "Action/GMActionSystem.h"
#include "DataPool/GMUIDataPool.h"
#include "DataPool/GMDataPool.h"
#include "Object/Manager/ObjectManager.h"
#include "Object/Logic/Object.h"
#include "Object/Logic/Character/Obj_Character.h"
#include "Object/Logic/Character/Obj_PlayerMySelf.h"
#include "Input/GMInputSystem.h"
#include "world/worldmanager.h"
#include "GIDbc_struct.h"
#include "object/Logic/character/obj_playermyself.h"
#include "GIUtil.h"
#include "ScriptCommandStruct.h"
#include "DBC\GMDataBase.h"
#include "DataPool\GMDP_CharacterData.h"
#include "GMInterface_Script_Tooltip.h"
#include "world/scene.h"
#include "Procedure/GamePro_Enter.h"
#include "Procedure\GameProcedure.h"
#include "Procedure\GamePro_main.h"
#include "GIUISystem.h"
#include "Sound\GMSoundSystem.h"

#include "Object\Logic\Item\Obj_Item_Equip.h"
#include "Object\ObjectCommandDef.h"
#include "GIGfxSystem.h"// 发送设置2级密码
#include "Action\GMActionSystem_MouseCmd.h"
#include "../Object/Manager/FakeObjectEntityManager.h"

#include "Engine/EngineInterface.h"

#include "GameStruct.h"
#include "DataPool/GMDataPool.h"
#include "FakeObject/GMFakeObjSystem.h"
#include "..\Action\GMActionSystem_Skill.h"


#include "Packets/CGSkillUpgradeBySouXia.h"
#include "Packets/CGItemCreate.h"
#include "Packets/CGSouXiaDismantle.h"
#include "Packets/CGSouXiaDiscard.h"
#include "Packets/CGSouXiaRefining.h"
#include "Packets/CGSouXiaSew.h"
#include "..\..\..\Common\Utilities\SplitStringLevelOne.h"


namespace SCRIPT_SANDBOX
{
	SouXiaLu SouXiaLu::s_SouXiaLu;
	LuaPlus::LuaObject* SouXiaLu::s_pMetaTable = NULL;



	INT SouXiaLu::Lua_IsEquipSoiuXia( LuaPlus::LuaState* state )
	{
		if ( IsEquipSouXia() )
			state->PushInteger(1);
		else
			state->PushInteger(-1);

		return 1;
	}
	
	INT SouXiaLu::Lua_GetSouXiaUsbNum(LuaPlus::LuaState *state)
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) ) //return 0;
		{
			KLThrow("LUA: Lua_GetSouXiaUsbNum Param1");
		}
		
		INT type = args[2].GetInteger();

		int  num = 0;
		m_vPageUsbID.clear();
		for(INT i=0; i<MAX_BAG_SIZE; i++)
		{
			if( IsSouXiaUsbItem( type, i ) )
			{
				num++;
				m_vPageUsbID.push_back( i );
			}
		}
		state->PushInteger( num );

		return 1;
	}

	INT SouXiaLu::Lua_GetSouXiaUsbItem( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if( !(args[2].IsInteger()) )// return 0;
		{
			KLThrow("LUA: Lua_GetSouXiaUsbItemWrong Param1");
		}
		if( !(args[3].IsInteger()) ) //return 0;
		{
			KLThrow("LUA: Lua_GetSouXiaUsbItemWrong Param2");
		}

		int pageNum = args[2].GetInteger();	//当前在显示栏第几页
		int pagePos = args[3].GetInteger();	//显示栏的第几个位置

		int num = (pageNum-1)*12 + pagePos;	//根据页数和位置得出是背包内的第几个符合条件的道具 
		int vSize =  m_vPageUsbID.size();
		if ( num <= vSize )		//容器中有这个物品
		{
			int bagpos = m_vPageUsbID[num-1];

			tActionItem* pActionItem = CActionSystem::GetMe()->EnumAction(bagpos, NAMETYPE_PACKAGEITEM);
			if(pActionItem) 
			{
				LuaObject objReturn = state->BoxPointer(pActionItem);
				objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
				objReturn.PushStack();
				state->PushInteger( bagpos );
				return 2;
			}
		}
		else
		{
			LuaObject objReturn = state->BoxPointer(&(CActionItem::s_InvalidAction));
			objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
			objReturn.PushStack();
			state->PushInteger(-1);
			return 2;
		}
	
		return 0;
	}

	INT SouXiaLu::Lua_StudyByUSBItem(LuaPlus::LuaState *state)
	{
		LuaStack args(state);
		if( !(args[2].IsInteger()) ) //return 0;
		{
			KLThrow("LUA: Lua_StudyByUSBItem Param1");
		}
		if( !(args[3].IsInteger()) ) //return 0;
		{
			KLThrow("LUA: Lua_StudyByUSBItem Param2");
		}

		//if( args[3].GetInteger() == -1 )
		//{
		//	CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, "请选择正确的目标进行学习，谢谢!~_~" );

		//	return 0;
		//}
		BYTE	SkillType = args[2].GetInteger();
		BYTE BagIndex = args[3].GetInteger();
		CGSkillUpgradeBySouXia msg;
		msg.SetStudySkillType( SkillType );
		msg.SetBagIndex( BagIndex );
		CNetManager::GetMe()->SendPacket( &msg );

		return 0;
	}

	INT SouXiaLu::Lua_GetSouXiaPageNum_Type(LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) ) //return 0;
		{
			KLThrow("LUA: Lua_GetSouXiaPageNum_Type Param1");
		}
		
		int pageType = args[2].GetInteger();
		
		tObject_Item* pItem = GetHEquipSouxia();
		int id = pItem->GetIdTable();
		const tDataBase* pdbcbase = CDataBaseSystem::GetMe()->GetDataBase( DBC_SOUXIA_DATA );
		const _DBC_SOUXIA_DATA* pLine = (const _DBC_SOUXIA_DATA*)((tDataBase*)pdbcbase)->Search_Index_EQU(id);

		int num = -1;
		switch( pageType )
		{
		case 1:
			num = pLine->nSouxiaParam1_SkillPageNum;
			break;

		case 2:
			num = pLine->nSouxiaParam2_ArmPageNum;
			break;

		case 3:
			num = pLine->nSouxiaParam3_PetPageNum;
			break;

		case 4:
			num = pLine->nSouxiaParam4_RidePageNum;
			break;
		}
		state->PushInteger( num );

		return 1;
	}
	

	INT SouXiaLu::Lua_GetSouXiaPageNum( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow("LUA: Lua_GetSouXiaPageNum Param1");
		}
		INT index = args[2].GetInteger();
		INT pageNum = 1;
		SOUXIA_DATA* data = GetHEquipSouxiaInfo();
		if ( !data )
		{
			state->PushInteger( 1 );
			return 1;
		}
		switch( index )
		{
		case 1:		//技能
			{
				INT num = data->GetCurSkillCount();
				pageNum = (num-1)/3+1;
			}
			break;

		case 2:
			{
				INT num = data->GetCurProductCount();
				if( num != 0 )
					pageNum = num;
			}
			break;

		case 3:
			{
				INT num = data->GetCurPetCount();
				pageNum = (num-1)/8 + 1;
			}
			break;

		case 4:
			{
				INT num = data->GetCurZuoJiCount();
				pageNum = (num-1)/8 + 1;
			}
			break;
		}
		state->PushInteger( pageNum );
		return 1;
	}


	INT SouXiaLu::Lua_GetSouXiaLevel(LuaPlus::LuaState *state)
	{
		tObject_Item* pItem = GetHEquipSouxia();
		if( !pItem ) return 0;
		int id = pItem->GetIdTable();
		const tDataBase* pdbcbase = CDataBaseSystem::GetMe()->GetDataBase( DBC_SOUXIA_DATA );
		const _DBC_SOUXIA_DATA* pLine = (const _DBC_SOUXIA_DATA*)((tDataBase*)pdbcbase)->Search_Index_EQU(id);

		INT dLevel = pLine->nSouxiaLevel;

		state->PushInteger( dLevel );

		return 1;
	}
	
	INT SouXiaLu::Lua_GetSouXiaIndex(LuaPlus::LuaState* state )
	{
		tObject_Item* pItem = GetHEquipSouxia();
		if( !pItem ) return 0;

		state->PushInteger( pItem->GetExtraParam1() );
		
		return 1;
	}

	INT SouXiaLu::Lua_GetSouXiaSkillID( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) ) //return 0;
		{
			KLThrow("LUA: Lua_GetSouXiaSkillID Param1");
		}
		if ( !(args[3].IsInteger()) ) //return 0;
		{
			KLThrow("LUA: Lua_GetSouXiaSkillID Param2");
		}
		if ( !(args[4].IsInteger()) )
		{
			KLThrow("LUA: Lua_GetSouXiaSkillID Param3");
		}

		INT pageNum = args[2].GetInteger();
		INT Index = args[3].GetInteger();

		//获得技能的类型ID，1.普通技能  2.神兽召唤技能  3.坐骑召唤技能
		INT SkillType = args[4].GetInteger();		

		SOUXIA_DATA* data = GetHEquipSouxiaInfo();

		if( !data )
		{
			state->PushInteger( -1 );
			return 1;
		}
		
		switch( SkillType)
		{
		case 1:	//普通技能
			{
				INT SkillCount = data->GetCurSkillCount();
				INT num = 3*(pageNum-1)+Index;
				if( SkillCount < num )
					state->PushInteger( -1 );
				else
				{
					INT Id = data->m_Skill.StudySkillId[num-1];
					//state->PushInteger( CActionSystem::GetMe()->GetAction_SkillID( Id )->GetID() );
					state->PushInteger( Id );
					return 1;
				}
			}
			break;

		case 2:	//神兽召唤技能
			{
				INT PetCount = data->GetCurPetCount();
				INT num = 8*(pageNum-1)+Index;
				if( PetCount < num )
					state->PushInteger(-1);
				else
				{
					INT Id = data->m_Pet.StudyPet[num-1].StudyZhaoHuan;
					INT PetNum = data->m_Pet.StudyPet[num-1].LeftUseTime;
					state->PushInteger( Id );
					state->PushInteger(PetNum);
					return 2;
				}
			}
			break;

		case 3:	//坐骑召唤技能
			{
				INT ZuojiCount = data->GetCurZuoJiCount();
				INT num = 8*(pageNum-1)+Index;
				if( ZuojiCount < num )
					state->PushInteger(-1);
				else
				{
					INT Id = data->m_ZuoJi.StudyZuoji[num-1].StudyZhaoHuan;
					INT RideNum = data->m_ZuoJi.StudyZuoji[num-1].LeftUseTime;
					state->PushInteger( Id );
					state->PushInteger(RideNum);
					return 2;
				}
			}
			break;
		}
		state->PushInteger(-1);
		return 1;
	}

	INT SouXiaLu::Lua_GetSouXiaActionIDBySkillID( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) ) //return 0;
		{
			KLThrow("LUA: Lua_GetSouXiaActionIDBySkillID Param1");
		}
		INT SkillID = args[2].GetInteger();
		if( SkillID == -1 )
		{
			state->PushInteger( -1 );
			return 1;
		}

		tActionItem* pItem = CActionSystem::GetMe()->GetAction_SkillID(SkillID);
		if ( !pItem )
		{
			state->PushInteger( -1 );
			return 1;
		}
		//if ( args[3].IsInteger() )
		//{
		//	INT SkillNum = args[3].GetInteger();
		//	tObject_Item* tobj = (tObject_Item*)(pItem->GetImpl());
		//	if(pItem)
		//	tobj->SetNumber(SkillNum);

		//}

		INT Id = pItem->GetID();	
		state->PushInteger( Id );

		return 1;
	}

	INT SouXiaLu::Lua_GetSouXiaSkillName( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow("LUA:Lua_GetSouXiaSkillName Param1");
		}

		INT SkillID = args[2].GetInteger();

		if( SkillID == -1 )
		{
			state->PushString( "" );
			return 1;
		}

		const tDataBase* pdbcbase = CDataBaseSystem::GetMe()->GetDataBase( DBC_SKILL_DATA );
		const _DBC_SKILL_DATA* pLine = (const _DBC_SKILL_DATA*)((tDataBase*)pdbcbase)->Search_Index_EQU(SkillID);
		if( !pLine )
		{
			state->PushString( "" );
			return 1;
		}

		const CHAR* name = pLine->m_lpszName;
		state->PushString( name );

		return 1;

	}

	INT SouXiaLu::Lua_GetSouXiaArmID( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow( "LUA:Lua_GetSouXiaArmID Param1" );
		}

		INT Index = args[2].GetInteger();

		SOUXIA_DATA* data = GetHEquipSouxiaInfo();

		if( !data || Index < 1 )
		{
			state->PushInteger( -1 );
			return 1;
		}
		INT ProID = data->m_Product.StudyProductId[Index-1];
		state->PushInteger( ProID );

		return 1;
	}

	INT SouXiaLu::Lua_GetSouXiaArmIcon( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow( "LUA:Lua_GetSouXiaArmIcon Param1" );
		}
		INT Index = args[2].GetInteger();

		const tDataBase* pdbcbase = CDataBaseSystem::GetMe()->GetDataBase( DBC_EQUIP_PRODUCT_INFO );
		const _DBC_EQUIP_PRODUCT_INFO* pLine = (const _DBC_EQUIP_PRODUCT_INFO*)((tDataBase*)pdbcbase)->Search_Index_EQU(Index);
		
		INT ProId = pLine->nProductId;

		const tDataBase* pdbcbase2 = CDataBaseSystem::GetMe()->GetDataBase( DBC_ITEM_MEDIC );
		const _DBC_ITEM_MEDIC* pLine2 = (const _DBC_ITEM_MEDIC*)((tDataBase*)pdbcbase)->Search_Index_EQU(ProId);

		const CHAR* strIcon = pLine2->szIcon;
		
		state->PushString( strIcon );
		state->PushString( pLine2->szName );
		state->PushString( pLine2->szDesc );

		return 3;
	}

	INT SouXiaLu::Lua_GetSouXiaArmData( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow( "LUA:Lua_GetSouXiaArmData Param1" );
		}
		INT Index = args[2].GetInteger();

		if ( Index != -1 )
		{
			const tDataBase* pdbcbase = CDataBaseSystem::GetMe()->GetDataBase( DBC_EQUIP_PRODUCT_INFO );
			const _DBC_EQUIP_PRODUCT_INFO* pLine = (const _DBC_EQUIP_PRODUCT_INFO*)((tDataBase*)pdbcbase)->Search_Index_EQU(Index);
			if ( pLine )
			{
				INT ProId = pLine->nProductId;
				CObject_Item* pItem = m_ProItem[ProId];
				tActionItem* pAction;
				//tActionItem* pAction = CActionSystem::GetMe()->GetAction_ItemIDTable_FromMyPacket(ProId);
				if ( !pItem )
				{
					pItem = CObject_Item::NewItem( ProId );
					pItem->SetGUID(0,0,0);
					//newElement->pData->setContainer(CT_TRANSFER_ITEM);
					pItem->SetTypeOwner( tObject_Item::IO_QUESTVIRTUALITEM );
					pItem->SetNumber(1);
					CActionSystem::GetMe()->CreateObjectItemByTableId(pItem);
					m_ProItem[ProId] = pItem;
				}
				pAction = CActionSystem::GetMe()->GetAction_ItemID( pItem->GetID() );
				state->PushInteger( pAction->GetID() );
				state->PushString( pAction->GetName() );
				state->PushString( pAction->GetDesc() );
				return 3;
			}	
		}		
		
		state->PushInteger( -1 );
		state->PushString( "" );
		state->PushString( "" );
		return 3;
	}

	INT SouXiaLu::Lua_GetSouXiaArmMatData( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow( "LUA:Lua_GetSouXiaArmData Param1" );
		}
		if ( !(args[3].IsInteger()) )
		{
			KLThrow( "LUA:Lua_GetSouXiaArmData Param2" );
		}
		INT Index = args[2].GetInteger();		//第几个配方
		INT MatIndex = args[3].GetInteger();		//第几个材料

		SOUXIA_DATA* data = GetHEquipSouxiaInfo();
		if( !data || Index < 1)
		{
			return 0;
		}

		INT ProID = data->m_Product.StudyProductId[Index-1];		//配方ID
		if ( ProID == -1 ) return 0;

		const tDataBase* pdbcbase = CDataBaseSystem::GetMe()->GetDataBase( DBC_EQUIP_PRODUCT_INFO );
		const _DBC_EQUIP_PRODUCT_INFO* pLine = (const _DBC_EQUIP_PRODUCT_INFO*)((tDataBase*)pdbcbase)->Search_Index_EQU(ProID);

		if( !pLine ) return 0;
		
		INT MatNum = pLine->szMat[MatIndex-1].nMaterialNum;	//该材料的所需要的数量

		INT MatId = GetMatIndex( pLine->szMat[MatIndex-1].nMaterial, pLine->szMat[MatIndex-1].nMatLowestLv );
		if( MatId == -1 ) return 0;
		INT BagMatNum = GetItemNum_ByIdTable( MatId );
		
		CObject_Item* pItem = m_ProItem[MatId];
		tActionItem* pAction;
		if ( !pItem )
		{
			pItem = CObject_Item::NewItem( MatId );
			//pItem->SetGUID(0,0,0);
			pItem->SetTypeOwner( tObject_Item::IO_QUESTVIRTUALITEM );
			pItem->SetNumber(1);
			CActionSystem::GetMe()->CreateObjectItemByTableId(pItem);
			m_ProItem[MatId] = pItem;
		}
		pAction = CActionSystem::GetMe()->GetAction_ItemID( pItem->GetID() );
		
		state->PushInteger( pAction->GetID() );
		state->PushString( pAction->GetName() );
		state->PushInteger( MatNum );
		state->PushInteger( BagMatNum );

		return 4;
	}

	INT SouXiaLu::Lua_GetSouXia_Data( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow( "LUA:Lua_GetSouXia_Data Param1" );
		}
		if ( !(args[3].IsInteger()) )
		{
			KLThrow( "LUA:Lua_GetSouXia_Data Param2" );
		}
		INT type = args[2].GetInteger();
		INT BagPos = args[3].GetInteger();
		INT Index = GetSouXiaUsb_Data( BagPos );
		switch( type )
		{
		case 2:
			{
				const tDataBase* pdbcbase = CDataBaseSystem::GetMe()->GetDataBase( DBC_EQUIP_PRODUCT_INFO );
				const _DBC_EQUIP_PRODUCT_INFO* pLine = (const _DBC_EQUIP_PRODUCT_INFO*)((tDataBase*)pdbcbase)->Search_Index_EQU(Index);
				if( !pLine )
				{
					state->PushInteger(-1);
					return 1;
				}

				INT ProId = pLine->nProductId;
				state->PushInteger(ProId);
				return 1;
			}
		case 1:
		case 3:
		case 4:
			{
				state->PushInteger( Index );
				return 1;
			}
		}

		state->PushInteger( -1 );
		return 1;
	}

	INT SouXiaLu::Lua_SetPrewSkill( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow( "LUA:Lua_SetPrewSkill Param1" );
		}
		INT id = args[2].GetInteger();

		CObjectManager::GetMe()->GetMySelf()->SetSouXiaUseSkill_id( id );

		return 0;
	}
	
	INT SouXiaLu::Lua_SetPrewArm( LuaPlus::LuaState* state )
	{
		LuaStack args( state );
		if ( !(args[2].IsInteger()) )
		{
			KLThrow( "LUA:Lua_SetPrewArm Param1" );
		}

		INT ArmId = args[2].GetInteger();

		//打开数据表equip_algo.tab
		DBC_DEFINEHANDLE( s_pItem_Visual, DBC_ITEM_EQUIP );
		//搜索纪录
		const _DBC_ITEM_EQUIP* pVisual = NULL;
		pVisual = ( const _DBC_ITEM_EQUIP* )s_pItem_Visual->Search_Index_EQU(ArmId);

		if( !pVisual ) return 0;

		CObjectManager::GetMe()->GetMySelf()->SetSouXiaPrewArm( ArmId, pVisual->nEquipPoint );

		////打开数据表
		//DBC_DEFINEHANDLE(s_pItem_Visual, DBC_ITEM_EQUIP_VISUAL);
		////搜索纪录
		//const _DBC_ITEM_EQUIP_VISUAL* pVisual =NULL;
		//pVisual =(const _DBC_ITEM_EQUIP_VISUAL*)s_pItem_Visual->Search_Index_EQU( ArmId );

		//if( !pVisual ) return 0;

		//INT ModelId = pVisual->nModelID;

		//DBC_DEFINEHANDLE(s_pWeaponItem, DBC_ITEM_VISUAL_LOCATOR);
		//const _DBC_ITEM_VISUAL_LOCATOR* pEquipVisual = (_DBC_ITEM_VISUAL_LOCATOR*)s_pWeaponItem->Search_Index_EQU( ModelId );
		//if( !pEquipVisual )
		//	return 0;


		//if( pEquipVisual->pObjFile_Right && pEquipVisual->pObjFile_Right[0] != '\0')
		//{
		//	//ModelName = pEquipVisual->pObjFile_Right;
		//	CObjectManager::GetMe()->GetMySelf()->SetSouXiaPrewArm(ModelId,WL_RIGHT );
		//}
		//else if( pEquipVisual->pObjFile_Left && pEquipVisual->pObjFile_Left[0] != '\0' )
		//{
		//	//ModelName = pEquipVisual->pObjFile_Left;
		//	CObjectManager::GetMe()->GetMySelf()->SetSouXiaPrewArm(ModelId,WL_LEFT );
		//}
		////if( ModelName.empty() ) return 0;

		////tEntityNode* pMountRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_ACTOR);
		////pMountRenderInterface->SetData( m_pAvatarArm->GetID() );
		////pMountRenderInterface->Actor_SetFile( ModelName.c_str() );

		return 0;
	}


	INT SouXiaLu::Lua_SetPrewPet( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow( "LUA:Lua_SetPrewPet Param1" );
		}
		INT modelId = args[2].GetInteger();

		CObjectManager::GetMe()->GetMySelf()->SetSouXiaPrewPet(modelId);

		return 0;
	}


	INT SouXiaLu::Lua_SetPrewZuoJi( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow( "LUA:Lua_SetPrewPet Param1" );
		}
		INT modelId = args[2].GetInteger();

		CObjectManager::GetMe()->GetMySelf()->SetSouXiaPrewZuoJi(modelId);

		return 0;
	}
	

	INT SouXiaLu::Lua_ClearPrewArm( LuaPlus::LuaState* state )
	{
		CObjectManager::GetMe()->GetMySelf()->ClearSouXiaPrewArm();

		return 0;
	}


	INT SouXiaLu::Lua_InitModelAvart( LuaPlus::LuaState* state )
	{
		//m_pAvatarArm =  (CObject_PlayerOther*)(CFakeObjSystem::GetMe()->NewFakeObject("CObject_PlayerOther", "PlayArm", "Camera_Main"));
		
		return 0;
	}
	

	INT SouXiaLu::Lua_SouXiaLu_ArmMake( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow( "LUA:Lua_SouXiaLu_ArmMake Param1" );
		}
		INT ProId = args[2].GetInteger();

		CGItemCreate msg;
		//Product_info.tab
		DBC_DEFINEHANDLE(s_pItem_Equip, DBC_EQUIP_PRODUCT_INFO);
		//搜索纪录
		const _DBC_EQUIP_PRODUCT_INFO* pGEquipProduct = (const _DBC_EQUIP_PRODUCT_INFO*)s_pItem_Equip->Search_Index_EQU(ProId);
	
		if( !pGEquipProduct ) return 0;
		INT ValidNum = 0;

		CREATEITEM_MATERIAL_INFO material[MAX_EQUIP_PROMATL_NUM];
		for (INT i= 0 ;i< MAX_EQUIP_PROMATL_NUM ; ++i) 
		{
			if ( pGEquipProduct->szMat[i].nMaterial != -1 && pGEquipProduct->szMat[i].nMatLowestLv != -1 )
			{
				material[i].m_nMaterialType = (pGEquipProduct->szMat[i].nMaterial);
				material[i].m_nMaterialLevel = (pGEquipProduct->szMat[i].nMatLowestLv);
				ValidNum++;
				msg.SetMaterialReq(material[i]);
			}
		}
		msg.SetItemIndex( pGEquipProduct->nProductId );
		msg.SetValidNum( ValidNum );
		msg.SetUniqueID(pGEquipProduct->nFormulaId);
		
		CNetManager::GetMe()->SendPacket(&msg);
		
		return 0;
	}


	INT SouXiaLu::Lua_IsBlankSouXiaItem( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow( "LUA:Lua_IsBlankSouXiaItem Param1" );
		}
		INT pos = args[2].GetInteger();

		if( !IsSouXiaItem(pos,0) )
		{
			state->PushInteger(-1);//不是搜侠录道具
			return 1;
		}
		if( IsBlankSouXiaItem(pos) )
			state->PushInteger(1);		//是空白搜侠录
		else
			state->PushInteger(0);		//不是空白搜侠录
		return 1;
	}

	INT SouXiaLu::Lua_GetSkillIdByActionID( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow( "LUA:Lua_GetSkillIdByActionID Param1" );
		}
		INT ActionID = args[2].GetInteger();
		//CActionItem_Skill* pActionItem = (CActionItem_Skill*)(CActionSystem::GetMe()->GetAction(ActionID));
		//if( !pActionItem )
		//{
		//	state->PushInteger(-1);
		//	return 1;
		//}
		//const SCLIENT_SKILL* pSkill = (const SCLIENT_SKILL*)pActionItem->GetImpl();
		//INT SkillID = pSkill->m_pDefine->m_nID;
		//state->PushInteger(SkillID);

		INT SkillID = GetSkillIdByActionID(ActionID);
		state->PushInteger(SkillID);

		return 1;
	}


	INT SouXiaLu::Lua_GetModelIDbySkillID( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow( "LUA:Lua_GetModelIDbySkillID Param1" );
		}
		INT SkillID = args[2].GetInteger();
		
		INT ModelID = GetModelIDbySkillID(SkillID);

		state->PushInteger(ModelID);

		return 1;
	}


	INT SouXiaLu::Lua_SouXiaLu_Dismantle_Accept( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow( "LUA:Lua_SouXiaLu_Dismantle_Accept Param1" );
		}
		if ( !(args[3].IsInteger()) )
		{
			KLThrow( "LUA:Lua_SouXiaLu_Dismantle_Accept Param2" );
		}

		INT DismantleSouXia = args[2].GetInteger();
		INT DismantleItem = args[3].GetInteger();

		CGSouXiaDismantle msg;
		msg.SetSouXiaItemIndex(DismantleSouXia);
		msg.SetDismantleItemIndex(DismantleItem);
		CNetManager::GetMe()->SendPacket(&msg);

		return 0;
	}


	INT SouXiaLu::Lua_SouXiaLu_Destory_Accept( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow( "LUA:Lua_SouXiaLu_Destory_Accept Param1" );
		}
		if ( !(args[3].IsInteger()) )
		{
			KLThrow( "LUA:Lua_SouXiaLu_Destory_Accept Param2" );
		}

		INT BagPos = args[2].GetInteger();
		INT NpcId = args[3].GetInteger();

		CGSouXiaDiscard msg;
		msg.SetOpt(0 );
		msg.SetBagIndex( BagPos );
		msg.SetUniqueNpc( NpcId );
		CNetManager::GetMe()->SendPacket(&msg);

		return 0;
	}


	INT SouXiaLu::Lua_SouXiaLu_Complex_Accept( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow( "LUA:Lua_SouXiaLu_Complex_Accept Param1" );
		}
		if ( !(args[3].IsInteger()) )
		{
			KLThrow( "LUA:Lua_SouXiaLu_Complex_Accept Param2" );
		}
		if ( !(args[4].IsInteger()) )
		{
			KLThrow( "LUA:Lua_SouXiaLu_Complex_Accept Param3" );
		}
		INT ItemBagPos1 = args[2].GetInteger();	//搜侠录1的索引
		INT ItemBagPos2 = args[3].GetInteger();	//搜侠录2的索引
		INT CommBagPos = args[4].GetInteger();	//炼化道具的索引

		CGSouXiaRefining msg;
		msg.SetRefiningSouXiaIndex(ItemBagPos1);
		msg.SetRefiningSouXia2Index( ItemBagPos2 );
		msg.SetRefiningItemIndex(CommBagPos);
		CNetManager::GetMe()->SendPacket(&msg);

		return 0;
	}


	INT SouXiaLu::Lua_SouXiaLu_Sew_Accept( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow( "LUA:Lua_SouXiaLu_Sew_Accept Param1" );
		}
		if ( !(args[3].IsInteger()) )
		{
			KLThrow( "LUA:Lua_SouXiaLu_Sew_Accept Param2" );
		}
		if ( !(args[4].IsInteger()) )
		{
			KLThrow( "LUA:Lua_SouXiaLu_Sew_Accept Param3" );
		}
		INT  SouXiaItemIndex = args[2].GetInteger(); // 捜侠录在背包中的索引
		INT  RelicItemIndex = args[3].GetInteger();  // 残页在背包中的索引
		INT  BagItemIndex = args[4].GetInteger();    // 缝合道具索引

		CGSouXiaSew msg;
		msg.SetSewItemIndex(BagItemIndex);
		msg.SetRelicItemIndex(RelicItemIndex);
		msg.SetSewSouXiaIndex(SouXiaItemIndex);
		CNetManager::GetMe()->SendPacket(&msg);

		return 0;
	}


	INT SouXiaLu::Lua_SouXia_SetLock( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow( "LUA:Lua_SouXia_SetLock Param1" );
		}
		if ( !(args[3].IsInteger()) )
		{
			KLThrow( "LUA:Lua_SouXia_SetLock Param2" );
		}

		BOOL lock = false;
		if( args[3].GetInteger() == 1 )
			lock = true;

		SetLockSouXiaItem( args[2].GetInteger(), lock );

		return 0;
	}


	INT SouXiaLu::Lua_Clear_SouXiaSkill( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow( "LUA:Lua_Clear_SouXiaSkill Param1" );
		}
		if ( !(args[3].IsInteger()) )
		{
			KLThrow( "LUA:Lua_Clear_SouXiaSkill Param2" );
		}
		INT nSkillId = args[2].GetInteger();
		INT nIndex = args[3].GetInteger();

		if( !IsSouXiaSkill(nSkillId) )return 0;

		CActionSystem::GetMe()->MainMenuBar_Set(nIndex, -1);

		return 0;
	}


	INT SouXiaLu::Lua_SetMainMenuBarSouXiaSkillNum( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow( "LUA:Lua_SetMainMenuBarSouXiaSkillNum Param1" );
		}
		if ( !(args[3].IsInteger()) )
		{
			KLThrow( "LUA:Lua_SetMainMenuBarSouXiaSkillNum Param2" );
		}
		INT ActionId = args[2].GetInteger();
		INT Index = args[3].GetInteger();

		SetMainMenuBarSouXiaSkillNum( ActionId, Index );
		return 0;
	}


	INT SouXiaLu::Lua_SouXia_NpcId( LuaPlus::LuaState* state )
	{
		int id = CUIDataPool::GetMe()->GetCurDialogNpcId();
		//CObject_Character* pObj = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject(id)) ;
		//LPCTSTR name = pObj->GetNameWithColor().c_str();

		state->PushInteger(id);
		//state->PushString(name);

		return 1;
	}
	

	BOOL SouXiaLu::IsEquipSouXia()
	{
		tActionItem* pActionItem = CActionSystem::GetMe()->EnumAction(HEQUIP_SOUXIA, NAMETYPE_EQUIP);
		if ( !pActionItem) return false;
		
		return true;
	}
	
	BOOL SouXiaLu::IsSouXiaUsbItem(INT type, INT pos)
	{
		tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem( pos );
		return IsSouXiaUsbItem( type, pItem );
	}

	BOOL SouXiaLu::IsSouXiaUsbItem(INT type, tObject_Item *pItem)
	{
		if( pItem == NULL ) return false;	//物品不存在

		int itemclass = pItem->GetItemClass();
		int itemtype = pItem->GetItemTableType();
		if( pItem->GetItemClass()!=ICLASS_COMITEM || pItem->GetItemTableType()!=COMITEM_SOUXIA )
			return false;		//不是搜侠录物品道具

		int id = pItem->GetIdTable();
		const tDataBase* pdbcbase = CDataBaseSystem::GetMe()->GetDataBase( DBC_SOUXIA_DATA );
		const _DBC_SOUXIA_DATA* pLine = (const _DBC_SOUXIA_DATA*)((tDataBase*)pdbcbase)->Search_Index_EQU(id);

		int Souxiatype = pLine->nType;
		int SouxiaUsbType = pLine->nSouxiaUsbType;

		if( Souxiatype != 1 )return false;	//不是USB记忆棒道具

		if( type == 0 ) return true;

		if( type == SouxiaUsbType ) return true;

		return false;
	}

	BOOL SouXiaLu::IsSouXiaItem(INT pos, INT type)
	{
		tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem( pos );
		if( !pItem ) return false;

		int id = pItem->GetIdTable();
		const tDataBase* pdbcbase = CDataBaseSystem::GetMe()->GetDataBase( DBC_SOUXIA_DATA );
		const _DBC_SOUXIA_DATA* pLine = (const _DBC_SOUXIA_DATA*)((tDataBase*)pdbcbase)->Search_Index_EQU(id);
		if( !pLine ) return false;

		int Souxiatype = pLine->nType;
		if( Souxiatype == type ) return true;

		return false;
	}


	BOOL SouXiaLu::IsBlankSouXiaItem( INT pos )
	{
		tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem( pos );
		if( !pItem ) return false;

		INT Param1 = pItem->GetExtraParam1();
		INT Param2 = pItem->GetExtraParam2();
		if( Param1 == 0 && Param2 == 0 )return true;

		return false;
	}


	BOOL SouXiaLu::IsSouXiadeformityItem( INT pos )
	{
		tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(pos);
		
		return IsSouXiadeformityItem(pItem);
	}

	BOOL SouXiaLu::IsSouXiadeformityItem( tObject_Item* pItem )
	{
		if(!pItem) return false;

		int itemclass = pItem->GetItemClass();
		int itemtype = pItem->GetItemTableType();
		if( itemclass!=ICLASS_COMITEM || itemtype!=COMITEM_SOUXIA_RELIC )
			return false;		//不是搜侠录物品残页

		return true;
	}

	BOOL SouXiaLu::IsBlankSouXiadeformity( INT pos )
	{
		tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(pos);

		return IsBlankSouXiadeformity(pItem);
	}

	BOOL SouXiaLu::IsBlankSouXiadeformity( tObject_Item* pItem )
	{
		if( !pItem ) return false;

		INT Param1 = pItem->GetExtraParam1();
		INT Param2 = pItem->GetExtraParam2();
		if( Param1 == 0 && Param2 == 0 )return true;

		return false;
	}

	INT SouXiaLu::GetSouXiadeformityType( INT pos )
	{
		tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(pos);

		return GetSouXiadeformityType(pItem);
	}

	INT SouXiaLu::GetSouXiadeformityType( tObject_Item* pItem )
	{
		if( !pItem ) return -1;

		if( !IsSouXiadeformityItem(pItem) ) return -1;		//不是搜侠录残页道具

		return pItem->GetExtraParam3();

	}

	SOUXIA_DATA* SouXiaLu::GetSouXiadeformityData( INT pos )
	{
		tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(pos);

		return GetSouXiadeformityData(pItem);
	}

	SOUXIA_DATA* SouXiaLu::GetSouXiadeformityData( tObject_Item* pItem )
	{
		if( !pItem ) return NULL;

		if( !IsSouXiadeformityItem(pItem) ) return NULL;		//不是搜侠录残页道具

		return CDataPool::GetMe()->UserSouXia_GetInfo( pItem->GetExtraParam2() );
	}

	BOOL SouXiaLu::IsSouXiaSkill( SkillID_t nSkillID )
	{
		DBC_DEFINEHANDLE(s_pSkillDBC, DBC_SKILL_DATA);
		const _DBC_SKILL_DATA* pSkillData = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_Index_EQU(nSkillID);

		if( !pSkillData ) return FALSE;

		if( pSkillData->m_bSouXiaSkill == 1 )return true;

		return FALSE;
	}

	tObject_Item* SouXiaLu::GetHEquipSouxia()
	{
		tActionItem* pActionItem = CActionSystem::GetMe()->EnumAction(HEQUIP_SOUXIA, NAMETYPE_EQUIP);
		if ( !pActionItem) return NULL;

		int id1 = pActionItem->GetID();
		tObject_Item* pItem = (tObject_Item*)(pActionItem->GetImpl());

		if( !pItem ) return NULL;

		return pItem;
	}


	SOUXIA_DATA* SouXiaLu::GetHEquipSouxiaInfo()
	{
		tObject_Item* pItem = GetHEquipSouxia();
		if( !pItem ) return NULL;

		INT index = pItem->GetExtraParam2();
		INT idTable = pItem->GetExtraParam1();

		if( index==0 && idTable == 0 ) return NULL;

		return CDataPool::GetMe()->UserSouXia_GetInfo( index );
	}

	INT SouXiaLu::GetSouXiaUsbType( tObject_Item* pItem )
	{
		if( pItem == NULL ) return -1;	//物品不存在

		int itemclass = pItem->GetItemClass();
		int itemtype = pItem->GetItemTableType();
		if( pItem->GetItemClass()!=ICLASS_COMITEM || pItem->GetItemTableType()!=COMITEM_SOUXIA )
			return -1;		//不是搜侠录物品道具

		int id = pItem->GetIdTable();
		const tDataBase* pdbcbase = CDataBaseSystem::GetMe()->GetDataBase( DBC_SOUXIA_DATA );
		const _DBC_SOUXIA_DATA* pLine = (const _DBC_SOUXIA_DATA*)((tDataBase*)pdbcbase)->Search_Index_EQU(id);

		int Souxiatype = pLine->nType;
		int SouxiaUsbType = pLine->nSouxiaUsbType;

		if( Souxiatype != 1 )return -1;	//不是USB记忆棒道具

		return SouxiaUsbType;

	}

	INT SouXiaLu::GetMatIndex(int type, int level)
	{
		if( type == -1 || level == -1 ) return -1;
		INT index = 11000000 + type*10000 + level;
		return index;
	}

	INT SouXiaLu::GetItemNum_ByIdTable( INT IdTable )
	{
		INT num = 0;
		for(INT i=0; i<MAX_BAG_SIZE; i++)
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem( i );
			if( !pItem ) 
				continue;

			if ( pItem->GetIdTable() == IdTable )
				num += pItem->GetNumber();
		}

		return num;
	}

	INT SouXiaLu::GetSouXiaUsb_Data( INT bagIndex )
	{
		tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem( bagIndex );
		if( !IsSouXiaUsbItem( 0, pItem ) ) return -1;

		//int id = pItem->GetIdTable();
		return pItem->GetExtraParam3();
	}

	VOID SouXiaLu::SetLockSouXiaItem( INT pos, BOOL lock )
	{
		tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem( pos );
		if( !pItem ) return;

		CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP_DISASS, pItem->GetID());
		pItem->SetLock(lock);
		//通知背包锁定状态
		CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
	}


	INT SouXiaLu::GetSouXiaEnumActionId( INT nPosIndex, LPCTSTR szFilter )
	{
		tActionItem* pActionItem = CActionSystem::GetMe()->EnumAction(nPosIndex, szFilter);
		if( !pActionItem ) return -1;
		return pActionItem->GetID();
	}

	VOID SouXiaLu::UpdateMainMenuBarSouxiaSkillIDNum( INT SkillId, INT num )
	{
		if( !IsSouXiaSkill(SkillId) )return;
		
		CEventSystem::GetMe()->PushEvent( GE_UPDATE_SOUXIA_SKILLNUM, SkillId, num );

	}

	INT SouXiaLu::GetSkillIdByActionID( INT nActionId )
	{
		CActionItem_Skill* pActionItem = (CActionItem_Skill*)(CActionSystem::GetMe()->GetAction(nActionId));
		if( !pActionItem ) return -1;
		const SCLIENT_SKILL* pSkill = (const SCLIENT_SKILL*)pActionItem->GetImpl();
		if( !pSkill )return -1;
		INT SkillID = pSkill->m_pDefine->m_nID;
		return SkillID;
	}

	INT SouXiaLu::GetSouXiaSkillNum( INT SkillId )
	{
		SOUXIA_DATA* pSouXiaData = GetHEquipSouxiaInfo();

		if( !pSouXiaData ) return 0; 

		INT num = 0;

		for( INT i=0; i<pSouXiaData->GetCurPetCount(); ++i )
		{
			if( pSouXiaData->m_Pet.StudyPet[i].StudyZhaoHuan == SkillId )
				num += pSouXiaData->m_Pet.StudyPet[i].LeftUseTime;
		}

		for( INT i=0; i<pSouXiaData->GetCurZuoJiCount(); ++i )
		{
			if( pSouXiaData->m_ZuoJi.StudyZuoji[i].StudyZhaoHuan == SkillId )
				num += pSouXiaData->m_ZuoJi.StudyZuoji[i].LeftUseTime;
		}


		return num;
	}

	VOID SouXiaLu::SetMainMenuBarSouXiaSkillNum( INT ActionId, INT Index )
	{
		INT SkillId = GetSkillIdByActionID(ActionId);

		if( !IsSouXiaSkill(SkillId) )return;

		INT num = GetSouXiaSkillNum(SkillId);

		if( 0 == num ) return;

		CEventSystem::GetMe()->PushEvent( GE_UPDATE_SOUXIA_INDEXSKILLNUM, Index, SkillId, num );
	}

	INT SouXiaLu::GetModelIDbySkillID( INT SkillID )
	{
		if ( SkillID == -1 )
			return -1;
		
		//查表
		DBC_DEFINEHANDLE(s_pSkillTable, DBC_SKILL_DATA);
		//搜索纪录
		const _DBC_SKILL_DATA* pSkill = (const _DBC_SKILL_DATA*)s_pSkillTable->Search_Index_EQU((UINT)SkillID);
		if(!pSkill) return -1;

		SplitStringLevelOne pSplitL1;
		pSplitL1.Reset();
		pSplitL1.Init( '|', NULL );
		pSplitL1.DoSplit( pSkill->m_lpszImpactParameter_1 );
		//INT ImactID	= pSplitL1.GetResultLineCount();
		std::string strImactID = pSplitL1.GetResultLine( 0 );

		INT ImactID = atoi( strImactID.c_str() );

		DBC_DEFINEHANDLE(s_pImactModelTable, DBC_IMPACT_SOUXIAMODEL );
		const _DBC_IMPACT_SOUXIAMODEL* pModel = (_DBC_IMPACT_SOUXIAMODEL*)s_pImactModelTable->Search_Index_EQU((UINT)ImactID);
		if( !pModel ) return -1;
		INT ModelID = pModel->nModelID;

		return ModelID;
	}

	VOID SouXiaLu::ShowSouXiaDeformity( tObject_Item* pItem )
	{
		INT type = SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.GetSouXiadeformityType(pItem);
		switch( type )
		{
		case 1:
			{

			}
			break;

		case 2:
			{

			}
			break;

		case 3:
			{

			}
			break;

		case 4:
			{

			}
			break;
		}
	}

}
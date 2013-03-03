/********************************************************************
	创建日期:	2005年11月11日
	创建时间:	16:35
	文件名称:	ItemRuler.cpp
	文件路径:	d:\Prj\Server\Server\Item\ItemRuler.cpp
	创建人:		曾鹏翔
	
	文件功能： 物品规则相关的维护
	修改纪录：
	
*********************************************************************/

#include "stdafx.h"
#include "ItemRuler.h"
#include "ItemTable.h"
#include "Rand.h"
#include "GameDefine2.h"
#include "TimeManager.h"
#include "ItemHelper.h"
#include "GameTable.h"
#include "Obj_Human.h"
#include "Obj_Monster.h"
#include "ScanOperator_ActiveTeammates.h"
#include "Config.h"
#include "Scene.h"
#include "Obj_ItemManager.h"
#include "Obj_Item.h"
#include "LogDefine.h"
#include "SouXia.h"

VOID	Obj_ItemRuler::CreateItemFromMonsterDrop(INT iMonsterID,
												 INT iPlayerLvl,					//玩家级别
												 BOOL bTeam,
												 FLOAT fControlValue,
											     Obj_ItemContaner& OutContaner)				//监控系数		
{

	__ENTER_FUNCTION

	UINT		dBaseDropRate;					//基本掉落率
	double		dDeltaDropRate	=	1.0;		//级别修正率
	UINT		dCurrentRate;					//当前掉落率随机数
	
	MONSTER_EXT_ATTR *pAttr = g_MonsterAttrExTbl.GetExtAttr( iMonsterID );
	Assert ( pAttr != NULL );
	
	INT iMonsterType = pAttr->m_BossFlag;
	INT iMonsterLvl  = pAttr->m_Level;
	
	UINT nTotalRand = RandGen::GetRand(0, 100);
	if (nTotalRand>(UINT)pAttr->m_nMonsterDropRate)
	{
		return;
	}
	if(pAttr->m_MonsterDropItemSetID < 0)
	{
		return;
	}
	MONSTER_DROP_TB* pTb = g_ItemTable.GetMonsterDropTB(pAttr->m_MonsterDropItemSetID);
	if(pTb)
	{
			if(pTb->m_ValidCount != 0)
			{
				DROP_ATT_TB*	pDropAtt = g_ItemTable.GetDropAttTB(iPlayerLvl-iMonsterLvl);
				if(pDropAtt)
				{
					if(bTeam)
					{
						dDeltaDropRate	=	pDropAtt->m_TeamAttValue;
					}
					else dDeltaDropRate = pDropAtt->m_AttValue;
				}
				else
					return;

				if((dDeltaDropRate == 0)||(g_Config.m_ConfigInfo.m_DropParam == 0))
					return;

				MONSTER_DROPCONTENT* pTbContent = pTb->m_Drops;
				_ITEM	it;
				INT		quality = 0;
				ITEM_LOG_PARAM ItemLogParam;
				_ITEM_TYPE itemType;
				BOOL	Ret = FALSE;
				for(UINT j =0; j<pTb->m_ValidCount; ++j)
				{
					//生成这个物品掉落的概率 概率=物品价值*级别修正率*全局掉落修正参数
					dBaseDropRate = (double)pTb->m_Drops[j].m_nItemValue*dDeltaDropRate*(double)g_Config.m_ConfigInfo.m_DropParam;
					
					//获得当前掉落率随机数
					dCurrentRate = RandGen::GetRand(0, MAX_MONSTER_VALUE);

					if(dCurrentRate<=dBaseDropRate)
					{
						if(pTb->m_Drops[j].m_nItemCount > 0)
						{
							for(INT nNum=0; nNum<pTb->m_Drops[j].m_nCycle; ++nNum)
							{
								int nPos = RandGen::GetRand(0, pTb->m_Drops[j].m_nItemCount-1);
								UINT uItemIndex = pTb->m_Drops[j].m_pDropContent[nPos];
								UCHAR	uClass		= GetSerialClass(uItemIndex);
								it.CleanUp();
								switch(uClass)
								{
								case ICLASS_EQUIP:
									{
										EQUIP_TB* pGET =	g_ItemTable.GetEquipTB(uItemIndex);
										if(!pGET)
										{
											g_pLog->FastSaveLog( LOG_FILE_1, "找不到对应物品的表格定义，请确定EQUIP资源正确 ItemType=%d",uItemIndex);
											OutContaner.CleanUp();
											return;
										}
										itemType = ConvertSerial2ItemType(uItemIndex);
										it.GetEquipData()->m_EquipType = ITEM_DROP;
										Ret = ItemCreateRuler::CreateEquipAttrib(&ItemLogParam, itemType, 0, it, quality, iMonsterLvl, iMonsterType);
										if (Ret)
										{
											if(g_pItemManager->GenItemGuid(it.m_ItemGUID))
											{
												OutContaner.AddItem(it);
											}
										}
									}
									break;
								case ICLASS_MATERIAL: 
									{
										COMMITEM_INFO_TB*	pGET		=		g_ItemTable.GetCommItemInfoTB(uItemIndex);
										if(!pGET)
										{
											g_pLog->FastSaveLog( LOG_FILE_1, "找不到对应物品的表格定义，请确定CommonItem资源正确 ItemType=%d",uItemIndex);
											OutContaner.CleanUp();
											return;
										}

										itemType = ConvertSerial2ItemType(uItemIndex);
										Ret = ItemCreateRuler::CreateMedicAttrib(itemType, it);
										if (Ret)
										{
											if(g_pItemManager->GenItemGuid(it.m_ItemGUID))
											{
												OutContaner.AddItem(it);
											}
										}
									}
									break;
								case ICLASS_COMITEM: 
								case ICLASS_IDENT:
									{
										COMMITEM_INFO_TB*	pGET		=		g_ItemTable.GetCommItemInfoTB(uItemIndex);
										if(!pGET)
										{
											g_pLog->FastSaveLog( LOG_FILE_1, "找不到对应物品的表格定义，请确定CommonItem资源正确 ItemType=%d",uItemIndex);
											OutContaner.CleanUp();
											return;
										}

										itemType = ConvertSerial2ItemType(uItemIndex);
										Ret = ItemCreateRuler::CreateMedicAttrib(itemType, it);
										if (Ret)
										{
											if(g_pItemManager->GenItemGuid(it.m_ItemGUID))
											{
												OutContaner.AddItem(it);
											}
										}
									}
									break;
								case ICLASS_GEM:
									{
										GEMINFO_TB*	pGET		=		g_ItemTable.GetGemInfoTB(uItemIndex);
										if(!pGET)
										{
											g_pLog->FastSaveLog( LOG_FILE_1, "找不到对应物品的表格定义，请确定Gem资源正确 ItemType=%d",uItemIndex);
											OutContaner.CleanUp();
											return;
										}
										itemType = ConvertSerial2ItemType(uItemIndex);
										Ret = ItemCreateRuler::CreateGemAttrib(itemType, it);
										if (Ret)
										{
											if(g_pItemManager->GenItemGuid(it.m_ItemGUID))
											{
												OutContaner.AddItem(it);
											}
										}
									}
									break;
								default:
									g_pLog->FastSaveLog( LOG_FILE_1, "找不到对应物品的类型 uClass=%d",uClass);
									OutContaner.CleanUp();
									return;
								}
							}
						}
					}
				}
			}
	}
	__LEAVE_FUNCTION
}

//创建装备属性
BOOL		ItemCreateRuler::CreateEquipAttrib(ITEM_LOG_PARAM* pLogParam,_ITEM_TYPE itemType,
											   INT iPlayerJob,_ITEM& it,INT& quality, INT iMonsterLvl, INT iMonsterType)
{
	__ENTER_FUNCTION
	Assert(pLogParam);

	EQUIP_TB* pGET	=	g_ItemTable.GetEquipTB(itemType.ToSerial());
	
	if(pGET)
	{
		it.m_ItemIndex							=	pGET->m_IndexID;
		it.m_RulerID							=	(CHAR)pGET->m_RulerID;
		//it.GetEquipData()->m_EquipPoint			=	pGET->m_EquipPoint;
		//it.GetEquipData()->m_SetNum				=	pGET->m_EquipSetNum;			//牌型
		it.GetEquipData()->m_BasePrice			=	pGET->m_BasePrice;	//售出价格
		//it.GetEquipData()->m_EquipPoint			=	pGET->m_EquipPoint;		//物品装佩点
		it.GetEquipData()->m_MaxDurPoint 		=	pGET->m_MaxDur;		//最大耐久值
		//it.GetEquipData()->m_NeedLevel 			=	pGET->m_ReqLevel;		//需求等级
		it.GetEquipData()->m_BaseGemHole 		=	pGET->m_BaseGemHole;		//最大宝石个数
		//it.GetEquipData()->m_MAXGemHole			=	pGET->m_MAXGemHole;					//最大镶嵌孔
		//it.GetEquipData()->m_PrefessionReq		=	pGET->m_PrefessionReq;				//职业需求
		
		//it.GetEquipData()->m_MaxNum				=	pGET->m_EquipSetMaxNum;				//最大套装数目
		
		//it.GetEquipData()->m_AttrCount			=	pGET->m_AttCount;	//属性的数量
		//it.GetEquipData()->m_nAffixSkillCount	=	pGET->m_AffSkillCount;		//附加技能个数
		//it.GetEquipData()->m_nEffectSkillCount	=	pGET->m_EffSkillCount;	//装备影响技能个数
		it.GetEquipData()->m_CurDamagePoint		 = 0;

		INT nAttCount = 0;
		if(pGET->m_NearAttack>0)
		{
			it.GetEquipData()->m_pAttr[nAttCount].m_AttrType		 =	IATTRIBUTE_BASE_ATTACK_NEAR;	//近程攻击
			it.GetEquipData()->m_pAttr[nAttCount].m_Value.m_Value	 =	pGET->m_NearAttack;	//近程攻击
			++nAttCount;
		}

		if(pGET->m_FarAttack>0)
		{
			it.GetEquipData()->m_pAttr[nAttCount].m_AttrType		 =	IATTRIBUTE_BASE_ATTACK_FAR;	//远程攻击
			it.GetEquipData()->m_pAttr[nAttCount].m_Value.m_Value	 =	pGET->m_FarAttack;	//远程攻击
			++nAttCount;
		}

		if(pGET->m_MagicNearAttack>0)
		{
			it.GetEquipData()->m_pAttr[nAttCount].m_AttrType		 =	IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR;	//近程内功攻击
			it.GetEquipData()->m_pAttr[nAttCount].m_Value.m_Value	 =	pGET->m_MagicNearAttack;		//近程内功攻击
			++nAttCount;
		}
		if(pGET->m_MagicFarAttack>0)
		{
			it.GetEquipData()->m_pAttr[nAttCount].m_AttrType		 =	IATTRIBUTE_BASE_ATTACK_MAGIC_FAR;//近程外功攻击
			it.GetEquipData()->m_pAttr[nAttCount].m_Value.m_Value	 =	pGET->m_MagicFarAttack;		//近程外功攻击
			++nAttCount;
		}

		if(pGET->m_NearDefence>0)
		{
			it.GetEquipData()->m_pAttr[nAttCount].m_AttrType		 =	IATTRIBUTE_BASE_DEFENCE_NEAR;	//近程防御
			it.GetEquipData()->m_pAttr[nAttCount].m_Value.m_Value	 =	pGET->m_NearDefence;	//近程防御
			++nAttCount;
		}

		if(pGET->m_FarDefence>0)
		{
			it.GetEquipData()->m_pAttr[nAttCount].m_AttrType		 =	IATTRIBUTE_BASE_DEFENCE_FAR;	//远程防御
			it.GetEquipData()->m_pAttr[nAttCount].m_Value.m_Value	 =	pGET->m_FarDefence;	//远程防御
			++nAttCount;
		}

		if(pGET->m_MagicNearDefense>0)
		{
			it.GetEquipData()->m_pAttr[nAttCount].m_AttrType		=	IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR;//近程内功防御
			it.GetEquipData()->m_pAttr[nAttCount].m_Value.m_Value	=	pGET->m_MagicNearDefense;	//近程内功防御
			++nAttCount;
		}
		if(pGET->m_MagicFarDefense>0)
		{
			it.GetEquipData()->m_pAttr[nAttCount].m_AttrType		=	IATTRIBUTE_BASE_DEFENCE_MAGIC_FAR;//近程外功防御
			it.GetEquipData()->m_pAttr[nAttCount].m_Value.m_Value	=	pGET->m_MagicFarDefense;	//近程外功防御
			++nAttCount;
		}

		it.GetEquipData()->m_PaiType	 = pGET->m_PaiType;
		
		it.GetEquipData()->m_CurDurPoint = pGET->m_MaxDur;
		it.GetEquipData()->m_CurMaxDur	 = pGET->m_MaxDur;
		it.GetEquipData()->m_CurGemHole  = pGET->m_BaseGemHole;
		it.GetEquipData()->m_AttrCount   = nAttCount;

		Assert(pGET->m_ReqLevelCount>0);
		INT nNeedLevel = pGET->m_ReqLevel[0];
		if(pGET->m_ReqLevelCount > 1)
		{
			INT nRand = RandGen::GetRand(0, pGET->m_ReqLevelCount-1);
			nNeedLevel = pGET->m_ReqLevel[nRand];
		}
		
		it.GetEquipData()->m_NeedLevel = nNeedLevel;								//需要等级

		if((HUMAN_EQUIP)pGET->m_EquipPoint == HEQUIP_SUIT)
		{
			memcpy(&it.GetEquipData()->m_pAttr[nAttCount], &pGET->m_Attr, sizeof(_ITEM_ATTR)*pGET->m_AttCount);
			it.GetEquipData()->m_AttrCount += pGET->m_AttCount;
			return TRUE;
		}
		// add by gh for souxia 捜侠录道具和坐骑没有影响
		if (HEQUIP_SOUXIA == (HUMAN_EQUIP)pGET->m_EquipPoint || HEQUIP_RIDER == (HUMAN_EQUIP)pGET->m_EquipPoint)
		{
			return TRUE;
		}
		// end of add
		if(it.GetEquipData()->m_EquipType == ITEM_DROP)
		{
			return CaculateDropEquipEffectAttrib(pLogParam, it, iPlayerJob, iMonsterLvl, iMonsterType);
		}
		else if(it.GetEquipData()->m_EquipType == ITEM_CREATE)
		{
			return CaculateCreateEquipEffectAttrib(pLogParam, it, iPlayerJob);
		}
		return CaculateEquipEffectAttrib(pLogParam, it, iPlayerJob);
	}
	Assert(FALSE);

	__LEAVE_FUNCTION

		return FALSE;
}
//---------------------------------------------------------------------------------------------*/
BOOL	ItemCreateRuler::CreateGemAttrib(_ITEM_TYPE itemType,_ITEM& it)
{
	__ENTER_FUNCTION
	
	GEMINFO_TB*	pGET		=		g_ItemTable.GetGemInfoTB( itemType.ToSerial());

	if(pGET)
	{
		it.m_ItemIndex						=	pGET->m_nTableIndex;
		it.m_RulerID						=	(CHAR)pGET->m_nRulerID;

		it.GetGemData()->m_nPrice			=	pGET->m_nPrice;
		it.GetGemData()->m_nSellPrice		=   pGET->m_nSellPrice;
		it.GetGemData()->m_bBroadCast		=   pGET->m_bBroadCast;
		memcpy(it.GetGemData()->m_EquipType, pGET->m_EquipType, sizeof(UINT)*MAX_BASE_EQUIP_COUNT);

		it.GetGemData()->m_AttCount = pGET->m_AttCount;
		for(UINT i=0; i<it.GetGemData()->m_AttCount; ++i)
		{
			it.GetGemData()->m_GenAttr[i].m_AttrType = pGET->m_GenAttr[i].m_AttrType;
			it.GetGemData()->m_GenAttr[i].m_Value.m_Value	= pGET->m_GenAttr[i].m_Value.m_Value;
		}

		it.GetGemData()->m_GemReqCount = pGET->m_GemReqCount;
		for(UINT i=0; i<it.GetGemData()->m_GemReqCount; ++i)
		{
			it.GetGemData()->m_GemReqAttr[i].m_GemType = pGET->m_GemReqAttr[i].m_GemType;
			it.GetGemData()->m_GemReqAttr[i].m_Value.m_Value	= pGET->m_GemReqAttr[i].m_Value.m_Value;
		}
		return TRUE;
	}
	else
		g_pLog->FastSaveLog( LOG_FILE_1, 
		"ItemCreateRuler::CreateEquipAttrib can't find GEMINFO_TB GemObjID =%d", itemType.ToSerial() ) ;

	return FALSE;
	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	ItemCreateRuler::CreateGemAttrib(UINT TableIndex,_ITEM& it)
{
	__ENTER_FUNCTION
	GEMINFO_TB*	pGET		=		g_ItemTable.GetGemInfoTB( TableIndex);

	if(pGET)
	{
		it.m_ItemIndex						=	pGET->m_nTableIndex;
		it.m_RulerID						=	(CHAR)pGET->m_nRulerID;

		it.GetGemData()->m_nPrice			=	pGET->m_nPrice;
		it.GetGemData()->m_nSellPrice		=   pGET->m_nSellPrice;
		it.GetGemData()->m_bBroadCast		=   pGET->m_bBroadCast;
		memcpy(it.GetGemData()->m_EquipType, pGET->m_EquipType, sizeof(MAX_BASE_EQUIP_COUNT));

		it.GetGemData()->m_AttCount = pGET->m_AttCount;
		for(UINT i=0; i<it.GetGemData()->m_AttCount; ++i)
		{
			it.GetGemData()->m_GenAttr[i].m_AttrType = pGET->m_GenAttr[i].m_AttrType;
			it.GetGemData()->m_GenAttr[i].m_Value.m_Value	= pGET->m_GenAttr[i].m_Value.m_Value;
		}

		it.GetGemData()->m_GemReqCount = pGET->m_GemReqCount;
		for(UINT i=0; i<it.GetGemData()->m_GemReqCount; ++i)
		{
			it.GetGemData()->m_GemReqAttr[i].m_GemType = pGET->m_GemReqAttr[i].m_GemType;
			it.GetGemData()->m_GemReqAttr[i].m_Value.m_Value	= pGET->m_GemReqAttr[i].m_Value.m_Value;
		}

		return TRUE;
	}
	else
		g_pLog->FastSaveLog( LOG_FILE_1, "ItemCreateRuler::CreateGemAttrib can't find GEMINFO_TB GemObjID =%d", TableIndex ) ;

	__LEAVE_FUNCTION

		return FALSE;
}




BOOL	ItemCreateRuler::CreateMaterialAttrib(_ITEM_TYPE itemType,_ITEM& it)
{
	__ENTER_FUNCTION

		Assert(FALSE);

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	ItemCreateRuler::CreateMedicAttrib(_ITEM_TYPE itemType,_ITEM& it)
{
	__ENTER_FUNCTION

	COMMITEM_INFO_TB*	pGET		=		g_ItemTable.GetCommItemInfoTB( itemType.ToSerial());

	if(pGET)
	{
		it.m_ItemIndex						=	pGET->m_nTableIndex;
		
		it.GetMedicData()->m_nLevel			=	pGET->m_nLevel;	
		it.GetMedicData()->m_nBasePrice		=	pGET->m_nBasePrice;
		it.GetMedicData()->m_nSellPrice		=	pGET->m_nSellPrice;
		it.m_RulerID						=	(CHAR)pGET->m_nRulerID;	
		it.GetMedicData()->m_nLayedNum		=	pGET->m_nLayedNum;	
		it.GetMedicData()->m_nScriptID		=	pGET->m_nScriptID;	
		it.GetMedicData()->m_nSkillID		=	pGET->m_nSkillID;	
		it.GetMedicData()->m_nCount			=	1; //叠放物品，产生一个
		it.GetMedicData()->m_nReqSkill		=	pGET->m_nReqSkill;
		it.GetMedicData()->m_nReqSkillLevel	=	pGET->m_nReqSkillLevel;
		it.GetMedicData()->m_TargetType		=	pGET->m_TargetType;
		it.GetMedicData()->m_bBroadcast		=	pGET->m_bBroadcast;
		//	add by gh for souxia 2010/05/11
		if (COMITEM_SOUXIA == itemType.m_Type)
		{
			return	CreateSouXiaAttrib(itemType, it);
		}
		return TRUE;
	}
	else
		Assert(FALSE);
	return FALSE;
	
	__LEAVE_FUNCTION
	return FALSE;
}


BOOL	ItemCreateRuler::CreateTaskItemAttrib(_ITEM_TYPE itemType,_ITEM& it)
{
	__ENTER_FUNCTION

	TASKITEM_INFO_TB*	pGET		=		g_ItemTable.GetTaskItemInfoTB( itemType.ToSerial());

	if(pGET)
	{
		it.m_ItemIndex						=	pGET->m_nTableIndex;

		it.GetMedicData()->m_nLevel			=	pGET->m_nLevel;	
		it.GetMedicData()->m_nBasePrice		=	pGET->m_nBasePrice;
		it.GetMedicData()->m_nSellPrice		=	pGET->m_nSellPrice;
		it.m_RulerID						=	(CHAR)pGET->m_nRulerID;	
		it.GetMedicData()->m_nLayedNum		=	pGET->m_nLayedNum;	
		it.GetMedicData()->m_nScriptID		=	pGET->m_nScriptID;	
		it.GetMedicData()->m_nSkillID		=	pGET->m_nSkillID;	
		it.GetMedicData()->m_nCount			=	1; //叠放物品，产生一个
		it.GetMedicData()->m_nReqSkill		=	pGET->m_nReqSkill;
		it.GetMedicData()->m_nReqSkillLevel	=	pGET->m_nReqSkillLevel;
		it.GetMedicData()->m_TargetType		=	pGET->m_TargetType;
		it.GetMedicData()->m_bBroadcast		=	pGET->m_bBroadcast;
		return TRUE;
	}
	else
		Assert(FALSE);
	return FALSE;

	__LEAVE_FUNCTION
		return FALSE;
}

BOOL	ItemCreateRuler::CreateExtraBagAttrib(_ITEM_TYPE itemType,_ITEM& it)
{
	__ENTER_FUNCTION

		EXTRABAG_INFO_TB*	pGET		=		g_ItemTable.GetExtraBagInfoTB(itemType.ToSerial());
	if(pGET)
	{
		it.m_ItemIndex							=	pGET->m_nTableIndex;
		it.m_RulerID							=	(CHAR)pGET->m_nRulerID;	
		it.GetExtraBagData()->m_nBasePrice		=	pGET->m_nBasePrice;
		it.GetExtraBagData()->m_nSellPrice		=	pGET->m_nSellPrice;
		it.GetExtraBagData()->m_nValidTime		=	pGET->m_nValidTime;
		it.GetExtraBagData()->m_nBeginTime		=	INVALID_TIME;
		it.GetExtraBagData()->m_nSize			=   pGET->m_nSize;
		return TRUE;
	}
	else
		Assert(FALSE);
	return FALSE;

	__LEAVE_FUNCTION
		return FALSE;
}

BOOL	ItemCreateRuler::CreateMountAttrib(_ITEM_TYPE itemType,_ITEM& it)   // [7/8/2010 陈军龙]
{
	__ENTER_FUNCTION

	MOUNT_INFO_TB*	pGET		=		g_ItemTable.GetMountInfoTB(itemType.ToSerial());
	
	if(pGET)
	{
		it.m_ItemIndex						=	pGET->m_nTableIndex;
		it.m_RulerID						=	(CHAR)pGET->m_nRule;	
        it.GetMountData()->m_nNeedLevel     =   pGET->m_nNeedLevel;
		it.GetMountData()->m_nAddtionSpeed  =   pGET->m_nAddtionSpeed;
		it.GetMountData()->m_nLife          =   pGET->m_nLife;
		it.GetMountData()->m_nBindType      =   pGET->m_nBindType;
		it.GetMountData()->m_nPrice         =   pGET->m_nPrice;

		return TRUE;
	}
	else
		Assert(FALSE);
	return FALSE;

	__LEAVE_FUNCTION
		return FALSE;
}

BOOL	ItemCreateRuler::CreateStoreMapAttrib(_ITEM_TYPE itemType,_ITEM& it)
{
	__ENTER_FUNCTION

	STORE_MAP_INFO_TB*	pGET		=		g_ItemTable.GetStoreMapTB(itemType.ToSerial());

	if(pGET)
	{
		it.m_ItemIndex						=	pGET->m_nTableIndex;
		//it.m_ItemType						=	pGET->m_ItemType;
		it.GetStoreMapData()->m_nLevel		=	pGET->m_nLevel;	
		it.m_RulerID						=	(CHAR)pGET->m_nRulerID;	
		it.GetStoreMapData()->m_xPos		=	pGET->m_xPos;
		it.GetStoreMapData()->m_zPos		=	pGET->m_zPos;
		it.GetStoreMapData()->m_SceneID		=	pGET->m_SceneID;
		it.GetStoreMapData()->m_GrowType	=	pGET->m_GrowPointType;
		
		return TRUE;
	}
	else
		Assert(FALSE);
	return FALSE;

	__LEAVE_FUNCTION
		return FALSE;
}
// add by gh 
BOOL	ItemCreateRuler::CreateSouXiaAttrib(_ITEM_TYPE itemType, _ITEM& it)
{
	__ENTER_FUNCTION

	SOUXIA_INFO_TB*	pGET = g_ItemTable.GetSouXiaTB(itemType.ToSerial());

	if(pGET)
	{
		it.m_ItemIndex	=	pGET->m_nTableIndex;
		
		if (ITEM_SOUXIALU == pGET->m_nUseType)
		{
			it.m_Param[0] = pGET->m_nTableIndex;
		} 
		else if(ITEM_MEMORYCLUB == pGET->m_nUseType )
		{
			for (int i=0; i<4; ++i)
			{
				if (pGET->m_nMemoryContent[i] > 0)
				{
					it.m_Param[2] = pGET->m_nMemoryContent[i];	// 有可能是从表格随机的要通知客户端
				}
			}
		}

		return TRUE;
	}
	else
		Assert(FALSE);
	return FALSE;

	__LEAVE_FUNCTION
		return FALSE;
}

BOOL	ItemCreateRuler::CreateSoulBeadAttrib(_ITEM_TYPE itemType,_ITEM& it)
{
	__ENTER_FUNCTION

	SOUL_BEAD_INFO_TB*	pGET = g_ItemTable.GetSoulBeadTB(itemType.ToSerial());

	if(pGET)
	{
		it.m_ItemIndex	=	pGET->m_nTableIndex;
		it.m_RulerID	=	pGET->m_nRuleID;
		//it.m_SoulBead.m_nSellPrice = 
		//it.m_SoulBead.m_nValidTime	=	pGET->m_nValidTime;
		return TRUE;
	}
	else
		Assert(FALSE);
	return FALSE;

	__LEAVE_FUNCTION
		return FALSE;
}
// end of add
BOOL	ItemCreateRuler::CreateQuestItemAttrib(_ITEM_TYPE itemType,_ITEM& it)
{
	__ENTER_FUNCTION

	return	FALSE;

	__LEAVE_FUNCTION

	return	FALSE;
}


BOOL	ItemCreateRuler::CaculateDropEquipEffectAttrib(ITEM_LOG_PARAM*	pLogParam,_ITEM& it, 
													   INT iPlayerJob, INT iMonsterLvl, INT iMonsterType)
{
	__ENTER_FUNCTION
		Assert(pLogParam);
	Assert(it.GetEquipData()->m_EquipType == ITEM_DROP);

	EQUIP_TB* pGET	=	g_ItemTable.GetEquipTB(it.m_ItemIndex);
	Assert(pGET);
	if (!pGET)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "[ItemCreateRuler::CaculateDropEquipEffectAttrib] GetEquipTB index is error [%d]",it.m_ItemIndex);
		return FALSE;
	}
	
	UINT nPointValue = g_ItemTable.GetEquipPointValue((HUMAN_EQUIP)pGET->m_EquipPoint);
	Assert(nPointValue != 0);
	UINT nAddLevel = 0;
	switch(iMonsterType)
	{
	case 0:
		nAddLevel = g_Config.m_ConfigInfo.m_nComMonsterDrop;
		break;
	case 1:
		nAddLevel = g_Config.m_ConfigInfo.m_nBossDrop;
	    break;
	default:
		Assert(0);
	    break;
	}
	INT nAttCount = it.GetEquipData()->m_AttrCount;
	if(ISCommonEquip(it.m_ItemIndex))//普通装备
	{
		UINT nSumRate = g_ItemTable.GetSumRateByReqLevel(it.GetEquipData()->m_NeedLevel);

		INT nSetCount = g_ItemTable.GetAttSetCount();
		//属性组随机算法
		INT nRand = RandGen::GetRand(0, nSumRate);

		EQUIP_ATT_SET_TB* pSetTB = g_ItemTable.GetAttSetTBByRate(nRand);
		if(pSetTB)
		{
			//it.GetEquipData()->m_nAttSet = pSetTB->m_IndexID;
			for(int nEquipAttCount=0; nEquipAttCount<pSetTB->m_AttCount; ++nEquipAttCount)
			{
				EQUIP_ATT_LEVEL_TB* pLevelTB = g_ItemTable.GetAttLevelValue((ITEM_ATTRIBUTE)pSetTB->m_Attr[nEquipAttCount]);
				if(pLevelTB)
				{
					it.GetEquipData()->m_pAttr[nAttCount].m_AttrType		 = pSetTB->m_Attr[nEquipAttCount];
					float fValue = (float)pLevelTB->m_Attr[iMonsterLvl+nAddLevel]*(float)nPointValue/(float)100.0;
					if (fValue<1)
					{
						fValue = 1;
					}
					it.GetEquipData()->m_pAttr[nAttCount].m_Value.m_Value	 = (SHORT)fValue;
					++nAttCount;
				}
				else
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "ItemCreateRuler::CaculateDropEquipEffectAttrib can't find EQUIP_ATT_LEVEL_TB") ;
					Assert(0);
				}
			}
			it.GetEquipData()->m_AttrCount = it.GetEquipData()->m_AttrCount + nAttCount;
		}
	}
	else
	{
		memcpy(&it.GetEquipData()->m_pAttr[nAttCount], &pGET->m_Attr, sizeof(_ITEM_ATTR)*pGET->m_AttCount);
		it.GetEquipData()->m_AttrCount = it.GetEquipData()->m_AttrCount + pGET->m_AttCount;
	}
	
	it.GetEquipData()->m_EquipScore  = pGET->m_EquipScore;
	return	TRUE;

	__LEAVE_FUNCTION

		return	FALSE;
}

BOOL	ItemCreateRuler::CaculateCreateEquipEffectAttrib(ITEM_LOG_PARAM*	pLogParam,_ITEM& it, INT iPlayerJob)
{
	__ENTER_FUNCTION
		Assert(pLogParam);
	Assert(it.GetEquipData()->m_EquipType == ITEM_CREATE);
	Assert(it.GetEquipData()->m_EquipScore>0);
	Assert(iPlayerJob >=0);
	Assert(iPlayerJob <MAX_JOB_SUM);

	EQUIP_TB* pGET	=	g_ItemTable.GetEquipTB(it.m_ItemIndex);
	Assert(pGET);
	if (!pGET)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "[ItemCreateRuler::CaculateCreateEquipEffectAttrib] GetEquipTB index is error [%d]",it.m_ItemIndex);
		return FALSE;
	}
	UINT nPointValue = g_ItemTable.GetEquipPointValue((HUMAN_EQUIP)pGET->m_EquipPoint);
	Assert(nPointValue != 0);

	if (it.GetEquipData()->m_EquipScore>(UINT)g_Config.m_ConfigInfo.m_nMinPaiScore)
	{
		if(it.GetEquipData()->m_EquipScore<(UINT)g_Config.m_ConfigInfo.m_nMinZiPaiScore)
		{
			UINT nCount = g_ItemTable.GetNoZiPaiCount();
			if (nCount>0)
			{
				INT nRand = RandGen::GetRand(0, nCount-1);
				it.GetEquipData()->m_PaiType = g_ItemTable.GetNoZiPai(nRand);
			}
		}
		else
		{
			UINT nCount = g_ItemTable.GetZiPaiCount();
			if (nCount>0)
			{
				INT nRand = RandGen::GetRand(0, nCount-1);
				it.GetEquipData()->m_PaiType = g_ItemTable.GetZiPai(nRand);
			}
		}
	}
	
	INT nAttCount = it.GetEquipData()->m_AttrCount;
	if (ISCommonEquip(it.m_ItemIndex))
	{
		EQUIPEXTRATT_TB* pEquipExtraAtt	= g_ItemTable.GetEquipExtraAttInfoTB(it.GetEquipData()->m_EquipScore);
		Assert(pEquipExtraAtt);
		if (!pEquipExtraAtt)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "[ItemCreateRuler::CaculateCreateEquipEffectAttrib] GetEquipExtraAttInfoTB index is error [%d]",it.GetEquipData()->m_EquipScore);
			return FALSE;
		}
		EQUIPEXTRATT equipExtraAtt = pEquipExtraAtt->m_EquipExtrAtt[iPlayerJob];
		INT nMaxValidNum = equipExtraAtt.m_ValidCount-1;


		Assert(equipExtraAtt.m_ValidCount <= MAX_ITEM_NATUREATTR);

		for (UINT i=0; i<equipExtraAtt.m_ValidCount; ++i)
		{
			UINT nAtt = RandGen::GetRand(0, equipExtraAtt.m_GenAttr[i].m_AttrCount-1);
			EQUIP_ATT_LEVEL_TB* pAttTB = g_ItemTable.GetAttLevelValue((ITEM_ATTRIBUTE)equipExtraAtt.m_GenAttr[i].m_Attr[nAtt]);
			Assert(pAttTB);
			if (!pAttTB)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[ItemCreateRuler::CaculateCreateEquipEffectAttrib] GetAttLevelValue index is error [%d]",(ITEM_ATTRIBUTE)equipExtraAtt.m_GenAttr[i].m_Attr[nAtt]);
				return FALSE;
			}
			UINT nValue = pAttTB->m_Attr[it.GetEquipData()->m_NeedLevel];
			//基础数值*被选中的次数*装备分值加成。
			double nLogScore = log((double)it.GetEquipData()->m_EquipScore);
			double nDValue = (double)nValue*nPointValue*nLogScore/(double)100;
			nValue = (UINT)nDValue; 
			if (nValue == 0)
			{
				nValue = 1;
			}
			it.GetEquipData()->m_pAttr[nAttCount].m_AttrType = equipExtraAtt.m_GenAttr[i].m_Attr[nAtt];
			it.GetEquipData()->m_pAttr[nAttCount].m_Value.m_Value = nValue;
			++nAttCount;
		}
		it.GetEquipData()->m_AttrCount = nAttCount;
	}
	else
	{
		for (UINT i=0; i<pGET->m_AttCount; ++i)
		{
			SHORT nValue = pGET->m_Attr[i].m_Value.m_Value;

			//基础数值*装备分值加成*装备权重。
			double nLogScore = log((double)it.GetEquipData()->m_EquipScore);
			float fValue = (float)nValue*(float)nLogScore;
			if (fValue == 0)
			{
				fValue = 1;
			}
			it.GetEquipData()->m_pAttr[nAttCount].m_AttrType = pGET->m_Attr[i].m_AttrType;
			it.GetEquipData()->m_pAttr[nAttCount].m_Value.m_Value = nValue;
			++nAttCount;
		}
		it.GetEquipData()->m_AttrCount = nAttCount;
	}

	return	TRUE;

	__LEAVE_FUNCTION

		return	FALSE;
}

BOOL	ItemCreateRuler::CaculateEquipEffectAttrib(ITEM_LOG_PARAM*	pLogParam,_ITEM& it, INT iPlayerLvl)
{
	__ENTER_FUNCTION
		Assert(pLogParam);
	Assert(it.GetEquipData()->m_EquipType != ITEM_DROP);
	Assert(it.GetEquipData()->m_EquipType != ITEM_CREATE);
	//Assert(!ISCommonEquip(it.m_ItemIndex)); //普通装备
	EQUIP_TB* pGET	=	g_ItemTable.GetEquipTB(it.m_ItemIndex);
	Assert(pGET);
	if (!pGET)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "[ItemCreateRuler::CaculateEquipEffectAttrib] GetEquipTB index is error [%d]",it.m_ItemIndex);
		return FALSE;
	}
	UINT nPointValue = g_ItemTable.GetEquipPointValue((HUMAN_EQUIP)pGET->m_EquipPoint);
	Assert(nPointValue != 0);

	INT nAttCount = it.GetEquipData()->m_AttrCount;
	if(ISCommonEquip(it.m_ItemIndex))//普通装备
	{
		UINT nSumRate = g_ItemTable.GetSumRateByReqLevel(it.GetEquipData()->m_NeedLevel);

		INT nSetCount = g_ItemTable.GetAttSetCount();
		//属性组随机算法
		INT nRand = RandGen::GetRand(0, nSumRate);

		EQUIP_ATT_SET_TB* pSetTB = g_ItemTable.GetAttSetTBByRate(nRand);
		if(pSetTB)
		{
			//it.GetEquipData()->m_nAttSet = pSetTB->m_IndexID;
			for(int nEquipAttCount=0; nEquipAttCount<pSetTB->m_AttCount; ++nEquipAttCount)
			{
				EQUIP_ATT_LEVEL_TB* pLevelTB = g_ItemTable.GetAttLevelValue((ITEM_ATTRIBUTE)pSetTB->m_Attr[nEquipAttCount]);
				if(pLevelTB)
				{
					it.GetEquipData()->m_pAttr[nAttCount].m_AttrType		 = pSetTB->m_Attr[nEquipAttCount];
					float fValue = (float)pLevelTB->m_Attr[it.GetEquipData()->m_NeedLevel]*(float)nPointValue/(float)100.0;
					if (fValue<1)
					{
						fValue = 1;
					}
					it.GetEquipData()->m_pAttr[nAttCount].m_Value.m_Value	 = (SHORT)fValue;
					++nAttCount;
				}
				else
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "ItemCreateRuler::CaculateEquipEffectAttrib can't find EQUIP_ATT_LEVEL_TB") ;
					Assert(0);
				}
			}
			it.GetEquipData()->m_AttrCount = nAttCount;
		}
	}
	else
	{
		memcpy(&it.GetEquipData()->m_pAttr[nAttCount], &pGET->m_Attr, sizeof(_ITEM_ATTR)*pGET->m_AttCount);
		it.GetEquipData()->m_AttrCount = it.GetEquipData()->m_AttrCount + pGET->m_AttCount;
	}

	it.GetEquipData()->m_EquipScore  = pGET->m_EquipScore;
	return	TRUE;

	__LEAVE_FUNCTION

		return	FALSE;
}

//最小获取经验为1
uint	MonsterExpCaculateRuler::CaculateBaseExp(INT iMonsterLevel,INT iKillerLevel,uint MonsterBaseExp, BOOL bTeam)
{

	__ENTER_FUNCTION
		
	DROP_ATT_TB* pDropAttTB = g_ItemTable.GetDropAttTB(iKillerLevel-iMonsterLevel);
	Assert(pDropAttTB);
	if (!pDropAttTB)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "[MonsterExpCaculateRuler::CaculateBaseExp] GetDropAttTB index is error [%d]",iKillerLevel-iMonsterLevel);
		return 1;
	}
	UINT nExp = 0;
	if (bTeam)
	{
		nExp  = (uint)(MonsterBaseExp*pDropAttTB->m_TeamExpValue);
		if (nExp == 0)
		{
			nExp = 1;
		}
	}
	else
	{
		nExp  = (uint)(MonsterBaseExp*pDropAttTB->m_ExpValue);
		if (nExp == 0)
		{
			nExp = 1;
		}
	}
	return nExp;
	
	__LEAVE_FUNCTION

		return 1 ;
}

BOX_DISTRIBUTE_RULER	MonsterDropRuler::GetMonsterDropRuler(Obj_Monster* pMonster)
{
	__ENTER_FUNCTION

		Assert(pMonster);
		
		INT	 iDataID		=	pMonster->GetDataID();
        
		MONSTER_EXT_ATTR *pAttr = g_MonsterAttrExTbl.GetExtAttr( iDataID );
		if(pAttr)
		{
			return (BOX_DISTRIBUTE_RULER)pAttr->m_BossFlag;
		}

		return BDR_UNKNOW;

	__LEAVE_FUNCTION

		return BDR_UNKNOW;
}


BOOL	MonsterDropRuler::CaculateCommOwnerList(Obj_Monster* pMonster)
{
	__ENTER_FUNCTION

		if(!pMonster)	
		{
			Assert(pMonster);
			return FALSE;
		}
		Scene*	pScene = pMonster->getScene();
		if(!pScene)
		{
			Assert(pScene);
			return FALSE;
		}
		
		DAMAGE_MEM_LIST DamageMemList =	pMonster->GetKillerRec();
		if (DamageMemList.m_uCount > 0)
		{
			for(UINT i=0; i<DamageMemList.m_uCount; ++i)
			{
				pMonster->GetOwnerList().AddOwner(DamageMemList.m_DamageRec[i].m_Killer, DamageMemList.m_DamageRec[i].m_KillerObjID);
			}
		}
		return TRUE;

	__LEAVE_FUNCTION
		
		return FALSE;
}

BOOL	MonsterDropRuler::CaculateBossOwnerList(Obj_Monster* pMonster)
{
	__ENTER_FUNCTION

		if(!pMonster)	
		{
			Assert(pMonster);
			return FALSE;
		}
		Scene*	pScene = pMonster->getScene();
		if(!pScene)
		{
			Assert(pScene);
			return FALSE;
		}

		DAMAGE_MEM_LIST DamageMemList =	pMonster->GetKillerRec();
		if (DamageMemList.m_uCount > 0)
		{
			for(UINT i=0; i<DamageMemList.m_uCount; ++i)
			{
				pMonster->GetOwnerList().AddOwner(DamageMemList.m_DamageRec[i].m_Killer, DamageMemList.m_DamageRec[i].m_KillerObjID);
			}
		}
		return TRUE;

		__LEAVE_FUNCTION

			return FALSE;
}

#define MD_PICK_RANGE		(100.0f)
BOOL	MonsterDropRuler::CaculateCommDropRuler(Obj_Monster* pMonster, BOX_DISTRIBUTE_RULER DropRuler)
{
	__ENTER_FUNCTION

		if(!pMonster)
		{
			Assert(FALSE);
			return FALSE;
		}

		Scene*	pScene = pMonster->getScene();
		if(!pScene)
		{
			Assert(pScene);
			return FALSE;
		}
	
		Obj_ItemManager*	pIBManager = pScene->GetObj_ItemManager();
		Assert(pIBManager);

		UINT	iMonsterLevel	=	pMonster->GetLevel();
		UINT	iKillerLevel	=	pMonster->GetOccupantLevel();
		INT		iDataID	        =	pMonster->GetDataID();
		
		TeamID_t teamID			=   pMonster->GetOccupantTeamID();
		BYTE	nDistribMode    =   pMonster->GetDistribMode();
		BYTE	nLastOwnerPos   =   pMonster->GetLastOwnerPos() + 1;
		GUID_t	nTeamLeaderGuid	=   pMonster->GetOwnTeamLeader();
		MONSTER_OWNER_LIST ownerList = pMonster->GetOwnerList();
		FLOAT	fSearchLength	=	pMonster->GetDropSearchRange();

		INT nValidMemberCount = 0;						// 有效范围内符合级别条件的玩家数量
		INT nValidNewbieMemberCount = 0;

		Obj_Human *apValidMember[MAX_TEAM_MEMBER];
		BOOL bTeam = FALSE;
		if(pMonster->GetOccupantTeamID() != INVALID_ID)
		{
			bTeam = TRUE;
			if (nDistribMode == ITEM_INTURNS_DISTRIB)
			{
				Obj_Human* pHuman = NULL;
				for( UINT i=0; i<ownerList.OwnerCount; i++ )
				{
					if ((ownerList.OwnerDropList[i].m_Guid != INVALID_ID)&&(ownerList.OwnerDropList[i].m_ObjID!=INVALID_ID))
					{
						pHuman = pScene->GetHumanManager()->GetHuman(ownerList.OwnerDropList[i].m_ObjID);
						if (pHuman)
						{
							if (pHuman->GetGUID() == ownerList.OwnerDropList[i].m_Guid)
							{
								if (pHuman->IsInValidRadius( pMonster, fSearchLength ))
								{
									apValidMember[nValidMemberCount++] = pHuman;
								}
							}
						}
					}
				}
			}
		}

		Obj_ItemContaner	IBContaner = 
			pIBManager->CaculateItemDropFromMonster(iKillerLevel,iDataID, bTeam);

		//判断掉出物品数量是否小于0或大于最大值
		if(IBContaner.m_nCount<=0) 
		{
			ownerList.CleanUp();
			return FALSE;
		}
		if(IBContaner.m_nCount>8)
		{
			ownerList.CleanUp();
			return FALSE;
		}

		//物品掉落
		WORLD_POS	dropPos;
		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType		= ITEM_CREATE_FROM_MONSTER;
		ItemLogParam.SceneID	= pScene->SceneID();
		ItemLogParam.NpcType	= iDataID;

		if(pMonster->GetOwnerList().OwnerCount >= 1)
		{
			for(INT m=0; m<IBContaner.m_nCount; ++m)
			{
				BOOL aLive = FALSE;
				if(!pMonster->GetNearCanDropPos(dropPos))
				{
					return TRUE;
				}

				Obj_Item* pItem = pIBManager->CreateMonsterDropObj_Item(&dropPos, &IBContaner.m_Item[m]);	//创建Item;
				if (bTeam)
				{
					switch(nDistribMode)
					{
					case ITEM_FREE_DISTRIB:
						{
							for (UINT i=0; i<ownerList.OwnerCount; ++i)
							{
								pItem->SetOwner(ownerList.OwnerDropList[i].m_Guid);
							}
						}
						break;
					case ITEM_TEAMLEADER_DISTRIB:
						{
							if (pMonster->GetOwnerList().OwnerCount == 1)
							{
								pItem->SetOwner(ownerList.OwnerDropList[0].m_Guid);
							}
							else
							{
								pItem->SetOwner(nTeamLeaderGuid);
							}
						}
						break;
					case ITEM_INTURNS_DISTRIB:
						{
							if (pMonster->GetOwnerList().OwnerCount == 1)
							{
								pItem->SetOwner(ownerList.OwnerDropList[0].m_Guid);
							}
							else
							{
								if (nLastOwnerPos>=nValidMemberCount)
								{
									nLastOwnerPos = 0;
								}

								pItem->SetOwner(apValidMember[nLastOwnerPos]->GetGUID());
								if (m == IBContaner.m_nCount-1)
								{
									for (INT i=0; i<nValidMemberCount; ++i)
									{
										TeamInfo* pTeamInfo = apValidMember[i]->GetTeamInfo();
										pTeamInfo->SetLastOwnerPos(nLastOwnerPos);
									}
								}
								++nLastOwnerPos;
							}
						}
						break;
					case ITEM_SEPARATE_DISTRIB:
						{
							pItem->SetOwner(ownerList.OwnerDropList[0].m_Guid);
						}
						break;
					default:
						Assert(0);
						break;
					}
				}
				else
				{
					for (UINT i=0; i<ownerList.OwnerCount; ++i)
					{
						pItem->SetOwner(ownerList.OwnerDropList[i].m_Guid);
					}
				}

				pItem->SetDropObjID(pMonster->GetID());
				pItem->SetActiveFlag(TRUE);
				ItemLogParam.ItemGuid		=	pItem->GetObj_Item()->m_ItemGUID;
				ItemLogParam.ItemType		=	pItem->GetObj_Item()->m_ItemIndex;
				ItemLogParam.XPos			=	dropPos.m_fX;
				ItemLogParam.ZPos			=	dropPos.m_fZ;

				SaveItemLog(&ItemLogParam);
			}
		}
		ownerList.CleanUp();

		return TRUE;

		__LEAVE_FUNCTION

			return FALSE;
}

BOOL	MonsterDropRuler::CaculateBossDropRuler(Obj_Monster*	pMonster, BOX_DISTRIBUTE_RULER DropRuler)
{
	__ENTER_FUNCTION

		if(!pMonster)
		{
			Assert(FALSE);
			return FALSE;
		}

		Scene*	pScene = pMonster->getScene();
		if(!pScene)
		{
			Assert(pScene);
			return FALSE;
		}

		Obj_ItemManager*	pIBManager = pScene->GetObj_ItemManager();
		Assert(pIBManager);

		UINT	iMonsterLevel	=	pMonster->GetLevel();
		UINT	iKillerLevel	=	pMonster->GetOccupantLevel();
		INT		iDataID	        =	pMonster->GetDataID();

		TeamID_t teamID			=   pMonster->GetOccupantTeamID();
		BYTE	nDistribMode    =   pMonster->GetDistribMode();
		BYTE	nLastOwnerPos   =   pMonster->GetLastOwnerPos() + 1;
		GUID_t	nTeamLeaderGuid	=   pMonster->GetOwnTeamLeader();
		MONSTER_OWNER_LIST ownerList = pMonster->GetOwnerList();
		FLOAT	fSearchLength	=	pMonster->GetDropSearchRange();

		INT nValidMemberCount = 0;						// 有效范围内符合级别条件的玩家数量
		INT nValidNewbieMemberCount = 0;

		Obj_Human *apValidMember[MAX_TEAM_MEMBER];
		BOOL bTeam = FALSE;
		if(pMonster->GetOccupantTeamID() != INVALID_ID)
		{
			bTeam = TRUE;
			if (nDistribMode == ITEM_INTURNS_DISTRIB)
			{
				Obj_Human* pHuman = NULL;
				for( UINT i=0; i<ownerList.OwnerCount; i++ )
				{
					if ((ownerList.OwnerDropList[i].m_Guid != INVALID_ID)&&(ownerList.OwnerDropList[i].m_ObjID!=INVALID_ID))
					{
						pHuman = pScene->GetHumanManager()->GetHuman(ownerList.OwnerDropList[i].m_ObjID);
						if (pHuman)
						{
							if (pHuman->GetGUID() == ownerList.OwnerDropList[i].m_Guid)
							{
								if (pHuman->IsInValidRadius( pMonster, fSearchLength ))
								{
									apValidMember[nValidMemberCount++] = pHuman;
								}
							}
						}
					}
				}
			}
		}

		Obj_ItemContaner	IBContaner = 
			pIBManager->CaculateItemDropFromMonster(iKillerLevel,iDataID, bTeam);

		//判断掉出物品数量是否小于0或大于最大值
		if(IBContaner.m_nCount<=0) 
		{
			ownerList.CleanUp();
			return FALSE;
		}
		if(IBContaner.m_nCount>100)
		{
			return FALSE;
		}
		//物品掉落
		WORLD_POS	dropPos;
		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType		= ITEM_CREATE_FROM_MONSTER;
		ItemLogParam.SceneID	= pScene->SceneID();
		ItemLogParam.NpcType	= iDataID;
		
		if(pMonster->GetOwnerList().OwnerCount >= 1)
		{
			for(INT m=0; m<IBContaner.m_nCount; ++m)
			{	
				BOOL aLive = FALSE;
				if(!pMonster->GetNearCanDropPos(dropPos))
				{
					return TRUE;
				}
				
				Obj_Item* pItem = pIBManager->CreateMonsterDropObj_Item(&dropPos, &IBContaner.m_Item[m]);	//创建Item;
				if (bTeam)
				{
					switch(nDistribMode)
					{
					case ITEM_FREE_DISTRIB:
						{
							for (UINT i=0; i<ownerList.OwnerCount; ++i)
							{
								pItem->SetOwner(ownerList.OwnerDropList[i].m_Guid);
							}
						}
						break;
					case ITEM_TEAMLEADER_DISTRIB:
						{
							if (pMonster->GetOwnerList().OwnerCount == 1)
							{
								pItem->SetOwner(ownerList.OwnerDropList[0].m_Guid);
							}
							else
							{
								pItem->SetOwner(nTeamLeaderGuid);
							}
						}
						break;
					case ITEM_INTURNS_DISTRIB:
						{
							if (pMonster->GetOwnerList().OwnerCount == 1)
							{
								pItem->SetOwner(ownerList.OwnerDropList[0].m_Guid);
							}
							else
							{
								if (nLastOwnerPos>=nValidMemberCount)
								{
									nLastOwnerPos = 0;
								}

								pItem->SetOwner(apValidMember[nLastOwnerPos]->GetGUID());
								if (m == IBContaner.m_nCount-1)
								{
									for (INT i=0; i<nValidMemberCount; ++i)
									{
										TeamInfo* pTeamInfo = apValidMember[i]->GetTeamInfo();
										pTeamInfo->SetLastOwnerPos(nLastOwnerPos);
									}
								}
								++nLastOwnerPos;
							}
						}
						break;
					case ITEM_SEPARATE_DISTRIB:
						{
							pItem->SetOwner(ownerList.OwnerDropList[0].m_Guid);
						}
						break;
					default:
						Assert(0);
						break;
					}
				}
				else
				{
					for (UINT i=0; i<ownerList.OwnerCount; ++i)
					{
						pItem->SetOwner(ownerList.OwnerDropList[i].m_Guid);
					}
				}

				pItem->SetDropObjID(pMonster->GetID());
				pItem->SetActiveFlag(TRUE);
				ItemLogParam.ItemGuid		=	pItem->GetObj_Item()->m_ItemGUID;
				ItemLogParam.ItemType		=	pItem->GetObj_Item()->m_ItemIndex;
				ItemLogParam.XPos			=	dropPos.m_fX;
				ItemLogParam.ZPos			=	dropPos.m_fZ;

				SaveItemLog(&ItemLogParam);
			}
		}
		ownerList.CleanUp();

		return TRUE;

		__LEAVE_FUNCTION

			return FALSE;
}

//--------------------------------------------------------------------------------------------------------

BOOL	ItemRulerCheck::CheckTypeRuler(ITEM_RULER_LIST ruler,const UINT uItemIndex)
{
	__ENTER_FUNCTION

	Assert(uItemIndex!=0);
	Assert(uItemIndex!=INVALID_ID);
	
	UCHAR	uClass		= GetSerialClass(uItemIndex);

	switch(uClass)
	{
	case ICLASS_EQUIP:
		{
			EQUIP_TB* pGET =	g_ItemTable.GetEquipTB(uItemIndex);
			if(!pGET)
			{
				CHAR Buff[255];
				memset(Buff,0,255);
				sprintf(Buff,"找不到对应物品的表格定义，请确定装备资源正确 ItemType=%d",uItemIndex);
				AssertEx(pGET,Buff);
				g_pLog->FastSaveLog( LOG_FILE_1, "[ItemRulerCheck::CheckTypeRuler] GetEquipTB index is error [%d]",uItemIndex);
				return FALSE;
			}
			return CheckRuler(ruler,pGET->m_RulerID);
		}
		break;
	case ICLASS_MATERIAL: 
	case ICLASS_COMITEM:
	case ICLASS_IDENT:
		{
			COMMITEM_INFO_TB*	pGET		=		g_ItemTable.GetCommItemInfoTB(uItemIndex);
			if(!pGET)
			{
				CHAR Buff[255];
				memset(Buff,0,255);
				sprintf(Buff,"找不到对应物品的表格定义，请确定CommonItem资源正确 ItemType=%d",uItemIndex);
				AssertEx(pGET,Buff);
				g_pLog->FastSaveLog( LOG_FILE_1, "[ItemRulerCheck::CheckTypeRuler] GetCommItemInfoTB index is error [%d]",uItemIndex);				
				return FALSE;
			}

			return CheckRuler(ruler,pGET->m_nRulerID);
		}
		break;
	case ICLASS_TASKITEM:
		{
			TASKITEM_INFO_TB*	pGET		=		g_ItemTable.GetTaskItemInfoTB(uItemIndex);
			if(!pGET)
			{
				CHAR Buff[255];
				memset(Buff,0,255);
				sprintf(Buff,"找不到对应物品的表格定义，请确定TaskItem资源正确 ItemType=%d",uItemIndex);
				AssertEx(pGET,Buff);
				g_pLog->FastSaveLog( LOG_FILE_1, "[ItemRulerCheck::CheckTypeRuler] GetTaskItemInfoTB index is error [%d]",uItemIndex);								
				return FALSE;
			}

			return CheckRuler(ruler,pGET->m_nRulerID);
		}
		break;
	case ICLASS_GEM:
		{
			GEMINFO_TB*	pGET		=		g_ItemTable.GetGemInfoTB(uItemIndex);
			if(!pGET)
			{
				CHAR Buff[255];
				memset(Buff,0,255);
				sprintf(Buff,"找不到对应物品的表格定义，请确定Gem资源正确 ItemType=%d",uItemIndex);
				AssertEx(pGET,Buff);
				g_pLog->FastSaveLog( LOG_FILE_1, "[ItemRulerCheck::CheckTypeRuler] GetGemInfoTB index is error [%d]",uItemIndex);												
				return FALSE;
			}
			return CheckRuler(ruler,pGET->m_nRulerID);
		}
		break;
	case ICLASS_STOREMAP:
		{
			STORE_MAP_INFO_TB*	pGET		=		g_ItemTable.GetStoreMapTB(uItemIndex);
			if(!pGET)
			{
				CHAR Buff[255];
				memset(Buff,0,255);
				sprintf(Buff,"找不到对应物品的表格定义，请确定藏宝图资源正确 ItemType=%d",uItemIndex);
				AssertEx(pGET,Buff);
				g_pLog->FastSaveLog( LOG_FILE_1, "[ItemRulerCheck::CheckTypeRuler] GetStoreMapTB index is error [%d]",uItemIndex);																
				return FALSE;
			}
			return CheckRuler(ruler,pGET->m_nRulerID);
			break;
		}
	case ICLASS_EXTRABAG:
		{
			EXTRABAG_INFO_TB*	pGET		=		g_ItemTable.GetExtraBagInfoTB(uItemIndex);
			if(!pGET)
			{
				CHAR Buff[255];
				memset(Buff,0,255);
				sprintf(Buff,"找不到对应物品的表格定义，请确定藏宝图资源正确 ItemType=%d",uItemIndex);
				AssertEx(pGET,Buff);
				g_pLog->FastSaveLog( LOG_FILE_1, "[ItemRulerCheck::CheckTypeRuler] GetExtraBagInfoTB index is error [%d]",uItemIndex);
				return FALSE;
			}
			return CheckRuler(ruler,pGET->m_nRulerID);
			break;
		}
	case ICLASS_TALISMAN: 
		Assert(FALSE);
		break;
	case ICLASS_GUILDITEM: 
		Assert(FALSE);
		break;
	case ICLASS_SOUL_BEAD:
		{
			SOUL_BEAD_INFO_TB* pGET =	g_ItemTable.GetSoulBeadTB(uItemIndex);
			if(!pGET)
			{
				CHAR Buff[255];
				memset(Buff,0,255);
				sprintf(Buff,"找不到对应物品的表格定义，请确定装备资源正确 ItemType=%d",uItemIndex);
				AssertEx(pGET,Buff);
				g_pLog->FastSaveLog( LOG_FILE_1, "[ItemRulerCheck::CheckTypeRuler] GetEquipTB index is error [%d]",uItemIndex);
				return FALSE;
			}
			return CheckRuler(ruler,pGET->m_nRuleID);
		}
		break;
	case ICLASS_MOUNT:  // [7/8/2010 陈军龙]
		{
		MOUNT_INFO_TB*	pGET		=		g_ItemTable.GetMountInfoTB(uItemIndex);
		if(!pGET)
			{
			CHAR Buff[255];
			memset(Buff,0,255);
			sprintf(Buff,"找不到对应坐骑的表格定义，请确定坐骑资源正确 ItemType=%d",uItemIndex);
			AssertEx(pGET,Buff);
			g_pLog->FastSaveLog( LOG_FILE_1, "[ItemRulerCheck::CheckTypeRuler] GetMountInfoTB index is error [%d]",uItemIndex);
			return FALSE;
			}
		//return CheckRuler(ruler,pGET->m_nRulerID);
		return TRUE;

		break;
		}
	default:
		Assert(FALSE);
		break;
	}
	Assert(FALSE);
	return FALSE;
	__LEAVE_FUNCTION

	return FALSE;
}

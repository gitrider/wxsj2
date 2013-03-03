
#include "StdAfx.h"
#include "GMDataPool.h"
#include "NetWork\NetManager.h"
#include "GameStruct_Skill.h"
#include "GMDP_CharacterData.h"
#include "GIException.h"
#include "Global.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_Character.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "Object\Logic\Item\Obj_Item_Equip.h"
#include "CGAskItemInfo.h"
#include "CGAskStudyXinfa.h"
#include "CGDispelBuffReq.h"
#include "Procedure\GameProcedure.h"
#include "GITimeSystem.h"
#include "BuffImpactMgr.h"
#include "Action\GMActionSystem.h"
#include "DBC/GMDataBase.h"
#include "Event\GMEventSystem.h"
#include "CGAskItemInfo.h"
#include "FakeObject\GMFakeObjSystem.h"
#include "CGCharUseSkill.h"
#include "GMDP_Struct_Relation.h"
#include "GMDP_Struct_Mail.h"
#include "GMDP_Struct_Guild.h"
#include "interface\GMInterface_Script_Talk.h"
#include "GIUtil.h"
#include "Action\GMActionSystem_MouseCmd.h"
#include "GIGameInterface.h"
#include "..\Object\Logic\TripperObj\GMTripperObj_ItemDrop.h"
#include "..\..\..\Common\Utilities\SplitStringLevelOne.h"
#include "SplitStringLevelTwo.h"

#include "CGGuild.h"
#include "Network\NetManager.h"
#include "GameStruct_MinorPasswd.h"
#include "Object\Logic\Item\Obj_Item_Mount.h"  //  [7/20/2010 陈军龙]


BOOL CDataPool::CopyTextToClipboard(HWND hWnd,LPCSTR content)
{
	size_t len = strlen(content);
	if( OpenClipboard(hWnd) )                    // 打开剪切板
	{
		EmptyClipboard();                          // 清空剪切板

		HANDLE hclip = NULL;                    
		char* buf   = NULL;
		hclip = GlobalAlloc(GMEM_MOVEABLE,len+1); // 在堆上分配制定数目的字节
		Assert( hclip != NULL );
		buf   = (char*)GlobalLock(hclip);          // 对内存块加锁,并返回它的内存地址
		Assert( buf != NULL );

		memset(buf,0,len+1);
		memcpy(buf,content,len);                    // 将字符拷贝到内存中

		GlobalUnlock(hclip);                            // 解锁
		SetClipboardData(CF_TEXT,hclip);    //向剪切板上放置内存中的数据
		CloseClipboard();                       // 关闭剪切板

		//GlobalFree(hclip);

		return TRUE;
	}
	return FALSE;
}

BOOL CDataPool::CopyTextFromClipboard(HWND hWnd, LPCSTR pBuf )
{
	if (OpenClipboard(hWnd) )//打开剪贴板
	{
		if (IsClipboardFormatAvailable(CF_TEXT))//判断格式是否是我们所需要
		{
			HANDLE hClip;
			char* pBuf;

			//读取数据
			hClip=GetClipboardData(CF_TEXT);
			pBuf=(char*)GlobalLock(hClip);
			GlobalUnlock(hClip);
			CloseClipboard();
		}
		return TRUE;
	}
	return FALSE;
}

void CDataPool::_Skill_OnUpdatePlayerAttrib(const EVENT* pEvent, UINT dwOwnerData)
{
	CActionSystem::GetMe()->UserSkill_Update();
}


KL_IMPLEMENT_DYNAMIC(CDataPool, GETCLASS(tDataPool));

CDataPool* CDataPool::s_pMe = NULL;


CDataPool::CDataPool()
{
	s_pMe = this;

	m_nIdentifyPosition = -1;

	m_vCoolDownGroup.resize(COOLDOWN_LIST_SIZE);
	m_vPetSkillCoolDownGroup.resize(COOLDOWN_LIST_SIZE);
	m_vQuestTimeGroup.resize(QUESTTIME_LIST_SIZE);

	m_vUserEquip.resize(HEQUIP_NUMBER, 0);
	m_vOtherPlayerEquip.resize(HEQUIP_NUMBER, 0);
	//m_vUserSouXia.resize(MAX_SOUXIA_CONTAINER,0);
	m_vUserBag.resize(MAX_BAG_SIZE, 0);
	m_vItemBox.resize(MAX_BOXITEM_NUMBER, 0);
	m_vtUserBagClone.resize(MAX_BAG_SIZE);
    m_vUserMount.resize(MAX_MOUNT_SIZE);//  [7/8/2010 陈军龙]
	
    //初始化商人货架
	m_nBoothNumber = 0;
	m_bBoothWindowIsDirty = TRUE;
//	m_bIsRepairing = FALSE;
	m_listBooth.resize(MAX_BOOTH_NUMBER, 0);
	m_listBoothSold.resize(MAX_BOOTH_SOLD_NUMBER, 0);
	m_nlistPrice.resize(MAX_BOOTH_NUMBER, 0);
	m_nBuyType=-1;			// 商店的收购类型
	m_nRepairType=-1;		// 修理类型
	
	//add by gh 初始化商城
	m_nIBShopNumber = 0;
	m_listIBShop.resize(MAX_IBSHOP_NUMBER, 0);
	m_nlistIBPrice.resize(MAX_IBSHOP_NUMBER, 0);
	m_nlistNewSell.resize(MAX_IBSHOP_NUMBER, 0);
	//初始化自己的银行
	m_CurBankEndIndex	=	0;
	m_listUserBank.resize(MAX_BANK_SIZE, 0);
	
	// 创建关系数据
	m_pRelation = new Relation;

	// 创建邮件数据池
	m_pMailPool = new CMailPool;

	//初始化自己的交易盒
	m_MyExchangeBox.CleanUp();

	//初始化对方的交易盒
	m_OtherExchangeBox.CleanUp();

	//当前与自己交易的人
	m_CurExID = 0;

	// 初始化自己的宝石合成/镶嵌界面
//	m_vPageEnchase.resize(2);
	//初始化申请交易者的队列
	m_listApplicantsID.resize(MAX_EXCHANGE_APPLICANTS_NUM, 0);
	m_pOut = 0;
	m_pIn = 0;

	//初始化任务物品列表
	m_MissionItemList.resize(MISSION_BOX_SIZE,0);

//	m_vAppendItem.resize((HEQUIP_NUMBER+MAX_BAG_SIZE)*MAX_ITEM_GEM,0);

	//摊位盒初始化
	m_MyStallBox.CleanUp();
	m_OtStallBox.CleanUp();

	m_PlayerShopList.resize(MAX_SHOP_NUM_PER_SCENE);

	m_bMeIsOwner = FALSE;

	m_nBuffImpactTime	= 0;

	m_X_SCRIPT.CleanUp();
	m_X_PARAM.CleanUp();

	m_nShopNum = 0;

	m_GuildNum = 0;
	m_GuildMemNum = 0;
	m_GuildMaxMemNum = 0;
	m_uAccess	=	0;

	m_pTargetEquip    = NULL;

	m_GuildDesc = "";


	//初始化骑乘宠物门派技能学习列表
	m_petSkillStudyList.resize(0);

	m_MyPlayerShop.CleanUp();
	m_OtPlayerShop.CleanUp();
	m_CommerFactor = 0;

	m_MyPsSelectItem.CleanUp();
	m_OtPsSelectItem.CleanUp();

	m_GuildInviteId = INVALID_ID;
	m_GuildInviteName = "";
	m_GuildInviteObjName = "";

	m_iGuildFamilyNum = 0;
	m_iSelectFamilyId = -1;

	m_FingerPlayerNum = 0;

	m_AtriAutoPoint = 0;


	// 称号
	for( INT i = 0; i < 3; ++i )
	{
		m_iTitleNum[i] = 0;
		m_iCurrTitleId[i] = 0;

		memset( m_vTitles, 0, sizeof(INT)*3*MAX_TITLE_SIZE );
	}


	//清空空套装
	m_nSetRow = 0;
	AllSetInfo.clear();
	AllSetEquipList.clear();
	AllSetEquipName.clear();
	memset(m_SelfActiveSetId ,-1 ,sizeof(m_SelfActiveSetId));
	m_SelfActiveSetNum = 0;
	memset(m_SelfActiveSetEquipNum, 0, sizeof(m_SelfActiveSetEquipNum));
	memset(m_OtherActiveSetId ,-1 ,sizeof(m_OtherActiveSetId));
	m_OtherActiveSetNum = 0;
	memset(m_OtherActiveSetEquipNUm, 0, sizeof(m_SelfActiveSetEquipNum));


	AllSpritActiveChainEqual.clear();
	AllSpritActiveChainSeries.clear();
	AllSpritActiveChainJiang.clear();
	memset(m_SelfSpritActive,0,sizeof(m_SelfSpritActive));
	memset(m_OterSpritActive,0,sizeof(m_OterSpritActive));
	memset(m_SelfSpritActiveAny,0,sizeof(m_SelfSpritActiveAny));
	memset(m_OterSpritActiveAny,0,sizeof(m_OterSpritActiveAny));


	AllSpritActiveInfo.clear();
	m_SelfAllSprite_Active = 0;
	m_OtherAllSprite_Active = 0;

 
	//
	for( INT i = 0; i < 11; ++i )
		m_nMapIndex[i] = -1;
	m_nMapIndexNum = 0;

	// 财产保护
	m_PasswordData = 0;
	m_InputPassword	= "";

	for( INT i = 0; i < 5; ++i )
		m_InputPasswdState[i] = FALSE;

	//m_vUserSouXia.resize(MAX_SOUXIA_CONTAINER,0);
	for ( INT i=0; i<MAX_SOUXIA_CONTAINER; ++i )
	{
		m_vUserSouXia[i] = NULL;
	}
}

CDataPool::~CDataPool()
{
	// 销毁关系数据
	SAFE_DELETE(m_pRelation);
	SAFE_DELETE(m_pMailPool);
}

VOID CDataPool::Initial(VOID*)
{
	//初始化角色技能冷却组
	for(INT i=0; i<COOLDOWN_LIST_SIZE; i++)
	{
		m_vCoolDownGroup[i].nTime = m_vCoolDownGroup[i].nTotalTime = -1;
	}

	//初始化任务时间组
	for(INT i=0; i<QUESTTIME_LIST_SIZE; i++)
	{
		m_vQuestTimeGroup[i] = -1;
	}
	m_nCommonCoolDown = -1;

	// 初始化骑宠技能冷却组
	for(INT i=0; i<COOLDOWN_LIST_SIZE; i++)
	{
		m_vPetSkillCoolDownGroup[i].nTime = m_vPetSkillCoolDownGroup[i].nTotalTime = -1;
	}

	// 初始化骑宠数据
	CHAR name[64] = {0};
	for(INT i=0; i<HUMAN_PET_MAX_COUNT; i++)
	{
		m_listPet[i].CleanUp();

		m_listPetAvatar[i].CleanUp();

		sprintf(name, "My_Pet_%d", i);
		m_listPetAvatar[i].strAvatarName = name;
		m_listPetAvatar[i].pAvatar = (CObject_PlayerNPC*)(CFakeObjSystem::GetMe()->NewFakeObject("CObject_PlayerNPC", 
										m_listPetAvatar[i].strAvatarName.c_str(), "Camera_Main"));
		if (m_listPetAvatar[i].pAvatar)
		{
			m_listPetAvatar[i].pAvatar->SetNpcType(NPC_TYPE_MOUNT);
		}
	}

	// 初始化坐骑数据  [7/14/2010 陈军龙]
	for(INT i=0; i<MAX_MOUNT_SIZE; i++)
	{
		m_listMountAvatar[i].CleanUp();

		sprintf(name, "My_Mount_%d", i);
		m_listMountAvatar[i].strAvatarName = name;
		m_listMountAvatar[i].pAvatar = (CObject_PlayerNPC*)(CFakeObjSystem::GetMe()->NewFakeObject("CObject_PlayerNPC", 
										m_listMountAvatar[i].strAvatarName.c_str(), "Camera_Main"));

		if (m_listMountAvatar[i].pAvatar)
		{
			m_listMountAvatar[i].pAvatar->SetNpcType(NPC_TYPE_MOUNT);
		}
	}

	m_TargetPet.CleanUp();
	m_TargetPetAvatar.CleanUp();
	m_TargetPetAvatar.strAvatarName = "My_TargetPet";
	m_TargetPetAvatar.pAvatar = (CObject_PlayerNPC*)(CFakeObjSystem::GetMe()->NewFakeObject("CObject_PlayerNPC", 
									m_TargetPetAvatar.strAvatarName.c_str(), "Camera_Main"));
	if (m_TargetPetAvatar.pAvatar)
	{
		m_TargetPetAvatar.pAvatar->SetNpcType(NPC_TYPE_MOUNT);
	}


	m_nBuffImpactTime = CGameProcedure::s_pTimeSystem->GetTimeNow();

	CEventSystem::GetMe()->RegisterEventHandle("UNIT_HP", _Skill_OnUpdatePlayerAttrib);
	CEventSystem::GetMe()->RegisterEventHandle("UNIT_MP", _Skill_OnUpdatePlayerAttrib);
	CEventSystem::GetMe()->RegisterEventHandle("UNIT_RAGE", _Skill_OnUpdatePlayerAttrib);
	CEventSystem::GetMe()->RegisterEventHandle("UNIT_EQUIP_WEAPON", _Skill_OnUpdatePlayerAttrib);




	//打开数据表 //equipset_attr.tab装备的套装
	DBC_DEFINEHANDLE(s_pSet_Equip, DBC_EQUIP_SET_INFO);
	//搜索纪录
	_DBC_EQUIP_SET_INFO* pSetEquip = NULL; 

	  m_nSetRow  =    s_pSet_Equip->GetRecordsNum();
	  Equip_Id    tempSt;

	for( INT idTable = 1 ;idTable<= m_nSetRow  ; ++ idTable)
	{

			
		pSetEquip = ( _DBC_EQUIP_SET_INFO*)s_pSet_Equip->Search_Index_EQU((UINT)idTable);

		if(pSetEquip) 
		{

			tempSt.CleanUp();
			EquipSet_Attr_Name    att_name;
			att_name.CleanUp();


			att_name.m_Name = pSetEquip->szSetName;

			for(INT i = 0 ;i< EQUIP_SET_ATTR_NUM  ;++i)
			{
				att_name.m_stAttr[i].nAttrId =  pSetEquip->stAttr[i].nAttrId;
				att_name.m_stAttr[i].nAttrNum = pSetEquip->stAttr[i].nAttrNum;
				att_name.m_stAttr[i].nAttrActiveNum = pSetEquip->stAttr[i].nAttrActiveNum;
			}


			AllSetInfo[pSetEquip->nSetId] = pSetEquip->nSetNum;
			AllSetEquipName[pSetEquip->nSetId] = att_name;

			const CHAR*  pSplitString	=	pSetEquip->szId;

			SplitStringLevelOne pSplitL1;
			SplitStringLevelTwo pSplitL2;
			pSplitL1.Reset();
			pSplitL2.Reset();
			pSplitL1.Init( '|', &pSplitL2 );
			pSplitL2.Init( '~', NULL );
			pSplitL1.DoSplit( pSplitString );
			INT iLineCount	= pSplitL2.GetResultLineCount();

			for( INT nID = 0; nID< iLineCount; ++nID )
			{
				tempSt.nIdList[nID] = atoi(pSplitL2.GetResultLine(nID));

			}
			
			tempSt.nValue = iLineCount;
			AllSetEquipList[pSetEquip->nSetId] = tempSt;


		}
	}


	//加载完成所有套装信息


	//记录小激活


	Euuip_Sprite_Chain   temSpriteChain;

	//打开数据表//chain_info.tab
	//3张牌组
	DBC_DEFINEHANDLE(s_pCardChain_Equip, DBC_EQUIP_CARD_GROUP_INFO);
	//搜索纪录
	_DBC_EQUIP_CARD_GROUP_INFO* pEquipCardChain = NULL; 


	for ( INT i =0 ; i<= max_sprite_chain_num ; ++i) 
	{
		pEquipCardChain = ( _DBC_EQUIP_CARD_GROUP_INFO*)s_pCardChain_Equip->Search_Index_EQU((UINT)i);

		if (pEquipCardChain )
		{
			temSpriteChain.m_pai1 = pEquipCardChain->nNeedCardId1;
			temSpriteChain.m_pai2 = pEquipCardChain->nNeedCardId2;
			temSpriteChain.m_pai3 = pEquipCardChain->nNeedCardId3;

			if (	Equip_Card_Is_Equal(temSpriteChain.m_pai1,temSpriteChain.m_pai2,temSpriteChain.m_pai3))
			{
					AllSpritActiveChainEqual[pEquipCardChain->nGroupId] = temSpriteChain;
			}else if(	Equip_Card_Is_Series(temSpriteChain.m_pai1,temSpriteChain.m_pai2,temSpriteChain.m_pai3) )
			{

				AllSpritActiveChainSeries[pEquipCardChain->nGroupId] = temSpriteChain;
			}else if (Equip_Card_Is_Jiang(temSpriteChain.m_pai1,temSpriteChain.m_pai2) && (temSpriteChain.m_pai3 ==-1))
			{

				AllSpritActiveChainJiang[pEquipCardChain->nGroupId] = temSpriteChain;
			}
		
		}
		
	}
 

	//记录大激活

	Euuip_Sprite_Soul   temSpriteSoul;
	//打开数据表 
	DBC_DEFINEHANDLE(s_pCardSoul_Equip, DBC_EQUIP_CARD_ALL);
	//搜索纪录
	_DBC_EQUIP_CARD_ALL* pEquipCardSoul = NULL; 


	for ( INT i = 0 ; i<= max_sprite_all_active_num ; ++i) 
	{
		pEquipCardSoul = ( _DBC_EQUIP_CARD_ALL*)s_pCardSoul_Equip->Search_Index_EQU((UINT)i);

		if (pEquipCardSoul )
		{
			temSpriteSoul.m_Chain1 = pEquipCardSoul->nNeedGropId1;
			temSpriteSoul.m_Chain2 = pEquipCardSoul->nNeedGropId2;
			temSpriteSoul.m_Chain3 = pEquipCardSoul->nNeedGropId3;
			temSpriteSoul.m_Chain4 = pEquipCardSoul->nNeedGropId4;
			temSpriteSoul.m_Chain5 = pEquipCardSoul->nNeedGropId5;
			AllSpritActiveInfo[pEquipCardSoul->nGroupId] = temSpriteSoul;

		}

	}



}

VOID CDataPool::Tick(VOID)
{
	//TickTime 单位: 毫秒
	INT nTickTime = (INT)(CGameProcedure::s_pTimeSystem->GetLoopTime() * 1000.f);
	
	// 优化过的，多一个if语句，总比每个tick都“-=”时间要好。
	tActionItem* NextAction;
	if( m_nCommonCoolDown > 0 )
	{
		m_nCommonCoolDown -= nTickTime;
		if( m_nCommonCoolDown <= 0 )
		{
			m_nCommonCoolDown = 0;
			NextAction = CActionSystem::GetMe()->GetSetoutAction();
			if(NextAction)
			{
//				NextAction->DoAction();
				CActionSystem::GetMe()->SetSetoutAction(NULL);	
			}
		}
	}

	for(INT i=0; i<QUESTTIME_LIST_SIZE; i++)
	{
		// 优化过的，多一个if语句，总比每个tick都“-=”时间要好。
		if( m_vQuestTimeGroup[i] > 0 )
		{
			m_vQuestTimeGroup[i] -= nTickTime;
			if( m_vQuestTimeGroup[i] <= 0 )
				m_vQuestTimeGroup[i] = 0;
		}
	}

	//冷却组更新

	// 待优化
	for(INT i=0; i<COOLDOWN_LIST_SIZE; i++)
	{
		COOLDOWN_GROUP& rCoolDown = m_vCoolDownGroup[i];
		if(0<rCoolDown.nTime)
		{
			rCoolDown.nTime -= nTickTime;
			if(rCoolDown.nTime < 0)
			{
				rCoolDown.nTime = 0;
			}
		}
	}

	//骑乘宠物冷却组更新

	// 待优化
	for(INT i=0; i<COOLDOWN_LIST_SIZE; i++)
	{
		COOLDOWN_GROUP& rCoolDown = m_vPetSkillCoolDownGroup[i];
		if (0 < rCoolDown.nTime)
		{
			rCoolDown.nTime -= nTickTime;
			if (0 > rCoolDown.nTime)
			{
				rCoolDown.nTime = 0;
			}
		}
	}

	// 待优化
	UINT uTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();
	INT nIntervalTime = (INT)(uTimeNow) - m_nBuffImpactTime;
	if( nIntervalTime >= 1000 )
	{
		m_nBuffImpactTime = (INT)(uTimeNow);
		if( m_mapBuffImpact.size() > 0 )
		{
			_BUFF_IMPACT_INFO *pBuffImpact;
			CBuffImpactMap::iterator itCur, itEnd;
			
			itCur		= m_mapBuffImpact.begin();
			itEnd		= m_mapBuffImpact.end();
			
			while ( itCur != itEnd )
			{
				pBuffImpact = itCur->second;
				if ( pBuffImpact->m_nTimer > 0 )
				{
					pBuffImpact->m_nTimer -= nIntervalTime;
					if ( pBuffImpact->m_nTimer < 0 )
					{
						pBuffImpact->m_nTimer = 0;
					}
				}
				itCur++;
			}

//			CEventSystem::GetMe()->PushEvent( GE_IMPACT_SELF_UPDATE_TIME );

		}
	}

 //掉落包tick
	ItemDrop_Tick();

}

VOID CDataPool::Release(VOID)
{
	//清空玩家装备
	UserEquip_Clear();
	//清空玩家背包
	UserBag_Clear();
	//清空箱子
	ItemBox_Clear();
	//清空货架
	Booth_Clear();
	Booth_Sold_Clear();

	IBShop_Clear();
	//清空玩家搜侠数据
	UserSouXia_Clear();

//	m_pPetAvatar = NULL;
//	m_pTargetPetAvatar = NULL;
//	m_pPetStudySkillAvatar = NULL;
}


CCharacterData* CDataPool::CharacterData_Create(CObject_Character* pCharacterData)
{
	KLAssert(pCharacterData);

	//创建新的角色数据
	CCharacterData* pNewCharacterData = new CCharacterData(pCharacterData);

	m_mapAllCharacterData.insert(std::make_pair(pCharacterData->GetID(), pNewCharacterData));

	return pNewCharacterData;
}

void CDataPool::CharacterData_Destroy(CObject_Character* pCharacterData)
{
	KLAssert(pCharacterData);

	std::map< INT, CCharacterData* >::iterator it = 
		m_mapAllCharacterData.find(pCharacterData->GetID());

	if(it == m_mapAllCharacterData.end()) return;

	//销毁
	delete (CCharacterData*)(it->second);
	m_mapAllCharacterData.erase(it);
}

CCharacterData* CDataPool::CharacterData_Get( INT nID )
{
	std::map< INT, CCharacterData* >::iterator it;

	it = m_mapAllCharacterData.find(nID);

	if(it == m_mapAllCharacterData.end())
		return NULL;

	return it->second;
}

//==================================================================================
const CDataPool::COOLDOWN_GROUP* CDataPool::CoolDownGroup_Get(INT nCoolDownID) const
{
	if(nCoolDownID<(INT)m_vCoolDownGroup.size())
		return &(m_vCoolDownGroup[nCoolDownID]);
	else
		return NULL;
}

VOID CDataPool::CoolDownGroup_UpdateList(const Cooldown_T *pCoolDownList, INT nUpdateNum)
{
	if(nUpdateNum<=0) return;
	if(nUpdateNum > (INT)m_vCoolDownGroup.size()) nUpdateNum = (INT)m_vCoolDownGroup.size();
	INT nCooldownID = INVALID_ID;
	for(INT i=0; nUpdateNum>i; i++)
	{
		nCooldownID = pCoolDownList[i].GetID();
		if(0> nCooldownID || COOLDOWN_LIST_SIZE<=nCooldownID) continue;

		//更新冷却组
		struct COOLDOWN_GROUP& rCooldown = m_vCoolDownGroup[nCooldownID]; 
		rCooldown.nTotalTime = pCoolDownList[i].GetCooldownTime();
		rCooldown.nTimeElapsed = pCoolDownList[i].GetCooldownElapsed();
		rCooldown.nTime = rCooldown.nTotalTime - rCooldown.nTimeElapsed;

		//刷新的ActionSystem
		CActionSystem::GetMe()->UpdateCoolDown(nCooldownID);
	}
}

//==================================================================================
const CDataPool::COOLDOWN_GROUP* CDataPool::PetSkillCoolDownGroup_Get(INT nCoolDownID) const
{
	if (nCoolDownID >= 0 && nCoolDownID<(INT)m_vCoolDownGroup.size())
		return &(m_vPetSkillCoolDownGroup[nCoolDownID]);

	return NULL;
}

VOID CDataPool::PetSkillCoolDownGroup_UpdateList(const Cooldown_T *pCoolDownList, INT nUpdateNum,const PET_GUID_t nPet_GUID)
{
	if (NULL == pCoolDownList)
		return;
	if (nUpdateNum<=0)
		return;

	if(nUpdateNum > COOLDOWN_LIST_SIZE)
	{
		nUpdateNum = COOLDOWN_LIST_SIZE;
		KLThrow("PetSkillCoolDownGroup_UpdateList(), nUpdateNum 数值越界");
	}

	INT nCooldownID = INVALID_ID;

	for (INT i=0; i<nUpdateNum; ++i)
	{
		nCooldownID = pCoolDownList[i].GetID();
		if (nCooldownID > 0 && nCooldownID < COOLDOWN_LIST_SIZE)
		{
			//更新冷却组
			struct COOLDOWN_GROUP& rCooldown	= m_vPetSkillCoolDownGroup[nCooldownID]; 

			rCooldown.nTotalTime				= pCoolDownList[i].GetCooldownTime();
			rCooldown.nTimeElapsed				= pCoolDownList[i].GetCooldownElapsed();
			rCooldown.nTime						= rCooldown.nTotalTime - rCooldown.nTimeElapsed;

			//刷新的ActionSystem
			CActionSystem::GetMe()->UpdateCoolDown(nCooldownID);
		}
	}
}

//任务时间组
const INT CDataPool::QuestTimeGroup_Get(INT nQuestTimeID) const
{
	if(nQuestTimeID<(INT)m_vQuestTimeGroup.size())
		return m_vQuestTimeGroup[nQuestTimeID];
	else
		return NULL;
}

VOID CDataPool::QuestTimeGroup_UpdateList(const INT nQuestTime, INT nUpdateNum)
{
	if(nUpdateNum<=0) return;
	if(nUpdateNum > (INT)m_vQuestTimeGroup.size()) nUpdateNum = (INT)m_vQuestTimeGroup.size();

	m_vCoolDownGroup[nUpdateNum].nTime = m_vCoolDownGroup[nUpdateNum].nTotalTime	= (INT)(nQuestTime);

	//刷新某处么？
	//CActionSystem::GetMe()->UpdateCoolDown(i);

}
//==================================================================================
//玩家装备系列
VOID CDataPool::UserEquip_Clear(VOID)
{
	for(INT i=0; i<HEQUIP_NUMBER; i++)
	{
		if(m_vUserEquip[i])
		{
			CObject_Item::DestroyItem((CObject_Item*)m_vUserEquip[i]);
			m_vUserEquip[i] = NULL;
		}
	}
}

VOID CDataPool::UserEquip_SetItem(HUMAN_EQUIP ptEquip, tObject_Item* pEquipItem, BOOL bClearOld)
{	
	if(m_vUserEquip[ptEquip] && bClearOld)
	{
		CObject_Item::DestroyItem((CObject_Item*)m_vUserEquip[ptEquip]);
	}

	if(pEquipItem)
	{
	  /* HUMAN_EQUIP pointEquip = ((CObject_Item_Equip *)pEquipItem)->GetItemType();
		if(pointEquip != ptEquip) return;*/

		// 设置所属
		pEquipItem->SetTypeOwner(tObject_Item::IO_MYSELF_EQUIP);

		// 设置装备在action item 中的位置.
		pEquipItem->SetPosIndex(ptEquip);
		m_vUserEquip[ptEquip] = pEquipItem;

	}
	else
	{
		// 在装备点清空数据
		m_vUserEquip[ptEquip] = pEquipItem;
	}



	
}

tObject_Item* CDataPool::UserEquip_GetItem(HUMAN_EQUIP ptEquip)
{
	if(ptEquip < 0 || ptEquip >= HEQUIP_NUMBER) return NULL;

	return m_vUserEquip[ptEquip];
}

//==================================================================================
//玩家坐骑系列   [7/8/2010 陈军龙] 
VOID CDataPool::UserMount_Clear(VOID)
{
	for(INT i=0; i<MAX_MOUNT_SIZE; i++)
	{
		if(m_vUserMount[i])
		{
			CObject_Item::DestroyItem((CObject_Item*)m_vUserMount[i]);
			m_vUserMount[i] = NULL;
		}
	}
}

VOID CDataPool::UserMount_SetItem(INT nIndex, tObject_Item* pMountItem, BOOL bClearOld)
{	
	if(m_vUserMount[nIndex] && bClearOld)
	{
		CObject_Item::DestroyItem((CObject_Item*)m_vUserMount[nIndex]);
	}

	if(pMountItem)
	{
		// 设置所属
		pMountItem->SetTypeOwner(tObject_Item::IO_MYSELF_MOUNT);

		// 设置装备在action item 中的位置.
		pMountItem->SetPosIndex(nIndex);
		m_vUserMount[nIndex] = pMountItem;

	}
	else
	{
		// 在装备点清空数据
		m_vUserMount[nIndex] = pMountItem;
	}

}

tObject_Item* CDataPool::UserMount_GetItem(INT nIndex)
{
	if(nIndex < 0 || nIndex >= MAX_MOUNT_SIZE) return NULL;

	return m_vUserMount[nIndex];
}

INT CDataPool::UserMount_GetItemIndex(tObject_Item* itemMount)
{
	for(INT i=0; i<MAX_MOUNT_SIZE; i++)
	{
		if(m_vUserMount[i] == itemMount)
		{
			return i;
		}
	}
	return -1;
}

//套装是否组合成功
// 返回的数量 0表示没有套装 1表示1套 2表示2套
INT CDataPool::UserEquip_IsUnion(VOID)
{
	/*
		会出现套装的装备
		HEQUIP_MAINHAND		=0,     //主手
		HEQUIP_ASSIHAND		=1,     //副手
		HEQUIP_HEAD			=2,		//头
		HEQUIP_NECK			=3,		//颈
		HEQUIP_SHOULDER		=4,		//肩
		HEQUIP_BACK			=5,		//躯干
		HEQUIP_WRIST		=6,		//腕
		HEQUIP_HAND			=7,		//手
		HEQUIP_WAIST		=8,		//腰
		HEQUIP_FEET			=9,		//脚
		HEQUIP_RING1		=10,	//戒指1
		HEQUIP_RING2		=11,	//戒指2
		HEQUIP_ADORN1		=12,	//饰品1
		HEQUIP_ADORN2		=13,	//饰品2
	
		人物身上可以存在的套装数量可能不只是一套
	*/


  
	//玩家的装备信息

  INT nSetID   = -1;
  std::map<INT , INT>	 playerSetInfo   ;//保存套装id对应的装备数量

  for (  INT i = HEQUIP_MAINHAND; i <= HEQUIP_ADORN2 ; i++ )
  {
		if (m_vUserEquip[i])
		{
		  if( static_cast<CObject_Item_Equip*>(m_vUserEquip[i])->GetDur() > 0 )
		  {
			 nSetID  	= static_cast<CObject_Item_Equip*>(m_vUserEquip[i])->GetSetID();
			if (nSetID > 0)
			{
				playerSetInfo[nSetID]++;
			}
	
		  }
	    }		 
  }
   //记录玩家激活的套装id	
  memset(m_SelfActiveSetId ,-1 ,sizeof(m_SelfActiveSetId));
  m_SelfActiveSetNum = 0;
  memset(m_SelfActiveSetEquipNum, 0, sizeof(m_SelfActiveSetEquipNum));
 
  for ( std::map<INT ,INT>::iterator it = playerSetInfo.begin() ; it!= playerSetInfo.end(); ++it)
	{
			//if (it->second  >= AllSetInfo [it->first] )
			{
				m_SelfActiveSetId[m_SelfActiveSetNum]  = it->first; // 是套装
				m_SelfActiveSetEquipNum[m_SelfActiveSetNum++] = it->second;	//当前已经激活的装备个数
			}
			   
	}

 return m_SelfActiveSetNum;
 
	
}

/**牌型是否组合成功,	局部的激活 */
BOOL		CDataPool::UserEquipSprite_IsActive( EM_EQUIP_CARD_GROUP index)
{

	// 主手 副手						 4
	//帽  项链 护腕                      1
	//戒指1 戒指2  鞋子					 2		
	//肩 衣服 手套						 3
	//饰品 1 饰品2  腰					 0
		//HEQUIP_MAINHAND		=0,     //主手
		//HEQUIP_ASSIHAND		=1,     //副手
		//HEQUIP_HEAD			=2,		//头
		//HEQUIP_NECK			=3,		//颈
		//HEQUIP_SHOULDER		=4,		//肩
		//HEQUIP_BACK			=5,		//躯干
		//HEQUIP_WRIST		=6,		//腕
		//HEQUIP_HAND			=7,		//手
		//HEQUIP_WAIST		=8,		//腰
		//HEQUIP_FEET			=9,		//脚
		//HEQUIP_RING1		=10,	//戒指1
		//HEQUIP_RING2		=11,	//戒指2
		//HEQUIP_ADORN1		=12,	//饰品1
		//HEQUIP_ADORN2		=13,	//饰品2
		//HEQUIP_SUIT			=14,	//外装
		//HEQUIP_RIDER		=15,	//坐骑
		//HEQUIP_NUMBER,				//主角装备点总数

	if ( index < EM_EQUIP_CARD_GROUP_CHAIN1  ||index >= EM_EQUIP_CARD_GROUP_MAXNUM)
	{
		return FALSE ;
	}


	 m_SelfSpritActive[index] = 0;  
	 m_SelfSpritActiveAny[index] = 0;

	CObject_Item_Equip *temp1 = NULL ,*temp2 = NULL ,*temp3 = NULL; 

	switch(index)
	{
		case EM_EQUIP_CARD_GROUP_CHAIN1:
			{
				temp1  =static_cast<CObject_Item_Equip *>(m_vUserEquip[HEQUIP_ADORN1]);
				temp2  =static_cast<CObject_Item_Equip *>(m_vUserEquip[HEQUIP_ADORN2]);
				temp3  =static_cast<CObject_Item_Equip *>(m_vUserEquip[HEQUIP_WAIST]);

			}
 			break;
		case EM_EQUIP_CARD_GROUP_CHAIN2:
			{
				temp1  =static_cast<CObject_Item_Equip *>(m_vUserEquip[HEQUIP_HEAD]);
				temp2  =static_cast<CObject_Item_Equip *>(m_vUserEquip[HEQUIP_NECK]);
				temp3  =static_cast<CObject_Item_Equip *>(m_vUserEquip[HEQUIP_WRIST]);
				 
			}
 			break;
		case EM_EQUIP_CARD_GROUP_CHAIN3:

			{
				temp1  =static_cast<CObject_Item_Equip *>(m_vUserEquip[HEQUIP_RING1]);
				temp2  =static_cast<CObject_Item_Equip *>(m_vUserEquip[HEQUIP_RING2]);
				temp3  =static_cast<CObject_Item_Equip *>(m_vUserEquip[HEQUIP_FEET]);
			}
			break;
		case EM_EQUIP_CARD_GROUP_CHAIN4:
			{

				temp1  =static_cast<CObject_Item_Equip *>(m_vUserEquip[HEQUIP_SHOULDER]);
				temp2  =static_cast<CObject_Item_Equip *>(m_vUserEquip[HEQUIP_BACK]);
				temp3  =static_cast<CObject_Item_Equip *>(m_vUserEquip[HEQUIP_HAND]);
			}
			break;
		case EM_EQUIP_CARD_GROUP_JIANG:

			{
				temp1  =static_cast<CObject_Item_Equip *>(m_vUserEquip[HEQUIP_MAINHAND]);
				temp2  =static_cast<CObject_Item_Equip *>(m_vUserEquip[HEQUIP_ASSIHAND]);	
			}
			break;
	}
    //   2张将组合
	 if (index == EM_EQUIP_CARD_GROUP_JIANG)

	 {
		 if ( !temp1 || !temp2 )
		 {

			 m_SelfSpritActive[index] = 0;  
			 m_SelfSpritActiveAny[index] =0;
			 return FALSE;
		 }
 

		if (Equip_Card_Is_Jiang(temp1->GetEquipCard(),temp2->GetEquipCard()) )
		{


			//任意将
			m_SelfSpritActive[index] = EQUIPPAI_ANYJIANG;  
			m_SelfSpritActiveAny[index] = EQUIPPAI_ANYJIANG;

			//是不是特殊将 
			
				for ( std::map<INT , Euuip_Sprite_Chain>::iterator it = AllSpritActiveChainJiang.begin() ; it != AllSpritActiveChainJiang.end();++it )
				{
					 
						if (it->second.m_pai1 == temp1->GetEquipCard() )
						{
							//记录小激活的id
							m_SelfSpritActive[index] = it->first;  
						}

				}

				return TRUE;
			
		}
		

	 }else	// 3zhang

	 {

				if ( !temp1 || !temp2 || !temp3)
				{
					//表示无效组合
					m_SelfSpritActive[index] = 0;  
					return FALSE;

				}

				SHORT  card1 = temp1->GetEquipCard();
				SHORT  card2 = temp2->GetEquipCard();
				SHORT  card3 = temp3->GetEquipCard();
		 			 
				//是不是相等的
			if(	Equip_Card_Is_Equal(card1,card2,card3))
			{
				m_SelfSpritActive[index] = EQUIPPAI_ANYKE;  //任意刻
				m_SelfSpritActiveAny[index] = EQUIPPAI_ANYKE;

				//是否特殊刻
				for ( std::map<INT , Euuip_Sprite_Chain>::iterator it = AllSpritActiveChainEqual.begin() ; it != AllSpritActiveChainEqual.end();++it )
				{
					
							if (it->second.m_pai1 == card1 )
							{
								//记录小激活的id
								m_SelfSpritActive[index] = it->first;  
								 
							}
							
				}
				
				
				return TRUE;


			}//是不是相连的
			else if ( Equip_Card_Is_Series(card1,card2,card3))
			{
				m_SelfSpritActive[index] = EQUIPPAI_ANYSHUN;   //任意顺
				m_SelfSpritActiveAny[index] =EQUIPPAI_ANYSHUN;

				for ( std::map<INT , Euuip_Sprite_Chain>::iterator it = AllSpritActiveChainSeries.begin() ; it != AllSpritActiveChainSeries.end();++it )
				{

						if (Equip_Card_Min(card1,card2,card3) ==  Equip_Card_Min(it->second.m_pai1,it->second.m_pai2,it->second.m_pai3)  )
						{
							//记录小激活的id
							m_SelfSpritActive[index] = it->first;  
				
						}

				}

				
				return TRUE;
				 
			}else

			{

				// 
				m_SelfSpritActive[index] = 0; 
				m_SelfSpritActiveAny[index] = 0;
				return FALSE;
			}

				 
		 
	 }

	 // 不激活
	 m_SelfSpritActive[index] = 0;  
	 m_SelfSpritActiveAny[index] = 0;
	 return FALSE;
 

}

BOOL			CDataPool::UserEquipSprite_ActiveFour( INT index )
{
	
	if (index < 0 || index >= max_sprite_all_active_num )
	{
		return FALSE;
	}


	if (AllSpritActiveInfo.count(index)  != 0 )   //去匹配每个组合
	{
		std::vector<int> v_Table; // 存储特殊组合
		std::vector<int> v_TableAnyShun; // 存储刻组合
		std::vector<int> v_TableAnyKe;   // 存顺组合
		std::vector<int> v_Equip; 
		INT  temChain = -1;
		for( INT j =  EM_EQUIP_CARD_GROUP_CHAIN1 ; j <= EM_EQUIP_CARD_GROUP_CHAIN4 ; ++j)         //表里的4项筛选出特许组合     

		{   
			switch( j)
			{
			case 0:
				temChain=	 AllSpritActiveInfo[index].m_Chain1 ;
				break;
			case 1:
				temChain=	AllSpritActiveInfo[index].m_Chain2 ;
				break;
			case 2:
				temChain=	AllSpritActiveInfo[index].m_Chain3 ;
				break;
			case 3:
				temChain=	AllSpritActiveInfo[index].m_Chain4 ;
				break;

			}

			if(temChain != -1 )
			{
				if(	temChain == EQUIPPAI_ANYSHUN )
				{
					v_TableAnyShun.push_back(temChain);
				}
				else if(temChain ==EQUIPPAI_ANYKE  )
				{
					v_TableAnyKe.push_back(temChain);

				}else
				{
					v_Table.push_back(temChain);
				}

			}	

		}

		for ( INT k = EM_EQUIP_CARD_GROUP_CHAIN1 ; k<= EM_EQUIP_CARD_GROUP_CHAIN4;++k )         //玩家的组合
		{

			if (m_SelfSpritActive[k] !=0 )
			{
				v_Equip.push_back(m_SelfSpritActive[k]);
			}

		}

		sort(v_Table.begin(),v_Table.end(),greater_equal<INT>());  //排序然后比较
		sort(v_Equip.begin(),v_Equip.end(),greater_equal<INT>());
		sort(v_TableAnyShun.begin(),v_TableAnyShun.end(),greater_equal<INT>());
		sort(v_TableAnyKe.begin(),v_TableAnyKe.end(),greater_equal<INT>());

		INT l  = 0;
		INT m  = 0;
		INT n  = 0;

		if (v_Equip.size() >= v_Table.size())   //比较特殊组合
		{
			for (  l = 0 ; l < (INT)v_Table.size();++l)
			{
				if ( v_Table[l] != v_Equip[l])
				{
					return FALSE;

				}

			}
		}
            for ( INT cur = l ;cur < v_Equip.size();++cur)
            {
				
				v_Equip[cur] = m_SelfSpritActiveAny[cur];

            }

			if ( l == v_Table.size())
			{

				if (v_TableAnyKe.size() > 0)
				{
					for(  m = 0 ;m< (INT)v_TableAnyKe.size() ;++m)
					{

						if ( v_TableAnyKe [m] != v_Equip[ m+v_Table.size()])
						{
							return FALSE;

						}
					}


				}

			}


			if ( m == v_TableAnyKe.size())
			{

				if (v_TableAnyShun.size() > 0)
				{
					for(   n = 0 ;n< (INT)v_TableAnyShun.size() ;++n)
					{

						if ( v_TableAnyShun [n] != v_Equip[m+n+v_Table.size()] )
						{
							return FALSE;

						}
					}


				}

			}


			if ( n == v_TableAnyShun.size())
			{
				m_SelfAllSprite_Active = index;
				return TRUE;
			}

		}




return FALSE;


}


/**牌型是否组合成功,	大激活 */
BOOL		CDataPool::UserEquipSprite_IsActiveAll( )
{

	m_SelfAllSprite_Active = 0;
	for (INT i = EM_EQUIP_CARD_GROUP_CHAIN1 ; i< EM_EQUIP_CARD_GROUP_MAXNUM ;i++)
	{
	   if(!	UserEquipSprite_IsActive(EM_EQUIP_CARD_GROUP (i)))
	   {
		   return FALSE;
	   }
	}


	//到这里 起码是胡牌的，最少是胡牌组合1 即屁湖 
	m_SelfAllSprite_Active  = 1; //1表示屁湖 组合

 
	for ( std::map<INT , Euuip_Sprite_Soul>::iterator it = AllSpritActiveInfo.begin() ;it!=AllSpritActiveInfo.end();++it)
	{

		 
			if ( it->second.m_Chain5 == m_SelfSpritActive[EM_EQUIP_CARD_GROUP_JIANG])  //将组合在表里找到
			{
				//判断其余4个组合

				if( UserEquipSprite_ActiveFour(it->first))
				{
					return  TRUE;
				} 


			}else if(it->second.m_Chain5 == EQUIPPAI_ANYJIANG )
			{

				//判断其余4个组合
				if(	 UserEquipSprite_ActiveFour(it->first))
				{

					return  TRUE;

				}
			}
	   

	}


	return TRUE;


}



//==================================================================================
//别人装备系列
VOID CDataPool::OtherPlayerEquip_Clear(VOID)
{
	for(INT i=0; i<HEQUIP_NUMBER; i++)
	{
		if(m_vOtherPlayerEquip[i])
		{
			CObject_Item::DestroyItem((CObject_Item*)m_vOtherPlayerEquip[i]);
			m_vOtherPlayerEquip[i] = NULL;
		}
	}
}

VOID CDataPool::OtherPlayerEquip_SetItem(HUMAN_EQUIP ptEquip, tObject_Item* pEquipItem, BOOL bClearOld)
{	
	if(m_vOtherPlayerEquip[ptEquip] && bClearOld)
	{
		CObject_Item::DestroyItem((CObject_Item*)m_vOtherPlayerEquip[ptEquip]);
	}

	if(pEquipItem)
	{
		HUMAN_EQUIP pointEquip = ((CObject_Item_Equip *)pEquipItem)->GetItemType();
		if(pointEquip != ptEquip) return;

		// 设置所属
		pEquipItem->SetTypeOwner(tObject_Item::IO_PLAYEROTHER_EQUIP);

		// 设置装备在action item 中的位置.
		pEquipItem->SetPosIndex(ptEquip);
		m_vOtherPlayerEquip[ptEquip] = pEquipItem;

	}
	else
	{
		// 在装备点清空数据
		m_vOtherPlayerEquip[ptEquip] = pEquipItem;
	}

}

tObject_Item* CDataPool::OtherPlayerEquip_GetItem(HUMAN_EQUIP ptEquip)
{
	if(ptEquip < 0 || ptEquip >= HEQUIP_NUMBER) return NULL;

	return m_vOtherPlayerEquip[ptEquip];
}

//套装是否组合成功
INT CDataPool::OtherPlayerEquip_IsUnion(VOID)
{
	/*
	会出现套装的装备
	HEQUIP_MAINHAND		=0,     //主手
	HEQUIP_ASSIHAND		=1,     //副手
	HEQUIP_HEAD			=2,		//头
	HEQUIP_NECK			=3,		//颈
	HEQUIP_SHOULDER		=4,		//肩
	HEQUIP_BACK			=5,		//躯干
	HEQUIP_WRIST		=6,		//腕
	HEQUIP_HAND			=7,		//手
	HEQUIP_WAIST		=8,		//腰
	HEQUIP_FEET			=9,		//脚
	HEQUIP_RING1		=10,	//戒指1
	HEQUIP_RING2		=11,	//戒指2
	HEQUIP_ADORN1		=12,	//饰品1
	HEQUIP_ADORN2		=13,	//饰品2

	人物身上可以存在的套装数量可能不只是一套
	*/



	//玩家的装备信息


	INT nSetID   = -1;
	std::map<INT , INT>	 playerSetInfo   ;//保存套装id对应的装备数量

	for (  INT i = HEQUIP_MAINHAND; i <= HEQUIP_ADORN2 ; i++ )
	{
		if (m_vUserEquip[i])
		{
			if( static_cast<CObject_Item_Equip*>(m_vOtherPlayerEquip[i])->GetDur() > 0 )
			{
				nSetID  	= static_cast<CObject_Item_Equip*>(m_vOtherPlayerEquip[i])->GetSetID();
				if (nSetID > 0)
				{
					playerSetInfo[nSetID]++;
				}
			}

		}		 
	}

    //记录玩家激活的套装id
	memset(m_OtherActiveSetId ,-1 ,sizeof(m_OtherActiveSetId));
	m_OtherActiveSetNum = 0;
	memset(m_OtherActiveSetEquipNUm ,-1 ,sizeof(m_OtherActiveSetEquipNUm));

	for ( std::map<INT ,INT>::iterator it = playerSetInfo.begin() ; it!= playerSetInfo.end(); ++it)
	{
		//if (it->second  >= AllSetInfo [it->first] )
		{
			m_OtherActiveSetId[m_OtherActiveSetNum]  = it->first; // 是套装
			m_OtherActiveSetEquipNUm[m_OtherActiveSetNum++] = it->second;	//套装已经激活的装备个数
		}

	}

	return m_OtherActiveSetNum;

	 
	
}


//====搜侠录道具====================================================================

VOID CDataPool::UserSouXia_Clear()
{
	for ( INT i=0; i<MAX_SOUXIA_CONTAINER; ++i )
	{
		if( m_vUserSouXia[i])
		{
			delete m_vUserSouXia[i];
			m_vUserSouXia[i] = NULL;
		}	
	}
}

VOID CDataPool::UserSouXia_SetInfo( INT nSouXiaIndex, SOUXIA_DATA* pInfo )
{

	if ( m_vUserSouXia[nSouXiaIndex] )
	{
		delete m_vUserSouXia[nSouXiaIndex];
		m_vUserSouXia[nSouXiaIndex] = NULL;
	}

	if( !pInfo )
		return;

	m_vUserSouXia[nSouXiaIndex] = (SOUXIA_DATA*)pInfo;
}

VOID CDataPool::UserSouXia_ClearInfo(INT nSouXiaIndex)
{
	if( ! m_vUserSouXia[nSouXiaIndex] )
		return;
	delete m_vUserSouXia[nSouXiaIndex];
	m_vUserSouXia[nSouXiaIndex] = NULL;
}

SOUXIA_DATA* CDataPool::UserSouXia_GetInfo(INT nSouXiaIndex)
{
	if( !m_vUserSouXia[nSouXiaIndex] )
		return NULL;

	return m_vUserSouXia[nSouXiaIndex];
}
//========================================================================



//========================================================================
VOID CDataPool::UserBag_Clear(VOID)
{
	for(INT i=0; i<MAX_BAG_SIZE; i++)
	{
		if(m_vUserBag[i])
		{
			CObject_Item::DestroyItem((CObject_Item*)m_vUserBag[i]);
			m_vUserBag[i] = NULL;
		}
	}

	m_setAskBagExtra.clear();
}

VOID CDataPool::UserBag_SetItem(INT nBagIndex, tObject_Item* pItem, BOOL bClearOld, BOOL reCalulate)
{

/*	BOOL		BagChange[10];
	for(INT i=0;i<10;i++)
	{
		BagChange[i] = FALSE;
	}
*/
	if(	nBagIndex < 0 || nBagIndex>MAX_BAG_SIZE) return;

	//销毁原有数据
	if(m_vUserBag[nBagIndex] && bClearOld)
	{
		CObject_Item* pItemLog = (CObject_Item*)m_vUserBag[nBagIndex];
		if( pItem && reCalulate )
			pItem->SetNumber( pItem->GetNumber() + pItemLog->GetNumber() );

/*
		for(INT i=0;i<9;i++)
		{
			INT ActionID = CActionSystem::GetMe()->MainMenuBar_Get(i);

			tActionItem* TheAction = CActionSystem::GetMe()->GetAction(ActionID);

			if(!TheAction) continue;

			if(TheAction->GetImpl() == m_vUserBag[nBagIndex])
			{
				BagChange[i] = TRUE;
			}
		}
*/
		//Log Item
		/*tObject_Item::ITEM_GUID temp;
		temp.m_idUnion = 0;
		pItemLog->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
		KLU_Log("PacketItem Delete [NAME=%s] [GUID:idWorld=%d idServer=%d m_uSerial=%d]", 
			pItemLog->GetName(), 
			temp.m_idOrg.m_idWorld,
			temp.m_idOrg.m_idServer, 
			temp.m_idOrg.m_uSerial
			);*/

		CObject_Item::DestroyItem((CObject_Item*)m_vUserBag[nBagIndex]);
	}

	if(pItem)
	{
		pItem->SetTypeOwner(tObject_Item::IO_MYSELF_PACKET);
		pItem->SetPosIndex(nBagIndex);
		//
		//// 马上向要装备的详细信息.
		//// 使客户端使用suppertooltip时能够快速得到详细信息 
		//CGAskItemInfo msg;
		//msg.setID(nBagIndex);
		//CNetManager::GetMe()->SendPacket(&msg);//

		CObject_Item* pItemLog = (CObject_Item*)m_vUserBag[nBagIndex];
		if(pItemLog)
		{
			////Log Item
			//tObject_Item::ITEM_GUID temp;
			//temp.m_idUnion = 0;
			//pItemLog->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
			//KLU_Log("PacketItem Delete [NAME=%s] [GUID:idWorld=%d idServer=%d m_uSerial=%d]", 
			//	pItemLog->GetName(), 
			//	temp.m_idOrg.m_idWorld,
			//	temp.m_idOrg.m_idServer, 
			//	temp.m_idOrg.m_uSerial
			//	);
			
			// 设置物品的保存状态为无详细信息。
			pItemLog->SetItemSaveStatus(CObject_Item::NO_MORE_INFO);
		}

		//Log Item
		/*tObject_Item::ITEM_GUID temp;
		temp.m_idUnion = 0;
		pItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
		KLU_Log("PacketItem Add [NAME=%s] [GUID:idWorld=%d idServer=%d m_uSerial=%d]", 
			pItem->GetName(), 
			temp.m_idOrg.m_idWorld,
			temp.m_idOrg.m_idServer, 
			temp.m_idOrg.m_uSerial
			);*/
	}
	else
	{//原来有物品，移走
		CObject_Item* pItemLog = (CObject_Item*)m_vUserBag[nBagIndex];
		if(pItemLog)
		{
			//Log Item
			/*tObject_Item::ITEM_GUID temp;
			temp.m_idUnion = 0;
			pItemLog->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
			KLU_Log("PacketItem Delete [NAME=%s] [GUID:idWorld=%d idServer=%d m_uSerial=%d]", 
				pItemLog->GetName(), 
				temp.m_idOrg.m_idWorld,
				temp.m_idOrg.m_idServer, 
				temp.m_idOrg.m_uSerial
				);*/
		}
	}

	m_vUserBag[nBagIndex] = pItem;
/*	for(INT i=0;i<10;i++)
	{
		if(BagChange[i] == TRUE)
		{
			tActionItem* TheAction = CActionSystem::GetMe()->GetAction(pItem->GetID());
			CActionSystem::GetMe()->MainMenuBar_Set(i,TheAction->GetID());
		};
	}*/

	

}

/**牌型是否组合成功,	局部的激活 */
BOOL		CDataPool::OtherUserEquipSprite_IsActive( EM_EQUIP_CARD_GROUP index)
{

	// 主手 副手						4
	//帽  项链 护腕                      1
	//戒指1 戒指2  鞋子					 2		
	//肩 衣服 手套						 3
	//饰品 1 饰品2  腰					 0
	//HEQUIP_MAINHAND		=0,     //主手
	//HEQUIP_ASSIHAND		=1,     //副手
	//HEQUIP_HEAD			=2,		//头
	//HEQUIP_NECK			=3,		//颈
	//HEQUIP_SHOULDER		=4,		//肩
	//HEQUIP_BACK			=5,		//躯干
	//HEQUIP_WRIST		=6,		//腕
	//HEQUIP_HAND			=7,		//手
	//HEQUIP_WAIST		=8,		//腰
	//HEQUIP_FEET			=9,		//脚
	//HEQUIP_RING1		=10,	//戒指1
	//HEQUIP_RING2		=11,	//戒指2
	//HEQUIP_ADORN1		=12,	//饰品1
	//HEQUIP_ADORN2		=13,	//饰品2
	//HEQUIP_SUIT			=14,	//外装
	//HEQUIP_RIDER		=15,	//坐骑
	//HEQUIP_NUMBER,				//主角装备点总数

	if ( index < EM_EQUIP_CARD_GROUP_CHAIN1  ||index >= EM_EQUIP_CARD_GROUP_MAXNUM)
	{
		return FALSE ;
	}


	m_OterSpritActive[index] = 0;  

	CObject_Item_Equip *temp1 = NULL ,*temp2 = NULL ,*temp3 = NULL; 

	switch(index)
	{
	case EM_EQUIP_CARD_GROUP_CHAIN1:
		{
			temp1  =static_cast<CObject_Item_Equip *>(m_vOtherPlayerEquip[HEQUIP_ADORN1]);
			temp2  =static_cast<CObject_Item_Equip *>(m_vOtherPlayerEquip[HEQUIP_ADORN2]);
			temp3  =static_cast<CObject_Item_Equip *>(m_vOtherPlayerEquip[HEQUIP_WAIST]);

		}
		break;
	case EM_EQUIP_CARD_GROUP_CHAIN2:
		{
			temp1  =static_cast<CObject_Item_Equip *>(m_vOtherPlayerEquip[HEQUIP_HEAD]);
			temp2  =static_cast<CObject_Item_Equip *>(m_vOtherPlayerEquip[HEQUIP_NECK]);
			temp3  =static_cast<CObject_Item_Equip *>(m_vOtherPlayerEquip[HEQUIP_WRIST]);

		}
		break;
	case EM_EQUIP_CARD_GROUP_CHAIN3:

		{
			temp1  =static_cast<CObject_Item_Equip *>(m_vOtherPlayerEquip[HEQUIP_RING1]);
			temp2  =static_cast<CObject_Item_Equip *>(m_vOtherPlayerEquip[HEQUIP_RING2]);
			temp3  =static_cast<CObject_Item_Equip *>(m_vOtherPlayerEquip[HEQUIP_FEET]);
		}
		break;
	case EM_EQUIP_CARD_GROUP_CHAIN4:
		{

			temp1  =static_cast<CObject_Item_Equip *>(m_vOtherPlayerEquip[HEQUIP_SHOULDER]);
			temp2  =static_cast<CObject_Item_Equip *>(m_vOtherPlayerEquip[HEQUIP_BACK]);
			temp3  =static_cast<CObject_Item_Equip *>(m_vOtherPlayerEquip[HEQUIP_HAND]);
		}
		break;
	case EM_EQUIP_CARD_GROUP_JIANG:

		{
			temp1  =static_cast<CObject_Item_Equip *>(m_vOtherPlayerEquip[HEQUIP_MAINHAND]);
			temp2  =static_cast<CObject_Item_Equip *>(m_vOtherPlayerEquip[HEQUIP_ASSIHAND]);	
		}
		break;
	}
	//   2张将组合
	if (index == EM_EQUIP_CARD_GROUP_JIANG)

	{
		if ( !temp1 || !temp2 )
		{

			m_SelfSpritActive[index] = 0;  
			return FALSE;
		}


		if (Equip_Card_Is_Jiang(temp1->GetEquipCard(),temp2->GetEquipCard()) )
		{

			//任意将
			m_OterSpritActive[index] = EQUIPPAI_ANYJIANG;  
			m_OterSpritActiveAny[index] =EQUIPPAI_ANYJIANG;
			//是不是特殊将 

			for ( std::map<INT , Euuip_Sprite_Chain>::iterator it = AllSpritActiveChainJiang.begin() ; it != AllSpritActiveChainJiang.end();++it )
			{
				
					if (it->second.m_pai1 == temp1->GetEquipCard() )
					{
						//记录小激活的id
						m_OterSpritActive[index] = it->first;  
						 
					}
			
			}

			return TRUE;

		}


	}else	// 3zhang

	{

		if ( !temp1 || !temp2 || !temp3)
		{
			//表示无效组合
			m_OterSpritActive[index] = 0;  
			return FALSE;

		}

		SHORT  card1 = temp1->GetEquipCard();
		SHORT  card2 = temp2->GetEquipCard();
		SHORT  card3 = temp3->GetEquipCard();

		//是不是相等的
		if(	Equip_Card_Is_Equal(card1,card2,card3))
		{

			m_OterSpritActive[index] = EQUIPPAI_ANYKE;  //任意刻
			m_OterSpritActiveAny[index] =EQUIPPAI_ANYKE;

			for ( std::map<INT , Euuip_Sprite_Chain>::iterator it = AllSpritActiveChainEqual.begin() ; it != AllSpritActiveChainEqual.end();++it )
			{
				
					if (it->second.m_pai1 == card1 )
					{
						//记录小激活的id
						m_OterSpritActive[index] = it->first;  
						 
					}
			

			}

			return TRUE;


		}//是不是相连的
		else if ( Equip_Card_Is_Series(card1,card2,card3))
		{

			m_OterSpritActive[index] = EQUIPPAI_ANYSHUN;   //任意顺
			m_OterSpritActiveAny[index] = EQUIPPAI_ANYSHUN;
			for ( std::map<INT , Euuip_Sprite_Chain>::iterator it = AllSpritActiveChainSeries.begin() ; it != AllSpritActiveChainSeries.end();++it )
			{
				if (Equip_Card_Min(card1,card2,card3) ==  Equip_Card_Min(it->second.m_pai1,it->second.m_pai2,it->second.m_pai3)  )
					{
						//记录小激活的id
						m_OterSpritActive[index] = it->first;  
						 
					}
				

			}

			
			return TRUE;

		}else

		{

			// 
			m_OterSpritActive[index] = 0;  
			m_OterSpritActiveAny[index] = 0;
			return FALSE;
		}



	}

	// 不激活
	m_OterSpritActive[index] = 0;  
	m_OterSpritActiveAny[index] = 0;
	return FALSE;


}

BOOL			CDataPool::OtherUserEquipSprite_ActiveFour( INT index )
{

	if (index < 0 || index >= max_sprite_all_active_num )
	{
		return FALSE;
	}


	if (AllSpritActiveInfo.count(index)  != 0 )   //去匹配每个组合
	{
		std::vector<int> v_Table; // 存储特殊组合
		std::vector<int> v_TableAnyShun; // 存储刻组合
		std::vector<int> v_TableAnyKe;   // 存顺组合
		std::vector<int> v_Equip; 
		INT  temChain = -1;
		for( INT j =  EM_EQUIP_CARD_GROUP_CHAIN1 ; j <= EM_EQUIP_CARD_GROUP_CHAIN4 ; ++j)         //表里的4项筛选出特许组合     

		{   
			switch( j)
			{
			case 0:
				temChain=	 AllSpritActiveInfo[j].m_Chain1 ;
				break;
			case 1:
				temChain=	AllSpritActiveInfo[j].m_Chain2 ;
				break;
			case 2:
				temChain=	AllSpritActiveInfo[j].m_Chain3 ;
				break;
			case 3:
				temChain=	AllSpritActiveInfo[j].m_Chain4 ;
				break;

			}

			if(temChain != -1 )
			{
				if(	temChain == EQUIPPAI_ANYSHUN )
				{
					v_TableAnyShun.push_back(temChain);
				}
				else if(temChain ==EQUIPPAI_ANYKE  )
				{
					v_TableAnyKe.push_back(temChain);

				}else
				{
					v_Table.push_back(temChain);
				}

			}	

		}

		for ( INT k = EM_EQUIP_CARD_GROUP_CHAIN1 ; k<= EM_EQUIP_CARD_GROUP_CHAIN4;++k )         //玩家的组合
		{

			if (m_OterSpritActive[k] !=0 )
			{
				v_Equip.push_back(m_OterSpritActive[k]);
			}

		}

		sort(v_Table.begin(),v_Table.end(),greater_equal<INT>());  //排序然后比较
		sort(v_Equip.begin(),v_Equip.end(),greater_equal<INT>());
		sort(v_TableAnyShun.begin(),v_TableAnyShun.end(),greater_equal<INT>());
		sort(v_TableAnyKe.begin(),v_TableAnyKe.end(),greater_equal<INT>());

		INT l  = 0;
		INT m  = 0;
		INT n  = 0;

		if (v_Equip.size() >= v_Table.size())   //比较特殊组合
		{
			for (  l = 0 ; l < (INT)v_Table.size();++l)
			{
				if ( v_Table[l] != v_Equip[l])
				{
					return FALSE;

				}

			}
		}


		for ( INT cur = l ;cur < v_Equip.size();++cur)
		{

			v_Equip[cur] = m_OterSpritActiveAny[cur];

		}
			if ( l == v_Table.size())
			{

				if (v_TableAnyKe.size() > 0)
				{
					for(  m = 0 ;m< (INT)v_TableAnyKe.size() ;++m)
					{

						if ( v_TableAnyKe [m] != v_Equip[ m+v_Table.size()])
						{
							return FALSE;

						}
					}


				}

			}


			if ( m == v_TableAnyKe.size())
			{

				if (v_TableAnyShun.size() > 0)
				{
					for(   n = 0 ;n< (INT)v_TableAnyShun.size() ;++n)
					{

						if ( v_TableAnyShun [n] != v_Equip[m+n+v_Table.size()] )
						{
							return FALSE;

						}
					}


				}

			}


			if ( n == v_TableAnyShun.size())
			{
				m_OtherAllSprite_Active = index;
				return TRUE;
			}

		}



	return FALSE;


}



/**牌型是否组合成功,	大激活 */
BOOL		CDataPool::OtherUserEquipSprite_IsActiveAll( )
{

	m_OtherAllSprite_Active = 0;
	for (INT i = EM_EQUIP_CARD_GROUP_CHAIN1 ; i< EM_EQUIP_CARD_GROUP_MAXNUM ;i++)
	{
		if(!	OtherUserEquipSprite_IsActive(EM_EQUIP_CARD_GROUP(i)))
		{
			return FALSE;
		}
	}


	//到这里 起码是胡牌的，最少是胡牌组合1 即屁湖 
	m_OtherAllSprite_Active  = 1; //1表示屁湖 组合
	for ( std::map<INT , Euuip_Sprite_Soul>::iterator it = AllSpritActiveInfo.begin() ;it!=AllSpritActiveInfo.end();++it)
	{

			if ( it->second.m_Chain5 == m_SelfSpritActive[EM_EQUIP_CARD_GROUP_JIANG])  //将组合在表里找到
			{
				//判断其余4个组合

				if( OtherUserEquipSprite_ActiveFour(it->first))
				{
					return  TRUE;
				} 


			}else if(it->second.m_Chain5 == EQUIPPAI_ANYJIANG )
			{

				//判断其余4个组合
				if(	 OtherUserEquipSprite_ActiveFour(it->first))
				{

					return  TRUE;

				}
			}

	}









	return TRUE;


}







tObject_Item* CDataPool::UserBag_GetItem(INT nBagIndex)
{
	if(	nBagIndex < 0 || nBagIndex>=MAX_BAG_SIZE) return NULL;
	return m_vUserBag[nBagIndex];
}

INT	CDataPool::UserBag_GetItemIndexByGUID(__int64 ItemGUID)
{
	INT index;
	for(index = 0; index<MAX_BAG_SIZE; index++)
	{
		if(m_vUserBag[index] == NULL)
			continue;
		if(m_vUserBag[index]->GetGUID() == ItemGUID)
			break;
	}
	if(index == MAX_BAG_SIZE)
	{
		return -1;
	}
	return index;
}

INT	CDataPool::UserBag_GetItemIndexByID( INT nID )
{
	INT index;
	for(index = 0; index<MAX_BAG_SIZE; index++)
	{
		if(m_vUserBag[index]->GetID() == nID)
			break;
	}
	if(index == MAX_BAG_SIZE)
	{
		return -1;
	}
	return index;
}


VOID CDataPool::UserBag_AskExtraInfo(INT nBagIndex)
{
	if(nBagIndex >= MAX_BAG_SIZE) return;

	//不重复提交
	if(m_setAskBagExtra.find(nBagIndex) != m_setAskBagExtra.end()) return;

	CGAskItemInfo msg;
	msg.setID((BYTE) nBagIndex);
	CNetManager::GetMe()->SendPacket(&msg);

	//记录请求号
	m_setAskBagExtra.insert(nBagIndex);
}

VOID CDataPool::UserBag_SetItemInfo(INT nBagIndex, BOOL bEmpty, const _ITEM* pItem)
{
	if(nBagIndex >= MAX_BAG_SIZE) return;

	//物品消失
	if(bEmpty)
	{
		if(m_vUserBag[nBagIndex])
		{
			//Log Item
			CObject_Item* pItemLog = (CObject_Item*)m_vUserBag[nBagIndex];
			tObject_Item::ITEM_GUID temp;
			temp.m_idUnion = 0;
			pItemLog->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
//			KLU_Log("PacketItem Delete [NAME=%s] [GUID:idWorld=%d idServer=%d m_uSerial=%d]", 
//				pItemLog->GetName(), 
//				temp.m_idOrg.m_idWorld,
//				temp.m_idOrg.m_idServer, 
//				temp.m_idOrg.m_uSerial
//				);

			CObject_Item::DestroyItem((CObject_Item*)m_vUserBag[nBagIndex]);
			m_vUserBag[nBagIndex] = NULL;
		}
	}
	//设置物品详细信息
	else
	{
		if(m_vUserBag[nBagIndex])
		{
		/*	m_vUserBag[nBagIndex]->SetGUID(
				pItem->m_ItemGUID.m_World, 
				pItem->m_ItemGUID.m_Server, 
				pItem->m_ItemGUID.m_Serial);*/

			
			if(m_vUserBag[nBagIndex]->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)
			{
				CObject_Item*  pItem = (CObject_Item*)m_vUserBag[nBagIndex];
				pItem->SetItemSaveStatus(CObject_Item::GET_MORE_INFO);
			}
			m_vUserBag[nBagIndex]->SetExtraInfo(pItem);
		}

		m_setAskBagExtra.erase(nBagIndex);
	}
}

INT	CDataPool::UserBag_CountItemByIDTable(INT ItemGUID)
{
	INT index,count;
	count = 0;
	for(index = 0; index<MAX_BAG_SIZE; index++)
	{
		if(m_vUserBag[index] == NULL)
			continue;
		if(m_vUserBag[index]->GetIdTable() == ItemGUID)
		{
			count += m_vUserBag[index]->GetNumber();
		}
	}
	return count;
}


//=====================================================================================
// 设置物品
VOID CDataPool::UserBagClone_SetItem(INT nBagIndex, CDataPool::USER_BAG_CLONE* pCloneObj)
{
	m_vtUserBagClone[nBagIndex].m_nItemNum	= pCloneObj->m_nItemNum;
	m_vtUserBagClone[nBagIndex].m_pItem		= pCloneObj->m_pItem;
}

//// 获得物品
const CDataPool::USER_BAG_CLONE* CDataPool::UserBagClone_GetItem(INT nBagIndex) const
{
	return &m_vtUserBagClone[nBagIndex];
}

//// 把背包内的物品Clone到这里
VOID CDataPool::UserBagClone_Clone(VOID)
{
	for(INT i=0; i<MAX_BAG_SIZE; i++)
	{
		if(m_vUserBag[i])
		{
			m_vtUserBagClone[i].m_pItem		= m_vUserBag[i];
			m_vtUserBagClone[i].m_nItemNum	= m_vUserBag[i]->GetNumber();
		}
		else
		{
			m_vtUserBagClone[i].m_pItem		= NULL;
			m_vtUserBagClone[i].m_nItemNum	= 0;
		}
	}
}

//// 清空
VOID CDataPool::UserBagClone_Clear(VOID)
{
	for(INT i=0; i<MAX_BAG_SIZE; i++)
	{
		m_vtUserBagClone[i].m_pItem		= NULL;
		m_vtUserBagClone[i].m_nItemNum	= 0;
	}
}



//=====================================================================================
VOID CDataPool::ItemBox_Clear(VOID)
{
	for(INT i=0; i<MAX_BOXITEM_NUMBER; i++)
	{
		if(m_vItemBox[i])
		{
			CObject_Item::DestroyItem((CObject_Item*)m_vItemBox[i]);
			m_vItemBox[i] = NULL;
		}
	}

	m_idItemBox = INVALID_UID;
}

VOID CDataPool::ItemBox_SetItem(INT nBoxIndex, tObject_Item* pItem, BOOL bClearOld)
{
	if(nBoxIndex < 0 || nBoxIndex >= MAX_BOXITEM_NUMBER) return;
	if(m_vItemBox[nBoxIndex] && bClearOld)
	{
		CObject_Item::DestroyItem((CObject_Item*)m_vItemBox[nBoxIndex]);
		m_vItemBox[nBoxIndex] = NULL;
	}

	if(pItem)
	{
		pItem->SetTypeOwner(tObject_Item::IO_ITEMBOX);
		pItem->SetPosIndex(nBoxIndex);
	}

	m_vItemBox[nBoxIndex] = pItem;
}

tObject_Item* CDataPool::ItemBox_GetItem(INT nBoxIndex)
{
	if(nBoxIndex < 0 || nBoxIndex >= MAX_BOXITEM_NUMBER) return NULL;

	return m_vItemBox[nBoxIndex];
}

tObject_Item* CDataPool::ItemBox_GetItem(WORD idWorld, WORD idServer, INT idSerial, INT& nIndex)
{
	for(INT i=0; i<MAX_BOXITEM_NUMBER; i++)
	{
		if(m_vItemBox[i])
		{
			WORD wWorld, wServer;
			GUID_t dwSerial;
			m_vItemBox[i]->GetGUID(wWorld, wServer, dwSerial);

			if(wWorld==idWorld && wServer==idServer && dwSerial==idSerial)
			{
				nIndex = i;
				return m_vItemBox[i];
			}
		}
	}
	return NULL;
}




INT CDataPool::ItemBox_GetNumber(VOID)
{
	INT nNumber = 0;
	for(INT i=0; i<MAX_BOXITEM_NUMBER; i++)
	{
		if(m_vItemBox[i]) nNumber++;
	}	

	return nNumber;
}



/** 清空 */
 VOID	CDataPool::ItemDrop_Clear(VOID)
{
	
	for(INT i=0; i< (INT)m_vItemDrop.size(); i++)
	{
		if(m_vItemDrop[i])
		{
			CObject_Item::DestroyItem((CObject_Item*)m_vItemDrop[i]);
			m_vItemDrop[i] = NULL;
		}
	}

	m_vItemDrop.erase(m_vItemDrop.begin(), m_vItemDrop.end());

}


VOID	CDataPool::ItemDrop_Destroy(INT id)
{

	if( id < 0  ) return;

	std::map< INT,tObject_Item*>::iterator  it = m_vItemDrop.find(id);

	if( it != m_vItemDrop.end())
	  if (it->second)
	  {
			CObject_Item::DestroyItem((CObject_Item*)m_vItemDrop[id]);
			m_vItemDrop.erase(it);

	  }
}
 
/** 设置物品 */
 VOID	CDataPool::ItemDrop_SetItem(INT nDropObjId, tObject_Item* pItem, BOOL bClearOld)

{
	 
	if(nDropObjId < 0  ) return;
	if( !pItem) return ;


	std::map< INT,tObject_Item*>::iterator  it = m_vItemDrop.find(nDropObjId);

	if( it != m_vItemDrop.end())
	{
		Assert("the same  drop  id");
	}

	
	pItem->SetTypeOwner(tObject_Item::IO_ITEMDROP);

	m_vItemDrop[nDropObjId] = pItem;


}
/** 取得其中物品 */
 tObject_Item*	CDataPool::ItemDrop_GetItem(INT nDropObjId)
{
   
	if(nDropObjId < 0  ) return NULL;

	std::map< INT,tObject_Item*>::iterator it = m_vItemDrop.find(nDropObjId);
	 if (it != m_vItemDrop.end())
	 {
		 return it->second;
	 }

	 return NULL;

	//return m_vItemDrop[nDropObjId];
}


 INT	CDataPool::ItemDrop_GetNumber(VOID) 

 {
	 INT nNumber = 0;
	 for(INT i=0; i<(INT)m_vItemDrop.size(); i++)
	 {
		 if(m_vItemDrop[i]) nNumber++;
	 }	

	 return nNumber;
	 
 }


VOID	CDataPool::ItemDrop_Tick(VOID)
{
	std::map< INT,tObject_Item*>::iterator it = m_vItemDrop.begin();
	for ( ;it != m_vItemDrop.end(); ++it )
	{
		if (it->second)
		{
			CObjectManager* pObjectManager = CObjectManager::GetMe();

			CTripperObject_ItemDrop* pObj = (CTripperObject_ItemDrop*)(pObjectManager->FindServerObject(it->first));
			if ( !pObj )
			{
				continue ;
			}



		 	pObj->Tick_UpdateInfoBoard();
			pObj->Tick();




		if( !pObj ->GetDropEveryOneCanPick())
		{
			pObj->SetPassTime( timeGetTime() - pObj->GetStartTime());
			if ( pObj->GetPassTime() > pObj->GetProtectTime())
			{
					 pObj ->SetDropEveryOneCanPick(true);
					 pObj->Tripper_AddEffect();
			}
		}

		}
	}

}

//=======================================================================================
VOID CDataPool::Booth_Clear(VOID)
{
	for(INT i=0; i<MAX_BOOTH_NUMBER; i++)
	{
		if(m_listBooth[i])
		{
			CObject_Item::DestroyItem((CObject_Item*)m_listBooth[i]);
			m_listBooth[i]	=	NULL;
			m_nlistPrice[i]	=	0;
		}
	}

	m_nBoothNumber = 0;
}

VOID CDataPool::Booth_Sold_Clear(VOID)
{
	for(INT i=0; i<MAX_BOOTH_SOLD_NUMBER; i++)
	{
		if(m_listBoothSold[i])
		{
			CObject_Item::DestroyItem((CObject_Item*)m_listBoothSold[i]);
			m_listBoothSold[i] = NULL;
		}
	}

	m_nBoothSoldNumber = 0;
}

VOID CDataPool::Booth_SetItem(INT nBoothIndex, tObject_Item* pItem)
{
	if(	nBoothIndex < 0 || nBoothIndex >= MAX_BOOTH_NUMBER ) return;

	//销毁原有数据
	if(m_listBooth[nBoothIndex])
	{
		CObject_Item::DestroyItem((CObject_Item*)m_listBooth[nBoothIndex]);
		m_listBooth[nBoothIndex] = NULL;
	}

	if(pItem)
	{
		pItem->SetTypeOwner(tObject_Item::IO_BOOTH);
		pItem->SetPosIndex(nBoothIndex);
	}
 
	m_listBooth[nBoothIndex] = pItem;
}

VOID CDataPool::Booth_SetItemPrice(INT nBoothIndex, UINT nPrice)
{
	
	if(	nBoothIndex < 0 || nBoothIndex >=  MAX_BOOTH_NUMBER ) return;
	
	m_nlistPrice[nBoothIndex]	=	nPrice;
}
//add by gh for IB Shop 2010/07/08
//=======================================================================================
VOID CDataPool::IBShop_Clear(VOID)
{
	for(INT i=0; i<MAX_IBSHOP_NUMBER; i++)
	{
		if(m_listIBShop[i])
		{
			CObject_Item::DestroyItem((CObject_Item*)m_listIBShop[i]);
			m_listIBShop[i]	=	NULL;
			m_nlistIBPrice[i]	=	0;
		}
	}

	m_nIBShopNumber = 0;
}

VOID CDataPool::IBShop_SetItem(INT nIBShopIndex, tObject_Item* pItem)
{
	if(	nIBShopIndex < 0 || nIBShopIndex >= MAX_IBSHOP_NUMBER ) return;

	//销毁原有数据
	if(m_listIBShop[nIBShopIndex])
	{
		CObject_Item::DestroyItem((CObject_Item*)m_listIBShop[nIBShopIndex]);
		m_listIBShop[nIBShopIndex] = NULL;
	}

	if(pItem)
	{
		pItem->SetTypeOwner(tObject_Item::IO_ITEMIBSHOP);
		pItem->SetPosIndex(nIBShopIndex);
	}

	m_listIBShop[nIBShopIndex] = pItem;
}

tObject_Item* CDataPool::IBShop_GetItem(INT nIBShopIndex)
{
	if(	nIBShopIndex < 0 || nIBShopIndex>=MAX_IBSHOP_NUMBER) return NULL;

	return m_listIBShop[nIBShopIndex];
}

UINT CDataPool::IBShop_GetItemPrice(INT nIBShopIndex)
{

	if(	nIBShopIndex < 0 || nIBShopIndex >= MAX_IBSHOP_NUMBER ) return 0;

	return m_nlistIBPrice[nIBShopIndex];
}

tObject_Item* CDataPool::IBShop_GetItemByID(INT IDtable)
{
	for(INT i=0; i<m_nIBShopNumber; i++)
	{
		if(m_listIBShop[i]->GetIdTable() == IDtable)
			return m_listIBShop[i];
	}
	return	NULL;
}

VOID CDataPool::IBShop_SetItemPrice(INT nIBShopIndex, UINT nPrice)
{

	if(	nIBShopIndex < 0 || nIBShopIndex >=  MAX_IBSHOP_NUMBER ) return;

	m_nlistIBPrice[nIBShopIndex]	=	nPrice;
}

VOID CDataPool::IBShop_SetItemIsNewSell(INT nIBShopIndex, INT isNew)
{

	if(	nIBShopIndex < 0 || nIBShopIndex >=  MAX_IBSHOP_NUMBER ) return;

	//m_nIBlistPrice[nIBShopIndex]	=	isNew;
}

// end of add

VOID CDataPool::Booth_SetSoldItem(INT nBoothIndex, tObject_Item* pItem)
{
	if(	nBoothIndex < 0 || nBoothIndex >= MAX_BOOTH_SOLD_NUMBER ) return;
	//销毁原有数据
	if(m_listBoothSold[nBoothIndex])
	{
		CObject_Item::DestroyItem((CObject_Item*)m_listBoothSold[nBoothIndex]);
		m_listBoothSold[nBoothIndex] = NULL;
	}

	if(pItem)
	{
		pItem->SetTypeOwner(tObject_Item::IO_BOOTH);
		pItem->SetPosIndex(nBoothIndex+200);//用来与货架中的普通商品区分
	}
	m_listBoothSold[nBoothIndex] = pItem;
}

tObject_Item* CDataPool::Booth_GetItem(INT nBoothIndex)
{
	if(	nBoothIndex < 0 || nBoothIndex>=MAX_BOOTH_NUMBER) return NULL;

	return m_listBooth[nBoothIndex];
}

UINT CDataPool::Booth_GetItemPrice(INT nBoothIndex)
{

	if(	nBoothIndex < 0 || nBoothIndex >= MAX_BOOTH_NUMBER ) return 0;
	
	return m_nlistPrice[nBoothIndex];
}

tObject_Item* CDataPool::Booth_GetSoldItem(INT nBoothIndex)
{
	if(	nBoothIndex < 0 || nBoothIndex>=MAX_BOOTH_SOLD_NUMBER) return NULL;

	return m_listBoothSold[nBoothIndex];
}


tObject_Item* CDataPool::Booth_GetItemByID(INT IDtable)
{
	for(INT i=0; i<m_nBoothNumber; i++)
	{
		if(m_listBooth[i]->GetIdTable() == IDtable)
			return m_listBooth[i];
	}
	return	NULL;
}

BOOL CDataPool::Booth_IsCanRepair(tObject_Item* pItem)
{
	if(!pItem)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CDataPool::Booth_IsCanBuy(tObject_Item* pItem)
{
	if(!pItem)
	{
		return FALSE;
	}
	return TRUE;
}

//================================================================================
//银行数据的访问
VOID CDataPool::UserBank_Clear(VOID)
{
	for(INT i=0; i<MAX_BANK_SIZE; i++)
	{
		if(m_listUserBank[i])
		{
			CObject_Item::DestroyItem((CObject_Item*)m_listUserBank[i]);
			m_listUserBank[i] = NULL;
		}
	}
}

VOID CDataPool::UserBank_SetItem(INT nBankIndex, tObject_Item* pItem, BOOL bClearOld)
{
	if(	nBankIndex < 0 || nBankIndex>=MAX_BANK_SIZE) return;
	//销毁原有数据
	if(m_listUserBank[nBankIndex] && bClearOld)
	{
		CObject_Item::DestroyItem((CObject_Item*)m_listUserBank[nBankIndex]);
		m_listUserBank[nBankIndex] = NULL;
	}

	if(pItem)
	{
		pItem->SetTypeOwner(tObject_Item::IO_MYSELF_BANK);
		pItem->SetPosIndex(nBankIndex);
	}

	m_listUserBank[nBankIndex] = pItem;
}

tObject_Item* CDataPool::UserBank_GetItem(INT nBankIndex)
{
	if(	nBankIndex < 0 || nBankIndex>=MAX_BANK_SIZE) return NULL;
	return m_listUserBank[nBankIndex];
}

VOID CDataPool::UserBank_SetItemExtraInfo(INT nBankIndex, BOOL bEmpty, const _ITEM* pItem)
{
	if(nBankIndex >= MAX_BANK_SIZE) return;
	//	if(m_setAskBagExtra.find(nBagIndex) == m_setAskBagExtra.end()) return;

	if(bEmpty)
	{
		if(m_listUserBank[nBankIndex])
		{
			CObject_Item::DestroyItem((CObject_Item*)m_listUserBank[nBankIndex]);
			m_listUserBank[nBankIndex] = NULL;
		}
	}
	else
	{
		if(m_listUserBank[nBankIndex])
			m_listUserBank[nBankIndex]->SetExtraInfo(pItem);
	}
}

BOOL CDataPool::UserBank_IsEmpty(INT nIndex)
{
	INT nBeginIndex, nEndIndex;
	if(nIndex == 1)
	{
		nBeginIndex = RENTBOX1_START_INDEX;
		nEndIndex = RENTBOX2_START_INDEX;
	}
	else if(nIndex == 2)
	{
		nBeginIndex = RENTBOX2_START_INDEX;
		nEndIndex = RENTBOX3_START_INDEX;
	}
	else if(nIndex == 3)
	{
		nBeginIndex = RENTBOX3_START_INDEX;
		nEndIndex = RENTBOX4_START_INDEX;
	}
	else if(nIndex == 4)
	{
		nBeginIndex = RENTBOX4_START_INDEX;
		nEndIndex = RENTBOX5_START_INDEX;
	}
	else if(nIndex == 5)
	{
		nBeginIndex = RENTBOX5_START_INDEX;
		nEndIndex = MAX_BANK_SIZE;
	}

	for(INT i=nBeginIndex; i<nEndIndex; i++)
	{
		if(m_listUserBank[i] == NULL)
		{
			return TRUE;
		}
	}
	return FALSE;
}

//=======================================================================================
// 交易盒数据访问方法
VOID CDataPool::CLIENT_EXCHANGE_BOX::CleanUp()
{
	m_CanConform			=	FALSE;
	m_IsLocked				=	FALSE;
	m_Money					=	0;

	m_ItemList.resize(EXCHANGE_BOX_SIZE, 0);
	m_Petlist.resize(EXCHANGE_PET_BOX_SIZE, 0);
}

VOID CDataPool::MyExBox_Clear(VOID)
{
	for(INT i=0; i<EXCHANGE_BOX_SIZE; i++)
	{
		if(m_MyExchangeBox.m_ItemList[i] )
		{
			CObject_Item::DestroyItem((CObject_Item*)m_MyExchangeBox.m_ItemList[i]);
			m_MyExchangeBox.m_ItemList[i] = NULL;

		}
	}

	for(INT i=0; i<EXCHANGE_PET_BOX_SIZE; i++)
	{//自己的骑乘宠物盒里放的一定是背包里的
		if(m_MyExchangeBox.m_Petlist[i] )
		{
			m_MyExchangeBox.m_Petlist[i] = NULL;
		}
	}

	m_MyExchangeBox.CleanUp();
}

VOID CDataPool::MyExBox_SetItem(INT nExBoxIndex, tObject_Item* pItem, BOOL bClearOld)
{
	if(	nExBoxIndex < 0 || nExBoxIndex>=EXCHANGE_BOX_SIZE) return;
	//销毁原有数据
	if(m_MyExchangeBox.m_ItemList[nExBoxIndex] && bClearOld )
	{
		CObject_Item::DestroyItem((CObject_Item*)m_MyExchangeBox.m_ItemList[nExBoxIndex] );
		m_MyExchangeBox.m_ItemList[nExBoxIndex] = NULL;
	}

	if(pItem)
	{
		pItem->SetTypeOwner(tObject_Item::IO_MYEXBOX);
		pItem->SetPosIndex(nExBoxIndex);
	}

	m_MyExchangeBox.m_ItemList[nExBoxIndex] = pItem;
}

tObject_Item*	CDataPool::MyExBox_GetItem(INT nExBoxIndex)
{
	if(	nExBoxIndex < 0 || nExBoxIndex>=EXCHANGE_BOX_SIZE) return NULL;
	return m_MyExchangeBox.m_ItemList[nExBoxIndex];
}

VOID CDataPool::MyExBox_SetItemExtraInfo(INT nExBoxIndex, BOOL bEmpty, const _ITEM* pItem)
{
	if(nExBoxIndex >= EXCHANGE_BOX_SIZE) return;
	//	if(m_setAskBagExtra.find(nBagIndex) == m_setAskBagExtra.end()) return;

	if(bEmpty)
	{
		if(m_MyExchangeBox.m_ItemList[nExBoxIndex])
		{
			CObject_Item::DestroyItem((CObject_Item*)m_MyExchangeBox.m_ItemList[nExBoxIndex]);
			m_MyExchangeBox.m_ItemList[nExBoxIndex]= NULL;
		}
	}
	else
	{
		if(m_MyExchangeBox.m_ItemList[nExBoxIndex])
			m_MyExchangeBox.m_ItemList[nExBoxIndex]->SetExtraInfo(pItem);
	}
}

BOOL CDataPool::MyExBox_IsEmpty(VOID)
{
	for(int i=0; i<EXCHANGE_BOX_SIZE; i++)
	{
		if(m_MyExchangeBox.m_ItemList[i] == NULL)
		{
			return TRUE;
		}
	}
	return FALSE;	
}


VOID CDataPool::OtExBox_Clear(BYTE bFlagClearPet)
{
	for(INT i=0; i<EXCHANGE_BOX_SIZE; i++)
	{
		if(m_OtherExchangeBox.m_ItemList[i])
		{
			CObject_Item::DestroyItem((CObject_Item*)m_OtherExchangeBox.m_ItemList[i] );
			m_OtherExchangeBox.m_ItemList[i] = NULL;

		}
	}

	if(bFlagClearPet)
	{
		for(INT i=0; i<EXCHANGE_PET_BOX_SIZE; i++)
		{
			if(m_OtherExchangeBox.m_Petlist[i] )
			{
				SAFE_DELETE(m_OtherExchangeBox.m_Petlist[i] );
				
			}
		}
	}
	m_OtherExchangeBox.CleanUp();
}

VOID CDataPool::OtExBox_SetItem(INT nExBoxIndex, tObject_Item* pItem, BOOL bClearOld)
{
	if(	nExBoxIndex < 0 || nExBoxIndex>=EXCHANGE_BOX_SIZE) return;
	//销毁原有数据
	if(m_OtherExchangeBox.m_ItemList[nExBoxIndex] && bClearOld)
	{
		CObject_Item::DestroyItem((CObject_Item*)m_OtherExchangeBox.m_ItemList[nExBoxIndex]);
		m_OtherExchangeBox.m_ItemList[nExBoxIndex] = NULL;
	}

	if(pItem)
	{
		pItem->SetTypeOwner(tObject_Item::IO_OTHEREXBOX);
		pItem->SetPosIndex(nExBoxIndex);
	}

	m_OtherExchangeBox.m_ItemList[nExBoxIndex] = pItem;
}

tObject_Item*	CDataPool::OtExBox_GetItem(INT nExBoxIndex)
{
	if(	nExBoxIndex < 0 || nExBoxIndex>=EXCHANGE_BOX_SIZE) return NULL;
	return m_OtherExchangeBox.m_ItemList[nExBoxIndex];
}

VOID CDataPool::OtExBox_SetItemExtraInfo(INT nExBoxIndex, BOOL bEmpty, const _ITEM* pItem)
{
	if(nExBoxIndex >= EXCHANGE_BOX_SIZE) return;
	//	if(m_setAskBagExtra.find(nBagIndex) == m_setAskBagExtra.end()) return;

	if(bEmpty)
	{
		if(m_OtherExchangeBox.m_ItemList[nExBoxIndex])
		{
			CObject_Item::DestroyItem((CObject_Item*)m_OtherExchangeBox.m_ItemList[nExBoxIndex]);
			m_OtherExchangeBox.m_ItemList[nExBoxIndex] = NULL;
		}
	}
	else
	{
		if(m_OtherExchangeBox.m_ItemList[nExBoxIndex])
			m_OtherExchangeBox.m_ItemList[nExBoxIndex]->SetExtraInfo(pItem);
	}
}

VOID CDataPool::AddToAppList(INT appid)
{
	//塞到m_pIn所指向的当前格子内
	m_listApplicantsID[m_pIn] = appid;
	if( m_pIn+1 == MAX_EXCHANGE_APPLICANTS_NUM ) // 加入指针已到达队尾
	{
		if(m_pOut == 0) // 如果队首还没取出
		{
			//加入指针停止不前，后面加入的替换掉当前队尾的
		}
		else
		{
			m_pIn = 0;
		}
	}
	else
	{
		if(m_pIn+1 == m_pOut)
		{
			//停止不前
		}
		else
		{
			m_pIn++;
		}
	}
}
INT  CDataPool::GetFromAppList()
{
	//取m_pOut当前所指向的格子的数据
	if(m_pOut == m_pIn)//空了
		return -1;
	if( m_pOut+1 >= MAX_EXCHANGE_APPLICANTS_NUM )
	{
		m_pOut = 0;
		return m_listApplicantsID[MAX_EXCHANGE_APPLICANTS_NUM-1];
	}
	else
	{
		return m_listApplicantsID[m_pOut++];
	}
}

BOOL CDataPool::IsStillAnyAppInList()
{
	return (m_pIn == m_pOut)? FALSE:TRUE;
}


/*
///////宝石合成/镶嵌界面/////////////////////////////////
VOID CDataPool::EnchasePage_Clear(VOID)
{
	for(INT i=0; i<2; i++)
	{
		if(m_vPageEnchase[i])
		{
			CObject_Item::DestroyItem((CObject_Item*)m_vPageEnchase[i]);
			m_vPageEnchase[i] = NULL;
		}
	}
}

VOID CDataPool::EnchasePage_SetItem(INT nEnchaseIndex, tObject_Item* pItem, BOOL bClearOld)
{
	if(	nEnchaseIndex < 0 || nEnchaseIndex>=2) return;
	//	销毁原有数据
	//	原m_vPageEnchase[nEnchaseIndex]代表的物品栏中的ITEM设置为彩色...
	
	if(m_vPageEnchase[nEnchaseIndex] && bClearOld)
	{
		CObject_Item::DestroyItem((CObject_Item*)m_vPageEnchase[nEnchaseIndex]);
		m_vPageEnchase[nEnchaseIndex] = NULL;
	}

	if(pItem)
	{
		pItem->SetTypeOwner(tObject_Item::IO_ENCHASEPAGE);
		pItem->SetPosIndex(nEnchaseIndex);
	}

	m_vPageEnchase[nEnchaseIndex] = pItem;
}

tObject_Item*	CDataPool::EnchasePage_GetItem(INT nEnchaseIndex)
{
	if(	nEnchaseIndex < 0 || nEnchaseIndex>=2) return NULL;
	return m_vPageEnchase[nEnchaseIndex];
}
*/
//=======================================================================================
// 任务递交界面
VOID	CDataPool::MissionBox_Clear(VOID)
{
	for(INT i=0; i<MISSION_BOX_SIZE; i++)
	{
		if(m_MissionItemList[i])
		{
			CObject_Item::DestroyItem((CObject_Item*)m_MissionItemList[i]);
			m_MissionItemList[i] = NULL;
		}
	}
}
VOID	CDataPool::MissionBox_SetItem(INT nMissionBoxIndex, tObject_Item* pItem, BOOL bClearOld)
{
	if(	nMissionBoxIndex < 0 || nMissionBoxIndex>=MISSION_BOX_SIZE) return;

	if(m_MissionItemList[nMissionBoxIndex] && bClearOld)
	{
		CObject_Item::DestroyItem((CObject_Item*)m_MissionItemList[nMissionBoxIndex]);
		m_MissionItemList[nMissionBoxIndex] = NULL;
	}
	if(pItem)
	{
		pItem->SetTypeOwner(tObject_Item::IO_MISSIONBOX);
		pItem->SetPosIndex(nMissionBoxIndex);
	}
	m_MissionItemList[nMissionBoxIndex] = pItem;
}
tObject_Item*	CDataPool::MissionBox_GetItem(INT nMissionBoxIndex)
{
	if(	nMissionBoxIndex < 0 || nMissionBoxIndex>=EXCHANGE_BOX_SIZE) return NULL;
	return m_MissionItemList[nMissionBoxIndex];
}

//=======================================================================================
// 任务列表物品界面
/*
VOID CDataPool::QUESTLOG_BOX::CleanUp()
{
//	m_ItemList.resize(QUESTLOG_BOX_SIZE, 0);
	for(INT i=0; i< MAX_CHAR_MISSION_NUM; i++)
	{
		if(m_vQuestLogItem[i].CleanUp())
		{
			CObject_Item::DestroyItem((CObject_Item*)m_vQuestLogItem[i]);
			m_vQuestLogItem[i] = NULL;
		}
	}
}
*/
VOID	CDataPool::QuestLogBox_Clear(VOID)
{
	for(INT i=0; i< MAX_CHAR_MISSION_NUM; i++)
	{
		for(INT j=0; j< QUESTLOG_BOX_SIZE; j++)
		{
			if(m_vQuestLogItem[i].m_ItemList[j])
			{
				CObject_Item::DestroyItem((CObject_Item*)m_vQuestLogItem[i].m_ItemList[j]);
				m_vQuestLogItem[i].m_ItemList[j] = NULL;
			}
		}
	}
}
VOID	CDataPool::QuestLogBox_SetItem(INT nQuestIndex, INT nItemIndex, tObject_Item* pItem, BOOL bClearOld)
{
	if(	nQuestIndex < 0 || nQuestIndex>=MAX_CHAR_MISSION_NUM) return;

	tObject_Item* theItem = m_vQuestLogItem[nQuestIndex].m_ItemList[nItemIndex];

	if(theItem && bClearOld)
	{
		CObject_Item::DestroyItem((CObject_Item*)m_vQuestLogItem[nQuestIndex].m_ItemList[nItemIndex]);
		m_vQuestLogItem[nQuestIndex].m_ItemList[nItemIndex] = NULL;
	}

	if(pItem)
	{
		pItem->SetTypeOwner(tObject_Item::IO_QUESTVIRTUALITEM);
		pItem->SetPosIndex(nQuestIndex);
	}
	m_vQuestLogItem[nQuestIndex].m_ItemList[nItemIndex] = pItem;
}

tObject_Item*	CDataPool::QuestLogBox_GetItem(INT nQuestIndex, INT nItemIndex)
{
	if(	nQuestIndex < 0 || nQuestIndex>=MAX_CHAR_MISSION_NUM) return NULL;
	if(	nItemIndex  < 0 || nItemIndex >=QUESTLOG_BOX_SIZE) return NULL;
	return m_vQuestLogItem[nQuestIndex].m_ItemList[nItemIndex];
}

//=======================================================================================
// 留言板数据访问
VOID CDataPool::BBS_t::CleanUp()
{
	m_nMessageNum = 0;
	m_szTitle	=	"";
	m_nSerial	=	0;
	MessageEntry_t	BBSinital;
	m_MessageList.resize(MAX_BBS_MESSAGE_NUM, BBSinital);						//留言板
}

//=======================================================================================
// 摊位盒数据访问
VOID CDataPool::CLIENT_STALL_BOX::CleanUp()
{
	m_nDefaultPage					=	ITEM_PAGE;
	m_ObjID							=	0;					//摊位拥有者的ID
	m_bIsOpen						=	FALSE;				//自己摊位是否已经打开
//	m_PosTax						=	0;					//摊位费
//	m_TradeTax						=	0;					//每次交易税收
	m_szStallName					=	"";					//标示摊位名
	m_szOwnerName					=	"";					//标示摊主名
	m_nFirstPage					=	0;					//起始页
	m_OtGUID						=	0;
	m_BBS.CleanUp();										//摊位留言板		
	STALL_ITEM	stallinital;
	m_ItemList.resize(STALL_BOX_SIZE, stallinital);			//标示摊位中的所有物品

	STALL_PET	stallPetinital;
	m_PetList.resize(STALL_PET_BOX_SIZE, stallPetinital);	//标示摊位中的所有骑乘宠物

}
VOID CDataPool::MyStallBox_Clear(VOID)
{
	for(INT i=0; i<STALL_BOX_SIZE; i++)
	{
		if(m_MyStallBox.m_ItemList[i].pItem)
		{
			CObject_Item::DestroyItem((CObject_Item*)m_MyStallBox.m_ItemList[i].pItem);
			m_MyStallBox.m_ItemList[i].pItem	= NULL;
			m_MyStallBox.m_ItemList[i].nPrice	= 0;
			m_MyStallBox.m_ItemList[i].nSerial	= 0;
		}
	}

	for(INT i=0; i<STALL_PET_BOX_SIZE; i++)
	{//自己的骑乘宠物指针指得是一个背包中的宠物
		if(m_MyStallBox.m_PetList[i].pPet)
		{
			m_MyStallBox.m_PetList[i].pPet = NULL;
		}
	}

	m_MyStallBox.CleanUp();
}
VOID CDataPool::MyStallBox_SetItem(INT nStallBoxIndex, tObject_Item* pItem, BOOL bClearOld)
{
	if(	nStallBoxIndex < 0 || nStallBoxIndex>=STALL_BOX_SIZE) return;
	//销毁原有数据
	if( m_MyStallBox.m_ItemList[nStallBoxIndex].pItem && bClearOld)
	{
		CObject_Item::DestroyItem((CObject_Item*)m_MyStallBox.m_ItemList[nStallBoxIndex].pItem);
		m_MyStallBox.m_ItemList[nStallBoxIndex].pItem	= NULL;
		m_MyStallBox.m_ItemList[nStallBoxIndex].nPrice	= 0;
		m_MyStallBox.m_ItemList[nStallBoxIndex].nSerial	= 0;
	}

	if(pItem)
	{
		pItem->SetTypeOwner(tObject_Item::IO_MYSTALLBOX);
		pItem->SetPosIndex(nStallBoxIndex);
	}

	m_MyStallBox.m_ItemList[nStallBoxIndex].pItem = pItem;
}
tObject_Item*	CDataPool::MyStallBox_GetItem(INT nStallBoxIndex)
{
	if(	nStallBoxIndex < 0 || nStallBoxIndex>=STALL_BOX_SIZE) return NULL;
	return m_MyStallBox.m_ItemList[nStallBoxIndex].pItem;
}
VOID CDataPool::MyStallBox_SetItemExtraInfo(INT nStallBoxIndex, BOOL bEmpty, const _ITEM* pItem)
{
	if(nStallBoxIndex >= STALL_BOX_SIZE) return;

	if(bEmpty)
	{
		if(m_MyStallBox.m_ItemList[nStallBoxIndex].pItem)
		{
			CObject_Item::DestroyItem((CObject_Item*)m_MyStallBox.m_ItemList[nStallBoxIndex].pItem);
			m_MyStallBox.m_ItemList[nStallBoxIndex].pItem = NULL;
			m_MyStallBox.m_ItemList[nStallBoxIndex].nPrice	= 0;
			m_MyStallBox.m_ItemList[nStallBoxIndex].nSerial	= 0;
		}
	}
	else
	{
		if(m_MyStallBox.m_ItemList[nStallBoxIndex].pItem)
			m_MyStallBox.m_ItemList[nStallBoxIndex].pItem->SetExtraInfo(pItem);
	}
}
VOID CDataPool::MyStallBox_SetItemPrice(INT nStallBoxIndex, UINT nPrice)
{
	if(	nStallBoxIndex < 0 || nStallBoxIndex>=STALL_BOX_SIZE) return;
	m_MyStallBox.m_ItemList[nStallBoxIndex].nPrice = nPrice;
}
VOID CDataPool::MyStallBox_SetItemSerial(INT nStallBoxIndex, UINT nSerial)
{
	if(	nStallBoxIndex < 0 || nStallBoxIndex>=STALL_BOX_SIZE) return;
	m_MyStallBox.m_ItemList[nStallBoxIndex].nSerial = nSerial;
}
UINT	CDataPool::MyStallBox_GetItemPrice(INT nStallBoxIndex)
{
	if(	nStallBoxIndex < 0 || nStallBoxIndex>=STALL_BOX_SIZE) return 0;
	return m_MyStallBox.m_ItemList[nStallBoxIndex].nPrice;
}
UINT	CDataPool::MyStallBox_GetItemSerial(INT nStallBoxIndex)
{
	if(	nStallBoxIndex < 0 || nStallBoxIndex>=STALL_BOX_SIZE) return 0;
	return m_MyStallBox.m_ItemList[nStallBoxIndex].nSerial;
}

INT	CDataPool::MyStallBox_GetItemIndexByGUID(__int64 ItemGUID)
{
	INT index;
	for(index = 0; index<STALL_BOX_SIZE; index++)
	{
		if(m_MyStallBox.m_ItemList[index].pItem == NULL)
			continue;
		if(m_MyStallBox.m_ItemList[index].pItem->GetGUID() == ItemGUID)
			break;
	}
	if(index == STALL_BOX_SIZE)
	{
		return -1;
	}
	return index;
}

INT	CDataPool::MyStallBox_GetPetByGuid(PET_GUID_t PetGuid)
{
	/*for(UINT i = 0; i< STALL_PET_BOX_SIZE; i++)
	{
		if(m_MyStallBox.m_PetList[i].pPet == NULL)
		{
			continue;
		}
		if(m_MyStallBox.m_PetList[i].pPet->m_GUID == PetGuid)
		{
			return i;
		}
	}*/
	return	-1;
}

VOID CDataPool::MyStallBox_SetPet(INT nIndex, SDATA_PET* pPetData, BOOL bClearOld)
{
	/*if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE)
	{
		if(m_MyStallBox.m_PetList[nIndex].pPet != NULL && bClearOld)
		{
			SAFE_DELETE(m_MyStallBox.m_PetList[nIndex].pPet);
		}
		m_MyStallBox.m_PetList[nIndex].pPet = pPetData; 
	}
	else
		return;*/
}

VOID CDataPool::MyStallBox_SetPetPrice(INT nIndex, INT nPetPrice)
{
	/*if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE)
	{
		m_MyStallBox.m_PetList[nIndex].nPrice = nPetPrice;
	}*/
}

VOID CDataPool::MyStallBox_SetPetSerial(INT nIndex, INT nSerial)
{
	/*if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE)
	{
		m_MyStallBox.m_PetList[nIndex].nSerial = nSerial;
	}*/
}

INT	CDataPool::MyStallBox_GetPetPrice(INT nIndex)
{
	/*if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE)
	{
		return m_MyStallBox.m_PetList[nIndex].nPrice;
	}*/
	return 0;
}

INT	CDataPool::MyStallBox_GetPetSerial(INT nIndex)
{
	/*if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE)
	{
		return m_MyStallBox.m_PetList[nIndex].nSerial;
	}*/
	return 0;
}

SDATA_PET*	CDataPool::MyStallBox_GetPet(INT nIndex)
{
	/*if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE) 
	{
		return m_MyStallBox.m_PetList[nIndex].pPet; 
	}
	else*/
		return NULL;
}

INT	CDataPool::MyStallBox_GetCount(VOID)
{
	INT count = 0;
	/*for(INT i=0; i<STALL_PET_BOX_SIZE; i++)
	{
		if(m_MyStallBox.m_PetList[i].pPet == NULL) continue;
		if(INVALID_ID == m_MyStallBox.m_PetList[i].pPet->m_nIsPresent) continue;
		count++;
	}*/
	return count;
}

VOID CDataPool::MyStallBox_SetSkill(INT nIndex,INT nSkillIndex,const _OWN_SKILL *Skill)
{

}

PET_SKILL*	CDataPool::MyStallBox_GetSkill(INT nIndex,INT nSkillIndex)
{
	/*if( nIndex < 0 ||nIndex >= STALL_PET_BOX_SIZE )
		return NULL;
	else
	{
		if(m_MyStallBox.m_PetList[nIndex].pPet == NULL)
			return NULL;
		else
		{
			return m_MyStallBox.m_PetList[nIndex].pPet->m_aSkill[nSkillIndex];
		}
	}*/
	return NULL;
}

//查询一个骑乘宠物是不是被上架了
BOOL CDataPool::MyStallBox_IsPetOnStall(PET_GUID_t PetGuid)
{
	/*for(INT i=0; i<STALL_PET_BOX_SIZE; i++)
	{
		if(m_MyStallBox.m_PetList[i].pPet == NULL)
			continue;
		if(INVALID_ID == m_MyStallBox.m_PetList[i].pPet->m_nIsPresent) 
			continue;

		if(m_MyStallBox.m_PetList[i].pPet->m_GUID == PetGuid)
		{
			return TRUE;
		}
	}*/
	return FALSE;
}

BOOL CDataPool::MyStallBox_IsHaveEmpty(INT nType)
{
	//nType : 1=item    2=pet
	if(nType == 1)  
	{
		for( int i=0; i<20; i++ )
		{
			if(m_MyStallBox.m_ItemList[i].pItem == NULL)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}


VOID	CDataPool::MyStallMsg_ClearUp(VOID)
{
	m_MyStallBox.m_BBS.CleanUp();
}
INT		CDataPool::MyStallMsg_SetIDByIndex(UINT nIndex, UINT nID)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		m_MyStallBox.m_BBS.m_MessageList[nIndex].nID	=	nID;
		return nIndex;
	}
	else
	{
		return -1;
	}
}

INT	CDataPool::MyStallMsg_SetTimeByIndex(UINT	nIndex, BYTE nHour, BYTE nMin)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		m_MyStallBox.m_BBS.m_MessageList[nIndex].nHour	=	nHour;
		m_MyStallBox.m_BBS.m_MessageList[nIndex].nMin	=	nMin;
		return nIndex;
	}
	else
	{
		return -1;
	}
}

INT	CDataPool::MyStallMsg_SetHasReplyByIndex(UINT	nIndex, BYTE bhasReply)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		m_MyStallBox.m_BBS.m_MessageList[nIndex].bHasReply	=	bhasReply;
		return nIndex;
	}
	else
	{
		return -1;
	}
}
INT	CDataPool::MyStallMsg_SetReTimeByIndex(UINT	nIndex, BYTE nHour, BYTE nMin)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		m_MyStallBox.m_BBS.m_MessageList[nIndex].nReHour	=	nHour;
		m_MyStallBox.m_BBS.m_MessageList[nIndex].nReMin		=	nMin;
		return nIndex;
	}
	else
	{
		return -1;
	}
}

VOID	CDataPool::MyStallMsg_SetTitle(LPCTSTR szMsg)
{
	m_MyStallBox.m_BBS.m_szTitle	=	szMsg;
}
INT		CDataPool::MyStallMsg_SetAuthorNameByIndex(UINT	nIndex, LPCTSTR szMsg)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		m_MyStallBox.m_BBS.m_MessageList[nIndex].AuthorName	=	szMsg;
		return nIndex;
	}
	else
	{
		return -1;
	}
}

INT	CDataPool::MyStallMsg_SetProposedMessageByIndex(UINT	nIndex, LPCTSTR szMsg)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		m_MyStallBox.m_BBS.m_MessageList[nIndex].szMessage	=	szMsg;
		return nIndex;
	}
	else
	{
		return -1;
	}
}
INT	CDataPool::MyStallMsg_SetReplyMessageByIndex(UINT nIndex, LPCTSTR szMsg)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		m_MyStallBox.m_BBS.m_MessageList[nIndex].szReplyMessage	=	szMsg;
		return nIndex;
	}
	else
	{
		return -1;
	}
}
INT	CDataPool::MyStallMsg_GetIDByIndex(UINT nIndex)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		return m_MyStallBox.m_BBS.m_MessageList[nIndex].nID;
	}
	else
	{
		return -1;
	}
}
BYTE	CDataPool::MyStallMsg_GetHasReplyByIndex(UINT nIndex)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		return m_MyStallBox.m_BBS.m_MessageList[nIndex].bHasReply;
	}
	else
	{
		return 0;
	}
}
LPCTSTR	CDataPool::MyStallMsg_GetTitle()
{
	return m_MyStallBox.m_BBS.m_szTitle.c_str();
}
LPCTSTR	CDataPool::MyStallMsg_GetAuthorNameByIndex(UINT nIndex)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		return m_MyStallBox.m_BBS.m_MessageList[nIndex].AuthorName.c_str();
	}
	else
	{
		return "";
	}
}
LPCTSTR	CDataPool::MyStallMsg_GetProposedMessageByIndex(UINT nIndex )
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		return m_MyStallBox.m_BBS.m_MessageList[nIndex].szMessage.c_str();
	}
	else
	{
		return "";
	}
}
LPCTSTR	CDataPool::MyStallMsg_GetReplyMessageByIndex(UINT nIndex)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		return m_MyStallBox.m_BBS.m_MessageList[nIndex].szReplyMessage.c_str();
	}
	else
	{
		return "";
	}
}

int CDataPool::MyStallMsg_GetMessageNum()
{
	return m_MyStallBox.m_BBS.m_nMessageNum;
}

VOID CDataPool::MyStallMsg_SetMessageNum(INT nNum)
{
	m_MyStallBox.m_BBS.m_nMessageNum = nNum;
}

int	CDataPool::MyStallMsg_GetHourByIndex(UINT	nIndex)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		return m_MyStallBox.m_BBS.m_MessageList[nIndex].nHour;
	}
	else
	{
		return -1;
	}
}

INT	CDataPool::MyStallMsg_GetMinByIndex(UINT	nIndex)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		return m_MyStallBox.m_BBS.m_MessageList[nIndex].nMin;
	}
	else
	{
		return -1;
	}
}
INT	CDataPool::MyStallMsg_GetReHourByIndex(UINT	nIndex)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		return m_MyStallBox.m_BBS.m_MessageList[nIndex].nReHour;
	}
	else
	{
		return -1;
	}
}
INT	CDataPool::MyStallMsg_GetReMinByIndex(UINT	nIndex)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		return m_MyStallBox.m_BBS.m_MessageList[nIndex].nReMin;
	}
	else
	{
		return -1;
	}
}

void CDataPool::OtStallBox_Clear(BYTE bFlagClearPet)
{
	for(INT i=0; i<STALL_BOX_SIZE; i++)
	{
		if(m_OtStallBox.m_ItemList[i].pItem)
		{
			CObject_Item::DestroyItem((CObject_Item*)m_OtStallBox.m_ItemList[i].pItem);
			m_OtStallBox.m_ItemList[i].pItem	= NULL;
			m_OtStallBox.m_ItemList[i].nPrice	= 0;
			m_OtStallBox.m_ItemList[i].nSerial	= 0;
		}
	}

	if(bFlagClearPet)
	{
		for(INT i=0; i<STALL_PET_BOX_SIZE; i++)
		{
			if(m_OtStallBox.m_PetList[i].pPet)
			{
				SAFE_DELETE(m_OtStallBox.m_PetList[i].pPet);
			}
		}
	}
	m_OtStallBox.CleanUp();
}
VOID CDataPool::OtStallBox_SetItem(INT nStallBoxIndex, tObject_Item* pItem, BOOL bClearOld)
{
	if(	nStallBoxIndex < 0 || nStallBoxIndex>=STALL_BOX_SIZE) return;
	//销毁原有数据
	if( m_OtStallBox.m_ItemList[nStallBoxIndex].pItem && bClearOld)
	{
		CObject_Item::DestroyItem((CObject_Item*)m_OtStallBox.m_ItemList[nStallBoxIndex].pItem);
		m_OtStallBox.m_ItemList[nStallBoxIndex].pItem	= NULL;
		m_OtStallBox.m_ItemList[nStallBoxIndex].nPrice	= 0;
		m_OtStallBox.m_ItemList[nStallBoxIndex].nSerial	= 0;
	}
	if(pItem)
	{
		pItem->SetTypeOwner(tObject_Item::IO_OTSTALLBOX);
		pItem->SetPosIndex(nStallBoxIndex);
	}

	m_OtStallBox.m_ItemList[nStallBoxIndex].pItem = pItem;
}
tObject_Item*	CDataPool::OtStallBox_GetItem(INT nStallBoxIndex)
{
	if(	nStallBoxIndex < 0 || nStallBoxIndex>=STALL_BOX_SIZE) return NULL;
	return m_OtStallBox.m_ItemList[nStallBoxIndex].pItem;
}
VOID CDataPool::OtStallBox_SetItemExtraInfo(INT nStallBoxIndex, BOOL bEmpty, const _ITEM* pItem)
{
	if(nStallBoxIndex >= STALL_BOX_SIZE) return;
	if(bEmpty)
	{
		if(m_OtStallBox.m_ItemList[nStallBoxIndex].pItem)
		{
			CObject_Item::DestroyItem((CObject_Item*)m_OtStallBox.m_ItemList[nStallBoxIndex].pItem);
			m_OtStallBox.m_ItemList[nStallBoxIndex].pItem	= NULL;
			m_OtStallBox.m_ItemList[nStallBoxIndex].nPrice	= 0;
			m_OtStallBox.m_ItemList[nStallBoxIndex].nSerial	= 0;
		}
	}
	else
	{
		if(m_OtStallBox.m_ItemList[nStallBoxIndex].pItem)
			m_OtStallBox.m_ItemList[nStallBoxIndex].pItem->SetExtraInfo(pItem);
	}
}
VOID CDataPool::OtStallBox_SetItemPrice(INT nStallBoxIndex, UINT nPrice)
{
	if(	nStallBoxIndex < 0 || nStallBoxIndex>=STALL_BOX_SIZE) return;
	m_OtStallBox.m_ItemList[nStallBoxIndex].nPrice = nPrice;
}
VOID CDataPool::OtStallBox_SetItemSerial(INT nStallBoxIndex, UINT nSerial)
{
	if(	nStallBoxIndex < 0 || nStallBoxIndex>=STALL_BOX_SIZE) return;
	m_OtStallBox.m_ItemList[nStallBoxIndex].nSerial = nSerial;
}
UINT CDataPool::OtStallBox_GetItemPrice(INT nStallBoxIndex)
{
	if(	nStallBoxIndex < 0 || nStallBoxIndex>=STALL_BOX_SIZE) return 0;
	return m_OtStallBox.m_ItemList[nStallBoxIndex].nPrice;
}
UINT CDataPool::OtStallBox_GetItemSerial(INT nStallBoxIndex)
{
	if(	nStallBoxIndex < 0 || nStallBoxIndex>=STALL_BOX_SIZE) return 0;
	return m_OtStallBox.m_ItemList[nStallBoxIndex].nSerial;
}
INT CDataPool::OtStallBox_GetItemIndexByGUID(__int64 ItemGUID)
{
	INT index;
	for(index = 0; index<STALL_BOX_SIZE; index++)
	{
		if(m_OtStallBox.m_ItemList[index].pItem == NULL)
			continue;
		if(m_OtStallBox.m_ItemList[index].pItem->GetGUID() == ItemGUID)
			break;
	}
	if(index == STALL_BOX_SIZE)
	{
		return -1;
	}
	return index;
}

//摆摊骑乘宠物
INT	CDataPool::OtStallBox_GetPetByGuid(PET_GUID_t PetGuid)
{
	/*for(UINT i = 0; i< STALL_PET_BOX_SIZE; i++)
	{
		if(m_OtStallBox.m_PetList[i].pPet == NULL)
		{
			continue;
		}
		if(m_OtStallBox.m_PetList[i].pPet->m_GUID == PetGuid)
		{
			return i;
		}
	}*/
	return	-1;
}

VOID CDataPool::OtStallBox_SetPet(INT nIndex, SDATA_PET* pPetData, BOOL bClearOld)
{
	/*if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE)
	{
		if(m_OtStallBox.m_PetList[nIndex].pPet != NULL && bClearOld)
		{
			SAFE_DELETE(m_OtStallBox.m_PetList[nIndex].pPet);
		}
		m_OtStallBox.m_PetList[nIndex].pPet = pPetData; 
	}
	else
		return;*/
}

VOID CDataPool::OtStallBox_SetPetPrice(INT nIndex, INT nPetPrice)
{
	/*if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE)
	{
		m_OtStallBox.m_PetList[nIndex].nPrice = nPetPrice;
	}*/
}

VOID CDataPool::OtStallBox_SetPetSerial(INT nIndex, INT nSerial)
{
	/*if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE)
	{
		m_OtStallBox.m_PetList[nIndex].nSerial = nSerial;
	}*/
}

INT	CDataPool::OtStallBox_GetPetPrice(INT nIndex)
{
	/*if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE)
	{
		return m_OtStallBox.m_PetList[nIndex].nPrice;
	}*/
	return 0;
}

INT	CDataPool::OtStallBox_GetPetSerial(INT nIndex)
{
	/*if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE)
	{
		return m_OtStallBox.m_PetList[nIndex].nSerial;
	}*/
	return 0;
}

SDATA_PET*	CDataPool::OtStallBox_GetPet(INT nIndex)
{
	/*if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE) 
	{
		return m_OtStallBox.m_PetList[nIndex].pPet; 
	}
	else*/
		return NULL;
}

INT	CDataPool::OtStallBox_GetCount(VOID)
{
	INT count = 0;
	/*for(INT i=0; i<STALL_PET_BOX_SIZE; i++)
	{
		if(m_OtStallBox.m_PetList[i].pPet == NULL) continue;
		if(INVALID_ID == m_OtStallBox.m_PetList[i].pPet->m_nIsPresent) continue;
		count++;
	}*/
	return count;
}

VOID CDataPool::OtStallBox_SetSkill(INT nIndex,INT nSkillIndex,const _OWN_SKILL *Skill)
{
	if( nIndex < 0 ||nIndex >= STALL_PET_BOX_SIZE )
		return;
	if(m_OtStallBox.m_PetList[nIndex].pPet == NULL)
		return;
	
	/*PET_SKILL* theSkill = m_OtStallBox.m_PetList[nIndex].pPet->m_aSkill[nSkillIndex];

	DBC_DEFINEHANDLE(s_pSkillDBC, DBC_SKILL_DATA);
	const _DBC_SKILL_DATA* pDefine = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_Index_EQU(Skill->m_nSkillID);

	if(!theSkill)
	{
		PET_SKILL* newSkill = new PET_SKILL;

		newSkill->m_pDefine = pDefine;
		newSkill->m_nPetNum = nIndex;
		newSkill->m_nPosIndex = nSkillIndex;
		newSkill->m_bCanUse = TRUE;

		m_OtStallBox.m_PetList[nIndex].pPet->m_aSkill[nSkillIndex] = newSkill;
	}
	else
	{
		theSkill->m_pDefine = pDefine;
		theSkill->m_nPetNum = nIndex;
		theSkill->m_bCanUse = TRUE;
		theSkill->m_nPosIndex = nSkillIndex;
	}*/
}

PET_SKILL*	CDataPool::OtStallBox_GetSkill(INT nIndex,INT nSkillIndex)
{
	/*if( nIndex < 0 ||nIndex >= STALL_PET_BOX_SIZE )
		return NULL;
	else
	{
		if(m_OtStallBox.m_PetList[nIndex].pPet == NULL)
			return NULL;
		else
		{
			return m_MyStallBox.m_PetList[nIndex].pPet->m_aSkill[nSkillIndex];
		}
	}*/
	return NULL;
}

VOID	CDataPool::OtStallMsg_ClearUp(VOID)
{
	m_OtStallBox.m_BBS.CleanUp();
}
INT		CDataPool::OtStallMsg_SetIDByIndex(UINT nIndex, UINT nID)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		m_OtStallBox.m_BBS.m_MessageList[nIndex].nID	=	nID;
		return nIndex;
	}
	else
	{
		return -1;
	}
}

INT		CDataPool::OtStallMsg_SetTimeByIndex(UINT	nIndex, BYTE nHour, BYTE nMin)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		m_OtStallBox.m_BBS.m_MessageList[nIndex].nHour	=	nHour;
		m_OtStallBox.m_BBS.m_MessageList[nIndex].nMin	=	nMin;
		return nIndex;
	}
	else
	{
		return -1;
	}
}

INT	CDataPool::OtStallMsg_SetHasReplyByIndex(UINT	nIndex, BYTE bhasReply)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		m_OtStallBox.m_BBS.m_MessageList[nIndex].bHasReply	=	bhasReply;
		return nIndex;
	}
	else
	{
		return -1;
	}

}

INT		CDataPool::OtStallMsg_SetReTimeByIndex(UINT	nIndex, BYTE nHour, BYTE nMin)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		m_OtStallBox.m_BBS.m_MessageList[nIndex].nReHour	=	nHour;
		m_OtStallBox.m_BBS.m_MessageList[nIndex].nReMin		=	nMin;
		return nIndex;
	}
	else
	{
		return -1;
	}
}

VOID	CDataPool::OtStallMsg_SetTitle(LPCTSTR szMsg)
{
	m_OtStallBox.m_BBS.m_szTitle	=	szMsg;
}
INT		CDataPool::OtStallMsg_SetAuthorNameByIndex(UINT	nIndex, LPCTSTR szMsg)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		m_OtStallBox.m_BBS.m_MessageList[nIndex].AuthorName	=	szMsg;
		return nIndex;
	}
	else
	{
		return -1;
	}
}

INT	CDataPool::OtStallMsg_SetProposedMessageByIndex(UINT	nIndex, LPCTSTR szMsg)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		m_OtStallBox.m_BBS.m_MessageList[nIndex].szMessage	=	szMsg;
		return nIndex;
	}
	else
	{
		return -1;
	}

}
INT	CDataPool::OtStallMsg_SetReplyMessageByIndex(UINT nIndex, LPCTSTR szMsg)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		m_OtStallBox.m_BBS.m_MessageList[nIndex].szReplyMessage	=	szMsg;
		return nIndex;
	}
	else
	{
		return -1;
	}
}
INT	CDataPool::OtStallMsg_GetIDByIndex(UINT nIndex)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		return m_OtStallBox.m_BBS.m_MessageList[nIndex].nID;
	}
	else
	{
		return -1;
	}
}
BYTE	CDataPool::OtStallMsg_GetHasReplyByIndex(UINT nIndex)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		return m_OtStallBox.m_BBS.m_MessageList[nIndex].bHasReply;
	}
	else
	{
		return 0;
	}
}
LPCTSTR	CDataPool::OtStallMsg_GetTitle()
{
	return m_OtStallBox.m_BBS.m_szTitle.c_str();
}
LPCTSTR	CDataPool::OtStallMsg_GetAuthorNameByIndex(UINT nIndex)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		return m_OtStallBox.m_BBS.m_MessageList[nIndex].AuthorName.c_str();
	}
	else
	{
		return "";
	}
}
LPCTSTR	CDataPool::OtStallMsg_GetProposedMessageByIndex(UINT nIndex )
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		return m_OtStallBox.m_BBS.m_MessageList[nIndex].szMessage.c_str();
	}
	else
	{
		return "";
	}
}
LPCTSTR	CDataPool::OtStallMsg_GetReplyMessageByIndex(UINT nIndex)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		return m_OtStallBox.m_BBS.m_MessageList[nIndex].szReplyMessage.c_str();
	}
	else
	{
		return "";
	}
}
INT	CDataPool::OtStallMsg_GetHourByIndex(UINT	nIndex)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		return m_OtStallBox.m_BBS.m_MessageList[nIndex].nHour;
	}
	else
	{
		return -1;
	}
}
INT	CDataPool::OtStallMsg_GetMinByIndex(UINT	nIndex)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		return m_OtStallBox.m_BBS.m_MessageList[nIndex].nMin;
	}
	else
	{
		return -1;
	}
}
INT	CDataPool::OtStallMsg_GetReHourByIndex(UINT	nIndex)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		return m_OtStallBox.m_BBS.m_MessageList[nIndex].nReHour;
	}
	else
	{
		return -1;
	}
}
INT	CDataPool::OtStallMsg_GetReMinByIndex(UINT	nIndex)
{
	if( nIndex>=0 && nIndex<MAX_BBS_MESSAGE_NUM )
	{
		return m_OtStallBox.m_BBS.m_MessageList[nIndex].nReMin;
	}
	else
	{
		return -1;
	}
}
INT CDataPool::OtStallMsg_GetMessageNum()
{
	return m_OtStallBox.m_BBS.m_nMessageNum;
}
VOID CDataPool::OtStallMsg_SetMessageNum(INT nNum)
{
	m_OtStallBox.m_BBS.m_nMessageNum = nNum;
}



VOID CDataPool::Pet_SetSkill(INT nIndex, INT nSkillIndex, INT nSkillID, BOOL bCanUse)
{
	if( nIndex < 0 ||nIndex >= HUMAN_PET_MAX_COUNT )
		return;
	if (nSkillIndex<0 || nSkillIndex >= PET_MAX_SKILL_COUNT)
		return;	

	PET_SKILL& theSkill = m_listPet[nIndex].m_Skill[nSkillIndex];

	DBC_DEFINEHANDLE(s_pSkillDBC, DBC_SKILL_DATA);
	const _DBC_SKILL_DATA* pDefine = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_Index_EQU(nSkillID);
	// 添加或更改技能
	if (pDefine)
	{
		theSkill.m_pDefine		= pDefine;
		theSkill.m_bCanUse		= bCanUse;			
	}
	// 删除技能
	else
	{
		theSkill.CleanUp();
		theSkill.m_bCanUse		= bCanUse;
	}

	//通知ActionSystem。
	CActionSystem::GetMe()->UserPetSkill_Update(nIndex);
}

VOID CDataPool::TargetPet_SetSkill(INT nSkillIndex, INT nSkillID, BOOL bCanUse)
{
	if (nSkillIndex < 0 || nSkillIndex >= PET_MAX_SKILL_COUNT)
		return;

	// 保存到骑乘宠物技能
	PET_SKILL& theSkill = m_TargetPet.m_Skill[nSkillIndex];

	DBC_DEFINEHANDLE(s_pSkillDBC, DBC_SKILL_DATA);
	const _DBC_SKILL_DATA* pDefine = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_Index_EQU(nSkillID);
	if (pDefine)
	{
		theSkill.m_pDefine		= pDefine;
		theSkill.m_bCanUse		= bCanUse;
	}
	else
	{
		theSkill.CleanUp();
		theSkill.m_bCanUse		= bCanUse;
	}

	//通知ActionSystem。
	CActionSystem::GetMe()->UserTargetPetSkill_Update();

}

PET_SKILL* CDataPool::Pet_GetSkill(INT nIndex,INT nSkillIndex)
{
	if(nIndex < 0 || nIndex >= HUMAN_PET_MAX_COUNT)
		return NULL;
	if(nSkillIndex < 0 || nSkillIndex >= PET_MAX_SKILL_COUNT)
		return NULL;

	PET_SKILL& PetSkill = m_listPet[nIndex].m_Skill[nSkillIndex];

	if(NULL == PetSkill.m_pDefine)
		return NULL;

	return &PetSkill;
}

PET_SKILL* CDataPool::TargetPet_GetSkill(INT nSkillIndex)
{
	if(nSkillIndex < 0 || nSkillIndex >= PET_MAX_SKILL_COUNT)
		return NULL;

	PET_SKILL& PetSkill = m_TargetPet.m_Skill[nSkillIndex];

	if(NULL == PetSkill.m_pDefine)
		return NULL;

	return &PetSkill;


}

VOID CDataPool::Pet_SetSkill(INT nIndex, INT nSkillIndex, PET_SKILL* pPetSkillData, BOOL bClearOld)
{
	//if (NULL == pPetSkillData)
	//	return;
	//if( nIndex < 0 ||nIndex >= HUMAN_PET_MAX_COUNT )
	//	return;
	//if (nSkillIndex<0 || nSkillIndex >= PET_MAX_SKILL_COUNT)
	//	return;


	//if(m_listPet[nIndex].m_Skill[nSkillIndex] != NULL && bClearOld)
	//{
	//	SAFE_DELETE(m_listPet[nIndex].m_Skill[nSkillIndex]);
	//}
	//m_listPet[nIndex].m_Skill[nSkillIndex] = pPetSkillData;

	////通知ActionSystem。
	//CActionSystem::GetMe()->UserPetSkill_Update(nIndex);
}

//暂时此函数只用来清空targetpet里的技能数据
VOID CDataPool::TargetPet_SetSkill(INT nSkillIndex, PET_SKILL* pPetSkillData, BOOL bClearOld)
{
	//if (NULL == pPetSkillData)
	//	return;
	//if (nSkillIndex < 0 || nSkillIndex >= PET_MAX_SKILL_COUNT)
	//	return;

	//PET_SKILL* theSkill = m_TargetPet.m_Skill[nSkillIndex];

	//if(pPetSkillData == NULL && theSkill != NULL && bClearOld)
	//{
	//	SAFE_DELETE(m_TargetPet.m_Skill[nSkillIndex]);
	//	m_TargetPet.m_Skill[nSkillIndex] = pPetSkillData;
	//	//通知ActionSystem。
	//	CActionSystem::GetMe()->CleanInvalidAction();
	//	return;
	//}
}

INT	CDataPool::Pet_GetCount(VOID)
{
	INT count = 0;
	for(INT i=0; i<HUMAN_PET_MAX_COUNT; i++)
	{
		if( NULL != m_listPet[i].m_pModelData) 
			count++;
	}
	return count;
}

BOOL CDataPool::Pet_SendUseSkillMessage( SkillID_t idSkill, ObjID_t idTargetObj, FLOAT fTargetX, FLOAT fTargetZ )
{
	SDATA_PET *pPetData = Pet_GetPet(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID());

	if ( pPetData && FALSE == pPetData->m_GUID.IsNull() )
	{
		CGCharUseSkill msg;

		msg.setObjID( CObjectManager::GetMe()->GetMySelf()->GetServerID() );
		msg.setSkillDataID( idSkill );
		msg.setTargetID( idTargetObj );
		msg.setTargetPos( WORLD_POS(fTargetX, fTargetZ) );
		msg.setDir( -1.0f );
		msg.setTargetGUID( INVALID_GUID );

		CNetManager::GetMe()->SendPacket( &msg );
	}

	return TRUE;
}
VOID CDataPool::TargetPet_SetPetFakeModel(INT nMountID)
{
	if (NULL == m_TargetPetAvatar.pAvatar)
		return;

	// UI模型没有变化
	if (m_TargetPetAvatar.nMountID == nMountID)
	{
		// 更新特效
		m_TargetPetAvatar.pAvatar->SetNpcMountEffect(nMountID, &m_TargetPet);
	}
	// 创建新的UI模型
	else
	{
		// 更新模型
		if (m_TargetPet.m_pModelData)
		{
			m_TargetPetAvatar.pAvatar->GetCharacterData()->Set_ModelID( m_TargetPet.m_pModelData->m_nModelID );
		}		
		// 更新特效
		m_TargetPetAvatar.pAvatar->SetNpcMountEffect(nMountID, &m_TargetPet);
		// 朝向
		m_TargetPetAvatar.pAvatar->SetFaceDir(-1.0f);
	}	
}
const CHAR*	CDataPool::TargetPet_GetPetFakeModel()
{
	return m_TargetPetAvatar.strAvatarName.c_str();

	// 目标坐骑是否和当前UI坐骑相同
	/*tObject* pObj = CObjectManager::GetMe()->GetMainTarget();
	if(!pObj || !g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
	{
		CObject_Character* pCharObj = (CObject_Character*)pObj;
		if (pCharObj->GetCharacterData()->Get_MountID() == m_TargetPetAvatar.nMountID)
		{
			return m_TargetPetAvatar.strAvatarName.c_str();
		}
	}

    return NULL;	*/
}

const _BUFF_IMPACT_INFO* CDataPool::BuffImpact_Get( INT nSN )const
{
	CBuffImpactMap::const_iterator	itFind, itEnd;
	itFind	= m_mapBuffImpact.find( nSN );
	itEnd	= m_mapBuffImpact.end();
	if ( itFind != itEnd )
	{
		return itFind->second;
	}
	return NULL;
}

const _BUFF_IMPACT_INFO* CDataPool::BuffImpact_GetByIndex( INT nIndex )const
{
	if ( (INT)(m_mapBuffImpact.size()) > nIndex )
	{
		CBuffImpactMap::const_iterator	itCur, itEnd;
		INT nCurIndex;
		itCur		= m_mapBuffImpact.begin();
		itEnd		= m_mapBuffImpact.end();
		nCurIndex	= 0;
		while ( itCur != itEnd )
		{
			if ( nCurIndex == nIndex )
			{
				return itCur->second;
			}
			nCurIndex++;
			itCur++;
		}
	}
	return NULL;
}

const _BUFF_IMPACT_INFO* CDataPool::BuffImpact_GetByID( INT nID )const
{
	// 20100423ModifyCode  nID是技能效果ID，有5位数，而m_mapBuffImpact为当前释放的技能效果队列，两个值不能比较
	// if ( (INT)(m_mapBuffImpact.size()) > nID )
	if ( (INT)(m_mapBuffImpact.size()) > 0 )
	{
		CBuffImpactMap::const_iterator	itCur, itEnd;
		const _BUFF_IMPACT_INFO *pBuffImpactInfo;
		itCur		= m_mapBuffImpact.begin();
		itEnd		= m_mapBuffImpact.end();
		while ( itCur != itEnd )
		{
			pBuffImpactInfo = itCur->second;
			if ( pBuffImpactInfo->m_nBuffID == nID )
			{
				return pBuffImpactInfo;
			}
			itCur++;
		}
	}
	return NULL;
}

BOOL CDataPool::BuffImpact_Add( const _BUFF_IMPACT_INFO *pBuffImpact )
{
	if( pBuffImpact == NULL )
		return FALSE;

	INT nSN = pBuffImpact->m_nSN;

	_BUFF_IMPACT_INFO *pFindBuffImpact;

	// 20100423 ModifyCodeBegin 因为每次的nSN都不一样，即nSN是技能释放效果的唯一关键字，所以不能通过这个判断，要通过impactID技能效果ID判断
	// itFind	= m_mapBuffImpact.find( nSN );
	CBuffImpactMap::iterator itFind, itEnd, itCur;	

	itEnd   = m_mapBuffImpact.end();
	itCur	= m_mapBuffImpact.begin();
	itFind  = itEnd; // 初始化itFind为未找到状态，在下面的代码如果找到会重新赋值为找到结点的iterator

	while ( itCur != itEnd )
	{
		if ( itCur->second->m_nBuffID == pBuffImpact->m_nBuffID )
		{
			itFind =  itCur;
			break;
		}

		itCur++;
	}
	// 20100423 ModifyCodeEnd

	if( itFind == itEnd )
	{
		pFindBuffImpact = new _BUFF_IMPACT_INFO;
		m_mapBuffImpact.insert( CBuffImpactMap::value_type( nSN, pFindBuffImpact ) );

		itFind = m_mapBuffImpact.find( nSN );
		if( itFind == itEnd )
		{
			Assert( itFind != itEnd && "CDataPool::BuffImpact_Add" );
			delete pFindBuffImpact;
			return FALSE;
		}
	}
	else
	{
		pFindBuffImpact = itFind->second;
	}

	*pFindBuffImpact = *pBuffImpact;

	CEventSystem::GetMe()->PushEvent(GE_IMPACT_SELF_UPDATE);

	return TRUE;
}

VOID CDataPool::BuffImpact_Remove( INT nSN )
{
	_BUFF_IMPACT_INFO *pBuffImpact;
	CBuffImpactMap::iterator	itFind, itEnd;
	itFind	= m_mapBuffImpact.find( nSN );
	itEnd	= m_mapBuffImpact.end();
	if ( itFind != itEnd )
	{
		pBuffImpact = itFind->second;
		delete pBuffImpact;
		pBuffImpact = NULL;
		m_mapBuffImpact.erase( itFind );
	}

	CEventSystem::GetMe()->PushEvent(GE_IMPACT_SELF_UPDATE);
}

VOID CDataPool::BuffImpact_RemoveByIndex( INT nIndex )
{
	if ( (INT)(m_mapBuffImpact.size()) > nIndex )
	{
		_BUFF_IMPACT_INFO *pBuffImpact;
		CBuffImpactMap::iterator	itCur, itEnd;
		INT nCurIndex;
		itCur		= m_mapBuffImpact.begin();
		itEnd		= m_mapBuffImpact.end();
		nCurIndex	= 0;
		while ( itCur != itEnd )
		{
			if ( nCurIndex == nIndex )
			{
				pBuffImpact = itCur->second;
				delete pBuffImpact;
				pBuffImpact = NULL;
				m_mapBuffImpact.erase( itCur );
			}
			nCurIndex++;
			itCur++;
		}
	}

	CEventSystem::GetMe()->PushEvent(GE_IMPACT_SELF_UPDATE);
}

VOID CDataPool::BuffImpact_RemoveAll( VOID )
{
	_BUFF_IMPACT_INFO *pBuffImpact;
	CBuffImpactMap::iterator itCur, itEnd;
	itCur		= m_mapBuffImpact.begin();
	itEnd		= m_mapBuffImpact.end();
	while ( itCur != itEnd )
	{
		pBuffImpact = itCur->second;
		delete pBuffImpact;
		itCur++;
	}
	m_mapBuffImpact.erase( m_mapBuffImpact.begin(), m_mapBuffImpact.end() );

	CEventSystem::GetMe()->PushEvent(GE_IMPACT_SELF_UPDATE);
}

const CHAR* CDataPool::BuffImpact_GetToolTips( INT nSN )const
{
	const _BUFF_IMPACT_INFO* pBuffImpactInfo = BuffImpact_Get( nSN );
	if ( pBuffImpactInfo != NULL )
	{
		_BUFF_IMPACT *pBuffImpact = GetBuffImpactMgr()->GetBuffImpact( pBuffImpactInfo->m_nBuffID );
		if ( pBuffImpact != NULL )
		{
			return pBuffImpact->m_pszInfo;
		}
	}
	return NULL;
}

const CHAR* CDataPool::BuffImpact_GetToolTipsByIndex( INT nIndex )const
{
	const _BUFF_IMPACT_INFO* pBuffImpactInfo = BuffImpact_GetByIndex( nIndex );
	if ( pBuffImpactInfo != NULL )
	{
		_BUFF_IMPACT *pBuffImpact = GetBuffImpactMgr()->GetBuffImpact( pBuffImpactInfo->m_nBuffID );
		if ( pBuffImpact != NULL )
		{
			return pBuffImpact->m_pszInfo;
		}
	}
	return NULL;
}

BOOL CDataPool::BuffImpact_Dispel( INT nSN )
{
	const _BUFF_IMPACT_INFO *pBuffImpactInfo = BuffImpact_Get( nSN );
	if ( pBuffImpactInfo != NULL )
	{
		CGDispelBuffReq msgDispelBuffReq;
		msgDispelBuffReq.SetSN( nSN );
		CNetManager::GetMe()->SendPacket( &msgDispelBuffReq );
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CDataPool::BuffImpact_DispelByIndex( INT nIndex )
{
	const _BUFF_IMPACT_INFO *pBuffImpactInfo = BuffImpact_GetByIndex( nIndex );
	if ( pBuffImpactInfo != NULL )
	{
		CGDispelBuffReq msgDispelBuffReq;
		msgDispelBuffReq.SetSN( pBuffImpactInfo->m_nSN );
		CNetManager::GetMe()->SendPacket( &msgDispelBuffReq );
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

INT CDataPool::BuffImpact_GetTime( INT nIndex )
{
	const _BUFF_IMPACT_INFO *pBuffImpactInfo = BuffImpact_Get( nIndex );
	if ( pBuffImpactInfo != NULL )
	{
		return pBuffImpactInfo->m_nTimer;
	}
	else
	{
		return -1;
	}
}

INT CDataPool::BuffImpact_GetTimeByIndex( INT nIndex )
{
	const _BUFF_IMPACT_INFO *pBuffImpactInfo = BuffImpact_GetByIndex( nIndex );
	if ( pBuffImpactInfo != NULL )
	{
		return pBuffImpactInfo->m_nTimer;
	}
	else
	{
		return -1;
	}
}

INT	CDataPool::Pet_GetPetByGuid(const PET_GUID_t& PetGuid)
{
	if (FALSE == PetGuid.IsNull())
	{
		for (INT i = 0; i< HUMAN_PET_MAX_COUNT; ++i)
		{
			if(m_listPet[i].m_GUID == PetGuid)
			{
				return i;
			}
		}
	}
	
	return	-1;
}


SDATA_PET* CDataPool::Pet_GetPet(INT nIndex) 
{
	if(nIndex >=0 && nIndex < HUMAN_PET_MAX_COUNT) 
		return &(m_listPet[nIndex]); 

	return NULL;
}

SDATA_PET* CDataPool::Pet_GetPet(const PET_GUID_t& PetGuid)
{
	if (FALSE == PetGuid.IsNull())
	{
		for (INT i=0; i<HUMAN_PET_MAX_COUNT; ++i)
		{
			if (m_listPet[i].m_GUID == PetGuid)
			{
				return &m_listPet[i];
			}
		}
	}

	return NULL;
}

INT	CDataPool::Pet_GetPetFoodType(PET_GUID_t guidPet)
{
	const tDataBase* pPetExtDB = CDataBaseSystem::GetMe()->GetDataBase(DBC_PET_EX_ATT);
	INT nFoodType = PET_FOOD_TYPE_INVALID;

	////获取食物类型
	//if(pPetExtDB && Pet_GetPet(guidPet))
	//{
	//	const _DBC_PET_EX_ATT* pLine = (const _DBC_PET_EX_ATT*)pPetExtDB->Search_Index_EQU(Pet_GetPet(guidPet)->m_nDataID);
	//	if(pLine)
	//	{
	//		nFoodType = pLine->m_FoodType;
	//	}
	//}

	return nFoodType;
}

VOID CDataPool::Pet_GetPetFoodTypeMinMax(INT nFoodType, INT* idMin /* = NULL */, INT* idMax /* = NULL */)
{
	//食物的最小及最大idTable号
	/*if(idMin && idMax)
	{
		switch(nFoodType)
		{
		case PET_FOOD_TYPE_MEAT:
			*idMin = ITEM_MEAT_MEDICINE;
			*idMax = ITEM_GRASS_MEDICINE-1;
			break;
		case PET_FOOD_TYPE_GRASS:
			*idMin = ITEM_GRASS_MEDICINE;
			*idMax = ITEM_WORM_MEDICINE-1;
			break;
		case PET_FOOD_TYPE_WORM:
			*idMin = ITEM_WORM_MEDICINE;
			*idMax = ITEM_PADDY_MEDICINE-1;
			break;
		case PET_FOOD_TYPE_PADDY:
			*idMin = ITEM_PADDY_MEDICINE;
			*idMax = ITEM_PET_FEED_MEDICINE_MAX-1;
			break;
		default:
			*idMin = -1;
			*idMax = -1;
			break;
		}
	}*/
}

tObject_Item* CDataPool::Pet_GetLowestLevel_Food_From_Package(PET_GUID_t pg,INT *idxPackage /* = NULL */)
{
	CObject_Item* pFind = NULL;
	//if(idxPackage) *idxPackage = -1;

	////获取食物类型
	//INT idTableMin = -1, idTableMax = -1, mod = 1000, modold = 0;
	//INT nFoodType = CDataPool::GetMe()->Pet_GetPetFoodType(pg);

	//INT k = PET_FOOD_TYPE_MEAT;
	////根据食物类型来判断循环范围
	//if(nFoodType >=1000)
	//{
	//	k = PET_FOOD_TYPE_MEAT;
	//	mod = 1000;
	//}
	//else if(nFoodType >=100)
	//{
	//	k = PET_FOOD_TYPE_GRASS;
	//	mod = 100;
	//}
	//else if(nFoodType >=10)
	//{
	//	k = PET_FOOD_TYPE_WORM;
	//	mod = 10;
	//}
	//else
	//{
 //       k = PET_FOOD_TYPE_PADDY;
	//	mod = 1;
	//}

	////开始查找物品
	//for(; k < PET_FOOD_TYPE_NUMBER; ++k)
	//{
	//	if(0 == mod) break; //keep loop safe.
	//	//calculate food type enum
	//	INT q = (nFoodType-modold)/mod;
	//	idTableMin = -1, idTableMax = -1;
	//	if(1 == q) Pet_GetPetFoodTypeMinMax(k, &idTableMin, &idTableMax);
	//	if(idTableMin < 0 || idTableMax < 0) continue;

	//	//检查背包里等级最低的食物
	//	//for(INT i = BASE_CONTAINER_OFFSET; i < MAT_CONTAINER_OFFSET; ++i)
	//	//{
	//	//	CObject_Item* pItem = (CObject_Item*)CDataPool::GetMe()->UserBag_GetItem(i);
	//	//	if(pItem)
	//	//	{
	//	//		INT id = pItem->GetIdTable();
	//	//		//id是否是食物
	//	//		if(id > idTableMax || id < idTableMin) continue;
	//	//		//道具的使用等级是否合法
	//	//		if(pItem->GetNeedLevel() > Pet_GetPet(pg)->m_nLevel) continue;

	//	//		if(pFind)
	//	//		{
	//	//			CObject_Item* pOldFind = pFind;
	//	//			pFind = (pItem->GetNeedLevel() > pFind->GetNeedLevel())?pFind:pItem;
	//	//			if(pFind != pOldFind && idxPackage) *idxPackage = i;
	//	//		}
	//	//		else
	//	//		{
	//	//			pFind = pItem;
	//	//			if(idxPackage) *idxPackage = i;
	//	//		}
	//	//	}// end of if(pItem)
	//	//}

	//	//change mod
	//	modold += mod;
	//	mod /= 10;
	//}

	return (tObject_Item*)pFind;
}

tObject_Item* CDataPool::Pet_GetLowestLevel_Dome_From_Package(PET_GUID_t pg, INT *idxPackage /* = NULL */)
{
	CObject_Item* pFind = NULL;
	if(idxPackage) *idxPackage = -1;

	INT idTableMin = ITEM_DOME_MEDICINE, idTableMax = ITEM_MEAT_MEDICINE-1;

	//for(INT i = BASE_CONTAINER_OFFSET; i < MAT_CONTAINER_OFFSET; ++i)
	//{
	//	CObject_Item* pItem = (CObject_Item*)CDataPool::GetMe()->UserBag_GetItem(i);
	//	if(pItem)
	//	{
	//		INT id = pItem->GetIdTable();
	//		if(id > idTableMax || id < idTableMin) continue;
	//		if(pItem->GetNeedLevel() > Pet_GetPet(pg)->m_nLevel) continue;

	//		if(pFind)
	//		{
	//			CObject_Item* pOldFind = pFind;
	//			pFind = (pItem->GetNeedLevel() > pFind->GetNeedLevel())?pFind:pItem;
	//			if(pFind != pOldFind && idxPackage) *idxPackage = i;
	//		}
	//		else
	//		{
	//			pFind = pItem;
	//			if(idxPackage) *idxPackage = i;
	//		}			
	//	}// end of if(pItem)
	//}

	return (tObject_Item*)pFind;
}

VOID CDataPool::Pet_ClearPet(INT nIndex)
{
	if(nIndex >=0 && nIndex < HUMAN_PET_MAX_COUNT) 
	{
		m_listPet[nIndex].CleanUp();
	}
}

VOID CDataPool::Pet_CopyToTarget(SDATA_PET *pSourcePet) 
{
//	SDATA_PET* pTargetPet = CDataPool::GetMe()->TargetPet_GetPet();
//	pTargetPet->CleanUp();
//	CActionSystem::GetMe()->CleanInvalidAction();
////	memcpy(pTargetPet,pSourcePet,sizeof(SDATA_PET));
//	pTargetPet->m_nIsPresent		= pSourcePet->m_nIsPresent;
//	pTargetPet->m_GUID				= pSourcePet->m_GUID;
//	pTargetPet->m_idServer			= pSourcePet->m_idServer;
//	pTargetPet->m_nDataID			= pSourcePet->m_nDataID;
//	pTargetPet->m_nAIType			= pSourcePet->m_nAIType;
//	pTargetPet->m_szName			= pSourcePet->m_szName;
//	pTargetPet->m_nLevel			= pSourcePet->m_nLevel;
//	pTargetPet->m_nExp				= pSourcePet->m_nExp;
//	pTargetPet->m_nHP				= pSourcePet->m_nHP;
//	pTargetPet->m_nHPMax			= pSourcePet->m_nHPMax;
//	pTargetPet->m_nAge				= pSourcePet->m_nAge;
//	pTargetPet->m_nEraCount			= pSourcePet->m_nEraCount;
//	pTargetPet->m_nHappiness		= pSourcePet->m_nHappiness;
//	pTargetPet->m_SpouseGUID		= pSourcePet->m_SpouseGUID;
//	pTargetPet->m_nModelID			= pSourcePet->m_nModelID;
//	pTargetPet->m_nMountID			= pSourcePet->m_nMountID;
////	pTargetPet->m_nAttPhysics		= pSourcePet->m_nAttPhysics;
//	pTargetPet->m_nAttMagic			= pSourcePet->m_nAttMagic;
////	pTargetPet->m_nDefPhysics		= pSourcePet->m_nDefPhysics;
//
//	pTargetPet->m_nAtt_Near		= pSourcePet->m_nAtt_Near;
//	pTargetPet->m_nDef_Near		= pSourcePet->m_nDef_Near;
//	pTargetPet->m_nAtt_Far		= pSourcePet->m_nAtt_Far;
//	pTargetPet->m_nDef_Far		= pSourcePet->m_nDef_Far;
//
//	pTargetPet->m_nDefMagic			= pSourcePet->m_nDefMagic;
//	pTargetPet->m_nHit				= pSourcePet->m_nHit;
//	pTargetPet->m_nMiss				= pSourcePet->m_nMiss;
//	pTargetPet->m_nCritical			= pSourcePet->m_nCritical;
//	pTargetPet->m_nAttrStrApt		= pSourcePet->m_nAttrStrApt;
//	pTargetPet->m_nAttrConApt		= pSourcePet->m_nAttrConApt;
//	pTargetPet->m_nAttrDexApt		= pSourcePet->m_nAttrDexApt;
////	pTargetPet->m_nAttrSprApt		= pSourcePet->m_nAttrSprApt;
//	pTargetPet->m_nAttrIntApt		= pSourcePet->m_nAttrIntApt;
//	pTargetPet->m_nAttrStr			= pSourcePet->m_nAttrStr;
//	pTargetPet->m_nAttrCon			= pSourcePet->m_nAttrCon;
//	pTargetPet->m_nAttrDex			= pSourcePet->m_nAttrDex;
////	pTargetPet->m_nAttrSpr			= pSourcePet->m_nAttrSpr;
//	pTargetPet->m_nAttrInt			= pSourcePet->m_nAttrInt;
//	pTargetPet->m_nBasic			= pSourcePet->m_nBasic;
//	pTargetPet->m_nPot				= pSourcePet->m_nPot;
//
//	PET_SKILL* pDestPetSkill = NULL;
//	PET_SKILL* pSourcePetSkill = NULL;
//
//	for(INT i = 0; i<PET_MAX_SKILL_COUNT; i++ )
//	{
////		TargetPet_SetSkill(i,NULL,TRUE);
//		pSourcePetSkill = pSourcePet->m_aSkill[i];
//		if( pSourcePetSkill )
//		{
//			pDestPetSkill = pTargetPet->m_aSkill[i];
//			if(!pDestPetSkill)
//			{
//				pTargetPet->m_aSkill[i] = new PET_SKILL;
//				pDestPetSkill = pTargetPet->m_aSkill[i];
//			}
//
//			pDestPetSkill->m_bCanUse	= pSourcePetSkill->m_bCanUse;
//			pDestPetSkill->m_nPetNum	= TARGETPET_INDEX;//pPetSourceData->m_aSkill[i]->m_nPetNum;
//			pDestPetSkill->m_nPosIndex	= i;//pPetSourceData->m_aSkill[i]->m_nPosIndex;
//			pDestPetSkill->m_pDefine	= pSourcePetSkill->m_pDefine;
//		}
//	}
//
//	CActionSystem::GetMe()->UserTargetPetSkill_Update();
}

INT	CDataPool::MyExBox_GetPetByGuid(PET_GUID_t PetGuid)
{
	/*for(UINT i = 0; i< EXCHANGE_PET_BOX_SIZE; i++)
	{
		if(m_MyExchangeBox.m_Petlist[i] == NULL)
		{
			continue;
		}
		if(m_MyExchangeBox.m_Petlist[i]->m_GUID == PetGuid)
		{
			return i;
		}
	}*/
	return	-1;
}

VOID CDataPool::MyExBox_SetPet(INT nIndex, SDATA_PET* pPetData, BOOL bClearOld)
{
	/*if(nIndex >=0 && nIndex < EXCHANGE_PET_BOX_SIZE)
	{
		if(m_MyExchangeBox.m_Petlist[nIndex] != NULL && bClearOld)
		{
			SAFE_DELETE(m_MyExchangeBox.m_Petlist[nIndex]);
		}
		m_MyExchangeBox.m_Petlist[nIndex] = pPetData; 
	}
	else
		return;*/
}


SDATA_PET*	CDataPool::MyExBox_GetPet(INT nIndex)
{
	/*if(nIndex >=0 && nIndex < EXCHANGE_PET_BOX_SIZE) 
		return m_MyExchangeBox.m_Petlist[nIndex]; 
	else*/
		return NULL;
}

INT	CDataPool::MyExBox_GetCount(VOID)
{
	INT count = 0;
	/*for(INT i=0; i<EXCHANGE_PET_BOX_SIZE; i++)
	{
		if(m_MyExchangeBox.m_Petlist[i])
		{
			if(INVALID_ID == m_MyExchangeBox.m_Petlist[i]->m_nIsPresent) 
				continue;
			count++;
		}
	}*/
	return count;
}

VOID CDataPool::MyExBox_SetSkill(INT nIndex,INT nSkillIndex,const _OWN_SKILL *Skill)
{
	/*if( nIndex < 0 ||nIndex >= EXCHANGE_PET_BOX_SIZE )
		return;
	PET_SKILL* theSkill = m_MyExchangeBox.m_Petlist[nIndex]->m_aSkill[nSkillIndex];

	DBC_DEFINEHANDLE(s_pSkillDBC, DBC_SKILL_DATA);
	const _DBC_SKILL_DATA* pDefine = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_Index_EQU(Skill->m_nSkillID);

	if(!theSkill)
	{
		PET_SKILL* newSkill = new PET_SKILL;

		newSkill->m_pDefine = pDefine;
		newSkill->m_nPetNum = nIndex;
		newSkill->m_nPosIndex = nSkillIndex;
		newSkill->m_bCanUse = TRUE;

		m_MyExchangeBox.m_Petlist[nIndex]->m_aSkill[nSkillIndex] = newSkill;
	}
	else
	{
		theSkill->m_pDefine = pDefine;
		theSkill->m_nPetNum = nIndex;
		theSkill->m_bCanUse = TRUE;
		theSkill->m_nPosIndex = nSkillIndex;
	}*/
}

PET_SKILL*	CDataPool::MyExBox_GetSkill(INT nIndex,INT nSkillIndex)
{
	/*if( nIndex < 0 ||nIndex >= EXCHANGE_PET_BOX_SIZE )
		return NULL;
	else
        return m_MyExchangeBox.m_Petlist[nIndex]->m_aSkill[nSkillIndex];*/
	return NULL;
}

INT	CDataPool::OtExBox_GetPetByGuid(PET_GUID_t PetGuid)
{
	/*for(UINT i = 0; i< EXCHANGE_PET_BOX_SIZE; i++)
	{
		if(m_OtherExchangeBox.m_Petlist[i] == NULL)
		{
			continue;
		}
		if(m_OtherExchangeBox.m_Petlist[i]->m_GUID == PetGuid)
		{
			return i;
		}
	}*/
	return	-1;
}

VOID CDataPool::OtExBox_SetPet(INT nIndex, SDATA_PET* pPetData, BOOL bClearOld)
{
	/*if(nIndex >=0 && nIndex < EXCHANGE_PET_BOX_SIZE)
	{
		if(m_OtherExchangeBox.m_Petlist[nIndex] != NULL && bClearOld)
		{
			SAFE_DELETE(m_OtherExchangeBox.m_Petlist[nIndex]);
		}
		m_OtherExchangeBox.m_Petlist[nIndex] = pPetData; 
	}
	else
		return;*/
}

SDATA_PET*	CDataPool::OtExBox_GetPet(INT nIndex)
{
	/*if(nIndex >=0 && nIndex < EXCHANGE_PET_BOX_SIZE) 
		return m_OtherExchangeBox.m_Petlist[nIndex]; 
	else*/
		return NULL;
}

INT	CDataPool::OtExBox_GetCount(VOID)
{
	INT count = 0;
	/*for(INT i=0; i<EXCHANGE_PET_BOX_SIZE; i++)
	{
		if(m_OtherExchangeBox.m_Petlist[i] )
		{
			if(INVALID_ID == m_OtherExchangeBox.m_Petlist[i]->m_nIsPresent ) 
				continue;
			count++;
		}
	}*/
	return count;
}

VOID CDataPool::OtExBox_SetSkill(INT nIndex,INT nSkillIndex,const _OWN_SKILL *Skill)
{
	/*if( nIndex < 0 ||nIndex >= EXCHANGE_PET_BOX_SIZE )
		return;
	PET_SKILL* theSkill = m_OtherExchangeBox.m_Petlist[nIndex]->m_aSkill[nSkillIndex];

	DBC_DEFINEHANDLE(s_pSkillDBC, DBC_SKILL_DATA);
	const _DBC_SKILL_DATA* pDefine = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_Index_EQU(Skill->m_nSkillID);

	if(!theSkill)
	{
		PET_SKILL* newSkill = new PET_SKILL;

		newSkill->m_pDefine		= pDefine;
		newSkill->m_nPetNum		= nIndex;
		newSkill->m_nPosIndex	= nSkillIndex;
		newSkill->m_bCanUse		= TRUE;

		m_OtherExchangeBox.m_Petlist[nIndex]->m_aSkill[nSkillIndex] = newSkill;
	}
	else
	{
		theSkill->m_pDefine = pDefine;
		theSkill->m_nPetNum = nIndex;
		theSkill->m_bCanUse = TRUE;
		theSkill->m_nPosIndex = nSkillIndex;
	}*/
}

VOID CDataPool::OtExBox_SetSkill(INT nIndex, INT nSkillIndex, PET_SKILL* pPetSkillData, BOOL bClearOld)
{
	/*if(m_OtherExchangeBox.m_Petlist[nIndex]->m_aSkill[nSkillIndex] != NULL && bClearOld)
	{
		SAFE_DELETE(m_OtherExchangeBox.m_Petlist[nIndex]->m_aSkill[nSkillIndex]);
	}
	m_OtherExchangeBox.m_Petlist[nIndex]->m_aSkill[nSkillIndex] = pPetSkillData;*/
}


PET_SKILL*	CDataPool::OtExBox_GetSkill(INT nIndex,INT nSkillIndex)
{
	/*if( nIndex < 0 ||nIndex >= EXCHANGE_PET_BOX_SIZE )
		return NULL;
	else
		return m_OtherExchangeBox.m_Petlist[nIndex]->m_aSkill[nSkillIndex];*/
	return NULL;
}

VOID CDataPool::Pet_SetPetFakeModel(INT nIndex, INT nMountID)
{
	if (nIndex < 0 || nIndex >= HUMAN_PET_MAX_COUNT)
		return;

	if (NULL == m_listPetAvatar[nIndex].pAvatar)
		return;

	// UI模型没有变化
	if (m_listPetAvatar[nIndex].nMountID == nMountID)
	{
		// 更新特效
		m_listPetAvatar[nIndex].pAvatar->SetNpcMountEffect(nMountID, &m_listPet[nIndex]);
	}
	// 创建新的UI模型
	else
	{
		// 更新模型
		if (m_listPet[nIndex].m_pModelData)
		{
			m_listPetAvatar[nIndex].pAvatar->GetCharacterData()->Set_ModelID( m_listPet[nIndex].m_pModelData->m_nModelID );
		}		
		// 更新特效
		m_listPetAvatar[nIndex].pAvatar->SetNpcMountEffect(nMountID, &m_listPet[nIndex]);
		// 朝向
		m_listPetAvatar[nIndex].pAvatar->SetFaceDir(-1.0f);
	}
}

//  [7/15/2010 陈军龙]
VOID CDataPool::Mount_SetMountFakeModel(INT nIndex, INT nMountID)
{
	if (nIndex < 0 || nIndex >= MAX_MOUNT_SIZE)
		return;

	if (NULL == m_listMountAvatar[nIndex].pAvatar)
		return;

	// UI模型没有变化
	if (m_listMountAvatar[nIndex].nMountID == nMountID)
	{
		// 更新特效
		//m_listMountAvatar[nIndex].pAvatar->SetNpcMountEffect(nMountID, &m_listPet[nIndex]);
	}
	// 创建新的UI模型
	else
	{
		// 更新模型
		m_listMountAvatar[nIndex].pAvatar->GetCharacterData()->Set_ModelID( nMountID );		
		// 更新特效
		//m_listMountAvatar[nIndex].pAvatar->SetNpcMountEffect(nMountID, &m_listPet[nIndex]);
		// 朝向
		m_listMountAvatar[nIndex].pAvatar->SetFaceDir(-1.0f);
	}
}

const CHAR* CDataPool::Pet_GetPetFakeName(INT nIndex)
{
	if (nIndex >= 0 && nIndex < HUMAN_PET_MAX_COUNT)
	{
		return m_listPetAvatar[nIndex].strAvatarName.c_str();
	}	

	return NULL;
}

//  [7/15/2010 陈军龙]
const CHAR* CDataPool::Mount_GetMountFakeName(INT nIndex)
{
	if (nIndex >= 0 && nIndex < MAX_MOUNT_SIZE)
	{
		return m_listMountAvatar[nIndex].strAvatarName.c_str();
	}	

	return NULL;
}

//  [7/15/2010 陈军龙]
INT CDataPool::Mount_GetMountCount(VOID)
{
	INT count = 0;
	std::vector< tObject_Item* >::iterator it;
	for( it = m_vUserMount.begin(); it != m_vUserMount.end(); it ++)	
	{
		CObject_Item_Mount* itemMount = static_cast<CObject_Item_Mount*>(*it);

		if ((NULL != itemMount) &&(itemMount->GetItemClass() == ICLASS_MOUNT))
		{
			++count;
		}
	}
	return count;
}

VOID CDataPool::Pet_SetPet( SDATA_PET Pet )
{
	if ( Pet.m_pModelData == NULL )
	{
		return;
	}
	for ( INT i=0; i<HUMAN_PET_MAX_COUNT; ++i )
	{
		if ( m_listPet[i].m_GUID == Pet.m_GUID )
		{
			m_listPet[i] = Pet;
		}
	}
}


INT CDataPool::Pet_AddPet( SDATA_PET Pet )
{
	if ( Pet.m_pModelData == NULL )
	{
		return -1;
	}
	for ( INT i=0; i<HUMAN_PET_MAX_COUNT; ++i )
	{
		if ( m_listPet[i].m_pModelData == NULL )
		{
			m_listPet[i] = Pet;
			return i;
		}
	}
	return -1;
}



void CDataPool::TargetPet_SetPetModel(INT nRaceID)
{
//	m_pTargetPetAvatar->GetCharacterData()->Set_RaceID(nRaceID);
//	m_pTargetPetAvatar->SetFaceDir(0.0f);
}

VOID CDataPool::PetStudySkill_SetPetModel(INT nRaceID)
{
//	m_pPetStudySkillAvatar->GetCharacterData()->Set_RaceID(nRaceID);
//	m_pPetStudySkillAvatar->SetFaceDir(0.0f);
}

LPCTSTR		CDataPool::PlayerShop_GetShopNameByIndex(UINT uIndex)
{
	if(uIndex < MAX_SHOP_NUM_PER_SCENE) 
		return m_PlayerShopList[uIndex].m_szShopName.c_str(); 
	else
		return "";
}

VOID		CDataPool::PlayerShop_SetShopNameByIndex(UINT uIndex, LPCTSTR pszShopName)
{
	if(uIndex < MAX_SHOP_NUM_PER_SCENE) 
	{
		m_PlayerShopList[uIndex].m_szShopName = pszShopName;
	}
}

LPCTSTR		CDataPool::PlayerShop_GetShopDescByIndex(UINT uIndex)
{
	if(uIndex < MAX_SHOP_NUM_PER_SCENE) 
		return m_PlayerShopList[uIndex].m_szShopDesc.c_str(); 
	else
		return "";
}

VOID		CDataPool::PlayerShop_SetShopDescByIndex(UINT uIndex, LPCTSTR pszShopDesc)
{
	if(uIndex < MAX_SHOP_NUM_PER_SCENE) 
	{
		m_PlayerShopList[uIndex].m_szShopDesc = pszShopDesc;
	}
}

LPCTSTR		CDataPool::PlayerShop_GetShopFoundedTimeByIndex(UINT uIndex)
{
	if(uIndex < MAX_SHOP_NUM_PER_SCENE) 
		return m_PlayerShopList[uIndex].m_szFoundedTime.c_str(); 
	else
		return "";
}

VOID		CDataPool::PlayerShop_SetShopFoundedTimeByIndex(UINT uIndex, LPCTSTR pszTime)
{
	if(uIndex < MAX_SHOP_NUM_PER_SCENE) 
	{
		m_PlayerShopList[uIndex].m_szFoundedTime = pszTime;
	}
}

BYTE		CDataPool::PlayerShop_GetStallNumOpened(UINT uIndex)
{
	if(uIndex < MAX_SHOP_NUM_PER_SCENE) 
		return m_PlayerShopList[uIndex].m_StallNumOpened; 
	else
		return 0;
}
VOID		CDataPool::PlayerShop_SetStallNumOpened(UINT uIndex, BYTE nStallNumOpened)
{
	if(uIndex < MAX_SHOP_NUM_PER_SCENE) 
	{
		m_PlayerShopList[uIndex].m_StallNumOpened = nStallNumOpened;
	}
}

BYTE		CDataPool::PlayerShop_GetStallNumOnSale(UINT uIndex)
{
	if(uIndex < MAX_SHOP_NUM_PER_SCENE) 
		return m_PlayerShopList[uIndex].m_StallNumOnSale; 
	else
		return 0;
}
VOID CDataPool::PlayerShop_SetStallNumOnSale(UINT uIndex, BYTE nStallNumOnSale)
{
	if(uIndex < MAX_SHOP_NUM_PER_SCENE) 
	{
		m_PlayerShopList[uIndex].m_StallNumOnSale = nStallNumOnSale;
	}
}
INT CDataPool::PlayerShop_GetStallEnableByIndex(BOOL bIsMine, UINT nStallIndex)
{
	if(bIsMine)
	{
		if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return FALSE;
		return m_MyPlayerShop.m_StallList[nStallIndex].m_bIsEnable;
	}
	else
	{
		if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return FALSE;
		return m_OtPlayerShop.m_StallList[nStallIndex].m_bIsEnable;
	}
}
VOID CDataPool::PlayerShop_SetStallEnableByIndex(BOOL bIsMine, UINT nStallIndex,  BYTE bIsEnable)
{
	if(bIsMine)
	{
		if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return;
		m_MyPlayerShop.m_StallList[nStallIndex].m_bIsEnable = bIsEnable;
	}
	else
	{
		if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return;
		m_OtPlayerShop.m_StallList[nStallIndex].m_bIsEnable = bIsEnable;
	}
}

BYTE CDataPool::PlayerShop_GetTypeByIndex(UINT uIndex)
{
	if(uIndex < MAX_SHOP_NUM_PER_SCENE) 
		return m_PlayerShopList[uIndex].m_Type; 
	else
		return 0;
}
VOID CDataPool::PlayerShop_SetTypeByIndex(UINT uIndex, BYTE nType)
{
	if(uIndex < MAX_SHOP_NUM_PER_SCENE) 
	{
		m_PlayerShopList[uIndex].m_Type = nType;
	}
}

LPCTSTR	CDataPool::PlayerShop_GetOwnerNameByIndex(UINT uIndex)
{
	if(uIndex < MAX_SHOP_NUM_PER_SCENE) 
		return m_PlayerShopList[uIndex].m_szOwnerName.c_str(); 
	else
		return "";
}

VOID CDataPool::PlayerShop_SetOwnerNameByIndex(UINT uIndex, LPCTSTR pszOwnerName)
{
	if(uIndex < MAX_SHOP_NUM_PER_SCENE) 
	{
		m_PlayerShopList[uIndex].m_szOwnerName = pszOwnerName;
	}
}

UINT CDataPool::PlayerShop_GetOwnerGuidByIndex(UINT uIndex)
{
	if(uIndex < MAX_SHOP_NUM_PER_SCENE) 
		return m_PlayerShopList[uIndex].m_OwnerGuid; 
	else
		return 0;
}

VOID CDataPool::PlayerShop_SetOwnerGuidByIndex(UINT uIndex, UINT uOwnerGuid)
{
	if(uIndex < MAX_SHOP_NUM_PER_SCENE) 
	{
		m_PlayerShopList[uIndex].m_OwnerGuid = uOwnerGuid;
	}
}

_PLAYERSHOP_GUID	CDataPool::PlayerShop_GetIDByIndex(UINT uIndex)
{
	if(uIndex < MAX_SHOP_NUM_PER_SCENE) 
		return m_PlayerShopList[uIndex].m_ID; 
	else
	{
		_PLAYERSHOP_GUID emptyguid;
		return emptyguid;
	}
}

VOID	CDataPool::PlayerShop_SetIDByIndex(UINT uIndex, _PLAYERSHOP_GUID nId)
{
	if(uIndex < MAX_SHOP_NUM_PER_SCENE) 
	{
		m_PlayerShopList[uIndex].m_ID = nId;
	}
}

BYTE	CDataPool::PlayerShop_GetIsFavorByIndex(UINT uIndex)
{
	if(uIndex < MAX_SHOP_NUM_PER_SCENE) 
	{
		return m_PlayerShopList[uIndex].m_bIsFavor;
	}
	return 0;
}

VOID	CDataPool::PlayerShop_SetIsFavorByIndex(UINT uIndex, BYTE bIsFavor)
{
	if(uIndex < MAX_SHOP_NUM_PER_SCENE) 
	{
		m_PlayerShopList[uIndex].m_bIsFavor = bIsFavor;
	}
}

//设置摊位物品
VOID	CDataPool::PlayerShop_SetItem(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, tObject_Item* pItem, BOOL bClearOld)
{
	if(bIsMine)
	{//自己的
		if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return;
		if(	nBoxIndex < 0 || nBoxIndex >= STALL_BOX_SIZE) return;

		if( m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].pItem &&  bClearOld)
		{
			CObject_Item::DestroyItem((CObject_Item*)m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].pItem);
			m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].pItem		= NULL;
			m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].nPrice	= 0;
			m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].nSerial	= 0;
		}
		if(pItem)
		{
			pItem->SetPosIndex( nStallIndex*STALL_BOX_SIZE + nBoxIndex );
			pItem->SetTypeOwner(tObject_Item::IO_PS_SELFBOX);
		}
		m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].pItem = pItem;
	}
	else
	{
		if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return;
		if(	nBoxIndex < 0 || nBoxIndex >= STALL_BOX_SIZE) return;

		if( m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].pItem &&  bClearOld)
		{
			CObject_Item::DestroyItem((CObject_Item*)m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].pItem);
			m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].pItem		= NULL;
			m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].nPrice	= 0;
			m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].nSerial	= 0;
		}
		if(pItem)
		{
			pItem->SetPosIndex( nStallIndex*STALL_BOX_SIZE + nBoxIndex );
			pItem->SetTypeOwner(tObject_Item::IO_PS_OTHERBOX);
		}
		m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].pItem = pItem;
	}
}
//设置物品价格
VOID	CDataPool::PlayerShop_SetItemPrice(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, UINT nPrice)
{
	if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return;
	if(	nBoxIndex < 0 || nBoxIndex >= STALL_BOX_SIZE) return;

	if(bIsMine)
	{
		m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].nPrice = nPrice;
	}
	else
	{
		m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].nPrice = nPrice;
	}
}
//设置物品序列号
VOID	CDataPool::PlayerShop_SetItemSerial(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, UINT nSerial)
{
	if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return;
	if(	nBoxIndex < 0 || nBoxIndex >= STALL_BOX_SIZE) return;

	if(bIsMine)
	{
		m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].nSerial = nSerial;
	}
	else
	{
		m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].nSerial = nSerial;
	}
}

//此物体是否上架
VOID	CDataPool::PlayerShop_SetItemOnSale(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, BYTE bIsEnable)
{
	if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return;
	if(	nBoxIndex < 0 || nBoxIndex >= STALL_BOX_SIZE) return;

	if(bIsMine)
	{
		m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].nbIsEnable = bIsEnable;
	}
	else
	{
		m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].nbIsEnable = bIsEnable;
	}
}
//设置商店本金
VOID	CDataPool::PlayerShop_SetBaseMoney(BOOL bIsMine, UINT uBaseMoney)
{
	if(bIsMine)
	{
		m_MyPlayerShop.m_BaseMoney = uBaseMoney;
	}
	else
	{
		m_OtPlayerShop.m_BaseMoney = uBaseMoney;
	}
}
//设置商店盈利金
VOID	CDataPool::PlayerShop_SetProfitMoney(BOOL bIsMine, UINT uProfitPrice)
{
	if(bIsMine)
	{
		m_MyPlayerShop.m_ProfitMoney = uProfitPrice;
	}
	else
	{
		m_OtPlayerShop.m_ProfitMoney = uProfitPrice;
	}
}

//设置指定位置的合作伙伴
VOID	CDataPool::PlayerShop_SetPartnerByIndex(UINT uIndex, GUID_t Guid, LPCTSTR szName)
{
	if(uIndex<0||uIndex>=MAX_PARTNER_PER_SHOP)
		return;
	m_MyPlayerShop.m_PartnerList[uIndex].m_Guid		= Guid;
	m_MyPlayerShop.m_PartnerList[uIndex].m_szName	= szName;
}

//获得指定位置的合作伙伴
PlayerShopPartners_t* CDataPool::PlayerShop_GetPartnerByIndex(UINT uIndex)
{
	if(uIndex<0||uIndex>=MAX_PARTNER_PER_SHOP)
		return NULL;
	return &m_MyPlayerShop.m_PartnerList[uIndex];
}


//设置商店此商店是否是出售状态
VOID	CDataPool::PlayerShop_SetIsSaleOut(BOOL bIsMine, BOOL bIsSaleOut)
{
	if(bIsMine)
	{
		m_MyPlayerShop.m_bIsSaleOut = bIsSaleOut;
	}
	else
	{
		m_OtPlayerShop.m_bIsSaleOut = bIsSaleOut;
	}
}

VOID	CDataPool::PlayerShop_SetShopType(BOOL bIsMine, INT nShopType)//设置商店类型
{
	if(bIsMine)
	{
		m_MyPlayerShop.m_nShopType = nShopType;
	}
	else
	{
		m_OtPlayerShop.m_nShopType = nShopType;
	}
}
UINT	CDataPool::PlayerShop_GetShopType(BOOL bIsMine)//设置商店类型
{
	if(bIsMine)
	{
		return m_MyPlayerShop.m_nShopType;
	}
	else
	{
		return m_OtPlayerShop.m_nShopType;
	}
}

//设置商店此商店是否是出售状态
BOOL	CDataPool::PlayerShop_GetIsSaleOut(BOOL bIsMine)
{
	if(bIsMine)
	{
		return m_MyPlayerShop.m_bIsSaleOut;
	}
	else
	{
		return m_OtPlayerShop.m_bIsSaleOut;
	}
}

//设置商店此商店是否是出售状态
VOID	CDataPool::PlayerShop_SetSaleOutPrice(BOOL bIsMine, UINT uSaleOutPrice)
{
	if(bIsMine)
	{
		m_MyPlayerShop.m_uSaleOutPrice = uSaleOutPrice;
	}
	else
	{
		m_OtPlayerShop.m_uSaleOutPrice = uSaleOutPrice;
	}
}

//获得商店的盘出价
UINT	CDataPool::PlayerShop_GetSaleOutPrice(BOOL bIsMine)
{
	if(bIsMine)
	{
		return m_MyPlayerShop.m_uSaleOutPrice;
	}
	else
	{
		return m_OtPlayerShop.m_uSaleOutPrice;
	}
}

//设置店主guid
VOID	CDataPool::PlayerShop_SetOwnerGuid(BOOL bIsMine, UINT uOwnerGuid)
{
	if(bIsMine)
	{
		m_MyPlayerShop.m_OwnerGuid = uOwnerGuid;
	}
	else
	{
		m_OtPlayerShop.m_OwnerGuid = uOwnerGuid;
	}
}

//获得店主guid
UINT	CDataPool::PlayerShop_GetOwnerGuid(BOOL bIsMine)
{
	if(bIsMine)
	{
		return m_MyPlayerShop.m_OwnerGuid;
	}
	else
	{
		return m_OtPlayerShop.m_OwnerGuid;
	}
}

//设置交易记录数量
VOID	CDataPool::PlayerShop_SetExRecNum(BOOL bIsMine, UINT uExRecNum)
{
	if(bIsMine)
	{
		m_MyPlayerShop.m_ExRecListNum = uExRecNum;
	}
	else
	{
		m_OtPlayerShop.m_ExRecListNum = uExRecNum;
	}
}

//获得交易记录数量
UINT	CDataPool::PlayerShop_GetExRecNum(BOOL bIsMine)
{
	if(bIsMine)
	{
		return m_MyPlayerShop.m_ExRecListNum;
	}
	else
	{
		return m_OtPlayerShop.m_ExRecListNum;
	}
}

//设置管理记录数量
VOID	CDataPool::PlayerShop_SetMaRecNum(BOOL bIsMine, UINT uMaRecNum)
{
	if(bIsMine)
	{
		m_MyPlayerShop.m_MaRecListNum = uMaRecNum;
	}
	else
	{
		m_OtPlayerShop.m_MaRecListNum = uMaRecNum;
	}
}

//获得管理记录数量
UINT	CDataPool::PlayerShop_GetMaRecNum(BOOL bIsMine)
{
	if(bIsMine)
	{
		return m_MyPlayerShop.m_MaRecListNum;
	}
	else
	{
		return m_OtPlayerShop.m_MaRecListNum;
	}
}

//设置已经拥有的柜台数
VOID CDataPool::PlayerShop_SetOpenStallNum(BOOL bIsMine, UINT uOpenStallNum)
{
	if(bIsMine)
	{
		m_MyPlayerShop.m_StallOpenNum = uOpenStallNum;
	}
	else
	{
		m_OtPlayerShop.m_StallOpenNum = uOpenStallNum;
	}
}

//获得已经拥有的柜台数
UINT CDataPool::PlayerShop_GetOpenStalldNum(BOOL bIsMine)
{
	if(bIsMine)
	{
		return m_MyPlayerShop.m_StallOpenNum;
	}
	else
	{
		return m_OtPlayerShop.m_StallOpenNum;
	}
}

//设置商店的类型
VOID CDataPool::PlayerShop_SetShopSubType(BOOL bIsMine, UINT uShopSubType)
{
	if(bIsMine)
	{
		m_MyPlayerShop.m_ShopSubType = uShopSubType;
	}
	else
	{
		m_OtPlayerShop.m_ShopSubType = uShopSubType;
	}
}

//获得商店的类型
UINT CDataPool::PlayerShop_GetShopSubType(BOOL bIsMine)
{
	if(bIsMine)
	{
		return m_MyPlayerShop.m_ShopSubType;
	}
	else
	{
		return m_OtPlayerShop.m_ShopSubType;
	}
}

//设置店主名
VOID	CDataPool::PlayerShop_SetOwnerName(BOOL bIsMine, LPCTSTR pszOwnerName)
{
	if(bIsMine)
	{
		m_MyPlayerShop.m_szOwnerName = pszOwnerName;
	}
	else
	{
		m_OtPlayerShop.m_szOwnerName = pszOwnerName;
	}
}

//获得店主名
LPCTSTR	CDataPool::PlayerShop_GetOwnerName(BOOL bIsMine)
{
	if(bIsMine)
	{
		return m_MyPlayerShop.m_szOwnerName.c_str();
	}
	else
	{
		return m_OtPlayerShop.m_szOwnerName.c_str();
	}
}

//设置商店名
VOID	CDataPool::PlayerShop_SetShopName(BOOL bIsMine, LPCTSTR pszShopName)
{
	if(bIsMine)
	{
		m_MyPlayerShop.m_szShopName = pszShopName;
	}
	else
	{
		m_OtPlayerShop.m_szShopName = pszShopName;
	}
}

//获得商店名
LPCTSTR	CDataPool::PlayerShop_GetShopName(BOOL bIsMine)
{
	if(bIsMine)
	{
		return m_MyPlayerShop.m_szShopName.c_str();
	}
	else
	{
		return m_OtPlayerShop.m_szShopName.c_str();
	}
}

//设置商店描述
VOID	CDataPool::PlayerShop_SetShopDesc(BOOL bIsMine, LPCTSTR pszShopDesc)
{
	if(bIsMine)
	{
		m_MyPlayerShop.m_szShopDesc = pszShopDesc;
	}
	else
	{
		m_OtPlayerShop.m_szShopDesc = pszShopDesc;
	}
}

//获得商店名
LPCTSTR	CDataPool::PlayerShop_GetShopDesc(BOOL bIsMine)
{
	if(bIsMine)
	{
		return m_MyPlayerShop.m_szShopDesc.c_str();
	}
	else
	{
		return m_OtPlayerShop.m_szShopDesc.c_str();
	}
}


//获得制定位置的物品
tObject_Item*	CDataPool::PlayerShop_GetItem(BOOL bIsMine, INT nStallIndex, INT nBoxIndex)
{
	if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return NULL;
	if(	nBoxIndex < 0 || nBoxIndex >= STALL_BOX_SIZE) return NULL;

	if(bIsMine)
	{
		return m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].pItem;
	}
	else
	{
		return m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].pItem;
	}
}
//获得物品价格
UINT	CDataPool::PlayerShop_GetItemPrice(BOOL bIsMine, INT nStallIndex, INT nBoxIndex)
{
	if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return 0;
	if(	nBoxIndex < 0 || nBoxIndex >= STALL_BOX_SIZE) return 0;

	if(bIsMine)
	{
		return m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].nPrice;
	}
	else
	{
		return m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].nPrice;
	}
}

//获得物品序列号
UINT	CDataPool::PlayerShop_GetItemSerial(BOOL bIsMine, INT nStallIndex, INT nBoxIndex)
{
	if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return 0;
	if(	nBoxIndex < 0 || nBoxIndex >= STALL_BOX_SIZE) return 0;

	if(bIsMine)
	{
		return m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].nSerial;
	}
	else
	{
		return m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].nSerial;
	}
}

//此物体是否上架
BYTE	CDataPool::PlayerShop_GetItemOnSale(BOOL bIsMine, INT nStallIndex, INT nBoxIndex)
{
	if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return 0;
	if(	nBoxIndex < 0 || nBoxIndex >= STALL_BOX_SIZE) return 0;

	if(bIsMine)
	{
		return m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].nbIsEnable;
	}
	else
	{
		return m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].nbIsEnable;
	}
}
//设置商店本金
UINT	CDataPool::PlayerShop_GetBaseMoney(BOOL bIsMine)
{
	if(bIsMine)
	{
		return m_MyPlayerShop.m_BaseMoney;
	}
	else
	{
		return m_OtPlayerShop.m_BaseMoney;
	}
}
//设置商店盈利金
UINT	CDataPool::PlayerShop_GetProfitMoney(BOOL bIsMine)
{
	if(bIsMine)
	{
		return m_MyPlayerShop.m_ProfitMoney;
	}
	else
	{
		return m_OtPlayerShop.m_ProfitMoney;
	}
}

//设置物品详细信息
VOID	CDataPool::PlayerShop_SetItemExtraInfo(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, BOOL bEmpty, const _ITEM* pItem)
{
	if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return;
	if(	nBoxIndex < 0 || nBoxIndex >= STALL_BOX_SIZE) return;

	if(bIsMine)
	{
		if(bEmpty)
		{
			if(m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].pItem)
			{
				CObject_Item::DestroyItem((CObject_Item*)m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].pItem);
				m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].pItem	= NULL;
				m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].nPrice	= 0;
				m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].nSerial	= 0;
			}
		}
		else
		{
			if(m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].pItem)
				m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].pItem->SetExtraInfo(pItem);
		}
	}
	else
	{
		if(bEmpty)
		{
			if(m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].pItem)
			{
				CObject_Item::DestroyItem((CObject_Item*)m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].pItem);
				m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].pItem	= NULL;
				m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].nPrice	= 0;
				m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].nSerial	= 0;
			}
		}
		else
		{
			if(m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].pItem)
				m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList[nBoxIndex].pItem->SetExtraInfo(pItem);
		}
	}
}
//获得摊位中的物品数量
INT		CDataPool::PlayerShop_GetItemNum(BOOL bIsMine, INT nStallIndex)
{
	if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return 0;

	if(bIsMine)
	{
		return (INT)m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList.size();
	}
	else
	{
		return (INT)m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList.size();
	}
}
//根据GUID查找物品位置
INT	CDataPool::PlayerShop_GetItemIndexByGUID(BOOL bIsMine, INT nStallIndex, __int64 ItemGUID)
{
	if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return 0;

	if(bIsMine)
	{
		INT index;
		for(index = 0; index<STALL_BOX_SIZE; index++)
		{
			if(m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList[index].pItem == NULL)
				continue;
			if(m_MyPlayerShop.m_StallList[nStallIndex].m_ItemList[index].pItem->GetGUID() == ItemGUID)
				break;
		}
		if(index == STALL_BOX_SIZE)
		{
			return -1;
		}
		return index;

	}
	else
	{
		INT index;
		for(index = 0; index<STALL_BOX_SIZE; index++)
		{
			if(m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList[index].pItem == NULL)
				continue;
			if(m_OtPlayerShop.m_StallList[nStallIndex].m_ItemList[index].pItem->GetGUID() == ItemGUID)
				break;
		}
		if(index == STALL_BOX_SIZE)
		{
			return -1;
		}
		return index;
	}
}

SDATA_PET*	CDataPool::PlayerShop_GetPet(BOOL bIsMine, INT nStallIndex, INT nIndex)
{
	/*if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return NULL;

	if(bIsMine)
	{
		if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE) 
		{
			return m_MyPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].pPet;
		}
		else
			return NULL;
	}
	else
	{
		if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE) 
		{
			return m_OtPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].pPet;
		}
		else
			return NULL;
	}*/
	return NULL;
}

//根据GUID在玩家商店指定柜台中查找物品
INT	CDataPool::PlayerShop_GetPetByGuid(BOOL bIsMine, INT nStallIndex, PET_GUID_t PetGuid)
{
	/*if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return -1;
	
	if(bIsMine)
	{
		for(UINT i = 0; i< STALL_PET_BOX_SIZE; i++)
		{
			if(m_MyPlayerShop.m_StallList[nStallIndex].m_PetList[i].pPet == NULL)
			{
				continue;
			}
			if(m_MyPlayerShop.m_StallList[nStallIndex].m_PetList[i].pPet->m_GUID == PetGuid)
			{
				return i;
			}
		}
		return	-1;
	}
	else
	{
		for(UINT i = 0; i< STALL_PET_BOX_SIZE; i++)
		{
			if(m_OtPlayerShop.m_StallList[nStallIndex].m_PetList[i].pPet == NULL)
			{
				continue;
			}
			if(m_OtPlayerShop.m_StallList[nStallIndex].m_PetList[i].pPet->m_GUID == PetGuid)
			{
				return i;
			}
		}
		return	-1;
	}*/
	return	-1;
}

//在指定柜台中设置骑乘宠物
VOID CDataPool::PlayerShop_SetPet(BOOL bIsMine, INT nStallIndex, INT nIndex, SDATA_PET* pPetData, BOOL bClearOld)
{
	/*if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return;

	if(bIsMine)
	{
		if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE)
		{
			if(m_MyPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].pPet != NULL && bClearOld)
			{
				SAFE_DELETE(m_MyPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].pPet);
			}
			m_MyPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].pPet = pPetData; 
		}
	}
	else
	{
		if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE)
		{
			if(m_OtPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].pPet != NULL && bClearOld)
			{
				SAFE_DELETE(m_OtPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].pPet);
			}
			m_OtPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].pPet = pPetData; 
		}
	}*/
}

//设置指定柜台的格子序列号
VOID CDataPool::PlayerShop_SetPetSerial(BOOL bIsMine, INT nStallIndex, INT nIndex, INT nSerial)
{
	/*if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return;

	if(bIsMine)
	{
		if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE)
		{
			m_MyPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].nSerial = nSerial;
		}
	}
	else
	{
		if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE)
		{
			m_OtPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].nSerial = nSerial;
		}
	}*/
}

//获得指定柜台的格子序列号
INT	CDataPool::PlayerShop_GetPetSerial(BOOL bIsMine, INT nStallIndex, INT nIndex)
{
	/*if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return -1;

	if(bIsMine)
	{
		if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE)
		{
			return m_MyPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].nSerial;
		}
	}
	else
	{
		if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE)
		{
			return m_OtPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].nSerial;
		}
	}*/
	return -1;
}


VOID CDataPool::PlayerShop_SetPetPrice(BOOL bIsMine, INT nStallIndex, INT nIndex, INT nPetPrice)
{
	/*if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return;

	if(bIsMine)
	{
		if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE)
		{
			m_MyPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].nPrice = nPetPrice;
		}
	}
	else
	{
		if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE)
		{
			m_OtPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].nPrice = nPetPrice;
		}
	}*/
}
INT	CDataPool::PlayerShop_GetPetPrice(BOOL bIsMine, INT nStallIndex, INT nIndex)
{
	/*if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return 0;
	if(bIsMine)
	{
		if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE)
		{
			return m_MyPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].nPrice;
		}
	}
	else
	{
		if(nIndex >=0 && nIndex < STALL_PET_BOX_SIZE)
		{
			return m_OtPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].nPrice;
		}
	}*/
	return 0;
}

PET_SKILL* CDataPool::PlayerShop_GetSkill(BOOL bIsMine, INT nStallIndex, INT nIndex,INT nSkillIndex)
{
	/*if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return NULL;

	if(bIsMine)
	{
		return m_MyPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].pPet->m_aSkill[nSkillIndex];
	}
	else
	{
		return m_OtPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].pPet->m_aSkill[nSkillIndex];
	}*/
	return NULL;
}

VOID CDataPool::PlayerShop_SetSkill(BOOL bIsMine, INT nStallIndex, INT nIndex, INT nSkillIndex, PET_SKILL* pPetSkillData, BOOL bClearOld)
{
	/*if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return;
	
	if(bIsMine)
	{
		if(m_MyPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].pPet->m_aSkill[nSkillIndex] != NULL && bClearOld)
		{
			SAFE_DELETE(m_MyPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].pPet->m_aSkill[nSkillIndex]);
		}
		m_MyPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].pPet->m_aSkill[nSkillIndex] = pPetSkillData;
	}
	else
	{
		if(m_OtPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].pPet->m_aSkill[nSkillIndex] != NULL && bClearOld)
		{
			SAFE_DELETE(m_OtPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].pPet->m_aSkill[nSkillIndex]);
		}
		m_OtPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].pPet->m_aSkill[nSkillIndex] = pPetSkillData;
	}*/
}

VOID CDataPool::PlayerShop_SetSkill(BOOL bIsMine, INT nStallIndex, INT nIndex,INT nSkillIndex, const _OWN_SKILL *Skill)
{
	/*if(	nStallIndex < 0 || nStallIndex >= MAX_STALL_NUM_PER_SHOP) return;

	if( nIndex < 0 ||nIndex >= STALL_PET_BOX_SIZE )
		return;

	if(bIsMine)
	{
		if(m_MyPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].pPet == NULL)
			return;

		PET_SKILL* theSkill = m_MyPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].pPet->m_aSkill[nSkillIndex];

		DBC_DEFINEHANDLE(s_pSkillDBC, DBC_SKILL_DATA);
		const _DBC_SKILL_DATA* pDefine = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_Index_EQU(Skill->m_nSkillID);

		if(!theSkill)
		{
			PET_SKILL* newSkill = new PET_SKILL;

			newSkill->m_pDefine = pDefine;
			newSkill->m_nPetNum = nIndex;
			newSkill->m_nPosIndex = nSkillIndex;
			newSkill->m_bCanUse = TRUE;

			m_MyPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].pPet->m_aSkill[nSkillIndex] = newSkill;
		}
		else
		{
			theSkill->m_pDefine = pDefine;
			theSkill->m_nPetNum = nIndex;
			theSkill->m_bCanUse = TRUE;
			theSkill->m_nPosIndex = nSkillIndex;
		}
	}
	else
	{
		if(m_OtPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].pPet == NULL)
			return;

		PET_SKILL* theSkill = m_OtPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].pPet->m_aSkill[nSkillIndex];

		DBC_DEFINEHANDLE(s_pSkillDBC, DBC_SKILL_DATA);
		const _DBC_SKILL_DATA* pDefine = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_Index_EQU(Skill->m_nSkillID);

		if(!theSkill)
		{
			PET_SKILL* newSkill = new PET_SKILL;

			newSkill->m_pDefine = pDefine;
			newSkill->m_nPetNum = nIndex;
			newSkill->m_nPosIndex = nSkillIndex;
			newSkill->m_bCanUse = TRUE;

			m_OtPlayerShop.m_StallList[nStallIndex].m_PetList[nIndex].pPet->m_aSkill[nSkillIndex] = newSkill;
		}
		else
		{
			theSkill->m_pDefine = pDefine;
			theSkill->m_nPetNum = nIndex;
			theSkill->m_bCanUse = TRUE;
			theSkill->m_nPosIndex = nSkillIndex;
		}
	}*/
}

//清除商店中的物品
VOID CDataPool::PlayerShop_CleanObjItem(BOOL bIsMine)
{
	/*if(bIsMine)
	{
		for(int i=0; i<MAX_STALL_NUM_PER_SHOP; i++)
		{
			for(INT j=0; j<STALL_BOX_SIZE; j++)
			{
				if(m_MyPlayerShop.m_StallList[i].m_ItemList[j].pItem == NULL)
					continue;
				CObject_Item::DestroyItem((CObject_Item*)m_MyPlayerShop.m_StallList[i].m_ItemList[j].pItem);

				if(m_MyPlayerShop.m_StallList[i].m_PetList[j].pPet == NULL)
					continue;
				SAFE_DELETE(m_MyPlayerShop.m_StallList[i].m_PetList[j].pPet);
			}
		}
	}
	else
	{
		for(int i=0; i<MAX_STALL_NUM_PER_SHOP; i++)
		{
			for(INT j=0; j<STALL_BOX_SIZE; j++)
			{
				if(m_OtPlayerShop.m_StallList[i].m_ItemList[j].pItem == NULL)
					continue;
				CObject_Item::DestroyItem((CObject_Item*)m_OtPlayerShop.m_StallList[i].m_ItemList[j].pItem);

				if(m_OtPlayerShop.m_StallList[i].m_PetList[j].pPet == NULL)
					continue;
				SAFE_DELETE(m_OtPlayerShop.m_StallList[i].m_PetList[j].pPet);

			}
		}
	}*/
}

//---------------------------------------------
//骑乘宠物公告板
//---------------------------------------------
INT  CDataPool::PetPlacard_GetItemCount(VOID)const
{
	return m_nPetPlacardItemCount;
}

const _PET_PLACARD_ITEM * CDataPool::PetPlacard_GetItem(INT nIndex)const
{
	if(nIndex >= 0 && nIndex < m_nPetPlacardItemCount)
	{
		return &m_aPetPlacardItems[nIndex];
	}
	else
	{
		return NULL;
	}
}

VOID  CDataPool::PetPlacard_AddItem(const _PET_PLACARD_ITEM *pItem)
{
	if(m_nPetPlacardItemCount < MAX_PETPLACARD_LIST_ITEM_NUM)
	{
		m_aPetPlacardItems[m_nPetPlacardItemCount++] = *pItem;
	}
}

VOID CDataPool::PetPlacard_CleanUp()
{
	for(INT i=0; i < m_nPetPlacardItemCount; ++i)
	{
		m_aPetPlacardItems[i].CleanUp();
	}
	m_nPetPlacardItemCount = 0;
}

//设置物品序列号
VOID CDataPool::PlayerShop_SetShopID(BOOL bIsMine, _PLAYERSHOP_GUID nShopID)
{
	if(bIsMine)
	{
		m_MyPlayerShop.m_ShopID	= nShopID;
	}
	else
	{
		m_OtPlayerShop.m_ShopID	= nShopID;
	}
}
//获得物品序列号
_PLAYERSHOP_GUID	CDataPool::PlayerShop_GetShopID(BOOL bIsMine)
{
	if(bIsMine)
	{
		return m_MyPlayerShop.m_ShopID;
	}
	else
	{
		return m_OtPlayerShop.m_ShopID;
	}
}

//设置商店序列号
VOID	CDataPool::PlayerShop_SetShopSerial(BOOL bIsMine, BYTE nShopSerial)
{
	if(bIsMine)
	{
		m_MyPlayerShop.m_Serial	= nShopSerial;
	}
	else
	{
		m_OtPlayerShop.m_Serial	= nShopSerial;
	}
}

//获得商店序列号
BYTE	CDataPool::PlayerShop_GetShopSerial(BOOL bIsMine)
{
	if(bIsMine)
	{
		return m_MyPlayerShop.m_Serial;
	}
	else
	{
		return m_OtPlayerShop.m_Serial;
	}
}

//清空玩家商店
VOID CDataPool::PlayerShop_CleanUp(BOOL bIsMine)
{
	if(bIsMine)
	{
		m_MyPlayerShop.CleanUp();
	}
	else
	{
		m_OtPlayerShop.CleanUp();
	}
}


VOID CDataPool::X_PARAM_Set(X_PARAM *pBuf,INT nUIIndex)
{
	X_PARAM* pXParam_DataPool = CDataPool::GetMe()->X_PARAM_Get();
	X_PARAM* pXParam_Rcv = (X_PARAM*)(pBuf);

	memcpy(pXParam_DataPool,pXParam_Rcv,sizeof(X_PARAM));

	CEventSystem::GetMe()->PushEvent(GE_UI_COMMAND, nUIIndex);

}


//---------------------------------------------------------------------------------------------------
void CDataPool::Relation_ClearFriend()
{
	m_uFriendCount = 0;
	for( INT i=0; i<RELATION_BLACKNAME_OFFSET - RELATION_FRIEND_OFFSET; i++ )
	{
		m_FriendInfo[i].CleanUp();
	}
}

VOID CDataPool::Relation_ClearBlack()
{
	m_uBlackCount = 0;
	for( INT i=0; i<MAX_RELATION_SIZE - RELATION_BLACKNAME_OFFSET - 40; i++ )
	{
		m_BlackNameInfo[i].CleanUp();
	}
}

VOID CDataPool::Relation_ClearEnemy()
{
	m_uEnemyCount = 0;
	for( INT i=0; i<40; i++ )
	{
		m_EnemyInfo[i].CleanUp();
	}
}


//---------------------------------------------------------------------------------------------------
VOID CDataPool::Relation_AddFriend(INT index, _OWN_RELATION* pRelation)
{
	Assert( pRelation );
	Assert( m_uFriendCount < RELATION_BLACKNAME_OFFSET - RELATION_FRIEND_OFFSET );

	m_FriendInfo[m_uFriendCount].m_GUID = pRelation->m_Member.m_MemberGUID;
	strncpy(m_FriendInfo[m_uFriendCount].m_szTargetName, pRelation->m_Member.m_szMemberName, sizeof(m_FriendInfo[m_uFriendCount].m_szTargetName) - 1);
	m_FriendInfo[m_uFriendCount].m_uNameSize = (UCHAR)strlen(pRelation->m_Member.m_szMemberName);
	m_FriendInfo[m_uFriendCount].m_uGroup = pRelation->m_Group;
	m_FriendInfo[m_uFriendCount].m_nFriendpoint = pRelation->m_FriendPoint;
}

VOID CDataPool::Relation_AddBlack(INT index, _OWN_RELATION* pRelation)
{
	Assert( pRelation );
	Assert( m_uBlackCount < MAX_RELATION_SIZE - RELATION_BLACKNAME_OFFSET - 40 );

	m_BlackNameInfo[m_uBlackCount].m_GUID = pRelation->m_Member.m_MemberGUID;
	strncpy(m_BlackNameInfo[m_uBlackCount].m_szTargetName, pRelation->m_Member.m_szMemberName, sizeof(m_BlackNameInfo[m_uBlackCount].m_szTargetName) - 1);
	m_BlackNameInfo[m_uBlackCount].m_uNameSize = (UCHAR)strlen(pRelation->m_Member.m_szMemberName);
}

VOID CDataPool::Relation_AddEnemy(INT index, _OWN_RELATION* pRelation)
{
	Assert( pRelation );
	Assert( m_uEnemyCount < 40 );

	m_EnemyInfo[m_uEnemyCount].m_GUID = pRelation->m_Member.m_MemberGUID;
	strncpy( m_EnemyInfo[m_uEnemyCount].m_szEnemyName, pRelation->m_Member.m_szMemberName, sizeof(m_EnemyInfo[m_uEnemyCount].m_szEnemyName) - 1 );
	m_EnemyInfo[m_uEnemyCount].m_uNameSize = (UCHAR)strlen( pRelation->m_Member.m_szMemberName );

}


VOID CDataPool::Relation_DelFriend()
{
}
VOID CDataPool::Relation_DelBlack()
{
}
VOID CDataPool::Relation_DelEnemy()
{
}

//---------------------------------------------------------------------------------------------------
BOOL CDataPool::Relation_GetFriend(INT index, _OWN_RELATION* pRelation)
{
	Assert(pRelation);
	Assert((UINT)index<m_uFriendCount);
	
	pRelation->m_Member.m_MemberGUID = m_FriendInfo[index].m_GUID;
	strncpy(pRelation->m_Member.m_szMemberName, m_FriendInfo[index].m_szTargetName,sizeof(m_FriendInfo[index].m_szTargetName) - 1);
	pRelation->m_Group = m_FriendInfo[index].m_uGroup;
	pRelation->m_FriendPoint = m_FriendInfo[index].m_nFriendpoint;

	return TRUE;
}
BOOL CDataPool::Relation_GetBlack(INT index, _OWN_RELATION* pRelation)
{
	Assert(pRelation);
	Assert((UINT)index<m_uBlackCount);

	pRelation->m_Member.m_MemberGUID = m_BlackNameInfo[index].m_GUID;
	strncpy(pRelation->m_Member.m_szMemberName, m_BlackNameInfo[index].m_szTargetName, sizeof(m_BlackNameInfo[index].m_szTargetName) - 1);

	return TRUE;
}
BOOL CDataPool::Relation_GetEnemy(INT index, _OWN_RELATION* pRelation)
{
	Assert(pRelation);
	Assert((UINT)index < m_uEnemyCount);

	pRelation->m_Member.m_MemberGUID = m_EnemyInfo[index].m_GUID;
	strncpy( pRelation->m_Member.m_szMemberName, m_EnemyInfo[index].m_szEnemyName, sizeof(m_EnemyInfo[index].m_szEnemyName) - 1);

	return TRUE;
}

BOOL CDataPool::Relation_GetFriendByName( LPCTSTR name, _OWN_RELATION* pRelation )
{
	Assert( pRelation );
	Assert( name );

	for( INT i = 0; i < (INT)m_uFriendCount; ++i )
	{
		if( 0 == strcmp( m_FriendInfo[i].m_szTargetName, name ) )
		{
			pRelation->m_Member.m_MemberGUID = m_FriendInfo[i].m_GUID;
			strncpy( pRelation->m_Member.m_szMemberName, m_FriendInfo[i].m_szTargetName, sizeof(m_FriendInfo[i].m_szTargetName) - 1 );
			pRelation->m_Group = m_FriendInfo[i].m_uGroup;
			pRelation->m_FriendPoint = m_FriendInfo[i].m_nFriendpoint;

			break;
		}
	}

	return TRUE;
}
BOOL CDataPool::Relation_GetBlackByName( LPCTSTR name, _OWN_RELATION* pRelation )
{
	Assert( pRelation );
	Assert( name );

	for( INT i = 0; i < (INT)m_uBlackCount; ++i )
	{
		if( 0 == strcmp( m_BlackNameInfo[i].m_szTargetName, name ) )
		{
			pRelation->m_Member.m_MemberGUID = m_BlackNameInfo[i].m_GUID;
			strncpy( pRelation->m_Member.m_szMemberName, m_BlackNameInfo[i].m_szTargetName, sizeof(m_BlackNameInfo[i].m_szTargetName) - 1 );
			break;
		}
	}

	return TRUE;
}
BOOL CDataPool::Relation_GetEnemyByName( LPCTSTR name, _OWN_RELATION* pRelation )
{
	Assert( pRelation );
	Assert( name );

	for( INT i = 0; i < (INT)m_uEnemyCount; ++i )
	{
		if( 0 == strcmp( m_EnemyInfo[i].m_szEnemyName, name ) )
		{
			pRelation->m_Member.m_MemberGUID = m_EnemyInfo[i].m_GUID;
			strncpy( pRelation->m_Member.m_szMemberName, m_EnemyInfo[i].m_szEnemyName, sizeof(m_EnemyInfo[i].m_szEnemyName) - 1 );
			break;
		}
	}

	return TRUE;
}


//添加新组
VOID CDataPool::Relation_SetNewGroup( INT index, LPCTSTR name )
{
	if( index >= 0 && index < 5 )
		m_NewGroup[index] = name;
}
// 获取新组
LPCTSTR CDataPool::Relation_GetNewGroup( INT index )
{
	if( index >= 0 && index < 5 )
		return m_NewGroup[index].c_str();

	return "";
}



//---------------------------------------------------------------------------------------------------

VOID CDataPool::PetSkillStudy_MenPaiList_AddSkill(INT skillId)
{
	/*PET_SKILL sk;

	sk.m_bCanUse = FALSE;
	sk.m_nPetNum = MENPAI_PETSKILLSTUDY_PETNUM;
	sk.m_nPosIndex = (INT)m_petSkillStudyList.size();

	DBC_DEFINEHANDLE(s_pSkillDBC, DBC_SKILL_DATA);
	const _DBC_SKILL_DATA* pDefine = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_Index_EQU(skillId);

	if(pDefine)
	{
		sk.m_pDefine = pDefine;
		m_petSkillStudyList.push_back(sk);
	}*/
}

VOID CDataPool::PetSkillStudy_MenPaiList_Clear()
{
	m_petSkillStudyList.resize(0);
}

PET_SKILL* CDataPool::PetSkillStudy_MenPaiList_Get(INT idx)
{
	if(!m_petSkillStudyList.size())
		return NULL;

	if(idx < 0 || idx >= (INT)m_petSkillStudyList.size())
		return NULL;
	else
		return &(m_petSkillStudyList.at(idx));
}

PET_SKILL* CDataPool::PetSkillStudy_MenPaiList_Get_BySkillId(INT sdx)
{
	if(!m_petSkillStudyList.size() || sdx < 0)
		return NULL;

	for(INT i=0; i < (INT)m_petSkillStudyList.size(); ++i)
	{
		if(m_petSkillStudyList.at(i).m_pDefine->m_nID == sdx)
			return &(m_petSkillStudyList.at(i));
	}

	return NULL;
}

//通过索引获得工会信息
GuildInfo_t*	CDataPool::Guild_GetInfoByIndex(INT nIndex)
{
	if( nIndex>=0 && nIndex<MAX_GUILD_SIZE )
	{
		return &m_GuildList[nIndex];
	}
	return NULL;
}

//通过索引设置工会信息
VOID	CDataPool::Guild_SetInfoByIndex(INT nIndex, GuildInfo_t* pGuildInfo)
{
	if( nIndex>=0 && nIndex<MAX_GUILD_SIZE )
	{
		m_GuildList[nIndex] = *pGuildInfo;
	}
}

//清空所有工会信息
VOID	CDataPool::Guild_ClearAllInfo()
{
	if(m_GuildNum == 0)
		return;

	for(INT i = 0; i<m_GuildNum; i++)
	{
		m_GuildList[i].CleanUp(); 
	}
	m_GuildNum = 0;
	return;
}

//清空所有帮众信息
VOID CDataPool::Guild_ClearMemInfo()
{
	if( m_GuildMemNum == 0 )
		return;

	for(INT i = 0; i<m_GuildMemNum; i++)
	{
		m_GuildMemList[i].CleanUp(); 
	}

	m_GuildMemNum		= 0;
	m_GuildMaxMemNum	= 0;
	m_GuildDesc = "";
	m_GuildName = "";
	m_uAccess	=	0;
	m_uPosition	=	0;

	return;
}

//通过索引获得帮众信息
GuildMemberInfo_t*	CDataPool::Guild_GetMemInfoByIndex(INT nIndex)
{
	if( nIndex>=0 && nIndex<USER_ARRAY_SIZE )
	{
		return &m_GuildMemList[nIndex];
	}
	return NULL;
}

//通过索引设置帮众信息
VOID	CDataPool::Guild_SetMemInfoByIndex(INT nIndex, GuildMemberInfo_t* pMemberInfo)
{
	if( nIndex>=0 && nIndex<USER_ARRAY_SIZE )
	{
		m_GuildMemList[nIndex] = *pMemberInfo;
	}
}

//清空帮派的详细信息
VOID	CDataPool::Guild_ClearDetailInfo()
{
	m_GuildDetailInfo.CleanUp();
}

//获得帮派的详细信息
GuildDetailInfo_t*	CDataPool::Guild_GetDetailInfo()
{
	return &m_GuildDetailInfo;
}

//设置帮派的详细信息
VOID	CDataPool::Guild_SetDetailInfo(GuildDetailInfo_t* pDetailInfo)
{
	m_GuildDetailInfo	=	*pDetailInfo;
}

//清空帮派可任命职位
VOID	CDataPool::Guild_ClearAppointInfo()
{
	for(INT i = 0; i<GUILD_POSITION_SIZE; i++)
	{
		m_GuildPosAvail[i].CleanUp();
	}
}

//获得帮派可任命职位
GuildAppointPos_t*	CDataPool::Guild_GetAppointInfoByIndex(INT nIndex)
{
	if(nIndex>=0 && nIndex<GUILD_POSITION_SIZE)
	{
		return &m_GuildPosAvail[nIndex];
	}
	return NULL;
}

//设置帮派可任命职位
VOID	CDataPool::Guild_SetAppointInfoByIndex( INT nIndex, GuildAppointPos_t* pAppointInfo)
{
	if(nIndex>=0 && nIndex<GUILD_POSITION_SIZE)
	{
		m_GuildPosAvail[nIndex] = *pAppointInfo;
	}

}


VOID CDataPool::Guild_SetInviteGuildId( INT nGuildId )
{
	m_GuildInviteId = nGuildId;
}
VOID CDataPool::Guild_SetInviteGuildName( LPCTSTR pGuildName )
{
	m_GuildInviteName = pGuildName;
}
VOID CDataPool::Guild_SetInviteGuildObjName( LPCTSTR pGuildObjName )
{
	m_GuildInviteObjName = pGuildObjName;
}

 
INT CDataPool::Guild_GetInviteGuildId( VOID )
{
	return m_GuildInviteId;
}
LPCTSTR CDataPool::Guild_GetInviteGuildName( VOID )
{
	return m_GuildInviteName.c_str();
}
LPCTSTR CDataPool::Guild_GetInviteGuildObjName( VOID )
{
	return m_GuildInviteObjName.c_str();
}


VOID CDataPool::Guild_ClearFamilyInfo()
{
	if( m_iGuildFamilyNum == 0 )
		return;

	for(INT i = 0; i<m_iGuildFamilyNum; i++)
	{
		m_FamilyList[i].CleanUp(); 
	}

	m_iGuildFamilyNum = 0;
	return;
}

VOID CDataPool::Guild_SetFamilyMemInfoByIndex(INT nIndex, GuildFamilyInfo* pFamilyInfo)
{
	if( nIndex>=0 && nIndex<GUILD_FAMILY_SIZE )
	{
		m_FamilyList[nIndex] = *pFamilyInfo;
	}
}

GuildFamilyInfo* CDataPool::Guild_GetFamilyInfoByIndex(INT nIndex)
{
	if( nIndex>=0 && nIndex<GUILD_FAMILY_SIZE )
	{
		return &m_FamilyList[nIndex];
	}

	return NULL;
}


// 界面的家族数据
VOID CDataPool::Guild_AddFamilyWindowData( INT index, STRING winName )
{
	m_mFamilyWinData;
}
VOID CDataPool::Guild_DelFamilyWindowData( INT index )
{
	m_mFamilyWinData;
}
STRING CDataPool::Guild_GetFamilyWindowData( INT index )
{
	m_mFamilyWinData;

	return "";
}


VOID CDataPool::Guild_AskInfo( INT index )
{
	if( INVALID_ID != index )
	{
		CGGuild pk;
		pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_ASKINFO;

		GUILD_CGW_ASKINFO* pAskInfo = (GUILD_CGW_ASKINFO*)pk.GetGuildPacket()->GetPacket( GUILD_PACKET_CG_ASKINFO );
		if( pAskInfo )
		{
			pAskInfo->m_GuildGUID = index;
			pAskInfo->m_Type = GUILD_CGW_ASKINFO::GUILD_MEMBER_INFO;

			STRING pInputPassword = CDataPool::GetMe()->Rich_GetInputPassword( );

			pAskInfo->m_cPwdSize = (UCHAR)strlen( pInputPassword.c_str() );
			strncpy( pAskInfo->m_Password, pInputPassword.c_str(), MAX_PWD * sizeof(CHAR) );
			pAskInfo->m_Password[MAX_PWD] = 0 ;

			CNetManager::GetMe()->SendPacket( &pk );
		}
	}
}



// 锁定鉴定轴 
void CDataPool::Identify_Lock(INT iBagPos)
{
	// 取消锁定状态
	m_nIdentifyPosition = iBagPos;
	//设置鼠标指令
	CActionSystem::GetMe()->SetDefaultAction(CActionItem_MouseCmd_Identify::GetMe());
	CActionSystem::GetMe()->SetNotCancleIden(true);
}

// 取消鉴定轴锁定 
void CDataPool::Identify_UnLock()
{
	if(-1 == m_nIdentifyPosition) return;

	//设置鼠标指令
	CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
	m_nIdentifyPosition = -1;
}




//---------------------------------------------------------------------------------------------------
// 称号

//称号数量
VOID CDataPool::Title_SetTitleNum( INT nType, INT num )
{
	if( nType <= 0 || nType > 3 )
		return;

	m_iTitleNum[nType] = num;
}
INT CDataPool::Title_GetTitleNum( INT nType )
{
	if( nType <= 0 || nType > 3 )
		return -1;

	return m_iTitleNum[nType];
}


// 当前称号
VOID CDataPool::Title_SetCurTitle( INT nType, INT id )	
{
	if( nType <= 0 || nType > 3 )
		return;

	m_iCurrTitleId[nType] = id; 
};
INT CDataPool::Title_GetCurTitle( INT nType )
{
	if( nType <= 0 || nType > 3 )
		return -1;

	return m_iCurrTitleId[nType]; 
};

// 全部称号
VOID CDataPool::Title_SetTitle( INT nType, INT nIndex, INT nTitleIndex )	// ( INT index, _TITLE_* title )
{
	if( nType <= 0 || nType > 3 )
		return;

	if( nIndex < 0 && nIndex >= MAX_TITLE_SIZE )
		return; 

	m_vTitles[nType][nIndex] = nTitleIndex;
}
INT CDataPool::Title_GetTitle( INT nType, INT nIndex )
{
	if( nType <= 0 || nType > 3 )
		return -1;

	if( nIndex < 0 && nIndex >= MAX_TITLE_SIZE )
		return -1; 

	return m_vTitles[nType][nIndex];
}




//---------------------------------------------------------------------------------------------------

// 清空所有玩家信息
VOID  CDataPool::Finger_ClearInfo()
{
	if( m_FingerPlayerNum == 0 )
		return;

	for( UINT i = 0; i < m_FingerPlayerNum; i++ )
	{
		m_FingerPlayerList[i].CleanUp(); 
	}

	m_FingerPlayerNum = 0;

	return;
}

// 索引玩家信息
FingerPlayerInfo_t* CDataPool::Finger_GetPlayerInfoByIndex(INT nIndex)
{
	if( nIndex >= 0 && nIndex < USER_ARRAY_SIZE )
	{
		return &m_FingerPlayerList[nIndex];
	}

	return NULL;
}

VOID CDataPool::Finger_SetPlayerInfoByIndex(INT nIndex, FingerPlayerInfo_t* pPalyerInfo)
{
	if( nIndex >= 0 && nIndex < USER_ARRAY_SIZE )
	{
		m_FingerPlayerList[nIndex] = *pPalyerInfo;
	}
}


//---------------------------------------------------------------------------------------------------
// 系统提示

BOOL CDataPool::SystemTip_AddEventInfo( INT type, const STRING& strDesc, INT Param )
{

	// 新添加的内容
	SystemTipEvent ste;

	ste.m_eType = (SYSTIP_TYPE)type;	// 事件的类型
	ste.m_strTipDesc = strDesc;			// 显示的提示信息
	
	ste.m_nParameter = Param;


	// 所在ui的索引
	m_UISysTipIndex = (INT)m_mapSysTip.size();
	ste.m_nIndex = m_UISysTipIndex + 1;		// 在界面中的索引
	ste.m_nLevelIndex = 0;


	// 遍历查找是否已经有该类型的提示存在了
	MAP_SYSTIP_ITER iter = m_mapSysTip.begin();
	for( ; iter != m_mapSysTip.end(); iter++ )	// 最多11个
	{
		std::list< SystemTipEvent >::iterator lIter = (*iter).second.begin();

		//如果有相同类型的，并且类型都是（队伍邀请）类型的，则不予显示重复的图标，只显示一个即可
		if (((*lIter).m_eType == ste.m_eType) && (ste.m_eType == STT_TEAMINVITE))
		{
			return TRUE;
		}

		// 类型和已有的相同， 放到同一组中
		if( (*lIter).m_eType == ste.m_eType )
		{
			ste.m_nIndex = (*lIter).m_nIndex;
			(*iter).second.push_back( ste );

			return TRUE;
		}

	}

	// 没有就创建新的类型
	std::list< SystemTipEvent > lSysTip;
	lSysTip.push_back( ste );

	m_nMapIndex[m_nMapIndexNum] = m_UISysTipIndex;
	m_mapSysTip.insert( std::make_pair( m_UISysTipIndex, lSysTip ) );

	m_UISysTipIndex += 1;
	m_nMapIndexNum++;

	return TRUE;
}

BOOL CDataPool::SystemTip_DelEventInfo( INT nIndex )
{
	// 索引合法
	if( nIndex < 0 || nIndex >= m_UISysTipIndex )
		return FALSE;


	INT index = m_nMapIndex[nIndex];
	MAP_SYSTIP::iterator it = m_mapSysTip.find( index );
	if( it != m_mapSysTip.end() )
	{
		// 查找是否保存了多个事件
		if( (*it).second.size() == 1 )	// 只有一个元素
		{
			(*it).second.clear();
			m_mapSysTip.erase( it );

			INT t1, t2;
			for( INT i = nIndex; i < 10; ++i)
			{
				t1 = m_nMapIndex[i];
				t2 = m_nMapIndex[i + 1];

				m_nMapIndex[i] = m_nMapIndex[i + 1];
			}

			m_nMapIndex[10] = -1;
			--m_nMapIndexNum;
		}
		else
		{
			(*it).second.pop_front();
		}

		// 更新索引
		m_UISysTipIndex = (INT)m_mapSysTip.size();
	}
	else 
	{
		return FALSE;
	}


	// 更新界面显示
	CEventSystem::GetMe()->PushEvent( GE_SHOW_SYSTEM_PROMPT );

	return TRUE;
}


INT CDataPool::SystemTip_GetUIIndex()
{
	return m_UISysTipIndex;
}


SystemTipEvent* CDataPool::SystemTip_GetEventInfo( INT nIndex )
{
	// 索引合法
	if( nIndex < 0 || nIndex >= m_UISysTipIndex )
		return NULL;

	MAP_SYSTIP_ITER iter = m_mapSysTip.find( m_nMapIndex[nIndex] );
	if( iter == m_mapSysTip.end() )
	{
		return NULL;
	}

	return &(*( (*iter).second.begin() ));
}

BOOL CDataPool::SystemTip_SetEventState(  )
{
	return TRUE;
}

INT CDataPool::SystemTip_GetIndexByType( INT type, INT Param )
{
	// 遍历查找是否已经有该类型的提示存在了
	MAP_SYSTIP_ITER iter = m_mapSysTip.begin();
	for( ; iter != m_mapSysTip.end(); iter++ )	// 最多11个
	{
		std::list< SystemTipEvent >::iterator lIter = (*iter).second.begin();
		for( ; lIter != (*iter).second.end(); lIter++ )
		{
			// 类型和参数都相同
			if( ( (*lIter).m_eType == type ) && 
				( (*lIter).m_nParameter == Param ) )
			{
				return (*lIter).m_nIndex;
			}
		}

	}

	return -1;
}


//---------------------------------------------------------------------------------------------------
// 密码保护

BOOL CDataPool::Rich_IsSet( MINORPASSWD_INFO::PROTECT_TYPE type )
{
	switch( type )
	{
	// 已经绑定的装备
	case MINORPASSWD_INFO::PROTECT_EQUIP:
		{
			if( ( m_PasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_EQUIP)) ) > 0 )
				return TRUE;
			else
				return FALSE;
		}
		break;
	
	// 仓库
	case MINORPASSWD_INFO::PROTECT_BANK:
		{
			if( ( m_PasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_BANK)) ) > 0 )
				return TRUE;
			else
				return FALSE;
		}
		break;

	// 金币使用
	case MINORPASSWD_INFO::PROTECT_MONEY:
		{
			if( ( m_PasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_MONEY)) ) > 0 )
				return TRUE;
			else
				return FALSE;
		}
		break;

	// 摆摊和购买
	case MINORPASSWD_INFO::PROTECT_STALL:
		{
			if( ( m_PasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_STALL)) ) > 0 )
				return TRUE;
			else
				return FALSE;
		}
		break;
	
	// 帮会 
	case MINORPASSWD_INFO::PROTECT_GUILD:
		{
			if( ( m_PasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_GUILD)) ) > 0 )
				return TRUE;
			else
				return FALSE;
		}
		break;
	};

	return FALSE;
}

INT CDataPool::GetAttriAutoPoint(INT nPos)
{
	switch(nPos)
	{
	case 1:
		return m_AtriAutoPoint & 0x000000FF;
		break;
	case 2:
		return (m_AtriAutoPoint & 0x0000FF00)>>8;
		break;
	case 3:
		return (m_AtriAutoPoint & 0x00FF0000)>>16;
		break;
	case 4:
		return (m_AtriAutoPoint & 0xFF000000)>>24;
		break;
	}
	return 0;
}


// 设置密码输入的状态（分类）
VOID CDataPool::Rich_SetInputPasswdState( INT nType, BOOL bState )
{
	if( nType < 0 || nType >= 5)
		return ;

	m_InputPasswdState[nType] = bState;

}
BOOL CDataPool::Rich_GetInputPasswdState( INT nType )
{
//	if( nType < 0 || nType >= 5)
//		return ;

	return m_InputPasswdState[nType];
}



//---------------------------------------------------------------------------------------------------
// 鼠标命令

void CDataPool::SetMouseCmd()
{
	//设置鼠标指令
	CActionSystem::GetMe()->SetDefaultAction( CActionItem_MouseCmd_Friend::GetMe() );
}
// 取消鼠标命令
void CDataPool::CancelMouseCmd()
{
	//设置鼠标指令
	CActionSystem::GetMe()->SetDefaultAction( CGameProcedure::s_pGameInterface->Skill_GetActive() );
}



//---------------------------------------------------------------------------------------------------
// 国家相关

VOID CDataPool::Country_Set( INT nType, STRING strName )
{
	if( nType < 0 || nType >= COUNTRY_FUNCTION_TYPE_NUM )
		return ;

	m_CountryFunctionInfo[nType].m_eType = (eFunctionaryType)nType; 
	m_CountryFunctionInfo[nType].m_sFunctionaryName = strName;
}

CountryFunctionaryInfo* CDataPool::Country_Get( INT nType )
{
	if( nType < 0 || nType >= COUNTRY_FUNCTION_TYPE_NUM )
		return NULL;

	return &m_CountryFunctionInfo[nType];
}

//---------------------------------------------------------------------------------------------------


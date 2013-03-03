
// 增加了一些升级判断，fancy 2005.12.6
#include "stdafx.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Player.h"
#include "Ability.h"
#include "AbilityManager.h"
#include "CGAskLeanAbility.h"
#include "GCAbilityLevel.h"
#include "LogDefine.h"
#include "Scene.h"


uint CGAskLeanAbilityHandler::Execute( CGAskLeanAbility* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer;
	Assert( pGamePlayer );

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	Assert(pPacket);
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
	if( !pGamePlayer->IsCanLogic() )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR CGAskLeanAbilityHandler::pGamePlayer->IsCanLogic( ): GUID=%X Name:%s.", 
			pGamePlayer->m_HumanGUID, pHuman->GetName() );
		return PACKET_EXE_CONTINUE;
	}

	AbilityID_t nId	= pPacket->GetAbilityID();
	Assert(nId > 0 && nId<MAX_ABILITY);

	INT	NpcID	= pPacket->GetNpcId();
	Obj* pNpcObj = (Obj*) (pScene->GetObjManager()->GetObj(NpcID));
	if(!pNpcObj || !IsMonsterObj( pNpcObj->GetObjType() ))
	{
		return PACKET_EXE_CONTINUE ;
	}

	GCAbilityLevel Msg;
	if(!pHuman->__IsCanLearnAbility(nId))
	{
		Msg.SetAbilityId(nId);
		Msg.SetLevel(pHuman->__GetAbilityLevel(nId));
		Msg.SetResult(ABILITY_UPGRADE_FAIL);
		pGamePlayer->SendPacket( &Msg );
		return PACKET_EXE_CONTINUE ; 
	}

	const AbilityLevelUpConfigManager *pConfigMgr = g_pAbilityManager->GetLevelUpConfigManager();
	Assert(pConfigMgr);

	Ability* pAbility	= g_pAbilityManager->GetAbility(nId);
	Assert(pAbility);

	const AbilityLevelUpConfig *pLevelUpConfig = pConfigMgr->GetLevelUpConfig(pAbility->LevelUpConfigIndex());
	Assert(pLevelUpConfig);

	INT nMyLevel	= pHuman->__GetAbilityLevel(nId);
	const AbilityLevelUpInfo *pLevelUpInfo = pLevelUpConfig->GetLevelUpInfo(nMyLevel+1);
	Assert(pLevelUpInfo);

	
	if(NULL  == pLevelUpInfo->GetNpcName() || (0 != strcmp(((Obj_Character*)pNpcObj)->GetName(), pLevelUpInfo->GetNpcName())) )
	{
		Msg.SetAbilityId(nId);
		Msg.SetLevel(pHuman->__GetAbilityLevel(nId));
		Msg.SetResult(ABILITY_UPGRADE_NPC_ERROR);
		pGamePlayer->SendPacket( &Msg );
		return PACKET_EXE_CONTINUE ;
	}
	//判定距离
	FLOAT fDist = MySqrt(pNpcObj->getWorldPos(), pHuman->getWorldPos());
	if(fDist>MAX_NPC_DISTANCE)
	{
		Msg.SetAbilityId(nId);
		Msg.SetLevel(pHuman->__GetAbilityLevel(nId));
		Msg.SetResult(ABILITY_UPGRADE_NPC_DISTANCE_FAR);
		pGamePlayer->SendPacket( &Msg );
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskLeanAbilityHandler Out Of Range ObjName = %s", pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ; 
	}
	//INT SkillExp = pHuman->__GetAbilityExp(nId);
	//if(pHuman->GetBitLock(Obj_Human::BLT_MONEY_OPT) == TRUE)
	//{
	//	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskLeanAbilityHandler BLT_MONEY_OPT fail GetGUID=%X ", pHuman->GetGUID()) ;
	//	else }

	//在这里去获得升级的花费,并判定升级条件是否满足（Money,Exp,SkillExp）
	UINT nNeedMoney		= (UINT)pLevelUpInfo->GetDemandMoney();
	UINT nNeedExp		= (UINT)pLevelUpInfo->GetDemandExp();
	UINT nNeedSkillExp	= (UINT)pLevelUpInfo->GetLimitAbilityExp();
	UINT nNeedLevel		= (UINT)pLevelUpInfo->GetLimitLevel();
	
	if (nNeedLevel > (UINT)pHuman->GetLevel())
	{
		Msg.SetAbilityId(nId);
		Msg.SetLevel(pHuman->__GetAbilityLevel(nId));
		Msg.SetResult(ABILITY_UPGRADE_HUMAN_LEVEL_ERROR);
		pGamePlayer->SendPacket( &Msg );
		return PACKET_EXE_CONTINUE;
	}
	if (nNeedMoney	> (UINT)pHuman->GetMoney())
	{
		Msg.SetAbilityId(nId);
		Msg.SetLevel(pHuman->__GetAbilityLevel(nId));
		Msg.SetResult(ABILITY_UPGRADE_MONEY_ERROR);
		pGamePlayer->SendPacket( &Msg );
		return PACKET_EXE_CONTINUE;
	}

	if (nNeedExp > (UINT)pHuman->GetExp())
	{
		Msg.SetAbilityId(nId);
		Msg.SetLevel(pHuman->__GetAbilityLevel(nId));
		Msg.SetResult(ABILITY_UPGRADE_EXP_ERROR);
		pGamePlayer->SendPacket( &Msg );
		return PACKET_EXE_CONTINUE;
	}

	if (nNeedSkillExp > pHuman->__GetAbilityExp(nId))
	{
		Msg.SetAbilityId(nId);
		Msg.SetLevel(pHuman->__GetAbilityLevel(nId));
		Msg.SetResult(ABILITY_UPGRADE_SKILL_EXP_ERROR);
		pGamePlayer->SendPacket( &Msg );
		return PACKET_EXE_CONTINUE;
	}

	if (nMyLevel > pAbility->AbilityLevelLimit())
	{
		Msg.SetAbilityId(nId);
		Msg.SetLevel(pHuman->__GetAbilityLevel(nId));
		Msg.SetResult(ABILITY_UPGRADE_LEVEL_ERROR);
		pGamePlayer->SendPacket( &Msg );
		return PACKET_EXE_CONTINUE;
	}
	// 验证完毕升级技能
	pHuman->__SetAbilityLevel(nId, nMyLevel + 1);

	ABILITY_LOG_PARAM	AbilityLogParam;
	AbilityLogParam.CharGUID	=	pHuman->GetGUID();
	AbilityLogParam.AbilityID	=	nId;
	AbilityLogParam.Level		=	pHuman->__GetAbilityLevel(nId);
	AbilityLogParam.OPType		=	ABILITY_OP_LEVELUP;
	AbilityLogParam.SceneID		=	pHuman->getScene()->SceneID();
	AbilityLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
	AbilityLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
	SaveAbilityLog(&AbilityLogParam);

	// 扣除金钱和经验值
	pHuman->SetExp(-(INT)nNeedExp);//pHuman->SetAlterExp( uSpareExp, EXP_OP_BY_LEARN_ABILITY );
	INT uSpareMoney = pHuman->GetMoney() - (UINT)nNeedMoney;
	pHuman->SetMoney( uSpareMoney );
	
	MONEY_LOG_PARAM	MoneyLogParam;
	MoneyLogParam.CharGUID	=	pHuman->GetGUID();
	MoneyLogParam.OPType	=	MONEY_LEARN_ABILITY_COST;
	MoneyLogParam.Count		=	nNeedMoney;
	MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
	MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
	MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
	SaveMoneyLog(&MoneyLogParam);

	//通知客户端学习或者升级成功
	Msg.SetAbilityId(nId);
	Msg.SetLevel(pHuman->__GetAbilityLevel(nId));
	Msg.SetResult(ABILITY_UPGRADE_SUCCESS);
	pGamePlayer->SendPacket( &Msg );
	
	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskLeanAbilityHandler::Execute GUID=%X AbiID=%d NeedMoney=%d NeedExp=%d NeedSkillExp=%d...OK",
		pGamePlayer->m_HumanGUID, nId, nNeedMoney, nNeedExp, nNeedSkillExp ) ;

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION
		return PACKET_EXE_ERROR ;
}

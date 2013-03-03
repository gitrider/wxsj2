//所有客户端发出的不需要处理的包的Handler都定义在这里, 不用每个都单写一个文件.

#include "StdAfx.h"

#include "CGPickDropItem.h"
using namespace Packets;
UINT CGPickDropItemHandler::Execute(class Packets::CGPickDropItem *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCMissionHaveDoneFlag.h"
UINT GCMissionHaveDoneFlagHandler::Execute(class Packets::GCMissionHaveDoneFlag *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGCharMoodState.h"
UINT CGCharMoodStateHandler::Execute(class Packets::CGCharMoodState *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGCountryRequestKindom.h"
UINT CGCountryRequestKindomHandler::Execute(class Packets::CGCountryRequestKindom *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}


#include "CGCountryInfoRequest.h"
UINT CGCountryInfoRequestHandler::Execute(class Packets::CGCountryInfoRequest *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGCountryAppoint.h"
UINT CGCountryAppointHandler::Execute(class Packets::CGCountryAppoint *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGCountryRemove.h"
UINT CGCountryRemoveHandler::Execute(class Packets::CGCountryRemove *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

//#include "CGExchangeSynchLockItemII.h"
//UINT CGExchangeSynchLockItemIIHandler::Execute(class Packets::CGExchangeSynchLockItemII *,class Player *)
//{
//	return PACKET_EXE_CONTINUE;
//}

#include "CGStallOpenPublic.h"
UINT CGStallOpenPublicHandler::Execute(class Packets::CGStallOpenPublic *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}


#include "CGFinger.h"
UINT CGFingerHandler::Execute(class Packets::CGFinger *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}


#include "CGSkillUpgrade.h"
UINT CGSkillUpgradeHandler::Execute(class Packets::CGSkillUpgrade *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}


#include "GCSkillUpgrade.h"
UINT GCSkillUpgradeHandler::Execute(class Packets::GCSkillUpgrade *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCNewBus.h"
UINT GCNewBusHandler::Execute(class Packets::GCNewBus *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCNewBus_Move.h"
UINT GCNewBus_MoveHandler::Execute(class Packets::GCNewBus_Move *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCBusMove.h"
UINT GCBusMoveHandler::Execute(class Packets::GCBusMove *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCBusStopMove.h"
UINT GCBusStopMoveHandler::Execute(class Packets::GCBusStopMove *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCBusAddPassenger.h"
UINT GCBusAddPassengerHandler::Execute(class Packets::GCBusAddPassenger *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCBusRemovePassenger.h"
UINT GCBusRemovePassengerHandler::Execute(class Packets::GCBusRemovePassenger *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCBusRemoveAllPassenger.h"
UINT GCBusRemoveAllPassengerHandler::Execute(class Packets::GCBusRemoveAllPassenger *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGEquipAddHole.h"
UINT CGEquipAddHoleHandler::Execute(class Packets::CGEquipAddHole *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGEquipLevelup.h"
UINT CGEquipLevelupHandler::Execute(class Packets::CGEquipLevelup *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGEquipAdsorbMagic.h"
UINT CGEquipAdsorbMagicHandler::Execute(class Packets::CGEquipAdsorbMagic *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGEquipCancelMagic.h"
UINT CGEquipCancelMagicHandler::Execute(class Packets::CGEquipCancelMagic *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGEquipBind.h"
UINT CGEquipBindHandler::Execute(class Packets::CGEquipBind *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGSaveEquipSuit.h"
UINT CGSaveEquipSuitHandler::Execute(class Packets::CGSaveEquipSuit *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGEquipSuitExchange.h"
UINT CGEquipSuitExchangeHandler::Execute(class Packets::CGEquipSuitExchange *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGShopSpecialRepair.h"
UINT CGShopSpecialRepairHandler::Execute(class Packets::CGShopSpecialRepair *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}


#include "CGItemCreate.h"
UINT CGItemCreateHandler::Execute(class Packets::CGItemCreate *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGEquipRemould.h"
UINT CGEquipRemouldHandler::Execute(class Packets::CGEquipRemould *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGEquipDecompound.h"
UINT CGEquipDecompoundHandler::Execute(class Packets::CGEquipDecompound *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGEquipChangePaiType.h"
UINT CGEquipChangePaiTypeHandler::Execute(class Packets::CGEquipChangePaiType *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGEquipExChangePaiType.h"
UINT CGEquipExChangePaiTypeHandler::Execute(class Packets::CGEquipExChangePaiType *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGUseExtraBag.h"
UINT CGUseExtraBagHandler::Execute(class Packets::CGUseExtraBag *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGUnExtraBag.h"
UINT CGUnExtraBagHandler::Execute(class Packets::CGUnExtraBag *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGAskSubTrainSetting.h"
UINT CGAskSubTrainSettingHandler::Execute(class Packets::CGAskSubTrainSetting *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGAskSubTrainMode.h"
UINT CGAskSubTrainModeHandler::Execute(class Packets::CGAskSubTrainMode *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGAskGetSubTrainExp.h"
UINT CGAskGetSubTrainExpHandler::Execute(class Packets::CGAskGetSubTrainExp *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGAskBeginSubTrain.h"
UINT CGAskBeginSubTrainHandler::Execute(class Packets::CGAskBeginSubTrain *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGAskEndSubTrain.h"
UINT CGAskEndSubTrainHandler::Execute(class Packets::CGAskEndSubTrain *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}


#include "CGSetTeamDistribMode.h"
UINT CGSetTeamDistribModeHandler::Execute(class Packets::CGSetTeamDistribMode *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGAskTeamDistribMode.h"
UINT CGAskTeamDistribModeHandler::Execute(class Packets::CGAskTeamDistribMode *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGModifyPKMode.h"
UINT CGModifyPKModeHandler::Execute(class Packets::CGModifyPKMode *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGOpenBox.h"
UINT CGOpenBoxHandler::Execute(class Packets::CGOpenBox *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGOpenBoxManipulate.h"
UINT CGOpenBoxManipulateHandler::Execute(class Packets::CGOpenBoxManipulate *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGTopList.h"
UINT CGTopListHandler::Execute(class Packets::CGTopList *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGMapInfo.h"
UINT CGMapInfoHandler::Execute(class Packets::CGMapInfo *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGRegisterTeamRecruit.h"
UINT CGRegisterTeamRecruitHandler::Execute(class Packets::CGRegisterTeamRecruit *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGAskTeamRecruitInfo.h"
UINT CGAskTeamRecruitInfoHandler::Execute(class Packets::CGAskTeamRecruitInfo *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGIssueHorseFindMateInfo.h"
UINT CGIssueHorseFindMateInfoHandler::Execute(class Packets::CGIssueHorseFindMateInfo *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGCancelHorseFindMateInfo.h"
UINT CGCancelHorseFindMateInfoHandler::Execute(class Packets::CGCancelHorseFindMateInfo *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGSearchHorseFindMateInfo.h"
UINT CGSearchHorseFindMateInfoHandler::Execute(class Packets::CGSearchHorseFindMateInfo *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGQueryHorseFindMateInfo.h"
UINT CGQueryHorseFindMateInfoHandler::Execute(class Packets::CGQueryHorseFindMateInfo *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGAskOtherOnlineDetailAttrib.h"
UINT CGAskOtherOnlineDetailAttribHandler::Execute(class Packets::CGAskOtherOnlineDetailAttrib *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGSearchOnlineUser.h"
UINT CGSearchOnlineUserHandler::Execute(class Packets::CGSearchOnlineUser *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGCountryCollect.h"
UINT CGCountryCollectHandler::Execute(class Packets::CGCountryCollect *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGCountryFetch.h"
UINT CGCountryFetchHandler::Execute(class Packets::CGCountryFetch *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGCountryDonative.h"
UINT CGCountryDonativeHandler::Execute(class Packets::CGCountryDonative *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGCountryDelate.h"
UINT CGCountryDelateHandler::Execute(class Packets::CGCountryDelate *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGCountryVoteOn.h"
UINT CGCountryVoteOnHandler::Execute(class Packets::CGCountryVoteOn *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGSkillUpgradeBySouXia.h"
UINT CGSkillUpgradeBySouXiaHandler::Execute(class Packets::CGSkillUpgradeBySouXia *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGSouXiaDismantle.h"
UINT CGSouXiaDismantleHandler::Execute(class Packets::CGSouXiaDismantle *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGSouXiaDiscard.h"
UINT CGSouXiaDiscardHandler::Execute(class Packets::CGSouXiaDiscard *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}
#include "CGSouXiaSew.h"
UINT CGSouXiaSewHandler::Execute(class Packets::CGSouXiaSew *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGSouXiaRefining.h"
UINT CGSouXiaRefiningHandler::Execute(class Packets::CGSouXiaRefining *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "CGEquipRefining.h"
UINT CGEquipRefiningHandler::Execute(class Packets::CGEquipRefining *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCNewObjItem.h"
UINT GCNewObjItemHandler::Execute(class Packets::GCNewObjItem *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCPickDropItemResult.h"
UINT GCPickDropItemResultHandler::Execute(class Packets::GCPickDropItemResult *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCCharUseSkill.h"
UINT GCCharUseSkillHandler::Execute(class Packets::GCCharUseSkill *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCBankSize.h"
UINT GCBankSizeHandler::Execute(class Packets::GCBankSize *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCExpandBankSize.h"
UINT GCExpandBankSizeHandler::Execute(class Packets::GCExpandBankSize *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

//#include "GCExchangeSynchLockItemII.h"
//UINT GCExchangeSynchLockItemIIHandler::Execute(class Packets::GCExchangeSynchLockItemII *,class Player *)
//{
//	return PACKET_EXE_CONTINUE;
//}

#include "GCStallOpenPublic.h"
UINT GCStallOpenPublicHandler::Execute(class Packets::GCStallOpenPublic *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCFinger.h"
UINT GCFingerHandler::Execute(class Packets::GCFinger *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCAddLockObj.h"
UINT GCAddLockObjHandler::Execute(class Packets::GCAddLockObj *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}


#include "GCUseBallImpact.h"
UINT GCUseBallImpactHandler::Execute(class Packets::GCUseBallImpact *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCHorseLevelUp.h"
UINT GCHorseLevelUpHandler::Execute(class Packets::GCHorseLevelUp *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCCountryReturn.h"
UINT GCCountryReturnHandler::Execute(class Packets::GCCountryReturn *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCEquipAdsorbMagicResult.h"
UINT GCEquipAdsorbMagicResultHandler::Execute(class Packets::GCEquipAdsorbMagicResult *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCEquipCancelMagicResult.h"
UINT GCEquipCancelMagicResultHandler::Execute(class Packets::GCEquipCancelMagicResult *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCDetailEquipAtt.h"
UINT GCDetailEquipAttHandler::Execute(class Packets::GCDetailEquipAtt *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCEquipAddHoleResult.h"
UINT GCEquipAddHoleResultHandler::Execute(class Packets::GCEquipAddHoleResult *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCEquipLevelupResult.h"
UINT GCEquipLevelupResultHandler::Execute(class Packets::GCEquipLevelupResult *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCEquipBindResult.h"
UINT GCEquipBindResultHandler::Execute(class Packets::GCEquipBindResult *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCEquipSuitExchangeResult.h"
UINT GCEquipSuitExchangeResultHandler::Execute(class Packets::GCEquipSuitExchangeResult *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCSaveEquipSuitResult.h"
UINT GCSaveEquipSuitResultHandler::Execute(class Packets::GCSaveEquipSuitResult *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCShopSpecialRepairResult.h"
UINT GCShopSpecialRepairResultHandler::Execute(class Packets::GCShopSpecialRepairResult *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCItemCreateResult.h"
UINT GCItemCreateResultHandler::Execute(class Packets::GCItemCreateResult *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCEquipRemouldResult.h"
UINT GCEquipRemouldResultHandler::Execute(class Packets::GCEquipRemouldResult *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCEquipDecompoundResult.h"
UINT GCEquipDecompoundResultHandler::Execute(class Packets::GCEquipDecompoundResult *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCEquipChangePaiTypeResult.h"
UINT GCEquipChangePaiTypeResultHandler::Execute(class Packets::GCEquipChangePaiTypeResult *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCEquipExChangePaiTypeResult.h"
UINT GCEquipExChangePaiTypeResultHandler::Execute(class Packets::GCEquipExChangePaiTypeResult *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCUseExtraBagResult.h"
UINT GCUseExtraBagResultHandler::Execute(class Packets::GCUseExtraBagResult *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCUnExtraBagResult.h"
UINT GCUnExtraBagResultHandler::Execute(class Packets::GCUnExtraBagResult *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCRetSubTrainSetting.h"
UINT GCRetSubTrainSettingHandler::Execute(class Packets::GCRetSubTrainSetting *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCEndSubTrainResult.h"
UINT GCEndSubTrainResultHandler::Execute(class Packets::GCEndSubTrainResult *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCSubTrainModeResult.h"
UINT GCSubTrainModeResultHandler::Execute(class Packets::GCSubTrainModeResult *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCSubTrainResult.h"
UINT GCSubTrainResultHandler::Execute(class Packets::GCSubTrainResult *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCRetTeamDistribMode.h"
UINT GCRetTeamDistribModeHandler::Execute(class Packets::GCRetTeamDistribMode *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCTopList.h"
UINT GCTopListHandler::Execute(class Packets::GCTopList *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}


#include "GCCountryInfoConfirm.h"
UINT GCCountryInfoConfirmHandler::Execute(class Packets::GCCountryInfoConfirm *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCRetTeamRecruitInfo.h"
UINT GCRetTeamRecruitInfoHandler::Execute(class Packets::GCRetTeamRecruitInfo *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCReturnHorseFindMateInfo.h"
UINT GCReturnHorseFindMateInfoHandler::Execute(class Packets::GCReturnHorseFindMateInfo *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCReturnOnlineUser.h"
UINT GCReturnOnlineUserHandler::Execute(class Packets::GCReturnOnlineUser *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCCountryCollect.h"
UINT GCCountryCollectHandler::Execute(class Packets::GCCountryCollect *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCCountryFetch.h"
UINT GCCountryFetchHandler::Execute(class Packets::GCCountryFetch *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCCountryDelateBegin.h"
UINT GCCountryDelateBeginHandler::Execute(class Packets::GCCountryDelateBegin *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCCountryVoteShow.h"
UINT GCCountryVoteShowHandler::Execute(class Packets::GCCountryVoteShow *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCOpenBox.h"
UINT GCOpenBoxHandler::Execute(class Packets::GCOpenBox *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCOpenBoxManipulate.h"
UINT GCOpenBoxManipulateHandler::Execute(class Packets::GCOpenBoxManipulate *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCSouXiaList.h"
UINT GCSouXiaListHandler::Execute(class Packets::GCSouXiaList *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCSkillUpgradeBySouXia.h"
UINT GCAddSouXiaSkillHandler::Execute(class Packets::GCAddSouXiaSkill *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCSouXiaDismantle.h"
UINT GCSouXiaDismantleHandler::Execute(class Packets::GCSouXiaDismantle *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCSouXiaRefining.h"
UINT GCSouXiaRefiningHandler::Execute(class Packets::GCSouXiaRefining *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCSouXiaSew.h"
UINT GCSouXiaSewHandler::Execute(class Packets::GCSouXiaSew *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCSouXiaDiscard.h"
UINT GCSouXiaDiscardHandler::Execute(class Packets::GCSouXiaDiscard *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCSouXiaSkillUpdate.h"
UINT GCSouXiaSkillUpdateHandler::Execute(class Packets::GCSouXiaSkillUpdate *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCSouXiaUpdate.h"
UINT GCSouXiaUpdateHandler::Execute(class Packets::GCSouXiaUpdate *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCEquipRefining.h"
UINT GCEquipRefiningHandler::Execute(class Packets::GCEquipRefining *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCNewPlayer_Death.h"
UINT GCNewPlayer_DeathHandler::Execute(class Packets::GCNewPlayer_Death *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCNewPlayer_Move.h"
UINT GCNewPlayer_MoveHandler::Execute(class Packets::GCNewPlayer_Move *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}

#include "GCNewPlayer.h"
UINT GCNewPlayerHandler::Execute(class Packets::GCNewPlayer *,class Player *)
{
	return PACKET_EXE_CONTINUE;
}






























































































































































































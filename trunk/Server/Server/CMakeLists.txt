CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT (Server)

ADD_DEFINITIONS(-D_FOX_SERVER)
ADD_DEFINITIONS(-D_FOX_CLIENT)
ADD_DEFINITIONS(-D_FOX_WORLD)
ADD_DEFINITIONS(-D_LOG_SV)
ADD_DEFINITIONS(-D_USE_32BIT_TIME_T)
ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)

IF(CMAKE_SYSTEM MATCHES Linux)
  ADD_DEFINITIONS(-D__LINUX__)
  ADD_DEFINITIONS(-D_REENTRANT)
ELSE(CMAKE_SYSTEM MATCHES Linux)
  ADD_DEFINITIONS(-D__WINDOWS__)
ENDIF(CMAKE_SYSTEM MATCHES Linux)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

SET(EXECUTABLE_OUTPUT_PATH ${TF_CURRENT_BINARY_PATH_BIN})

##update_begin


INCLUDE_DIRECTORIES(..\\..\\Common\\Utilities)
INCLUDE_DIRECTORIES(Skills\\SkillLogic\\Human)
INCLUDE_DIRECTORIES(AI)
INCLUDE_DIRECTORIES(Item)
INCLUDE_DIRECTORIES(Skills\\SkillLogic\\Common)
INCLUDE_DIRECTORIES(Scene)
INCLUDE_DIRECTORIES(Skills\\SpecialObjLogic)
INCLUDE_DIRECTORIES(..\\..\\Common\\Packets\\MsgMediator)
INCLUDE_DIRECTORIES(Main)
INCLUDE_DIRECTORIES(Skills\\SkillLogic)
INCLUDE_DIRECTORIES(Skills\\SkillLogic\\Pet)
INCLUDE_DIRECTORIES(Other)
INCLUDE_DIRECTORIES(DB)
INCLUDE_DIRECTORIES(MenPai)
INCLUDE_DIRECTORIES(EventModule)
INCLUDE_DIRECTORIES(Team)
INCLUDE_DIRECTORIES(Obj\\Horse)
INCLUDE_DIRECTORIES(Base)
INCLUDE_DIRECTORIES(Skills\\ImpactLogic)
INCLUDE_DIRECTORIES(Script)
INCLUDE_DIRECTORIES(..\\..\\Common)
INCLUDE_DIRECTORIES(..\\..\\Common\\Packets)
INCLUDE_DIRECTORIES(Obj)
INCLUDE_DIRECTORIES(Ability)
INCLUDE_DIRECTORIES(..\\..\\Common\\DataBase)
INCLUDE_DIRECTORIES(Scene\\Scan)
INCLUDE_DIRECTORIES(ActionModule)
INCLUDE_DIRECTORIES(Obj\\Visitor)
INCLUDE_DIRECTORIES(Player)
INCLUDE_DIRECTORIES(SMU)
INCLUDE_DIRECTORIES(..\\..\\Common\\Combat)
INCLUDE_DIRECTORIES(Packets)
INCLUDE_DIRECTORIES(Skills)
INCLUDE_DIRECTORIES(Obj\\HumanAssistantClass)
INCLUDE_DIRECTORIES(Mission)


SET (COMMON_PACKETS_ITEMPART_ITEMSYNCH_LIST
	../../Common/Packets/CGItemSynch.cpp
	../../Common/Packets/CGItemSynch.h
	../../Common/Packets/GCItemList.cpp
	../../Common/Packets/GCItemList.h
	../../Common/Packets/GCItemSynch.cpp
	../../Common/Packets/GCItemSynch.h
	../../Common/Packets/ItemSynchStructDefine.h
)

SET (COMMON_PACKETS_ITEMPART_LIST
	../../Common/Packets/CGAskDetailEquipList.cpp
	../../Common/Packets/CGAskDetailEquipList.h
	../../Common/Packets/CGAskItemInfo.cpp
	../../Common/Packets/CGAskItemInfo.h
	../../Common/Packets/CGAskLockObj.cpp
	../../Common/Packets/CGAskLockObj.h
	../../Common/Packets/CGAskMyBagList.cpp
	../../Common/Packets/CGAskMyBagList.h
	../../Common/Packets/CGDiscardItem.cpp
	../../Common/Packets/CGDiscardItem.h
	../../Common/Packets/CGEquipAddHole.cpp
	../../Common/Packets/CGEquipAddHole.h
	../../Common/Packets/CGEquipAdsorbMagic.cpp
	../../Common/Packets/CGEquipAdsorbMagic.h
	../../Common/Packets/CGEquipBind.cpp
	../../Common/Packets/CGEquipBind.h
	../../Common/Packets/CGEquipCancelMagic.cpp
	../../Common/Packets/CGEquipCancelMagic.h
	../../Common/Packets/CGEquipChangePaiType.cpp
	../../Common/Packets/CGEquipChangePaiType.h
	../../Common/Packets/CGEquipDecompound.cpp
	../../Common/Packets/CGEquipDecompound.h
	../../Common/Packets/CGEquipExChangePaiType.cpp
	../../Common/Packets/CGEquipExChangePaiType.h
	../../Common/Packets/CGEquipLevelup.cpp
	../../Common/Packets/CGEquipLevelup.h
	../../Common/Packets/CGEquipRefining.cpp
	../../Common/Packets/CGEquipRefining.h
	../../Common/Packets/CGEquipRemould.cpp
	../../Common/Packets/CGEquipRemould.h
	../../Common/Packets/CGEquipSuitExchange.cpp
	../../Common/Packets/CGEquipSuitExchange.h
	../../Common/Packets/CGItemCreate.cpp
	../../Common/Packets/CGItemCreate.h
	../../Common/Packets/CGOpenItemBox.cpp
	../../Common/Packets/CGOpenItemBox.h
	../../Common/Packets/CGPackage_SwapItem.cpp
	../../Common/Packets/CGPackage_SwapItem.h
	../../Common/Packets/CGPackUpPacket.cpp
	../../Common/Packets/CGPackUpPacket.h
	../../Common/Packets/CGPickBoxItem.cpp
	../../Common/Packets/CGPickBoxItem.h
	../../Common/Packets/CGPickDropItem.cpp
	../../Common/Packets/CGPickDropItem.h
	../../Common/Packets/CGRemoveGem.cpp
	../../Common/Packets/CGRemoveGem.h
	../../Common/Packets/CGSaveEquipSuit.cpp
	../../Common/Packets/CGSaveEquipSuit.h
	../../Common/Packets/CGSplitItem.cpp
	../../Common/Packets/CGSplitItem.h
	../../Common/Packets/CGUnEquip.cpp
	../../Common/Packets/CGUnEquip.h
	../../Common/Packets/CGUnExtraBag.cpp
	../../Common/Packets/CGUnExtraBag.h
	../../Common/Packets/CGUseEquip.cpp
	../../Common/Packets/CGUseEquip.h
	../../Common/Packets/CGUseExtraBag.cpp
	../../Common/Packets/CGUseExtraBag.h
	../../Common/Packets/CGUseGem.cpp
	../../Common/Packets/CGUseGem.h
	../../Common/Packets/CGUseItem.cpp
	../../Common/Packets/CGUseItem.h
	../../Common/Packets/GCAddLockObj.cpp
	../../Common/Packets/GCAddLockObj.h
	../../Common/Packets/GCBankItemInfo.cpp
	../../Common/Packets/GCBankItemInfo.h
	../../Common/Packets/GCBoxItemList.cpp
	../../Common/Packets/GCBoxItemList.h
	../../Common/Packets/GCDetailEquipAtt.cpp
	../../Common/Packets/GCDetailEquipAtt.h
	../../Common/Packets/GCDetailEquipList.cpp
	../../Common/Packets/GCDetailEquipList.h
	../../Common/Packets/GCDiscardItemResult.cpp
	../../Common/Packets/GCDiscardItemResult.h
	../../Common/Packets/GCEquipAddHoleResult.cpp
	../../Common/Packets/GCEquipAddHoleResult.h
	../../Common/Packets/GCEquipAdsorbMagicResult.cpp
	../../Common/Packets/GCEquipAdsorbMagicResult.h
	../../Common/Packets/GCEquipBindResult.cpp
	../../Common/Packets/GCEquipBindResult.h
	../../Common/Packets/GCEquipCancelMagicResult.cpp
	../../Common/Packets/GCEquipCancelMagicResult.h
	../../Common/Packets/GCEquipChangePaiTypeResult.cpp
	../../Common/Packets/GCEquipChangePaiTypeResult.h
	../../Common/Packets/GCEquipDecompoundResult.cpp
	../../Common/Packets/GCEquipDecompoundResult.h
	../../Common/Packets/GCEquipExChangePaiTypeResult.cpp
	../../Common/Packets/GCEquipExChangePaiTypeResult.h
	../../Common/Packets/GCEquipLevelupResult.cpp
	../../Common/Packets/GCEquipLevelupResult.h
	../../Common/Packets/GCEquipRefining.cpp
	../../Common/Packets/GCEquipRefining.h
	../../Common/Packets/GCEquipRemouldResult.cpp
	../../Common/Packets/GCEquipRemouldResult.h
	../../Common/Packets/GCEquipSuitExchangeResult.cpp
	../../Common/Packets/GCEquipSuitExchangeResult.h
	../../Common/Packets/GCItemCreateResult.cpp
	../../Common/Packets/GCItemCreateResult.h
	../../Common/Packets/GCItemInfo.cpp
	../../Common/Packets/GCItemInfo.h
	../../Common/Packets/GCMyBagList.cpp
	../../Common/Packets/GCMyBagList.h
	../../Common/Packets/GCNewItemBox.cpp
	../../Common/Packets/GCNewItemBox.h
	../../Common/Packets/GCNewObjItem.cpp
	../../Common/Packets/GCNewObjItem.h
	../../Common/Packets/GCNotifyEquip.cpp
	../../Common/Packets/GCNotifyEquip.h
	../../Common/Packets/GCPackage_SwapItem.cpp
	../../Common/Packets/GCPackage_SwapItem.h
	../../Common/Packets/GCPackUpPacket.cpp
	../../Common/Packets/GCPackUpPacket.h
	../../Common/Packets/GCPickDropItemResult.cpp
	../../Common/Packets/GCPickDropItemResult.h
	../../Common/Packets/GCPickResult.cpp
	../../Common/Packets/GCPickResult.h
	../../Common/Packets/GCRemoveGemResult.cpp
	../../Common/Packets/GCRemoveGemResult.h
	../../Common/Packets/GCSaveEquipSuitResult.cpp
	../../Common/Packets/GCSaveEquipSuitResult.h
	../../Common/Packets/GCSplitItemResult.cpp
	../../Common/Packets/GCSplitItemResult.h
	../../Common/Packets/GCUnEquipResult.cpp
	../../Common/Packets/GCUnEquipResult.h
	../../Common/Packets/GCUnExtraBagResult.cpp
	../../Common/Packets/GCUnExtraBagResult.h
	../../Common/Packets/GCUseEquipResult.cpp
	../../Common/Packets/GCUseEquipResult.h
	../../Common/Packets/GCUseExtraBagResult.cpp
	../../Common/Packets/GCUseExtraBagResult.h
	../../Common/Packets/GCUseGemResult.cpp
	../../Common/Packets/GCUseGemResult.h
	../../Common/Packets/GCUseItemResult.cpp
	../../Common/Packets/GCUseItemResult.h
)

SET (COMMON_PACKETS_DATAPOOL_LIST
	../../Common/Packets/CGAskActiveTimeUpdate.h
	../../Common/Packets/CGAskBeginSubTrain.cpp
	../../Common/Packets/CGAskBeginSubTrain.h
	../../Common/Packets/CGAskDetailAbilityInfo.cpp
	../../Common/Packets/CGAskDetailAbilityInfo.h
	../../Common/Packets/CGAskDetailAttrib.cpp
	../../Common/Packets/CGAskDetailAttrib.h
	../../Common/Packets/CGAskDetailSkillList.cpp
	../../Common/Packets/CGAskDetailSkillList.h
	../../Common/Packets/CGAskDetailXinFaList.cpp
	../../Common/Packets/CGAskDetailXinFaList.h
	../../Common/Packets/CGAskEndSubTrain.cpp
	../../Common/Packets/CGAskEndSubTrain.h
	../../Common/Packets/CGAskGetSubTrainExp.cpp
	../../Common/Packets/CGAskGetSubTrainExp.h
	../../Common/Packets/CGAskMissionList.cpp
	../../Common/Packets/CGAskMissionList.h
	../../Common/Packets/CGAskSetting.cpp
	../../Common/Packets/CGAskSetting.h
	../../Common/Packets/CGAskSubTrainMode.cpp
	../../Common/Packets/CGAskSubTrainMode.h
	../../Common/Packets/CGAskSubTrainSetting.cpp
	../../Common/Packets/CGAskSubTrainSetting.h
	../../Common/Packets/CGCleanSubTrainInfo.cpp
	../../Common/Packets/CGCleanSubTrainInfo.h
	../../Common/Packets/CGDiscardEquip.cpp
	../../Common/Packets/CGDiscardEquip.h
	../../Common/Packets/CGDoEvent.h
	../../Common/Packets/CGDoEvnet.cpp
	../../Common/Packets/CGEventRequest.cpp
	../../Common/Packets/CGEventRequest.h
	../../Common/Packets/CGExecuteScript.cpp
	../../Common/Packets/CGExecuteScript.h
	../../Common/Packets/CGMissionAbandon.cpp
	../../Common/Packets/CGMissionAbandon.h
	../../Common/Packets/CGMissionAccept.cpp
	../../Common/Packets/CGMissionAccept.h
	../../Common/Packets/CGMissionCheck.cpp
	../../Common/Packets/CGMissionCheck.h
	../../Common/Packets/CGMissionContinue.cpp
	../../Common/Packets/CGMissionContinue.h
	../../Common/Packets/CGMissionRefuse.cpp
	../../Common/Packets/CGMissionRefuse.h
	../../Common/Packets/CGMissionSubmit.cpp
	../../Common/Packets/CGMissionSubmit.h
	../../Common/Packets/CGModifySetting.cpp
	../../Common/Packets/CGModifySetting.h
	../../Common/Packets/CGPlayerCallOfResult.cpp
	../../Common/Packets/CGPlayerCallOfResult.h
	../../Common/Packets/CGPlayerDieResult.cpp
	../../Common/Packets/CGPlayerDieResult.h
	../../Common/Packets/CGReqLevelUp.cpp
	../../Common/Packets/CGReqLevelUp.h
	../../Common/Packets/CGReqManualAttr.cpp
	../../Common/Packets/CGReqManualAttr.h
	../../Common/Packets/CGReqResetAttr.cpp
	../../Common/Packets/CGReqResetAttr.h
	../../Common/Packets/GCAbandonAbility.cpp
	../../Common/Packets/GCAbandonAbility.h
	../../Common/Packets/GCAbilityExp.cpp
	../../Common/Packets/GCAbilityExp.h
	../../Common/Packets/GCAbilityLevel.cpp
	../../Common/Packets/GCAbilityLevel.h
	../../Common/Packets/GCAddCanPickMissionItem.cpp
	../../Common/Packets/GCAddCanPickMissionItem.h
	../../Common/Packets/GCCanPickMissionItemList.cpp
	../../Common/Packets/GCCanPickMissionItemList.h
	../../Common/Packets/GCDetailAbilityInfo.cpp
	../../Common/Packets/GCDetailAbilityInfo.h
	../../Common/Packets/GCDetailAttrib.cpp
	../../Common/Packets/GCDetailAttrib.h
	../../Common/Packets/GCDetailBUFF.cpp
	../../Common/Packets/GCDetailBUFF.h
	../../Common/Packets/GCDetailHealsAndDamages.cpp
	../../Common/Packets/GCDetailHealsAndDamages.h
	../../Common/Packets/GCDetailImpactListUpdate.cpp
	../../Common/Packets/GCDetailImpactListUpdate.h
	../../Common/Packets/GCDetailSkillList.cpp
	../../Common/Packets/GCDetailSkillList.h
	../../Common/Packets/GCDetailSubEffectList.cpp
	../../Common/Packets/GCDetailSubEffectList.h
	../../Common/Packets/GCDetailXinFaList.cpp
	../../Common/Packets/GCDetailXinFaList.h
	../../Common/Packets/GCDiscardEquipResult.cpp
	../../Common/Packets/GCDiscardEquipResult.h
	../../Common/Packets/GCEndSubTrainResult.cpp
	../../Common/Packets/GCEndSubTrainResult.h
	../../Common/Packets/GCGetSubTrainExpResult.cpp
	../../Common/Packets/GCGetSubTrainExpResult.h
	../../Common/Packets/GCHorseLevelUp.cpp
	../../Common/Packets/GCHorseLevelUp.h
	../../Common/Packets/GCLevelUp.cpp
	../../Common/Packets/GCLevelUp.h
	../../Common/Packets/GCLevelUpResult.cpp
	../../Common/Packets/GCLevelUpResult.h
	../../Common/Packets/GCManualAttrResult.cpp
	../../Common/Packets/GCManualAttrResult.h
	../../Common/Packets/GCMissionAdd.cpp
	../../Common/Packets/GCMissionAdd.h
	../../Common/Packets/GCMissionHaveDoneFlag.cpp
	../../Common/Packets/GCMissionHaveDoneFlag.h
	../../Common/Packets/GCMissionList.cpp
	../../Common/Packets/GCMissionList.h
	../../Common/Packets/GCMissionModify.cpp
	../../Common/Packets/GCMissionModify.h
	../../Common/Packets/GCMissionRemove.cpp
	../../Common/Packets/GCMissionRemove.h
	../../Common/Packets/GCMissionResult.cpp
	../../Common/Packets/GCMissionResult.h
	../../Common/Packets/GCNotifyGoodBad.cpp
	../../Common/Packets/GCNotifyGoodBad.h
	../../Common/Packets/GCPlayerCallOf.cpp
	../../Common/Packets/GCPlayerCallOf.h
	../../Common/Packets/GCPlayerDie.cpp
	../../Common/Packets/GCPlayerDie.h
	../../Common/Packets/GCPlayerRelive.cpp
	../../Common/Packets/GCPlayerRelive.h
	../../Common/Packets/GCPrescription.cpp
	../../Common/Packets/GCPrescription.h
	../../Common/Packets/GCRemoveCanPickMissionItem.cpp
	../../Common/Packets/GCRemoveCanPickMissionItem.h
	../../Common/Packets/GCReqResetAttrResult.cpp
	../../Common/Packets/GCReqResetAttrResult.h
	../../Common/Packets/GCRetSetting.cpp
	../../Common/Packets/GCRetSetting.h
	../../Common/Packets/GCRetSubTrainSetting.cpp
	../../Common/Packets/GCRetSubTrainSetting.h
	../../Common/Packets/GCScriptCommand.cpp
	../../Common/Packets/GCScriptCommand.h
	../../Common/Packets/GCSubTrainModeResult.cpp
	../../Common/Packets/GCSubTrainModeResult.h
	../../Common/Packets/GCSubTrainResult.cpp
	../../Common/Packets/GCSubTrainResult.h
	../../Common/Packets/GCUICommand.cpp
	../../Common/Packets/GCUICommand.h
	../../Common/Packets/GCUseBallImpact.cpp
	../../Common/Packets/GCUseBallImpact.h
)

SET (COMMON_PACKETS_OBJECT_LIST
	../../Common/Packets/CGAskOtherOnlineDetailAttrib.cpp
	../../Common/Packets/CGAskOtherOnlineDetailAttrib.h
	../../Common/Packets/CGAttack.cpp
	../../Common/Packets/CGAttack.h
	../../Common/Packets/CGCharAllTitles.cpp
	../../Common/Packets/CGCharAllTitles.h
	../../Common/Packets/CGCharAskBaseAttrib.cpp
	../../Common/Packets/CGCharAskBaseAttrib.h
	../../Common/Packets/CGCharAskEquipment.cpp
	../../Common/Packets/CGCharAskEquipment.h
	../../Common/Packets/CGCharDefaultEvent.h
	../../Common/Packets/CGCharDefaultEvnet.cpp
	../../Common/Packets/CGCharIdle.cpp
	../../Common/Packets/CGCharIdle.h
	../../Common/Packets/CGCharJump.cpp
	../../Common/Packets/CGCharJump.h
	../../Common/Packets/CGCharMoodState.cpp
	../../Common/Packets/CGCharMoodState.h
	../../Common/Packets/CGCharMove.cpp
	../../Common/Packets/CGCharMove.h
	../../Common/Packets/CGCharPositionWarp.cpp
	../../Common/Packets/CGCharPositionWarp.h
	../../Common/Packets/CGCharSit.cpp
	../../Common/Packets/CGCharSit.h
	../../Common/Packets/CGCharStopLogic.cpp
	../../Common/Packets/CGCharStopLogic.h
	../../Common/Packets/CGCharUpdateCurTitle.cpp
	../../Common/Packets/CGCharUpdateCurTitle.h
	../../Common/Packets/CGCharUseSkill.cpp
	../../Common/Packets/CGCharUseSkill.h
	../../Common/Packets/CGDispelBuffReq.cpp
	../../Common/Packets/CGDispelBuffReq.h
	../../Common/Packets/CGLockTarget.cpp
	../../Common/Packets/CGLockTarget.h
	../../Common/Packets/CGOtherEquip.cpp
	../../Common/Packets/CGOtherEquip.h
	../../Common/Packets/CGSkill.cpp
	../../Common/Packets/CGSkill.h
	../../Common/Packets/CGSkillUpgrade.cpp
	../../Common/Packets/CGSkillUpgrade.h
	../../Common/Packets/CGUseSectorSkill.cpp
	../../Common/Packets/CGUseSectorSkill.h
	../../Common/Packets/GCArrive.cpp
	../../Common/Packets/GCArrive.h
	../../Common/Packets/GCAttack.cpp
	../../Common/Packets/GCAttack.h
	../../Common/Packets/GCCharAction.cpp
	../../Common/Packets/GCCharAction.h
	../../Common/Packets/GCCharAllTitles.cpp
	../../Common/Packets/GCCharAllTitles.h
	../../Common/Packets/GCCharBaseAttrib.cpp
	../../Common/Packets/GCCharBaseAttrib.h
	../../Common/Packets/GCCharBUFF.cpp
	../../Common/Packets/GCCharBUFF.h
	../../Common/Packets/GCCharDirectImpact.cpp
	../../Common/Packets/GCCharDirectImpact.h
	../../Common/Packets/GCCharDoAction.cpp
	../../Common/Packets/GCCharDoAction.h
	../../Common/Packets/GCCharEquipment.cpp
	../../Common/Packets/GCCharEquipment.h
	../../Common/Packets/GCCharIdle.cpp
	../../Common/Packets/GCCharIdle.h
	../../Common/Packets/GCCharImpactListUpdate.cpp
	../../Common/Packets/GCCharImpactListUpdate.h
	../../Common/Packets/GCCharJump.cpp
	../../Common/Packets/GCCharJump.h
	../../Common/Packets/GCCharModifyAction.cpp
	../../Common/Packets/GCCharModifyAction.h
	../../Common/Packets/GCCharMove.cpp
	../../Common/Packets/GCCharMove.h
	../../Common/Packets/GCCharMoveResult.cpp
	../../Common/Packets/GCCharMoveResult.h
	../../Common/Packets/GCCharSkill_CreateBullet.cpp
	../../Common/Packets/GCCharSkill_CreateBullet.h
	../../Common/Packets/GCCharSkill_Gather.cpp
	../../Common/Packets/GCCharSkill_Gather.h
	../../Common/Packets/GCCharSkill_Gather_Modify.cpp
	../../Common/Packets/GCCharSkill_Gather_Modify.h
	../../Common/Packets/GCCharSkill_Lead.cpp
	../../Common/Packets/GCCharSkill_Lead.h
	../../Common/Packets/GCCharSkill_Lead_Modify.cpp
	../../Common/Packets/GCCharSkill_Lead_Modify.h
	../../Common/Packets/GCCharSkill_Missed.cpp
	../../Common/Packets/GCCharSkill_Missed.h
	../../Common/Packets/GCCharSkill_Send.cpp
	../../Common/Packets/GCCharSkill_Send.h
	../../Common/Packets/GCCharStopAction.cpp
	../../Common/Packets/GCCharStopAction.h
	../../Common/Packets/GCCharUseSkill.cpp
	../../Common/Packets/GCCharUseSkill.h
	../../Common/Packets/GCCoolDownUpdate.cpp
	../../Common/Packets/GCCoolDownUpdate.h
	../../Common/Packets/GCDelObject.cpp
	../../Common/Packets/GCDelObject.h
	../../Common/Packets/GCDie.cpp
	../../Common/Packets/GCDie.h
	../../Common/Packets/GCErrorAttack.cpp
	../../Common/Packets/GCErrorAttack.h
	../../Common/Packets/GCErrorSkill.cpp
	../../Common/Packets/GCErrorSkill.h
	../../Common/Packets/GCModifySpeed.cpp
	../../Common/Packets/GCModifySpeed.h
	../../Common/Packets/GCMonsterRealMove.cpp
	../../Common/Packets/GCMonsterRealMove.h
	../../Common/Packets/GCMonsterSpeak.cpp
	../../Common/Packets/GCMonsterSpeak.h
	../../Common/Packets/GCNewItem.cpp
	../../Common/Packets/GCNewItem.h
	../../Common/Packets/GCNewMonster.cpp
	../../Common/Packets/GCNewMonster.h
	../../Common/Packets/GCNewMonster_Death.cpp
	../../Common/Packets/GCNewMonster_Death.h
	../../Common/Packets/GCNewMonster_Move.cpp
	../../Common/Packets/GCNewMonster_Move.h
	../../Common/Packets/GCNewPet.cpp
	../../Common/Packets/GCNewPet.h
	../../Common/Packets/GCNewPet_Death.cpp
	../../Common/Packets/GCNewPet_Death.h
	../../Common/Packets/GCNewPet_Move.cpp
	../../Common/Packets/GCNewPet_Move.h
	../../Common/Packets/GCNewPlatform.cpp
	../../Common/Packets/GCNewPlatform.h
	../../Common/Packets/GCNewPlayer.cpp
	../../Common/Packets/GCNewPlayer.h
	../../Common/Packets/GCNewPlayer_Death.cpp
	../../Common/Packets/GCNewPlayer_Death.h
	../../Common/Packets/GCNewPlayer_Move.cpp
	../../Common/Packets/GCNewPlayer_Move.h
	../../Common/Packets/GCNewPortal.cpp
	../../Common/Packets/GCNewPortal.h
	../../Common/Packets/GCNewSkillObj.cpp
	../../Common/Packets/GCNewSkillObj.h
	../../Common/Packets/GCNewSpecial.cpp
	../../Common/Packets/GCNewSpecial.h
	../../Common/Packets/GCObjTeleport.cpp
	../../Common/Packets/GCObjTeleport.h
	../../Common/Packets/GCOtherAttack.cpp
	../../Common/Packets/GCOtherAttack.h
	../../Common/Packets/GCOtherEquip.cpp
	../../Common/Packets/GCOtherEquip.h
	../../Common/Packets/GCOtherSkill.cpp
	../../Common/Packets/GCOtherSkill.h
	../../Common/Packets/GCPlayerRealMove.cpp
	../../Common/Packets/GCPlayerRealMove.h
	../../Common/Packets/GCSkill.cpp
	../../Common/Packets/GCSkill.h
	../../Common/Packets/GCSkillPrepare.cpp
	../../Common/Packets/GCSkillPrepare.h
	../../Common/Packets/GCSkillSuccessfulUse.cpp
	../../Common/Packets/GCSkillSuccessfulUse.h
	../../Common/Packets/GCSkillUpgrade.cpp
	../../Common/Packets/GCSkillUpgrade.h
	../../Common/Packets/GCSpecialObj_ActNow.cpp
	../../Common/Packets/GCSpecialObj_ActNow.h
	../../Common/Packets/GCSpecialObj_FadeOut.cpp
	../../Common/Packets/GCSpecialObj_FadeOut.h
	../../Common/Packets/GCTargetListAndHitFlags.cpp
	../../Common/Packets/GCTargetListAndHitFlags.h
	../../Common/Packets/GWAskOtherOnlineAttrib_Human.cpp
	../../Common/Packets/GWAskOtherOnlineAttrib_Human.h
	../../Common/Packets/GWDetailOtherOnlineAttrib_Horse.cpp
	../../Common/Packets/GWDetailOtherOnlineAttrib_Horse.h
	../../Common/Packets/GWDetailOtherOnlineAttrib_Human.cpp
	../../Common/Packets/GWDetailOtherOnlineAttrib_Human.h
	../../Common/Packets/GWDetailOtherOnlineEquipList_Human.cpp
	../../Common/Packets/GWDetailOtherOnlineEquipList_Human.h
	../../Common/Packets/GWRefreshDataToWorld.cpp
	../../Common/Packets/GWRefreshDataToWorld.h
	../../Common/Packets/WGAskOtherOnlineAttrib_Human.cpp
	../../Common/Packets/WGAskOtherOnlineAttrib_Human.h
	../../Common/Packets/WGDetailOtherOnlineAttrib_Horse.cpp
	../../Common/Packets/WGDetailOtherOnlineAttrib_Horse.h
	../../Common/Packets/WGDetailOtherOnlineAttrib_Human.cpp
	../../Common/Packets/WGDetailOtherOnlineAttrib_Human.h
	../../Common/Packets/WGDetailOtherOnlineEquipList_Human.cpp
	../../Common/Packets/WGDetailOtherOnlineEquipList_Human.h
)

SET (COMMON_PACKETS_WORLD_LIST
	../../Common/Packets/CGAskChangeScene.cpp
	../../Common/Packets/CGAskChangeScene.h
	../../Common/Packets/CGEnterScene.cpp
	../../Common/Packets/CGEnterScene.h
	../../Common/Packets/CGEnvRequest.cpp
	../../Common/Packets/CGEnvRequest.h
	../../Common/Packets/CGHeartBeat.cpp
	../../Common/Packets/CGHeartBeat.h
	../../Common/Packets/CGLeaveScene.cpp
	../../Common/Packets/CGLeaveScene.h
	../../Common/Packets/CGOpenBox.cpp
	../../Common/Packets/CGOpenBox.h
	../../Common/Packets/CGOpenBoxManipulate.cpp
	../../Common/Packets/CGOpenBoxManipulate.h
	../../Common/Packets/GCEnterScene.cpp
	../../Common/Packets/GCEnterScene.h
	../../Common/Packets/GCLeaveScene.cpp
	../../Common/Packets/GCLeaveScene.h
	../../Common/Packets/GCNotifyChangeScene.cpp
	../../Common/Packets/GCNotifyChangeScene.h
	../../Common/Packets/GCNotifyTeamInfo.cpp
	../../Common/Packets/GCNotifyTeamInfo.h
	../../Common/Packets/GCOpenBox.cpp
	../../Common/Packets/GCOpenBox.h
	../../Common/Packets/GCOpenBoxManipulate.cpp
	../../Common/Packets/GCOpenBoxManipulate.h
	../../Common/Packets/GCRetChangeScene.cpp
	../../Common/Packets/GCRetChangeScene.h
	../../Common/Packets/GWAskSceneData.cpp
	../../Common/Packets/GWAskSceneData.h
	../../Common/Packets/GWCallOfHuman.cpp
	../../Common/Packets/GWCallOfHuman.h
	../../Common/Packets/GWChangeMenpai.cpp
	../../Common/Packets/GWChangeMenpai.h
	../../Common/Packets/GWLevelUp.cpp
	../../Common/Packets/GWLevelUp.h
	../../Common/Packets/GWUpdateTitle.cpp
	../../Common/Packets/GWUpdateTitle.h
	../../Common/Packets/WGCallOfHuman.cpp
	../../Common/Packets/WGCallOfHuman.h
	../../Common/Packets/WGRetSceneData.cpp
	../../Common/Packets/WGRetSceneData.h
)

SET (COMMON_PACKETS_ABILITY_LIST
	../../Common/Packets/CGAskLeanAbility.cpp
	../../Common/Packets/CGAskLeanAbility.h
	../../Common/Packets/CGGemCompound.cpp
	../../Common/Packets/CGGemCompound.h
	../../Common/Packets/CGUseAbility.cpp
	../../Common/Packets/CGUseAbility.h
	../../Common/Packets/GCAbilityAction.cpp
	../../Common/Packets/GCAbilityAction.h
	../../Common/Packets/GCAbilityResult.cpp
	../../Common/Packets/GCAbilityResult.h
	../../Common/Packets/GCAbilitySucc.cpp
	../../Common/Packets/GCAbilitySucc.h
	../../Common/Packets/GCAbilityTeacherInfo.cpp
	../../Common/Packets/GCAbilityTeacherInfo.h
	../../Common/Packets/GCAskLeanAbility.cpp
	../../Common/Packets/GCAskLeanAbility.h
)

SET (COMMON_PACKETS_TEAM_LIST
	../../Common/Packets/CGAskTeamDistribMode.cpp
	../../Common/Packets/CGAskTeamDistribMode.h
	../../Common/Packets/CGAskTeamFollow.cpp
	../../Common/Packets/CGAskTeamFollow.h
	../../Common/Packets/CGAskTeamInfo.cpp
	../../Common/Packets/CGAskTeamInfo.h
	../../Common/Packets/CGAskTeamMemberInfo.cpp
	../../Common/Packets/CGAskTeamMemberInfo.h
	../../Common/Packets/CGAskTeamRecruitInfo.cpp
	../../Common/Packets/CGAskTeamRecruitInfo.h
	Packets/CGAskTeamRecruitInfoHandler.cpp
	../../Common/Packets/CGRegisterTeamRecruit.cpp
	../../Common/Packets/CGRegisterTeamRecruit.h
	../../Common/Packets/CGReturnTeamFollow.cpp
	../../Common/Packets/CGReturnTeamFollow.h
	../../Common/Packets/CGSetTeamDistribMode.cpp
	../../Common/Packets/CGSetTeamDistribMode.h
	../../Common/Packets/CGStopTeamFollow.cpp
	../../Common/Packets/CGStopTeamFollow.h
	../../Common/Packets/CGTeamApply.cpp
	../../Common/Packets/CGTeamApply.h
	../../Common/Packets/CGTeamAppoint.cpp
	../../Common/Packets/CGTeamAppoint.h
	../../Common/Packets/CGTeamDismiss.cpp
	../../Common/Packets/CGTeamDismiss.h
	../../Common/Packets/CGTeamInvite.cpp
	../../Common/Packets/CGTeamInvite.h
	../../Common/Packets/CGTeamKick.cpp
	../../Common/Packets/CGTeamKick.h
	../../Common/Packets/CGTeamLeaderRetInvite.cpp
	../../Common/Packets/CGTeamLeaderRetInvite.h
	../../Common/Packets/CGTeamLeave.cpp
	../../Common/Packets/CGTeamLeave.h
	../../Common/Packets/CGTeamRetApply.cpp
	../../Common/Packets/CGTeamRetApply.h
	../../Common/Packets/CGTeamRetInvite.cpp
	../../Common/Packets/CGTeamRetInvite.h
	../../Common/Packets/GCAskTeamFollow.cpp
	../../Common/Packets/GCAskTeamFollow.h
	../../Common/Packets/GCRetTeamDistribMode.cpp
	../../Common/Packets/GCRetTeamDistribMode.h
	../../Common/Packets/GCRetTeamRecruitInfo.cpp
	../../Common/Packets/GCRetTeamRecruitInfo.h
	../../Common/Packets/GCReturnTeamFollow.cpp
	../../Common/Packets/GCReturnTeamFollow.h
	../../Common/Packets/GCTeamAskApply.cpp
	../../Common/Packets/GCTeamAskApply.h
	../../Common/Packets/GCTeamAskInvite.cpp
	../../Common/Packets/GCTeamAskInvite.h
	../../Common/Packets/GCTeamError.cpp
	../../Common/Packets/GCTeamError.h
	../../Common/Packets/GCTeamFollowErr.cpp
	../../Common/Packets/GCTeamFollowErr.h
	../../Common/Packets/GCTeamFollowList.cpp
	../../Common/Packets/GCTeamFollowList.h
	../../Common/Packets/GCTeamLeaderAskInvite.cpp
	../../Common/Packets/GCTeamLeaderAskInvite.h
	../../Common/Packets/GCTeamList.cpp
	../../Common/Packets/GCTeamList.h
	../../Common/Packets/GCTeamMemberInfo.cpp
	../../Common/Packets/GCTeamMemberInfo.h
	../../Common/Packets/GCTeamResult.cpp
	../../Common/Packets/GCTeamResult.h
	../../Common/Packets/GWAskTeamInfo.cpp
	../../Common/Packets/GWAskTeamInfo.h
	../../Common/Packets/GWAskTeamMemberInfo.cpp
	../../Common/Packets/GWAskTeamMemberInfo.h
	../../Common/Packets/GWAskTeamRecruitInfo.cpp
	../../Common/Packets/GWAskTeamRecruitInfo.h
	../../Common/Packets/GWEnterTeamFollow.cpp
	../../Common/Packets/GWEnterTeamFollow.h
	../../Common/Packets/GWRegisterTeamRecruit.cpp
	../../Common/Packets/GWRegisterTeamRecruit.h
	../../Common/Packets/GWSetTeamDistribMode.cpp
	../../Common/Packets/GWSetTeamDistribMode.h
	../../Common/Packets/GWStopTeamFollow.cpp
	../../Common/Packets/GWStopTeamFollow.h
	../../Common/Packets/GWTeamApply.cpp
	../../Common/Packets/GWTeamApply.h
	../../Common/Packets/GWTeamAppoint.cpp
	../../Common/Packets/GWTeamAppoint.h
	../../Common/Packets/GWTeamDismiss.cpp
	../../Common/Packets/GWTeamDismiss.h
	../../Common/Packets/GWTeamInvite.cpp
	../../Common/Packets/GWTeamInvite.h
	../../Common/Packets/GWTeamKick.cpp
	../../Common/Packets/GWTeamKick.h
	../../Common/Packets/GWTeamLeaderDie.cpp
	../../Common/Packets/GWTeamLeaderDie.h
	../../Common/Packets/GWTeamLeaderRetInvite.cpp
	../../Common/Packets/GWTeamLeaderRetInvite.h
	../../Common/Packets/GWTeamLeave.cpp
	../../Common/Packets/GWTeamLeave.h
	../../Common/Packets/GWTeamMemberEnterScene.cpp
	../../Common/Packets/GWTeamMemberEnterScene.h
	../../Common/Packets/GWTeamRetApply.cpp
	../../Common/Packets/GWTeamRetApply.h
	../../Common/Packets/GWTeamRetInvite.cpp
	../../Common/Packets/GWTeamRetInvite.h
	../../Common/Packets/WGRetTeamDistribMode.cpp
	../../Common/Packets/WGRetTeamDistribMode.h
	../../Common/Packets/WGRetTeamRecruitInfo.cpp
	../../Common/Packets/WGRetTeamRecruitInfo.h
	../../Common/Packets/WGTeamAskApply.cpp
	../../Common/Packets/WGTeamAskApply.h
	../../Common/Packets/WGTeamAskInvite.cpp
	../../Common/Packets/WGTeamAskInvite.h
	../../Common/Packets/WGTeamError.cpp
	../../Common/Packets/WGTeamError.h
	../../Common/Packets/WGTeamFollowList.cpp
	../../Common/Packets/WGTeamFollowList.h
	../../Common/Packets/WGTeamLeaderAskInvite.cpp
	../../Common/Packets/WGTeamLeaderAskInvite.h
	../../Common/Packets/WGTeamList.cpp
	../../Common/Packets/WGTeamList.h
	../../Common/Packets/WGTeamMemberInfo.cpp
	../../Common/Packets/WGTeamMemberInfo.h
	../../Common/Packets/WGTeamResult.cpp
	../../Common/Packets/WGTeamResult.h
)

SET (COMMON_PACKETS_CHAT_LIST
	../../Common/Packets/CGAskMail.cpp
	../../Common/Packets/CGAskMail.h
	../../Common/Packets/CGChannelCreate.cpp
	../../Common/Packets/CGChannelCreate.h
	../../Common/Packets/CGChannelDismiss.cpp
	../../Common/Packets/CGChannelDismiss.h
	../../Common/Packets/CGChannelInvite.cpp
	../../Common/Packets/CGChannelInvite.h
	../../Common/Packets/CGChannelKick.cpp
	../../Common/Packets/CGChannelKick.h
	../../Common/Packets/CGChat.cpp
	../../Common/Packets/CGChat.h
	../../Common/Packets/CGMail.cpp
	../../Common/Packets/CGMail.h
	../../Common/Packets/GCChannelError.cpp
	../../Common/Packets/GCChannelError.h
	../../Common/Packets/GCChannelResult.cpp
	../../Common/Packets/GCChannelResult.h
	../../Common/Packets/GCChat.cpp
	../../Common/Packets/GCChat.h
	../../Common/Packets/GCMail.cpp
	../../Common/Packets/GCMail.h
	../../Common/Packets/GCNotifyMail.cpp
	../../Common/Packets/GCNotifyMail.h
	../../Common/Packets/GWAskMail.cpp
	../../Common/Packets/GWAskMail.h
	../../Common/Packets/GWBatchMail.cpp
	../../Common/Packets/GWBatchMail.h
	../../Common/Packets/GWChannelCreate.cpp
	../../Common/Packets/GWChannelCreate.h
	../../Common/Packets/GWChannelDismiss.cpp
	../../Common/Packets/GWChannelDismiss.h
	../../Common/Packets/GWChannelInvite.cpp
	../../Common/Packets/GWChannelInvite.h
	../../Common/Packets/GWChannelKick.cpp
	../../Common/Packets/GWChannelKick.h
	../../Common/Packets/GWChat.cpp
	../../Common/Packets/GWChat.h
	../../Common/Packets/GWMail.cpp
	../../Common/Packets/GWMail.h
	../../Common/Packets/WGChannelError.cpp
	../../Common/Packets/WGChannelError.h
	../../Common/Packets/WGChannelResult.cpp
	../../Common/Packets/WGChannelResult.h
	../../Common/Packets/WGChat.cpp
	../../Common/Packets/WGChat.h
	../../Common/Packets/WGMail.cpp
	../../Common/Packets/WGMail.h
	../../Common/Packets/WGNotifyMail.cpp
	../../Common/Packets/WGNotifyMail.h
)

SET (COMMON_PACKETS_COMMERCE_LIST
	../../Common/Packets/CGShopBuy.cpp
	../../Common/Packets/CGShopBuy.h
	../../Common/Packets/CGShopClose.cpp
	../../Common/Packets/CGShopClose.h
	../../Common/Packets/CGShopRepair.cpp
	../../Common/Packets/CGShopRepair.h
	../../Common/Packets/CGShopSell.cpp
	../../Common/Packets/CGShopSell.h
	../../Common/Packets/CGShopSpecialRepair.cpp
	../../Common/Packets/CGShopSpecialRepair.h
	../../Common/Packets/GCShopBuy.cpp
	../../Common/Packets/GCShopBuy.h
	../../Common/Packets/GCShopMerchandiseList.cpp
	../../Common/Packets/GCShopMerchandiseList.h
	../../Common/Packets/GCShopRepair.cpp
	../../Common/Packets/GCShopRepair.h
	../../Common/Packets/GCShopSell.cpp
	../../Common/Packets/GCShopSell.h
	../../Common/Packets/GCShopSoldList.cpp
	../../Common/Packets/GCShopSoldList.h
	../../Common/Packets/GCShopSpecialRepairResult.cpp
	../../Common/Packets/GCShopSpecialRepairResult.h
	../../Common/Packets/GCShopUpdateMerchandiseList.cpp
	../../Common/Packets/GCShopUpdateMerchandiseList.h
)

SET (COMMON_PACKETS_MENPAI_LIST
	../../Common/Packets/CGAskJoinMenpai.cpp
	../../Common/Packets/CGAskJoinMenpai.h
	../../Common/Packets/CGAskStudyXinfa.cpp
	../../Common/Packets/CGAskStudyXinfa.h
	../../Common/Packets/GCJoinMenpai.cpp
	../../Common/Packets/GCJoinMenpai.h
	../../Common/Packets/GCMenpaiInfo.cpp
	../../Common/Packets/GCMenpaiInfo.h
	../../Common/Packets/GCStudyXinfa.cpp
	../../Common/Packets/GCStudyXinfa.h
	../../Common/Packets/GCXinfaStudyInfo.cpp
	../../Common/Packets/GCXinfaStudyInfo.h
)

SET (COMMON_PACKETS_BANK_LIST
	../../Common/Packets/CGBankAcquireList.cpp
	../../Common/Packets/CGBankAcquireList.h
	../../Common/Packets/CGBankAddItem.cpp
	../../Common/Packets/CGBankAddItem.h
	../../Common/Packets/CGBankClose.cpp
	../../Common/Packets/CGBankClose.h
	../../Common/Packets/CGBankMoney.cpp
	../../Common/Packets/CGBankMoney.h
	../../Common/Packets/CGBankRemoveItem.cpp
	../../Common/Packets/CGBankRemoveItem.h
	../../Common/Packets/CGBankSwapItem.cpp
	../../Common/Packets/CGBankSwapItem.h
	../../Common/Packets/GCBankAcquireList.cpp
	../../Common/Packets/GCBankAcquireList.h
	../../Common/Packets/GCBankAddItem.cpp
	../../Common/Packets/GCBankAddItem.h
	../../Common/Packets/GCBankBegin.cpp
	../../Common/Packets/GCBankBegin.h
	../../Common/Packets/GCBankMoney.cpp
	../../Common/Packets/GCBankMoney.h
	../../Common/Packets/GCBankRemoveItem.cpp
	../../Common/Packets/GCBankRemoveItem.h
	../../Common/Packets/GCBankSize.cpp
	../../Common/Packets/GCBankSize.h
	../../Common/Packets/GCBankSwapItem.cpp
	../../Common/Packets/GCBankSwapItem.h
	../../Common/Packets/GCExpandBankSize.cpp
	../../Common/Packets/GCExpandBankSize.h
)

SET (COMMON_PACKETS_EXCHANGE_STEPI_LIST
	../../Common/Packets/CGExchangeApplyI.cpp
	../../Common/Packets/CGExchangeApplyI.h
	../../Common/Packets/CGExchangeReplyI.cpp
	../../Common/Packets/CGExchangeReplyI.h
	../../Common/Packets/GCExchangeApplyI.cpp
	../../Common/Packets/GCExchangeApplyI.h
	../../Common/Packets/GCExchangeEstablishI.cpp
	../../Common/Packets/GCExchangeEstablishI.h
)

SET (COMMON_PACKETS_EXCHANGE_STEPII_LIST
	../../Common/Packets/CGExchangeSynchItemII.cpp
	../../Common/Packets/CGExchangeSynchItemII.h
	../../Common/Packets/CGExchangeSynchMoneyII.cpp
	../../Common/Packets/CGExchangeSynchMoneyII.h
	../../Common/Packets/GCExchangeSynchConfirmII.cpp
	../../Common/Packets/GCExchangeSynchConfirmII.h
	../../Common/Packets/GCExchangeSynchII.cpp
	../../Common/Packets/GCExchangeSynchII.h
)

SET (COMMON_PACKETS_EXCHANGE_STEPIII_LIST
	../../Common/Packets/CGExchangeOkIII.cpp
	../../Common/Packets/CGExchangeOkIII.h
	../../Common/Packets/GCExchangeSuccessIII.cpp
	../../Common/Packets/GCExchangeSuccessIII.h
)

SET (COMMON_PACKETS_EXCHANGE_LIST
	../../Common/Packets/CGExchangeCancel.cpp
	../../Common/Packets/CGExchangeCancel.h
	../../Common/Packets/CGExchangeSynchLock.cpp
	../../Common/Packets/CGExchangeSynchLock.h
	../../Common/Packets/ExchangeMsgDefine.h
	../../Common/Packets/GCExchangeCancel.cpp
	../../Common/Packets/GCExchangeCancel.h
	../../Common/Packets/GCExchangeError.cpp
	../../Common/Packets/GCExchangeError.h
	../../Common/Packets/GCExchangeSynchLock.cpp
	../../Common/Packets/GCExchangeSynchLock.h
)

SET (COMMON_PACKETS_MAIN_LIST
	../../Common/Packets/CGCommand.cpp
	../../Common/Packets/CGCommand.h
	../../Common/Packets/CGConnect.cpp
	../../Common/Packets/CGConnect.h
	../../Common/Packets/CGMapInfo.cpp
	../../Common/Packets/CGMapInfo.h
	../../Common/Packets/CGMinorPasswd.cpp
	../../Common/Packets/CGMinorPasswd.h
	../../Common/Packets/CGTest.cpp
	../../Common/Packets/CGTest.h
	../../Common/Packets/GCConnect.cpp
	../../Common/Packets/GCConnect.h
	../../Common/Packets/GCMinorPasswd.cpp
	../../Common/Packets/GCMinorPasswd.h
	../../Common/Packets/GCOperateResult.cpp
	../../Common/Packets/GCOperateResult.h
	../../Common/Packets/GCTest.cpp
	../../Common/Packets/GCTest.h
	../../Common/Packets/GCWorldTime.cpp
	../../Common/Packets/GCWorldTime.h
	../../Common/Packets/GGSceneNotify.cpp
	../../Common/Packets/GGSceneNotify.h
	../../Common/Packets/GWAskChangeScene.cpp
	../../Common/Packets/GWAskChangeScene.h
	../../Common/Packets/GWAskUserData.cpp
	../../Common/Packets/GWAskUserData.h
	../../Common/Packets/GWCommand.cpp
	../../Common/Packets/GWCommand.h
	../../Common/Packets/GWHeartBeat.cpp
	../../Common/Packets/GWHeartBeat.h
	../../Common/Packets/GWNotifyUser.cpp
	../../Common/Packets/GWNotifyUser.h
	../../Common/Packets/SSConnect.cpp
	../../Common/Packets/SSConnect.h
	../../Common/Packets/SSScenePlayerCount.cpp
	../../Common/Packets/SSScenePlayerCount.h
	../../Common/Version.h
	../../Common/Packets/WGNotifyUser.cpp
	../../Common/Packets/WGNotifyUser.h
	../../Common/Packets/WGOperateResult.cpp
	../../Common/Packets/WGOperateResult.h
	../../Common/Packets/WGRetChangeScene.cpp
	../../Common/Packets/WGRetChangeScene.h
	../../Common/Packets/WGRetUserData.cpp
	../../Common/Packets/WGRetUserData.h
	../../Common/Packets/WGWorldTime.cpp
	../../Common/Packets/WGWorldTime.h
)

SET (COMMON_PACKETS_PET_LIST
	../../Common/Packets/CGIssuePetPlacard.cpp
	../../Common/Packets/CGIssuePetPlacard.h
	../../Common/Packets/CGManipulatePet.cpp
	../../Common/Packets/CGManipulatePet.h
	../../Common/Packets/CGSetPetAttrib.cpp
	../../Common/Packets/CGSetPetAttrib.h
	../../Common/Packets/GCDetailAttrib_Pet.cpp
	../../Common/Packets/GCDetailAttrib_Pet.h
	../../Common/Packets/GCManipulatePetRet.cpp
	../../Common/Packets/GCManipulatePetRet.h
	../../Common/Packets/GCPetPlacardList.cpp
	../../Common/Packets/GCPetPlacardList.h
	../../Common/Packets/GCRemovePet.cpp
	../../Common/Packets/GCRemovePet.h
	../../Common/Packets/PetDetailExtraStructDefine.h
)

SET (COMMON_PACKETS_GUILD_LIST
	../../Common/Packets/CGGuild.cpp
	../../Common/Packets/CGGuild.h
	../../Common/Packets/CGGuildApply.cpp
	../../Common/Packets/CGGuildApply.h
	../../Common/Packets/CGGuildJoin.cpp
	../../Common/Packets/CGGuildJoin.h
	../../Common/Packets/GCGuild.cpp
	../../Common/Packets/GCGuild.h
	../../Common/Packets/GCGuildApply.cpp
	../../Common/Packets/GCGuildApply.h
	../../Common/Packets/GCGuildError.cpp
	../../Common/Packets/GCGuildError.h
	../../Common/Packets/GCGuildReturn.cpp
	../../Common/Packets/GCGuildReturn.h
	../../Common/Packets/GWGuild.cpp
	../../Common/Packets/GWGuild.h
	../../Common/Packets/WGGuild.cpp
	../../Common/Packets/WGGuild.h
	../../Common/Packets/WGGuildError.cpp
	../../Common/Packets/WGGuildError.h
	../../Common/Packets/WGGuildReturn.cpp
	../../Common/Packets/WGGuildReturn.h
)

SET (COMMON_PACKETS_STALL_CHECK_LIST
	../../Common/Packets/CGStallApply.cpp
	../../Common/Packets/CGStallApply.h
	../../Common/Packets/CGStallEstablish.cpp
	../../Common/Packets/CGStallEstablish.h
	../../Common/Packets/GCStallApply.cpp
	../../Common/Packets/GCStallApply.h
	../../Common/Packets/GCStallEstablish.cpp
	../../Common/Packets/GCStallEstablish.h
)

SET (COMMON_PACKETS_STALL_SYNCH_LIST
	../../Common/Packets/CGStallAddItem.cpp
	../../Common/Packets/CGStallAddItem.h
	../../Common/Packets/CGStallBuy.cpp
	../../Common/Packets/CGStallBuy.h
	../../Common/Packets/CGStallItemPrice.cpp
	../../Common/Packets/CGStallItemPrice.h
	../../Common/Packets/CGStallOpen.cpp
	../../Common/Packets/CGStallOpen.h
	../../Common/Packets/CGStallOpenPublic.cpp
	../../Common/Packets/CGStallOpenPublic.h
	../../Common/Packets/CGStallRemoveItem.cpp
	../../Common/Packets/CGStallRemoveItem.h
	../../Common/Packets/CGStallShopName.cpp
	../../Common/Packets/CGStallShopName.h
	../../Common/Packets/GCStallAddItem.cpp
	../../Common/Packets/GCStallAddItem.h
	../../Common/Packets/GCStallBuy.cpp
	../../Common/Packets/GCStallBuy.h
	../../Common/Packets/GCStallOpen.cpp
	../../Common/Packets/GCStallOpen.h
	../../Common/Packets/GCStallOpenPublic.cpp
	../../Common/Packets/GCStallOpenPublic.h
	../../Common/Packets/GCStallRemoveItem.cpp
	../../Common/Packets/GCStallRemoveItem.h
)

SET (COMMON_PACKETS_STALL_CLOSE_LIST
	../../Common/Packets/CGStallClose.cpp
	../../Common/Packets/CGStallClose.h
	../../Common/Packets/GCStallClose.cpp
	../../Common/Packets/GCStallClose.h
)

SET (COMMON_PACKETS_STALL_BBS_LIST
	../../Common/Packets/CGBBSApply.cpp
	../../Common/Packets/CGBBSApply.h
	../../Common/Packets/CGBBSSychMessages.cpp
	../../Common/Packets/CGBBSSychMessages.h
	../../Common/Packets/GCBBSMessages.cpp
	../../Common/Packets/GCBBSMessages.h
)

SET (COMMON_PACKETS_STALL_LIST
	../../Common/Packets/CGStallDefaultPage.cpp
	../../Common/Packets/CGStallDefaultPage.h
	../../Common/Packets/GCStallError.cpp
	../../Common/Packets/GCStallError.h
	../../Common/Packets/GCStallItemPrice.cpp
	../../Common/Packets/GCStallItemPrice.h
)

SET (COMMON_PACKETS_RELATION_LIST
	../../Common/Packets/CGFinger.cpp
	../../Common/Packets/CGFinger.h
	../../Common/Packets/CGRelation.cpp
	../../Common/Packets/CGRelation.h
	../../Common/Packets/GCFinger.cpp
	../../Common/Packets/GCFinger.h
	../../Common/Packets/GCRelation.cpp
	../../Common/Packets/GCRelation.h
	../../Common/Packets/GWFinger.cpp
	../../Common/Packets/GWFinger.h
	../../Common/Packets/GWRelation.cpp
	../../Common/Packets/GWRelation.h
	../../Common/Packets/WGFinger.cpp
	../../Common/Packets/WGFinger.h
	../../Common/Packets/WGRelation.cpp
	../../Common/Packets/WGRelation.h
)

SET (COMMON_PACKETS_PLAYERSHOP_APPLY_LIST
	../../Common/Packets/CGPlayerShopApply.cpp
	../../Common/Packets/CGPlayerShopApply.h
	../../Common/Packets/CGPlayerShopEstablish.cpp
	../../Common/Packets/CGPlayerShopEstablish.h
	../../Common/Packets/GCPlayerShopApply.cpp
	../../Common/Packets/GCPlayerShopApply.h
	../../Common/Packets/GCPlayerShopEstablish.cpp
	../../Common/Packets/GCPlayerShopEstablish.h
)

SET (COMMON_PACKETS_PLAYERSHOP_MANAGER_LIST
	../../Common/Packets/CGPlayerShopAskForRecord.cpp
	../../Common/Packets/CGPlayerShopAskForRecord.h
	../../Common/Packets/CGPlayerShopBuyItem.cpp
	../../Common/Packets/CGPlayerShopBuyItem.h
	../../Common/Packets/CGPlayerShopBuyShop.cpp
	../../Common/Packets/CGPlayerShopBuyShop.h
	../../Common/Packets/CGPlayerShopDesc.cpp
	../../Common/Packets/CGPlayerShopDesc.h
	../../Common/Packets/CGPlayerShopFavorite.cpp
	../../Common/Packets/CGPlayerShopFavorite.h
	../../Common/Packets/CGPlayerShopMoney.cpp
	../../Common/Packets/CGPlayerShopMoney.h
	../../Common/Packets/CGPlayerShopName.cpp
	../../Common/Packets/CGPlayerShopName.h
	../../Common/Packets/CGPlayerShopOnSale.cpp
	../../Common/Packets/CGPlayerShopOnSale.h
	../../Common/Packets/CGPlayerShopOpenStall.cpp
	../../Common/Packets/CGPlayerShopOpenStall.h
	../../Common/Packets/CGPlayerShopPartner.cpp
	../../Common/Packets/CGPlayerShopPartner.h
	../../Common/Packets/CGPlayerShopSaleOut.cpp
	../../Common/Packets/CGPlayerShopSaleOut.h
	../../Common/Packets/CGPlayerShopSize.cpp
	../../Common/Packets/CGPlayerShopSize.h
	../../Common/Packets/CGPlayerShopType.cpp
	../../Common/Packets/CGPlayerShopType.h
	../../Common/Packets/GCPlayerShopBuyShop.cpp
	../../Common/Packets/GCPlayerShopBuyShop.h
	../../Common/Packets/GCPlayerShopMoney.cpp
	../../Common/Packets/GCPlayerShopMoney.h
	../../Common/Packets/GCPlayerShopOnSale.cpp
	../../Common/Packets/GCPlayerShopOnSale.h
	../../Common/Packets/GCPlayerShopOpenStall.cpp
	../../Common/Packets/GCPlayerShopOpenStall.h
	../../Common/Packets/GCPlayerShopRecordList.cpp
	../../Common/Packets/GCPlayerShopRecordList.h
	../../Common/Packets/GCPlayerShopSaleOut.cpp
	../../Common/Packets/GCPlayerShopSaleOut.h
	../../Common/Packets/GCPlayerShopStallStatus.cpp
	../../Common/Packets/GCPlayerShopStallStatus.h
	../../Common/Packets/GCPlayerShopType.cpp
	../../Common/Packets/GCPlayerShopType.h
	../../Common/Packets/GCPlayerShopUpdateFavorite.cpp
	../../Common/Packets/GCPlayerShopUpdateFavorite.h
	../../Common/Packets/GCPlayerShopUpdatePartners.cpp
	../../Common/Packets/GCPlayerShopUpdatePartners.h
)

SET (COMMON_PACKETS_PLAYERSHOP_VIEW_LIST
	../../Common/Packets/CGPlayerShopAcquireItemList.cpp
	../../Common/Packets/CGPlayerShopAcquireItemList.h
	../../Common/Packets/CGPlayerShopAcquireShopList.cpp
	../../Common/Packets/CGPlayerShopAcquireShopList.h
	../../Common/Packets/GCPlayerShopAcquireShopList.cpp
	../../Common/Packets/GCPlayerShopAcquireShopList.h
)

SET (COMMON_PACKETS_PLAYERSHOP_LIST
	../../Common/Packets/GCPlayerShopError.cpp
	../../Common/Packets/GCPlayerShopError.h
)

SET (COMMON_PACKETS_CITY_LIST
	../../Common/Packets/CGCityAskAttr.cpp
	../../Common/Packets/CGCityAskAttr.h
	../../Common/Packets/GCCityAttr.cpp
	../../Common/Packets/GCCityAttr.h
	../../Common/Packets/GCCityError.cpp
	../../Common/Packets/GCCityError.h
	../../Common/Packets/GCCityNotify.cpp
	../../Common/Packets/GCCityNotify.h
	../../Common/Packets/GWCityApplyNewCity.cpp
	../../Common/Packets/GWCityApplyNewCity.h
	../../Common/Packets/GWCityAskInitInfo.cpp
	../../Common/Packets/GWCityAskInitInfo.h
	../../Common/Packets/GWCityClose.cpp
	../../Common/Packets/GWCityClose.h
	../../Common/Packets/GWCityCloseSuccess.cpp
	../../Common/Packets/GWCityCloseSuccess.h
	../../Common/Packets/GWCityOpt.cpp
	../../Common/Packets/GWCityOpt.h
	../../Common/Packets/WGCityApplyNewCity.cpp
	../../Common/Packets/WGCityApplyNewCity.h
	../../Common/Packets/WGCityAttr.cpp
	../../Common/Packets/WGCityAttr.h
	../../Common/Packets/WGCityClose.cpp
	../../Common/Packets/WGCityClose.h
	../../Common/Packets/WGCityCloseSuccess.cpp
	../../Common/Packets/WGCityCloseSuccess.h
	../../Common/Packets/WGCityError.cpp
	../../Common/Packets/WGCityError.h
	../../Common/Packets/WGCityInitInfo.cpp
	../../Common/Packets/WGCityInitInfo.h
)

SET (COMMON_PACKETS_PRIVATEINFO_LIST
	../../Common/Packets/CGApplyPrivateInfo.cpp
	../../Common/Packets/CGApplyPrivateInfo.h
	../../Common/Packets/CGAskPrivateInfo.cpp
	../../Common/Packets/CGAskPrivateInfo.h
	../../Common/Packets/CGSetMoodToHead.cpp
	../../Common/Packets/CGSetMoodToHead.h
	../../Common/Packets/GCPrivateInfo.cpp
	../../Common/Packets/GCPrivateInfo.h
)

SET (COMMON_PACKETS_MSGMEDIATOR_LIST
	../../Common/Packets/MsgMediator/MsgMediator.h
	../../Common/Packets/MsgMediator/MsgMediatorFactory.cpp
	../../Common/Packets/MsgMediator/MsgMediatorFactory.h
	../../Common/Packets/MsgMediator/MsgSkillMediator.cpp
	../../Common/Packets/MsgMediator/MsgSkillMediator.h
)

SET (COMMON_PACKETS_TITLE_LIST
	../../Common/Packets/GWCharUpdateCurTitle.cpp
	../../Common/Packets/GWCharUpdateCurTitle.h
	../../Common/Packets/GWUpdateTitle.cpp
	../../Common/Packets/GWUpdateTitle.h
	../../Common/Packets/WGUpdateTitle.cpp
	../../Common/Packets/WGUpdateTitle.h
)

SET (COMMON_PACKETS_COUNTRY_LIST
	../../Common/Packets/CGCountryAppoint.cpp
	../../Common/Packets/CGCountryAppoint.h
	../../Common/Packets/CGCountryCollect.cpp
	../../Common/Packets/CGCountryCollect.h
	../../Common/Packets/CGCountryDelate.cpp
	../../Common/Packets/CGCountryDelate.h
	../../Common/Packets/CGCountryDonative.cpp
	../../Common/Packets/CGCountryDonative.h
	../../Common/Packets/CGCountryFetch.cpp
	../../Common/Packets/CGCountryFetch.h
	../../Common/Packets/CGCountryInfoRequest.cpp
	../../Common/Packets/CGCountryInfoRequest.h
	../../Common/Packets/CGCountryRemove.cpp
	../../Common/Packets/CGCountryRemove.h
	../../Common/Packets/CGCountryRequestKindom.cpp
	../../Common/Packets/CGCountryRequestKindom.h
	../../Common/Packets/CGCountryVoteOn.cpp
	../../Common/Packets/CGCountryVoteOn.h
	../../Common/Packets/CGModifyPKMode.cpp
	../../Common/Packets/CGModifyPKMode.h
	../../Common/Packets/GCCountryCollect.cpp
	../../Common/Packets/GCCountryCollect.h
	../../Common/Packets/GCCountryDelateBegin.cpp
	../../Common/Packets/GCCountryDelateBegin.h
	../../Common/Packets/GCCountryFetch.cpp
	../../Common/Packets/GCCountryFetch.h
	../../Common/Packets/GCCountryInfoConfirm.cpp
	../../Common/Packets/GCCountryInfoConfirm.h
	../../Common/Packets/GCCountryReturn.cpp
	../../Common/Packets/GCCountryReturn.h
	../../Common/Packets/GCCountryVoteShow.cpp
	../../Common/Packets/GCCountryVoteShow.h
	../../Common/Packets/GWCountryAP.cpp
	../../Common/Packets/GWCountryAP.h
	../../Common/Packets/GWCountryAppoint.cpp
	../../Common/Packets/GWCountryAppoint.h
	../../Common/Packets/GWCountryCollect.cpp
	../../Common/Packets/GWCountryCollect.h
	../../Common/Packets/GWCountryDelate.cpp
	../../Common/Packets/GWCountryDelate.h
	../../Common/Packets/GWCountryDelateRequest.cpp
	../../Common/Packets/GWCountryDelateRequest.h
	../../Common/Packets/GWCountryDonative.cpp
	../../Common/Packets/GWCountryDonative.h
	../../Common/Packets/GWCountryFetch.cpp
	../../Common/Packets/GWCountryFetch.h
	../../Common/Packets/GWCountryInfoRequest.cpp
	../../Common/Packets/GWCountryInfoRequest.h
	../../Common/Packets/GWCountryRemove.cpp
	../../Common/Packets/GWCountryRemove.h
	../../Common/Packets/GWCountryRequestKindom.cpp
	../../Common/Packets/GWCountryRequestKindom.h
	../../Common/Packets/GWCountryVoteOn.cpp
	../../Common/Packets/GWCountryVoteOn.h
	../../Common/Packets/GWCountryVoteRequest.cpp
	../../Common/Packets/GWCountryVoteRequest.h
	../../Common/Packets/WGCountryCollect.cpp
	../../Common/Packets/WGCountryCollect.h
	../../Common/Packets/WGCountryDelateConfirm.cpp
	../../Common/Packets/WGCountryDelateConfirm.h
	../../Common/Packets/WGCountryFetch.cpp
	../../Common/Packets/WGCountryFetch.h
	../../Common/Packets/WGCountryInfoConfirm.cpp
	../../Common/Packets/WGCountryInfoConfirm.h
	../../Common/Packets/WGCountryReturn.cpp
	../../Common/Packets/WGCountryReturn.h
	../../Common/Packets/WGCountryVoteConfirm.cpp
	../../Common/Packets/WGCountryVoteConfirm.h
)

SET (COMMON_PACKETS_TOPLIST_LIST
	../../Common/Packets/CGTopList.cpp
	../../Common/Packets/CGTopList.h
	../../Common/Packets/GCTopList.cpp
	../../Common/Packets/GCTopList.h
	../../Common/Packets/GWTopList.cpp
	../../Common/Packets/GWTopList.h
	../../Common/Packets/WGTopList.cpp
	../../Common/Packets/WGTopList.h
)

SET (COMMON_PACKETS_HORSE_LIST
	../../Common/Packets/CGCancelHorseFindMateInfo.cpp
	../../Common/Packets/CGCancelHorseFindMateInfo.h
	../../Common/Packets/CGIssueHorseFindMateInfo.cpp
	../../Common/Packets/CGIssueHorseFindMateInfo.h
	../../Common/Packets/CGQueryHorseFindMateInfo.cpp
	../../Common/Packets/CGQueryHorseFindMateInfo.h
	../../Common/Packets/CGSearchHorseFindMateInfo.cpp
	../../Common/Packets/CGSearchHorseFindMateInfo.h
	../../Common/Packets/GCReturnHorseFindMateInfo.cpp
	../../Common/Packets/GCReturnHorseFindMateInfo.h
	../../Common/Packets/GWCancelHorseFindMateInfo.cpp
	../../Common/Packets/GWCancelHorseFindMateInfo.h
	../../Common/Packets/GWHorseChangeState.cpp
	../../Common/Packets/GWHorseChangeState.h
	../../Common/Packets/GWIssueHorseFindMateInfo.cpp
	../../Common/Packets/GWIssueHorseFindMateInfo.h
	../../Common/Packets/GWQueryHorseFindMateInfo.cpp
	../../Common/Packets/GWQueryHorseFindMateInfo.h
	../../Common/Packets/GWSearchHorseFindMateInfo.cpp
	../../Common/Packets/GWSearchHorseFindMateInfo.h
	../../Common/Packets/WGReturnHorseFindMateInfo.cpp
	../../Common/Packets/WGReturnHorseFindMateInfo.h
)

SET (COMMON_PACKETS_ONLINEUSER_LIST
	../../Common/Packets/CGSearchOnlineUser.cpp
	../../Common/Packets/CGSearchOnlineUser.h
	../../Common/Packets/GCReturnOnlineUser.cpp
	../../Common/Packets/GCReturnOnlineUser.h
	../../Common/Packets/GWSearchOnlineUser.cpp
	../../Common/Packets/GWSearchOnlineUser.h
	../../Common/Packets/WGReturnOnlineUser.cpp
	../../Common/Packets/WGReturnOnlineUser.h
)

SET (COMMON_PACKETS_SOUXIA_LIST
	../../Common/Packets/CGSkillUpgradeBySouXia.cpp
	../../Common/Packets/CGSkillUpgradeBySouXia.h
	../../Common/Packets/CGSouXiaDiscard.cpp
	../../Common/Packets/CGSouXiaDiscard.h
	../../Common/Packets/CGSouXiaDismantle.cpp
	../../Common/Packets/CGSouXiaDismantle.h
	../../Common/Packets/CGSouXiaRefining.cpp
	../../Common/Packets/CGSouXiaRefining.h
	../../Common/Packets/CGSouXiaSew.cpp
	../../Common/Packets/CGSouXiaSew.h
	../../Common/Packets/GCSkillUpgradeBySouXia.cpp
	../../Common/Packets/GCSkillUpgradeBySouXia.h
	../../Common/Packets/GCSouXiaDiscard.cpp
	../../Common/Packets/GCSouXiaDiscard.h
	../../Common/Packets/GCSouXiaDismantle.cpp
	../../Common/Packets/GCSouXiaDismantle.h
	../../Common/Packets/GCSouXiaList.cpp
	../../Common/Packets/GCSouXiaList.h
	../../Common/Packets/GCSouXiaRefining.cpp
	../../Common/Packets/GCSouXiaRefining.h
	../../Common/Packets/GCSouXiaSew.cpp
	../../Common/Packets/GCSouXiaSew.h
	../../Common/Packets/GCSouXiaSkillUpdate.cpp
	../../Common/Packets/GCSouXiaSkillUpdate.h
	../../Common/Packets/GCSouXiaUpdate.cpp
	../../Common/Packets/GCSouXiaUpdate.h
)

SET (COMMON_PACKETS_BUS_LIST
	../../Common/Packets/GCBusAddPassenger.cpp
	../../Common/Packets/GCBusAddPassenger.h
	../../Common/Packets/GCBusMove.cpp
	../../Common/Packets/GCBusMove.h
	../../Common/Packets/GCBusRemoveAllPassenger.cpp
	../../Common/Packets/GCBusRemoveAllPassenger.h
	../../Common/Packets/GCBusRemovePassenger.cpp
	../../Common/Packets/GCBusRemovePassenger.h
	../../Common/Packets/GCBusStopMove.cpp
	../../Common/Packets/GCBusStopMove.h
	../../Common/Packets/GCNewBus.cpp
	../../Common/Packets/GCNewBus.h
	../../Common/Packets/GCNewBus_Move.cpp
	../../Common/Packets/GCNewBus_Move.h
)

SET (COMMON_PACKETS_LIST
)

SET (COMMON_DATABASE_LIST
	../../Common/DataBase/WXSJ_DBC.cpp
	../../Common/DataBase/WXSJ_DBC.h
)

SET (COMMON_COMBAT_LIST
	../../Common/Combat/CampAndStand.cpp
	../../Common/Combat/CampAndStand.h
	../../Common/Combat/DataRecords.cpp
	../../Common/Combat/DataRecords.h
	../../Common/Combat/TemplateManager.cpp
	../../Common/Combat/TemplateManager.h
)

SET (COMMON_COMMON_LIST
	../../Common/Array_Base.cpp
	../../Common/Array_Base.h
	../../Common/Assertx.cpp
	../../Common/Assertx.h
	../../Common/BaseType.h
	../../Common/DB_Struct.cpp
	../../Common/DB_Struct.h
	../../Common/FileAPI.h
	../../Common/FileDataMgr.cpp
	../../Common/FileDataMgr.h
	../../Common/formula.h
	../../Common/GameDefine.h
	../../Common/GameDefine2.h
	../../Common/GameDefine_Attr.h
	../../Common/GameDefine_City.h
	../../Common/GameDefine_Guild.h
	../../Common/GameDefine_Item.h
	../../Common/GameDefine_PlayerShop.h
	../../Common/GameDefine_Result.cpp
	../../Common/GameDefine_Result.h
	../../Common/GameDefine_Scene.h
	../../Common/GameStruct.cpp
	../../Common/GameStruct.h
	../../Common/GameStruct_Bus.cpp
	../../Common/GameStruct_Bus.h
	../../Common/GameStruct_City.h
	../../Common/GameStruct_Country.h
	../../Common/GameStruct_Finger.cpp
	../../Common/GameStruct_Finger.h
	../../Common/GameStruct_Guild.h
	../../Common/GameStruct_Impact.h
	../../Common/GameStruct_Item.h
	../../Common/GameStruct_MinorPasswd.cpp
	../../Common/GameStruct_MinorPasswd.h
	../../Common/GameStruct_Pet.cpp
	../../Common/GameStruct_Pet.h
	../../Common/GameStruct_Query.h
	../../Common/GameStruct_Relation.cpp
	../../Common/GameStruct_Relation.h
	../../Common/GameStruct_Scene.h
	../../Common/GameStruct_Script.cpp
	../../Common/GameStruct_Script.h
	../../Common/GameStruct_Skill.h
	../../Common/GameStruct_Team.h
	../../Common/GameStruct_TopList.h
	../../Common/GameUtil.cpp
	../../Common/GameUtil.h
	../../Common/Minidump.cpp
	../../Common/Minidump.h
	../../Common/OResultDef.h
	../../Common/Packet.h
	../../Common/PacketDefine.h
	../../Common/PacketFactory.h
	../../Common/PacketFactoryManager.cpp
	../../Common/PacketFactoryManager.h
	../../Common/PlayerStatus.h
	../../Common/Rand.cpp
	../../Common/Rand.h
	../../Common/ResultDefine.h
	../../Common/ScriptCommandStruct.cpp
	../../Common/ScriptCommandStruct.h
	../../Common/ServerSocket.h
	../../Common/ShareMemAPI.cpp
	../../Common/ShareMemAPI.h
	../../Common/Socket.h
	../../Common/SocketAPI.h
	../../Common/SocketInputStream.h
	../../Common/SocketOutputStream.h
	../../Common/Type.h
)

SET (COMMON_UTILITIES_LIST
	../../Common/Utilities/SplitStringBase.cpp
	../../Common/Utilities/SplitStringBase.h
	../../Common/Utilities/SplitStringLevelOne.cpp
	../../Common/Utilities/SplitStringLevelOne.h
	../../Common/Utilities/SplitStringLevelTwo.cpp
	../../Common/Utilities/SplitStringLevelTwo.h
)

SET (COMMON_RELATION_LIST
	../../Common/Packets/CGFinger.cpp
	../../Common/Packets/CGFinger.h
	../../Common/Packets/CGRelation.cpp
	../../Common/Packets/CGRelation.h
	../../Common/Packets/GCFinger.cpp
	../../Common/Packets/GCFinger.h
	../../Common/Packets/GCRelation.cpp
	../../Common/Packets/GCRelation.h
	../../Common/Packets/GWFinger.cpp
	../../Common/Packets/GWFinger.h
	../../Common/Packets/GWRelation.cpp
	../../Common/Packets/GWRelation.h
	../../Common/Packets/WGFinger.cpp
	../../Common/Packets/WGFinger.h
	../../Common/Packets/WGRelation.cpp
	../../Common/Packets/WGRelation.h
)

SET (COMMON_LIST
)

SET (OBJ_VISITOR_LIST
	Obj/Visitor/CharacterVisitor.cpp
	Obj/Visitor/CharacterVisitor.h
	Obj/Visitor/InitMonsterVisitor.cpp
	Obj/Visitor/InitMonsterVisitor.h
)

SET (OBJ_HUMANASSISTANTCLASS_LIST
	Obj/HumanAssistantClass/HumanSkillStudy.cpp
	Obj/HumanAssistantClass/HumanSkillStudy.h
	Obj/HumanAssistantClass/HumanSkillUpgrad.cpp
	Obj/HumanAssistantClass/HumanSkillUpgrad.h
)

SET (OBJ_HORSE_LIST
	Obj/Horse/DropFromMonster.cpp
	Obj/Horse/DropFromMonster.h
	Obj/Horse/HorseAttrCalculater.cpp
	Obj/Horse/HorseAttrCalculater.h
	Obj/Horse/HorseFindMateModule.cpp
	Obj/Horse/HorseFindMateModule.h
	Obj/Horse/HorseManager.cpp
	Obj/Horse/HorseManager.h
	Obj/Horse/HorseMatingRule.cpp
	Obj/Horse/HorseMatingRule.h
	Obj/Horse/HorsePool.cpp
	Obj/Horse/HorsePool.h
	Obj/Horse/HorseStruct.h
	Obj/Horse/Obj_Horse.cpp
	Obj/Horse/Obj_Horse.h
)

SET (OBJ_BUS_LIST
	Obj/BusManager.cpp
	Obj/BusManager.h
	Obj/BusPool.cpp
	Obj/BusPool.h
	Main/BusRule.cpp
	Main/BusRule.h
	Obj/Obj_Bus.cpp
	Obj/Obj_Bus.h
)

SET (OBJ_LIST
	Obj/Attributes.h
	Obj/CharacterManager.cpp
	Obj/CharacterManager.h
	Obj/HumanManager.cpp
	Obj/HumanManager.h
	Obj/ItemBoxManager.cpp
	Obj/ItemBoxManager.h
	Obj/ItemBoxPool.cpp
	Obj/ItemBoxPool.h
	Obj/MonsterManager.cpp
	Obj/MonsterManager.h
	Obj/MonsterPool.cpp
	Obj/MonsterPool.h
	Obj/Obj.cpp
	Obj/Obj.h
	Obj/Obj_Character.cpp
	Obj/Obj_Character.h
	Obj/Obj_Character_Attributes.cpp
	Obj/Obj_Character_DirtyFlag.cpp
	Obj/Obj_Character_Event.cpp
	Obj/Obj_Character_Impact.cpp
	Obj/Obj_Character_Logic.cpp
	Obj/Obj_Character_MenPai.cpp
	Obj/Obj_Character_Skill.cpp
	Obj/Obj_Dynamic.cpp
	Obj/Obj_Dynamic.h
	Obj/Obj_Human.cpp
	Obj/Obj_Human.h
	Obj/Obj_Human.inl
	Obj/Obj_Human_Attributes.cpp
	Obj/Obj_Human_AttrRefesh.cpp
	Obj/Obj_Human_Calc.cpp
	Obj/Obj_Human_Skill.cpp
	Obj/Obj_Item.cpp
	Obj/Obj_Item.h
	Obj/Obj_Item.inl
	Obj/Obj_ItemBox.cpp
	Obj/Obj_ItemBox.h
	Obj/Obj_ItemBox.inl
	Obj/Obj_ItemManager.cpp
	Obj/Obj_ItemManager.h
	Obj/Obj_ItemPool.cpp
	Obj/Obj_ItemPool.h
	Obj/Obj_Monster.cpp
	Obj/Obj_Monster.h
	Obj/Obj_Monster_Attributes.cpp
	Obj/Obj_Monster_AttrRefesh.cpp
	Obj/Obj_Monster_Calc.cpp
	Obj/Obj_Monster_Skill.cpp
	Obj/Obj_Pet.cpp
	Obj/Obj_Pet.h
	Obj/Obj_Pet.inl
	Obj/Obj_Pet_Attributes.cpp
	Obj/Obj_Pet_AttrRefesh.cpp
	Obj/Obj_Pet_Calc.cpp
	Obj/Obj_Pet_Skill.cpp
	Obj/Obj_Platform.cpp
	Obj/Obj_Platform.h
	Obj/Obj_Special.cpp
	Obj/Obj_Special.h
	Obj/Obj_Static.cpp
	Obj/Obj_Static.h
	Obj/ObjList.h
	Obj/ObjManager.cpp
	Obj/ObjManager.h
	Obj/ObjSingleManager.cpp
	Obj/ObjSingleManager.h
	Obj/PetManager.cpp
	Obj/PetManager.h
	Obj/PetPlacard.cpp
	Obj/PetPlacard.h
	Obj/PetPool.cpp
	Obj/PetPool.h
	Obj/PlatformManager.cpp
	Obj/PlatformManager.h
	Obj/PlatformPool.cpp
	Obj/PlatformPool.h
	Obj/SpecialManager.cpp
	Obj/SpecialManager.h
	Obj/SpecialPool.cpp
	Obj/SpecialPool.h
)

SET (BASE_LIST
	Base/Config.cpp
	Base/Config.h
	Base/File.cpp
	Base/File.h
	Base/FileDef.h
	Base/IDTable.cpp
	Base/IDTable.h
	Base/Ini.cpp
	Base/Ini.h
	Base/Log.cpp
	Base/Log.h
	Base/LogDefine.cpp
	Base/LogDefine.h
	Base/ObjPool.cpp
	Base/ObjPool.h
	Base/PetConfig.h
	Base/ScriptDef.h
	Base/StrTable.cpp
	Base/StrTable.h
	Base/Thread.cpp
	Base/Thread.h
	Base/TimeManager.cpp
	Base/TimeManager.h
)

SET (MAIN_LIST
	Main/ClientManager.cpp
	Main/ClientManager.h
	Main/DaemonThread.cpp
	Main/DaemonThread.h
	Main/DiePenaltyManager.cpp
	Main/DiePenaltyManager.h
	Main/GameTable.cpp
	Main/GameTable.h
	Main/GUIDManager.cpp
	Main/GUIDManager.h
	Main/MachineManager.cpp
	Main/MachineManager.h
	Main/Performance.cpp
	Main/Performance.h
	Main/Server.cpp
	Main/Server.h
	Main/ServerManager.cpp
	Main/ServerManager.h
	Main/ServerThread.cpp
	Main/ServerThread.h
	Main/ShareMemManager.cpp
	Main/ShareMemManager.h
	Main/ThreadManager.cpp
	Main/ThreadManager.h
	Main/ThreadPool.cpp
	Main/ThreadPool.h
	Main/TimerDoingManager.cpp
	Main/TimerDoingManager.h
	Main/User.cpp
	Main/User.h
	Main/UserPool.cpp
	Main/UserPool.h
	Main/WorldManager.cpp
	Main/WorldManager.h
)

SET (PACKETS_SKILLSTUDY_LIST
	Packets/CGAskLeanAbilityHandler.cpp
	Packets/CGAskStudyXinfaHandler.cpp
)

SET (PACKETS_TEAM_FOLLOW_LIST
	Packets/CGAskTeamFollowHandler.cpp
	Packets/CGReturnTeamFollowHandler.cpp
	Packets/CGStopTeamFollowHandler.cpp
	Packets/WGTeamFollowListHandler.cpp
)

SET (PACKETS_TEAM_LIST
	Packets/CGAskTeamDistribModeHandler.cpp
	Packets/CGAskTeamInfoHandler.cpp
	Packets/CGAskTeamMemberInfoHandler.cpp
	Packets/CGRegisterTeamRecruitHandler.cpp
	Packets/CGSetTeamDistribModeHandler.cpp
	Packets/CGTeamApplyHandler.cpp
	Packets/CGTeamAppointHandler.cpp
	Packets/CGTeamDismissHandler.cpp
	Packets/CGTeamInviteHandler.cpp
	Packets/CGTeamKickHandler.cpp
	Packets/CGTeamLeaderRetInviteHandler.cpp
	Packets/CGTeamLeaveHandler.cpp
	Packets/CGTeamRetApplyHandler.cpp
	Packets/CGTeamRetInviteHandler.cpp
	Packets/WGRetTeamDistribModeHandler.cpp
	Packets/WGRetTeamRecruitInfoHandler.cpp
	Packets/WGTeamAskApplyHandler.cpp
	Packets/WGTeamAskInviteHandler.cpp
	Packets/WGTeamErrorHandler.cpp
	Packets/WGTeamLeaderAskInviteHandler.cpp
	Packets/WGTeamListHandler.cpp
	Packets/WGTeamMemberInfoHandler.cpp
	Packets/WGTeamResultHandler.cpp
)

SET (PACKETS_STALL_BBS_LIST
	Packets/CGBBSApplyHandler.cpp
	Packets/CGBBSSychMessagesHandler.cpp
)

SET (PACKETS_STALL_LIST
	Packets/CGStallAddItemHandler.cpp
	Packets/CGStallApplyHandler.cpp
	Packets/CGStallBuyHandler.cpp
	Packets/CGStallCloseHandler.cpp
	Packets/CGStallDefaultPageHandler.cpp
	Packets/CGStallEstablishHandler.cpp
	Packets/CGStallItemPriceHandler.cpp
	Packets/CGStallOpenHandler.cpp
	Packets/CGStallOpenPublicHandler.cpp
	Packets/CGStallRemoveItemHandler.cpp
	Packets/CGStallShopNameHandler.cpp
)

SET (PACKETS_ITEM_LIST
	Packets/CGAskLockObjHandler.cpp
	Packets/CGItemSynchHandler.cpp
	Packets/CGPackUpPacketHandler.cpp
	Packets/CGSplitItemHandler.cpp
)

SET (PACKETS_PLAYERSHOP_LIST
	Packets/CGPlayerShopAcquireItemListHandler.cpp
	Packets/CGPlayerShopAcquireShopListHandler.cpp
	Packets/CGPlayerShopApplyHandler.cpp
	Packets/CGPlayerShopAskForRecordHandler.cpp
	Packets/CGPlayerShopBuyItemHandler.cpp
	Packets/CGPlayerShopBuyShopHandler.cpp
	Packets/CGPlayerShopDescHandler.cpp
	Packets/CGPlayerShopEstablishHandler.cpp
	Packets/CGPlayerShopFavoriteHandler.cpp
	Packets/CGPlayerShopMoneyHandler.cpp
	Packets/CGPlayerShopNameHandler.cpp
	Packets/CGPlayerShopOnSaleHandler.cpp
	Packets/CGPlayerShopOpenStallHandler.cpp
	Packets/CGPlayerShopPartnerHandler.cpp
	Packets/CGPlayerShopSaleOutHandler.cpp
	Packets/CGPlayerShopSizeHandler.cpp
	Packets/CGPlayerShopTypeHandler.cpp
)

SET (PACKETS_EXCHANGE_LIST
	Packets/CGExchangeApplyIHandler.cpp
	Packets/CGExchangeCancelHandler.cpp
	Packets/CGExchangeOkIIIHandler.cpp
	Packets/CGExchangeReplyIHandler.cpp
	Packets/CGExchangeSynchItemIIHandler.cpp
	Packets/CGExchangeSynchLockHandler.cpp
	Packets/CGExchangeSynchMoneyIIHandler.cpp
)

SET (PACKETS_CITY_LIST
	Packets/CGCityAskAttrHandler.cpp
	Packets/WGCityApplyNewCityHandler.cpp
	Packets/WGCityAttrHandler.cpp
	Packets/WGCityCloseHandler.cpp
	Packets/WGCityCloseSuccessHandler.cpp
	Packets/WGCityErrorHandler.cpp
	Packets/WGCityInitInfoHandler.cpp
)

SET (PACKETS_PRIVATEINFO_LIST
	Packets/CGApplyPirvateInfoHandler.cpp
	Packets/CGAskPrivateInfoHandler.cpp
	Packets/CGSetMoodToHeadHandler.cpp
)

SET (PACKETS_GUILD_LIST
	Packets/CGGuildApplyHandler.cpp
	Packets/CGGuildHandler.cpp
	Packets/CGGuildJoinHandler.cpp
	Packets/WGGuildErrorHandler.cpp
	Packets/WGGuildHandler.cpp
	Packets/WGGuildReturnHandler.cpp
)

SET (PACKETS_TOPLIST_LIST
	Packets/CGTopListHandler.cpp
	Packets/WGTopListHandler.cpp
)

SET (PACKETS_HORSE_LIST
	Packets/CGCancelHorseFindMateInfoHandler.cpp
	Packets/CGIssueHorseFindMateInfoHandler.cpp
	Packets/CGQueryHorseFindMateInfoHandler.cpp
	Packets/CGSearchHorseFindMateInfoHandler.cpp
	Packets/WGReturnHorseFindMateInfoHandler.cpp
)

SET (PACKETS_COUNTRY_LIST
	Packets/CGCountryAppointHandler.cpp
	Packets/CGCountryCollectHandler.cpp
	Packets/CGCountryDelateHandler.cpp
	Packets/CGCountryDonativeHandler.cpp
	Packets/CGCountryFetchHandler.cpp
	Packets/CGCountryInfoRequestHandler.cpp
	Packets/CGCountryRemoveHandler.cpp
	Packets/CGCountryRequestKindomHandler.cpp
	Packets/CGCountryVoteOnHandler.cpp
	Packets/WGCountryCollectHandler.cpp
	Packets/WGCountryDelateConfirmHandler.cpp
	Packets/WGCountryFetchHandler.cpp
	Packets/WGCountryInfoConfirmHandler.cpp
	Packets/WGCountryReturnHandler.cpp
	Packets/WGCountryVoteConfirmHandler.cpp
)

SET (PACKETS_LIST
	Packets/CGAskBeginSubTrainHandler.cpp
	Packets/CGAskChangeSceneHandler.cpp
	Packets/CGAskDetailAbilityInfoHandler.cpp
	Packets/CGAskDetailAttribHandler.cpp
	Packets/CGAskDetailEquipListHandler.cpp
	Packets/CGAskDetailSkillListHandler.cpp
	Packets/CGAskDetaiXinFaListHandler.cpp
	Packets/CGAskEndSubTrainHandler.cpp
	Packets/CGAskGetSubTrainExpHandler.cpp
	Packets/CGAskItemInfoHandler.cpp
	Packets/CGAskJoinMenpaiHandler.cpp
	Packets/CGAskMailHandler.cpp
	Packets/CGAskMissionListHandler.cpp
	Packets/CGAskMyBagListHandler.cpp
	Packets/CGAskOtherOnlineDetailAttribHandler.cpp
	Packets/CGAskSettingHandler.cpp
	Packets/CGAskSubTrainModeHandler.cpp
	Packets/CGAskSubTrainSettingHandler.cpp
	Packets/CGAttackHandler.cpp
	Packets/CGBankAcquireListHandler.cpp
	Packets/CGBankAddItemHandler.cpp
	Packets/CGBankCloseHandler.cpp
	Packets/CGBankMoneyHandler.cpp
	Packets/CGBankRemoveItemHandler.cpp
	Packets/CGBankSwapItemHandler.cpp
	Packets/CGChannelCreateHandler.cpp
	Packets/CGChannelDismissHandler.cpp
	Packets/CGChannelInviteHandler.cpp
	Packets/CGChannelKickHandler.cpp
	Packets/CGCharAllTitlesHandler.cpp
	Packets/CGCharAskBaseAttribHandler.cpp
	Packets/CGCharAskEquipmentHandler.cpp
	Packets/CGCharDefaultEventHander.cpp
	Packets/CGCharIdleHandler.cpp
	Packets/CGCharJumpHandler.cpp
	Packets/CGCharMoodStateHandler.cpp
	Packets/CGCharMoveHandler.cpp
	Packets/CGCharPositionWarpHandler.cpp
	Packets/CGCharSitHandler.cpp
	Packets/CGCharStopLogicHandler.cpp
	Packets/CGCharUpdateCurTitleHandler.cpp
	Packets/CGCharUseSkillHandler.cpp
	Packets/CGChatHandler.cpp
	Packets/CGCleanSubTrainInfoHandler.cpp
	Packets/CGCommandHandler.cpp
	Packets/CGConnectHandler.cpp
	Packets/CGDiscardEquipHandler.cpp
	Packets/CGDiscardItemHandler.cpp
	Packets/CGDispelBuffReqHandler.cpp
	Packets/CGDoEventHandler.cpp
	Packets/CGEnterSceneHandler.cpp
	Packets/CGEnvRequestHandler.cpp
	Packets/CGEquipAddHoleHandler.cpp
	Packets/CGEquipAdsorbMagicHandler.cpp
	Packets/CGEquipBindHandler.cpp
	Packets/CGEquipCancelMagicHandler.cpp
	Packets/CGEquipChangePaiTypeHandler.cpp
	Packets/CGEquipDecompoundHandler.cpp
	Packets/CGEquipExChangePaiTypeHandler.cpp
	Packets/CGEquipLevelupHandler.cpp
	Packets/CGEquipRemouldHandler.cpp
	Packets/CGEquipSuitExchangeHandler.cpp
	Packets/CGEqupRefiningHandler.cpp
	Packets/CGEventRequestHander.cpp
	Packets/CGExecuteScriptHandler.cpp
	Packets/CGFingerHandler.cpp
	Packets/CGGemCompoundHandler.cpp
	Packets/CGHeartBeatHandler.cpp
	Packets/CGIssuePetPlacardHandler.cpp
	Packets/CGItemCreateHandler.cpp
	Packets/CGLeaveSceneHandler.cpp
	Packets/CGLockTargetHandler.cpp
	Packets/CGMailHandler.cpp
	Packets/CGManipulatePetHandler.cpp
	Packets/CGMapInfoHandler.cpp
	Packets/CGMinorPasswordHandler.cpp
	Packets/CGMissionAbandonHander.cpp
	Packets/CGMissionAcceptHander.cpp
	Packets/CGMissionCheckHandler.cpp
	Packets/CGMissionContinueHander.cpp
	Packets/CGMissionRefuseHander.cpp
	Packets/CGMissionSubmitHander.cpp
	Packets/CGModifyPKModeHandler.cpp
	Packets/CGModifySettingHandler.cpp
	Packets/CGOpenBoxHandler.cpp
	Packets/CGOpenBoxManipulateHandler.cpp
	Packets/CGOpenItemBoxHander.cpp
	Packets/CGOtherEquipHandler.cpp
	Packets/CGPackage_SwipItemHandle.cpp
	Packets/CGPickBoxItemHandler.cpp
	Packets/CGPickDropItemHandler.cpp
	Packets/CGPlayerCallOfResultHandler.cpp
	Packets/CGPlayerDieResultHandler.cpp
	Packets/CGRelationHandler.cpp
	Packets/CGRemoveGemHandler.cpp
	Packets/CGReqLevelUpHandler.cpp
	Packets/CGReqManualAttrHandler.cpp
	Packets/CGReqResetAttrHandler.cpp
	Packets/CGSaveEquipSuitHandler.cpp
	Packets/CGSearchOnlineUserHandler.cpp
	Packets/CGSetPetAttribHandler.cpp
	Packets/CGShopBuyHandler.cpp
	Packets/CGShopCloseHandler.cpp
	Packets/CGShopRepairHandler.cpp
	Packets/CGShopSellHandler.cpp
	Packets/CGShopSpecialRepairHandler.cpp
	Packets/CGSkillHandler.cpp
	Packets/CGSkillUpgradeBySouXiaHandler.cpp
	Packets/CGSkillUpgradeHandler.cpp
	Packets/CGSouXiaDiscardHandler.cpp
	Packets/CGSouXiaDismantleHandler.cpp
	Packets/CGSouXiaRefiningHandler.cpp
	Packets/CGSouXiaSewHandler.cpp
	Packets/CGTestHandler.cpp
	Packets/CGUnEquipHandler.cpp
	Packets/CGUnExtraBagHandler.cpp
	Packets/CGUseAbilityHandler.cpp
	Packets/CGUseEquipHandler.cpp
	Packets/CGUseExtraBagHandler.cpp
	Packets/CGUseGemHandler.cpp
	Packets/CGUseItemHandler.cpp
	Packets/CGUseSectorSkillHandler.cpp
	Packets/GCEquipAdsorbMagicResultHandler.cpp
	Packets/GCEquipCancelMagicResultHandler.cpp
	Packets/GCHandler.cpp
	Packets/GGSceneNotifyHandler.cpp
	Packets/GWHandler.cpp
	Packets/SSConnectHandler.cpp
	Packets/SSScenePlayerCountHandler.cpp
	Packets/WGAskOtherOnlineAttrib_HumanHandler.cpp
	Packets/WGCallOfHumanHandler.cpp
	Packets/WGChannelErrorHandler.cpp
	Packets/WGChannelResultHandler.cpp
	Packets/WGChatHandler.cpp
	Packets/WGCountryInfoConfirmHandler.cpp
	Packets/WGCountryReturnHandler.cpp
	Packets/WGDetailOtherOnlineAttrib_HorseHandler.cpp
	Packets/WGDetailOtherOnlineAttrib_HumanHandler.cpp
	Packets/WGDetailOtherOnlineEquipList_HumanHandler.cpp
	Packets/WGFingerHandler.cpp
	Packets/WGMailHandler.cpp
	Packets/WGNotifyMailHandler.cpp
	Packets/WGNotifyUserHandler.cpp
	Packets/WGOperateResultHandler.cpp
	Packets/WGRelationHandler.cpp
	Packets/WGRetChangeSceneHandler.cpp
	Packets/WGRetSceneDataHandler.cpp
	Packets/WGReturnOnlineUserHandler.cpp
	Packets/WGRetUserDataHandler.cpp
	Packets/WGUpdateTitleHandler.cpp
	Packets/WGWorldTimeHandler.cpp
)

SET (PLAYER_LIST
	Player/GamePlayer.cpp
	Player/GamePlayer.h
	Player/IncomingPlayerManager.cpp
	Player/IncomingPlayerManager.h
	Player/Player.cpp
	Player/Player.h
	Player/PlayerManager.cpp
	Player/PlayerManager.h
	Player/PlayerPool.cpp
	Player/PlayerPool.h
	Player/RecyclePlayerManager.cpp
	Player/RecyclePlayerManager.h
	Player/ServerPlayer.cpp
	Player/ServerPlayer.h
)

SET (SCRIPT_LIST
	Script/LuaFn_OpenBox.h
	Script/LuaFnMacro.h
	Script/LuaFnRegister.cpp
	Script/LuaFnRegister.h
	Script/LuaFnTbl_Ability.h
	Script/LuaFnTbl_Attr.h
	Script/LuaFnTbl_Battle.h
	Script/LuaFnTbl_Bus.h
	Script/LuaFnTbl_City.h
	Script/LuaFnTbl_DoAction.h
	Script/LuaFnTbl_Guild.h
	Script/LuaFnTbl_Misc.h
	Script/LuaFnTbl_Mission.h
	Script/LuaFnTbl_Pet.h
	Script/LuaFnTbl_PetPlacard.h
	Script/LuaFnTbl_Relation.h
	Script/LuaFnTbl_Scene.h
	Script/LuaFnTbl_Shop.h
	Script/LuaFnTbl_Team.h
	Script/LuaInterface.cpp
	Script/LuaInterface.h
	Script/MisBuf.h
	Script/SceneTimer.cpp
	Script/SceneTimer.h
)

SET (ITEM_GROWPOINT_LIST
	Item/GrowPointManager.cpp
	Item/GrowPointManager.h
	Item/GrowPointManager.inl
)

SET (ITEM_ITEMWRAP_LIST
	Item/HumanItemContainer.cpp
	Item/HumanItemContainer.h
	Item/HumanItemExtraBag.cpp
	Item/HumanItemExtraBag.h
	Item/HumanItemExtraContainer.cpp
	Item/HumanItemExtraContainer.h
	Item/HumanPetContainer.cpp
	Item/HumanPetContainer.h
	Item/Item.cpp
	Item/Item.h
	Item/Item.inl
	Item/ItemContainer.cpp
	Item/ItemContainer.h
	Item/ItemOperator.cpp
	Item/ItemOperator.h
)

SET (ITEM_ITEMBASE_LIST
	Item/ItemHelper.cpp
	Item/ItemHelper.h
	Item/ItemManager.cpp
	Item/ItemManager.h
	Item/ItemRuler.cpp
	Item/ItemRuler.h
	Item/ItemTable.cpp
	Item/ItemTable.h
	Item/ItemTypes.h
)

SET (ITEM_ITEMLOGIC_LIST
	Item/HumanItemBagLogic.cpp
	Item/HumanItemBankLogic.cpp
	Item/HumanItemEquipLogic.cpp
	Item/HumanItemExtraLogic.cpp
	Item/HumanItemLogic.cpp
	Item/HumanItemLogic.h
)

SET (ITEM_LIST
)

SET (SMU_LIST
	SMU/ShareMemAO.cpp
	SMU/ShareMemAO.h
	SMU/SMUManager.cpp
	SMU/SMUManager.h
)

SET (TEAM_LIST
	Team/Team.cpp
	Team/Team.h
	Team/Team.inl
)

SET (MENPAI_MISSION_LIST
	Mission/Human_Mission.cpp
	Mission/Human_Mission.h
	Mission/MissionManager.cpp
	Mission/MissionManager.h
)

SET (MENPAI_LIST
	MenPai/EMeiCEM.cpp
	MenPai/EMeiCEM.h
	MenPai/EMeiFEM.cpp
	MenPai/EMeiFEM.h
	MenPai/GaiBangGGB.cpp
	MenPai/GaiBangGGB.h
	MenPai/GaiBangZGB.cpp
	MenPai/GaiBangZGB.h
	MenPai/MenPai.cpp
	MenPai/MenPai.h
	MenPai/MenPais.h
	MenPai/ShaoLinGWS.cpp
	MenPai/ShaoLinGWS.h
	MenPai/ShaoLinTLH.cpp
	MenPai/ShaoLinTLH.h
	MenPai/TangMenDT.cpp
	MenPai/TangMenDT.h
	MenPai/TangMenXJT.cpp
	MenPai/TangMenXJT.h
	MenPai/TaoHuaGuDTH.cpp
	MenPai/TaoHuaGuDTH.h
	MenPai/TaoHuaGuYTH.cpp
	MenPai/TaoHuaGuYTH.h
	MenPai/WuDangJWD.cpp
	MenPai/WuDangJWD.h
	MenPai/WuDangQWD.cpp
	MenPai/WuDangQWD.h
	MenPai/WuMenPai.cpp
	MenPai/WuMenPai.h
	MenPai/XiaoYaoXXY.cpp
	MenPai/XiaoYaoXXY.h
	MenPai/XiaoYaoZXY.cpp
	MenPai/XiaoYaoZXY.h
	MenPai/YaoWangGuKYW.cpp
	MenPai/YaoWangGuKYW.h
	MenPai/YaoWangGuYYW.cpp
	MenPai/YaoWangGuYYW.h
)

SET (AI_LIST
	AI/AI_Character.cpp
	AI/AI_Character.h
	AI/AI_Horse.cpp
	AI/AI_Horse.h
	AI/AI_HorseDef.h
	AI/AI_Human.cpp
	AI/AI_Human.h
	AI/AI_Monster.cpp
	AI/AI_Monster.h
	AI/AI_Monster_Def.h
	AI/AI_Pet.cpp
	AI/AI_Pet.h
	AI/AI_PetDef.h
	AI/AIScript.cpp
	AI/AIScript.h
	AI/AIScriptDef.h
	AI/AIScriptFunction.cpp
	AI/AIScriptFunction.h
	AI/AvoidOverlap.h
	AI/Patrol.cpp
	AI/Patrol.h
	AI/SinCos.cpp
	AI/SinCos.h
	AI/State.cpp
	AI/State.h
)

SET (OTHER_PLAYERSHOP_LIST
	Other/PlayerShop.cpp
	Other/PlayerShop.h
	Other/PlayerShopDefine.h
	Other/PlayerStallBox.cpp
	Other/PlayerStallBox.h
	Other/Record.h
	Other/RecordOpt.cpp
	Other/RecordOpt.h
)

SET (OTHER_SOUXIA_LIST
	Other/SouXia.cpp
	Other/SouXia.h
	Other/SouXiaLogic.cpp
	Other/SouXiaLogic.h
)

SET (OTHER_LIST
	Other/BBS.cpp
	Other/BBS.h
	Other/DoubleMoodAction.cpp
	Other/DoubleMoodAction.h
	Other/ExchangBox.cpp
	Other/ExchangBox.h
	Other/HumanOpenBox.cpp
	Other/HumanOpenBox.h
	Other/HumanRelation.cpp
	Other/HumanRelation.h
	Other/OpenBox.cpp
	Other/OpenBox.h
	Other/ShopManager.cpp
	Other/ShopManager.h
	Other/StallBox.cpp
	Other/StallBox.h
	Other/StallInfoManager.cpp
	Other/StallInfoManager.h
	Other/StoreManager.cpp
	Other/StoreManager.h
)

SET (ABILITY_LIST
	Ability/Ability.cpp
	Ability/Ability.h
	Ability/Ability.inl
	Ability/AbilityCompound.cpp
	Ability/AbilityCompound.h
	Ability/AbilityCooking.cpp
	Ability/AbilityCooking.h
	Ability/AbilityFoundry.cpp
	Ability/AbilityFoundry.h
	Ability/AbilityGather.cpp
	Ability/AbilityGather.h
	Ability/AbilityGatherFish.cpp
	Ability/AbilityGatherFish.h
	Ability/AbilityGeneric.h
	Ability/AbilityInlay.cpp
	Ability/AbilityInlay.h
	Ability/AbilityLevelUpConfig.cpp
	Ability/AbilityLevelUpConfig.h
	Ability/AbilityMachining.cpp
	Ability/AbilityMachining.h
	Ability/AbilityManager.cpp
	Ability/AbilityManager.h
	Ability/AbilityTailor.cpp
	Ability/AbilityTailor.h
)

SET (EVENTMODULE_LIST
	EventModule/EventCore.cpp
	EventModule/EventCore.h
	EventModule/EventStruct.cpp
	EventModule/EventStruct.h
)

SET (ACTIONMODULE_LIST
	ActionModule/ActionCallBack.cpp
	ActionModule/ActionCallBack.h
	ActionModule/ActionCallBackForScript.cpp
	ActionModule/ActionCallBackForScript.h
	ActionModule/ActionCallBackForSkill.cpp
	ActionModule/ActionCallBackForSkill.h
	ActionModule/ActionDelegator.cpp
	ActionModule/ActionDelegator.h
	ActionModule/ActionLogic.cpp
	ActionModule/ActionLogic.h
	ActionModule/ChannelAction.cpp
	ActionModule/ChannelAction.h
	ActionModule/ChargeAction.cpp
	ActionModule/ChargeAction.h
	ActionModule/InstantAction.cpp
	ActionModule/InstantAction.h
)

SET (SCENE_SCAN_LIST
	Scene/Scan/ScanOperator_ActiveTeammates.cpp
	Scene/Scan/ScanOperator_ActiveTeammates.h
	Scene/Scan/ScanOperator_AEImpact.cpp
	Scene/Scan/ScanOperator_AEImpact.h
	Scene/Scan/ScanOperator_AESkill.cpp
	Scene/Scan/ScanOperator_AESkill.h
	Scene/Scan/ScanOperator_Chat.cpp
	Scene/Scan/ScanOperator_Chat.h
	Scene/Scan/ScanOperator_Enemy.cpp
	Scene/Scan/ScanOperator_Enemy.h
	Scene/Scan/ScanOperator_SectorSkill.cpp
	Scene/Scan/ScanOperator_SectorSkill.h
	Scene/Scan/ScanOperator_Teammate.cpp
	Scene/Scan/ScanOperator_Teammate.h
	Scene/Scan/ScanOpt_SObj_ScanChar.cpp
	Scene/Scan/ScanOpt_SObj_ScanChar.h
	Scene/Scan/ScanOpt_SObj_ScanTrap.cpp
	Scene/Scan/ScanOpt_SObj_ScanTrap.h
)

SET (SCENE_LIST
	Scene/AreaManager.cpp
	Scene/AreaManager.h
	Scene/ChatPipe.cpp
	Scene/ChatPipe.h
	Scene/CopySceneManager.cpp
	Scene/CopySceneManager.h
	Scene/DynamicSceneManager.cpp
	Scene/DynamicSceneManager.h
	Scene/Map.cpp
	Scene/Map.h
	Scene/MapUnit.h
	Scene/PathFinder.cpp
	Scene/PathFinder.h
	Scene/ScanOperator.cpp
	Scene/ScanOperator.h
	Scene/Scene.cpp
	Scene/Scene.h
	Scene/Scene.inl
	Scene/Scene_Core.cpp
	Scene/Scene_Event.cpp
	Scene/SceneDropPosManager.cpp
	Scene/SceneDropPosManager.h
	Scene/SceneFile.h
	Scene/SceneManager.cpp
	Scene/SceneManager.h
	Scene/ScenePlayerManager.cpp
	Scene/ScenePlayerManager.h
	Scene/SceneScan.cpp
	Scene/SceneThread.cpp
	Scene/SceneThread.h
	Scene/Zone.cpp
	Scene/Zone.h
)

SET (SKILL_IMPACTLOGIC_LIST
	Skills/ImpactLogic/ImpactLogic.cpp
	Skills/ImpactLogic/ImpactLogic.h
	Skills/ImpactLogic/StdImpact000.cpp
	Skills/ImpactLogic/StdImpact000.h
	Skills/ImpactLogic/StdImpact001.cpp
	Skills/ImpactLogic/StdImpact001.h
	Skills/ImpactLogic/StdImpact002.cpp
	Skills/ImpactLogic/StdImpact002.h
	Skills/ImpactLogic/StdImpact003.cpp
	Skills/ImpactLogic/StdImpact003.h
	Skills/ImpactLogic/StdImpact004.cpp
	Skills/ImpactLogic/StdImpact004.h
	Skills/ImpactLogic/StdImpact005.cpp
	Skills/ImpactLogic/StdImpact005.h
	Skills/ImpactLogic/StdImpact006.cpp
	Skills/ImpactLogic/StdImpact006.h
	Skills/ImpactLogic/StdImpact007.cpp
	Skills/ImpactLogic/StdImpact007.h
	Skills/ImpactLogic/StdImpact008.cpp
	Skills/ImpactLogic/StdImpact008.h
	Skills/ImpactLogic/StdImpact009.cpp
	Skills/ImpactLogic/StdImpact009.h
	Skills/ImpactLogic/StdImpact010.cpp
	Skills/ImpactLogic/StdImpact010.h
	Skills/ImpactLogic/StdImpact011.cpp
	Skills/ImpactLogic/StdImpact011.h
	Skills/ImpactLogic/StdImpact012.cpp
	Skills/ImpactLogic/StdImpact012.h
	Skills/ImpactLogic/StdImpact013.cpp
	Skills/ImpactLogic/StdImpact013.h
	Skills/ImpactLogic/StdImpact014.cpp
	Skills/ImpactLogic/StdImpact014.h
	Skills/ImpactLogic/StdImpact015.cpp
	Skills/ImpactLogic/StdImpact015.h
	Skills/ImpactLogic/StdImpact016.cpp
	Skills/ImpactLogic/StdImpact016.h
	Skills/ImpactLogic/StdImpact017.cpp
	Skills/ImpactLogic/StdImpact017.h
	Skills/ImpactLogic/StdImpact018.cpp
	Skills/ImpactLogic/StdImpact018.h
	Skills/ImpactLogic/StdImpact019.cpp
	Skills/ImpactLogic/StdImpact019.h
	Skills/ImpactLogic/StdImpact020.cpp
	Skills/ImpactLogic/StdImpact020.h
	Skills/ImpactLogic/StdImpact021.cpp
	Skills/ImpactLogic/StdImpact021.h
	Skills/ImpactLogic/StdImpact022.cpp
	Skills/ImpactLogic/StdImpact022.h
	Skills/ImpactLogic/StdImpact023.cpp
	Skills/ImpactLogic/StdImpact023.h
	Skills/ImpactLogic/StdImpact024.cpp
	Skills/ImpactLogic/StdImpact024.h
	Skills/ImpactLogic/StdImpact025.cpp
	Skills/ImpactLogic/StdImpact025.h
	Skills/ImpactLogic/StdImpact026.cpp
	Skills/ImpactLogic/StdImpact026.h
	Skills/ImpactLogic/StdImpact027.cpp
	Skills/ImpactLogic/StdImpact027.h
	Skills/ImpactLogic/StdImpact028.cpp
	Skills/ImpactLogic/StdImpact028.h
	Skills/ImpactLogic/StdImpact029.cpp
	Skills/ImpactLogic/StdImpact029.h
	Skills/ImpactLogic/StdImpact030.cpp
	Skills/ImpactLogic/StdImpact030.h
	Skills/ImpactLogic/StdImpact031.cpp
	Skills/ImpactLogic/StdImpact031.h
	Skills/ImpactLogic/StdImpact032.cpp
	Skills/ImpactLogic/StdImpact032.h
	Skills/ImpactLogic/StdImpact033.cpp
	Skills/ImpactLogic/StdImpact033.h
	Skills/ImpactLogic/StdImpact034.cpp
	Skills/ImpactLogic/StdImpact034.h
	Skills/ImpactLogic/StdImpact035.cpp
	Skills/ImpactLogic/StdImpact035.h
	Skills/ImpactLogic/StdImpact036.cpp
	Skills/ImpactLogic/StdImpact036.h
	Skills/ImpactLogic/StdImpact037.cpp
	Skills/ImpactLogic/StdImpact037.h
	Skills/ImpactLogic/StdImpact038.cpp
	Skills/ImpactLogic/StdImpact038.h
	Skills/ImpactLogic/StdImpact039.cpp
	Skills/ImpactLogic/StdImpact039.h
	Skills/ImpactLogic/StdImpact040.cpp
	Skills/ImpactLogic/StdImpact040.h
	Skills/ImpactLogic/StdImpact041.cpp
	Skills/ImpactLogic/StdImpact041.h
	Skills/ImpactLogic/StdImpact042.cpp
	Skills/ImpactLogic/StdImpact042.h
	Skills/ImpactLogic/StdImpact043.cpp
	Skills/ImpactLogic/StdImpact043.h
	Skills/ImpactLogic/StdImpact044.cpp
	Skills/ImpactLogic/StdImpact044.h
	Skills/ImpactLogic/StdImpact045.cpp
	Skills/ImpactLogic/StdImpact045.h
	Skills/ImpactLogic/StdImpact046.cpp
	Skills/ImpactLogic/StdImpact046.h
	Skills/ImpactLogic/StdImpact047.cpp
	Skills/ImpactLogic/StdImpact047.h
	Skills/ImpactLogic/StdImpact048.cpp
	Skills/ImpactLogic/StdImpact048.h
	Skills/ImpactLogic/StdImpact049.cpp
	Skills/ImpactLogic/StdImpact049.h
	Skills/ImpactLogic/StdImpact050.cpp
	Skills/ImpactLogic/StdImpact050.h
	Skills/ImpactLogic/StdImpact051.cpp
	Skills/ImpactLogic/StdImpact051.h
	Skills/ImpactLogic/StdImpact052.cpp
	Skills/ImpactLogic/StdImpact052.h
	Skills/ImpactLogic/StdImpact053.cpp
	Skills/ImpactLogic/StdImpact053.h
	Skills/ImpactLogic/StdImpact054.cpp
	Skills/ImpactLogic/StdImpact054.h
	Skills/ImpactLogic/StdImpact055.cpp
	Skills/ImpactLogic/StdImpact055.h
	Skills/ImpactLogic/StdImpact056.cpp
	Skills/ImpactLogic/StdImpact056.h
	Skills/ImpactLogic/StdImpact057.cpp
	Skills/ImpactLogic/StdImpact057.h
	Skills/ImpactLogic/StdImpact058.cpp
	Skills/ImpactLogic/StdImpact058.h
	Skills/ImpactLogic/StdImpact059.cpp
	Skills/ImpactLogic/StdImpact059.h
	Skills/ImpactLogic/StdImpact061.cpp
	Skills/ImpactLogic/StdImpact061.h
	Skills/ImpactLogic/StdImpact062.cpp
	Skills/ImpactLogic/StdImpact062.h
	Skills/ImpactLogic/StdImpact063.cpp
	Skills/ImpactLogic/StdImpact063.h
	Skills/ImpactLogic/StdImpact064.cpp
	Skills/ImpactLogic/StdImpact064.h
	Skills/ImpactLogic/StdImpact065.cpp
	Skills/ImpactLogic/StdImpact065.h
	Skills/ImpactLogic/StdImpact066.cpp
	Skills/ImpactLogic/StdImpact066.h
	Skills/ImpactLogic/StdImpact067.cpp
	Skills/ImpactLogic/StdImpact067.h
	Skills/ImpactLogic/StdImpact068.cpp
	Skills/ImpactLogic/StdImpact068.h
	Skills/ImpactLogic/StdImpact069.cpp
	Skills/ImpactLogic/StdImpact069.h
	Skills/ImpactLogic/StdImpact070.cpp
	Skills/ImpactLogic/StdImpact070.h
	Skills/ImpactLogic/StdImpact071.cpp
	Skills/ImpactLogic/StdImpact071.h
	Skills/ImpactLogic/StdImpact072.cpp
	Skills/ImpactLogic/StdImpact072.h
	Skills/ImpactLogic/StdImpact073.cpp
	Skills/ImpactLogic/StdImpact073.h
	Skills/ImpactLogic/StdImpact074.cpp
	Skills/ImpactLogic/StdImpact074.h
	Skills/ImpactLogic/StdImpact075.cpp
	Skills/ImpactLogic/StdImpact075.h
	Skills/ImpactLogic/StdImpact076.cpp
	Skills/ImpactLogic/StdImpact076.h
	Skills/ImpactLogic/StdImpact077.cpp
	Skills/ImpactLogic/StdImpact077.h
	Skills/ImpactLogic/StdImpact078.cpp
	Skills/ImpactLogic/StdImpact078.h
	Skills/ImpactLogic/StdImpact079.cpp
	Skills/ImpactLogic/StdImpact079.h
	Skills/ImpactLogic/StdImpact080.cpp
	Skills/ImpactLogic/StdImpact080.h
	Skills/ImpactLogic/StdImpact081.cpp
	Skills/ImpactLogic/StdImpact081.h
	Skills/ImpactLogic/StdImpact082.cpp
	Skills/ImpactLogic/StdImpact082.h
	Skills/ImpactLogic/StdImpact083.cpp
	Skills/ImpactLogic/StdImpact083.h
	Skills/ImpactLogic/StdImpactSubSkill60.cpp
	Skills/ImpactLogic/StdImpactSubSkill60.h
)

SET (SKILL_SKILLLOGIC_HUMAN_LIST
	Skills/SkillLogic/Human/DaLiSkill001.cpp
	Skills/SkillLogic/Human/DaLiSkill001.h
	Skills/SkillLogic/Human/EMeiSkill001.cpp
	Skills/SkillLogic/Human/EMeiSkill001.h
	Skills/SkillLogic/Human/GaiBangSkill001.cpp
	Skills/SkillLogic/Human/GaiBangSkill001.h
	Skills/SkillLogic/Human/MingJiaoSkill001.cpp
	Skills/SkillLogic/Human/MingJiaoSkill001.h
	Skills/SkillLogic/Human/MingJiaoSkill002.cpp
	Skills/SkillLogic/Human/MingJiaoSkill002.h
	Skills/SkillLogic/Human/TianShanSkill001.cpp
	Skills/SkillLogic/Human/TianShanSkill001.h
	Skills/SkillLogic/Human/WuDangSkill001.cpp
	Skills/SkillLogic/Human/WuDangSkill001.h
	Skills/SkillLogic/Human/XiaoYaoSkill001.cpp
	Skills/SkillLogic/Human/XiaoYaoSkill001.h
	Skills/SkillLogic/Human/XiaoYaoSkill002.cpp
	Skills/SkillLogic/Human/XiaoYaoSkill002.h
)

SET (SKILL_SKILLLOGIC_OTHER_LIST
)

SET (SKILL_SKILLLOGIC_COMMON_LIST
	Skills/SkillLogic/Common/CommonSkill001.cpp
	Skills/SkillLogic/Common/CommonSkill001.h
	Skills/SkillLogic/Common/CommonSkill002.cpp
	Skills/SkillLogic/Common/CommonSkill002.h
	Skills/SkillLogic/Common/CommonSkill003.cpp
	Skills/SkillLogic/Common/CommonSkill003.h
)

SET (SKILL_SKILLLOGIC_ITEM_LIST
)

SET (SKILL_SKILLLOGIC_MONSTER_LIST
)

SET (SKILL_SKILLLOGIC_PET_LIST
	Skills/SkillLogic/Pet/PetSkill001.cpp
	Skills/SkillLogic/Pet/PetSkill001.h
)

SET (SKILL_SKILLLOGIC_LIST
	Skills/SkillLogic/ArmorMastery.cpp
	Skills/SkillLogic/ArmorMastery.h
	Skills/SkillLogic/ImpactsToTarget.cpp
	Skills/SkillLogic/ImpactsToTarget.h
	Skills/SkillLogic/ImpactsToTargetAndSelf.cpp
	Skills/SkillLogic/ImpactsToTargetAndSelf.h
	Skills/SkillLogic/ImpactsToTargetAndSelfForRun.cpp
	Skills/SkillLogic/ImpactsToTargetAndSelfForRun.h
	Skills/SkillLogic/SkillLogic.cpp
	Skills/SkillLogic/SkillLogic.h
	Skills/SkillLogic/TeleportAndImpacts.cpp
	Skills/SkillLogic/TeleportAndImpacts.h
	Skills/SkillLogic/WeaponMastery.cpp
	Skills/SkillLogic/WeaponMastery.h
)

SET (SKILL_SPECIALOBJLOGIC_LIST
	Skills/SpecialObjLogic/SpecialObjLogic.cpp
	Skills/SpecialObjLogic/SpecialObjLogic.h
	Skills/SpecialObjLogic/TotemLogic.cpp
	Skills/SpecialObjLogic/TotemLogic.h
	Skills/SpecialObjLogic/TrapLogic.cpp
	Skills/SpecialObjLogic/TrapLogic.h
)

SET (SKILL_LIST
	Skills/Combat_Core.cpp
	Skills/Combat_Core.h
	Skills/ConditionAndDeplete_Core.cpp
	Skills/ConditionAndDeplete_Core.h
	Skills/Impact_Core.cpp
	Skills/Impact_Core.h
	Skills/ImpactLogics.cpp
	Skills/ImpactLogics.h
	Skills/LogicManager.cpp
	Skills/LogicManager.h
	Skills/RelationshipChecker.cpp
	Skills/RelationshipChecker.h
	Skills/ScriptLogic.cpp
	Skills/ScriptLogic.h
	Skills/Skill_Core.cpp
	Skills/Skill_Core.h
	Skills/SkillInfo.cpp
	Skills/SkillInfo.h
	Skills/SkillLogics.cpp
	Skills/SkillLogics.h
	Skills/TargetChecker.cpp
	Skills/TargetChecker.h
	Skills/TargetingAndDepletingParams.cpp
	Skills/TargetingAndDepletingParams.h
)

SET (DB_LIST
	DB/HumanDB.cpp
	DB/HumanDB.h
	DB/HumanDB.inl
)

SET (STDAFX_LIST
	stdafx.cpp
	stdafx.h
)

SET (SERVER//SERVER//SERVER.VCPROJ_LIST
	Readme.txt
)



if (WIN32)
	source_group(Common\\Packets\\ItemPart\\ItemSynch FILES ${COMMON_PACKETS_ITEMPART_ITEMSYNCH_LIST})
	source_group(Common\\Packets\\ItemPart FILES ${COMMON_PACKETS_ITEMPART_LIST})
	source_group(Common\\Packets\\DataPool FILES ${COMMON_PACKETS_DATAPOOL_LIST})
	source_group(Common\\Packets\\Object FILES ${COMMON_PACKETS_OBJECT_LIST})
	source_group(Common\\Packets\\World FILES ${COMMON_PACKETS_WORLD_LIST})
	source_group(Common\\Packets\\Ability FILES ${COMMON_PACKETS_ABILITY_LIST})
	source_group(Common\\Packets\\Team FILES ${COMMON_PACKETS_TEAM_LIST})
	source_group(Common\\Packets\\Chat FILES ${COMMON_PACKETS_CHAT_LIST})
	source_group(Common\\Packets\\Commerce FILES ${COMMON_PACKETS_COMMERCE_LIST})
	source_group(Common\\Packets\\Menpai FILES ${COMMON_PACKETS_MENPAI_LIST})
	source_group(Common\\Packets\\Bank FILES ${COMMON_PACKETS_BANK_LIST})
	source_group(Common\\Packets\\Exchange\\StepI FILES ${COMMON_PACKETS_EXCHANGE_STEPI_LIST})
	source_group(Common\\Packets\\Exchange\\StepII FILES ${COMMON_PACKETS_EXCHANGE_STEPII_LIST})
	source_group(Common\\Packets\\Exchange\\StepIII FILES ${COMMON_PACKETS_EXCHANGE_STEPIII_LIST})
	source_group(Common\\Packets\\Exchange FILES ${COMMON_PACKETS_EXCHANGE_LIST})
	source_group(Common\\Packets\\Main FILES ${COMMON_PACKETS_MAIN_LIST})
	source_group(Common\\Packets\\Pet FILES ${COMMON_PACKETS_PET_LIST})
	source_group(Common\\Packets\\Guild FILES ${COMMON_PACKETS_GUILD_LIST})
	source_group(Common\\Packets\\Stall\\Check FILES ${COMMON_PACKETS_STALL_CHECK_LIST})
	source_group(Common\\Packets\\Stall\\Synch FILES ${COMMON_PACKETS_STALL_SYNCH_LIST})
	source_group(Common\\Packets\\Stall\\Close FILES ${COMMON_PACKETS_STALL_CLOSE_LIST})
	source_group(Common\\Packets\\Stall\\BBS FILES ${COMMON_PACKETS_STALL_BBS_LIST})
	source_group(Common\\Packets\\Stall FILES ${COMMON_PACKETS_STALL_LIST})
	source_group(Common\\Packets\\Relation FILES ${COMMON_PACKETS_RELATION_LIST})
	source_group(Common\\Packets\\PlayerShop\\Apply FILES ${COMMON_PACKETS_PLAYERSHOP_APPLY_LIST})
	source_group(Common\\Packets\\PlayerShop\\Manager FILES ${COMMON_PACKETS_PLAYERSHOP_MANAGER_LIST})
	source_group(Common\\Packets\\PlayerShop\\View FILES ${COMMON_PACKETS_PLAYERSHOP_VIEW_LIST})
	source_group(Common\\Packets\\PlayerShop FILES ${COMMON_PACKETS_PLAYERSHOP_LIST})
	source_group(Common\\Packets\\City FILES ${COMMON_PACKETS_CITY_LIST})
	source_group(Common\\Packets\\PrivateInfo FILES ${COMMON_PACKETS_PRIVATEINFO_LIST})
	source_group(Common\\Packets\\MsgMediator FILES ${COMMON_PACKETS_MSGMEDIATOR_LIST})
	source_group(Common\\Packets\\Title FILES ${COMMON_PACKETS_TITLE_LIST})
	source_group(Common\\Packets\\Country FILES ${COMMON_PACKETS_COUNTRY_LIST})
	source_group(Common\\Packets\\TopList FILES ${COMMON_PACKETS_TOPLIST_LIST})
	source_group(Common\\Packets\\Horse FILES ${COMMON_PACKETS_HORSE_LIST})
	source_group(Common\\Packets\\OnlineUser FILES ${COMMON_PACKETS_ONLINEUSER_LIST})
	source_group(Common\\Packets\\SouXia FILES ${COMMON_PACKETS_SOUXIA_LIST})
	source_group(Common\\Packets\\Bus FILES ${COMMON_PACKETS_BUS_LIST})
	source_group(Common\\Packets FILES ${COMMON_PACKETS_LIST})
	source_group(Common\\DataBase FILES ${COMMON_DATABASE_LIST})
	source_group(Common\\Combat FILES ${COMMON_COMBAT_LIST})
	source_group(Common\\Common FILES ${COMMON_COMMON_LIST})
	source_group(Common\\Utilities FILES ${COMMON_UTILITIES_LIST})
	source_group(Common\\Relation FILES ${COMMON_RELATION_LIST})
	source_group(Common FILES ${COMMON_LIST})
	source_group(Obj\\Visitor FILES ${OBJ_VISITOR_LIST})
	source_group(Obj\\HumanAssistantClass FILES ${OBJ_HUMANASSISTANTCLASS_LIST})
	source_group(Obj\\Horse FILES ${OBJ_HORSE_LIST})
	source_group(Obj\\Bus FILES ${OBJ_BUS_LIST})
	source_group(Obj FILES ${OBJ_LIST})
	source_group(Base FILES ${BASE_LIST})
	source_group(Main FILES ${MAIN_LIST})
	source_group(Packets\\SkillStudy FILES ${PACKETS_SKILLSTUDY_LIST})
	source_group(Packets\\Team\\Follow FILES ${PACKETS_TEAM_FOLLOW_LIST})
	source_group(Packets\\Team FILES ${PACKETS_TEAM_LIST})
	source_group(Packets\\Stall\\BBS FILES ${PACKETS_STALL_BBS_LIST})
	source_group(Packets\\Stall FILES ${PACKETS_STALL_LIST})
	source_group(Packets\\Item FILES ${PACKETS_ITEM_LIST})
	source_group(Packets\\PlayerShop FILES ${PACKETS_PLAYERSHOP_LIST})
	source_group(Packets\\Exchange FILES ${PACKETS_EXCHANGE_LIST})
	source_group(Packets\\City FILES ${PACKETS_CITY_LIST})
	source_group(Packets\\PrivateInfo FILES ${PACKETS_PRIVATEINFO_LIST})
	source_group(Packets\\Guild FILES ${PACKETS_GUILD_LIST})
	source_group(Packets\\TopList FILES ${PACKETS_TOPLIST_LIST})
	source_group(Packets\\Horse FILES ${PACKETS_HORSE_LIST})
	source_group(Packets\\Country FILES ${PACKETS_COUNTRY_LIST})
	source_group(Packets FILES ${PACKETS_LIST})
	source_group(Player FILES ${PLAYER_LIST})
	source_group(Script FILES ${SCRIPT_LIST})
	source_group(Item\\Growpoint FILES ${ITEM_GROWPOINT_LIST})
	source_group(Item\\ItemWrap FILES ${ITEM_ITEMWRAP_LIST})
	source_group(Item\\ItemBase FILES ${ITEM_ITEMBASE_LIST})
	source_group(Item\\ItemLogic FILES ${ITEM_ITEMLOGIC_LIST})
	source_group(Item FILES ${ITEM_LIST})
	source_group(SMU FILES ${SMU_LIST})
	source_group(Team FILES ${TEAM_LIST})
	source_group(MenPai\\Mission FILES ${MENPAI_MISSION_LIST})
	source_group(MenPai FILES ${MENPAI_LIST})
	source_group(AI FILES ${AI_LIST})
	source_group(Other\\PlayerShop FILES ${OTHER_PLAYERSHOP_LIST})
	source_group(Other\\SouXia FILES ${OTHER_SOUXIA_LIST})
	source_group(Other FILES ${OTHER_LIST})
	source_group(Ability FILES ${ABILITY_LIST})
	source_group(EventModule FILES ${EVENTMODULE_LIST})
	source_group(ActionModule FILES ${ACTIONMODULE_LIST})
	source_group(Scene\\Scan FILES ${SCENE_SCAN_LIST})
	source_group(Scene FILES ${SCENE_LIST})
	source_group(Skill\\ImpactLogic FILES ${SKILL_IMPACTLOGIC_LIST})
	source_group(Skill\\SkillLogic\\Human FILES ${SKILL_SKILLLOGIC_HUMAN_LIST})
	source_group(Skill\\SkillLogic\\Other FILES ${SKILL_SKILLLOGIC_OTHER_LIST})
	source_group(Skill\\SkillLogic\\Common FILES ${SKILL_SKILLLOGIC_COMMON_LIST})
	source_group(Skill\\SkillLogic\\Item FILES ${SKILL_SKILLLOGIC_ITEM_LIST})
	source_group(Skill\\SkillLogic\\Monster FILES ${SKILL_SKILLLOGIC_MONSTER_LIST})
	source_group(Skill\\SkillLogic\\Pet FILES ${SKILL_SKILLLOGIC_PET_LIST})
	source_group(Skill\\SkillLogic FILES ${SKILL_SKILLLOGIC_LIST})
	source_group(Skill\\SpecialObjLogic FILES ${SKILL_SPECIALOBJLOGIC_LIST})
	source_group(Skill FILES ${SKILL_LIST})
	source_group(DB FILES ${DB_LIST})
	source_group(StdAfx FILES ${STDAFX_LIST})
	source_group( FILES ${SERVER//SERVER//SERVER.VCPROJ_LIST})
endif()


ADD_EXECUTABLE(Server
	${COMMON_PACKETS_ITEMPART_ITEMSYNCH_LIST}
	${COMMON_PACKETS_ITEMPART_LIST}
	${COMMON_PACKETS_DATAPOOL_LIST}
	${COMMON_PACKETS_OBJECT_LIST}
	${COMMON_PACKETS_WORLD_LIST}
	${COMMON_PACKETS_ABILITY_LIST}
	${COMMON_PACKETS_TEAM_LIST}
	${COMMON_PACKETS_CHAT_LIST}
	${COMMON_PACKETS_COMMERCE_LIST}
	${COMMON_PACKETS_MENPAI_LIST}
	${COMMON_PACKETS_BANK_LIST}
	${COMMON_PACKETS_EXCHANGE_STEPI_LIST}
	${COMMON_PACKETS_EXCHANGE_STEPII_LIST}
	${COMMON_PACKETS_EXCHANGE_STEPIII_LIST}
	${COMMON_PACKETS_EXCHANGE_LIST}
	${COMMON_PACKETS_MAIN_LIST}
	${COMMON_PACKETS_PET_LIST}
	${COMMON_PACKETS_GUILD_LIST}
	${COMMON_PACKETS_STALL_CHECK_LIST}
	${COMMON_PACKETS_STALL_SYNCH_LIST}
	${COMMON_PACKETS_STALL_CLOSE_LIST}
	${COMMON_PACKETS_STALL_BBS_LIST}
	${COMMON_PACKETS_STALL_LIST}
	${COMMON_PACKETS_RELATION_LIST}
	${COMMON_PACKETS_PLAYERSHOP_APPLY_LIST}
	${COMMON_PACKETS_PLAYERSHOP_MANAGER_LIST}
	${COMMON_PACKETS_PLAYERSHOP_VIEW_LIST}
	${COMMON_PACKETS_PLAYERSHOP_LIST}
	${COMMON_PACKETS_CITY_LIST}
	${COMMON_PACKETS_PRIVATEINFO_LIST}
	${COMMON_PACKETS_MSGMEDIATOR_LIST}
	${COMMON_PACKETS_TITLE_LIST}
	${COMMON_PACKETS_COUNTRY_LIST}
	${COMMON_PACKETS_TOPLIST_LIST}
	${COMMON_PACKETS_HORSE_LIST}
	${COMMON_PACKETS_ONLINEUSER_LIST}
	${COMMON_PACKETS_SOUXIA_LIST}
	${COMMON_PACKETS_BUS_LIST}
	${COMMON_PACKETS_LIST}
	${COMMON_DATABASE_LIST}
	${COMMON_COMBAT_LIST}
	${COMMON_COMMON_LIST}
	${COMMON_UTILITIES_LIST}
	${COMMON_RELATION_LIST}
	${COMMON_LIST}
	${OBJ_VISITOR_LIST}
	${OBJ_HUMANASSISTANTCLASS_LIST}
	${OBJ_HORSE_LIST}
	${OBJ_BUS_LIST}
	${OBJ_LIST}
	${BASE_LIST}
	${MAIN_LIST}
	${PACKETS_SKILLSTUDY_LIST}
	${PACKETS_TEAM_FOLLOW_LIST}
	${PACKETS_TEAM_LIST}
	${PACKETS_STALL_BBS_LIST}
	${PACKETS_STALL_LIST}
	${PACKETS_ITEM_LIST}
	${PACKETS_PLAYERSHOP_LIST}
	${PACKETS_EXCHANGE_LIST}
	${PACKETS_CITY_LIST}
	${PACKETS_PRIVATEINFO_LIST}
	${PACKETS_GUILD_LIST}
	${PACKETS_TOPLIST_LIST}
	${PACKETS_HORSE_LIST}
	${PACKETS_COUNTRY_LIST}
	${PACKETS_LIST}
	${PLAYER_LIST}
	${SCRIPT_LIST}
	${ITEM_GROWPOINT_LIST}
	${ITEM_ITEMWRAP_LIST}
	${ITEM_ITEMBASE_LIST}
	${ITEM_ITEMLOGIC_LIST}
	${ITEM_LIST}
	${SMU_LIST}
	${TEAM_LIST}
	${MENPAI_MISSION_LIST}
	${MENPAI_LIST}
	${AI_LIST}
	${OTHER_PLAYERSHOP_LIST}
	${OTHER_SOUXIA_LIST}
	${OTHER_LIST}
	${ABILITY_LIST}
	${EVENTMODULE_LIST}
	${ACTIONMODULE_LIST}
	${SCENE_SCAN_LIST}
	${SCENE_LIST}
	${SKILL_IMPACTLOGIC_LIST}
	${SKILL_SKILLLOGIC_HUMAN_LIST}
	${SKILL_SKILLLOGIC_OTHER_LIST}
	${SKILL_SKILLLOGIC_COMMON_LIST}
	${SKILL_SKILLLOGIC_ITEM_LIST}
	${SKILL_SKILLLOGIC_MONSTER_LIST}
	${SKILL_SKILLLOGIC_PET_LIST}
	${SKILL_SKILLLOGIC_LIST}
	${SKILL_SPECIALOBJLOGIC_LIST}
	${SKILL_LIST}
	${DB_LIST}
	${STDAFX_LIST}
	${SERVER//SERVER//SERVER.VCPROJ_LIST}
)


##update_end

if (WIN32)
TARGET_LINK_LIBRARIES(Server FoxNet ScriptEngine LuaLib)
else()
TARGET_LINK_LIBRARIES(Server FoxNet ScriptEngine LuaLib pthread)
endif()

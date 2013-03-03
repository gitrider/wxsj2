//-----------------------------------------------------------------------------
// �ļ��� : LuaFnRegister.cpp
// ģ��	:	Script
// ����	 :  ��Luaע��c��������
// �޸���ʷ: 2008-2-1 �����
// �޸���ʷ��2010-5-21 yuyazhou
//	yangfei����
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "LuaFnRegister.h"
#include "LuaFnTbl_Mission.h"
#include "LuaFnTbl_Misc.h"
#include "LuaFnTbl_Ability.h"
#include "LuaFnTbl_Attr.h"
#include "LuaFnTbl_Pet.h"
#include "LuaFnTbl_Battle.h"
#include "LuaFnTbl_Shop.h"
#include "LuaFnTbl_PetPlacard.h"
#include "LuaFnTbl_Scene.h"
#include "LuaFnTbl_Team.h"
#include "LuaFnTbl_DoAction.h"
#include "LuaFnTbl_Relation.h"
#include "LuaFnTbl_Guild.h"
#include "LuaFnTbl_City.h"
#include "LuaFn_OpenBox.h"			//������
#include "LuaFnTbl_Bus.h"
#include "Log.h"


namespace LuaFnTbl
{
	struct _Str2Func functbl[] =
	{
		{"AddEventList",FuncProto(LuaFnAddNumText)},
		{"GetMission", FuncProto(LuaFnGetMission)},
		{"GetMissionCount", FuncProto(LuaFnGetMissionCount)},
		{"SetMissionByIndex", FuncProto(LuaFnSetMissionByIndex)},
		{"AddMission", FuncProto(LuaFnAddMission)},
		{"AddMissionEx", FuncProto(LuaFnAddMissionEx)},
		{"SetMissionEvent", FuncProto(LuaFnSetMissionEvent)},
		{"DelMission", FuncProto(LuaFnDelMission)},
		{"MissionCom", FuncProto(LuaFnMissionCom)},
		{"IsMissionHaveDone", FuncProto(LuaFnIsMissionHaveDone)},
		{"IsHaveMission", FuncProto(LuaFnIsHaveMission)},
		{"GetMissionIndexByID", FuncProto(LuaFnGetMissionIndexByID)},
		{"GetMissionParam", FuncProto(LuaFnGetMissionParam)},
		{"GetMissionData",FuncProto(LuaFnGetMissionData)},
		{"SetMissionData",FuncProto(LuaFnSetMissionData)},
		{"GetMissionDataBit",FuncProto(LuaFnGetMissionDataBit)},
		{"SetMissionDataBit",FuncProto(LuaFnSetMissionDataBit)},
		{"AddMonsterDropItem",FuncProto(LuaFnAddMonsterDropItem)},
		{"GetMonsterOwnerCount",FuncProto(LuaFnGetMonsterOwnerCount)},
		{"GetMonsterOwnerID",FuncProto(LuaFnGetMonsterOwnerID)},
		{"LuaFnSetNumText",FuncProto(LuaFnSetNumText)},
		{"GetNumText",FuncProto(LuaFnGetNumText)},
		{"GetEventList",FuncProto(LuaFnGetNumText)},
		{"LuaFnGetOneMissionInfo",FuncProto(LuaFnGetOneMissionInfo)},

		//��������ʾ���		
		{"BeginEvent", FuncProto(LuaFnBeginEvent)},
		{"EndEvent", FuncProto(LuaFnEndEvent)},
		{"DispatchEventList", FuncProto(LuaFnDispatchEventList)},
		{"DispatchRegieEventList",FuncProto(LuaFnDispatchRegieEventList)},
		{"DispatchMissionInfo", FuncProto(LuaFnDispatchMissionInfo)},
		{"DispatchMissionDemandInfo", FuncProto(LuaFnDispatchMissionDemandInfo)},
		{"DispatchMissionContinueInfo", FuncProto(LuaFnDispatchMissionContinueInfo)},
		{"DispatchMissionTips", FuncProto(LuaFnDispatchMissionTips)},
		{"DispatchMissionResult", FuncProto(LuaFnDispatchMissionResult)},
		{"AddText", FuncProto(LuaFnAddText)},
		{"AddNumber", FuncProto(LuaFnAddNumber)},
		{"AddNumText", FuncProto(LuaFnAddNumText)},
		{"AddRandItemBonus", FuncProto(LuaFnAddRandItemBonus)},
		{"AddRadioItemBonus", FuncProto(LuaFnAddRadioItemBonus)},
		{"AddMoneyBonus", FuncProto(LuaFnAddMoneyBonus)},
		{"AddItemBonus", FuncProto(LuaFnAddItemBonus)},
		{"AddItemDemand", FuncProto(LuaFnAddItemDemand)},

		{"BeginUICommand", FuncProto(LuaFnBeginUICommand)},
		{"EndUICommand", FuncProto(LuaFnEndUICommand)},
		{"DispatchUICommand", FuncProto(LuaFnDispatchUICommand)},
		{"UICommand_AddInt", FuncProto(LuaFnUICommand_AddInt)},
		{"UICommand_AddString", FuncProto(LuaFnUICommand_AddString)},
		{"GetQuestionsRecord", FuncProto(LuaFnGetQuestionsRecord)},

		//���������			
		{"BeginAddItem", FuncProto(LuaFnBeginAddItem)},
		{"AddItem", FuncProto(LuaFnAddItem)},
		{"EndAddItem", FuncProto(LuaFnEndAddItem)},
		{"AddItemListToHuman", FuncProto(LuaFnAddItemListToHuman)},
		//�������Լ�����		
		{"CallScriptFunction", FuncProto(LuaFnCallScriptFunction)},
		{"ShowNpcEventList", FuncProto(LuaFnShowNpcEventList)},
		{"LuaFnSetNumText",FuncProto(LuaFnSetNumText)},

		//ÿ�ջ��� 20100723 BLL
		{"IsMissionInTime",FuncProto(LuaFnIsMissionInTime)},
		{"MissionCommonCheck",FuncProto(LuaFnMissionCommonCheck)},
		{"IsTodayMission",FuncProto(LuaFnIsTodayMission)},
		{"SetTimeParam",FuncProto(LuaFnSetTimeParam)},
		{"GetTodayDate",FuncProto(LuaFnGetTodayDate)},
		
		//������				
		{"IsCaptain", FuncProto(LuaFnIsCaptain)},
		{"GetTeamId", FuncProto(LuaFnGetTeamId)},
		{"LeaveTeam", FuncProto(LuaFnLeaveTeam)},
		{"Msg2Player", FuncProto(LuaFnMsg2Player)},
		{"Msg2Team", FuncProto(LuaFnMsg2Team)},
		{"GetTeamSize",FuncProto(LuaFnGetTeamSize)},
		{"IsTeamFollow", FuncProto(LuaFnIsTeamFollow)},
		{"GetFollowedMembersCount",FuncProto(LuaFnGetFollowedMembersCount)},
		{"GetFollowedMember",FuncProto(LuaFnGetFollowedMember)},
		{"LuaFnGetTeamSceneMember",FuncProto(LuaFnGetTeamSceneMember)},
		{"GetTeamSceneMemberCount",FuncProto(LuaFnGetTeamSceneMemberCount)},
		{"SetTeamFollowSpeed",FuncProto(LuaFnSetTeamFollowSpeed)},
		{"StopTeamFollow",FuncProto(LuaFnStopTeamFollow)},
		{"ClearFollowedMembers",FuncProto(LuaFnClearFollowedMembers)},			
		{"LuaFnIsTeamLeader",FuncProto(LuaFnIsTeamLeader)},
		{"GetTeamMemberGuid",FuncProto(LuaFnGetTeamMemberGuid)},
		{"LuaFnHasTeam",FuncProto(LuaFnHasTeam)},
		{"GetNearTeamCount",FuncProto(LuaFnGetNearTeamCount)},
		{"GetNearTeamMember",FuncProto(LuaFnGetNearTeamMember)},
		{"GetMonsterGroupID",FuncProto(LuaFnGetMonsterGroupID)},
		{"GetMonsterTeamID",FuncProto(LuaFnGetMonsterTeamID)},
		{"GetMonsterDataID",FuncProto(LuaFnGetMonsterDataID)},
		{"NpcMsg2Player",FuncProto(LuaFnNpcMsg2Player)},

		//����ħ�����			
		{"AddXinFa", FuncProto(LuaFnAddXinFa)},
		{"AddSkill", FuncProto(LuaFnAddSkill)},
		{"DelXinFa", FuncProto(LuaFnDelXinFa)},
		{"DelSkill", FuncProto(LuaFnDelSkill)},
		{"HaveXinFa", FuncProto(LuaFnHaveXinFa)},
		{"HaveSkill", FuncProto(LuaFnHaveSkill)},
		{"UseSkill", FuncProto(LuaFnUseSkill)},

		//�������
		{"GetCurCountry", FuncProto(LuaFnGetCurCountry)},

		//��Ӫ���				
		{"GetCurCamp", FuncProto(LuaFnGetCurCamp)},
		{"SetCurCamp", FuncProto(LuaFnSetCurCamp)},
		{"RestoreNpcCamp", FuncProto(LuaFnRestoreNpcCamp)},
		{"SetNpcCurCamp", FuncProto(LuaFnSetNpcCurCamp)},
		//�������				
		{"GetMenPai", FuncProto(LuaFnGetMenPai)},
		{"SetMenPai", FuncProto(LuaFnSetMenPai)},
		{"GetMenPaiMaster", FuncProto(LuaFnGetMenPaiMaster)},
		{"GetMenPaiFigure", FuncProto(LuaFnGetMenPaiFigure)},
		{"DestroyMenPai", FuncProto(LuaFnDestroyMenPai)},
		{"GetMenPaiTitle", FuncProto(LuaFnGetMenPaiTitle)},
		//�����������			
		{"LuaFnGetGUID", FuncProto(LuaFnGetGUID)},
		{"GetExp", FuncProto(LuaFnGetExp)},
		{"AddExp", FuncProto(LuaFnAddExp)},
		{"GetHp", FuncProto(LuaFnGetHp)},
		{"RestoreHp", FuncProto(LuaFnRestoreHp)},
		{"GetMp", FuncProto(LuaFnGetMp)},
		{"RestoreMp", FuncProto(LuaFnRestoreMp)},

		{"GetCon", FuncProto(LuaFnGetCon)},
		{"GetSex", FuncProto(LuaFnGetSex)},
		{"GetName", FuncProto(LuaFnGetName)},
		{"GetMoney", FuncProto(LuaFnGetMoney)},
		{"CostMoney", FuncProto(LuaFnCostMoney)},
		{"AddMoney", FuncProto(LuaFnAddMoney)},
		{"GetLevel", FuncProto(LuaFnGetLevel)},
		{"SetLevel", FuncProto(LuaFnSetLevel)},
		{"LuaFnCanLevelUp", FuncProto(LuaFnCanLevelUp)},
		{"GetItemTableIndexByIndex", FuncProto(LuaFnGetItemTableIndexByIndex)},
		{"LuaFnGetMaterialStartBagPos", FuncProto(LuaFnGetMaterialStartBagPos)},
		{"LuaFnGetMaterialEndBagPos", FuncProto(LuaFnGetMaterialEndBagPos)},
		{"LuaFnGetItemCountInBagPos", FuncProto(LuaFnGetItemCountInBagPos)},
		{"GetItemCount", FuncProto(LuaFnGetItemCount)},
		{"HaveItem", FuncProto(LuaFnHaveItem)},
		{"UseItem", FuncProto(LuaFnUseItem)},
		{"HaveItemInBag",FuncProto(LuaFnHaveItemInBag)},
		{"DelItem", FuncProto(LuaFnDelItem)},
		{"EraseItem", FuncProto(LuaFnEraseItem)},
		{"GetWorldPos", FuncProto(LuaFnGetWorldPos)},
		{"GetSceneNPCPos", FuncProto(LuaFnGetSceneNPCPos)},
		{"NewWorld", FuncProto(LuaFnNewWorld)},
		{"SetPos", FuncProto(LuaFnSetPos)},
		{"SetRevPos", FuncProto(LuaFnSetRevPos)},
		{"GetHumanMaxVigor", FuncProto(LuaFnGetHumanMaxVigor)},
		{"GetHumanMaxEnergy", FuncProto(LuaFnGetHumanMaxEnergy)},
		{"GetHumanVigor", FuncProto(LuaFnGetHumanVigor)},
		{"GetHumanEnergy", FuncProto(LuaFnGetHumanEnergy)},
		{"GetHumanVigorRegeneRate", FuncProto(LuaFnGetHumanVigorRegeneRate)},
		{"GetHumanEnergyRegeneRate", FuncProto(LuaFnGetHumanEnergyRegeneRate)},
		{"SetHumanMaxVigor", FuncProto(LuaFnSetHumanMaxVigor)},
		{"SetHumanMaxEnergy", FuncProto(LuaFnSetHumanMaxEnergy)},
		{"SetHumanVigor", FuncProto(LuaFnSetHumanVigor)},
		{"SetHumanEnergy", FuncProto(LuaFnSetHumanEnergy)},
		{"SetHumanVigorRegeneRate", FuncProto(LuaFnSetHumanVigorRegeneRate)},
		{"SetHumanEnergyRegeneRate", FuncProto(LuaFnSetHumanEnergyRegeneRate)},
		{"GetHumanHairColor", FuncProto(LuaFnGetHumanHairColor)},
		{"SetHumanHairColor", FuncProto(LuaFnSetHumanHairColor)},
		{"GetHumanHairModel", FuncProto(LuaFnGetHumanHairModel)},
		{"SetHumanHairModel", FuncProto(LuaFnSetHumanHairModel)},
		{"LuaFnChangeHumanHairModel", FuncProto(LuaFnChangeHumanHairModel)},
		{"LuaFnGetHumanGoodBadValue", FuncProto(LuaFnGetHumanGoodBadValue)},
		{"LuaFnSetHumanGoodBadValue", FuncProto(LuaFnSetHumanGoodBadValue)},

		{"LuaFnSetPKValue", FuncProto(LuaFnSetPKValue)},
		{"LuaFnGetPKValue", FuncProto(LuaFnGetPKValue)},
        {"LuaFnSetExchangStatus", FuncProto(LuaFnSetExchangStatus)},


		//Ѳ�����				
		{"SetPatrolId", FuncProto(LuaFnSetPatrolId)},

		//��ʱ�����			
		{"SetTimer", FuncProto(LuaFnSetTimer)},
		{"StopTimer", FuncProto(LuaFnStopTimer)},
		{"CheckTimer", FuncProto(LuaFnCheckTimer)},
		{"GetDayTime", FuncProto(LuaGetDayTime)},
		{"GetObjCreateTime", FuncProto(LuaGetObjCreateTime)},

		//ϵͳ��ʱ���¼�
		{"StartupSystemTimer", FuncProto(LuaFnStartupSystemTimer)},
		{"StopSystemTimer", FuncProto(LuaFnStopSystemTimer)},
		{"SetSystemTimerTick", FuncProto(LuaFnSetSystemTimerTick)},
		{"SetSystemTimerParam", FuncProto(LuaFnSetSystemTimerParam)},
		{"GetSystemTimerParam", FuncProto(LuaFnGetSystemTimerParam)},	
		{"CheckSystemTimerValidity", FuncProto(LuaFnCheckSystemTimerValidity)},	

		//�������			
		{"AddGlobalCountNews", FuncProto(LuaFnAddGlobalCountNews)},
		{"GetGameTime", FuncProto(LuaFnGetGameTime)},
		{"PrintNum", FuncProto(LuaFnPrintNum)},
		{"PrintStr", FuncProto(LuaFnPrintStr)},
		{"IsInDist", FuncProto(LuaFnIsInDist)},
		{"LuaFnGuid2ObjId", FuncProto(LuaFnGuid2ObjId)},
		{"LuaFnObjId2Guid", FuncProto(LuaFnObjId2Guid)},
		{"GetDist",FuncProto(LuaFnGetDist)},
		//{"RemoveMonster", FuncProto(LuaFnRemoveMonster)},
		{"QueryAreaStandingTime",FuncProto(LuaFnQueryAreaStandingTime)},
		{"ResetAreaStandingTime",FuncProto(LuaFnResetAreaStandingTime)},
		{"LuaFnGetTickCount",FuncProto(LuaFnGetTickCount)},
		{"LuaFnGetCurrentTime",FuncProto(LuaFnGetCurrentTime)},
		{"LuaFnIsInStall",FuncProto(LuaFnIsInStall)},

		//�������Ʒϵͳ��غ���
		{"GetItemIsEmpty", FuncProto(LuaFnGetItemIsEmpty)},
		{"GetHumanGUID", FuncProto(LuaFnGetHumanGUID)},
		{"GetItemIndex", FuncProto(LuaFnGetItemIndex)},
		{"GetGemEmbededCount", FuncProto(LuaFnGetGemEmbededCount)},
		{"GetBagItemDur", FuncProto(LuaFnGetBagItemDur)},
		{"SetBagItemDur", FuncProto(LuaFnSetBagItemDur)},
		{"LuaFnSetBodyItemDur", FuncProto(LuaFnSetBodyItemDur)},
		{"LuaFnGetBodyItemDur", FuncProto(LuaFnGetBodyItemDur)},

		{"LuaFnGetNotBindItemCountInBag", FuncProto(LuaFnGetNotBindItemCountInBag)},
		{"LuaFnGetNotBindItemCountInBody", FuncProto(LuaFnGetNotBindItemCountInBody)},
		{"LuaFnDropNotBindItemInBag", FuncProto(LuaFnDropNotBindItemInBag)},
		{"LuaFnDropNotBindItemInBody", FuncProto(LuaFnDropNotBindItemInBody)},

		{"GetBagItemParam", FuncProto(LuaFnGetBagItemParam)},
		{"SetBagItemParam", FuncProto(LuaFnSetBagItemParam)},
		{"GetBagItemIdent", FuncProto(LuaFnGetBagItemIdent)},
		{"SetBagItemIdent", FuncProto(LuaFnSetBagItemIdent)},
		{"GetItemQuality", FuncProto(LuaFnGetItemQuality)},
		{"LuaFnGetItemType", FuncProto(LuaFnGetItemType)},
		{"LuaFnGetBagEquipType", FuncProto(LuaFnGetBagEquipType)},
		{"ItemBoxEnterScene", FuncProto(LuaFnItemBoxEnterScene)},
		{"QueryHumanAbility", FuncProto(LuaFnQueryHumanAbility)},
		{"QueryHumanAbilityLevel", FuncProto(LuaFnQueryHumanAbilityLevel)}, //�����
		{"SetHumanAbilityLevel", FuncProto(LuaFnSetHumanAbilityLevel)},
		{"SetAbilityExp", FuncProto(LuaFnSetAbilityExp)},
		{"GetAbilityExp", FuncProto(LuaFnGetAbilityExp)},
		{"GetAbilityExpPlus", FuncProto(LuaFnGetAbilityExpPlus)},
		{"SetPrescription", FuncProto(LuaFnSetPrescription)},
		{"IsPrescrLearned", FuncProto(LuaFnIsPrescrLearned)},
		{"GetAbilityObjIndex", FuncProto(LuaFnGetAbilityObjIndex)},
		{"AddItemToBox", FuncProto(LuaFnAddItemToBox)},
		{"GetItemBoxRequireAbilityID", FuncProto(LuaFnGetItemBoxRequireAbilityID)},
		{"GetItemBoxRecycleTimes", FuncProto(LuaFnGetItemBoxRecycleTimes)},
		{"SetItemBoxRecycleTimes", FuncProto(LuaFnSetItemBoxRecycleTimes)},
		{"CreateMonsterOnScene", FuncProto(LuaFnCreateMonsterOnScene)},
		{"LuaFnCreateMonster", FuncProto(LuaFnCreateMonster)},
		{"LuaFnDeleteMonster", FuncProto(LuaFnDeleteMonster)},
		{"AddStorePointOnScene", FuncProto(LuaFnAddStorePointOnScene)},
		{"DelStorePointOnScene", FuncProto(LuaFnDelStorePointOnScene)},
		{"FindStorePointOnScene", FuncProto(LuaFnFindStorePointOnScene)},
		{"GetStorePointType", FuncProto(LuaFnGetStorePointType)},
		{"TryRecieveItem", FuncProto(LuaFnTryRecieveItem)},
		{"SetItemBoxOwner", FuncProto(LuaFnSetItemBoxOwner)},
		{"GetItemBoxOwner", FuncProto(LuaFnGetItemBoxOwner)},
		{"DelGPOwner", FuncProto(LuaFnDelGPOwner)},
		{"AddGPOwner", FuncProto(LuaFnAddGPOwner)},
		{"LuaFnSetItemCreator", FuncProto(LuaFnSetItemCreator)},
		{"GetGPCountByOwner", FuncProto(LuaFnGetGPCountByOwner)},
		{"GetItemBoxMaxGrowTime", FuncProto(LuaFnGetItemBoxMaxGrowTime)},
		{"SetItemBoxMaxGrowTime", FuncProto(LuaFnSetItemBoxMaxGrowTime)},
		{"SetItemBoxPickOwnerTime", FuncProto(LuaFnSetItemBoxPickOwnerTime)},
		{"EnableItemBoxPickOwnerTime", FuncProto(LuaFnEnableItemBoxPickOwnerTime)},
		{"GetItemBoxWorldPosX", FuncProto(LuaFnGetItemBoxWorldPosX)},
		{"GetItemBoxWorldPosZ", FuncProto(LuaFnGetItemBoxWorldPosZ)},
		{"GetHumanWorldX", FuncProto(LuaFnGetHumanWorldX)},
		{"GetHumanWorldZ", FuncProto(LuaFnGetHumanWorldZ)},
		{"GetStoreMapX", FuncProto(LuaFnGetStoreMapX)},
		{"GetStoreMapZ", FuncProto(LuaFnGetStoreMapZ)},
		{"GetStoreMapSceneID", FuncProto(LuaFnGetStoreMapSceneID)},
		{"GetItemBoxOpenFlag", FuncProto(LuaFnGetItemBoxOpenFlag)},
		{"SetAbilityOperaRobotTime", FuncProto(LuaFnSetAbilityOperaRobotTime)},
		{"GetAbilityOperaRobotTime", FuncProto(LuaFnGetAbilityOperaRobotTime)},
		{"SetAbilityOperaTime", FuncProto(LuaFnSetAbilityOperaTime)},
		{"LuaFnSendAbilitySuccessMsg", FuncProto(LuaFnSendAbilitySuccessMsg)},
		{"IsEquipItem", FuncProto(LuaFnIsEquipItem)},//add by gh 2010/07/22
		{"GetItemEquipPoint", FuncProto(LuaFnGetItemEquipPoint)},
		{"GeEquipReqLevel", FuncProto(LuaFnGeEquipReqLevel)},
		{"LuaFnGetAbilityLevelUpConfig", FuncProto(LuaFnGetAbilityLevelUpConfig)},
		{"LuaFnGetAbilityLevelUpConfig2", FuncProto(LuaFnGetAbilityLevelUpConfig2)},

		//�̵����				
		{"DispatchShopItem", FuncProto(LuaFnDispatchShopItem)},
		//�������
		{"DispatchMenpaiInfo", FuncProto(LuaFnDispatchMenpaiInfo)},
		{"DispatchXinfaLevelInfo", FuncProto(LuaFnDispatchXinfaLevelInfo)},
		{"LuaFnJoinMenpai", FuncProto(LuaFnJoinMenpai)},
		//ѧϰ�����,�˷Ѵ���,����û������,by gh 
		{"DispatchAbilityInfo",FuncProto(LuaFnDispatchAbilityInfo)},
		//�ƺ����
		{"AwardTitle", FuncProto(LuaFnAwardTitle)},
		{"GetTitle", FuncProto(LuaFnGetTitle)},
		{"DeleteTitle", FuncProto(LuaFnDeleteTitle)},
		{"DispatchAllTitle", FuncProto(LuaFnDispatchAllTitle)},
		{"GetDyTitlesNum", FuncProto(LuaFnGetDyTitlesNum)},
		{"AddDyTitle", FuncProto(LuaFnAddDyTitle)},
		{"AwardBangpaiTitle", FuncProto(LuaFnAwardBangPaiTitle)},
		{"AwardSpouseTitle", FuncProto(LuaFnAwardSpouseTitle)},
		{"AwardNickTitle", FuncProto(LuaFnAwardNickTitle)},
		{"AwardJieBaiTitle", FuncProto(LuaFnAwardJieBaiTitle)},
		{"AwardShiTuTitle", FuncProto(LuaFnAwardShiTuTitle)},
		{"AwardShangDianTitle", FuncProto(LuaFnAwardShangDianTitle)},
		{"EnableBankRentIndex", FuncProto(LuaFnEnableBankRentIndex)},
		{"GetBankRentIndex", FuncProto(LuaFnGetBankRentIndex)},
		{"BankBegin", FuncProto(LuaFnBankBegin)},
		{"SetPlayerDefaultReliveInfo", FuncProto(LuaSetPlayerDefaultReliveInfo)},
		//�������
		{"LuaFnGetSceneType",FuncProto(LuaFnGetSceneType)},
		{"LuaFnCreateCopyScene",FuncProto(LuaFnCreateCopyScene)},
		{"LuaFnGetSceneStatue",FuncProto(LuaFnGetSceneStatue)},
		{"LuaFnSetSceneLoad_Map",FuncProto(LuaFnSetSceneLoad_Map)},
		{"LuaFnSetSceneLoad_Monster",FuncProto(LuaFnSetSceneLoad_Monster)},
		{"LuaFnSetSceneLoad_Platform",FuncProto(LuaFnSetSceneLoad_Platform)},
		{"LuaFnSetSceneLoad_GrowPointData",FuncProto(LuaFnSetSceneLoad_GrowPointData)},
		{"LuaFnSetSceneLoad_GrowPointSetup",FuncProto(LuaFnSetSceneLoad_GrowPointSetup)},
		{"LuaFnSetSceneLoad_Area",FuncProto(LuaFnSetSceneLoad_Area)},
		{"LuaFnSetSceneLoad_Pet",FuncProto(LuaFnSetSceneLoad_Pet)},
		{"LuaFnSetSceneLoad_PatrolPointData",FuncProto(LuaFnSetSceneLoad_PatrolPointData)},
		{"LuaFnSetCopySceneData_TeamLeader",FuncProto(LuaFnSetCopySceneData_TeamLeader)},
		{"LuaFnSetCopySceneData_NoUserCloseTime",FuncProto(LuaFnSetCopySceneData_NoUserCloseTime)},
		{"LuaFnSetCopySceneData_Timer",FuncProto(LuaFnSetCopySceneData_Timer)},
		{"LuaFnSetCopySceneData_Param",FuncProto(LuaFnSetCopySceneData_Param)},
		{"LuaFnGetCopySceneData_TeamLeader",FuncProto(LuaFnGetCopySceneData_TeamLeader)},
		{"LuaFnGetCopySceneData_NoUserCloseTime",FuncProto(LuaFnGetCopySceneData_NoUserCloseTime)},
		{"LuaFnGetCopySceneData_Timer",FuncProto(LuaFnGetCopySceneData_Timer)},
		{"LuaFnGetCopySceneData_Param",FuncProto(LuaFnGetCopySceneData_Param)},
		{"LuaFnGetCopyScene_HumanCount",FuncProto(LuaFnGetCopyScene_HumanCount)},
		{"LuaFnGetCopyScene_HumanObjId",FuncProto(LuaFnGetCopyScene_HumanObjId)},
		{"LuaFnIsObjValid",FuncProto(LuaFnIsObjValid)},
		{"LuaFnIsCharacterLiving",FuncProto(LuaFnIsCharacterLiving)},
		{"LuaFnSendNormalMail",FuncProto(LuaFnSendNormalMail)},
		{"LuaFnSendScriptMail",FuncProto(LuaFnSendScriptMail)},
		{"LuaFnSendMailToAllFriend",FuncProto(LuaFnSendMailToAllFriend)},
		{"GetMonsterCount",FuncProto(LuaFnGetMonsterCount)},
		{"GetMonsterObjID",FuncProto(LuaFnGetMonsterObjID)},
		{"IsCanEnterCopyScene",FuncProto(LuaFnIsCanEnterCopyScene)},
		{"LuaFnGetNewSceneInfoByIndex",FuncProto(LuaFnGetNewSceneInfoByIndex)},


		// ��ϵϵͳ���
		//����
		{"LuaFnGetObjType", FuncProto(LuaFnGetObjType)},
		{"LuaFnAddEnemy", FuncProto(LuaFnAddEnemy)},
		// ����
		{"LuaFnIsFriend",FuncProto(LuaFnIsFriend)},
		{"LuaFnGetFriendName",FuncProto(LuaFnGetFriendName)},
		{"LuaFnGetFriendPoint",FuncProto(LuaFnGetFriendPoint)},
		{"LuaFnSetFriendPoint",FuncProto(LuaFnSetFriendPoint)},
		{"LuaFnSetDeadLinkFriendPoint",FuncProto(LuaFnSetDeadLinkFriendPoint)},
		{"LuaFnSetFriendPointByGUID",FuncProto(LuaFnSetFriendPointByGUID)},
		{"LuaFnDelFriendByGUID",FuncProto(LuaFnDelFriendByGUID)},
		// ���
		{"LuaFnIsMarried",FuncProto(LuaFnIsMarried)},
		{"LuaFnGetSpouseGUID",FuncProto(LuaFnGetSpouseGUID)},
		{"LuaFnMarry",FuncProto(LuaFnMarry)},
		{"LuaFnIsSpouses",FuncProto(LuaFnIsSpouses)},
		{"LuaFnUnMarry",FuncProto(LuaFnUnMarry)},
		{"LuaFnDivorce",FuncProto(LuaFnDivorce)},
		// ʦͽ
		{"LuaFnIsMaster",FuncProto(LuaFnIsMaster)},
		{"LuaFnIsPrentice",FuncProto(LuaFnIsPrentice)},
		{"LuaFnGetPrenticeGUID",FuncProto(LuaFnGetPrenticeGUID)},
		{"LuaFnHaveMaster",FuncProto(LuaFnHaveMaster)},
		{"LuaFnGetMasterGUID",FuncProto(LuaFnGetMasterGUID)},
		{"LuaFnGetMasterMoralPoint",FuncProto(LuaFnGetMasterMoralPoint)},
		{"LuaFnSetMasterMoralPoint",FuncProto(LuaFnSetMasterMoralPoint)},
		{"LuaFnGetPrenticeCount",FuncProto(LuaFnGetPrenticeCount)},
		{"LuaFnGetPrenticeBetrayTime",FuncProto(LuaFnGetPrenticeBetrayTime)},
		{"LuaFnAprentice",FuncProto(LuaFnAprentice)},
		{"LuaFnFinishAprentice",FuncProto(LuaFnFinishAprentice)},
		{"LuaFnBetrayMaster",FuncProto(LuaFnBetrayMaster)},
		{"LuaFnExpelPrentice",FuncProto(LuaFnExpelPrentice)},
		{"LuaFnUpdateAttr",FuncProto(LuaFnUpdateAttr)},
		{"LuaFnIsPasswordSetup",FuncProto(LuaFnIsPasswordSetup)},
		{"LuaFnIsPasswordUnlocked",FuncProto(LuaFnIsPasswordUnlocked)},
		// ���
		{"LuaFnIsBrother",FuncProto(LuaFnIsBrother)},
		//������Ӻ���ע��,�ֹ�����벻Ҫ��������
		{"LuaFnAddExp",FuncProto(LuaFnAddExp)},
		{"LuaFnAddGlobalCountNews",FuncProto(LuaFnAddGlobalCountNews)},
		{"LuaFnAddItem",FuncProto(LuaFnAddItem)},
		{"LuaFnAddItemListToHuman",FuncProto(LuaFnAddItemListToHuman)},
		{"LuaFnAddMoney",FuncProto(LuaFnAddMoney)},
		{"LuaFnAwardSpouseTitle",FuncProto(LuaFnAwardSpouseTitle)},
		{"LuaFnAwardTitle",FuncProto(LuaFnAwardTitle)},
		{"LuaFnCostMoney",FuncProto(LuaFnCostMoney)},
		{"LuaFnDispatchAllTitle",FuncProto(LuaFnDispatchAllTitle)},
			
		{"LuaFnBeginAddItem",FuncProto(LuaFnBeginAddItem)},
		{"LuaFnEndAddItem",FuncProto(LuaFnEndAddItem)},
		{"LuaFnEraseItem",FuncProto(LuaFnEraseItem)},
		{"LuaFnGetFollowedMembersCount",FuncProto(LuaFnGetFollowedMembersCount)},

		{"LuaFnGetItemCount",FuncProto(LuaFnGetItemCount)},
		{"LuaFnGetItemQuality",FuncProto(LuaFnGetItemQuality)},
		{"LuaFnGetItemTableIndexByIndex",FuncProto(LuaFnGetItemTableIndexByIndex)},
		{"LuaFnGetLevel",FuncProto(LuaFnGetLevel)},
		{"LuaFnGetMenPai",FuncProto(LuaFnGetMenPai)},
		{"LuaFnGetMoney",FuncProto(LuaFnGetMoney)},
		{"LuaFnGetName",FuncProto(LuaFnGetName)},
		{"LuaFnGetSex",FuncProto(LuaFnGetSex)},
		{"LuaFnGetTeamSceneMemberCount",FuncProto(LuaFnGetTeamSceneMemberCount)},
		{"LuaFnGetTeamSize",FuncProto(LuaFnGetTeamSize)},
		{"LuaFnGetWorldPos",FuncProto(LuaFnGetWorldPos)},
		{"LuaFnIsPrescrLearned",FuncProto(LuaFnIsPrescrLearned)},
		{"LuaFnMsg2Player",FuncProto(LuaFnMsg2Player)},
		{"LuaFnTryRecieveItem",FuncProto(LuaFnTryRecieveItem)},

		//����
		{"LuaFnGetPetCount",FuncProto(LuaFnGetPetCount)},
		{"LuaFnGetPet_DataID",FuncProto(LuaFnGetPet_DataID)},
		{"LuaFnGetPet_Level",FuncProto(LuaFnGetPet_Level)},
		{"LuaFnGetPet_TakeLevel",FuncProto(LuaFnGetPet_TakeLevel)},
		{"LuaFnGetPet_HP",FuncProto(LuaFnGetPet_HP)},
		{"LuaFnGetPet_MaxHP",FuncProto(LuaFnGetPet_MaxHP)},
		{"LuaFnGetPet_Life",FuncProto(LuaFnGetPet_Life)},
		{"LuaFnGetPet_Type",FuncProto(LuaFnGetPet_Type)},
		{"LuaFnGetPet_Generation",FuncProto(LuaFnGetPet_Generation)},
		{"LuaFnGetPet_Happyness",FuncProto(LuaFnGetPet_Happyness)},
		{"LuaFnGetPet_StrPerception",FuncProto(LuaFnGetPet_StrPerception)},
		{"LuaFnGetPet_ConPerception",FuncProto(LuaFnGetPet_SmartnessPerception)},
		{"LuaFnGetPet_DexPerception",FuncProto(LuaFnGetPet_MindPerception)},
		{"LuaFnGetPet_IntPerception",FuncProto(LuaFnGetPet_ConstitutionPerception)},
		{"LuaFnGetPet_GenGu",FuncProto(LuaFnGetPet_GenGu)},
		{"LuaFnGetPet_RemainPoint",FuncProto(LuaFnGetPet_RemainPoint)},
		{"LuaFnGetPet_Exp",FuncProto(LuaFnGetPet_Exp)},
		{"LuaFnGetPet_AttrLevel1",FuncProto(LuaFnGetPet_AttrLevel1)},
		{"LuaFnGetPet_Skill",FuncProto(LuaFnGetPet_Skill)},
		{"LuaFnCreatePet",FuncProto(LuaFnCreatePet)},
		{"LuaFnDeletePet",FuncProto(LuaFnDeletePet)},
		{"LuaFnGetPetHP",FuncProto(LuaFnGetPetHP)},
		{"LuaFnGetPetMaxHP",FuncProto(LuaFnGetPetMaxHP)},
		{"LuaFnGetPetLife",FuncProto(LuaFnGetPetLife)},
		{"LuaFnGetPetHappiness",FuncProto(LuaFnGetPetHappiness)},
		{"LuaFnSetPetHP",FuncProto(LuaFnSetPetHP)},
		{"LuaFnSetPetMaxHP",FuncProto(LuaFnSetPetMaxHP)},
		{"LuaFnSetPetLife",FuncProto(LuaFnSetPetLife)},
		{"LuaFnSetPetHappiness",FuncProto(LuaFnSetPetHappiness)},
		{"LuaFnPetCanUseFood",FuncProto(LuaFnPetCanUseFood)},
		{"LuaFnPetReturnToChild",FuncProto(LuaFnPetReturnToChild)},
		{"LuaFnPetCanReturnToChild",FuncProto(LuaFnPetCanReturnToChild)},
		{"LuaFnGetPetGUID",FuncProto(LuaFnGetPetGUID)},

        //PK���
        {"LuaFnGetPKPoint",FuncProto(LuaFnGetPKPoint)},
		{"LuaFnGetPKLevel",FuncProto(LuaFnGetPKLevel)},
		{"LuaFnGetPKPunish",FuncProto(LuaFnGetPKPunish)},

		//���﹫������LUA�����ӿ�
		{"IssuePetPlacardByIndex",FuncProto(LuaFnIssuePetPlacardByIndex)},
		{"IssuePetPlacard",FuncProto(LuaFnIssuePetPlacard)},
		{"CancelPetPlacard",FuncProto(LuaFnCancelPetPlacard)},
		{"DispatchPetPlacardList",FuncProto(LuaFnDispatchPetPlacardList)},

		//����ѵ�����
		{"CalcPetDomesticationMoney", FuncProto(LuaFnCalcPetDomesticationMoney)},
		{"PetDomestication", FuncProto(LuaFnPetDomestication)},

		//Unit Use Skill
		{"LuaFnUnitUseSkill", FuncProto(LuaFnUnitUseSkill)},
		{"LuaFnSetDamage", FuncProto(LuaFnSetDamage)},
		{"LuaFnGmKillObj", FuncProto(LuaFnGmKillObj)},
		{"LuaFnFindMonsterByGUID", FuncProto(LuaFnFindMonsterByGUID)},
		{"PetStudySkill", FuncProto(LuaFnPetStudySkill)},
		{"PetStudySkill_MenPai", FuncProto(LuaFnPetStudySkill_MenPai)},
		{"LuaFnGetScriptDynamicParamByIndex", FuncProto(LuaFnGetScriptDynamicParamByIndex)},
		{"LuaFnSetScriptDynamicParamByIndex", FuncProto(LuaFnSetScriptDynamicParamByIndex)},
		{"LuaFnDepletingUsedItem", FuncProto(LuaFnDepletingUsedItem)},
		{"LuaFnVerifyUsedItem", FuncProto(LuaFnVerifyUsedItem)},
		{"LuaFnGetTargetObjID", FuncProto(LuaFnGetTargetObjID)},
		{"LuaFnGetTargetPosition_X", FuncProto(LuaFnGetTargetPosition_X)},
		{"LuaFnGetTargetPosition_Z", FuncProto(LuaFnGetTargetPosition_Z)},
		{"LuaFnGetTargetBagSlot", FuncProto(LuaFnGetTargetBagSlot)},
		{"LuaFnGetBagIndexOfUsedItem", FuncProto(LuaFnGetBagIndexOfUsedItem)},
		{"LuaFnGetItemIndexOfUsedItem", FuncProto(LuaFnGetItemIndexOfUsedItem)},//add by gh for ability
		{"LuaFnGetLowSectionOfTargetPetGuid", FuncProto(LuaFnGetLowSectionOfTargetPetGuid)},
		{"LuaFnGetHighSectionOfTargetPetGuid", FuncProto(LuaFnGetHighSectionOfTargetPetGuid)},
		{"LuaFnIsScriptDynamicParamIndexLegal", FuncProto(LuaFnIsScriptDynamicParamIndexLegal)},
		{"LuaFnSendSpecificImpactToUnit", FuncProto(LuaFnSendSpecificImpactToUnit)},
		{"LuaFnCancelSpecificImpact", FuncProto(LuaFnCancelSpecificImpact)},
		{"LuaFnDoAction", FuncProto(LuaFnDoAction)},
		{"LuaFnUnitDoEmoteAction", FuncProto(LuaFnUnitDoEmoteAction)},

		//����
		{"GuildCreate", FuncProto(LuaFnGuildCreate)},
		{"GuildList", FuncProto(LuaFnGuildList)},
		{"AddGuildUserPoint", FuncProto(LuaAddGuildUserPoint)},
		{"SubGuildUserPoint", FuncProto(LuaSubGuildUserPoint)},
        {"AddTotalGP", FuncProto(LuaAddTotalGP)},
        {"SubTotalGP", FuncProto(LuaSubTotalGP)},

		//����
		{"CityCreate", FuncProto(LuaFnCityCreate)},
		{"CityDelete", FuncProto(LuaFnCityDelete)},
		//{"CityGetCityList", FuncProto(LuaFnCityGetCityList)},
		{"CityBuildingChange", FuncProto(LuaFnCityBuildingChange)},
		{"CityMoveTo", FuncProto(LuaFnCityMoveTo)},

        //����
        {"ExpandBankSize", FuncProto(LuaFnExpandBankSize)},
        {"GetBankSize", FuncProto(LuaFnGetBankSize)},

        //PK
        {"AddHonor", FuncProto(LuaAddHonor)},
        {"SubHonor", FuncProto(LuaSubHonor)},

        //����
        {"AddCountryActionPoint", FuncProto(LuaAddActionPoint)},
        {"SubCountryActionPoint", FuncProto(LuaSubActionPoint)},

        {"CountryDeletaBegin", FuncProto(LuaCountryDeletaBegin)},
        {"CountryVoteBegin", FuncProto(LuaCountryVoteBegin)},
		//by zheng 
		//���������
		{"OpenRndomBox", FuncProto(LuaFnProcessOpenBox)},

		// Bus
		{"LuaFnCreateBus", FuncProto(LuaFnCreateBus)},
		{"LuaFnCreateBusByPatrolPathId", FuncProto(LuaFnCreateBusByPatrolPathId)},
		{"LuaFnDeleteBus", FuncProto(LuaFnDeleteBus)},
		{"LuaFnBusAddPassenger", FuncProto(LuaFnBusAddPassenger)},
		{"LuaFnBusRemovePassenger", FuncProto(LuaFnBusRemovePassenger)},
		{"LuaFnBusRemoveAllPassenger", FuncProto(LuaFnBusRemoveAllPassenger)},
		{"LuaFnBusStart", FuncProto(LuaFnBusStart)},
		{"LuaFnBusGetObjIDByGUID", FuncProto(LuaFnBusGetObjIDByGUID)},
		{"LuaFnBusAddPassenger_Shuttle", FuncProto(LuaFnBusAddPassenger_Shuttle)},
		{"LuaFnBusAddPassengerList", FuncProto(LuaFnBusAddPassengerList)},
		{"LuaFnGetBusScriptParam", FuncProto(LuaFnGetBusScriptParam)},
		{"LuaFnSetBusScriptParam", FuncProto(LuaFnSetBusScriptParam)},
		{"LuaFnGetBusPassengerCount", FuncProto(LuaFnGetBusPassengerCount)},
		{"LuaFnGetBusPassengerIDByIndex", FuncProto(LuaFnGetBusPassengerIDByIndex)},
		{"LuaFnGetBusPassengerIDIsInBus", FuncProto(LuaFnGetBusPassengerID_IsInBus)},

	};
}

LuaCFuncRegister::LuaCFuncRegister()
: mOwner(NULL)
{
	/**/
}

LuaCFuncRegister::~LuaCFuncRegister()
{
	/**/
}

//��ʱ���׼�⺯��,ֻ��debug�����еĲ��Գ���
//�Ѹ���Luaע�ắ����ע��ɨ�����
//���ڸ�Դ���ļ�������Ŀ¼����ScriptFuncForDesigner.h
#ifdef _DEBUG
//#include <direct.h>

#define FUNCNUM 1024
#define NAMELEN 64*2

CHAR strbuf[_MAX_PATH];

#define REGISTERNUM 8
struct _RegisterInfo
{
	CHAR funcname[NAMELEN];
	CHAR regname[REGISTERNUM][NAMELEN];	

	INT regCount;
};

INT compare( const VOID *arg1, const VOID *arg2 )
{
	return tstricmp( (CHAR*)arg1,  (CHAR*)arg2 );
}

INT compareRegInfo( const VOID* arg1,const VOID* arg2)
{
	const _RegisterInfo* reg1 = (_RegisterInfo*)arg1;
	const _RegisterInfo* reg2 = (_RegisterInfo*)arg2;

	return tstricmp(reg1->funcname,reg2->funcname);
}

CHAR* GetFuncName(CHAR* pStr1,CHAR* pStr2)
{
	memset(strbuf,0,MAX_FILE_PATH);
	Q_strncpyz(strbuf,pStr1,MAX_FILE_PATH);
	
	//"INT aaa ()"
	//�ҵ���������ͷָ��
	//�ҵ���������βָ��
	
	CHAR* temp = strbuf;
	temp = temp + 3;

	BOOL toggle = FALSE;
	while(*temp++)
	{
		if(isalpha(*temp))
			break;
	}
	
	CHAR* temp2 = temp;
	while(*temp2++)
	{
		if(!isalpha(*temp2) && !isdigit(*temp2) && *temp2 != '_')
		{
			*temp2 = '\0';
			break;
		}
	}
	return temp;
}

_RegisterInfo regInfo[FUNCNUM];
CHAR funcname[FUNCNUM][NAMELEN];

INT numFunc = 0;
INT numReg = 0;

//������鿼��ɨ�豾��Ŀ¼�õ�?
CHAR* pLuaFnTblFile[] =
{
	"LuaFnTbl_Ability.h",
	"LuaFnTbl_Attr.h",
	"LuaFnTbl_Battle.h",
	"LuaFnTbl_DoAction.h",
	"LuaFnTbl_Guild.h",
	"LuaFnTbl_Misc.h",
	"LuaFnTbl_Mission.h",
	"LuaFnTbl_Pet.h",
	"LuaFnTbl_PetPlacard.h",
	"LuaFnTbl_Relation.h",
	"LuaFnTbl_Scene.h",
	"LuaFnTbl_Shop.h",
	"LuaFnTbl_Team.h",
	"LuaFnRegister.cpp",
	'\0'
};

//��Lua�������ļ�
VOID ReadLuaFnTblFiles()
{
	CHAR** pScanFile = pLuaFnTblFile;

	//ɨ���ļ�
	while(*pScanFile)
	{
		CHAR buf[256];
		tsnprintf(buf,256,__FILE__);
		CHAR* pStr = strrchr(buf,'\\');
		strcpy(pStr+1,*pScanFile);
		FILE* fp = fopen(buf,"r");

		if( fp != NULL) 
		{
			CHAR line[_MAX_PATH * 4];
			
	
			CHAR *pStr1,*pStr2,*pStr3,*pStr4;
			INT linenum = 0;
			BOOL commentBegin = FALSE;
			BOOL beginNamaSpace = FALSE;
			while( fgets( line, sizeof( line), fp)) 
			{	
				if(strstr(line,"namespace"))
					beginNamaSpace = TRUE;
				if(!beginNamaSpace)
					continue;
		
				if(strstr(line,"endnamespace"))
					break;

				//�ҵ���������
				if( (pStr1 = strstr(line,"INT")) && (pStr2 = strstr(line,"Lua_State")) && (pStr2 > pStr1))//�� 
				{
					Assert((pStr2 - pStr1) <= NAMELEN);
					if((pStr2 - pStr1) <= NAMELEN)
					{
						const CHAR* temp = GetFuncName(pStr1,pStr2);
						Q_strncpyz(funcname[numFunc++],temp,NAMELEN);
					}
				}

				//�ҵ���ע�ắ��,û�п��ǿո�
				if((pStr1 = strstr(line,"FuncProto")))
				{
					pStr2 = strstr(line,")");
					pStr1 = pStr1 + strlen("FuncProto") + 1;

					pStr3 = strchr(line,'"');
					pStr4 = strchr(pStr3+1,'"');

					char szFuncName[NAMELEN];
					char szRegName[NAMELEN];

					Q_strncpyz(szFuncName,pStr1,pStr2 - pStr1 + 1);//��һ
					Q_strncpyz(szRegName,pStr3 + 1,pStr4 - pStr3);

					BOOL toggle = FALSE; 
					INT i;
					for(i =0;i<numReg;i++)
					{
						if(!strcmp(regInfo[i].funcname,szFuncName))
						{
							toggle = TRUE;
							break;
						}
					}
				
					if(!toggle)
					{//��һ��ע��ú���
						Q_strncpyz(regInfo[numReg++].funcname,szFuncName,NAMELEN);
						Q_strncpyz(regInfo[numReg - 1].regname[regInfo[numReg -1].regCount++],szRegName,NAMELEN);						
					}
					else
					{
						Q_strncpyz(regInfo[i].regname[regInfo[i].regCount++],szRegName,NAMELEN);						
					}
				}
			}
			fclose(fp);

			
		}

		pScanFile++;//ɨ����һ���ļ�
	}
}

VOID ParseTheResult()
{
	qsort(funcname,numFunc,NAMELEN,compare);
	qsort(regInfo,numReg,sizeof(_RegisterInfo),compareRegInfo);

	//����
	//numRegע�ắ����Ŀ
	//numFuncʵ�ʺ�����Ŀ
	//ֻ�����һ������������ע����ע��`
	if(numReg != numFunc)
	{
		INT inuseDecl[FUNCNUM];
		memset(inuseDecl,0,sizeof(INT)*FUNCNUM);

		INT startindex = 0;
		INT ret = 0;
		for(INT i=0;i<FUNCNUM;i++)
		{
			for(INT j=startindex;j<FUNCNUM;j++)
			{
				ret = strcmp(regInfo[j].funcname,funcname[i]);
				if( ret > 0)
				{
					break;
				}
				else if (ret == 0)
				{
					startindex = j + 1;
					inuseDecl[i] = 1;
					break;
				}
			}
		}

		CHAR buf[_MAX_PATH];
		tsnprintf(buf,sizeof(buf)-1,"Attention : register & func num mismatch : numReg = %d : numFunc = %d\n",numReg,numFunc);

		Log::SaveLog( SERVER_FUNCTIONFILE, buf) ;

		Log::SaveLog( SERVER_FUNCTIONFILE, "\n") ;
		Log::SaveLog( SERVER_FUNCTIONFILE, "No regisgter begin :\n") ;

		for(INT i=0;i<numFunc;i++)
		{
			if(!inuseDecl[i])
				Log::SaveLog( SERVER_FUNCTIONFILE,"%s\n",funcname[i]) ;
		}

		Log::SaveLog( SERVER_FUNCTIONFILE, "No register end\n") ;
	}
	else
	{
		FILE* fp = fopen(SERVER_FUNCTIONFILE,"w");
		if(fp)
		{
			fclose(fp);
		}
	}
}

//д�ļ�
VOID WriteScriptFile()
{
	CHAR buf[256];
	tsnprintf(buf,256,__FILE__);
	CHAR* pStr = strrchr(buf,'\\');
	strcpy(pStr+1,"ScriptFuncForDesigner.h");
	FILE* fpScriptFile = fopen(buf,"w");

	CHAR** pScanFile = pLuaFnTblFile;
	while(*pScanFile)
	{
		CHAR buf[_MAX_PATH];
		tsnprintf(buf,256,__FILE__);
		CHAR* pStr = strrchr(buf,'\\');
		strcpy(pStr+1,*pScanFile);
		FILE* fp = fopen(buf,"r");

		if( fp != NULL  && fpScriptFile != NULL) 
		{
			//д�ļ�ͷ
			fprintf(fpScriptFile,"%s\t%s\n","�ű�ϵͳC���������ĵ�",*pScanFile);

			CHAR line[_MAX_PATH * 4];
			
	
			CHAR *pStr1,*pStr2;//,*pStr3,*pStr4;
			INT linenum = 0;
			BOOL commentBegin = FALSE;
			BOOL beginNamaSpace = FALSE;
			BOOL bDelim = FALSE;
			while( fgets( line, sizeof( line), fp)) 
			{	
				if(strstr(line,"namespace"))
					beginNamaSpace = TRUE;
				if(!beginNamaSpace)
					continue;
		
				if(strstr(line,"endnamespace"))
					break;


				if(strstr(line,"/**"))
				{
					commentBegin = TRUE;
				}
				if(strstr(line,"*/"))
				{
					fprintf(fpScriptFile,"%s",line);
					commentBegin = FALSE;
				}
				if(commentBegin)
				{
					fprintf(fpScriptFile,"%s",line);
					continue;
				}

				if( (pStr1 = strstr(line,"INT")) && (pStr2 = strstr(line,"Lua_State")) && (pStr2 > pStr1))//�� 
				{
					Assert((pStr2 - pStr1) <= NAMELEN);
					
					const CHAR* temp = GetFuncName(pStr1,pStr2);
					
					for(int i=0;i<numReg;i++)
					{
						if(!strcmp(regInfo[i].funcname,temp))
						{
							for(int j=0;j<regInfo[i].regCount;j++)
								fprintf(fpScriptFile,"\t�ú���ע��Ϊ %s\n",regInfo[i].regname[j]);
						}
					}
					
					//�޳�Lua_State����
					/*char* pDelim = strchr(line,',');
					if(pDelim){
						char szLine[_MAX_PATH];
						memset(szLine,0,_MAX_PATH);
						strncpy(szLine,line,pStr2-line);
						strcat(szLine,++pDelim);
						fprintf(fpScriptFile,"%s\n\n",szLine);
					}
					else{*/
						//fprintf(fpScriptFile,"%s\n\n",line);
					//}
					
						char* pDelim = strchr(line,'{');
						if(pDelim == NULL){
							bDelim = TRUE;
							//fprintf(fpScriptFile,"%s",line);
						}
						else{
							fprintf(fpScriptFile,"%s\n\n",line);
						}
					

				}

				if(strchr(line,'{') && bDelim == TRUE)
				{
					fprintf(fpScriptFile,"\n\n");
					bDelim = FALSE;
				}
				if(bDelim)
				{
					fprintf(fpScriptFile,"%s",line);
				}

			}
			fclose(fp);
			
		}

		pScanFile++;//����һ���ļ�
	}

	if(fpScriptFile)
	{
		//д�ļ�β
		fprintf(fpScriptFile,"�ĵ���������:%d��%d��%d��%dʱ",g_pTimeManager->GetYear(),g_pTimeManager->GetMonth(),g_pTimeManager->GetDay(),g_pTimeManager->GetHour());
		fclose(fpScriptFile);
	}
}

//��Ҫ���LuaFnע��ͷ�ĺ���
VOID NeedAddLuaFn()
{
	//�ļ�·����
	CHAR buf[256];
	tsnprintf(buf,256,__FILE__);
	CHAR* pStr = strrchr(buf,'\\');
	strcpy(pStr+1,"NeedAddLuaFn.h");
	FILE* fpScriptFile = NULL;

	for(INT i=0;i<numReg;i++)
	{
		BOOL toggle = FALSE;
		for(INT j=0;j<regInfo[i].regCount;j++)
		{
			if(!strcmp(regInfo[i].funcname,regInfo[i].regname[j]))
			{
				toggle = TRUE;
				break;
			}
			
		}

		if(!toggle)
		{
			if(!fpScriptFile)
				fpScriptFile = fopen(buf,"w");
			
			//��ʽ
			//{"AddEventList",FuncProto(LuaFnAddNumText)},
			fprintf(fpScriptFile,"{\"%s\",FuncProto(%s)},\n",regInfo[i].funcname,regInfo[i].funcname);
		}
	}

	if(fpScriptFile)
		fclose(fpScriptFile);
}

//����Lua�ű��д��滻ע�ắ��
//ExportLuaFn.h�ļ���LuaTool��ȡ
VOID ExportRegName()
{
	//�ļ�·����
	CHAR buf[256];
	tsnprintf(buf,256,__FILE__);
	CHAR* pStr = strrchr(buf,'\\');
	strcpy(pStr+1,"ExportLuaFn.h");
	FILE* fpScriptFile = NULL;

	for(INT i=0;i<numReg;i++)
	{
		BOOL toggle = FALSE;
		for(INT j=0;j<regInfo[i].regCount;j++)
		{
			if(strcmp(regInfo[i].funcname,regInfo[i].regname[j]))
			{
				if(!fpScriptFile)
					fpScriptFile = fopen(buf,"w");
				if(toggle == FALSE)
				{
					toggle = TRUE;
					fprintf(fpScriptFile,"\n");					
				}

				//��ʽ
				//{"AddEventList",},
				fprintf(fpScriptFile,"\"%s\"",regInfo[i].regname[j]);
				
			}
		}
		if(toggle == TRUE)
			fprintf(fpScriptFile,",%s",regInfo[i].funcname);
	}

	if(fpScriptFile)
		fclose(fpScriptFile);
}

VOID InitParse()
{
	numFunc = 0;
	numReg = 0;
	memset(funcname,0,FUNCNUM * NAMELEN);
	memset(regInfo,0,FUNCNUM * sizeof(_RegisterInfo));
}

VOID LuaCFuncRegister::ValidateFuncs()
{
#ifdef __WINDOWS__
	static INT runtime = 0;
	if(runtime++ != 0)
		return;
	
	InitParse();

	ReadLuaFnTblFiles();
	ParseTheResult();
	WriteScriptFile();	
	NeedAddLuaFn();
	ExportRegName();
#endif
	


}

#endif

VOID LuaCFuncRegister::RegisterCFunction()
{
	__ENTER_FUNCTION

#define GENERATE_HEADER
#ifdef _DEBUG
#ifdef GENERATE_HEADER
	ValidateFuncs();
#endif
#endif

	for(INT i=0; i<sizeof(LuaFnTbl::functbl)/sizeof(_Str2Func); i++)
	{
		mOwner->mLua.RegisterFunction(LuaFnTbl::functbl[i].funcname,(VOID*)(LuaFnTbl::functbl[i].proto));
	}
	
	__LEAVE_FUNCTION	
}

VOID LuaCFuncRegister::SetOwner(LuaInterface* pInter)
{
	__ENTER_FUNCTION
	
	mOwner = pInter;

	__LEAVE_FUNCTION
}

LuaInterface* LuaCFuncRegister::GetOwner()
{
	__ENTER_FUNCTION
		Assert(mOwner);
	__LEAVE_FUNCTION
	return mOwner;
}


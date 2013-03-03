CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT (WORLD)

ADD_DEFINITIONS(-D_FOX_WORLD)
ADD_DEFINITIONS(-D_FOX_LOGIN)
ADD_DEFINITIONS(-D_FOX_SERVER)
ADD_DEFINITIONS(-D_LOG_WORLD)
ADD_DEFINITIONS(-D_USE_32BIT_TIME_T)
ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)

IF(CMAKE_SYSTEM MATCHES Linux)
  ADD_DEFINITIONS(-D__LINUX__)
ELSE(CMAKE_SYSTEM MATCHES Linux)
  ADD_DEFINITIONS(-D__WINDOWS__)
ENDIF(CMAKE_SYSTEM MATCHES Linux)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

SET(EXECUTABLE_OUTPUT_PATH ${TF_CURRENT_BINARY_PATH_BIN})

##update_begin


INCLUDE_DIRECTORIES(..\\..\\Common\\Packets)
INCLUDE_DIRECTORIES(..\\..\\Server\\Server\\SMU)
INCLUDE_DIRECTORIES(..\\..\\Common)
INCLUDE_DIRECTORIES(WorldData)
INCLUDE_DIRECTORIES(Main)
INCLUDE_DIRECTORIES(Packets)
INCLUDE_DIRECTORIES(..\\..\\Common\\DataBase)
INCLUDE_DIRECTORIES(..\\..\\Server\\Server\\Base)


SET (BASE_LIST
	../../Server/Server/Base/Config.cpp
	../../Server/Server/Base/Config.h
	../../Server/Server/Base/File.cpp
	../../Server/Server/Base/File.h
	../../Server/Server/Base/IDTable.cpp
	../../Server/Server/Base/IDTable.h
	../../Server/Server/Base/Ini.cpp
	../../Server/Server/Base/Ini.h
	../../Server/Server/Base/Log.cpp
	../../Server/Server/Base/Log.h
	../../Server/Server/Base/StrTable.cpp
	../../Server/Server/Base/StrTable.h
	../../Server/Server/Base/Thread.cpp
	../../Server/Server/Base/Thread.h
	../../Server/Server/Base/TimeManager.cpp
	../../Server/Server/Base/TimeManager.h
)

SET (PACKETS_CITY_LIST
	Packets/GWCityApplyNewCityHandler.cpp
	Packets/GWCityAskInitInfoHandler.cpp
	Packets/GWCityCloseHandler.cpp
	Packets/GWCityCloseSuccessHandler.cpp
	Packets/GWCityOptHandler.cpp
)

SET (PACKETS_TEAM_LIST
	Packets/GWAskTeamInfoHandler.cpp
	Packets/GWAskTeamMemberInfoHandler.cpp
	Packets/GWAskTeamRecruitInfoHandler.cpp
	Packets/GWRegisterTeamRecruitHandler.cpp
	Packets/GWSetTeamDistribModeHandler.cpp
	Packets/GWTeamApplyHandler.cpp
	Packets/GWTeamAppointHandler.cpp
	Packets/GWTeamDismissHandler.cpp
	Packets/GWTeamInviteHandler.cpp
	Packets/GWTeamKickHandler.cpp
	Packets/GWTeamLeaderDieHandler.cpp
	Packets/GWTeamLeaderRetInviteHandler.cpp
	Packets/GWTeamLeaveHandler.cpp
	Packets/GWTeamMemberEnterSceneHandler.cpp
	Packets/GWTeamRetApplyHandler.cpp
	Packets/GWTeamRetInviteHandler.cpp
)

SET (PACKETS_COUNTRY_LIST
	Packets/GWCountryAPHandler.cpp
	Packets/GWCountryAppointHandler.cpp
	Packets/GWCountryCollectHandler.cpp
	Packets/GWCountryDelateHandler.cpp
	Packets/GWCountryDelateRequestHandler.cpp
	Packets/GWCountryDonativeHandler.cpp
	Packets/GWCountryFetchHandler.cpp
	Packets/GWCountryInfoRequestHandler.cpp
	Packets/GWCountryRemoveHandler.cpp
	Packets/GWCountryRequestKindomHandler.cpp
	Packets/GWCountryVoteOnHandler.cpp
	Packets/GWCountryVoteRequestHandler.cpp
)

SET (PACKETS_LIST
	Packets/GGSceneNotifyHandler.cpp
	Packets/GWAskChangeSceneHandler.cpp
	Packets/GWAskMailHandler.cpp
	Packets/GWAskOtherOnlineAttrib_HumanHandler.cpp
	Packets/GWAskSceneDataHandler.cpp
	Packets/GWAskUserDataHandler.cpp
	Packets/GWBatchMailHandler.cpp
	Packets/GWCallOfHumanHandler.cpp
	Packets/GWCancelHorseFindMateInfoHandler.cpp
	Packets/GWChangeMenpaiHandler.cpp
	Packets/GWChannelCreateHandler.cpp
	Packets/GWChannelDismissHandler.cpp
	Packets/GWChannelInviteHandler.cpp
	Packets/GWChannelKickHandler.cpp
	Packets/GWCharUpdateCurTitleHandler.cpp
	Packets/GWChatHandler.cpp
	Packets/GWCommandHandler.cpp
	Packets/GWDetailOtherOnlineAttrib_HorseHandler.cpp
	Packets/GWDetailOtherOnlineAttrib_HumanHandler.cpp
	Packets/GWDetailOtherOnlineEquipList_HumanHandler.cpp
	Packets/GWEnterTeamFollowHandler.cpp
	Packets/GWFingerHandler.cpp
	Packets/GWGuildHandler.cpp
	Packets/GWHeartBeatHandler.cpp
	Packets/GWHorseChangeStateHandler.cpp
	Packets/GWIssueHorseFindMateInfoHandler.cpp
	Packets/GWLevelUpHandler.cpp
	Packets/GWMailHandler.cpp
	Packets/GWNotifyUserHandler.cpp
	Packets/GWQueryHorseFindMateInfoHandler.cpp
	Packets/GWRefreshDataToWorldHandler.cpp
	Packets/GWRelationHandler.cpp
	Packets/GWSearchHorseFindMateInfoHandler.cpp
	Packets/GWSearchOnlineUserHandler.cpp
	Packets/GWStopTeamFollowHandler.cpp
	Packets/GWTopListHandler.cpp
	Packets/GWUpdateTitleHandler.cpp
	Packets/LWAskCharLoginHandler.cpp
	Packets/LWChangeNameHandler.cpp
	Packets/LWNotifyUserHandler.cpp
	Packets/LWRetBillingEndHandler.cpp
	Packets/LWRetBillingKeepHandler.cpp
	Packets/LWRetBillingStartHandler.cpp
	Packets/LWRetPrizeHandler.cpp
	Packets/SSConnectHandler.cpp
	Packets/SSScenePlayerCountHandler.cpp
	Packets/WGHandler.cpp
	Packets/WLHandler.cpp
)

SET (MAIN_LIST
	Main/Player.cpp
	Main/Player.h
	Main/PlayerManager.cpp
	Main/PlayerManager.h
	Main/PlayerPool.cpp
	Main/PlayerPool.h
	Main/ServerManager.cpp
	Main/ServerManager.h
	Main/ServerPlayer.cpp
	Main/ServerPlayer.h
	Main/ShareMemManager.cpp
	Main/ShareMemManager.h
	Main/World.cpp
	Main/World.h
	Main/WorldTable.cpp
	Main/WorldTable.h
)

SET (COMMON_PACKETS_CITY_LIST
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

SET (COMMON_PACKETS_TEAM_LIST
	../../Common/Packets/GWAskTeamRecruitInfo.cpp
	../../Common/Packets/GWAskTeamRecruitInfo.h
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
)

SET (COMMON_PACKETS_LOGIN_LIST
	../../Common/Packets/LWAskCharLogin.cpp
	../../Common/Packets/LWAskCharLogin.h
	../../Common/Packets/LWChangeName.cpp
	../../Common/Packets/LWChangeName.h
	../../Common/Packets/LWNotifyUser.cpp
	../../Common/Packets/LWNotifyUser.h
	../../Common/Packets/LWRetBillingEnd.cpp
	../../Common/Packets/LWRetBillingEnd.h
	../../Common/Packets/LWRetBillingKeep.cpp
	../../Common/Packets/LWRetBillingKeep.h
	../../Common/Packets/LWRetBillingStart.cpp
	../../Common/Packets/LWRetBillingStart.h
	../../Common/Packets/LWRetPrize.cpp
	../../Common/Packets/LWRetPrize.h
	../../Common/Packets/WLAskPrize.cpp
	../../Common/Packets/WLAskPrize.h
	../../Common/Packets/WLBillingEnd.cpp
	../../Common/Packets/WLBillingEnd.h
	../../Common/Packets/WLBillingKeep.cpp
	../../Common/Packets/WLBillingKeep.h
	../../Common/Packets/WLBillingKick.cpp
	../../Common/Packets/WLBillingKick.h
	../../Common/Packets/WLBillingStart.cpp
	../../Common/Packets/WLBillingStart.h
	../../Common/Packets/WLChangeName.cpp
	../../Common/Packets/WLChangeName.h
	../../Common/Packets/WLNotifyUser.cpp
	../../Common/Packets/WLNotifyUser.h
	../../Common/Packets/WLRetCharLogin.cpp
	../../Common/Packets/WLRetCharLogin.h
)

SET (COMMON_PACKETS_LIST
	../../Common/Packets/GWAskChangeScene.cpp
	../../Common/Packets/GWAskChangeScene.h
	../../Common/Packets/GWAskMail.cpp
	../../Common/Packets/GWAskMail.h
	../../Common/Packets/GWAskOtherOnlineAttrib_Human.cpp
	../../Common/Packets/GWAskOtherOnlineAttrib_Human.h
	../../Common/Packets/GWAskSceneData.cpp
	../../Common/Packets/GWAskSceneData.h
	../../Common/Packets/GWAskTeamInfo.cpp
	../../Common/Packets/GWAskTeamInfo.h
	../../Common/Packets/GWAskTeamMemberInfo.cpp
	../../Common/Packets/GWAskTeamMemberInfo.h
	../../Common/Packets/GWAskUserData.cpp
	../../Common/Packets/GWAskUserData.h
	../../Common/Packets/GWBatchMail.cpp
	../../Common/Packets/GWBatchMail.h
	../../Common/Packets/GWCallOfHuman.cpp
	../../Common/Packets/GWCallOfHuman.h
	../../Common/Packets/GWCancelHorseFindMateInfo.cpp
	../../Common/Packets/GWCancelHorseFindMateInfo.h
	../../Common/Packets/GWChangeMenpai.cpp
	../../Common/Packets/GWChangeMenpai.h
	../../Common/Packets/GWChannelCreate.cpp
	../../Common/Packets/GWChannelCreate.h
	../../Common/Packets/GWChannelDismiss.cpp
	../../Common/Packets/GWChannelDismiss.h
	../../Common/Packets/GWChannelInvite.cpp
	../../Common/Packets/GWChannelInvite.h
	../../Common/Packets/GWChannelKick.cpp
	../../Common/Packets/GWChannelKick.h
	../../Common/Packets/GWCharUpdateCurTitle.cpp
	../../Common/Packets/GWCharUpdateCurTitle.h
	../../Common/Packets/GWChat.cpp
	../../Common/Packets/GWChat.h
	../../Common/Packets/GWCommand.cpp
	../../Common/Packets/GWCommand.h
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
	../../Common/Packets/GWDetailOtherOnlineAttrib_Horse.cpp
	../../Common/Packets/GWDetailOtherOnlineAttrib_Horse.h
	../../Common/Packets/GWDetailOtherOnlineAttrib_Human.cpp
	../../Common/Packets/GWDetailOtherOnlineAttrib_Human.h
	../../Common/Packets/GWDetailOtherOnlineEquipList_Human.cpp
	../../Common/Packets/GWDetailOtherOnlineEquipList_Human.h
	../../Common/Packets/GWEnterTeamFollow.cpp
	../../Common/Packets/GWEnterTeamFollow.h
	../../Common/Packets/GWFinger.cpp
	../../Common/Packets/GWFinger.h
	../../Common/Packets/GWGuild.cpp
	../../Common/Packets/GWGuild.h
	../../Common/Packets/GWHeartBeat.cpp
	../../Common/Packets/GWHeartBeat.h
	../../Common/Packets/GWHorseChangeState.cpp
	../../Common/Packets/GWHorseChangeState.h
	../../Common/Packets/GWIssueHorseFindMateInfo.cpp
	../../Common/Packets/GWIssueHorseFindMateInfo.h
	../../Common/Packets/GWLevelUp.cpp
	../../Common/Packets/GWLevelUp.h
	../../Common/Packets/GWMail.cpp
	../../Common/Packets/GWMail.h
	../../Common/Packets/GWNotifyUser.cpp
	../../Common/Packets/GWNotifyUser.h
	../../Common/Packets/GWQueryHorseFindMateInfo.cpp
	../../Common/Packets/GWQueryHorseFindMateInfo.h
	../../Common/Packets/GWRefreshDataToWorld.cpp
	../../Common/Packets/GWRefreshDataToWorld.h
	../../Common/Packets/GWRelation.cpp
	../../Common/Packets/GWRelation.h
	../../Common/Packets/GWSearchHorseFindMateInfo.cpp
	../../Common/Packets/GWSearchHorseFindMateInfo.h
	../../Common/Packets/GWSearchOnlineUser.cpp
	../../Common/Packets/GWSearchOnlineUser.h
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
	../../Common/Packets/GWTopList.cpp
	../../Common/Packets/GWTopList.h
	../../Common/Packets/GWUpdateTitle.cpp
	../../Common/Packets/GWUpdateTitle.h
	../../Common/Packets/SSConnect.cpp
	../../Common/Packets/SSConnect.h
	../../Common/Packets/SSScenePlayerCount.cpp
	../../Common/Packets/SSScenePlayerCount.h
	../../Common/Packets/WGAskOtherOnlineAttrib_Human.cpp
	../../Common/Packets/WGAskOtherOnlineAttrib_Human.h
	../../Common/Packets/WGCallOfHuman.cpp
	../../Common/Packets/WGCallOfHuman.h
	../../Common/Packets/WGChannelError.cpp
	../../Common/Packets/WGChannelError.h
	../../Common/Packets/WGChannelResult.cpp
	../../Common/Packets/WGChannelResult.h
	../../Common/Packets/WGChat.cpp
	../../Common/Packets/WGChat.h
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
	../../Common/Packets/WGDetailOtherOnlineAttrib_Horse.cpp
	../../Common/Packets/WGDetailOtherOnlineAttrib_Horse.h
	../../Common/Packets/WGDetailOtherOnlineAttrib_Human.cpp
	../../Common/Packets/WGDetailOtherOnlineAttrib_Human.h
	../../Common/Packets/WGDetailOtherOnlineEquipList_Human.cpp
	../../Common/Packets/WGDetailOtherOnlineEquipList_Human.h
	../../Common/Packets/WGFinger.cpp
	../../Common/Packets/WGFinger.h
	../../Common/Packets/WGGuild.cpp
	../../Common/Packets/WGGuild.h
	../../Common/Packets/WGGuildError.cpp
	../../Common/Packets/WGGuildError.h
	../../Common/Packets/WGGuildReturn.cpp
	../../Common/Packets/WGGuildReturn.h
	../../Common/Packets/WGMail.cpp
	../../Common/Packets/WGMail.h
	../../Common/Packets/WGNotifyMail.cpp
	../../Common/Packets/WGNotifyMail.h
	../../Common/Packets/WGNotifyUser.cpp
	../../Common/Packets/WGNotifyUser.h
	../../Common/Packets/WGOperateResult.cpp
	../../Common/Packets/WGOperateResult.h
	../../Common/Packets/WGRelation.cpp
	../../Common/Packets/WGRelation.h
	../../Common/Packets/WGRetChangeScene.cpp
	../../Common/Packets/WGRetChangeScene.h
	../../Common/Packets/WGRetSceneData.cpp
	../../Common/Packets/WGRetSceneData.h
	../../Common/Packets/WGReturnHorseFindMateInfo.cpp
	../../Common/Packets/WGReturnHorseFindMateInfo.h
	../../Common/Packets/WGReturnOnlineUser.cpp
	../../Common/Packets/WGReturnOnlineUser.h
	../../Common/Packets/WGRetUserData.cpp
	../../Common/Packets/WGRetUserData.h
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
	../../Common/Packets/WGTopList.cpp
	../../Common/Packets/WGTopList.h
	../../Common/Packets/WGUpdateTitle.cpp
	../../Common/Packets/WGUpdateTitle.h
	../../Common/Packets/WGWorldTime.cpp
	../../Common/Packets/WGWorldTime.h
)

SET (COMMON_DATABASE_LIST
	../../Common/DataBase/WXSJ_DBC.cpp
	../../Common/DataBase/WXSJ_DBC.h
)

SET (COMMON_LIST
	../../Common/Array_Base.cpp
	../../Common/Array_Base.h
	../../Common/Assertx.cpp
	../../Common/Assertx.h
	../../Common/BaseType.h
	../../Common/Chain.cpp
	../../Common/Chain.h
	../../Common/DB_Struct.cpp
	../../Common/DB_Struct.h
	../../Common/FileAPI.h
	../../Common/GameDefine.h
	../../Common/GameDefine2.h
	../../Common/GameDefine_Attr.h
	../../Common/GameDefine_City.h
	../../Common/GameDefine_Guild.h
	../../Common/GameDefine_Item.h
	../../Common/GameDefine_Result.cpp
	../../Common/GameDefine_Result.h
	../../Common/GameDefine_Scene.h
	../../Common/GameStruct.cpp
	../../Common/GameStruct.h
	../../Common/GameStruct_City.h
	../../Common/GameStruct_Country.h
	../../Common/GameStruct_Finger.cpp
	../../Common/GameStruct_Finger.h
	../../Common/GameStruct_Guild.h
	../../Common/GameStruct_Item.h
	../../Common/GameStruct_Query.h
	../../Common/GameStruct_Relation.cpp
	../../Common/GameStruct_Relation.h
	../../Common/GameStruct_Skill.h
	../../Common/GameStruct_Team.h
	../../Common/GameStruct_TopList.h
	../../Common/GameUtil.cpp
	../../Common/GameUtil.h
	../../Common/Minidump.cpp
	../../Common/Minidump.h
	../../Common/OResultDef.h
	../../Common/PacketDefine.h
	../../Common/PacketFactory.h
	../../Common/PacketFactoryManager.cpp
	../../Common/PacketFactoryManager.h
	../../Common/PacketStruct.cpp
	../../Common/PacketStruct.h
	../../Common/PlayerStatus.h
	../../Common/Rand.cpp
	../../Common/Rand.h
	../../Common/ScriptCommandStruct.cpp
	../../Common/ScriptCommandStruct.h
	../../Common/ServerSocket.h
	../../Common/ShareMemAPI.cpp
	../../Common/ShareMemAPI.h
	../../Common/Type.h
)

SET (WORLDDATA_LIST
	WorldData/AllUser.cpp
	WorldData/AllUser.h
	WorldData/ChatCenter.cpp
	WorldData/ChatCenter.h
	WorldData/City.cpp
	WorldData/City.h
	WorldData/CityManager.cpp
	WorldData/CityManager.h
	WorldData/Country.cpp
	WorldData/Country.h
	WorldData/CountryManager.cpp
	WorldData/CountryManager.h
	WorldData/Guild.cpp
	WorldData/Guild.h
	WorldData/GuildManager.cpp
	WorldData/GuildManager.h
	WorldData/HorseFindMateManager.cpp
	WorldData/HorseFindMateManager.h
	WorldData/MailCenter.cpp
	WorldData/MailCenter.h
	WorldData/MailDefine.cpp
	WorldData/MailDefine.h
	WorldData/MailPool.cpp
	WorldData/MailPool.h
	WorldData/OnlineUser.cpp
	WorldData/OnlineUser.h
	WorldData/SceneInfo.cpp
	WorldData/SceneInfo.h
	WorldData/Team.cpp
	WorldData/Team.h
	WorldData/TeamRecruitManager.cpp
	WorldData/TeamRecruitManager.h
	WorldData/TopList.cpp
	WorldData/TopList.h
	WorldData/TopListManager.cpp
	WorldData/TopListManager.h
	WorldData/User.cpp
	WorldData/User.h
	WorldData/User.inl
	WorldData/WorldTimeInfo.cpp
	WorldData/WorldTimeInfo.h
)

SET (SMU_LIST
	../../Server/Server/SMU/ShareMemAO.cpp
	../../Server/Server/SMU/ShareMemAO.h
	../../Server/Server/SMU/SMUManager.cpp
	../../Server/Server/SMU/SMUManager.h
)

SET (STDAFX_LIST
	stdafx.cpp
	stdafx.h
)



if (WIN32)
	source_group(Base FILES ${BASE_LIST})
	source_group(Packets\\City FILES ${PACKETS_CITY_LIST})
	source_group(Packets\\Team FILES ${PACKETS_TEAM_LIST})
	source_group(Packets\\Country FILES ${PACKETS_COUNTRY_LIST})
	source_group(Packets FILES ${PACKETS_LIST})
	source_group(Main FILES ${MAIN_LIST})
	source_group(Common\\Packets\\City FILES ${COMMON_PACKETS_CITY_LIST})
	source_group(Common\\Packets\\Team FILES ${COMMON_PACKETS_TEAM_LIST})
	source_group(Common\\Packets\\Login FILES ${COMMON_PACKETS_LOGIN_LIST})
	source_group(Common\\Packets FILES ${COMMON_PACKETS_LIST})
	source_group(Common\\DataBase FILES ${COMMON_DATABASE_LIST})
	source_group(Common FILES ${COMMON_LIST})
	source_group(WorldData FILES ${WORLDDATA_LIST})
	source_group(SMU FILES ${SMU_LIST})
	source_group(StdAfx FILES ${STDAFX_LIST})
endif()


ADD_EXECUTABLE(World
	${BASE_LIST}
	${PACKETS_CITY_LIST}
	${PACKETS_TEAM_LIST}
	${PACKETS_COUNTRY_LIST}
	${PACKETS_LIST}
	${MAIN_LIST}
	${COMMON_PACKETS_CITY_LIST}
	${COMMON_PACKETS_TEAM_LIST}
	${COMMON_PACKETS_LOGIN_LIST}
	${COMMON_PACKETS_LIST}
	${COMMON_DATABASE_LIST}
	${COMMON_LIST}
	${WORLDDATA_LIST}
	${SMU_LIST}
	${STDAFX_LIST}
)


##update_end

if (WIN32)
TARGET_LINK_LIBRARIES(World FoxNet)
else()
TARGET_LINK_LIBRARIES(World FoxNet pthread)
endif()
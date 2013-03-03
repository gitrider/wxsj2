
#include "stdafx.h"
#include "GWNotifyUser.h"




uint GWNotifyUserHandler::Execute( GWNotifyUser* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


#include "GWMail.h"




uint GWMailHandler::Execute( GWMail* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GWLevelUp.h"



uint GWLevelUpHandler::Execute(GWLevelUp* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
#include "GWGuild.h"

uint GWGuildHandler::Execute( GWGuild* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}


#include "GWCommand.h"




uint GWCommandHandler::Execute( GWCommand* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


#include "GWChannelKick.h"




uint GWChannelKickHandler::Execute( GWChannelKick* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


#include "GWChat.h"




uint GWChatHandler::Execute( GWChat* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GWChannelInvite.h"




uint GWChannelInviteHandler::Execute( GWChannelInvite* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


#include "GWChannelDismiss.h"




uint GWChannelDismissHandler::Execute( GWChannelDismiss* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GWChannelCreate.h"




uint GWChannelCreateHandler::Execute( GWChannelCreate* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GWBatchMail.h"

uint GWBatchMailHandler::Execute( GWBatchMail* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

#include "GWAskUserData.h"




uint GWAskUserDataHandler::Execute( GWAskUserData* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GWAskSceneData.h"




uint GWAskSceneDataHandler::Execute( GWAskSceneData* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GWAskMail.h"




uint GWAskMailHandler::Execute( GWAskMail* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GWAskChangeScene.h"




uint GWAskChangeSceneHandler::Execute( GWAskChangeScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GWRelation.h"


uint GWRelationHandler::Execute( GWRelation* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GWTeamRetApply.h"



uint GWTeamRetApplyHandler::Execute( GWTeamRetApply* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GWTeamMemberEnterScene.h"

uint GWTeamMemberEnterSceneHandler::Execute( GWTeamMemberEnterScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

#include "GWTeamLeave.h"



uint GWTeamLeaveHandler::Execute( GWTeamLeave* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GWTeamLeaderRetInvite.h"



uint GWTeamLeaderRetInviteHandler::Execute( GWTeamLeaderRetInvite* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GWTeamKick.h"



uint GWTeamKickHandler::Execute( GWTeamKick* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GWTeamInvite.h"



uint GWTeamInviteHandler::Execute( GWTeamInvite* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GWTeamDismiss.h"



uint GWTeamDismissHandler::Execute( GWTeamDismiss* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GWTeamAppoint.h"


uint GWTeamAppointHandler::Execute( GWTeamAppoint* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GWTeamApply.h"



uint GWTeamApplyHandler::Execute( GWTeamApply* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GWAskTeamMemberInfo.h"

uint GWAskTeamMemberInfoHandler::Execute( GWAskTeamMemberInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

#include "GWTeamRetInvite.h"



uint GWTeamRetInviteHandler::Execute( GWTeamRetInvite* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GWAskTeamInfo.h"

uint GWAskTeamInfoHandler::Execute( GWAskTeamInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GWEnterTeamFollow.h"

uint GWEnterTeamFollowHandler::Execute( GWEnterTeamFollow* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

#include "GWStopTeamFollow.h"

uint GWStopTeamFollowHandler::Execute( GWStopTeamFollow* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

#include "GWCityApplyNewCity.h"

uint GWCityApplyNewCityHandler::Execute( GWCityApplyNewCity* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

#include "GWCityAskInitInfo.h"

uint GWCityAskInitInfoHandler::Execute( GWCityAskInitInfo* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

#include "GWCityClose.h"

uint GWCityCloseHandler::Execute( GWCityClose* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

#include "GWCityCloseSuccess.h"

uint GWCityCloseSuccessHandler::Execute( GWCityCloseSuccess* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

#include "GWCityOpt.h"

uint GWCityOptHandler::Execute( GWCityOpt* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

#include "GWUpdateTitle.h"

UINT GWUpdateTitleHandler::Execute( GWUpdateTitle* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

#include "GWChangeMenpai.h"

UINT GWChangeMenpaiHandler::Execute( GWChangeMenpai* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

#include "GWCallOfHuman.h"

UINT GWCallOfHumanHandler::Execute( GWCallOfHuman* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

#include "GWFinger.h"
UINT GWFingerHandler::Execute(GWFinger* pPacket,Player* pPlayer)
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

#include "GWHeartBeat.h"
UINT GWHeartBeatHandler::Execute(GWHeartBeat* pPacket,Player* pPlayer)
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

#include "GWCharUpdateCurTitle.h"
UINT GWCharUpdateCurTitleHandler::Execute( GWCharUpdateCurTitle* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

    return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

    return PACKET_EXE_ERROR;
}

#include "GWCountryAppoint.h"
UINT GWCountryAppointHandler::Execute( GWCountryAppoint* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR;
}

#include "GWCountryRemove.h"
UINT GWCountryRemoveHandler::Execute( GWCountryRemove* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR;
}

#include "GWCountryRequestKindom.h"
UINT GWCountryRequestKindomHandler::Execute( GWCountryRequestKindom* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR;
}

#include "GWTopList.h"
UINT GWTopListHandler::Execute(GWTopList* pPacket,Player* pPlayer)
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

#include "GWCountryInfoRequest.h"
UINT GWCountryInfoRequestHandler::Execute(GWCountryInfoRequest* pPacket,Player* pPlayer)
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR;
}

#include "GWRegisterTeamRecruit.h"
UINT GWRegisterTeamRecruitHandler::Execute(GWRegisterTeamRecruit* pPacket,Player* pPlayer)
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR;
}

#include "GWAskTeamRecruitInfo.h"
UINT GWAskTeamRecruitInfoHandler::Execute(GWAskTeamRecruitInfo* pPacket,Player* pPlayer)
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR;
}

#include "GWIssueHorseFindMateInfo.h"

UINT GWIssueHorseFindMateInfoHandler::Execute( GWIssueHorseFindMateInfo* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR;
}

#include "GWSearchHorseFindMateInfo.h"

UINT GWSearchHorseFindMateInfoHandler::Execute( GWSearchHorseFindMateInfo* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR;
}

#include "GWCancelHorseFindMateInfo.h"

UINT GWCancelHorseFindMateInfoHandler::Execute( GWCancelHorseFindMateInfo* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR;
}

#include "GWHorseChangeState.h"

UINT GWHorseChangeStateHandler::Execute( GWHorseChangeState* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR;
}

#include "GWQueryHorseFindMateInfo.h"
UINT GWQueryHorseFindMateInfoHandler::Execute( GWQueryHorseFindMateInfo* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR;
}


#include "GWAskOtherOnlineAttrib_Human.h"
UINT GWAskOtherOnlineAttrib_HumanHandler::Execute( GWAskOtherOnlineAttrib_Human* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR;
}

#include "GWDetailOtherOnlineAttrib_Human.h"
UINT GWDetailOtherOnlineAttrib_HumanHandler::Execute( GWDetailOtherOnlineAttrib_Human* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

#include "GWRefreshDataToWorld.h"
UINT GWRefreshDataToWorldHandler::Execute(GWRefreshDataToWorld* pPacket, Player* pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

#include "GWSearchOnlineUser.h"
UINT GWSearchOnlineUserHandler::Execute(GWSearchOnlineUser* pPacket, Player* pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}


#include "GWCountryCollect.h"
UINT GWCountryCollectHandler::Execute(GWCountryCollect* pPacket,Player* pPlayer)
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR;
}

#include "GWCountryDonative.h"
UINT GWCountryDonativeHandler::Execute(GWCountryDonative* pPacket,Player* pPlayer)
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR;
}

#include "GWCountryFetch.h"
UINT GWCountryFetchHandler::Execute(GWCountryFetch* pPacket,Player* pPlayer)
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR;
}

#include "GWCountryAP.h"
UINT GWCountryAPHandler::Execute(GWCountryAP* pPacket,Player* pPlayer)
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR;
}

#include "GWCountryDelateRequest.h"
UINT GWCountryDelateRequestHandler::Execute(GWCountryDelateRequest* pPacket,Player* pPlayer)
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR;
}

#include "GWCountryDelate.h"
UINT GWCountryDelateHandler::Execute(GWCountryDelate* pPacket,Player* pPlayer)
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR;
}

#include "GWCountryVoteOn.h"
UINT GWCountryVoteOnHandler::Execute(GWCountryVoteOn* pPacket,Player* pPlayer)
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR;
}

#include "GWCountryVoteRequest.h"
UINT GWCountryVoteRequestHandler::Execute(GWCountryVoteRequest* pPacket,Player* pPlayer)
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR;
}

#include "GWDetailOtherOnlineAttrib_Horse.h"
UINT GWDetailOtherOnlineAttrib_HorseHandler::Execute( GWDetailOtherOnlineAttrib_Horse* pPacket, Player* pPlayer )
{
	return PACKET_EXE_CONTINUE;
}

#include "GWDetailOtherOnlineEquipList_Human.h"
UINT GWDetailOtherOnlineEquipList_HumanHandler::Execute( GWDetailOtherOnlineEquipList_Human* pPacket, Player* pPlayer )
{
	return PACKET_EXE_CONTINUE;
}

#include "GWSetTeamDistribMode.h"
UINT GWSetTeamDistribModeHandler::Execute(GWSetTeamDistribMode* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

#include "GWTeamLeaderDie.h"
UINT GWTeamLeaderDieHandler::Execute(GWTeamLeaderDie* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}


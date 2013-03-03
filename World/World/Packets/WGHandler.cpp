

#include "stdafx.h"
#include "WGTeamResult.h"
#include "WGTeamMemberInfo.h"
#include "WGTeamList.h"
#include "WGTeamLeaderAskInvite.h"
#include "WGTeamFollowList.h"
#include "WGTeamError.h"
#include "WGTeamAskInvite.h"
#include "WGTeamAskApply.h"
#include "WGRetUserData.h"
#include "WGRetSceneData.h"
#include "WGRetChangeScene.h"
#include "WGRelation.h"
#include "WGNotifyUser.h"
#include "WGNotifyMail.h"
#include "WGMail.h"
#include "WGGuildReturn.h"
#include "WGGuild.h"
#include "WGGuildError.h"
#include "WGChat.h"
#include "WGChannelResult.h"
#include "WGChannelError.h"
#include "WGWorldTime.h"
#include "WGCallOfHuman.h"
#include "WGFinger.h"

#include "WGCityInitInfo.h"
#include "WGCityApplyNewCity.h"
#include "WGCityAttr.h"
#include "WGCityClose.h"
#include "WGCityCloseSuccess.h"
#include "WGCityError.h"
#include "WGUpdateTitle.h"
#include "WGCountryReturn.h"
#include "WGCountryInfoConfirm.h"

#include "WGTopList.h"
#include "WGRetTeamRecruitInfo.h"
#include "WGReturnHorseFindMateInfo.h"
#include "WGOperateResult.h"

#include "WGDetailOtherOnlineAttrib_Horse.h"
#include "WGDetailOtherOnlineAttrib_Human.h"
#include "WGReturnOnlineUser.h"
#include "WGAskOtherOnlineAttrib_Human.h"

uint WGTeamResultHandler::Execute( WGTeamResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

uint WGTeamMemberInfoHandler::Execute( WGTeamMemberInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

uint WGTeamListHandler::Execute( WGTeamList* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

uint WGTeamLeaderAskInviteHandler::Execute( WGTeamLeaderAskInvite* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

uint WGTeamFollowListHandler::Execute( WGTeamFollowList* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

uint WGTeamErrorHandler::Execute( WGTeamError* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

uint WGTeamAskInviteHandler::Execute( WGTeamAskInvite* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

uint WGTeamAskApplyHandler::Execute( WGTeamAskApply* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

uint WGRetUserDataHandler::Execute( WGRetUserData* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

uint WGRetSceneDataHandler::Execute( WGRetSceneData* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

uint WGRetChangeSceneHandler::Execute( WGRetChangeScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

uint WGRelationHandler::Execute( WGRelation* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

uint WGNotifyUserHandler::Execute( WGNotifyUser* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

uint WGNotifyMailHandler::Execute( WGNotifyMail* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

uint WGMailHandler::Execute( WGMail* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

uint WGGuildReturnHandler::Execute( WGGuildReturn* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

uint WGGuildHandler::Execute( WGGuild* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

uint WGGuildErrorHandler::Execute( WGGuildError* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

uint WGChatHandler::Execute( WGChat* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

uint WGChannelResultHandler::Execute( WGChannelResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

uint WGChannelErrorHandler::Execute( WGChannelError* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

uint	WGWorldTimeHandler::Execute( WGWorldTime* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

uint	WGCallOfHumanHandler::Execute( WGCallOfHuman* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

uint	WGFingerHandler::Execute( WGFinger* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

uint WGCityApplyNewCityHandler::Execute( WGCityApplyNewCity* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

uint WGCityAttrHandler::Execute( WGCityAttr* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

uint WGCityCloseHandler::Execute( WGCityClose* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

uint WGCityCloseSuccessHandler::Execute( WGCityCloseSuccess* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

uint WGCityErrorHandler::Execute( WGCityError* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

uint WGCityInitInfoHandler::Execute( WGCityInitInfo* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

uint WGUpdateTitleHandler::Execute( WGUpdateTitle* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR ;
}

uint WGCountryReturnHandler::Execute( WGCountryReturn* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR ;
}

uint WGTopListHandler::Execute( WGTopList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

uint WGCountryInfoConfirmHandler::Execute( WGCountryInfoConfirm* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR ;
}


uint WGRetTeamRecruitInfoHandler::Execute( WGRetTeamRecruitInfo* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR ;
}

uint WGReturnHorseFindMateInfoHandler::Execute( WGReturnHorseFindMateInfo* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR ;
}

UINT WGOperateResultHandler::Execute( WGOperateResult* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR ;

}


UINT WGDetailOtherOnlineAttrib_HorseHandler::Execute( WGDetailOtherOnlineAttrib_Horse* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;

}

UINT WGDetailOtherOnlineAttrib_HumanHandler::Execute( WGDetailOtherOnlineAttrib_Human* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;

}

#include "WGDetailOtherOnlineEquipList_Human.h"

UINT WGDetailOtherOnlineEquipList_HumanHandler::Execute(WGDetailOtherOnlineEquipList_Human* pPacket, Player* pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

UINT WGReturnOnlineUserHandler::Execute(WGReturnOnlineUser* pPacket, Player* pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;

}

#include "WGCountryCollect.h"
uint WGCountryCollectHandler::Execute( WGCountryCollect* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR ;
}

#include "WGCountryFetch.h"
uint WGCountryFetchHandler::Execute( WGCountryFetch* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR ;
}

#include "WGCountryDelateConfirm.h"
uint WGCountryDelateConfirmHandler::Execute( WGCountryDelateConfirm* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR ;
}

#include "WGCountryVoteConfirm.h"
uint WGCountryVoteConfirmHandler::Execute( WGCountryVoteConfirm* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

        return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR ;
}

UINT WGAskOtherOnlineAttrib_HumanHandler::Execute(WGAskOtherOnlineAttrib_Human* pPacket, Player* pPlayer)
{
	return PACKET_EXE_CONTINUE;
}

#include "WGRetTeamDistribMode.h"
UINT WGRetTeamDistribModeHandler::Execute(WGRetTeamDistribMode* pPacket, Player* pPlayer)
{
	return PACKET_EXE_CONTINUE;
}

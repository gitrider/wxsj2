/**
功能:CGUnExtraBag消息包处理实现文件
创建:客户端开发
修改记录:
1.因为CG消息包客户端处理可以为空,目前把缺少的Handler都临时补在这个文件中.2009.11.27.修改人:yangfei_ch@hotmail.com 
*/
#include   "StdAfx.h"

#include   "CGUnExtraBag.h"
#include   "CGAskSubTrainSetting.h"
#include   "CGAskSubTrainMode.h"
#include   "CGAskBeginSubTrain.h"
#include   "CGAskGetSubTrainExp.h"
#include   "CGAskEndSubTrain.h"
#include   "CGSetTeamDistribMode.h"
#include   "CGAskTeamDistribMode.h"
#include   "CGModifyPKMode.h"
#include   "CGOpenBoxManipulate.h"
#include   "CGRegisterTeamRecruit.h"
#include   "CGAskTeamRecruitInfo.h"
#include   "CGIssueHorseFindMateInfo.h"
#include   "CGCancelHorseFindMateInfo.h"
#include   "CGSearchHorseFindMateInfo.h"
#include   "CGQueryHorseFindMateInfo.h"
#include   "CGAskOtherOnlineDetailAttrib.h"
#include   "CGSearchOnlineUser.h"
#include   "CGCountryCollect.h"
#include   "CGCountryFetch.h"
#include   "CGCountryDonative.h"
#include   "CGCountryDelate.h"
#include   "CGCountryVoteOn.h"
#include   "GCBankSize.h"
#include   "GCExpandBankSize.h"
#include   "GCUseExtraBagResult.h"
#include   "GCUnExtraBagResult.h"
#include   "GCRetSubTrainSetting.h"
#include   "GCEndSubTrainResult.h"
#include   "GCSubTrainModeResult.h"
#include   "GCSubTrainResult.h"
#include   "GCRetTeamDistribMode.h"
#include   "GCRetTeamRecruitInfo.h"
#include   "GCReturnHorseFindMateInfo.h"
#include   "GCReturnOnlineUser.h"
#include   "GCCountryCollect.h"
#include   "GCCountryFetch.h"
#include   "GCCountryDelateBegin.h"
#include   "GCCountryVoteShow.h"
#include   "GCOpenBox.h"
#include   "CGAskSubTrainMode.h"
#include   "CGOpenBox.h"
#include   "GCOpenBoxManipulate.h"

using namespace Packets;

uint	CGUnExtraBagHandler::Execute(CGUnExtraBag* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}
uint	CGAskSubTrainSettingHandler::Execute(CGAskSubTrainSetting *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}
uint	CGAskSubTrainModeHandler::Execute(CGAskSubTrainMode *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	CGAskGetSubTrainExpHandler::Execute(CGAskGetSubTrainExp *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	CGAskBeginSubTrainHandler::Execute(CGAskBeginSubTrain *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	CGAskEndSubTrainHandler::Execute(CGAskEndSubTrain *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	CGSetTeamDistribModeHandler::Execute(CGSetTeamDistribMode *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	CGAskTeamDistribModeHandler::Execute(CGAskTeamDistribMode *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	CGModifyPKModeHandler::Execute(CGModifyPKMode *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	CGOpenBoxManipulateHandler::Execute(CGOpenBoxManipulate *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	CGRegisterTeamRecruitHandler::Execute(CGRegisterTeamRecruit *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	CGAskTeamRecruitInfoHandler::Execute(CGAskTeamRecruitInfo *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	CGIssueHorseFindMateInfoHandler::Execute(CGIssueHorseFindMateInfo *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	CGCancelHorseFindMateInfoHandler::Execute(CGCancelHorseFindMateInfo *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	CGSearchHorseFindMateInfoHandler::Execute(CGSearchHorseFindMateInfo *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	CGQueryHorseFindMateInfoHandler::Execute(CGQueryHorseFindMateInfo *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	CGAskOtherOnlineDetailAttribHandler::Execute(CGAskOtherOnlineDetailAttrib *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	CGSearchOnlineUserHandler::Execute(CGSearchOnlineUser *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	CGCountryCollectHandler::Execute(CGCountryCollect *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	CGCountryFetchHandler::Execute(CGCountryFetch *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	CGCountryDonativeHandler::Execute(CGCountryDonative *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	CGCountryDelateHandler::Execute(CGCountryDelate *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	CGCountryVoteOnHandler::Execute(CGCountryVoteOn *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	GCBankSizeHandler::Execute(GCBankSize *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	GCExpandBankSizeHandler::Execute(GCExpandBankSize *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	GCUseExtraBagResultHandler::Execute(GCUseExtraBagResult *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	GCUnExtraBagResultHandler::Execute(GCUnExtraBagResult *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	GCRetSubTrainSettingHandler::Execute(GCRetSubTrainSetting *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	GCEndSubTrainResultHandler::Execute(GCEndSubTrainResult *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	GCSubTrainModeResultHandler::Execute(GCSubTrainModeResult *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	GCSubTrainResultHandler::Execute(GCSubTrainResult *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
//这个函数添加完毕,在这屏蔽掉 2010.3.4
//uint	GCRetTeamDistribModeHandler::Execute(GCRetTeamDistribMode *pPacket,class Player * pPlayer)
//{
//	__ENTER_FUNCTION
//
//		return PACKET_EXE_CONTINUE;
//
//	__LEAVE_FUNCTION
//
//		return	PACKET_EXE_ERROR;
//}
uint	GCRetTeamRecruitInfoHandler::Execute(GCRetTeamRecruitInfo *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	GCReturnHorseFindMateInfoHandler::Execute(GCReturnHorseFindMateInfo *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	GCReturnOnlineUserHandler::Execute(GCReturnOnlineUser *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	GCCountryCollectHandler::Execute(GCCountryCollect *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	GCCountryFetchHandler::Execute(GCCountryFetch *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	GCCountryDelateBeginHandler::Execute(GCCountryDelateBegin *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	GCCountryVoteShowHandler::Execute(GCCountryVoteShow *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	GCOpenBoxHandler::Execute(GCOpenBox *pPacket,class Player * pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	CGOpenBoxHandler::Execute(CGOpenBox* pPacket,class Player* pPlayer ) 
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
uint	GCOpenBoxManipulateHandler::Execute(GCOpenBoxManipulate* pPacket,class Player* pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
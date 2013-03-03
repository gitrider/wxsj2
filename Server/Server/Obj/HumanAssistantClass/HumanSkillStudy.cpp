/********************************************************************
	created:	2008/01/09
	created:	9:1:2008   14:21
	filename: 	MapServer\Server\Obj\HumanAssistantClass\HumanSkillStudy.cpp
	file path:	MapServer\Server\Obj\HumanAssistantClass
	file base:	HumanSkillStudy
	file ext:	cpp
	author:		Richard
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "HumanSkillStudy.h"
#include "Obj_Human.h"
#include "GCDetailXinFaList.h"
#include "GCDetailSkillList.h"
#include "GamePlayer.h"
#include "GWChangeMenpai.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"

HumanSkillStudy::HumanSkillStudy()
{

}

HumanSkillStudy::~HumanSkillStudy()
{

}

VOID HumanSkillStudy::StudySkill( SKILL_LOG_PARAM* pSkillLog, XINFA_LOG_PARAM* pXinFaLog, INT Menpai, Obj_Human* pHuman )
{
	__ENTER_FUNCTION

	if( Menpai<0 || Menpai>=MATTRIBUTE_NUMBER )
	{
		AssertEx( FALSE, "门派取值范围错误！" ) ;
		return ;
	}
	
	HumanDB& oDB = *(pHuman->GetDB());

	const _XINFA_LIST& rXinFaList = pHuman->Skill_GetXinFaList();
	const _SKILL_LIST& rSkillList = pHuman->Skill_GetSkillList();
	StudyXinFa(oDB, Menpai, pXinFaLog, rXinFaList);

	AddSkill(oDB, pSkillLog, rSkillList, Menpai);

	FlushItemEffect(pHuman);

	UpdateMP(pHuman);
	UpdateHP(pHuman);
	//如果在设置此值，会造成PlayerTemplate.tab里面设置的初始元气值无效。
	//UpdatePneuma(pHuman);

	SendAttrChangeMessage(pHuman, rXinFaList, rSkillList, Menpai);

	__LEAVE_FUNCTION
}

VOID HumanSkillStudy::AddXinFaToDB( INT Menpai, Combat_Module::Skill_Module::XinFaData_T const* pXinFaData, 
								   HumanDB &oDB, 
								   INT nID, 
								   BYTE rCount, 
								   XINFA_LOG_PARAM* pXinFaLog 
								   )
{
	if(Menpai == pXinFaData->GetMenPai())
	{
		oDB.SetXinfaID(nID,rCount);
		oDB.SetXinfaLevel(1,rCount);
		oDB.SetXinFaCount(rCount+1);

		pXinFaLog->XinFaID  =	nID;
		pXinFaLog->Level	=	1;
		pXinFaLog->Count	=	rCount+1;
		SaveXinFaLog(pXinFaLog);
	}
}

VOID HumanSkillStudy::StudyXinFa( HumanDB &oDB, INT Menpai, XINFA_LOG_PARAM* pXinFaLog, const _XINFA_LIST &rXinFaList )
{
	oDB.SetMenPai(Menpai);

	oDB.SetXinFaCount(0);
	pXinFaLog->Count	= 0 ;
	SaveXinFaLog(pXinFaLog);
	BYTE rCount = rXinFaList.m_Count;
	INT nID = 0;
	INT nScaned = 0;
	while(nID<g_XinFaDataMgr.MAXSIZE && nScaned<g_XinFaDataMgr.Size())
	{
		Combat_Module::Skill_Module::XinFaData_T const* pXinFaData = g_XinFaDataMgr.GetInstanceByID(nID);
		if(NULL!=pXinFaData)
		{
			++nScaned;
			AddXinFaToDB(Menpai, pXinFaData, oDB, nID, rCount, pXinFaLog);
		}
		rCount = oDB.GetXinFaCount();
		if(rCount>=MAX_CHAR_XINFA_NUM)
		{
			break;
		}
		++nID;
	}
	InitXinFaOfDB(rCount, oDB);
}

VOID HumanSkillStudy::AddSkill( HumanDB &oDB, SKILL_LOG_PARAM* pSkillLog, const _SKILL_LIST &rSkillList, INT Menpai )
{
	pSkillLog->Count	= oDB.GetSkillCount();
	for( INT i=0; i<oDB.GetSkillCount(); ++i )
	{
		_OWN_SKILL& oSkill  = oDB.GetSkillID( i );
		oSkill.m_eState		= _OWN_SKILL::OWN_SKILL_CAN_USE;
		oSkill.m_nLevel		= 1;
	}
	SaveSkillLog(pSkillLog);
	BYTE rCount = rSkillList.m_Count;
	INT nID = 0;
	INT nScaned = 0;
	while(nID<g_SkillTemplateDataMgr.MAXSIZE && nScaned<g_SkillTemplateDataMgr.Size())
	{
		nScaned = AddSkillToDB(nID, nScaned, Menpai, oDB, rCount, pSkillLog);

		rCount = oDB.GetSkillCount();
		if(rCount>=MAX_CHAR_SKILL_NUM)
		{
			break;
		}
		++nID;
	}
	InitSkillIDOfDB(rCount, oDB);
}

VOID HumanSkillStudy::FlushItemEffect( Obj_Human* pHuman )
{
	pHuman->ItemEffectFlush();
}

VOID HumanSkillStudy::UpdateHP( Obj_Human* pHuman )
{
	pHuman->SetHP(pHuman->GetMaxHP());
}

VOID HumanSkillStudy::UpdateMP( Obj_Human* pHuman )
{
	pHuman->SetMP(pHuman->GetMaxMP());
}

VOID HumanSkillStudy::UpdatePneuma( Obj_Human* pHuman )
{
	pHuman->SetPneuma(pHuman->GetMaxPneuma());
}

VOID HumanSkillStudy::SendAttrChangeMessage( Obj_Human* pHuman, const _XINFA_LIST &rXinFaList, const _SKILL_LIST &rSkillList, INT Menpai )
{
	//SendRefeshAttribMsg(pHuman);

	SendXinFaMsg(pHuman, rXinFaList);
 
	SendSkillMsg(pHuman, rSkillList);

	SendMenPaiMsgToGLServer(pHuman, Menpai);
}

VOID HumanSkillStudy::SendPacket( Obj_Human* pHuman, Packet* pPacket )
{
	Player* pGamePlayer = pHuman->GetPlayer();
	if(NULL!=pGamePlayer)
	{
		pGamePlayer->SendPacket(pPacket); 
	}
}

//send detail information to player
VOID HumanSkillStudy::SendRefeshAttribMsg( Obj_Human* pHuman )
{
	pHuman->SendMsg_RefeshAttrib(); 
}

// send detail XinFa information to player
VOID HumanSkillStudy::SendXinFaMsg( Obj_Human* pHuman, const _XINFA_LIST &rXinFaList )
{
	GCDetailXinFaList	XinFaMsg;
	XinFaMsg.setObjID(pHuman->GetID());
	XinFaMsg.setXinFaList((WORD)rXinFaList.m_Count, rXinFaList.m_aXinFa);
	SendPacket(pHuman, &XinFaMsg);
}

// send detail Skill information to player
VOID HumanSkillStudy::SendSkillMsg( Obj_Human* pHuman, const _SKILL_LIST &rSkillList )
{
	GCDetailSkillList	SkillMsg;
	SkillMsg.setObjID(pHuman->GetID());
	SkillMsg.setSkillList((WORD)rSkillList.m_Count, rSkillList.m_aSkill);
	SendPacket(pHuman, &SkillMsg);
}

VOID HumanSkillStudy::SendMenPaiMsgToGLServer( Obj_Human* pHuman, INT Menpai )
{
	GWChangeMenpai* pMsg = (GWChangeMenpai*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHANGEMENPAI);
	pMsg->SetGUID( pHuman->GetGUID() );
	pMsg->SetMenpai( Menpai );

	g_pServerManager->SendPacket( pMsg, INVALID_ID );
}

VOID HumanSkillStudy::InitXinFaOfDB( BYTE rCount, HumanDB &oDB )
{
	for(INT nIdx=rCount;MAX_CHAR_XINFA_NUM>nIdx;++nIdx)
	{
		oDB.SetXinfaID(INVALID_ID,nIdx);
		oDB.SetXinfaLevel(0,nIdx);
	}
}

INT HumanSkillStudy::AddSkillToDB( INT nID, INT nScaned, INT Menpai, HumanDB &oDB, BYTE rCount, SKILL_LOG_PARAM* pSkillLog )
{
	Combat_Module::Skill_Module::SkillTemplateData_T const* pSkillTemplateData = g_SkillTemplateDataMgr.GetInstanceByID(nID);
	if(NULL!=pSkillTemplateData)
	{
		++nScaned;
		AddSkillWithUserClassCheck(pSkillTemplateData, Menpai, oDB, nID, rCount, pSkillLog);
	}	
	return nScaned;
}

VOID HumanSkillStudy::InitSkillIDOfDB( BYTE rCount, HumanDB &oDB )
{
	_OWN_SKILL oSkill;
	oSkill.m_eState		= _OWN_SKILL::OWN_SKILL_DISABLE;
	oSkill.m_nLevel		= 0;
	oSkill.m_nSkillID	= INVALID_ID;
	for(INT nIdx=rCount;MAX_CHAR_SKILL_NUM>nIdx;++nIdx)
	{
		oDB.SetSkillID(oSkill,nIdx);
	}
}

VOID HumanSkillStudy::AddSkillWithUserClassCheck
					( 
					Combat_Module::Skill_Module::SkillTemplateData_T const* pSkillTemplateData, 
					INT Menpai, 
					HumanDB &oDB, 
					INT nID, 
					BYTE rCount, 
					SKILL_LOG_PARAM* pSkillLog 
					)
{
	//如果使用者是玩家
	if(0==pSkillTemplateData->GetClassByUser())
	{
		AddSkillWithMenpaiCheck(Menpai, pSkillTemplateData, oDB, nID, rCount, pSkillLog);
	}
}

VOID HumanSkillStudy::AddSkillWithMenpaiCheck
					( 
					INT Menpai, 
					Combat_Module::Skill_Module::SkillTemplateData_T const* pSkillTemplateData,
					HumanDB &oDB,
					INT nID, 
					BYTE rCount, 
					SKILL_LOG_PARAM* pSkillLog
					)
{
	if(Menpai == pSkillTemplateData->GetMenPai())
	{
		_OWN_SKILL oSkill;
		oSkill.m_eState		= _OWN_SKILL::OWN_SKILL_DISABLE;
		oSkill.m_nLevel		= 0;
		oSkill.m_nSkillID	= nID;

		oDB.SetSkillID(oSkill,rCount);
		Time_t ActiveTime = 0>pSkillTemplateData->GetActiveTime()?-1:0;
		oDB.SetSkillCount(rCount+1);
		pSkillLog->SkillID = nID;
		pSkillLog->Count	= rCount+1;
		SaveSkillLog(pSkillLog);
	}
}
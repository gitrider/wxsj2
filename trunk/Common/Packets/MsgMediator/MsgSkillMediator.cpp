/********************************************************************
	created:	2008/01/24
	created:	24:1:2008   17:07
	filename: 	Common\Packets\MsgMediator\MsgSkillMediator.cpp
	file path:	Common\Packets\MsgMediator
	file base:	MsgSkillMediator
	file ext:	cpp
	author:		Richard
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "MsgSkillMediator.h"

VOID MsgSkillMediator::PostGCCharDirectImpact( const GCCharDirectImpact* pMsg )
{
	__ENTER_FUNCTION
		m_oGCCharDirectImpact = *pMsg;
	__LEAVE_FUNCTION
}

VOID MsgSkillMediator::PostGCDetailHealsAndDamages( const GCDetailHealsAndDamages* pMsg )
{
	__ENTER_FUNCTION
		m_oGCDetailHealsAndDamages = *pMsg;
	__LEAVE_FUNCTION

}

VOID MsgSkillMediator::PostGCCharSkill_Send( const GCCharSkill_Send* pMsg )
{
	__ENTER_FUNCTION
		m_oGCCharSkill_Send = *pMsg;
	__LEAVE_FUNCTION
}

VOID MsgSkillMediator::PostGCTargetListAndHitFlags( const GCTargetListAndHitFlags* pMsg )
{

}

VOID MsgSkillMediator::SendAllMsgToClient()
{

}

MsgSkillMediator::MsgSkillMediator()
{

}

MsgSkillMediator::~MsgSkillMediator()
{

}
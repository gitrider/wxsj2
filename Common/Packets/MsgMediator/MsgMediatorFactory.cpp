/********************************************************************
	created:	2008/01/24
	created:	24:1:2008   17:24
	filename: 	Packets\MsgMediator\MsgMediatorFactory.cpp
	file path:	Packets\MsgMediator
	file base:	MsgMediatorFactory
	file ext:	cpp
	author:		Richard
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "MsgMediatorFactory.h"

MsgMediatorFactory::MsgMediatorFactory()
{

}

MsgMediatorFactory::~MsgMediatorFactory()
{

}

MsgSkillMediator* MsgMediatorFactory::GetMsgSkillMediator()
{
	return &m_oMsgSkillMediator;
}
/********************************************************************
	created:	2008/01/23
	created:	23:1:2008   14:11
	filename: 	Common\Packets\MsgMediator\MsgMediator.h
	file path:	Common\Packets\MsgMediator
	file base:	MsgMediator
	file ext:	h
	author:		Richard
	
	purpose:	server 向 client 发送消息的中介者类的基类

	将技能系统与相关的消息对象解耦，这样，技能的逻辑流程不再直接导致特定的消息发送顺序

	可以方便的将消息合并或拆分或颠倒顺序，以适应消息改动的需求。此方案实现为Mediator模式

	在基类中只对中介者的接口进行抽象. 每个具体中介者类应重载自己关心的消息消息接受函数提
	
	供，具体的实现
*********************************************************************/

#ifndef MsgMediator_h__
#define MsgMediator_h__

#include "Type.h"
#include "GCCharDirectImpact.h"
#include "GCDetailHealsAndDamages.h"
#include "GCCharSkill_Send.h"
#include "GCTargetListAndHitFlags.h"

class MsgMediator
{
public:
	enum MMT_TYPE
	{
		MMT_MsgMediator = 0,
		MMT_MsgSkillMediator
	};
//接口
public:
	//************************************
	// Method:    GetMediatorType
	// FullName:  MsgMediator::GetMediatorType
	// Access:    virtual public 
	// Returns:   MsgMediator::MMT_TYPE
	// Qualifier:
	//************************************
	virtual MMT_TYPE GetMediatorType(){ return MMT_MsgMediator; }
	//************************************
	// Method:    PostGCCharDirectImpact
	// FullName:  MsgMediator::PostGCCharDirectImpact
	// Access:    virtual public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: const GCCharDirectImpact * pMsg
	//************************************
	virtual VOID PostGCCharDirectImpact( const GCCharDirectImpact* pMsg ){};

	//************************************
	// Method:    PostGCDetailHealsAndDamages
	// FullName:  MsgMediator::PostGCDetailHealsAndDamages
	// Access:    virtual public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: const GCDetailHealsAndDamages * pMsg
	//************************************
	virtual VOID PostGCDetailHealsAndDamages( const GCDetailHealsAndDamages* pMsg ){};

	//************************************
	// Method:    PostGCCharSkill_Send
	// FullName:  MsgMediator::PostGCCharSkill_Send
	// Access:    virtual public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: const GCCharSkill_Send * pMsg
	//************************************
	virtual VOID PostGCCharSkill_Send( const GCCharSkill_Send* pMsg ){};

	//************************************
	// Method:    PostGCTargetListAndHitFlags
	// FullName:  MsgMediator::PostGCTargetListAndHitFlags
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: const GCTargetListAndHitFlags * pMsg
	//************************************
	virtual VOID PostGCTargetListAndHitFlags( const GCTargetListAndHitFlags* pMsg ){};

	//************************************
	// Method:    SendAllMsgToClient
	// FullName:  MsgMediator::SendAllMsgToClient
	// Access:    virtual public 
	// Returns:   VOID
	// Qualifier:
	//************************************
	virtual VOID SendAllMsgToClient() = 0;

};


#endif // MsgMediator_h__
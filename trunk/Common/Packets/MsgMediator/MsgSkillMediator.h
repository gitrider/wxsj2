/********************************************************************
	created:	2008/01/24
	created:	24:1:2008   17:04
	filename: 	Common\Packets\MsgMediator\MsgSkillMediator.h
	file path:	Common\Packets\MsgMediator
	file base:	MsgSkillMediator
	file ext:	h
	author:		Richard
	
	purpose:	处理技能消息的具体中介者类
*********************************************************************/

#ifndef MsgSkillMediator_h__
#define MsgSkillMediator_h__

#include "MsgMediator.h"

class MsgSkillMediator : public MsgMediator
{
public:
	MsgSkillMediator();
	~MsgSkillMediator();

public:
	//************************************
	// Method:    GetMediatorType
	// FullName:  MsgSkillMediator::GetMediatorType
	// Access:    virtual public 
	// Returns:   MMT_TYPE
	// Qualifier:
	//************************************
	virtual MMT_TYPE GetMediatorType(){ return MMT_MsgSkillMediator; }

	//************************************
	// Method:    PostGCCharDirectImpact
	// FullName:  MsgMediator::PostGCCharDirectImpact
	// Access:    virtual public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: const GCCharDirectImpact * pMsg
	//************************************
	virtual VOID PostGCCharDirectImpact( const GCCharDirectImpact* pMsg );

	//************************************
	// Method:    PostGCDetailHealsAndDamages
	// FullName:  MsgMediator::PostGCDetailHealsAndDamages
	// Access:    virtual public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: const GCDetailHealsAndDamages * pMsg
	//************************************
	virtual VOID PostGCDetailHealsAndDamages( const GCDetailHealsAndDamages* pMsg );

	//************************************
	// Method:    PostGCCharSkill_Send
	// FullName:  MsgMediator::PostGCCharSkill_Send
	// Access:    virtual public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: const GCCharSkill_Send * pMsg
	//************************************
	virtual VOID PostGCCharSkill_Send( const GCCharSkill_Send* pMsg );

	//************************************
	// Method:    PostGCTargetListAndHitFlags
	// FullName:  MsgMediator::PostGCTargetListAndHitFlags
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: const GCTargetListAndHitFlags * pMsg
	//************************************
	virtual VOID PostGCTargetListAndHitFlags( const GCTargetListAndHitFlags* pMsg );

	//************************************
	// Method:    SendAllMsgToClient
	// FullName:  MsgMediator::SendAllMsgToClient
	// Access:    virtual public 
	// Returns:   VOID
	// Qualifier:
	//************************************
	virtual VOID SendAllMsgToClient();

protected:
	GCCharDirectImpact			m_oGCCharDirectImpact;
	GCDetailHealsAndDamages		m_oGCDetailHealsAndDamages;
	GCCharSkill_Send			m_oGCCharSkill_Send;
	GCTargetListAndHitFlags		m_oGCTargetListAndHitFlags;

};

#endif // MsgSkillMediator_h__
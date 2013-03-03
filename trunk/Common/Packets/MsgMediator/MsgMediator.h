/********************************************************************
	created:	2008/01/23
	created:	23:1:2008   14:11
	filename: 	Common\Packets\MsgMediator\MsgMediator.h
	file path:	Common\Packets\MsgMediator
	file base:	MsgMediator
	file ext:	h
	author:		Richard
	
	purpose:	server �� client ������Ϣ���н�����Ļ���

	������ϵͳ����ص���Ϣ���������������ܵ��߼����̲���ֱ�ӵ����ض�����Ϣ����˳��

	���Է���Ľ���Ϣ�ϲ����ֻ�ߵ�˳������Ӧ��Ϣ�Ķ������󡣴˷���ʵ��ΪMediatorģʽ

	�ڻ�����ֻ���н��ߵĽӿڽ��г���. ÿ�������н�����Ӧ�����Լ����ĵ���Ϣ��Ϣ���ܺ�����
	
	���������ʵ��
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
//�ӿ�
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
#ifndef __FI_AIINTERFACE_H__
#define __FI_AIINTERFACE_H__

#include "fi_share/fi_messagestruct.h"


#include "fi_gamebasic/lgb_objopbasestruct.h"

#pragma once

struct FI_RPG_Event;

struct LOM_BaseActObject;

struct LOM_IRPGEventProcess;

struct LOM_IAIProcessInterface
{
	virtual bool		ProcessInputEvent( FI_RPG_Event *rpgE )									=	0;

	virtual bool		Update( int nTimePass)																=	0;

	virtual bool		RegisterOperatorData( FI_ObjOP_Data *pOPData)							=	0;

	virtual bool		RemoveOperatorData( FI_GID_T idOpData )									=	0;

	virtual LOM_IRPGEventProcess*		GetSupportRPGListen()									=	0;

	virtual void		SetOwnerObject( LOM_BaseActObject *pObj)								=	0;

};

struct LOM_IAIProcessCreatorInterface
{
	virtual LOM_IAIProcessInterface*		CreateAIProcess()											=	0;
	virtual void							DestroyAIProcess(LOM_IAIProcessInterface* p)				=	0;
};


struct LOM_IAIManagerInterface
{
	virtual LOM_IAIProcessInterface		*		CreateAIProcess()										=	0;
	virtual	void								DestroyAIProcess(LOM_IAIProcessInterface* p)			=	0;

	virtual void				RegisterAIProcessCreator(LOM_IAIProcessCreatorInterface *p)				=	0;
	virtual void				RemoveAIProcessCreator(LOM_IAIProcessCreatorInterface *p)				=	0;
};

struct LOM_IConditonMeet
{
	virtual bool	IsEventConditionMeet( FI_OBJOP_Condition *pCondition, FI_RPG_Event &rpg )				=	0;
	virtual bool	IsObjectStatusConditionMeet( FI_OBJOP_Condition *pContion, LOM_BaseActObject *pObj )		=	0;
	virtual bool	IsObjectContionMeet( FI_OBJOP_Condition *pContion, LOM_BaseActObject *pObj )				=	0;
};

struct LOM_IOperatorExecute
{
	virtual bool	ExecOperator( FI_OBJOP_Operator *p, LOM_BaseActObject *pObj )	=	0;
	virtual bool	ExecOperator( FI_OBJOP_Operator *p, LOM_BaseActObject *pObj, FI_RPG_Event *pRPGE )		=	0;
	virtual bool	IsOperatorValid( FI_OBJOP_Operator *p, int nOPStartTime )							=	0;
};

struct LOM_IOperatorOPInterface
{
	virtual bool DoAction( int nTimePass )				=	0;
	virtual bool OnRPGEvent( FI_RPG_Event &event )		=	0;
	virtual bool needRegisterEvent()					=	0;
	virtual bool needRegisterUpdate()					=	0;
	virtual bool InitOperator()							=	0;
	virtual bool IsOperatorOver()						=	0;
	virtual	bool ExecOperator()							=	0;
	virtual FI_GID_T	GetOperatorID()					=	0;
	virtual void	SetOwnerObject( LOM_BaseActObject *) = 0;
	virtual LOM_IRPGEventProcess*	GetEventListen()	=	0;
};



struct LOM_IObjectOPCreator
{
	virtual LOM_IOperatorOPInterface*	CreateObjectOperator( FI_ObjOP_Data *p )	=	0;	
};

struct LOM_IObjectOPManagerInterface
{
	virtual void	AddCreatorInterface( LOM_IObjectOPCreator *p )	 = 0;
	virtual void	RemoveCreatorInterface(LOM_IObjectOPCreator *p ) = 0;

	virtual LOM_IOperatorOPInterface*	CreateObjectOperator( FI_ObjOP_Data *p ) = 0;
};
#endif


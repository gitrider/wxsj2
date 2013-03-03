/********************************************************************
	created:	2008/01/24
	created:	24:1:2008   17:19
	filename: 	Common\Packets\MsgMediator\MsgMediatorFactory.h
	file path:	Common\Packets\MsgMediator
	file base:	MsgMediatorFactory
	file ext:	h
	author:		Richard
	
	purpose:	通过接口获得特定的MsgMediator对象实例
*********************************************************************/
#ifndef MsgMediatorFactory_h__
#define MsgMediatorFactory_h__

#include "MsgSkillMediator.h"

class MsgMediatorFactory
{
public :
	MsgMediatorFactory();
	~MsgMediatorFactory();
public:
	//************************************
	// Method:    GetMsgSkillMediator
	// FullName:  MsgMediatorFactory::GetMsgSkillMediator
	// Access:    public 
	// Returns:   MsgSkillMediator*
	// Qualifier:
	// 获取技能消息中介对象
	//************************************
	MsgSkillMediator* GetMsgSkillMediator();

private:
	MsgSkillMediator m_oMsgSkillMediator;
};

#endif // MsgMediatorFactory_h__
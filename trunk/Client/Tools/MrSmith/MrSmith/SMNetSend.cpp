#include "StdAfx.h"
#include "SMAgentManager.h"
#include "SMAgent.h"
#include "LuaPlus.h"
#include "SMNetSend.h"
#include "SMNetManager.h"
#include "SMVariable.h"
#include "SMUtil.h"
#include "SMLog.h"
#include "SMInfo.h"
#include "DataPool\SMDataPool.h"
#include "DataPool\SMDataPool_CharacterList.h"

#include "CLConnect.h"
#include "CLAskLogin.h"
#include "CGConnect.h"
#include "CGEnterScene.h"
#include "CGEnvRequest.h"
#include "CLAskCharList.h"
#include "CLAskCreateChar.h"
#include "CLAskDeleteChar.h"
#include "CLAskCharLogin.h"
#include "CGAskChangeScene.h"
#include "CGCharAskBaseAttrib.h"
#include "CGAskDetailAttrib.h"
#include "CGCharAskEquipment.h"
#include "CGAskDetailAbilityInfo.h"
#include "CGAskDetailXinFaList.h"
#include "CGAskDetailSkillList.h"
#include "CGAskTeamInfo.h"
#include "CGCityAskAttr.h"
#include "CGMinorPasswd.h"
#include "CGAskDetailEquipList.h"
#include "CGAskMyBagList.h"
#include "CGAskMissionList.h"
#include "CGCharAllTitles.h"
#include "CGAskSetting.h"
#include "CGRelation.h"
#include "CGAskMail.h"
#include "CGBankMoney.h"
#include "CGHeartBeat.h"
#include "CGStallApply.h"
#include "CGStallEstablish.h"
#include "CGCommand.h"
#include "KeyExchange.h"
#include "SMGameDefine.h"

using namespace Packets;

namespace SMITH
{

INT		GetTLBBVersion()
{
	return g_cSMDefine.GetVersion();
}

USHORT MsgSend_HeartBeat(Agent* pAgent)
{
	CGHeartBeat msg;
	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
}

//CLConnect
USHORT MsgSend_CLConnect(Agent* pAgent, const LuaPlus::LuaStack& args)
{
	CLConnect msg;
	msg.SetUseMiBao( 0 );
	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
}

//CLAskLogin
USHORT MsgSend_CLAskLogin(Agent* pAgent, LuaPlus::LuaStack& args)
{
	CLAskLogin msg;

	const char* szAccount = pAgent->getVariable()->getAs_String("AccountName").c_str(); 
	msg.SetAccount((char*)(szAccount));
	msg.SetPassWord((char *)Util::md5String(pAgent->getVariable()->getAs_String("AccountPassword").c_str()).c_str());
	msg.SetVersion(GetTLBBVersion());
	pAgent->getNetManager()->sendPacket(&msg);

	pAgent->getVariable()->setVariable("User_NAME", szAccount);

	return msg.GetPacketID();
}

//CGConnect
USHORT MsgSend_CGConnect(Agent* pAgent, const LuaStack& args)
{
	Variable* pVariable = pAgent->getVariable();
	//KeyExchange msgKeyExchange;
	//msgKeyExchange.SetKey(pAgent->getNetManager()->GetInterKey(), NetManager::KEY_SIZE_IN_BYTE);
	//pAgent->getNetManager()->sendPacket(&msgKeyExchange);

	CGConnect msgConnect;
	msgConnect.SetKey((UINT)pVariable->getAs_Int("GameServer_Key"));
	msgConnect.SetServerID(INVALID_ID);
	msgConnect.SetGUID((GUID_t)pVariable->getAs_Int("User_GUID"));
	msgConnect.SetAccount((char*)pVariable->getAs_String("User_NAME").c_str());
	msgConnect.SetGender(pVariable->getAs_Int("User_GENDER"));
	msgConnect.SetCheckVer(GetTLBBVersion());
	msgConnect.SetCountry(1);
	msgConnect.SetProfession(13);
	pAgent->getNetManager()->sendPacket(&msgConnect);

	return msgConnect.GetPacketID();
}

//CGEnterScene
USHORT MsgSend_CGEnterScene(Agent* pAgent, const LuaStack& args)
{
	Variable* pVariable = pAgent->getVariable();

	CGEnterScene msg;
	msg.setEnterType((BYTE)pVariable->getAs_Int("Scene_Entertype"));
	msg.setSceneID(pVariable->getAs_Int("Scene_ID"));

	std::pair< float, float > fvEnterPos = pVariable->getAs_Vector2("Scene_EnterPos");
	msg.setEnterPos(WORLD_POS(fvEnterPos.first, fvEnterPos.second));

	pAgent->getNetManager()->sendPacket(&msg);
	pAgent->getLog()->addLogFmt( "CGEnterScene(Scene_Entertype:%d,Scene_ID:%d,x:%f,z:%f)",
		pVariable->getAs_Int("Scene_Entertype"),
		pVariable->getAs_Int("Scene_ID"),
		fvEnterPos.first,
		fvEnterPos.second );
	return msg.GetPacketID();
}

//CGAskChangeScene
USHORT MsgSend_CGAskChangeScene(Agent* pAgent, const LuaStack& args)
{
	Variable* pVariable = pAgent->getVariable();

	CGAskChangeScene msg;
	msg.SetSourSceneID(pVariable->getAs_Int("Scene_ID"));
	msg.SetDestSceneID(pVariable->getAs_Int("DestScene_ID"));

	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
}

//CGEnvRequest
USHORT MsgSend_CGEnvRequest(Agent* pAgent, const LuaStack& args)
{
	Variable* pVariable = pAgent->getVariable();

	CGEnvRequest msg;
	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
}

//CLAskCharList
USHORT MsgSend_CLAskCharList(Agent* pAgent, const LuaPlus::LuaStack& args)
{
	Variable* pVariable = pAgent->getVariable();

	CLAskCharList msg;
	msg.SetAccount((char*)pVariable->getAs_String("User_NAME").c_str());
	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
}

//CLAskCreateChar
USHORT MsgSend_CLAskCreateChar(Agent* pAgent, const LuaPlus::LuaStack& args)
{
	Variable* pVariable = pAgent->getVariable();

	CLAskCreateChar msg;

	msg.SetName( SMITH::Util::generateName().c_str() );
	msg.SetFaceColor(0);
	msg.SetFaceModel(120); //CharHeadGeo.txt
	msg.SetHairColor(0);
	msg.SetHairModel(0);//CharHairGeo.txt
	msg.SetSex(0);//CharRace.txt
	msg.SetHeadID(1);
	msg.SetCharGuid(0);
	msg.SetProfession(13);
	msg.SetCountry(1);

	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
}

USHORT MsgSend_CLAskDeleteChar(Agent* pAgent, const LuaPlus::LuaStack& args)
{
	LoginCharacterList* pCharList = pAgent->getDataPool()->m_pLoginCharacterList;
	CLAskDeleteChar msg;

	DWORD dwCount = pCharList->getCharCounts();
	if( dwCount > 0 )
	{
		const DB_CHAR_BASE_INFO* p =  pCharList->getCharacter(dwCount-1);
		if( p ) 
		{
			msg.SetCharGuid(p->m_GUID);
			pAgent->getNetManager()->sendPacket(&msg);
		}
	}
	return msg.GetPacketID();
}

//CLAskCharLogin
USHORT MsgSend_CLAskCharLogin(Agent* pAgent)
{
	LoginCharacterList* pCharList = pAgent->getDataPool()->m_pLoginCharacterList;

	CLAskCharLogin msg;
	DWORD dwCount = pCharList->getCharCounts();

	if( dwCount > 0 ) 
	{
		const DB_CHAR_BASE_INFO* p = pCharList->getCharacter(0);
		if( p ) 
		{
			msg.SetCharGuid(p->m_GUID);
			pAgent->getNetManager()->sendPacket(&msg);

			pAgent->getVariable()->setAs_Int("User_GUID", p->m_GUID);
		}
	}
	return msg.GetPacketID();
}

//CGCharAskBaseAttrib
USHORT MsgSend_CGCharAskBaseAttrib(Agent* pAgent, LuaPlus::LuaStack& args)
{
	int nID = pAgent->getVariable()->getAs_Int("MySelf_ID");
	if( args[3].IsInteger() )
	{
		nID = args[3].GetInteger();
	}
	CGCharAskBaseAttrib msg;
	msg.setTargetID(nID);
	pAgent->getNetManager()->sendPacket(&msg);
	//pAgent->getVariable()->setAs_Int( "CharAskBaseAttrib_Tick", GetTickCount() );
	if( nID == pAgent->getVariable()->getAs_Int("MySelf_ID") )
	{
		pAgent->getLog()->addLog( "·¢ËÍCGCharAskBaseAttrib ok¡£" );
	}
	return msg.GetPacketID();
}

//CGAskDetailAttrib
USHORT MsgSend_CGAskDetailAttrib(Agent* pAgent, const LuaPlus::LuaStack& args)
{
	CGAskDetailAttrib msg;
	msg.setTargetID(pAgent->getVariable()->getAs_Int("MySelf_ID"));
	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
}

//CGAskEquipment
USHORT MsgSend_CGAskEquipment(Agent* pAgent, const LuaPlus::LuaStack& args)
{
	CGCharAskEquipment msg;
	msg.setObjID(pAgent->getVariable()->getAs_Int("MySelf_ID"));
	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
	return 0;
}

//CGAskDetailAbilityInfo
USHORT MsgSend_CGAskDetailAbilityInfo(Agent* pAgent, const LuaPlus::LuaStack& args)
{
	CGAskDetailAbilityInfo msg;
	msg.setTargetID(pAgent->getVariable()->getAs_Int("MySelf_ID"));
	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
	return 0;
}

//CGAskDetailXinFaList
USHORT MsgSend_CGAskDetailXinFaList(Agent* pAgent, const LuaPlus::LuaStack& args)
{
	CGAskDetailXinFaList msg;
	msg.setTargetID(pAgent->getVariable()->getAs_Int("MySelf_ID"));
	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
	return 0;
}

//CGAskDetailSkillList
USHORT MsgSend_CGAskDetailSkillList(Agent* pAgent, const LuaPlus::LuaStack& args)
{
	CGAskDetailSkillList msg;
	msg.setTargetID(pAgent->getVariable()->getAs_Int("MySelf_ID"));
	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
	return 0;
}

//CGAskTeamInfo
USHORT MsgSend_CGAskTeamInfo(Agent* pAgent, const LuaPlus::LuaStack& args)
{
	CGAskTeamInfo msg;
	msg.SetObjID(pAgent->getVariable()->getAs_Int("MySelf_ID"));
	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
	return 0;
}

//CGAskCityAttr
USHORT MsgSend_CGAskCityAttr(Agent* pAgent, const LuaPlus::LuaStack& args)
{
	CGCityAskAttr msg;
	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
	return 0;
}

//CGAskMinorPasswd
USHORT MsgSend_CGAskMinorPasswd(Agent* pAgent, const LuaPlus::LuaStack& args)
{
	CGMinorPasswd msg;
	msg.GetMinorPassword()->CleanUp();
	msg.GetMinorPassword()->m_Type = MREQT_PASSWDSETUP;
	pAgent->getNetManager()->sendPacket(&msg);

	msg.GetMinorPassword()->m_Type = MREQT_DELETEPASSWDTIME;
	pAgent->getNetManager()->sendPacket(&msg);

	return msg.GetPacketID();
	return 0;
}

//CGAskDetailEquipList
USHORT MsgSend_CGAskDetailEquipList(Agent* pAgent, const LuaPlus::LuaStack& args)
{
	CGAskDetailEquipList msg;
	msg.setObjID(pAgent->getVariable()->getAs_Int("MySelf_ID"));
	msg.SetAskMode(ASK_EQUIP_ALL);
	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
	return 0;
}

//CGAskMyBagList
USHORT MsgSend_CGAskMyBagList(Agent* pAgent, const LuaPlus::LuaStack& args)
{
	CGAskMyBagList	msg;
	msg.SetAskMode(ASK_ALL);
	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
	return 0;
}

//CGAskMissionList
USHORT MsgSend_CGAskMissionList(Agent* pAgent, const LuaPlus::LuaStack& args)
{
	CGAskMissionList	msg;
	msg.setTargetID( pAgent->getVariable()->getAs_Int("MySelf_ID") );
	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
	return 0;
}

//CGAskCharAllTitles
USHORT MsgSend_CGAskCharAllTitles(Agent* pAgent, const LuaPlus::LuaStack& args)
{
	CGCharAllTitles	msg;
	msg.setTargetID( pAgent->getVariable()->getAs_Int("MySelf_ID") );
	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
	return 0;
}

//CGAskSetting
USHORT MsgSend_CGAskSetting(Agent* pAgent, const LuaPlus::LuaStack& args)
{
	CGAskSetting msg;
	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
	return 0;
}

//CGAskRelation
USHORT MsgSend_CGAskRelation(Agent* pAgent, const LuaPlus::LuaStack& args)
{
	CGRelation msg;
	msg.GetRelation()->m_Type = REQ_RELATIONLIST;
	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
	return 0;
}

//CGAskMail
USHORT MsgSend_CGAskMail(Agent* pAgent, const LuaPlus::LuaStack& args)
{
	CGAskMail msg;
	msg.SetAskType( ASK_TYPE_LOGIN );
	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
	return 0;
}

//CGAskBankMoney
USHORT MsgSend_CGAskBankMoney(Agent* pAgent, const LuaPlus::LuaStack& args)
{
	CGBankMoney	msg;
	msg.SetIsSave(CGBankMoney::UPDATE_MONEY);
	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
	return 0;
}

//CGStallApply
USHORT MsgSend_CGStallApply(Agent* pAgent, const LuaPlus::LuaStack& args)
{
	CGStallApply msg;
	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
}

//CGStallEstablish
USHORT MsgSend_CGStallEstablish(Agent* pAgent, const LuaPlus::LuaStack& args)
{
	CGStallEstablish msg;
	pAgent->getNetManager()->sendPacket(&msg);
	return msg.GetPacketID();
}

//CGCommand
USHORT MsgSend_CGCommand(Agent* pAgent, LuaPlus::LuaStack& args)
{
	CGCommand	msg;
	if( args[3].IsString() )
	{
        msg.SetCommand( (CHAR*)( args[3].GetString() ) );
		msg.SetCommandSize( (BYTE)strlen( args[3].GetString() ) );
		pAgent->getNetManager()->sendPacket( &msg );
	}
	return msg.GetPacketID();
}

}

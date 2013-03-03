/****************************************\
*										*
*		   Process Msg Send				*
*										*
*						 by jinchao		*
\****************************************/

#pragma once

namespace SMITH
{

INT		GetTLBBVersion();

USHORT MsgSend_HeartBeat(Agent* pAgent);

//登陆连接
//CLConnect
USHORT MsgSend_CLConnect(Agent* pAgent, const LuaPlus::LuaStack& args);
//CLAskLogin
USHORT MsgSend_CLAskLogin(Agent* pAgent, LuaPlus::LuaStack& args);

//选择、创建、删除人物
//CLAskCharList
USHORT MsgSend_CLAskCharList(Agent* pAgent, const LuaPlus::LuaStack& args);
//CLAskCreateChar
USHORT MsgSend_CLAskCreateChar(Agent* pAgent, const LuaPlus::LuaStack& args);
//CLAskDeleteChar
USHORT MsgSend_CLAskDeleteChar(Agent* pAgent, const LuaPlus::LuaStack& args);
//CLAskCharLogin
USHORT MsgSend_CLAskCharLogin(Agent* pAgent);

//CGConnect
USHORT MsgSend_CGConnect(Agent* pAgent, const LuaPlus::LuaStack& args);
//CGEnterScene
USHORT MsgSend_CGEnterScene(Agent* pAgent, const LuaPlus::LuaStack& args);
//CGEnvRequest
USHORT MsgSend_CGEnvRequest(Agent* pAgent, const LuaPlus::LuaStack& args);
//CGAskChangeScene
USHORT MsgSend_CGAskChangeScene(Agent* pAgent, const LuaPlus::LuaStack& args);

//请求人物数据
//CGCharAskBaseAttrib
USHORT MsgSend_CGCharAskBaseAttrib(Agent* pAgent, LuaPlus::LuaStack& args);
//CGAskDetailAttrib
USHORT MsgSend_CGAskDetailAttrib(Agent* pAgent, const LuaPlus::LuaStack& args);
//CGAskEquipment
USHORT MsgSend_CGAskEquipment(Agent* pAgent, const LuaPlus::LuaStack& args);
//CGAskDetailAbilityInfo
USHORT MsgSend_CGAskDetailAbilityInfo(Agent* pAgent, const LuaPlus::LuaStack& args);
//CGAskDetailXinFaList
USHORT MsgSend_CGAskDetailXinFaList(Agent* pAgent, const LuaPlus::LuaStack& args);
//CGAskDetailSkillList
USHORT MsgSend_CGAskDetailSkillList(Agent* pAgent, const LuaPlus::LuaStack& args);
//CGAskTeamInfo
USHORT MsgSend_CGAskTeamInfo(Agent* pAgent, const LuaPlus::LuaStack& args);
//CGAskCityAttr
USHORT MsgSend_CGAskCityAttr(Agent* pAgent, const LuaPlus::LuaStack& args);
//CGAskMinorPasswd
USHORT MsgSend_CGAskMinorPasswd(Agent* pAgent, const LuaPlus::LuaStack& args);
//CGAskDetailEquipList
USHORT MsgSend_CGAskDetailEquipList(Agent* pAgent, const LuaPlus::LuaStack& args);
//CGAskMyBagList
USHORT MsgSend_CGAskMyBagList(Agent* pAgent, const LuaPlus::LuaStack& args);
//CGAskMissionList
USHORT MsgSend_CGAskMissionList(Agent* pAgent, const LuaPlus::LuaStack& args);
//CGAskCharAllTitles
USHORT MsgSend_CGAskCharAllTitles(Agent* pAgent, const LuaPlus::LuaStack& args);
//CGAskSetting
USHORT MsgSend_CGAskSetting(Agent* pAgent, const LuaPlus::LuaStack& args);
//CGAskRelation
USHORT MsgSend_CGAskRelation(Agent* pAgent, const LuaPlus::LuaStack& args);
//CGAskMail
USHORT MsgSend_CGAskMail(Agent* pAgent, const LuaPlus::LuaStack& args);
//CGAskBankMoney
USHORT MsgSend_CGAskBankMoney(Agent* pAgent, const LuaPlus::LuaStack& args);
//CGStallApply
USHORT MsgSend_CGStallApply(Agent* pAgent, const LuaPlus::LuaStack& args);
//CGStallEstablish
USHORT MsgSend_CGStallEstablish(Agent* pAgent, const LuaPlus::LuaStack& args);
//CGCommand
USHORT MsgSend_CGCommand(Agent* pAgent, LuaPlus::LuaStack& args);

}
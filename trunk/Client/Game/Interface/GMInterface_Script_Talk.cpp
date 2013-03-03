
#include "StdAfx.h"
#include <map>
#include <deque>
#include "Network\NetManager.h"
#include "GMGameInterface.h"
#include "GMInterface_Script_Talk.h"
#include "GIException.h"
#include "GITimeSystem.h"
#include "GIEventDefine.h"
#include "GIUISystem.h"
#include "GIUtil.h"
#include "LuaPlus.h"
#include "Global.h"
#include "Object/Manager/ObjectManager.h"
#include "Object/Logic/Object.h"
#include "Object/Logic/Character/Obj_Character.h"
#include "object/Logic/character/obj_playermyself.h"
#include "DataPool/GMDataPool.h"
#include "DataPool\GMDP_CharacterData.h"
#include "Procedure\GameProcedure.h"
#include "Event/GMEventSystem.h"
#include "Input\GMInputSystem.h"
#include "DataPool/GMDP_Struct_Relation.h"
#include "DBC/GMDataBase.h"
#include "Action/GMActionSystem.h"
#include "World/WorldManager.h"
#include "TransferItem/GMTransferItemSystem.h"

#include <OgreStringConverter.h>

//
//脚本函数沙箱，这些函数应该由脚本调用
//
namespace SCRIPT_SANDBOX
{
	////当前所在处的频道
	//const char*		NAMETYPE_TALK_NEAR		= "near";		//附近
	//const char*		NAMETYPE_TALK_PRIVATE	= "private";	//私聊
	//const char*		NAMETYPE_TALK_FAMILY	= "family";		//家族
	//const char*		NAMETYPE_TALK_TEAM		= "team";		//队伍
	//const char*		NAMETYPE_TALK_GUILD		= "guild";		//帮会
	//const char*		NAMETYPE_TALK_NATION	= "nation";		//国家
	//const char*		NAMETYPE_TALK_SYSTEM	= "system";		//系统
	//const char*		NAMETYPE_TALK_WORLD		= "world";		//世界

	////字符串头标识检测
	//const INT		MESSAGE_HEAD_LENGTH		= 3;	//需检测消息头长度
	//const char		MESSAGETYPE_TALK_NEAR[MESSAGE_HEAD_LENGTH+1]		= "/F ";	//附近
	//const char		MESSAGETYPE_TALK_PRIVATE[MESSAGE_HEAD_LENGTH+1]		= "/S ";	//私聊
	//const char		MESSAGETYPE_TALK_FAMILY[MESSAGE_HEAD_LENGTH+1]		= "/J ";	//家族
	//const char		MESSAGETYPE_TALK_TEAM[MESSAGE_HEAD_LENGTH+1]		= "/D ";	//队伍
	//const char		MESSAGETYPE_TALK_GUILD[MESSAGE_HEAD_LENGTH+1]		= "/B ";	//帮会
	//const char		MESSAGETYPE_TALK_NATION[MESSAGE_HEAD_LENGTH+1]		= "/G ";	//国家
	//const char		MESSAGETYPE_TALK_SYSTEM[MESSAGE_HEAD_LENGTH+1]		= "/X ";	//系统
	//const char		MESSAGETYPE_TALK_WORLD[MESSAGE_HEAD_LENGTH+1]		= "/W ";	//世界
	//当前所在处的频道
	const char*		NAMETYPE_TALK_NEAR		= "near";
	const char*		NAMETYPE_TALK_SCENE		= "scene";
	const char*		NAMETYPE_TALK_SYSTEM	= "system";
	const char*		NAMETYPE_TALK_TEAM		= "team";
	const char*		NAMETYPE_TALK_GUILD		= "guild";
	const char*		NAMETYPE_TALK_USER		= "user";
	const char*		NAMETYPE_TALK_PRIVATE	= "private";
	const char*		NAMETYPE_TALK_SELF		= "self";
	const char*		NAMETYPE_TALK_MENPAI	= "menpai";
	const char*		NAMETYPE_TALK_WORLD = "world";
	const char*		NAMETYPE_TALK_COUNTRY = "country";

	//字符串头标识检测
	const INT		MESSAGE_HEAD_LENGTH		= 3;	//需检测消息头长度
	const char		MESSAGETYPE_TALK_NEAR_L[MESSAGE_HEAD_LENGTH+1]		= "/F ";	//附近聊天
	const char		MESSAGETYPE_TALK_NEAR_U[MESSAGE_HEAD_LENGTH+1]		= "/f ";	//附近聊天
	const char		MESSAGETYPE_TALK_WORD_L[MESSAGE_HEAD_LENGTH+1]		= "/W ";	//世界聊天
	const char		MESSAGETYPE_TALK_WORD_U[MESSAGE_HEAD_LENGTH+1]		= "/w ";	//世界聊天
	const char		MESSAGETYPE_TALK_TEAM_L[MESSAGE_HEAD_LENGTH+1]		= "/D ";	//组队聊天
	const char		MESSAGETYPE_TALK_TEAM_U[MESSAGE_HEAD_LENGTH+1]		= "/d ";	//组队聊天
	const char		MESSAGETYPE_TALK_GUILD_L[MESSAGE_HEAD_LENGTH+1]		= "/B ";	//帮派聊天
	const char		MESSAGETYPE_TALK_GUILD_U[MESSAGE_HEAD_LENGTH+1]		= "/b ";	//帮派聊天
	const char		MESSAGETYPE_TALK_MENPAI_L[MESSAGE_HEAD_LENGTH+1]	= "/M ";	//门派聊天
	const char		MESSAGETYPE_TALK_MENPAI_U[MESSAGE_HEAD_LENGTH+1]	= "/m ";	//门派聊天
	const char		MESSAGETYPE_TALK_PRIVATE_L[MESSAGE_HEAD_LENGTH+1]	= "/S ";	//私聊
	const char		MESSAGETYPE_TALK_PRIVATE_U[MESSAGE_HEAD_LENGTH+1]	= "/s ";	//私聊

	// 最大输入字符长度
	const INT		MAX_INPUT_CHAR_LENGTH			= 64;

	//保存密语对象的最大数量
	const INT		TALK_TO_USER_MAXNUM				= 5;

	//自建频道可以添加的玩家最大数量
	const INT		CHANNEL_INVITE_USER_MAXNUM		=11;

	//说话的时间差（单位：秒）
	const INT		CHAT_TALK_SLICE					= 5;

	//10级以下人物对话时的说话间隔（单位：秒）
	const INT		CHAT_TALK_LV10_SLICE			= 10;

	const INT		CHAT_PINGBI_MAXUSER_NUMBERS		= 20;
	//GM命令的前导字符,长度必须是2个的字符，因为发送函数里只检测头2两个字符
	const char*		GM_TALK_HEADER			= "!!";

	//聊天动作的前导字符
	const char*		CHAT_ACT_HEADER			="*";

	//聊天最大Tab数量
	const INT		CHAT_TAB_MAX_NUMBER				= 7;

	Talk Talk::s_Talk;
	LuaPlus::LuaObject* Talk::s_pMetaTable = NULL;

	Talk::Talk()
	{
		Channel* newChannel = new Channel;
		
		newChannel->SetType(CHAT_TYPE_NORMAL);
		m_listChannel.push_back(newChannel);

		newChannel = new Channel;
		newChannel->SetType(CHAT_TYPE_WORLD);
		m_listChannel.push_back(newChannel);

		newChannel = new Channel;
		newChannel->SetType(CHAT_TYPE_USER);
		m_listChannel.push_back(newChannel);

		//newChannel = new Channel;
		//newChannel->SetType(CHAT_TYPE_TEAM);
		//m_listChannel.push_back(newChannel);

		newChannel = new Channel;
		newChannel->SetType(CHAT_TYPE_COUNTRY);
		m_listChannel.push_back(newChannel);

		newChannel = new Channel;
		newChannel->SetType(CHAT_TYPE_GUILD);
		//m_listChannel.push_back(newChannel);
		
		newChannel = new Channel;
		newChannel->SetType(CHAT_TYPE_SCENE);
		//m_listChannel.push_back(newChannel);

		m_SelfCh = new SelfChannel;
		m_SelfCh->SetType(CHAT_TYPE_SELF);

		m_SysCh = new SystemChannel;
		m_SysCh->SetType(CHAT_TYPE_SYSTEM);

		m_PrvCh = NULL;
		m_UsrCh = NULL;
		m_TeamCh = NULL;
		m_MenPaiCh = NULL;
		m_GuildCh = NULL;

		m_MaxSaveNum = 0;
		m_PrevSendPos = -1;
		m_PrevGMCmdPos = -1;
		m_bPaoPaoRule = FALSE;

		m_CurTab = -1;
		m_TabCfg.resize(CHAT_TAB_MAX_NUMBER);
	}

	VOID Talk::Initial()
	{
		static BOOL bFirst = FALSE;

		if(bFirst) return;

		// 注册时间事件回调函数
		CGameProcedure::s_pEventSystem->RegisterEventHandle("TIME_UPDATE", _SendTalkHelpMsg);
		CGameProcedure::s_pEventSystem->RegisterEventHandle("VIEW_RESOLUTION_CHANGED", _HandleWindowSizeChange);

		// 聊天人物休闲动作生成，及相应的action button
		GenTalkActMap();
		CActionSystem::GetMe()->UserChatMood_Update();

		// 聊天规则设置
		std::list< Channel* >::iterator it;
		for(it=m_listChannel.begin(); it != m_listChannel.end(); it++)
		{
			(*it)->LoadChannelConfig();
		}

		bFirst = TRUE;
	}

	Talk::~Talk()
	{
		SAFE_DELETE(m_SelfCh);
		SAFE_DELETE(m_PrvCh);
		SAFE_DELETE(m_SysCh);

		//below is deleted by m_listChannel
		//SAFE_DELETE(m_UsrCh);
		//SAFE_DELETE(m_TeamCh);
		//SAFE_DELETE(m_MenPaiCh);
		//SAFE_DELETE(m_GuildCh);

		std::list< Channel* >::iterator it;
		for(it=m_listChannel.begin(); it != m_listChannel.end(); it++)
		{
			SAFE_DELETE(*it);
		}

		m_listChannel.clear();
	}
		
	//HistoryMsg Class
	Talk::HistoryMsg::HistoryMsg(ENUM_CHAT_TYPE type)
	{
		m_Name = "";
		m_ChannelType = type;
		m_Data = "";
		m_Time = CGameProcedure::s_pTimeSystem->GetTimeNow();
	}

	VOID Talk::HistoryMsg::SetMsgData(STRING data, CHAR* file, INT line)
	{
#ifdef _MSG_POSITION_SHOW_
		if(file)
		{
			CHAR Info[128] = {0};
			_snprintf(Info, 127, "#W DataEmpty(%s)DataSize(%u)File:%s(Line:%d)", (data.empty())?"Yes":"False",data.size(), file, line);
			m_Data = data + Info;
		}
		else
		{
			m_Data = data;
		}
#else
		m_Data = data;
#endif
	}

	INT Talk::HistoryMsg::SetByPacket(GCChat *packet)
	{
		if(NULL == packet)
		{
			Assert(FALSE);
			return -1;
		}

		CHAR szTmp[MAX_CHAT_SIZE];
		memset(szTmp, 0, MAX_CHAT_SIZE);

		memcpy(szTmp, packet->GetSourName(), packet->GetSourNameSize());
		//talker
		m_Name = szTmp;

		memset(szTmp, 0, MAX_CHAT_SIZE);
		memcpy(szTmp, packet->GetContex(), packet->GetContexSize());
		//talk contex
		m_Data = szTmp;

		//channel type
		m_ChannelType = (ENUM_CHAT_TYPE)packet->GetChatType();

		//saveTime
		m_Time = CGameProcedure::s_pTimeSystem->GetTimeNow();

		return 0;
	}

	VOID Talk::HistoryMsg::SetItemCollect(ItemCollect* pCollect)
	{
		if(!pCollect)	return;
		if(pCollect->empty()) return;

		m_vInfo = *pCollect;
	}

	VOID Talk::HistoryMsg::DeleteInfoElement()
	{
		for ( INT i=0; i<(INT)m_vInfo.size(); ++i )
		{
			if ( CTransferItemSystem::GetMe() )
			{
				CTransferItemSystem::GetMe()->DeleteElement(m_vInfo[i]);
			}
		}
	}

	VOID Talk::HistoryMsg::AddInfoElement()
	{
		for ( INT i=0; i<(INT)m_vInfo.size(); ++i )
		{
			if( CTransferItemSystem::GetMe() )
			{
				CTransferItemSystem::GetMe()->AddElementUseCount( m_vInfo[i]);
			}
		}
	}

	//Channel Class
	BOOL Talk::Channel::TalkNeedCheck()
	{
		BOOL bOk = TRUE;

		TALK_NEED_ARRAY::iterator it;
		for(UINT i = 0; i < CHAT_NEED_NUMBER; ++i)
		{
			switch(m_need[i].type)
			{
			case CHAT_NEED_NONE:
				break;
			case CHAT_NEED_MP:
				{
					CObject* pObj = (CObject*)CObjectManager::GetMe()->GetMySelf();
					INT curMp = ((CObject_Character*)pObj)->GetCharacterData()->Get_MP();

					if(curMp - m_need[i].value < 0)	//相应属性值的修改在服务器端做。
						bOk = FALSE;
				}
				break;
			case CHAT_NEED_ENERGY:
				{
					CObject* pObj = (CObject*)CObjectManager::GetMe()->GetMySelf();
					INT curEnergy = ((CObject_Character*)pObj)->GetCharacterData()->Get_Energy();

					if(curEnergy - m_need[i].value < 0)	//相应属性值的修改在服务器端做。
						bOk = FALSE;
				}
				break;
			case CHAT_NEED_VIGOR:
				{
					CObject* pObj = (CObject*)CObjectManager::GetMe()->GetMySelf();
					INT curVigor = ((CObject_Character*)pObj)->GetCharacterData()->Get_Vigor();

					if(curVigor - m_need[i].value < 0)	//相应属性值的修改在服务器端做。
						bOk = FALSE;
				}
				break;
			default:
				break;
			}

			if(!bOk) break;
		}

		return bOk;
	}

	BOOL Talk::Channel::TalkTimeCheck(UINT dwTimeNow)
	{
		BOOL bOk = TRUE;

		if(0 == m_lastSendTime)
		{
			m_lastSendTime = dwTimeNow;
		}
		else
		{
			UINT dwTime = CGameProcedure::s_pTimeSystem->CalSubTime(m_lastSendTime,dwTimeNow);
			if(dwTime/1000 > GetSliceTime())
			{
				m_lastSendTime = dwTimeNow;
			}
			else
			{
				bOk = FALSE;
			}
		}

		return bOk;
	}

	VOID Talk::Channel::SetNeed(INT tp, INT value)
	{
		if(tp >= CHAT_NEED_NUMBER || tp < 0) return; 
		m_need[tp].type = (CHAT_NEED_TYPE)tp;
		m_need[tp].value = value;
	}

	VOID Talk::Channel::LoadChannelConfig()
	{
		const tDataBase* pTalkConfig = CDataBaseSystem::GetMe()->GetDataBase(DBC_TALK_CONFIG);
		if(pTalkConfig)
		{
			const _DBC_TALK_CONFIG* pLine = (const _DBC_TALK_CONFIG*)pTalkConfig->Search_LineNum_EQU(m_channelType);
			if(pLine)
			{
				/*m_channelName = pLine->szChannelName;
				m_channelHeader = pLine->szChannelHeader;
				m_TimeSlice = pLine->nTimeSlice;

				SetNeed(pLine->nNeedType1, pLine->nNeedValue1);
				SetNeed(pLine->nNeedType2, pLine->nNeedValue2);
				SetNeed(pLine->nNeedType3, pLine->nNeedValue3);*/
			}
		}
	}

	VOID Talk::Channel::SetSendPacket(CGChat *packet, TalkAnalyze* pAna)
	{
		if(!packet || !pAna) return;

		packet->SetChatType(pAna->getChannelType());
		packet->SetContex(pAna->getStr(SEND_STR).c_str());
		packet->SetContexSize((BYTE)pAna->getStr(SEND_STR).size());
		packet->SetTargetName( pAna->getStr(PRIVATE_STR).c_str());
		packet->SetTargetSize( pAna->getStr(PRIVATE_STR).size() );
	}

	//PrivChannel Class
	VOID Talk::PrivateChannel::Clean()
	{
		m_GUID = INVALID_GUID;
		m_InviteQue.clear();
	}

	INT Talk::PrivateChannel::AddToQue(STRING user)
	{
		if(user.empty())
		{
			KLAssert(FALSE);
			return -1;
		}

		if(m_InviteQue.size() == CHANNEL_INVITE_USER_MAXNUM)
		{
			return 1;	//已经邀请的人数满
		}

		if(0 <= IsInQue(user))
		{
			return 2;	//已经在邀请队列中
		}

		m_InviteQue.push_back(user);

		return 0;
	}

	INT Talk::PrivateChannel::IsInQue(STRING user)
	{
		for(UINT i = 0; i < m_InviteQue.size(); ++i)
		{
			if(user == m_InviteQue[i]) return i;
		}
		return -1;
	}

	INT Talk::PrivateChannel::KickFromQue(STRING user)
	{
		std::deque< STRING >::iterator it;
		for(it = m_InviteQue.begin(); it != m_InviteQue.end(); it++)
		{
			if(user == *it)
			{
				m_InviteQue.erase(it);
			}
		}
		return 0;
	}

	STRING Talk::PrivateChannel::GetUserByIndex(UINT idx)
	{
		STRING strR = "";
		if(idx >= m_InviteQue.size())
		{
			return strR;
		}

		strR = m_InviteQue[idx];
		return strR;
	}

	//UserChannel Class
	INT Talk::UserChannel::AddUser(STRING user)
	{
		if(user.empty())
		{
			return -1;
		}
		
		if(0 <= IsInQue(user))
		{
			return 2;	//已经在密语对象队列中
		}
		
		m_UserQue.push_back(user);

		if(m_UserQue.size() > TALK_TO_USER_MAXNUM)
		{
			m_UserQue.pop_front();	//人数满，弹出旧对象
		}
		m_curSelPos = 1;

		return 0;
	}

	INT Talk::UserChannel::IsInQue(STRING user)
	{
		for(UINT i = 0; i < m_UserQue.size(); ++i)
		{
			if(user == m_UserQue[i]) return i;
		}
		return -1;
	}

	VOID Talk::UserChannel::Clean()
	{
		m_UserQue.clear();

		m_curSelPos = -1;
	}

	STRING Talk::UserChannel::GetUserByIndex(UINT idx)
	{
		STRING strR = "";
		if(idx >= m_UserQue.size())
		{
			return strR;
		}

		strR = m_UserQue[idx];
		return strR;
	}

	STRING Talk::UserChannel::SelectFirstUser()
	{
		STRING strR = "";
		if(m_UserQue.empty())
		{
			return strR;
		}
		
		m_curSelPos = 0;
		strR = *(m_UserQue.begin());
		return strR;
	}

	STRING Talk::UserChannel::SelectLastUser()
	{
		STRING strR = "";
		if(m_UserQue.empty())
		{
			return strR;
		}

		m_curSelPos = (INT)m_UserQue.size()-1;
		strR = *(m_UserQue.rbegin());
		return strR;
	}

	STRING Talk::UserChannel::SelectNextUser(BOOL dir)
	{
		STRING strR = "";
		if(m_UserQue.empty() || m_curSelPos < 0)
		{
			return strR;
		}

		if(dir)
		{
			m_curSelPos++;
			if(m_curSelPos == (INT)m_UserQue.size())	m_curSelPos = 0;
		}
		else
		{
			m_curSelPos--;
			if(0 > m_curSelPos) m_curSelPos = (INT)m_UserQue.size()-1;
		}

		strR = m_UserQue[m_curSelPos];
		return strR;
	}

	BOOL Talk::UserChannel::SetSelectByUser(STRING name)
	{
		if(m_UserQue.empty() || name.empty())
		{
			return FALSE;
		}

		UINT i = 0;
		for(; i < m_UserQue.size(); ++i)
		{
			if(name == GetUserByIndex(i)) break;
		}

		if(i != m_UserQue.size()) 
		{
			m_curSelPos = i;
			return TRUE;
		}
		else
			return FALSE;
	}

	VOID Talk::UserChannel::SetSendPacket(CGChat *packet, TalkAnalyze* pAna)
	{
		if(!packet || !pAna) return;
		if(pAna->getChannelType() != CHAT_TYPE_TELL) return;

		Talk::Channel::SetSendPacket(packet, pAna);
		//设置密聊对象
		packet->SetTargetSize((BYTE)pAna->getStr(PRIVATE_STR).size());
		packet->SetTargetName((CHAR*)pAna->getStr(PRIVATE_STR).c_str());

		//填加私聊对象
		AddUser(pAna->getStr(PRIVATE_STR));
	}

	VOID Talk::TeamChannel::SetSendPacket(CGChat *packet, TalkAnalyze* pAna)
	{
		if(!packet || !pAna) return;
		if(pAna->getChannelType() != CHAT_TYPE_TEAM) return;

		Talk::Channel::SetSendPacket(packet, pAna);
		packet->SetTeamID(m_teamID);
	}

	VOID Talk::MenPaiChannel::SetSendPacket(CGChat *packet,TalkAnalyze* pAna)
	{
		if(!packet || !pAna) return;
		if(pAna->getChannelType() != CHAT_TYPE_MENPAI) return;

		Talk::Channel::SetSendPacket(packet, pAna);
		packet->SetMenpaiID((BYTE)m_menpaiID);
	}

	VOID Talk::GuildChannel::SetSendPacket(CGChat *packet, TalkAnalyze* pAna)
	{
		if(!packet || !pAna) return;
		if(pAna->getChannelType() != CHAT_TYPE_GUILD) return;

		Talk::Channel::SetSendPacket(packet, pAna);
		packet->SetGuildID(m_guildID);
	}
//---------------------------------------------------------------------------------------------------------------------
// 事件回调函数，每1秒钟调用一次
	VOID Talk::_SendTalkHelpMsg(const EVENT *pEvent, UINT dwOwnerData)
	{
		static UINT m_count = 0;

		m_count++;
		if(m_count > 300) //每5分钟
		{
			if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
			{
				Talk::s_Talk.ShowTalkHelpMsg();
			}
			m_count = 0;
		}
	}

	VOID Talk::_HandleWindowSizeChange(const EVENT *pEvent, UINT dwOwnerData)
	{
		INT wmsg=0;
		static INT m_height = 0;

		if(pEvent->vArg.size() > 0)
		{
			wmsg = atoi(pEvent->vArg[0].c_str());

			if(wmsg == WM_SIZE || wmsg == WM_EXITSIZEMOVE)
			{
				RECT rect;
				GetClientRect(g_hMainWnd, &rect);

				if(m_height != rect.bottom-rect.top)
				{
					m_height = rect.bottom-rect.top;
					Talk::s_Talk.TalkDisplaySizeChanged(rect.right-rect.left, m_height);
				}
			}
		}
	}
//---------------------------------------------------------------------------------------------------------------------
//LuaPlus界面相关接口
	INT	Talk::GetChannelNumber(LuaPlus::LuaState* state)	//获得玩家可以设为默认频道的数目
	{
		state->PushInteger((INT)m_listChannel.size());

		return 1;
	}

	INT	Talk::GetChannel(LuaPlus::LuaState* state)
	{
		//检查参数
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA: Talk GetChannel Wrong Param1");
			return -1;
		}

		INT nChannelNum = args[2].GetInteger();

		INT nTempIndex = 0;
		std::list< Channel* >::iterator it;
		for(it=m_listChannel.begin(); nTempIndex<nChannelNum; it++, nTempIndex++);
		
		//所需要的频道
		Channel* theChannel = *it;
		//if(theChannel->GetType() != CHAT_TYPE_TELL)
		{
			state->PushString(ChannelType2String(theChannel->GetType()).c_str());
			state->PushString(theChannel->GetName().c_str());
			return 2;
		}
		//else
		//{
		//	//todo_yangjun
		//	state->PushString(ChannelType2String(theChannel->GetType()).c_str());

		//	UserChannel *UsrCh = (UserChannel*)theChannel;
		//	for(UINT i = 0; i < UsrCh->GetUserNum(); ++i)
		//		state->PushString(UsrCh->GetUserByIndex(i).c_str());

		//	return (INT)UsrCh->GetUserNum()+1;
		//}
	}

	INT	Talk::GetChannelHeader(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsString()))
		{
			KLThrow("LUA: Talk GetChannelHeader Wrong Param1");
			return -1;
		}

		STRING strType = args[2].GetString();
		ENUM_CHAT_TYPE type = String2ChannelType(strType);

		/* Bug:Maybe Channel not Create when Lua call this func.
		if(type != CHAT_TYPE_INVALID)
		{
			std::list< Channel* >::iterator it;
			for(it=m_listChannel.begin(); it!=m_listChannel.end(); it++)
			{
				if((*it)->GetType() == type)
				{
					state->PushString((*it)->GetHeader().c_str());
					break;
				}
			}
		}
		*/

		const tDataBase* pTalkConfig = CDataBaseSystem::GetMe()->GetDataBase(DBC_TALK_CONFIG);
		if(pTalkConfig)
		{
			// 此处的 ENUM_CHAT_TYPE 定义还没修改， 所以这里超出范围

			if( type > CHAT_TYPE_GUILD )
			{
				if( type == CHAT_TYPE_SELF )
				{
					state->PushString("self");
					return 1;
				}

				return 0;
			}

			const _DBC_TALK_CONFIG* pLine = (const _DBC_TALK_CONFIG*)pTalkConfig->Search_LineNum_EQU( type );
			if(pLine)
			{
				state->PushString(pLine->szChannelHeader);
			}
		}

		return 1;
	}

	INT	Talk::SendChatMessage(LuaPlus::LuaState* state)
	{
		//检查参数
		LuaStack args(state);
		if (!(args[2].IsString() && args[3].IsString()))
		{
			KLThrow("LUA: Talk GetChannel Wrong Param1 or Param2");
			return 0;
		}

		//check procedure
		if(CGameProcedure::GetActiveProcedure() != (CGameProcedure*)CGameProcedure::s_pProcMain) return 0;

		STRING strTalkCh = args[2].GetString();
		STRING strTalkMsg = args[3].GetString();
		if( strTalkMsg.empty() )
		{
			state->PushString("");
			return 1;
		}

		if( ParGMCharacter(strTalkMsg) )
		{
			return 0;
		}

		//公用的聊天信息发送速度控制变量
		INT bSend = SEND_FAILED_NOREASON;	//是否发送

		//输入字符串分析
		SendTalkAnalyze saN(String2ChannelType(strTalkCh), strTalkMsg);
		saN.doAnalyze(STRING(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name()));
		if(!saN.IsOk())
		{
            if(!(saN.getErrStr().empty()))
			{
				ADDTALKMSG(saN.getErrStr());
			}
			return 0;
		}

		//GM命令解析
		if(saN.getStrType() == STRINGTYPE_GM)
		{
			SendGMCommand(saN.getStr(ORGINAL_STR).c_str());
			return 0;
		}

		//查找频道是否存在
		Talk::Channel *pChannel = NULL;
		std::list<Talk::Channel *>::iterator it = m_listChannel.begin();
		for(; it != m_listChannel.end(); it++)
		{
			if((*it)->GetType() == saN.getChannelType())
			{
				pChannel = *it;
				break;
			}
		}

		if(NULL == pChannel) return 0;

		//根据类型构造不同的消息发送
		CGChat   msg;
		pChannel->SetSendPacket(&msg, &saN);

		ENUM_CHAT_TYPE chatType = (ENUM_CHAT_TYPE)msg.GetChatType();

		//检查该频道的聊天时间限制
		if(pChannel->isClose())
			bSend = SEND_FAILED_LV10;
		else if(!(pChannel->TalkNeedCheck()))
			bSend = SEND_FAILED_NEED;
		else
		{
			if(pChannel->TalkTimeCheck(CGameProcedure::s_pTimeSystem->GetTimeNow()))
				bSend = SEND_CHAT_OK;
			else
				bSend = SEND_FAILED_TIME_SLICE;
		}

		if ( !IsSendSucce(saN.getStr(HISTORY_STR)) )
		{
			bSend = SEND_FAILED_TIME_DATA;
		}
		//else
		//{
		//	bSend = SEND_FAILED_TIME_DATA;
		//}

		//根据策划需求，暂时删除这个功能
		////检查私聊对象是不是被屏蔽了
		//if(chatType == CHAT_TYPE_TELL)
		//{
		//	if(IsInPingBi(STRING(msg.GetTargetName())))
		//	{
		//		bSend = SEND_FAILED_PINGBI;
		//	}
		//}

		if(SEND_CHAT_OK == bSend)
		{
			CNetManager::GetMe()->SendPacket(&msg);

			//保留发送历史记录
			HistoryMsg hmsg;
			hmsg.SetID();
			hmsg.SetChannelType(chatType);
			hmsg.SetMsgName(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name());
			hmsg.SetMsgData(saN.getStr(HISTORY_STR));
			hmsg.SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
			hmsg.SetItemCollect( saN.GetItemCollect() );

			AddToSendHistoryQue(hmsg);
			AddToSendHistoryText(hmsg);
			m_SendTimeHisQue.push_back(hmsg);

			//密聊信息需要自己填加到自己的聊天窗体显示
			if(chatType == CHAT_TYPE_TELL)
			{
				//STRING strType = ChannelType2String(hmsg.GetChannelType());
				//
				//std::vector< STRING > strParam;
				//strParam.push_back(strType);
				//strParam.push_back(msg.GetTargetName());	//私聊的名称处显示对方的名称
				//strParam.push_back(TalkFixTail(STRING(msg.GetContex())));
				//
				//if(CEventSystem::GetMe())
				//{
				//	CEventSystem::GetMe()->PushEvent(GE_CHAT_MESSAGE, strParam);
				//}
				
				//修改成正确的聊天信息
				hmsg.SetMsgName(msg.GetTargetName());
				hmsg.SetMsgData(msg.GetContex());

				//保存历史消息
				//AddToRecvHistoryQue(hmsg);

				//保存最近主动私聊的玩家对象
				AddUserToUserChannel( saN.getStr( PRIVATE_STR ) );
			}
		}
		else
		{
			if(bSend == SEND_FAILED_LV10)
			{
				STRING strTemp = "";
				strTemp = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_Level_Not_Enough");
				ADDTALKMSG(strTemp);//
				CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, "#R等级不够，不能在此频道说话！" );
				//ADDTALKMSG(STRING("#R等级不够，不能在此频道说话！"));
			}
			else if(bSend == SEND_FAILED_TIME_DATA)
			{
				STRING strTemp = "";
				strTemp = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_Talk_Too_Fast");
				ADDTALKMSG(strTemp);//
				CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, "#R请在30秒后发送重复的话语" );
				//ADDTALKMSG(STRING("#R请不要说话速度太快！#B谢谢！"));
			}
			else if(bSend == SEND_FAILED_NEED)
			{
				STRING strTemp = "";
				strTemp = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_Condition_Not_Enough");
				ADDTALKMSG(strTemp);
				//ADDTALKMSG(STRING("#R条件不足，不能说话！"));
			}
			else if(bSend == SEND_FAILED_PINGBI)
			{
				STRING strTemp = "";
				strTemp = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_Obj_PingBi", msg.GetTargetName());
				ADDTALKMSG(strTemp);
			}
			else
			{
				STRING strTemp = "";
				strTemp = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_Talk_Too_Fast");
				ADDTALKMSG(strTemp);
				//ADDTALKMSG(STRING("#R请不要说话速度太快！#B谢谢！"));
			}
		}

		//密聊信息需要填加回输入窗口，方便玩家下次继续密聊
		// 玩家频道命令新规则 Nick 2008.1.10
		switch( chatType )
		{
		case CHAT_TYPE_TELL:
			state->PushString("/S ");
			return 1;
		case CHAT_TYPE_MENPAI:
			state->PushString("/M ");
			return 1;
		case CHAT_TYPE_TEAM:
			state->PushString("/D ");
			return 1;
		case CHAT_TYPE_GUILD:
			state->PushString("/B ");
			return 1;
		case CHAT_TYPE_WORLD:
			state->PushString("/W ");
			return 1;
		}
	
		return 0;
	}

	INT Talk::CreateChannel(LuaPlus::LuaState *state)
	{
		
		if(NULL != m_PrvCh)
		{
			return 1;				//已经创建了自建频道
		}

		GUID_t myGUID = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();

		CGChannelCreate msg;
		msg.SetGUID(myGUID);
		CNetManager::GetMe()->SendPacket(&msg);

		return 0;
	}

	INT Talk::JoinUserToChannel(LuaPlus::LuaState *state)
	{
		if(NULL == m_PrvCh)
		{
			return 1;				//不存在自建频道
		}

		LuaStack args(state);
		if(!(args[2].IsString()))
		{
			KLThrow("LUA: Talk JoinUserToChannel Wrong Param1");
			return -1;
		}

		STRING user = args[2].GetString();

		//todo_yangjun
		return 0;
	}

	INT Talk::KickUserFromChannel(LuaPlus::LuaState *state)
	{
		if(NULL == m_PrvCh)
		{
			return 1;				//不存在自建频道
		}
		
		LuaStack args(state);
		if(!(args[2].IsString()))
		{
			KLThrow("LUA: Talk KickUserFromChannel Wrong Param1");
			return -1;
		}

		STRING user = args[2].GetString();

		//todo_yangjun
		return 0;
	}

	INT Talk::DestoryChannel(LuaPlus::LuaState *state)
	{

		if(NULL == m_PrvCh)
		{
			return 1;				//不存在自建频道
	}
	
		GUID_t myGUID = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();
		
		CGChannelDismiss msg;
		msg.SetGUID(myGUID);
		CNetManager::GetMe()->SendPacket(&msg);

		return 0;
	}

	INT	Talk::InsertHistory(LuaPlus::LuaState *state)
	{
		LuaStack args(state);
		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Talk InsertHistory Wrong Param1");
			return -1;
		}

		INT nIdx = args[2].GetInteger();
		STRING strConfig = args[3].GetString();

		INT idx = 0;
		//CHAR szHis[128] = {0};

		switch(nIdx)
		{
		case 0:	//全部信息
			{
				std::deque< HistoryMsg >::iterator it = m_RecvHisQue.begin();
				std::vector< STRING > strParam;

				for(;it != m_RecvHisQue.end();it++)
				{
					STRING strType = ChannelType2String((*it).GetChannelType());
					CHAR szIdx[16] = {0};
					_snprintf(szIdx,15,"%d", idx);
					char tmpstr[16] = {0};
					_snprintf(tmpstr, 15, "%d", (*it).GetID());
					STRING strID = tmpstr;

					strParam.clear();
					strParam.push_back(strType);
					strParam.push_back((*it).GetMsgName());
					strParam.push_back((*it).GetMsgData());
					strParam.push_back(strID);

					(*it).AddInfoElement();
					
					//if(CEventSystem::GetMe())
					//{
					//	CEventSystem::GetMe()->PushEvent(GE_CHAT_MESSAGE, strParam);
					//}
					AddChatMessageToUI(strParam,1);	//change tab normal board
					//if(FALSE == (*it).IsDisappear())
					//{
					//	AddChatMessageToUI(strParam, (*it).GetIPRegion(),1);	//change tab normal board
					//}
					//else
					//{
					//	AddChatMessageToUI(strParam, (*it).GetIPRegion(),3, (*it).GetDisappearTime()/1000);	//change tab disappear board
					//}
					idx++;

					//_snprintf(szHis, 127, "HistoryMsg(%d):(%s)%s\n", idx, (*it).GetMsgName().c_str(),(*it).GetMsgData().c_str());
					//OutputDebugString(szHis);
				}

				strParam.clear();
				AddChatMessageToUI(strParam,2);	//refresh history wnd
			}
			break;
		case 1:	//系统			
			{
				std::deque< HistoryMsg >::iterator it = m_RecvHisQue.begin();
				std::vector< STRING > strParam;
				for(;it != m_RecvHisQue.end();it++)
				{
					if((*it).GetChannelType() == CHAT_TYPE_SYSTEM )
					{
						char tmpstr[16] = {0};
						_snprintf(tmpstr, 15, "%d", (*it).GetID());
						STRING strID = tmpstr;
						STRING strType = ChannelType2String((*it).GetChannelType());

						strParam.clear();
						strParam.push_back(strType);
						strParam.push_back((*it).GetMsgName());
						strParam.push_back((*it).GetMsgData());
						strParam.push_back(strID);

						(*it).AddInfoElement();

						//if(CEventSystem::GetMe())
						//{
						//	CEventSystem::GetMe()->PushEvent(GE_CHAT_MESSAGE, strParam);
						//}
						AddChatMessageToUI(strParam, 1);	//change tab normal board
						//if(FALSE == (*it).IsDisappear())
						//{
						//	AddChatMessageToUI(strParam, (*it).GetIPRegion(),1);	//change tab normal board
						//}
						//else
						//{
						//	AddChatMessageToUI(strParam, (*it).GetIPRegion(),3, (*it).GetDisappearTime()/1000);	//change tab disappear board
						//}
					}
				}

				strParam.clear();
				AddChatMessageToUI(strParam ,2);	//refresh history wnd
			}
			break;

		case 2:	//帮会
			{
				std::deque< HistoryMsg >::iterator it = m_RecvHisQue.begin();
				std::vector< STRING > strParam;
				for(;it != m_RecvHisQue.end();it++)
				{
					if((*it).GetChannelType() == CHAT_TYPE_GUILD )
					{
						STRING strType = ChannelType2String((*it).GetChannelType());
						char tmpstr[16] = {0};
						_snprintf(tmpstr, 15, "%d", (*it).GetID());
						STRING strID = tmpstr;

						strParam.clear();
						strParam.push_back(strType);
						strParam.push_back((*it).GetMsgName());
						strParam.push_back((*it).GetMsgData());
						strParam.push_back(strID);

						(*it).AddInfoElement();

						//if(CEventSystem::GetMe())
						//{
						//	CEventSystem::GetMe()->PushEvent(GE_CHAT_MESSAGE, strParam);
						//}
						AddChatMessageToUI(strParam, 1);	//change tab normal board
						//if(FALSE == (*it).IsDisappear())
						//{
						//	AddChatMessageToUI(strParam, (*it).GetIPRegion(),1);	//change tab normal board
						//}
						//else
						//{
						//	AddChatMessageToUI(strParam, (*it).GetIPRegion(),3, (*it).GetDisappearTime()/1000);	//change tab disappear board
						//}
					}
				}

				strParam.clear();
				AddChatMessageToUI(strParam,2);	//refresh history wnd
			}
			break;

		case 3:	//队伍
			{
				std::deque< HistoryMsg >::iterator it = m_RecvHisQue.begin();
				std::vector< STRING > strParam;
				for(;it != m_RecvHisQue.end();it++)
				{
					if((*it).GetChannelType() == CHAT_TYPE_TEAM )
					{
						STRING strType = ChannelType2String((*it).GetChannelType());
						char tmpstr[16] = {0};
						_snprintf(tmpstr, 15, "%d", (*it).GetID());
						STRING strID = tmpstr;

						strParam.clear();
						strParam.push_back(strType);
						strParam.push_back((*it).GetMsgName());
						strParam.push_back((*it).GetMsgData());
						strParam.push_back(strID);

						(*it).AddInfoElement();

						//if(CEventSystem::GetMe())
						//{
						//	CEventSystem::GetMe()->PushEvent(GE_CHAT_MESSAGE, strParam);
						//}
						AddChatMessageToUI(strParam,1);	//change tab normal board
						//if(FALSE == (*it).IsDisappear())
						//{
						//	AddChatMessageToUI(strParam, (*it).GetIPRegion(),1);	//change tab normal board
						//}
						//else
						//{
						//	AddChatMessageToUI(strParam, (*it).GetIPRegion(),3, (*it).GetDisappearTime()/1000);	//change tab disappear board
						//}
					}
				}

				strParam.clear();
				AddChatMessageToUI(strParam,2);	//refresh history wnd
			}
			break;

		case 4:	//私聊
			{
				std::deque< HistoryMsg >::iterator it = m_RecvHisQue.begin();
				std::vector< STRING > strParam;
				for(;it != m_RecvHisQue.end();it++)
				{
					if((*it).GetChannelType() == CHAT_TYPE_USER )
					{
						STRING strType = ChannelType2String((*it).GetChannelType());
						char tmpstr[16] = {0};
						_snprintf(tmpstr, 15, "%d", (*it).GetID());
						STRING strID = tmpstr;

						strParam.clear();
						strParam.push_back(strType);
						strParam.push_back((*it).GetMsgName());
						strParam.push_back((*it).GetMsgData());
						strParam.push_back(strID);

						(*it).AddInfoElement();

						//if(CEventSystem::GetMe())
						//{
						//	CEventSystem::GetMe()->PushEvent(GE_CHAT_MESSAGE, strParam);
						//}
						AddChatMessageToUI(strParam,1);	//change tab normal board
						//if(FALSE == (*it).IsDisappear())
						//{
						//	AddChatMessageToUI(strParam, (*it).GetIPRegion(),1);	//change tab normal board
						//}
						//else
						//{
						//	AddChatMessageToUI(strParam, (*it).GetIPRegion(),3, (*it).GetDisappearTime()/1000);	//change tab disappear board
						//}
					}
				}

				strParam.clear();
				AddChatMessageToUI(strParam,2);	//refresh history wnd
			}
			break;

		case 5:	//国家
			{
				std::deque< HistoryMsg >::iterator it = m_RecvHisQue.begin();
				std::vector< STRING > strParam;
				for(;it != m_RecvHisQue.end();it++)
				{
					if((*it).GetChannelType() == CHAT_TYPE_COUNTRY )
					{
						STRING strType = ChannelType2String((*it).GetChannelType());
						char tmpstr[16] = {0};
						_snprintf(tmpstr, 15, "%d", (*it).GetID());
						STRING strID = tmpstr;

						strParam.clear();
						strParam.push_back(strType);
						strParam.push_back((*it).GetMsgName());
						strParam.push_back((*it).GetMsgData());
						strParam.push_back(strID);

						(*it).AddInfoElement();

						//if(CEventSystem::GetMe())
						//{
						//	CEventSystem::GetMe()->PushEvent(GE_CHAT_MESSAGE, strParam);
						//}
						AddChatMessageToUI(strParam,1);	//change tab normal board
						//if(FALSE == (*it).IsDisappear())
						//{
						//	AddChatMessageToUI(strParam, (*it).GetIPRegion(),1);	//change tab normal board
						//}
						//else
						//{
						//	AddChatMessageToUI(strParam, (*it).GetIPRegion(),3, (*it).GetDisappearTime()/1000);	//change tab disappear board
						//}
					}
				}

				strParam.clear();
				AddChatMessageToUI(strParam,2);	//refresh history wnd
			}
			break;

		case 6:
			//自建频道，需要分析相应的配置字串，有选择性的填加信息
			{
				INT icfg[CHAT_TYPE_NUMBER];
				for(INT i = 0; i < CHAT_TYPE_NUMBER; ++i) icfg[i] = 0;
				//getconfig
				for(UINT i = 0; i < strConfig.size() && i < CHAT_TYPE_NUMBER; ++i) icfg[i] = (strConfig.at(i) == '1')?1:0;

				std::deque< HistoryMsg >::iterator it = m_RecvHisQue.begin();
				std::vector< STRING > strParam;
				for(; it != m_RecvHisQue.end();it++)
				{
					if(icfg[(*it).GetChannelType()] == 1)
					{
						STRING strType = ChannelType2String((*it).GetChannelType());
						CHAR szIdx[16] = {0};
						_snprintf(szIdx,15,"%d", idx);

						char tmpstr[16] = {0};
						_snprintf(tmpstr, 15, "%d", (*it).GetID());
						STRING strID = tmpstr;

						strParam.clear();
						strParam.push_back(strType);
						strParam.push_back((*it).GetMsgName());
						strParam.push_back((*it).GetMsgData());
						strParam.push_back(strID);

						(*it).AddInfoElement();
						
						//if(CEventSystem::GetMe())
						//{
						//	CEventSystem::GetMe()->PushEvent(GE_CHAT_MESSAGE, strParam);
						//}
						AddChatMessageToUI(strParam,1);	//change tab normal board
						//if(FALSE == (*it).IsDisappear())
						//{
						//	AddChatMessageToUI(strParam, (*it).GetIPRegion(),1);	//change tab normal board
						//}
						//else
						//{
						//	AddChatMessageToUI(strParam, (*it).GetIPRegion(),3, (*it).GetDisappearTime()/1000);	//change tab disappear board
						//}
					}
					idx++;
				}
				strParam.clear();
				AddChatMessageToUI(strParam, 2);	//refresh history wnd
			}
			break;
		default:
			break;
		}

		return 0;
	}

	INT	Talk::SetMaxSaveNumber(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Talk SetMaxSaveNumber Wrong Param1");
			return -1;
		}

		INT maxNum = args[2].GetInteger();
		if(0 < maxNum) m_MaxSaveNum = maxNum;

		return 0;
	}

	INT	Talk::CreateTab(LuaPlus::LuaState* state)
	{
		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_TAB_CREATE);
		}

		return 0;
	}

	INT	Talk::ConfigTab(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsString()))
		{
			KLThrow("LUA: Talk ConfigTab Wrong Param1");
			return -1;
		}

		if(!(args[3].IsString()))
		{
			KLThrow("LUA: Talk ConfigTab Wrong Param2");
			return -1;
		}

		std::vector< STRING > strParam;
		strParam.push_back(args[2].GetString());
		strParam.push_back(args[3].GetString());

		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_TAB_CONFIG, strParam);
		}

		return 0;
	}

	INT Talk::CreateTabFinish(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsString()) || !(args[3].IsString()) || !(args[4].IsString()))
		{
			KLThrow("LUA: Talk CreateTabFinish Wrong Param");
			return -1;
		}

		STRING strChannelName = args[2].GetString();
		std::vector< STRING > strParam;
		strParam.push_back(args[2].GetString());
		strParam.push_back(args[3].GetString());
		strParam.push_back(args[4].GetString());

		//敏感字符过滤
		if(!CGameProcedure::s_pUISystem->CheckStringFilter(strChannelName, FT_CHAT) )
		{
			STRING strErr = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_InvalidContent");
			ADDTALKMSG(strErr);
			strParam[2] = "cancel";
		}

		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_TAB_CREATE_FINISH, strParam);
		}

		return 0;
	}

	INT Talk::ConfigTabFinish(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsString()) || !(args[3].IsString()) || !(args[4].IsString()))
		{
			KLThrow("LUA: Talk ConfigTabFinish Wrong Param");
			return -1;
		}

		std::vector< STRING > strParam;
		strParam.push_back(args[2].GetString());
		strParam.push_back(args[3].GetString());
		strParam.push_back(args[4].GetString());
		
		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_TAB_CONFIG_FINISH, strParam);
		}

		return 0;
	}

	INT Talk::SaveTab(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Talk SaveTab Wrong Param1");
			return -1;
		}
		if(!(args[3].IsString()))
		{
			KLThrow("LUA: Talk SaveTab Wrong Param2");
			return -1;
		}
		if(!(args[4].IsString()))
		{
			KLThrow("LUA: Talk SaveTab Wrong Param3");
			return -1;
		}

		INT tabId = args[2].GetInteger();			//tab索引
		STRING tabName = args[3].GetString();		//tab名字
		STRING tabConfig = args[4].GetString();		//tab配置
        
		//1~4为玩家自建tab页面
		if(tabId < 1 || tabId > 4) return 0;
		//tab页的名称最多6个ANSI字符长
		if(tabName.empty() || tabName.size() > 6) return 0;
		//tab页的配置必须为9个由0/1组成的字串构成
		if(tabConfig.size() != CHAT_TYPE_NUMBER) return 0;
		for(UINT i=0; i< tabConfig.size(); ++i)
		{
			if(tabConfig.at(i) != 48 && tabConfig.at(i) != 49) return 0;
		}

		KLAssert(5 == sizeof(_OWN_SETTING));

		CHAR szTabName[6+1] = {0};
		strcpy(szTabName,tabName.c_str());
		//生成配置
		CGModifySetting pk1;
		pk1.SetType(SETTING_TYPE_CHAT_TAB1_PART1+2*(tabId-1));
		_OWN_SETTING *pSetting1 = pk1.GetValue();
		pSetting1->CleanUp();
		memcpy(pSetting1, szTabName, sizeof(_OWN_SETTING));

		CGModifySetting pk2;
		pk2.SetType(SETTING_TYPE_CHAT_TAB1_PART2+2*(tabId-1));
		_OWN_SETTING *pSetting2 = pk2.GetValue();
		pSetting2->CleanUp();
		memcpy(&(pSetting2->m_SettingType), szTabName+sizeof(_OWN_SETTING), sizeof(BYTE));
		for(UINT i=0; i< tabConfig.size(); ++i)
		{
			if(tabConfig.at(i) != 48) pSetting2->m_SettingData |= 1<<i;
		}

		//发送到服务器
		CNetManager::GetMe()->SendPacket(&pk1);
		CNetManager::GetMe()->SendPacket(&pk2);

		return 0;
	}

	INT Talk::ClearTab(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Talk ClearTab Wrong Param1");
			return -1;
		}

		INT tabId = args[2].GetInteger();
		if(tabId < 1 || tabId > 4) return 0;
        
		CGModifySetting pk1;
		pk1.SetType(SETTING_TYPE_CHAT_TAB1_PART1+2*(tabId-1));
		pk1.GetValue()->CleanUp();

		CGModifySetting pk2;
		pk2.SetType(SETTING_TYPE_CHAT_TAB1_PART2+2*(tabId-1));
		pk2.GetValue()->CleanUp();

		CNetManager::GetMe()->SendPacket(&pk1);
		CNetManager::GetMe()->SendPacket(&pk2);

		return 0;
	}

	INT Talk::GetChannelType(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsString()))
		{
			KLThrow("LUA: Talk GetChannelType Wrong Param1");
			return -1;
		}

		STRING strType = args[2].GetString();

		state->PushInteger((INT)String2ChannelType(strType));
		return 1;
	}

	INT	Talk::SelectFaceMotion(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsString()))
		{
			KLThrow("LUA: Talk SelectFaceMotion Wrong Param1");
			return -1;
		}

		if(!(args[3].IsString()))
		{
			KLThrow("LUA: Talk SelectFaceMotion Wrong Param2");
			return -1;
		}

		std::vector< STRING > strParam;
		strParam.push_back(args[2].GetString());
		strParam.push_back(args[3].GetString());

		if(0 == strcmp(args[2].GetString(), "sucess"))
		{
			STRING strColor = args[3].GetString();
			for(INT i = 0; i < (INT)strColor.size(); ++i)
			{
				PostMessage(g_hMainWnd, WM_CHAR, (WPARAM)strColor.at(i),0);
			}
			return 0;
		}

		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_FACEMOTION_SELECT, strParam);
		}

		return 0;
	}

	INT	Talk::SelectTextColor(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsString()))
		{
			KLThrow("LUA: Talk SelectTextColor Wrong Param1");
			return -1;
		}

		if(!(args[3].IsString()))
		{
			KLThrow("LUA: Talk SelectTextColor Wrong Param2");
			return -1;
		}

		std::vector< STRING > strParam;
		strParam.push_back(args[2].GetString());
		strParam.push_back(args[3].GetString());

		if(0 == strcmp(args[2].GetString(), "sucess"))
		{
			STRING strColor = args[3].GetString();
			for(INT i = 0; i < (INT)strColor.size(); ++i)
			{
				PostMessage(g_hMainWnd, WM_CHAR, (WPARAM)strColor.at(i),0);
			}
			return 0;
		}

		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_TEXTCOLOR_SELECT, strParam);
		}

		return 0;
	}

	INT Talk::SaveOldTalkMsg(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsString()) || !(args[3].IsString()))
		{
			KLThrow("LUA: Talk SaveOldTalkMsg Wrong Param");
			return -1;
		}

		STRING strTalkCh = args[2].GetString();
		STRING strMsg = args[3].GetString();
		
		if(strMsg.empty()) return 0;
		//if(strMsg.at(0) == '@') return 0;
		//if(IsActCommand(strMsg.c_str())) return 0;

		TalkAnalyze taN( String2ChannelType(strTalkCh), strMsg );
		taN.CheckItemElementInfo();

		HistoryMsg hmsg;
		hmsg.SetID();
		hmsg.SetChannelType(String2ChannelType(strTalkCh));
		hmsg.SetMsgName(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name());
		hmsg.SetMsgData(strMsg);
		hmsg.SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
		hmsg.SetItemCollect( taN.GetItemCollect() );

		//检查消息队列是不是已经存在完全相同的内容了
		INT k=0;
		for (k=0; k<(INT)m_SendHisTextQue.size(); ++k )
		{
			HistoryMsg	*pMsg = &( m_SendHisTextQue[k] );

			if ( pMsg->GetMsgData() == hmsg.GetMsgData() )
			{
				if( *(pMsg->GetItemCollect()) == *(hmsg.GetItemCollect()) )break;
			}
		}
		if(k != (INT)m_SendHisTextQue.size()) return 0;

		//OK继续添加
		AddToSendHistoryQue(hmsg);

		return 0;
	}

	INT Talk::ChangeCurrentChannel(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsString()))
		{
			KLThrow("LUA: Talk ChangeCurrentChannel Wrong Param1");
			return -1;
		}

		if(!(args[3].IsString()))
		{
			KLThrow("LUA: Talk ChangeCurrentChannel Wrong Param2");
			return -1;
		}

		if(!(args[4].IsInteger()))
		{
			KLThrow("LUA: Talk ChangeCurrentChannel Wrong Param3");
			return -1;
		}


		STRING strCh = args[2].GetString();
		STRING strName = args[3].GetString();

		INT step = args[4].GetInteger();
		step = (0 >= step)?-1:1;

		ENUM_CHAT_TYPE curtype = String2ChannelType(strCh);
		if(CHAT_TYPE_INVALID != curtype)
		{
			std::list<Talk::Channel *>::iterator it = m_listChannel.begin();
			for(;it != m_listChannel.end(); it++)
			{
				if(curtype == (*it)->GetType()) break;
			}

			if(it != m_listChannel.end()) //lua脚本传过来的频道一定要合法
			{
				std::list<Talk::Channel *>::iterator itfin;

				//if(CHAT_TYPE_TELL == curtype && m_UsrCh) //当前选择的是“密聊”频道
				//{
				//	if(m_UsrCh->IsLastUserSelect() && step >= 0)
				//	{
				//		itfin = ++it;
				//		if(itfin == m_listChannel.end()) itfin = m_listChannel.begin();
				//	}
				//	else if(m_UsrCh->IsFirstUserSelect() && step < 0)
				//	{
				//		if(it == m_listChannel.begin())
				//		{
				//			std::list<Talk::Channel *>::iterator itend = m_listChannel.end();
				//			itfin = --itend;
				//		}
				//		else 
				//			itfin = --it;
				//	}
				//	else
				//	{
				//		itfin = it;
				//	}

				//	if(CEventSystem::GetMe())
				//	{
				//		std::vector< STRING > strParam;
				//		strParam.push_back(ChannelType2String((*itfin)->GetType()));

				//		if(CHAT_TYPE_TELL == (*itfin)->GetType())
				//			strParam.push_back(m_UsrCh->SelectNextUser(step));
				//		else
				//			strParam.push_back((*itfin)->GetName());
				//			//strParam.push_back((*itfin)->GetHeader());

				//			
				//		//return value for lua.
				//		for(int i=0; i < 2; ++i)
				//			state->PushString(strParam[i].c_str());
				//	}//end of if(CEventSystem::GetMe())

				//}
				//else if(CHAT_TYPE_TELL != curtype) //当前选择的不是“密聊”频道
				{
					if(step >= 0)
					{
						itfin = ++it;
						if(itfin == m_listChannel.end()) itfin = m_listChannel.begin();
					}
					else if(step < 0)
					{
						if(it == m_listChannel.begin())
						{
							std::list<Talk::Channel *>::iterator itend = m_listChannel.end();
							itfin = --itend;
						}
						else 
							itfin = --it;
					}

					if(CEventSystem::GetMe())
					{
						std::vector< STRING > strParam;
						strParam.push_back(ChannelType2String((*itfin)->GetType()));

						//if(CHAT_TYPE_TELL == (*itfin)->GetType() && step >= 0)
						//	strParam.push_back(m_UsrCh->SelectFirstUser());
						//else if(CHAT_TYPE_TELL == (*itfin)->GetType() && step < 0)
						//	strParam.push_back(m_UsrCh->SelectLastUser());
						//else
							strParam.push_back((*itfin)->GetName());
							//strParam.push_back((*itfin)->GetHeader());
							
						//return value for lua.
						for(int i=0; i < 2; ++i)
							state->PushString(strParam[i].c_str());
					}//end of if(CEventSystem::GetMe())
				}
			}// end of if(it != m_listChannel.end())
			else
			{
				KLThrow("LUA: Talk ChangeCurrentChannel Wrong Current Channel Send from Lua!<2>");
				return -1;
			}
		}

		return 2;
	}

	INT Talk::ModifyChatTxt(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsString()) || !(args[3].IsString()) || !(args[4].IsString()))
		{
			KLThrow("LUA: Talk ModifyChatTxt Wrong Param");
			return -1;
		}

		STRING strType = args[2].GetString();
		STRING strName = args[3].GetString();
		STRING strOldTxt = args[4].GetString();

		STRING strNewTxt = strOldTxt;
		//remove msg head.
		if(!strOldTxt.empty())
		{
			if(strOldTxt.at(0) == '/' && strOldTxt.at(1) != '/')
			{
				if(STRING::npos == strOldTxt.find_first_of(" "))
					strNewTxt = "";
				else
					strNewTxt = strOldTxt.substr(strOldTxt.find_first_of(" ")+1);
			}

			if(2 > strOldTxt.size())
				strNewTxt = "";
		}

		ENUM_CHAT_TYPE type = String2ChannelType(strType);

		//下面的内容是在修改字串的时候，先增加TransperItem的计数，这样才能保证正确。
		TalkAnalyze taN( type, strNewTxt );
		taN.CheckItemElementInfo();
		
		HistoryMsg hmsg;
		hmsg.SetID();
		hmsg.SetChannelType( type );
		hmsg.SetMsgName( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name() );
		hmsg.SetMsgData( taN.getStr( ORGINAL_STR ) );
		hmsg.SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow() );
		hmsg.SetItemCollect( taN.GetItemCollect() );

		hmsg.AddInfoElement();		//这句是关键


		if(CHAT_TYPE_TELL == type)
		{
			//set selected user idx in m_UsrCh.
			if(m_UsrCh)
			{
				for(UINT i=0; i < m_UsrCh->GetUserNum(); ++i)
				{
					if(strName == m_UsrCh->GetUserByIndex(i))
					{
						m_UsrCh->SetSelectByUser(strName);
						break;
					}
				}
			}
			
			STRING strFinal = "/s " + strName + " " +strNewTxt;
			state->PushString(strFinal.c_str());
		}
		else
			state->PushString(strNewTxt.c_str());

		return 1;
	}

	INT	Talk::ShowContexMenu(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsString()))
		{
			KLThrow("LUA: Talk ShowContexMenu Wrong Param1");
			return -1;
		}


		m_PrevMenuLink = args[2].GetString();
		STRING data = args[3].GetString();
		if(m_PrevMenuLink.empty())
			return 0;

		POINT pt = CInputSystem::GetMe()->MouseGetPos();
		CHAR szXPos[32]; _snprintf(szXPos, 32, "%d", pt.x);
		CHAR szYPos[32]; _snprintf(szYPos, 32, "%d", pt.y);

		std::vector< STRING > vParam;

		vParam.push_back("chat_private");
		vParam.push_back("-1");
		vParam.push_back(szXPos);
		vParam.push_back(szYPos);
		vParam.push_back(m_PrevMenuLink);
		vParam.push_back(data);
		CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);

		return 0;
	}

	INT	Talk::ContexMenuTalk(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsString()))
		{
			if(m_PrevMenuLink.empty()) return 0;
		}
		else if(args[2].IsString())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_CHANGE_PRIVATENAME, args[2].GetString());
			return 0;
		}

		CEventSystem::GetMe()->PushEvent(GE_CHAT_CHANGE_PRIVATENAME, m_PrevMenuLink.c_str());
		m_PrevMenuLink.erase();

		return 0;
	}

	INT Talk::ContexMenuCopy(LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if(!(args[2].IsString()))
		{
			KLThrow("LUA: Talk ContexMenuCopy Wrong Param1"); 
			return 0;
		}
		const char* str = args[2].GetString();
		
		CDataPool::GetMe()->CopyTextToClipboard(NULL, str );

		return 0;
	}

	INT Talk::ChangeClipboardTxt(LuaPlus::LuaState* state )
	{
		LuaStack args( state );

		if ( !( args[2].IsString() ) )
		{
			KLThrow("LUA: Talk ContexMenuCopy Wrong Param1"); 
			return 0;
		}

		STRING str = args[2].GetString();
		for (STRING::iterator iter = str.begin(); iter != str.end(); ++iter )
		{
			if ( (*iter)=='[' || (*iter)==']' )
				(*iter) = ' ';
		}
		state->PushString( str.c_str() );
		return 1;
	}

	INT Talk::GetCurrentSelectName( LuaPlus::LuaState* state )
	{
		state->PushString( m_PrevMenuLink.c_str() );
		return 1;
	}

	INT Talk::HandleMenuAction(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsString()))
		{
			KLThrow("LUA: Talk HandleMenuAction Wrong Param1");
			return -1;
		}

		STRING strOp = args[2].GetString();

		if(strOp == "Name")
		{
			if(m_PrevMenuLink.empty()) return 0;
			state->PushString(m_PrevMenuLink.c_str());
		}
		else if(strOp == "PingBi")
		{
			if(m_PrevMenuLink.empty()) return 0;
			AddPingBi(m_PrevMenuLink);
		}
		m_PrevMenuLink.erase();

		return 1;
	}

	INT Talk::GetTalkTemplateString_Lua(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsString()))
		{
			KLThrow("LUA: Talk GetTalkTemplateString_Lua Wrong Param1");
			return -1;
		}
		if(!(args[3].IsString()))
		{
			KLThrow("LUA: Talk GetTalkTemplateString_Lua Wrong Param2");
			return -1;
		}

		STRING strTalker = args[2].GetString();
		STRING strContex = args[3].GetString();

		LuaTalkAnalyze luN(CHAT_TYPE_INVALID, strContex);
		luN.doAnalyze(strTalker);

		if(!(luN.IsOk()))
		{
			if(!(luN.getErrStr().empty())) 
			{
				ADDTALKMSG(luN.getErrStr());
			}
			return 0;
		}

		state->PushString(luN.getStr(TALK_STR).c_str());

		return 1;
	}

	INT Talk::EnumChatMood(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		//检查参数
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA: EnumChatMood Wrong Param1");
		}

		INT nIndex = args[2].GetInteger();

		//查询动作
		if(nIndex >= 0)
		{
			tActionItem* pActionItem = CActionSystem::GetMe()->EnumAction(nIndex , NAMETYPE_CHATMOOD);
			if(pActionItem)
			{
				LuaObject objReturn = state->BoxPointer(pActionItem);
				objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
				objReturn.PushStack();

				return 1;
			}
		}

		//非法ActionItem
		LuaObject objReturn = state->BoxPointer(&(CActionItem::s_InvalidAction));
		objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
		objReturn.PushStack();

		return 1;
	}

	INT Talk::ShowChatMood(LuaPlus::LuaState* State)
	{
		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_ACT_SELECT);
		}

		return 0;
	}

	INT Talk::HideChatMood(LuaPlus::LuaState* State)
	{
		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_ACT_HIDE);
		}

		return 0;
	}

	INT Talk::ShowPingBi(LuaPlus::LuaState* State)
	{
		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_PINGBI_LIST);
		}
		return 0;
	}

	INT Talk::GetPingBiNum(LuaPlus::LuaState* State)
	{
		State->PushInteger((INT)m_PingBiQue.size());
		return 1;
	}

	INT Talk::GetPingBiName(LuaPlus::LuaState* State)
	{
		LuaStack args(State);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA: GetPingBiName Wrong Param1");
		}

		INT lidx = args[2].GetInteger();

		if(lidx > (INT)m_PingBiQue.size() || lidx < 0) return 0;
		if(m_PingBiQue.empty()) return 0;

		State->PushString(m_PingBiQue.at(lidx).c_str());

		return 1;
	}

	INT Talk::DelPingBi(LuaPlus::LuaState* State)
	{
		LuaStack args(State);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA: GetPingBiName Wrong Param1");
		}

		INT lidx = args[2].GetInteger();

		if(lidx > (INT)m_PingBiQue.size() || lidx < 0) return 0;
		if(m_PingBiQue.empty()) return 0;

		DelPingBi(m_PingBiQue.at(lidx));

		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_PINGBI_UPDATE);
		}

		return 0;
	}

	INT Talk::GetCurInputLanguage(LuaPlus::LuaState* state)
	{
		INT imeState = CGameProcedure::s_pUISystem->GetCurrentIMEState();
		
		if(1 == imeState)
		{
			state->PushInteger(2052);
		}
		else if(2 == imeState)
		{
			state->PushInteger(1033);
		}
		else
		{
			state->PushInteger(1);
		}

		return 1;
	}


	INT Talk::GetContexMenuObjectName(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( m_PrevMenuLink.empty() )
			state->PushString( "" );
		else
			state->PushString( m_PrevMenuLink.c_str() );

		return 1;
	}

	INT Talk::ClearAllHistoryFrame(LuaPlus::LuaState* state )
	{
		//m_SendHisQue.clear();
		m_RecvHisQue.clear();
	//	m_GMCmdHisQue.clear();

		return 0;
	}

	//INT Talk::GetSendHistoryText(LuaPlus::LuaState *state)
	//{
	//	LuaStack args(state);
	//	if (!(args[2].IsInteger()))
	//	{
	//		KLThrow("LUA: GetSendHistoryText Wrong Param1");
	//	}
	//	INT lidx = args[2].GetInteger();

	//	HistoryMsg hmsg = m_SendHisTextQue[lidx];
	//	state->PushString( hmsg.GetMsgData().c_str() );

	//	return 1;
	//}

	//INT Talk::GetSendHistorySize( LuaPlus::LuaState* state )
	//{
	//	state->PushInteger( m_SendHisTextQue.size() );
	//	return 1;
	//}

//---------------------------------------------------------------------------------------------------------------------
//网络处理相关接口
	INT Talk::HandleRecvTalkPacket(GCChat *pPacket)
	{
		if( NULL == pPacket)
		{
			KLAssert(FALSE);
			return -1;
		}

		// 如果发言人在黑名单里，则不显示
		if( CDataPool::GetMe()->GetRelation()->IsBlackName( pPacket->GetSourName() ) )
		{
			return 0;
		}

		// 如果发言人在屏蔽列表里，不显示
		if(IsInPingBi(STRING(pPacket->GetSourName())))
		{
			return 0;
		}

		HistoryMsg msg;
		msg.SetID();
		if( 0 == msg.SetByPacket(pPacket) )
		{

			STRING strType = ChannelType2String(msg.GetChannelType());
			
			RecvTalkAnalyze reN(msg.GetChannelType(), msg.GetMsgData());
			reN.doAnalyze(msg.GetMsgName());

			if(!reN.IsOk())
			{
				if(!(reN.getErrStr().empty())) 
				{
					ADDTALKMSG(reN.getErrStr());
				}
				return 0;
			}

			if(!HandleSpecialSelfString(&reN))
			{
				std::vector< STRING > strParam;
				char tmpstr[16] = {0};
				_snprintf(tmpstr, 15, "%d", msg.GetID());
				STRING strID = tmpstr;
				strParam.push_back(ChannelType2String((ENUM_CHAT_TYPE)reN.getChannelType()));
				strParam.push_back(reN.getTalker());
				strParam.push_back(reN.getStr(TALK_STR));
				strParam.push_back(strID);

				msg.SetItemCollect(reN.GetItemCollect());
				//AddChatMessageToUI(strParam);	//normal cgchat handle
				
				//if( CEventSystem::GetMe() )
				//{
				//	CEventSystem::GetMe()->PushEvent(GE_CHAT_MESSAGE, strParam);
				//}
				//if(IsTabReceiveChannelMsg(m_CurTab, reN.getChannelType()))
				//{
				//	/*CEventSystem::GetMe()->PushEvent(GE_CHAT_MESSAGE, strParam);*/
				//	AddChatMessageToUI(strParam);	//normal cgchat handle
				//}
				//else
				//{
				//	if(reN.getChannelType() == CHAT_TYPE_SYSTEM )
				//	{
				//		AddChatMessageToUI(strParam,4);		//add disappear board
				//	}
				//}

				msg.SetChannelType((ENUM_CHAT_TYPE)reN.getChannelType());
				msg.SetMsgData(reN.getStr(HISTORY_STR));
				
				// 保存历史消息
				AddToRecvHistoryQue(msg);

				//泡泡信息显示
				if( !( reN.getStr(PAOPAO_STR).empty() ) )
				{
					INT extLength = 0;
					/*switch(reN.getChannelType()) 
					{
					case CHAT_TYPE_TEAM:
						extLength += (INT)(COLORMSGFUNC("GMGameInterface_Script_Talk_Info_SetPaoPaoTxt").size());
						break;
					default:
						break;
					}*/
					if ( reN.getChannelType() != CHAT_TYPE_TEAM )
					{
						SetPaoPaoTxt(reN.getTalker(), reN.getStr(PAOPAO_STR), reN.getStrType(), extLength);
					}

					
				}

				// 聊天动作命令还要进行做具体的动作
				if( reN.getStrType() == STRINGTYPE_ACT )
				{
					STRING strBackUp = reN.getStr(ORGINAL_STR);
					STRING strAct = strBackUp.substr(strlen(CHAT_ACT_HEADER), strBackUp.find(" ")-strlen(CHAT_ACT_HEADER));
					DoAct(strAct.c_str(), reN.getTalker().c_str());
				}
			}
		}

		return 0;
	}

	/*
	HandleSpecialSelfString					服务器端脚本中配置的特殊控制信息
	msg:
		"@*;flagadd;100.0;103.0;tipmsg"		填加方向点
		"@*;flashadd;150.0;103.0;tipmsg"	填加闪光点
		"@*;flagdel;100.0;103.0;tipmsg"		删除方向点
		"@*;flashdel;150.0;103.0;tipmsg"	删除闪光点
		"@*;npcpaopao;idServer;idTable"		怪物泡泡
	 */
	BOOL Talk::HandleSpecialSelfString(TalkAnalyze* pAna)
	{
		if( !pAna ) 
			return FALSE;

		// 不是 系统消息 或 仅客户端使用的消息
		if( pAna->getChannelType() != CHAT_TYPE_SELF && 
			pAna->getChannelType() != CHAT_TYPE_SYSTEM )
			return FALSE;
		
		BOOL bHandle = TRUE;

		switch( pAna->getStrType() ) 
		{
		case STRINGTYPE_FLAGADD:
			// 填加方向点
			HandleFlagAdd(pAna->getStr(ORGINAL_STR).c_str());
			break;
		case STRINGTYPE_FLAGDEL:
			// 删除方向点
			HandleFlagDel(pAna->getStr(ORGINAL_STR).c_str());
			break;
		case STRINGTYPE_FLASHADD:
			// 填加闪光点
			HandleFlashAdd(pAna->getStr(ORGINAL_STR).c_str());
			break;
		case STRINGTYPE_FLASHDEL:
			// 删除闪光点
			HandleFlashDel(pAna->getStr(ORGINAL_STR).c_str());
			break;
		case STRINGTYPE_FLAG_NPC_ADD:	
			// 用NPC名字来添加指示点
			HandleFlagNPCAdd(pAna->getStr(ORGINAL_STR).c_str());
			break;
		case STRINGTYPE_FLAG_POS_ADD:
			// 按x,z来添加指示点
			HandleFlagPOSAdd(pAna->getStr(ORGINAL_STR).c_str());
			break;
		case STRINGTYPE_FLASH_NPC_ADD:	
			// 用NPC名字来添加黄色指示点
			HandleFlashNPCAdd(pAna->getStr(ORGINAL_STR).c_str());
			break;
		case STRINGTYPE_FLASH_POS_ADD:	
			// 用x,z来添加黄色指示点
			HandleFlashPOSAdd(pAna->getStr(ORGINAL_STR).c_str());
			break;
		case STRINGTYPE_MONSTER_PAOPAO:
			// 怪物泡泡
			HandleMonsterPaoPao(pAna->getStr(ORGINAL_STR).c_str());
			break;
		case STRINGTYPE_SERVERMSG_TALK:
			// 服务器端传送过来的提示信息
			HandleServerMsgTalk(pAna->getStr(ORGINAL_STR).c_str(),pAna->getChannelType());
			break;
		default:
			bHandle = FALSE;
			break;
		}

		return bHandle;
	}

	VOID Talk::HandleFlagAdd(LPCTSTR msg)
	{
		std::vector<std::string> vCmd;
		INT nNumber = DBC::DBCFile::_ConvertStringToVector(msg,vCmd,";",TRUE,TRUE);

		std::list< MAP_POS_DEFINE >* pActiveList = CWorldManager::GetMe()->GetObjectListByClass( MINIMAP_TYPE_ACTIVE );
		if(!pActiveList) return;

		if(nNumber != 6)
		{
			KLThrow("Talk HandleSpecialSelfString: Not Enough StringRule.");
			return;
		}

		float x,z;
		DWORD sceneId;
		sscanf(vCmd[SELF_SCENEID].c_str(), " %u", &sceneId);
		sscanf(vCmd[SELF_X].c_str(), " %f", &x);
		sscanf(vCmd[SELF_Z].c_str(), " %f", &z);

		MAP_POS_DEFINE data;
		data.dwSceneID = sceneId;
		data.pos.x = x;
		data.pos.z = z;
		_snprintf(data.name, sizeof(data.name),"%s", vCmd[SELF_TIP].c_str());

		pActiveList->push_back(data);
	}

	VOID Talk::HandleFlagDel(LPCTSTR msg)
	{
		std::vector<std::string> vCmd;
		INT nNumber = DBC::DBCFile::_ConvertStringToVector(msg,vCmd,";",TRUE,TRUE);

		std::list< MAP_POS_DEFINE >* pActiveList = CWorldManager::GetMe()->GetObjectListByClass( MINIMAP_TYPE_ACTIVE );
		if(!pActiveList) return;

		if(nNumber != 6)
		{
			KLThrow("Talk HandleSpecialSelfString: Not Enough StringRule.");
			return;
		}

		float x,z;
		DWORD sceneId;
		sscanf(vCmd[SELF_SCENEID].c_str(), " %u", &sceneId);
		sscanf(vCmd[SELF_X].c_str(), " %f", &x);
		sscanf(vCmd[SELF_Z].c_str(), " %f", &z);

		std::list< MAP_POS_DEFINE >::iterator it = pActiveList->begin();
		while(it != pActiveList->end())
		{
			if(x == (*it).pos.x && z == (*it).pos.z && sceneId == (*it).dwSceneID)
			{
				pActiveList->erase(it);
				it = pActiveList->begin();
				continue;
			}

			it++;
		}
	}

	VOID Talk::HandleFlashAdd(LPCTSTR msg)
	{
		std::vector<std::string> vCmd;
		INT nNumber = DBC::DBCFile::_ConvertStringToVector(msg,vCmd,";",TRUE,TRUE);

		std::list< MAP_POS_DEFINE >* pFlashLish = CWorldManager::GetMe()->GetObjectListByClass( MINIMAP_TYPE_FLASH );
		if(!pFlashLish) return;

		if(nNumber != 6)
		{
			KLThrow("Talk HandleSpecialSelfString: Not Enough StringRule.");
			return;
		}

		float x,z;
		DWORD sceneId;
		sscanf(vCmd[SELF_SCENEID].c_str(), " %u", &sceneId);
		sscanf(vCmd[SELF_X].c_str(), " %f", &x);
		sscanf(vCmd[SELF_Z].c_str(), " %f", &z);

		MAP_POS_DEFINE data;
		data.dwSceneID = sceneId;
		data.pos.x = x;
		data.pos.z = z;
		_snprintf(data.name, sizeof(data.name),"%s", vCmd[SELF_TIP].c_str());

		pFlashLish->clear();
		pFlashLish->push_back(data);
	}

	VOID Talk::HandleFlashDel(LPCTSTR msg)
	{
		std::vector<std::string> vCmd;
		INT nNumber = DBC::DBCFile::_ConvertStringToVector(msg,vCmd,";",TRUE,TRUE);

		std::list< MAP_POS_DEFINE >* pFlashLish = CWorldManager::GetMe()->GetObjectListByClass( MINIMAP_TYPE_FLASH );
		if(!pFlashLish) return;

		if(nNumber != 6)
		{
			KLThrow("Talk HandleSpecialSelfString: Not Enough StringRule.");
			return;
		}

		float x,z;
		DWORD sceneId;
		sscanf(vCmd[SELF_SCENEID].c_str(), " %u", &sceneId);
		sscanf(vCmd[SELF_X].c_str(), " %f", &x);
		sscanf(vCmd[SELF_Z].c_str(), " %f", &z);

		std::list< MAP_POS_DEFINE >::iterator it = pFlashLish->begin();
		while(it != pFlashLish->end())
		{
			if(x == (*it).pos.x && z == (*it).pos.z && sceneId == (*it).dwSceneID)
			{
				pFlashLish->erase(it);
				it = pFlashLish->begin();
				continue;
			}

			it++;
		}
	}

	VOID Talk::HandleFlagNPCAdd(LPCTSTR msg)
	{
		std::vector<std::string> vCmd;
		INT nNumber = DBC::DBCFile::_ConvertStringToVector(msg,vCmd,";",TRUE,TRUE);

		if(nNumber != 6)
		{
			KLThrow("Talk HandleSpecialSelfString: FlagNPCAdd error.");
			return;
		}

		float x,z;
		INT sceneId;
		sscanf(vCmd[SELF_SCENEID].c_str(), " %d", &sceneId);
		sscanf(vCmd[SELF_X].c_str(), " %f", &x);
		sscanf(vCmd[SELF_Z].c_str(), " %f", &z);

		if(CActivePosManager::GetMe())
		{
			CActivePosManager::GetMe()->AddActivePos(true, vCmd[SELF_NPC_NAME].c_str(), x,z, sceneId);
		}
	}

	VOID Talk::HandleFlagPOSAdd(LPCTSTR msg)
	{
		std::vector<std::string> vCmd;
		INT nNumber = DBC::DBCFile::_ConvertStringToVector(msg,vCmd,";",TRUE,TRUE);

		if(nNumber != 6)
		{
			KLThrow("Talk HandleSpecialSelfString: FlagPOSAdd error.");
			return;
		}

		float x,z;
		INT sceneId;
		sscanf(vCmd[SELF_SCENEID].c_str(), " %d", &sceneId);
		sscanf(vCmd[SELF_X].c_str(), " %f", &x);
		sscanf(vCmd[SELF_Z].c_str(), " %f", &z);

		if(CActivePosManager::GetMe())
		{
			CActivePosManager::GetMe()->AddActivePos(false, vCmd[SELF_TIP].c_str(), x,z, sceneId);
		}
	}

	VOID Talk::HandleFlashNPCAdd(LPCTSTR msg)
	{
		std::vector<std::string> vCmd;
		INT nNumber = DBC::DBCFile::_ConvertStringToVector(msg,vCmd,";",TRUE,TRUE);

		if(nNumber != 6)
		{
			KLThrow("Talk HandleSpecialSelfString: FlashNPCAdd error.[%s]", msg);
			return;
		}

		float x,z;
		INT sceneId;
		sscanf(vCmd[SELF_SCENEID].c_str(), " %d", &sceneId);
		sscanf(vCmd[SELF_X].c_str(), " %f", &x);
		sscanf(vCmd[SELF_Z].c_str(), " %f", &z);

		if(CActivePosManager::GetMe())
		{
			CActivePosManager::GetMe()->AddFlashPos(true, vCmd[SELF_NPC_NAME].c_str(), x,z, sceneId);
		}
	}

	VOID Talk::HandleFlashPOSAdd(LPCTSTR msg)
	{
		std::vector<std::string> vCmd;
		INT nNumber = DBC::DBCFile::_ConvertStringToVector(msg,vCmd,";",TRUE,TRUE);

		if(nNumber != 6)
		{
			KLThrow("Talk HandleSpecialSelfString: FlashPOSAdd error.[%s]", msg);
			return;
		}

		float x,z;
		INT sceneId;
		sscanf(vCmd[SELF_SCENEID].c_str(), " %d", &sceneId);
		sscanf(vCmd[SELF_X].c_str(), " %f", &x);
		sscanf(vCmd[SELF_Z].c_str(), " %f", &z);

		if(CActivePosManager::GetMe())
		{
			CActivePosManager::GetMe()->AddFlashPos(false, vCmd[SELF_TIP].c_str(), x,z, sceneId);
		}
	}

	VOID Talk::HandleMonsterPaoPao(LPCTSTR msg)
	{
		std::vector<std::string> vCmd;
		INT nNumber = DBC::DBCFile::_ConvertStringToVector(msg,vCmd,";",TRUE,TRUE);

		if(nNumber != 4)
		{
			KLThrow("Talk HandleSpecialSelfString: npcpaopao error.");
			return;
		}

		INT idServer, idMsg;
		sscanf(vCmd[SELF_MONSTER_SERVER_ID].c_str(), " %d", &idServer);
		sscanf(vCmd[SELF_MONSTER_PAOPAO_ID].c_str(), " %d", &idMsg);

		SetMonsterPaoPaoText(idServer, idMsg);
	}


	VOID Talk::HandleServerMsgTalk(LPCTSTR msg, INT channelType)
	{
		std::vector<std::string> vCmd;
		INT nNumber = DBC::DBCFile::_ConvertStringToVector(msg,vCmd,";",TRUE,TRUE);

		if(nNumber < 3)
		{
			KLThrow("Talk HandleSpecialSelfString: srvmsg error.");
			return;
		}

		enum {
			P1 = SELF_SERVERMSG_PARAM1,
			P2,
			P3,
			P4,
			P5,
			P6,
			P7,
			P8,
			P9,
			P10,
		};
		INT paramNum =(INT)(vCmd.size()-SELF_SERVERMSG_PARAM1);
		STRING strTmp= "";
		STRING strHeader = vCmd[SELF_SERVERMSG_TEMPLAT].substr(0,4);
		if(strHeader == "DBG:")
		{
			vCmd[SELF_SERVERMSG_TEMPLAT] = vCmd[SELF_SERVERMSG_TEMPLAT].substr(4);
		}

		// 滚动公告的状态解析		20100712 BLL
		// 例如：
		// "@*;SrvMsg;SCL:这是一条滚动消息内容"
		if(strHeader == "SCL:")//横滚+横滚颜色默认
		{
			if(CEventSystem::GetMe())
			{
				CEventSystem::GetMe()->PushEvent( GE_SET_SCROLL_INFO, vCmd[SELF_SERVERMSG_TEMPLAT].substr(4).c_str(), "0" );
			}
			return;
		}
		if(strHeader == "SUC:") //横滚+横滚颜色使用传过来的值
		{
			if(CEventSystem::GetMe())
			{
				CEventSystem::GetMe()->PushEvent( GE_SET_SCROLL_INFO, vCmd[SELF_SERVERMSG_TEMPLAT].substr(4).c_str(), "1" );
			}
			return;
		}
		if(strHeader == "SCA:")	//横滚+聊天窗口+横滚颜色默认
		{
			if(CEventSystem::GetMe())
			{
				CEventSystem::GetMe()->PushEvent( GE_SET_SCROLL_INFO, vCmd[SELF_SERVERMSG_TEMPLAT].substr(4).c_str(), "0" );
			}

			ADD_SYSTEM_MSG( vCmd[SELF_SERVERMSG_TEMPLAT].substr(4) );

			return;
		}
		if(strHeader == "AUC:")	//横滚+聊天窗口+横滚颜色使用传过来的值
		{
			if(CEventSystem::GetMe())
			{
				CEventSystem::GetMe()->PushEvent( GE_SET_SCROLL_INFO, vCmd[SELF_SERVERMSG_TEMPLAT].substr(4).c_str(), "1" );
			}

			ADD_SYSTEM_MSG( vCmd[SELF_SERVERMSG_TEMPLAT].substr(4) );

			return;
		}

		switch(paramNum) {
		case 0:
			strTmp = COLORMSGFUNC(vCmd[SELF_SERVERMSG_TEMPLAT].c_str());
			break;
		case 1:
			strTmp = COLORMSGFUNC(vCmd[SELF_SERVERMSG_TEMPLAT].c_str(), vCmd[P1].c_str());
			break;
		case 2:
			strTmp = COLORMSGFUNC(vCmd[SELF_SERVERMSG_TEMPLAT].c_str(), vCmd[P1].c_str(), vCmd[P2].c_str());
			break;
		case 3:
			strTmp = COLORMSGFUNC(vCmd[SELF_SERVERMSG_TEMPLAT].c_str(), vCmd[P1].c_str(), vCmd[P2].c_str(),
				vCmd[P3].c_str());
			break;
		case 4:
			strTmp = COLORMSGFUNC(vCmd[SELF_SERVERMSG_TEMPLAT].c_str(), vCmd[P1].c_str(), vCmd[P2].c_str(),
				vCmd[P3].c_str(),vCmd[P4].c_str());
			break;
		case 5:
			strTmp = COLORMSGFUNC(vCmd[SELF_SERVERMSG_TEMPLAT].c_str(), vCmd[P1].c_str(), vCmd[P2].c_str(),
				vCmd[P3].c_str(),vCmd[P4].c_str(),vCmd[P5].c_str());
			break;
		case 6:
			strTmp = COLORMSGFUNC(vCmd[SELF_SERVERMSG_TEMPLAT].c_str(), vCmd[P1].c_str(), vCmd[P2].c_str(),
				vCmd[P3].c_str(),vCmd[P4].c_str(),vCmd[P5].c_str(),vCmd[P6].c_str());
			break;
		case 7:
			strTmp = COLORMSGFUNC(vCmd[SELF_SERVERMSG_TEMPLAT].c_str(), vCmd[P1].c_str(), vCmd[P2].c_str(),
				vCmd[P3].c_str(),vCmd[P4].c_str(),vCmd[P5].c_str(),vCmd[P6].c_str(),vCmd[P7].c_str());
			break;
		case 8:
			strTmp = COLORMSGFUNC(vCmd[SELF_SERVERMSG_TEMPLAT].c_str(), vCmd[P1].c_str(), vCmd[P2].c_str(),
				vCmd[P3].c_str(),vCmd[P4].c_str(),vCmd[P5].c_str(),vCmd[P6].c_str(),vCmd[P7].c_str(),vCmd[P8].c_str());
			break;
		case 9:
			strTmp = COLORMSGFUNC(vCmd[SELF_SERVERMSG_TEMPLAT].c_str(), vCmd[P1].c_str(), vCmd[P2].c_str(),
				vCmd[P3].c_str(),vCmd[P4].c_str(),vCmd[P5].c_str(),vCmd[P6].c_str(),vCmd[P7].c_str(),vCmd[P8].c_str(),
				vCmd[P9].c_str());
			break;
		case 10:
			strTmp = COLORMSGFUNC(vCmd[SELF_SERVERMSG_TEMPLAT].c_str(), vCmd[P1].c_str(), vCmd[P2].c_str(),
				vCmd[P3].c_str(),vCmd[P4].c_str(),vCmd[P5].c_str(),vCmd[P6].c_str(),vCmd[P7].c_str(),vCmd[P8].c_str(),
				vCmd[P9].c_str(),vCmd[P10].c_str());
			break;
		default:
			break;
		}

		if(strTmp.empty()) return;
		if(strHeader == "DBG:")
		{
			ADDNEWDEBUGMSG(strTmp);
			return;
		}

		switch(channelType) {
		case CHAT_TYPE_SYSTEM:
			{
				ADD_SYSTEM_MSG(strTmp);
			}
			break;
		case CHAT_TYPE_SELF:
			{
				ADDTALKMSG(strTmp);
			}
			break;
		default:
			break;
		}
	}

	INT Talk::HandleChannelResultPacket(GCChannelResult *pPacket)
	{
		if( NULL == pPacket)
		{
			KLAssert(FALSE);
			return -1;
		}

		switch(pPacket->GetReturn())
		{
		case CHANNEL_RESULT_CREATE:
			{
			}
			break;
		case CHANNEL_RESULT_ADDMEMBER:
		{
			}
			break;
		case CHANNEL_RESULT_DISMISS:
			{
			}
				break;
		case CHANNEL_RESULT_KICKMEMBER:
			{
			}
			break;
		default:
			break;
		}

		return 0;
	}
	
	INT Talk::HandleChannelErrorPacket(GCChannelError *pPacket)
	{
		if( NULL == pPacket)
		{
			KLAssert(FALSE);
			return -1;
		}

		switch(pPacket->GetErrorCode())
		{
		case CHANNEL_ERROR_HASCHANNEL:
			{
			}
				break;
		case CHANNEL_ERROR_CHANNELFULL:
			{
			}
			break;
		case CHANNEL_ERROR_NOCHANNEL:
			{
			}
			break;
		case CHANNEL_ERROR_CHANNELMEMBERFULL:
			{
			}
			break;
		case CHANNEL_ERROR_MEMBEREXIST:
			{
			}
			break;
		case CHANNEL_ERROR_NOTINCHANNEL:
			{
			}
			break;
		default:
			break;
		}

		return 0;
	}

	STRING Talk::ChannelType2String(ENUM_CHAT_TYPE type)
	{
		STRING strType;

		//switch(type)
		//{
		//case CHAT_TYPE_NORMAL:
		//	strType = NAMETYPE_TALK_NEAR;
		//	break;

		//case CHAT_TYPE_PRIVATE:
		//	strType = NAMETYPE_TALK_PRIVATE;
		//	break;

		//case CHAT_TYPE_FAMILY:
		//	strType = NAMETYPE_TALK_FAMILY;
		//	break;

		//case CHAT_TYPE_TEAM:
		//	strType = NAMETYPE_TALK_TEAM;
		//	break;

		//case CHAT_TYPE_GUILD:
		//	strType = NAMETYPE_TALK_GUILD;
		//	break;

		//case CHAT_TYPE_NATION:
		//	strType = NAMETYPE_TALK_NATION;
		//	break;

		//case CHAT_TYPE_WORLD:
		//	strType = NAMETYPE_TALK_WORLD;
		//	break;	
		//
		//case CHAT_TYPE_SYSTEM:
		//	strType = NAMETYPE_TALK_SYSTEM;
		//	break;

		//default:
		//	strType = "";
		//	break;
		//}
		switch(type)
		{
		case CHAT_TYPE_NORMAL:
			strType = NAMETYPE_TALK_NEAR;
			break;
		case CHAT_TYPE_SCENE:
			strType = NAMETYPE_TALK_SCENE;
			break;
		case CHAT_TYPE_CHANNEL:
			strType = NAMETYPE_TALK_PRIVATE;
			break;
		case CHAT_TYPE_SYSTEM:
			strType = NAMETYPE_TALK_SYSTEM;
			break;
		case CHAT_TYPE_GUILD:
			strType = NAMETYPE_TALK_GUILD;
			break;
		case CHAT_TYPE_SELF:
			strType = NAMETYPE_TALK_SELF;
			break;
		case CHAT_TYPE_TEAM:
			strType = NAMETYPE_TALK_TEAM;
			break;
		case CHAT_TYPE_MENPAI:
			strType = NAMETYPE_TALK_MENPAI;
			break;
		case CHAT_TYPE_WORLD:
			strType = NAMETYPE_TALK_WORLD;
			break;
		case CHAT_TYPE_COUNTRY:
			strType = NAMETYPE_TALK_COUNTRY;
			break;
		case CHAT_TYPE_USER:
			strType = NAMETYPE_TALK_PRIVATE;
			break;
		default:
			strType = "";
			break;
		}		
		return strType;
	}

	ENUM_CHAT_TYPE Talk::String2ChannelType(STRING strType)
	{
		ENUM_CHAT_TYPE type = CHAT_TYPE_INVALID;
		
		//if(strType == NAMETYPE_TALK_NEAR)
		//	type = CHAT_TYPE_NORMAL;

		//else if(strType == NAMETYPE_TALK_PRIVATE)
		//	type = CHAT_TYPE_PRIVATE;

		//else if(strType == NAMETYPE_TALK_FAMILY)
		//	type = CHAT_TYPE_FAMILY;

		//else if(strType == NAMETYPE_TALK_TEAM)
		//	type = CHAT_TYPE_TEAM;

		//else if(strType == NAMETYPE_TALK_GUILD)
		//	type = CHAT_TYPE_GUILD;

		//else if(strType == NAMETYPE_TALK_NATION)
		//	type = CHAT_TYPE_NATION;

		//else if(strType == NAMETYPE_TALK_WORLD)
		//	type = CHAT_TYPE_WORLD;

		//else if(strType == NAMETYPE_TALK_SYSTEM)
		//	type = CHAT_TYPE_SYSTEM;
		if(strType == NAMETYPE_TALK_NEAR)
			type = CHAT_TYPE_NORMAL;
		else if(strType == NAMETYPE_TALK_SCENE)
			type = CHAT_TYPE_SCENE;
		else if(strType == NAMETYPE_TALK_TEAM)
			type = CHAT_TYPE_TEAM;					//??? TEAM, GUILD
		else if(strType == NAMETYPE_TALK_GUILD)
			type = CHAT_TYPE_GUILD;
		//else if(strType == NAMETYPE_TALK_USER)
		//	type = CHAT_TYPE_TELL;
		else if(strType == NAMETYPE_TALK_PRIVATE)
			type = CHAT_TYPE_TELL;
		else if(strType == NAMETYPE_TALK_MENPAI)
			type = CHAT_TYPE_MENPAI;
		else if(strType == NAMETYPE_TALK_SELF)
			type = CHAT_TYPE_SELF;
		else if(strType == NAMETYPE_TALK_SYSTEM)
			type = CHAT_TYPE_SYSTEM;
		else if(strType == NAMETYPE_TALK_WORLD)
			type = CHAT_TYPE_WORLD;
		else if(strType == NAMETYPE_TALK_COUNTRY)
			type = CHAT_TYPE_COUNTRY;
		return type;
	}

	VOID Talk::UserCreate()
	{
		if(NULL == m_UsrCh)
		{
			m_UsrCh = new Talk::UserChannel();
			m_UsrCh->LoadChannelConfig();
			//m_listChannel.push_back(m_UsrCh);
		}
	}

	VOID Talk::TeamCreate(TeamID_t id)
	{
		if(id < 0)
		{
			KLThrow("ERROR found in Talk::TeamCreate");
		}

		if(NULL != m_TeamCh)
			return;

		m_TeamCh = new TeamChannel;
		m_TeamCh->SetTeamID(id);
		m_TeamCh->LoadChannelConfig();

		m_listChannel.push_back(m_TeamCh);
	}

	VOID Talk::TeamDestory()
	{
		if(NULL == m_TeamCh)
			return;

		std::list<Channel*>::iterator it;
		for(it = m_listChannel.begin(); it != m_listChannel.end(); ++it)
		{
			if(*it == m_TeamCh)
			{
				m_listChannel.erase(it);
				break;
			}
		}

		SAFE_DELETE(m_TeamCh);

		if(CGameProcedure::s_pEventSystem)
		{
            CGameProcedure::s_pEventSystem->PushEvent(GE_CHAT_CHANNEL_CHANGED);
		}
	}

	VOID Talk::MenPaiJoin(MenPaiID_t id)
	{
		if(id < 0)
		{
			KLThrow("ERROR found in Talk::MenPaiJoin");
		}

		if(NULL != m_MenPaiCh)
			return;

		m_MenPaiCh = new MenPaiChannel;
		m_MenPaiCh->SetMenPaiID(id);
		m_MenPaiCh->LoadChannelConfig();

		m_listChannel.push_back(m_MenPaiCh);
	}

	VOID Talk::MenPaiLeave()
	{
		if(NULL == m_MenPaiCh)
			return;

		std::list<Channel*>::iterator it;
		for(it = m_listChannel.begin(); it != m_listChannel.end(); ++it)
		{
			if(*it == m_MenPaiCh)
			{
				m_listChannel.erase(it);
				break;
			}
		}

		SAFE_DELETE(m_MenPaiCh);

		if(CGameProcedure::s_pEventSystem)
		{
            CGameProcedure::s_pEventSystem->PushEvent(GE_CHAT_CHANNEL_CHANGED);
		}
	}

	VOID Talk::GuildJoin(GuildID_t id)
	{
		if(id < 0)
		{
			KLThrow("ERROR found in Talk::GuildJoin");
		}

		if(NULL != m_GuildCh)
			return;

		m_GuildCh = new GuildChannel;
		m_GuildCh->SetGuildID(id);
		m_GuildCh->LoadChannelConfig();

		m_listChannel.push_back(m_GuildCh);
	}

	VOID Talk::GuildLeave()
	{
		if(NULL == m_GuildCh)
			return;

		std::list<Channel*>::iterator it;
		for(it = m_listChannel.begin(); it != m_listChannel.end(); ++it)
		{
			if(*it == m_GuildCh)
			{
				m_listChannel.erase(it);
				break;
			}
		}

		SAFE_DELETE(m_GuildCh);

		if(CGameProcedure::s_pEventSystem)
		{
            CGameProcedure::s_pEventSystem->PushEvent(GE_CHAT_CHANNEL_CHANGED);
		}
	}

	ENUM_CHAT_TYPE Talk::GetChatTypeFromMessage(STRING &strMsg)
	{
		STRING strHead = strMsg.substr(0,MESSAGE_HEAD_LENGTH);
		ENUM_CHAT_TYPE retType = CHAT_TYPE_INVALID;

		//if(strHead == MESSAGETYPE_TALK_NEAR)
		//	retType = CHAT_TYPE_NORMAL;

		//else if(strHead == MESSAGETYPE_TALK_PRIVATE)
		//	retType = CHAT_TYPE_PRIVATE;

		//else if(strHead == MESSAGETYPE_TALK_FAMILY)
		//	retType = CHAT_TYPE_FAMILY;
		//
		//else if(strHead == MESSAGETYPE_TALK_TEAM)
		//	retType = CHAT_TYPE_TEAM;

		//else if(strHead == MESSAGETYPE_TALK_GUILD)
		//	retType = CHAT_TYPE_GUILD;

		//else if(strHead == MESSAGETYPE_TALK_NATION)
		//	retType = CHAT_TYPE_NATION;

		//else if(strHead == MESSAGETYPE_TALK_WORLD)
		//	retType = CHAT_TYPE_WORLD;	

		//else if(strHead == MESSAGETYPE_TALK_SYSTEM)
		//	retType = CHAT_TYPE_SYSTEM;		

		//else if(strHead.size() < MESSAGE_HEAD_LENGTH) // 不足MESSAGE_HEAD_LENGTH个字符的消息，当作普通消息
		//	retType = CHAT_TYPE_INVALID;
		if(strHead == MESSAGETYPE_TALK_NEAR_L || strHead == MESSAGETYPE_TALK_NEAR_U)
			retType = CHAT_TYPE_NORMAL;
		else if(strHead == MESSAGETYPE_TALK_WORD_L || strHead == MESSAGETYPE_TALK_WORD_U)
			retType = CHAT_TYPE_WORLD;
		else if(strHead == MESSAGETYPE_TALK_TEAM_L || strHead == MESSAGETYPE_TALK_TEAM_U)
			retType = CHAT_TYPE_TEAM;
		else if(strHead == MESSAGETYPE_TALK_GUILD_L || strHead == MESSAGETYPE_TALK_GUILD_U)
			retType = CHAT_TYPE_GUILD;
		else if(strHead == MESSAGETYPE_TALK_MENPAI_L || strHead == MESSAGETYPE_TALK_MENPAI_U)
			retType = CHAT_TYPE_MENPAI;						
		else if(strHead == MESSAGETYPE_TALK_PRIVATE_L || strHead == MESSAGETYPE_TALK_PRIVATE_U)
			retType = CHAT_TYPE_TELL;
		else if(strHead.size() < MESSAGE_HEAD_LENGTH)		//不足MESSAGE_HEAD_LENGTH个字符的消息，当作普通消息
			retType = CHAT_TYPE_INVALID;
		return retType;
	}

	STRING Talk::GetTalkTemplateString(STRING& strMsg, STRING& strTalker)
	{
		STRING strKey, strResult, strTarget, strFinal = "";
		UINT idx = TALK_TEMPLATE_MAX;

		//const CHAR* pHyperB = "#aB{";
		//const CHAR* pHyperM = "}";
		//const CHAR* pHyperE = "#aE";

		//get talk template key & idx
		if(strMsg.find(" ") != STRING::npos)
		{
			strKey = strMsg.substr(1,strMsg.find(" ")-1);
			strTarget = strMsg.substr(strMsg.find(" ")+1);

			if(strTarget.size() > MAX_CHARACTER_NAME || strTarget.size() < 2) return strFinal; //角色名称太短或者太长

			if(strTarget == strTalker) //说话对象
				idx = TALK_TEMPLATE_MYSELF;
			else
				idx = TALK_TEMPLATE_OTHER;
		}
		else
		{
			strKey = strMsg.substr(1);
			idx = TALK_TEMPLATE_NOBODY;
		}

		if(strKey.empty()) return strFinal;

		//get talk tempalte string
		strResult = CGameProcedure::s_pUISystem->getTalkTemplate(strKey, idx);
		
		//generate real talker and target
		// $N --- TalkerName  $n --- TargetName
		STRING strRealTalker, strRealTarget;
		if(strResult.find("$N") != STRING::npos)
		{
			//说话者和客户端角色名相同
			if(strTalker == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name())
			{
				//strRealTalker = "#B你#W";
				STRING strTemp = "";
				strTemp = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_You");
				strRealTalker = strTemp;
				//strRealTalker = "你";
			}
			else
			{
				//strRealTalker = pHyperB + strTalker + pHyperM + strTalker + pHyperE;
				strRealTalker = "#R" + strTalker + "#W";
			}
		}
		else
			return strFinal;

		if(idx == TALK_TEMPLATE_OTHER && strResult.find("$n") != STRING::npos)
		{
			//目标者和客户端角色名相同
			if(strTarget == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name())
			{
				//strRealTarget = "#B你#W";
				STRING strTemp = "";
				strTemp = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_You");
				strRealTarget = strTemp;
				//strRealTarget = "你";
			}
			else
			{
				//strRealTarget = pHyperB + strTarget + pHyperM + strTarget + pHyperE;
				strRealTarget = "#R" + strTarget + "#W";
			}
		}
		else if(idx == TALK_TEMPLATE_OTHER && strResult.find("$n") == STRING::npos)
		{
			return strFinal;
		}

		//generate final template string.
		strFinal = CGameProcedure::s_pUISystem->ModifyChatTemplate(strResult, strRealTalker, strRealTarget);

		return strFinal;
	}

	STRING Talk::GetTalkActString(STRING& strMsg, STRING& strTalker)
	{
		STRING strKey, strResult, strTarget, strFinal = "";
		UINT idx = TALK_ACT_NUMBER;

		const CHAR* pHyperB = "#aB{";
		const CHAR* pHyperM = "}";
		const CHAR* pHyperE = "#aE";

		//get talk act key & idx
		if(strMsg.find(" ") != STRING::npos)
		{
			strKey = strMsg.substr(1,strMsg.find(" ")-1);
			strTarget = strMsg.substr(strMsg.find(" ")+1);

			if(strTarget.size() > MAX_CHARACTER_NAME || strTarget.size() < 2) return strFinal; //角色名称太短或者太长

			if(strTarget == strTalker) //说话对象
				idx = TALK_ACT_MYSELF;
			else
				idx = TALK_ACT_OTHER;
		}
		else
		{
			strKey = strMsg.substr(1);
			idx = TALK_ACT_NOBODY;
		}

		if(strKey.empty()) return strFinal;

		//get talk tempalte string
		strResult = s_Talk.GetTalkActMsg(strKey.c_str(), idx);

		//generate real talker and target
		// $N --- TalkerName  $n --- TargetName
		STRING strRealTalker, strRealTarget;
		if(strResult.find("$N") != STRING::npos)
		{
			//说话者和客户端角色名相同
			if(strTalker == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name())
			{
				//strRealTalker = "#B你#W";
				STRING strTemp = "";
				strTemp = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_You");
				strRealTalker = strTemp;
				//strRealTalker = "你";
			}
			else
			{
				//strRealTalker = pHyperB + strTalker + pHyperM + strTalker + pHyperE;
				strRealTalker = "#R" + strTalker + "#W";
			}
		}
		else
			return strFinal;

		if(idx == TALK_ACT_OTHER && strResult.find("$n") != STRING::npos)
		{
			//目标者和客户端角色名相同
			if(strTarget == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name())
			{
				//strRealTarget = "#B你#W";
				STRING strTemp = "";
				strTemp = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_You");
				strRealTarget = strTemp;
				//strRealTarget = "你";
			}
			else
			{
				//strRealTarget = pHyperB + strTarget + pHyperM + strTarget + pHyperE;
				strRealTarget = "#R" + strTarget + "#W";
			}
		}
		else if(idx == TALK_ACT_OTHER && strResult.find("$n") == STRING::npos)
		{
			strRealTarget = "#R" + strTarget + "#W";
		}

		//generate final template string.
		strFinal = CGameProcedure::s_pUISystem->ModifyChatTemplate(strResult, strRealTalker, strRealTarget);
		return strFinal;
	}

	VOID Talk::SetPaoPaoTxt(STRING& strUser, STRING& strTxt, INT strType, INT extLength)
	{
		CObject_Character* pObj = CObjectManager::GetMe()->FindCharacterByName(strUser);
		if(pObj && pObj->GetInfoBoard()->isShow())
		{
			pObj->Say(TalkFixTail(strTxt));
		}
	}

	VOID Talk::SetMonsterPaoPaoText(INT idServer, INT idMsg)
	{
		CObject_Character* pObj = (CObject_Character*)CObjectManager::GetMe()->FindServerObject(idServer);
		if(pObj && !g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
		{
			pObj = NULL;
		}

		if(pObj && pObj->GetInfoBoard()->isShow())
		{
			const tDataBase* pDB = CDataBaseSystem::GetMe()->GetDataBase(DBC_MONSTER_PAOPAO);
			if(pDB)
			{
				const _DBC_MONSTER_PAOPAO* pLine = (const _DBC_MONSTER_PAOPAO*)pDB->Search_Index_EQU(idMsg);
				if(pLine)
				{
					pObj->Say(STRING(pLine->szPaoPaoTxt));
				}
			}
		}
	}

	VOID Talk::AddPingBi(STRING& strUser)
	{
		if(!IsInPingBi(strUser))
		{
			m_PingBiQue.push_back(strUser);
			if((INT)m_PingBiQue.size() > CHAT_PINGBI_MAXUSER_NUMBERS) m_PingBiQue.pop_front();

			if(CEventSystem::GetMe())
			{
				CEventSystem::GetMe()->PushEvent(GE_CHAT_PINGBI_UPDATE);
			}
		}
	}

	VOID Talk::DelPingBi(STRING& strUser)
	{
		if(m_PingBiQue.empty()) return;

		std::deque<STRING>::iterator it;
		for(it = m_PingBiQue.begin(); it!= m_PingBiQue.end(); it++)
		{
			if(*it == strUser) break;
		}

		if(it != m_PingBiQue.end())
		{
			m_PingBiQue.erase(it);
		}
	}

	BOOL Talk::IsInPingBi(STRING& strUser)
	{
		if(m_PingBiQue.empty()) return FALSE;

		std::deque<STRING>::iterator it;
		for(it = m_PingBiQue.begin(); it!= m_PingBiQue.end(); it++)
		{
			if(*it == strUser) break;
		}

		return (it == m_PingBiQue.end())?FALSE:TRUE;
	}

	VOID Talk::ClearPingBi()
	{
		m_PingBiQue.erase(m_PingBiQue.begin(), m_PingBiQue.end());
	}

	CHAR* Talk::GetColorByChannelType(INT nType, BOOL bHeader)
	{
		CHAR *pRet = "#W";

		typedef struct {
			CHAR* pContentColor;		//频道内容颜色
			CHAR* pNameColor;			//频道名称颜色
		}CH_C;

		static CH_C s_Color[] = {
			{"#cFFFFFF", "#e010101#cFFFFFF"},	//普通说话消息
			//{"#cFFFFFF", "#e010101#cCC99FF"},	//队聊消息
			{"#c0000FF", "#e010101#c00009F"},	//队聊消息
			{"#cFFFF00", "#e010101#cFFFF00"},	//场景消息
			{"#cFF7C80", "#e010101#cFF7C80"},	//私聊消息
			{"#cFF0000", "#e010101#cFF0000"},	//系统消息
			{"", ""},							//自建聊天频道消息
			//{"#cFFFFFF", "#e010101#cFFCC99"},	//帮派消息
			{"#c00FF00", "#e010101#c00FF00"},	//帮派消息
			{"#cFFFFFF", "#e010101#cFFFF00"},	//门派消息
			{"#e010101#cFFFFFF", ""},			//仅客户端使用的消息
			{"#e010101#cFFFFFF", ""},			//帮助消息
			{"#W", "#e010101#cffff00"},	//喇叭消息
			{"#cFFFFFF", "#e010101#cff9900"},	//同乡消息
		};

		if(nType >= 0 && nType < CHAT_TYPE_NUMBER)
		{
			pRet = (bHeader)?s_Color[nType].pNameColor:s_Color[nType].pContentColor;
		}

		return pRet;
	}

	VOID Talk::ShowTalkHelpMsg()
	{
		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level() < 10)
		{
			if(!(CGameProcedure::s_pUISystem->getTalkRandHelpMsg().empty()))
			{
				STRING strTemp = "";
				strTemp = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_strHelp");
				STRING strHelp = strTemp;
				//STRING strHelp = "[#G帮助#W]";
				strHelp += CGameProcedure::s_pUISystem->getTalkRandHelpMsg();

				ADDTALKMSG(strHelp);
			}
		}
	}

	VOID Talk::TalkDisplaySizeChanged(INT width, INT height)
	{
		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_ADJUST_MOVE_CTL, width, height);
		}
	}

	STRING Talk::TalkFixTail(STRING &strIn)
	{
		STRING strOut = "";
		const BYTE byANSIBegin		= 0X20;
		const BYTE byANSIEnd		= 0X80;

		const BYTE by1GBKBegin	= 0X81;
		const BYTE by1GBKEnd	= 0XFE;

		const BYTE by2GBKBegin1	= 0X40;
		const BYTE by2GBKEnd1	= 0X7E;
		const BYTE by2GBKBegin2	= 0X80;
		const BYTE by2GBKEnd2	= 0XFE;

		const CHAR * const szERROR = "<ERROR>";

		//预先分配好内存
		INT nSourSize = (INT)strIn.size();
		strOut.reserve(nSourSize+1);
		strOut = "";

		for(INT i=0; i<nSourSize; )
		{
			BYTE byChar = strIn[i];

			//标准ANSI英文字符
			if(byChar >= byANSIBegin && byChar <= byANSIEnd || byChar == '\r' || byChar == '\n' || byChar == '\t')
			{
				strOut += byChar;
				i++; 
				continue;
			}
			//汉字第一个字节
			else if(byChar >= by1GBKBegin && byChar <= by1GBKEnd)
			{
				//半个汉字
				if(i+1 == nSourSize) goto CHECKSTRING_ERROR;

				//取出第二个字节
				BYTE byNext = strIn[++i];
				if(	!( byNext >= by2GBKBegin1 && byNext <= by2GBKEnd1 || byNext >= by2GBKBegin2 && byNext <= by2GBKEnd2) )
				{
					//非法的汉字
					goto CHECKSTRING_ERROR;
				}

				//加入完整的汉字
				CHAR szTemp[8];
				szTemp[0] = byChar;
				szTemp[1] = byNext;
				szTemp[2] = 0;
				strOut += szTemp;

				i++;
				continue;
			}
			else goto CHECKSTRING_ERROR;
		}

CHECKSTRING_ERROR:

		return strOut;
	}

	INT	Talk::AddToSendHistoryQue(HistoryMsg& msg)
	{
		msg.AddInfoElement();
		m_SendHisQue.push_back(msg);

		if((INT)(m_SendHisQue.size()) > m_MaxSaveNum)
		{
			//删除旧有的InfoElement
			std::deque<HistoryMsg>::iterator it = m_SendHisQue.begin();
			(*it).DeleteInfoElement();

			m_SendHisQue.pop_front();
		}
		
		return 0;
	}

	INT	Talk::AddToSendHistoryText( HistoryMsg& msg )
	{
		std::deque<HistoryMsg>::iterator iter;
		for( iter=m_SendHisTextQue.begin(); iter!=m_SendHisTextQue.end(); ++iter )
		{
			if ( msg.GetMsgData() == iter->GetMsgData() )
				return 0;
		}
		m_SendHisTextQue.push_back( msg );

		if ( m_SendHisTextQue.size() > 10 )
		{
			m_SendHisTextQue.pop_front();
		}

		m_PrevSendPos = -1;
		return 0;
	}

	INT	Talk::AddToRecvHistoryQue(HistoryMsg& msg)
	{
		std::vector< STRING > strParam;
		STRING strType = ChannelType2String(msg.GetChannelType());
		char tmpstr[16] = {0};
		_snprintf(tmpstr, 15, "%d", msg.GetID());
		STRING strID = tmpstr;

		strParam.clear();
		strParam.push_back(strType);
		strParam.push_back(msg.GetMsgName());
		strParam.push_back(msg.GetMsgData());
		strParam.push_back(strID);
		AddChatMessageToUI(strParam,1);

		msg.AddInfoElement();
		m_RecvHisQue.push_back(msg);

		if((INT)(m_RecvHisQue.size()) > m_MaxSaveNum)
		{
			//删除旧有的InfoElement
			std::deque<HistoryMsg>::iterator it = m_RecvHisQue.begin();
			(*it).DeleteInfoElement();

			m_RecvHisQue.pop_front();
		}

		return 0;
	}

	INT	Talk::AddToGMCmdHistoryQue(HistoryMsg& msg)
	{
		msg.AddInfoElement();
		m_GMCmdHisQue.push_back(msg);

		if((INT)(m_GMCmdHisQue.size()) > m_MaxSaveNum)
		{
			//删除旧有的InfoElement
			std::deque<HistoryMsg>::iterator it = m_GMCmdHisQue.begin();
			(*it).DeleteInfoElement();

			m_GMCmdHisQue.pop_front();
		}

		m_PrevGMCmdPos = -1;
		return 0;
	}

	bool	Talk::IsSendSucce(STRING data)
	{
		UINT nowTime = CGameProcedure::s_pTimeSystem->GetTimeNow();
		while ( m_SendTimeHisQue.begin() != m_SendTimeHisQue.end() )
		{
			std::deque<HistoryMsg>::iterator iter = m_SendTimeHisQue.begin();
			if ( nowTime - iter->GetMsgTime() > 30*1000 )
			{
				m_SendTimeHisQue.pop_front();
			}
			else
			{
				break;
			}
		}

		for (std::deque<HistoryMsg>::iterator iter = m_SendTimeHisQue.begin(); 
			iter != m_SendTimeHisQue.end(); ++iter )
		{
			if ( iter->GetMsgData() == data )
				return false;
		}
		return true;
	}

	VOID Talk::AddUserToUserChannel(STRING &strUser)
	{
		if(strUser.empty()) return;

		if(NULL == m_UsrCh)
		{
			m_UsrCh = new Talk::UserChannel();
			m_UsrCh->LoadChannelConfig();
			//m_listChannel.push_back(m_UsrCh);
		}

		m_UsrCh->AddUser(strUser);
	}

	VOID Talk::SetTalkRule()
	{
		INT usrLv = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level();
		const tDataBase* pTalkConfig = CDataBaseSystem::GetMe()->GetDataBase(DBC_TALK_CONFIG);

		//当前频道聊天信息发送间隔
		std::list<Talk::Channel *>::iterator it = m_listChannel.begin();
		for(; it != m_listChannel.end(); it++)
		{
			//频道是否开启的等级检查
			if(pTalkConfig)
			{
				const _DBC_TALK_CONFIG* pLine = (const _DBC_TALK_CONFIG*)pTalkConfig->Search_LineNum_EQU((*it)->GetType());
				if(pLine)
				{
					if( usrLv >= pLine->nNeedLv)
					{
						(*it)->Open();
					}
					else
					{
						(*it)->Close();
					}
				}
			}//end if(pTalkConfig)

		}// end for

		//泡泡文字限制
		m_bPaoPaoRule = (usrLv < 10)?TRUE:FALSE;
		//KLU_Log("m_bPaoPaoRule : %d", (INT)m_bPaoPaoRule);
	}

	VOID Talk::LoadTabSetting(GCRetSetting* pk)
	{
		if(!pk) return;
		KLAssert(5 == sizeof(_OWN_SETTING));

		INT tabId = 1;
		for(INT i=SETTING_TYPE_CHAT_TAB1_PART1; i < SETTING_TYPE_CHAT_TAB4_PART2; i += 2, ++tabId)
		{
			_OWN_SETTING* pSetting1 = &(pk->GetSetting()[i]);
			_OWN_SETTING* pSetting2 = &(pk->GetSetting()[i+1]);
			if(!pSetting1 || !pSetting2) break;

			if(pSetting1->m_SettingType == 0 && pSetting1->m_SettingData == 0) break;

			CHAR szTabName[6+1] = {0};
			memcpy(szTabName, pSetting1, sizeof(_OWN_SETTING));
			memcpy(szTabName+sizeof(_OWN_SETTING), &(pSetting2->m_SettingType), sizeof(BYTE));

			STRING strConfig;
			strConfig.erase();

			for(UINT i=0; i< CHAT_TYPE_NUMBER; ++i)
			{
				(pSetting2->m_SettingData & 1<<i)?strConfig += "1":strConfig += "0";
			}

			//OK,通知聊天窗口创建出Tab页来
			CEventSystem::GetMe()->PushEvent(GE_CHAT_LOAD_TAB_CONFIG, szTabName, strConfig.c_str(), tabId);
		}
	}

	bool Talk::GetMsgFromCurTabHistoryQue(HistoryMsg& msg,int id)
	{
		std::deque<HistoryMsg>::iterator it = m_RecvHisQue.begin();
		for(;it!=m_RecvHisQue.end();it++)
		{
			//if((*it).GetID() == id)
			//{

				if( (*it).GetChannelType()!=CHAT_TYPE_SYSTEM	&&
					(*it).GetChannelType()!=CHAT_TYPE_SELF  && 
					id!=-1)
				{
					msg = (*it);
					return true;
				}

			//}
		}
		return false;
	}

	VOID Talk::HandleKey_Shift_ArrowUp(INT type)
	{
		switch(type)
		{
		case 0:	//normal send his
			if(m_SendHisTextQue.empty()) return;

			//caculate send pos.
			if(m_PrevSendPos >= 0)
			{
				m_PrevSendPos = (INT)((m_PrevSendPos - 1 < 0)?(m_SendHisTextQue.size()-1):m_PrevSendPos-1);
				m_SendHisTextQue[m_PrevSendPos].AddInfoElement();
			}
			else
			{
				m_PrevSendPos = (INT)(m_SendHisTextQue.size()-1);
				m_SendHisTextQue[m_PrevSendPos].AddInfoElement();
			}

			//push accelerate key event.
			if(CEventSystem::GetMe())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "save_old");

				std::vector< STRING > strParam;
				strParam.push_back("shift_up");
				strParam.push_back(m_SendHisTextQue[m_PrevSendPos].GetMsgData());
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, strParam);
			}

			//AxTrace(0,0,"m_PrevSendPos:%d", m_PrevSendPos);
			break;
		case 1: //gm cmd send his
			if(m_GMCmdHisQue.empty()) return;

			//caculate send pos.
			if(m_PrevGMCmdPos >= 0)
			{
				m_PrevGMCmdPos = (INT)((m_PrevGMCmdPos - 1 < 0)?(m_GMCmdHisQue.size()-1):m_PrevGMCmdPos-1);
				m_GMCmdHisQue[m_PrevGMCmdPos].AddInfoElement();
			}
			else
			{
				m_PrevGMCmdPos = (INT)(m_GMCmdHisQue.size()-1);
				m_GMCmdHisQue[m_PrevGMCmdPos].AddInfoElement();
			}

			//push accelerate key event.
			if(CEventSystem::GetMe())
			{
				//CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "save_old");

				std::vector< STRING > strParam;
				strParam.push_back("shift_up");
				strParam.push_back(m_GMCmdHisQue[m_PrevGMCmdPos].GetMsgData());
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, strParam);
			}
			break;
		default:
			break;
		}
	}

	VOID Talk::HandleKey_Shift_ArrowDown(INT type)
	{
		switch(type)
		{
		case 0: //normal send his
			if(m_SendHisTextQue.empty()) return;

			//caculate send pos.
			if(m_PrevSendPos >=0)
			{
				m_PrevSendPos = (INT)((m_PrevSendPos+1 == m_SendHisTextQue.size())?0:m_PrevSendPos+1);
				m_SendHisTextQue[m_PrevSendPos].AddInfoElement();
			}
			else
			{
				m_PrevSendPos = 0;
				m_SendHisTextQue[m_PrevSendPos].AddInfoElement();
			}

			//push accelerate key event.
			if(CEventSystem::GetMe())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "save_old");

				std::vector< STRING > strParam;
				strParam.push_back("shift_down");
				strParam.push_back(m_SendHisTextQue[m_PrevSendPos].GetMsgData());
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, strParam);
			}

			//AxTrace(0,0,"m_PrevSendPos:%d", m_PrevSendPos);
			break;
		case 1: //gm cmd send his
			if(m_GMCmdHisQue.empty()) return;

			//caculate send pos.
			if(m_PrevGMCmdPos >=0)
			{
				m_PrevGMCmdPos = (INT)((m_PrevGMCmdPos+1 == m_GMCmdHisQue.size())?0:m_PrevGMCmdPos+1);
				m_GMCmdHisQue[m_PrevGMCmdPos].AddInfoElement();
			}
			else
			{
				m_PrevGMCmdPos = 0;
				m_GMCmdHisQue[m_PrevGMCmdPos].AddInfoElement();
			}

			//push accelerate key event.
			if(CEventSystem::GetMe())
			{
				//CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "save_old");

				std::vector< STRING > strParam;
				strParam.push_back("shift_down");
				strParam.push_back(m_GMCmdHisQue[m_PrevGMCmdPos].GetMsgData());
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, strParam);
			}
			break;
		default:
			break;
		}
	}

	VOID Talk::HandleKey_Ctrl_ArrowUp()
	{
		if( m_UsrCh == NULL )return;
		STRING data("/S ");
		data += m_UsrCh->SelectNextUser( false ) ;
		if( CEventSystem::GetMe() )
		{
			std::vector<STRING> strParam;
			strParam.push_back( "ctrl_up" );
			strParam.push_back( data );
			CEventSystem::GetMe()->PushEvent( GE_ACCELERATE_KEYSEND, strParam );
		}
		
	}

	VOID Talk::HandleKey_Ctrl_ArrowDown()
	{
		if( m_UsrCh == NULL ) return;
		STRING data( "/S " );
		data += m_UsrCh->SelectNextUser( true ) ;
		if ( CEventSystem::GetMe() )
		{
			std::vector<STRING> strParam;
			strParam.push_back( "ctrl_down" );
			strParam.push_back( data );
			CEventSystem::GetMe()->PushEvent( GE_ACCELERATE_KEYSEND, strParam );
		}
	}

	VOID Talk::HandleKey_Alt_S()
	{
		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "send_msg");
		}
	}

	VOID Talk::HandleKey_Ctrl_Enter()
	{
		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "send_msg");
		}
	}

	VOID Talk::HandleKey_Alt_FaceMotion()
	{
		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_face");
		}
	}

	VOID Talk::HandleKey_PageUp()
	{
		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "page_up");
		}
	}

	VOID Talk::HandleKey_PageDown()
	{
		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "page_down");
		}
	}

	VOID Talk::SendGMCommand(LPCTSTR msg)
	{

		//保存命令历史
		TalkAnalyze taN( CHAT_TYPE_SELF, msg );
		taN.CheckItemElementInfo();

		HistoryMsg hmsg(CHAT_TYPE_SELF);
		hmsg.SetID();
		hmsg.SetMsgName(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name());
		hmsg.SetMsgData(STRING(msg));
		hmsg.SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
		hmsg.SetItemCollect( taN.GetItemCollect() );

		AddToGMCmdHistoryQue(hmsg);

		if ( ExecuteGMCommand(msg) )
		{
			return;
		}

		//发送GM指令
		CGameInterface::GetMe()->GM_SendCommand(msg+2);
	}

	BOOL	Talk::ExecuteGMCommand(LPCTSTR pCommand)		//执行客户端GM命令
	{
#define MAX_COMMAND_SIZE 256
		if (pCommand == NULL)
		{
			return FALSE;
		}
		CHAR szCommand[MAX_COMMAND_SIZE] ;
		memset( szCommand, 0, MAX_COMMAND_SIZE ) ;

		INT iSize = strlen(pCommand);

		if( iSize>=MAX_COMMAND_SIZE || iSize<0 )
			return COMMAND_TYPE_NONE ;

		INT iPos ;
		for( iPos=0; iPos<iSize; iPos++ )
		{
			if( pCommand[iPos]==' ' || pCommand[iPos]==0 || pCommand[iPos]=='\t' )
				break ;
			szCommand[iPos] = pCommand[iPos] ;
		}
		CHAR* pCom =  szCommand+2 ;

		if( tstricmp( pCom, "reloadscript")==0 )
		{
			const CHAR* pParam = pCommand+iPos ;

			const CHAR* pUIName = strchr( pParam, '=' ) ;
			if( pUIName==NULL ) return FALSE ;

			pUIName ++ ;
			
			if( false == CGameProcedure::s_pUISystem->ReloadWindowScript(pUIName) )
			{
				STRING szMsg = "找不到窗口,是否窗口名称写错了 窗口:";
				szMsg += pUIName;
				CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE, szMsg.c_str());
			}

			return TRUE;
		}

		if( tstricmp( pCom, "show")==0 )
		{
			const CHAR* pParam = pCommand+iPos ;

			//const CHAR* pShowOrHide = strchr( pParam, '=' ) ;
			//if( pShowOrHide==NULL ) 
			//{
			//	return FALSE ;
			//}

			//bool bShow = pShowOrHide[1] == '1';

			//pParam  = pShowOrHide + 1;

			const CHAR* pType = strchr(pParam, '=' ) ;
			if (pType == NULL)
			{
				return FALSE;
			}
			pType++;
			
			CGameProcedure::s_pGfxSystem->SetShowObjectByType( pType);//pType
	
			return TRUE;
		}
		return FALSE;
	}

	BOOL Talk::ParGMCharacter( STRING strTalkMsg )
	{
		LPCTSTR name1("!!help=show");
		LPCTSTR name2("!!help =show");
		LPCTSTR name3("!!help= show");
		LPCTSTR name4("!!help = show");
		if ( 
				0 == strcmp(strTalkMsg.c_str(), name1) ||
				0 == strcmp(strTalkMsg.c_str(), name2) ||
				0 == strcmp(strTalkMsg.c_str(), name3) ||
				0 == strcmp(strTalkMsg.c_str(), name4) 
			)
		{
			std::vector<STRING> vHelpShow;
			vHelpShow.push_back("show =lm----switch show logicmode");
			vHelpShow.push_back("show =se----switch show staticEntity");
			vHelpShow.push_back("show =sl----switch show terrainliquid");
			vHelpShow.push_back("show =ef----switch show effect");
			vHelpShow.push_back("show =ps----switch show particle system");
			vHelpShow.push_back("show =all---switch show all thing");

			std::vector<STRING>::iterator it = vHelpShow.begin();
			for( ; it != vHelpShow.end(); ++it )
			{
				ADDTALKMSG( (*it) );
			}
			
			return TRUE;
		}

		
		return FALSE;
	}

	VOID Talk::DoAct(LPCTSTR act, LPCTSTR user)
	{
		if(act && user)
		{
			CObject_Character* pObj = CObjectManager::GetMe()->FindCharacterByName(user);
			if(pObj)
			{
				STRING strOrder = GetTalkActOrder(act);
				//分拆动作命令
				if(strOrder != "besit")
				{
					pObj->SetChatMoodAction(strOrder);
				}
				else
				{
					//坐下需要单独处理
					pObj->SitDown();
				}
				//播放动作命令
				//pObj->PlayChatMoodAction();
			}
		}
	}

	VOID Talk::GenTalkActMap()
	{
		const tDataBase* pActDBC = CDataBaseSystem::GetMe()->GetDataBase(DBC_TALK_ACT);
		TALK_ACT_STRUCT	act;

		if(pActDBC)
		{
			INT num = pActDBC->GetRecordsNum();
			for(INT i=0; i < num; ++i)
			{
				act.m_actMsg.resize(0);
				act.m_actOrder = "";
				act.m_iconName.erase();
				act.m_tip.erase();

				const _DBC_TALK_ACT* pLine = (const _DBC_TALK_ACT*)pActDBC->Search_LineNum_EQU(i);

				TALK_ACTMAP::iterator it = m_TalkActMap.find(pLine->szCmd);
				//没找到的才往查找表中填加
				if(it == m_TalkActMap.end())
				{
					act.m_actMsg.push_back(pLine->szNobody);
					act.m_actMsg.push_back(pLine->szMyself);
					act.m_actMsg.push_back(pLine->szOther);

					act.m_actOrder = STRING(pLine->szAct);
					//第一行必须是坐下，因为坐下需要单独处理
					if(i == 0) act.m_actOrder = "besit";

					act.m_actIdx = (INT)m_TalkActMap.size();

					act.m_iconName = STRING(pLine->szIconName);
					act.m_tip = STRING(pLine->szToolTip);

					m_TalkActMap.insert(std::make_pair(pLine->szCmd, act));
				}
			}
		}// end of if(pActDBC)
	}

	VOID Talk::ChangeDefaultChannel(INT nType, const STRING& strName)
	{
		if(nType < 0 || nType >= CHAT_TYPE_NUMBER) return;

		if(nType == CHAT_TYPE_TELL)
		{
			if(!m_UsrCh) UserCreate();
			m_UsrCh->AddUser(strName);
		}

		if(CEventSystem::GetMe())
		{
			std::vector<STRING> vParam;
			vParam.push_back("infochannel");
			vParam.push_back(ChannelType2String((ENUM_CHAT_TYPE)nType));
			vParam.push_back(strName);

			//CEventSystem::GetMe()->PushEvent(GE_CHAT_MENUBAR_ACTION, vParam);
		}
	}

	STRING Talk::GetTalkActMsg(LPCTSTR act, INT idx)
	{
		STRING strRet = "";
		if(act && idx >= 0 && idx < TALK_ACT_NUMBER)
		{
			TALK_ACTMAP::iterator it = m_TalkActMap.find(act);
			if(it != m_TalkActMap.end())
			{
				TALK_ACT_STRUCT* pAct = &(it->second);
				if(pAct && idx < (INT)pAct->m_actMsg.size())
				{
					strRet = pAct->m_actMsg.at(idx);
				}
			}
		}
		return strRet;
	}

	STRING Talk::GetTalkActOrder(LPCTSTR act)
	{
		STRING strRet = "";
		if(act)
		{
			TALK_ACTMAP::iterator it = m_TalkActMap.find(act);
			if(it != m_TalkActMap.end())
			{
				TALK_ACT_STRUCT* pAct = &(it->second);
				if(pAct)
				{
					strRet = pAct->m_actOrder;
				}
			}
		}
		return strRet;
	}

	STRING Talk::FindTalkActKey(INT idx)
	{
		STRING strRet = "";
		
		if(idx >= 0  && idx < (INT)m_TalkActMap.size())
		{
			TALK_ACTMAP::iterator it = m_TalkActMap.begin();
			for(; it != m_TalkActMap.end(); it++)
			{
				TALK_ACT_STRUCT* pAct = &(it->second);

				if(idx == pAct->m_actIdx)
					strRet = it->first;
			}
		}

		return strRet;
	}

	TALK_ACT_STRUCT* Talk::GetTalkActStructByID(INT idx)
	{
		TALK_ACT_STRUCT* pAct = NULL;

		if(idx >= 0 && idx < (INT)m_TalkActMap.size())
		{
			TALK_ACTMAP::iterator it = m_TalkActMap.begin();
			for(; it != m_TalkActMap.end(); it++)
			{
				TALK_ACT_STRUCT* pActNow = &(it->second);
				if(idx == pActNow->m_actIdx)
					pAct = pActNow;
			}
		}

		return pAct;
	}

	//BOOL Talk::IsTabReceiveChannelMsg(INT tabIdx, INT channelType)
	//{
	//	//if(channelType == CHAT_TYPE_SPEAKER)
	//	//{
	//	//	//如果是小喇叭，规则按照系统频道处理，是在是没有办法
	//	//	//channelType = CHAT_TYPE_SYSTEM;  //这句话注掉，我就知道策划最终肯定会改的很恶心
	//	//	//现在改为只有系统分页显示
	//	//	if(tabIdx == 1)
	//	//	{
	//	//		return TRUE;
	//	//	}
	//	//	return FALSE;

	//	//}
	//	if(tabIdx<0||tabIdx>=CHAT_TAB_MAX_NUMBER) return FALSE;
	//	if(channelType<0||channelType>=CHAT_TYPE_NUMBER) return FALSE;

	//	return (m_TabCfg[tabIdx].icfg[channelType] > 0)?TRUE:FALSE;
	//}

	VOID Talk::AddChatMessageToUI(const std::vector< STRING >& vParam, INT nOp )
	{
		if(vParam.empty() && nOp == 2)
		{
			//刷新窗体
			if(CGameProcedure::s_pUISystem)
			{
				CGameProcedure::s_pUISystem->addChatHistoryMessage(-1,"Chat_Frame_History", "", nOp);
			}
			return;
		}

		KLAssert(vParam.size() == 4);
		STRING strmsgID = vParam.at(3);
		INT msgID = atoi(strmsgID.c_str());
		//聊天参数
		STRING content = vParam.at(2);
		if(vParam.at(2).empty()) return;

		INT type = String2ChannelType(vParam.at(0));
		if(CHAT_TYPE_INVALID == type) return;

		//不能在这里判断，因为不是过滤而是简单的添加
		//if(!IsTabReceiveChannelMsg(m_CurTab, type)) return;
		STRING msg;
		//////////////////////////////////////////////////////////////////////////
		//Npc频道
		if(vParam.at(2).substr(0,7) == "NPCTALK")
		{
			msg = vParam.at(2).substr(7);
			//添加信息到窗体
			if(CGameProcedure::s_pUISystem)
			{
				CGameProcedure::s_pUISystem->addChatHistoryMessage(msgID,"Chat_Frame_History", msg.c_str(), nOp);
			}
			return;
		}
		//////////////////////////////////////////////////////////////////////////
		//旧的说话逻辑

		//0.频道信息头
		STRING strName = vParam.at(1);
		STRING strHeader;
		if(strName.substr(0,1) == "*")
		{
			strName = strName.substr(1);
		}

		//if(/*type > 0 && */type != CHAT_TYPE_TELL)
		{
			//查表得知频道名称
			const tDataBase* pTalkConfig = CDataBaseSystem::GetMe()->GetDataBase(DBC_TALK_CONFIG);
			if(pTalkConfig)
			{
				const _DBC_TALK_CONFIG* pLine = (const _DBC_TALK_CONFIG*)pTalkConfig->Search_LineNum_EQU(type);
				if(pLine)
				{
					strHeader = pLine->szChannelHeader;
					strName = pLine->szChannelName;
				}
			}
		}

		//生成InfoItem
		if(type != CHAT_TYPE_SYSTEM && type != CHAT_TYPE_SELF /*&& type != CHAT_TYPE_HELP */ )
		{
			if(CTransferItemSystem::GetMe())
			{
				STRING straddname = vParam.at(1);
				CTransferItemSystem::Element* pEl = CTransferItemSystem::GetMe()->AddChannel(type, strName);

				CHAR id[64] = {0};
				_snprintf(id,63,"#{_INFOID%d}", pEl->nID);

				strHeader = id;
		
				// 如果是私聊消息，向脚本发送加为临时好友信息
				if ( type == CHAT_TYPE_USER )
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_ADD_TEMPFRIEND, straddname.c_str() );
				}

			}
		}

		if(vParam.at(1).empty() && type != CHAT_TYPE_SELF /*&& type != CHAT_TYPE_HELP*/)
		{
			msg = GetColorByChannelType(type, TRUE);
			msg += "[" + strHeader + "]"; // 例：格式为“[系统]”
			msg += GetColorByChannelType(type, FALSE) + vParam.at(2);
		}
		else
		{
			if(type != CHAT_TYPE_SELF /*&& type != CHAT_TYPE_HELP*/)
			{
				msg = GetColorByChannelType(type, TRUE);

				BOOL bNormal = FALSE;
				if(vParam.at(2).at(0) != '*' && vParam.at(2).at(0) != '@') bNormal = TRUE;
				if(vParam.at(2).size() >= 2 && vParam.at(2).at(0) == '*' && vParam.at(2).at(1) == '*') bNormal = TRUE;
				if(vParam.at(2).size() < 2 && vParam.at(2).at(0) == '*') bNormal = TRUE;
				//::OutputDebugString(vParam.at(2).c_str());
				//::OutputDebugString("\n");
				if(bNormal)
				{
					//msg += "[" + strHeader + "]";
					msg += strHeader;

					if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name() == vParam.at(1))
					{
						//msg += "#W[" + vParam.at(1) + "]";
						msg += GetColorByChannelType(type, FALSE);
						msg+= "[" + vParam.at(1) + "]";
					}
					else
					{
						BOOL bMySelf = FALSE;
						STRING strUser = vParam.at(1);
						STRING strHyperLink;
						if(strUser.at(0) == '*')
						{
							bMySelf = TRUE;
							strUser = strUser.substr(1);
						}

						if(CTransferItemSystem::GetMe())
						{
							CTransferItemSystem::Element* pEl = CTransferItemSystem::GetMe()->AddHyperLink(strUser, vParam.at(2), type);

							CHAR id[64] = {0};
							_snprintf(id,63,"#{_INFOID%d}", pEl->nID);

							//if(type == CHAT_TYPE_TELL)
							//{
							//	if(!bMySelf)
							//	{
							//		strHyperLink = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_Private_Other", id);
							//	}else
							//	{
							//		strHyperLink = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_Private_You", id);
							//	}
							//}
							//else
							{
								strHyperLink = id;
							}
						}

						//msg += "#c00CCFF" + strHyperLink;
						msg += GetColorByChannelType( type, FALSE) + strHyperLink;
					}
					
					msg += GetColorByChannelType(type, FALSE);
					//汉字全角引号"：" = "\xA3\xBA"
					msg += "\xA3\xBA" /*":"*/ + vParam.at(2);
				}
				else
				{
					LuaTalkAnalyze luN(CHAT_TYPE_INVALID, vParam.at(2));
					luN.doAnalyze(STRING(vParam.at(1)));

					if(!(luN.IsOk()))
					{
						if(!(luN.getErrStr().empty())) 
						{
							ADDTALKMSG(luN.getErrStr());
						}
						return;
					}

					//msg += "[" + strHeader + "]";
					msg += strHeader;
					msg += luN.getStr(TALK_STR);
				}
			}
			else
			{
				msg = GetColorByChannelType(type, FALSE) + vParam.at(2);
			}
		}

		//添加信息到窗体
		if(CGameProcedure::s_pUISystem)
		{
			CGameProcedure::s_pUISystem->addChatHistoryMessage(msgID,"Chat_Frame_History", msg.c_str(), nOp);
		}
	}

//------------------------------------------------------------------------------------------------------ 
//字符串分析
	TalkAnalyze::TalkAnalyze(INT ty, STRING str)
	{
		m_OrInputStr = str;
		m_OrChannelTye = ty;

		m_ChannelType = CHAT_TYPE_INVALID;
		m_Ok = FALSE;
		m_StrType = STRINGTYPE_INVALID;

		m_HeaderStr.erase();
	}

	STRING TalkAnalyze::getStr(INT type)
	{
		STRING strRet = "";
		if(!m_Ok) return strRet;

		switch(type) 
		{
		case ORGINAL_STR:
			strRet = m_OrInputStr;
			break;
		case SEND_STR:
			strRet = m_SendStr;
			break;
		case TALK_STR:
			strRet = m_TalkStr;
			break;
		case PAOPAO_STR:
			strRet = m_PaoPaoStr;
			break;
		case HISTORY_STR:
			strRet = m_HistoryStr;
			break;
		case PRIVATE_STR:
			strRet = m_TargetStr;
			break;
		default:
			break;
		}

		return strRet;
	}

	VOID TalkAnalyze::doAnalyze(STRING &strTalker)
	{
		m_PrevResult = m_OrInputStr;
		//m_PrevResult = CreateItemElementInfo1( m_OrInputStr );
		//m_ChannelType = m_OrChannelTye;

		JudgeChannelType();
		MoveHeader();
		JudgeStringType();
		RulerCheck();
		GenAllStr();

		m_PrevResult.erase();
	}

	VOID TalkAnalyze::JudgeChannelType()
	{
		if(m_PrevResult.empty()) return;

		m_ChannelType = Talk::s_Talk.GetChatTypeFromMessage(m_PrevResult);
		if(CHAT_TYPE_INVALID == m_ChannelType)
		{
			m_ChannelType = m_OrChannelTye;
		}

		if(m_ChannelType == CHAT_TYPE_TELL)
		{
			Talk::s_Talk.UserCreate();
		}
	}

	VOID TalkAnalyze::MoveHeader()
	{
		if(m_PrevResult.empty()) return;

		INT sz = (INT)m_PrevResult.size();
		CHAR cH = '\0';

		if(sz>=1) cH = m_PrevResult.at(0);
		if(cH == '/')
		{
			STRING::size_type sF = m_PrevResult.find_first_of(" ");
			if(sF != STRING::npos && sz >= 2)
			{
				// 玩家频道命令新规则, 为私聊规则作修改 Nick 2008.1.10
				STRING str = m_PrevResult.substr( sF + 1 );
				STRING::size_type sF1 = str.find_first_of(" ");		
				if(m_ChannelType == CHAT_TYPE_TELL)
				{
					m_TargetStr = str.substr(0, sF1);
					if(m_TargetStr.size() > MAX_CHARACTER_NAME) m_TargetStr.erase();
				}

				m_HeaderStr = m_PrevResult.substr(0, sF);
				m_PrevResult = m_PrevResult.substr(sF);
				if(!(m_PrevResult.empty()) && m_PrevResult.size() >= 2)
				{
					m_HeaderStr += m_PrevResult.substr(0,1);
					m_PrevResult = str.substr(sF1+1);
				}
				else
				{
					m_PrevResult.erase();
					m_HeaderStr.erase();
				}
			}
			else
			{
				m_PrevResult.erase();
				m_HeaderStr.erase();
			}
		}
	}

	VOID TalkAnalyze::JudgeStringType()
	{
		if(m_PrevResult.empty()) return;
		
		INT sz = (INT)m_PrevResult.size();
		CHAR cH[2] = {0};

		if(sz>=1) cH[0] = m_PrevResult.at(0);
		if(sz>=2) cH[1] = m_PrevResult.at(1);

		struct st{
			TALKANALYZE_STRTYPE_ENUM	ty;
			const CHAR*					pCmd;
			INT							num;
		};

		//@*特殊类型表
		static st typeTable[] = {
			{ STRINGTYPE_FLAGADD,			"flagadd",		6 },
			{ STRINGTYPE_FLASHADD,			"flashadd",		6 },
			{ STRINGTYPE_FLAGDEL,			"flagdel",		6 },
			{ STRINGTYPE_FLASHDEL,			"flashdel",		6 },

			{ STRINGTYPE_FLAG_NPC_ADD,		"flagNPC",		6 },
			{ STRINGTYPE_FLAG_POS_ADD,		"flagPOS",		6 },
			{ STRINGTYPE_FLASH_NPC_ADD,		"flashNPC",		6 },
			{ STRINGTYPE_FLASH_POS_ADD,		"flashPOS",		6 },
			{ STRINGTYPE_MONSTER_PAOPAO,	"npcpaopao",	4 },
			{ STRINGTYPE_SERVERMSG_TALK,	"SrvMsg",		3 },	//个数不定，最少3个
		};
		
		//@@	目标不在线
		//@*	其他控制命令
		//@		文字表情
		//*		人物动作
		if(cH[0] == '@')
		{
			if(cH[1] == '@') 
			{
				m_StrType = STRINGTYPE_NOUSER;
			}
			else if(cH[1] == '*')
			{
				std::vector<std::string> vCmd;
				INT nNumber = DBC::DBCFile::_ConvertStringToVector(m_PrevResult.c_str(),vCmd,";",TRUE,TRUE);

				for(INT i = 0; i < sizeof(typeTable)/sizeof(st); ++i)
				{
					if(nNumber >= typeTable[i].num && 0 == strcmp(vCmd[1].c_str(), typeTable[i].pCmd))
						m_StrType = typeTable[i].ty;
				}
			}
			else
			{
				m_StrType = STRINGTYPE_TXTACT;
			}
		}
		else if(cH[0] == '*')
		{
			m_StrType = STRINGTYPE_ACT;
		}
		else if(cH[0] == '!' && cH[1] == '!')
		{
			m_StrType = STRINGTYPE_GM;
		}
		else
		{
			m_StrType = STRINGTYPE_NORMAL;
		}
	}

	VOID TalkAnalyze::GenAllStr()
	{
		if(m_PrevResult.empty()) return;

		switch(m_StrType)
		{
		case STRINGTYPE_NORMAL:			//普通消息
		case STRINGTYPE_TXTACT:			//文字表情
		case STRINGTYPE_ACT:			//人物休闲动作
			GenSendStr();
			GenTalkStr();
			GenPaoPaoStr();
			GenHistoryStr();
			break;
		case STRINGTYPE_NOUSER:			//私聊目标不在线，服务器传回来的提示信息
			GenTalkStr();
			GenHistoryStr();
			break;
		case STRINGTYPE_MONSTER_PAOPAO:	//怪物泡泡
		case STRINGTYPE_SERVERMSG_TALK:	//Server端发送过来的提示信息消息
		case STRINGTYPE_GM:				//GM命令
		case STRINGTYPE_FLAGADD:		//地图指示点填加
		case STRINGTYPE_FLASHADD:		//地图闪烁点填加
		case STRINGTYPE_FLAGDEL:		//地图指示点删除
		case STRINGTYPE_FLASHDEL:		//地图闪烁点删除

		case STRINGTYPE_FLAG_NPC_ADD:	//用NPC名字来添加指示点
		case STRINGTYPE_FLAG_POS_ADD:	//按x,z来添加指示点
		case STRINGTYPE_FLASH_NPC_ADD:	//用NPC名字来添加黄色指示点
		case STRINGTYPE_FLASH_POS_ADD:	//用x,z来添加黄色指示点
			break;
		case STRINGTYPE_INVALID:
		default:
			break;
		}
		
	}

	BOOL TalkAnalyze::IsOk()
	{
		m_Ok = TRUE;

		switch(m_StrType)
		{
		case STRINGTYPE_NORMAL:			//普通消息
			{
				if(m_SendStr.empty() && m_TalkStr.empty()) m_Ok = FALSE;
				if(!CGameProcedure::s_pUISystem->CheckStringFilter(m_SendStr, FT_CHAT))
				{
					//m_ErrStr = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_InvalidContent");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "你输入的文字中含有敏感内容，发送不成功。");
					m_Ok = FALSE;
				}
			}
			break;
		case STRINGTYPE_TXTACT:			//文字表情
		case STRINGTYPE_ACT:			//人物休闲动作
			{
				//if(m_ChannelType != CHAT_TYPE_NORMAL) m_Ok = FALSE; //现在有强制切换玩家频道的要求，暂不判断这个要求
				if(m_SendStr.empty() && m_PaoPaoStr.empty()) m_Ok = FALSE;
			}
			break;
		case STRINGTYPE_NOUSER:			//私聊目标不在线，服务器传回来的提示信息
			{
				if(m_ChannelType != CHAT_TYPE_SELF) m_Ok = FALSE;
				if(m_TalkStr.empty() || m_HistoryStr.empty()) m_Ok = FALSE;
			}
			break;
		case STRINGTYPE_GM:				//GM命令
			{
				if(m_OrInputStr.size() < 2) m_Ok = FALSE;
			}
			break;
		case STRINGTYPE_SERVERMSG_TALK:	//Server端发送过来的提示信息消息
			{
				if(m_ChannelType != CHAT_TYPE_SELF && m_ChannelType != CHAT_TYPE_SYSTEM) m_Ok = FALSE;
				if(m_OrInputStr.size() < 2) m_Ok = FALSE;
			}
			break;
		case STRINGTYPE_MONSTER_PAOPAO:	//怪物泡泡
		case STRINGTYPE_FLAGADD:		//地图指示点填加
		case STRINGTYPE_FLASHADD:		//地图闪烁点填加
		case STRINGTYPE_FLAGDEL:		//地图指示点删除
		case STRINGTYPE_FLASHDEL:		//地图闪烁点删除

		case STRINGTYPE_FLAG_NPC_ADD:	//用NPC名字来添加指示点
		case STRINGTYPE_FLAG_POS_ADD:	//按x,z来添加指示点
		case STRINGTYPE_FLASH_NPC_ADD:	//用NPC名字来添加黄色指示点
		case STRINGTYPE_FLASH_POS_ADD:	//用x,z来添加黄色指示点
			{
				if(m_ChannelType != CHAT_TYPE_SELF) m_Ok = FALSE;
				if(m_OrInputStr.size() < 2) m_Ok = FALSE;
			}
			break;
		case STRINGTYPE_INVALID:
		default:
			m_Ok = FALSE;
			break;
		}

		if(m_ChannelType == CHAT_TYPE_INVALID) m_Ok = FALSE;
		return m_Ok;
	}

	VOID TalkAnalyze::RulerCheck()
	{
		BOOL bCheck = Talk::s_Talk.m_bPaoPaoRule;
		if(!bCheck || m_PrevResult.empty()) return;

		//需要限制玩家能够发送的文字数目
		if(m_StrType == STRINGTYPE_NORMAL && m_ChannelType != CHAT_TYPE_TELL)
		{
			STRING str24 = m_PrevResult.substr(0, MAX_INPUT_CHAR_LENGTH);
			m_PrevResult = Talk::s_Talk.TalkFixTail(str24);
		}
	}

	STRING TalkAnalyze::RemoveInvalidControlChar(const STRING& strSrc)
	{
		const CHAR KeyStart = '#';
		STRING strOut;
		STRING strErr;

		STRING::size_type sB = 0;
		STRING::size_type sE = strSrc.find_first_of(KeyStart);
		STRING::size_type sLen = strSrc.size();

		do
		{	
			if(sE == STRING::npos)
			{
				//copy last
				strOut += strSrc.substr(sB);
				break;
			}

			//copy normal
			strOut += strSrc.substr(sB, sE-sB);

			//get op
			STRING strOp = strSrc.substr(sE+1, 1);

			//erase invalid control char
			//#- is invalid control
			if(	strOp == "R" || strOp == "G" || strOp == "B" ||
				strOp == "K" || strOp == "Y" || strOp == "W" ||
				strOp == "b" || strOp == "u" || strOp == "r" ||
				strOp == "n" || strOp == "#" || strOp == "-")
			{
				//ok valid control char
				strOut += strSrc.substr(sE, 2);
				sE += 2;
			}
			else if(strOp == "c" || strOp == "e")
			{
				STRING strColor = strSrc.substr(sE+2, 6);
				STRING strValid;

				for(INT i = 0; i < (INT)strColor.size(); ++i)
				{
					if(	(strColor.at(i) >= 48 && strColor.at(i) <= 57)  ||		/*0-9*/
						(strColor.at(i) >= 65 && strColor.at(i) <= 70)  ||		/*A-F*/
						(strColor.at(i) >= 97 && strColor.at(i) <= 102))		/*a-f*/
					{
						strValid += strColor.substr(i,1);
					}
					else
					{
						break;
					}
				}

				if(strValid.size() != 6)
				{
					//invalid extend color input, skip
					sE += 2 + strValid.size();
					strErr += " Invalid #" + strOp + strValid + "!!!";
				}
				else
				{
					//ok valid control char
					strOut += strSrc.substr(sE, 2 + 6);
					sE += 2 + 6;
				}
			}
			else if(strOp.size() >= 1 && strOp.at(0) >= 48 && strOp.at(0) <= 57)
			{
				STRING strNum2 = strSrc.substr(sE+2,1);
				if(strNum2.size() >= 1 && strNum2.at(0) >= 48 && strOp.at(0) <= 57)
				{
					//ok valid Animate numbers( Two )
					strOut += strSrc.substr(sE, 2 + 1);
					sE += 2 + 1;
				}
				else
				{
					//ok valid Animate number( One )
					strOut += strSrc.substr(sE, 2);
					sE += 2;
				}
			}
			else if(strOp == "a" || strOp == "{" || strOp == "}") //user can't input hyper-link type string
			{
				//invalid control char in input, skip
				sE += 2;
				strErr += " Invalid #" + strOp + "!!!";
			}
			else
			{
				//single # , skip
				sE += 1;
				strErr += " Invalid Single \'#\'!!!";
			}

			if(sE >= sLen) break;

			//save new begin point
			sB = sE;

			//find next KeyStart
			sE = strSrc.find(KeyStart, sB);

		}while(TRUE);

		return strOut;
	}

	STRING TalkAnalyze::RemoveSpaceChar(const STRING& strIn)
	{
		const CHAR ansiSpace = ' ';			//英文空格
		const CHAR doubleSpace[3] = "　";	//中文空格
		STRING strMid;
		STRING strOut;

		STRING::size_type sB = 0;
		STRING::size_type sE = strIn.find_first_of(ansiSpace);
		STRING::size_type sLen = strIn.size();

		//删除英文空格
		do
		{	
			if(sE == STRING::npos)
			{
				//copy last
				strMid += strIn.substr(sB);
				break;
			}

			//copy normal
			strMid += strIn.substr(sB, sE-sB);

			//erase ansi space
			sE += 1;

			if(sE >= sLen) break;

			//save new begin point
			sB = sE;

			//find next KeyStart
			sE = strIn.find(ansiSpace, sB);

		}while(TRUE);

		//删除中文空格
		sE = 0;
		sLen = strMid.size();

		do
		{	
			if(sE+2 > sLen)
			{
				//copy last
				strOut += strMid.substr(sE);
				break;
			}

			//get 2 char
			STRING strOp = strMid.substr(sE, 2);

			if(strOp.at(0) != doubleSpace[0] || strOp.at(1) != doubleSpace[1])
			{
				//copy normal
				strOut += strMid.substr(sE, 2);
			}

			sE += 2;

		}while(TRUE);

		return strOut;
	}

	STRING TalkAnalyze::ReplaceItemElementInfo1(const STRING &strSrc)
	{
		const CHAR KeyStart		= '#';

		const STRING MsgBegin	= "#{_INFOMSG";
		const CHAR ContentsEnd = '}';

		/*
		1.转化前:	请看这把#{_INFOID123}多棒啊。
		2.经过CTransferItemSystem的转化，把123号element转化为一个合法字串。
		3.转化后:	请看这把#{_INFOMSG长刀的说明}多棒啊。
		*/

		STRING strOut;

		STRING::size_type sB = 0;
		STRING::size_type sE = strSrc.find_first_of(KeyStart);
		STRING::size_type sLen = strSrc.size();

		do
		{
			if( sE == STRING::npos )
			{
				//copy last
				strOut += strSrc.substr( sB );
				break;
			}

			//copy normal
			strOut += strSrc.substr( sB, sE- sB );

			//get op
			STRING strOp = strSrc.substr( sE+1, 1 );
			
			if ( strOp == "{" )	//OK, check magic #{} string.
			{
				//item element is valid. ex: #{_INFOID123}
				STRING strItemElement = strSrc.substr( sE+2, 7 );
				if ( strItemElement == "_INFOID" )
				{
					//get itemId
					//todo_yangjun	需要仔细检查剩下的字符是否还是一个完整的INFOID信息
					STRING::size_type sIDEnd = strSrc.find(ContentsEnd, sE+2+7);
					if(sE+2+7 >= sLen)	// fix dead loop if str is "xxx#{_INFOID" [9/25/2006]
					{
						//skip invalid #{
						sE += 2;
						goto LengthOver;
					}
					STRING strId = strSrc.substr(sE+2+7, sIDEnd-sE-2-7);
					INT itemId = atoi(strId.c_str());

					if(CTransferItemSystem::GetMe() && CTransferItemSystem::GetMe()->IsElementExist(itemId) && CGameProcedure::s_pUISystem)
					{//ok, valid item element found.

						//0. push item to collect.
						m_vInfo.push_back(itemId);

						//1. head
						strOut += MsgBegin;

						//2. content
						strOut += CTransferItemSystem::GetMe()->GetElementContents(itemId);

						//3. tail
						strOut += ContentsEnd;
					}

					//step to new point.
					sE = sIDEnd + 1;
				}
				else
				{
					//all other things is not allowed.
					strOut += strSrc.substr(sE,2);
					sE += 2;
				}
			}
			else
			{
				//single #
				strOut += strSrc.substr(sE,1);
				sE += 1;
			}
LengthOver:
			if(sE >= sLen) break;

			//save new begin point
			sB = sE;

			//find next KeyStart
			sE = strSrc.find(KeyStart, sB);

		}while(TRUE);

		return strOut;
	}

	STRING TalkAnalyze::CreateItemElementInfo1(const STRING &strSrc)
	{
		const CHAR		KeyStart		= '#';

		const STRING	MsgBegin		= "#{_INFOMSG";
		const CHAR		ContentsEnd		= '}';

		const STRING	IdBegin			= "#{_INFOID";
		const STRING	IdEnd			= "}";

		const STRING	ErrSign			= "<ERRTRANS>";

		/*
		1.转化前:	请看这把#{_INFOMSG长刀的说明}多棒啊。
		2.经过CTransferSystem的转化，创建了一个新的Element，其编号为123。
		3.转化后:	请看这把#{_INFOID123}多棒啊。
		*/

		STRING strOut;

		STRING::size_type sB = 0;
		STRING::size_type sE = strSrc.find_first_of(KeyStart);
		STRING::size_type sLen = strSrc.size();

		do
		{	
			if(sE == STRING::npos)
			{
				//copy last
				strOut += strSrc.substr(sB);
				break;
			}

			//copy normal
			strOut += strSrc.substr(sB, sE-sB);

			//get op
			STRING strOp = strSrc.substr(sE+1, 1);

			if(strOp == "{")	//ok, check magic #{} string.
			{
				//item element is valid. ex: #{_INFOMSG长刀的说明}
				if(sE+2 >= sLen) break;
				STRING strItemElement = strSrc.substr(sE+2, 8);
				if(strItemElement == "_INFOMSG")
				{
					////get item display name
					////todo_yangjun	需要仔细检查剩下的字符是否还是一个完整的INFOMSG信息
					//INT nContentsLen = atoi(strSrc.substr(sE+2+8,3).c_str());
					//STRING::size_type sIDEnd = sE+2+8+3+nContentsLen; //strSrc.find(ContentsEnd, sE+2+8);
					//if(sIDEnd >= sLen) break;
					//if(strSrc.at(sIDEnd) != ContentsEnd)
					//{
					//	//todo_yangjun 需要仔细查找为何不是结尾标识的原因
					//	sIDEnd = strSrc.find(ContentsEnd,sIDEnd);
					//}
					//if(sIDEnd == STRING::npos) break;

					//STRING strContents = strSrc.substr(sE+2+8, 3+nContentsLen);
					STRING strContents = strSrc.substr( sE+2+8, sLen-sE-2-8 );
					//STRING::size_type sIDEnd = strContents.find_first_of(IdEnd);
					STRING::size_type sIDEnd = strSrc.find(IdEnd, sE);
					STRING::size_type nContentsLen = sIDEnd - sE - 2- 8; //strSrc.find(ContentsEnd, sE+2+8);
					if(sIDEnd >= sLen) break;
					if(strSrc.at(sIDEnd) != ContentsEnd)
					{
						//todo_yangjun 需要仔细查找为何不是结尾标识的原因
						sIDEnd = strSrc.find(ContentsEnd,sIDEnd);
					}
					if(sIDEnd == STRING::npos) break;

					strContents = strSrc.substr(sE+2+8, nContentsLen);


					if(CTransferItemSystem::GetMe())
					{//ok, valid item element found.

						//2. content
						CTransferItemSystem::Element* pE = CTransferItemSystem::GetMe()->ConvertFromString("", strContents);
						if(pE)
						{
							//1. head
							strOut += IdBegin;

							CHAR itemId[64] = {0};
							_snprintf(itemId, 63, "%d", pE->nID);
							strOut += itemId;

							//3. push item to collect.
							m_vInfo.push_back(pE->nID);

							//4. tail
							strOut += IdEnd;
						}
						else
						{
							strOut += ErrSign;
						}
					}

					//step to new point.
					sE = sIDEnd + 1;
				}
				else
				{
					//all other things
					strOut += strSrc.substr(sE, 2);
					sE += 2;
				}
			}
			else
			{
				//single #
				strOut += strSrc.substr(sE, 1);
				sE += 1;
			}

			if(sE >= sLen) break;

			//save new begin point
			sB = sE;

			//find next KeyStart
			sE = strSrc.find(KeyStart, sB);

		}while(TRUE);

		return strOut;
	}


	STRING TalkAnalyze::ReplaceItemElementInfo(const STRING& strSrc)
	{
		const CHAR		KeyStart		= '#';

		const STRING	MsgBegin		= "#{_INFOMSG<";
		const CHAR		DisplayEnd		= '>';
		const CHAR		ContentsEnd		= '}';

		/*
		1.转化前:	请看这把#{_INFOID123}多棒啊。
		2.经过CTransferSystem的转化，把123号element转化为一个合法字串。
		3.转化后:	请看这把#{_INFOMSG<长刀>长刀的说明}多棒啊。	
		*/

		STRING strOut;

		STRING::size_type sB = 0;
		STRING::size_type sE = strSrc.find_first_of(KeyStart);
		STRING::size_type sLen = strSrc.size();

		do
		{	
			if(sE == STRING::npos)
			{
				//copy last
				strOut += strSrc.substr(sB);
				break;
			}

			//copy normal
			strOut += strSrc.substr(sB, sE-sB);

			//get op
			STRING strOp = strSrc.substr(sE+1, 1);

			if(strOp == "{")	//ok, check magic #{} string.
			{
				//item element is valid. ex: #{_INFOID123}
				STRING strItemElement = strSrc.substr(sE+2, 7);
				if(strItemElement == "_INFOID")
				{
					//get itemId
					//todo_yangjun	需要仔细检查剩下的字符是否还是一个完整的INFOID信息
					STRING::size_type sIDEnd = strSrc.find(ContentsEnd, sE+2+7);
					STRING strId = strSrc.substr(sE+2+7, sIDEnd-sE-2-7);
					INT itemId = atoi(strId.c_str());

					if(CTransferItemSystem::GetMe() && CTransferItemSystem::GetMe()->IsElementExist(itemId) && CGameProcedure::s_pUISystem)
					{//ok, valid item element found.

						//0. push item to collect.
						m_vInfo.push_back(itemId);

						//1. head
						strOut += MsgBegin;

						//2. content
						STRING strNoColor;
						CGameProcedure::s_pUISystem->ParserString_NoColor(CTransferItemSystem::GetMe()->GetOrgElementDisplayName(itemId), strNoColor);
						strOut += strNoColor;
						strOut += DisplayEnd;
						strOut += CTransferItemSystem::GetMe()->GetElementContents(itemId);

						//3. tail
						strOut += ContentsEnd;
					}

					//step to new point.
					sE = sIDEnd + 1;
				}
				else
				{
					//all other things
					strOut += strSrc.substr(sE,2);
					sE += 2;
				}
			}
			else
			{
				//single #
				strOut += strSrc.substr(sE,1);
				sE += 1;
			}

			if(sE >= sLen) break;

			//save new begin point
			sB = sE;

			//find next KeyStart
			sE = strSrc.find(KeyStart, sB);

		}while(TRUE);

		return strOut;
	}

	STRING TalkAnalyze::CreateItemElementInfo(const STRING& strSrc)
	{
		const CHAR		KeyStart		= '#';

		const STRING	MsgBegin		= "#{_INFOMSG<";
		const CHAR		DisplayEnd		= '>';
		const CHAR		ContentsEnd		= '}';

		const STRING	IdBegin			= "#{_INFOID";
		const STRING	IdEnd			= "}";

		/*
		1.转化前:	请看这把#{_INFOMSG<长刀>长刀的说明}多棒啊。
		2.经过CTransferSystem的转化，创建了一个新的Element，其编号为123。
		3.转化后:	请看这把#{_INFOID123}多棒啊。
		*/

		STRING strOut;

		STRING::size_type sB = 0;
		STRING::size_type sE = strSrc.find_first_of(KeyStart);
		STRING::size_type sLen = strSrc.size();

		do
		{	
			if(sE == STRING::npos)
			{
				//copy last
				strOut += strSrc.substr(sB);
				break;
			}

			//copy normal
			strOut += strSrc.substr(sB, sE-sB);

			//get op
			STRING strOp = strSrc.substr(sE+1, 1);

			if(strOp == "{")	//ok, check magic #{} string.
			{
				//item element is valid. ex: #{_INFOMSG<长刀>长刀的说明}
				STRING strItemElement = strSrc.substr(sE+2, 9);
				if(strItemElement == "_INFOMSG<")
				{
					//get item display name
					//todo_yangjun	需要仔细检查剩下的字符是否还是一个完整的INFOMSG信息
					STRING::size_type sIDEnd = strSrc.find(DisplayEnd, sE+2+9);
					STRING strDisplayName = strSrc.substr(sE+2+9, sIDEnd-sE-2-9);

					STRING::size_type sContentEnd = strSrc.find(ContentsEnd, sE+2+9+strDisplayName.length()+1);
					STRING strContents = strSrc.substr(sE+2+9+strDisplayName.length()+1, sContentEnd-sE-2-9-strDisplayName.length()-1);

					if(CTransferItemSystem::GetMe())
					{//ok, valid item element found.

						//1. head
						strOut += IdBegin;

						//2. content
						CTransferItemSystem::Element* pE = CTransferItemSystem::GetMe()->ConvertFromString(strDisplayName, strContents);
						CHAR itemId[64] = {0};
						_snprintf(itemId, 63, "%d", pE->nID);
						strOut += itemId;

						//3. push item to collect.
						m_vInfo.push_back(pE->nID);

						//4. tail
						strOut += IdEnd;
					}

					//step to new point.
					sE = sContentEnd + 1;
				}
				else
				{
					//all other things
					strOut += strSrc.substr(sE,2);
					sE += 2;
				}
			}
			else
			{
				//single #
				strOut += strSrc.substr(sE,1);
				sE += 1;
			}

			if(sE >= sLen) break;

			//save new begin point
			sB = sE;

			//find next KeyStart
			sE = strSrc.find(KeyStart, sB);

		}while(TRUE);

		return strOut;
	}


	VOID TalkAnalyze::CheckItemElementInfo()
	{
		const CHAR		KeyStart		= '#';
		const CHAR		ContentsEnd		= '}';

		/*
		1.统计当前的聊天字串里的ItemElement的数量。
		*/
		STRING strSrc = m_OrInputStr;

		STRING::size_type sB = 0;
		STRING::size_type sE = strSrc.find_first_of(KeyStart);
		STRING::size_type sLen = strSrc.size();

		do
		{	
			if(sE == STRING::npos)
			{
				//copy last
				break;
			}

			//copy normal

			//get op
			STRING strOp = strSrc.substr(sE+1, 1);

			if(strOp == "{")	//ok, check magic #{} string.
			{
				//item element is valid. ex: #{_INFOID123}
				STRING strItemElement = strSrc.substr(sE+2, 7);
				if(strItemElement == "_INFOID")
				{
					//get itemId
					//todo_yangjun	需要仔细检查剩下的字符是否还是一个完整的INFOID信息
					STRING::size_type sIDEnd = strSrc.find(ContentsEnd, sE+2+7);
					if(sE+2+7 >= sLen)	// fix dead loop if str is "xxx#{_INFOID" [9/25/2006]
					{
						//skip invalid #{
						sE += 2;
						goto LengthOver1;
					}
					STRING strId = strSrc.substr(sE+2+7, sIDEnd-sE-2-7);
					INT itemId = atoi(strId.c_str());

					if(CTransferItemSystem::GetMe() && CTransferItemSystem::GetMe()->IsElementExist(itemId) && CGameProcedure::s_pUISystem)
					{//ok, valid item element found.

						//0. push item to collect.
						m_vInfo.push_back(itemId);
					}

					//step to new point.
					sE = sIDEnd + 1;
				}
				else
				{
					//all other things
					sE += 2;
				}
			}
			else
			{
				//single #
				sE += 1;
			}
LengthOver1:
			if(sE >= sLen) break;

			//save new begin point
			sB = sE;

			//find next KeyStart
			sE = strSrc.find(KeyStart, sB);

		}while(TRUE);

		m_Ok = TRUE;
	}

// ------------------------------------------------------------------------------------------------------------------------
	//发送信息时生成
	VOID SendTalkAnalyze::GenSendStr()
	{
		//普通类型字串
		if(m_StrType == STRINGTYPE_NORMAL)
		{
			m_SendStr = m_PrevResult;
		}

		//文字表情
		if(m_StrType == STRINGTYPE_TXTACT)
		{
			//check
			STRING strKey = m_PrevResult.substr(0,m_PrevResult.find(" "));
			STRING strTalker = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name();
			m_SendStr = Talk::s_Talk.GetTalkTemplateString( strKey, strTalker );
			if(m_SendStr.empty()) 
			{
				m_ErrStr = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_Template_Error");
				return;
			}

			m_SendStr = strKey;
		}

		//休闲动作
		if(m_StrType == STRINGTYPE_ACT)
		{
			//check
			STRING strAct = m_PrevResult.substr(strlen(CHAT_ACT_HEADER), m_PrevResult.find(" ")-strlen(CHAT_ACT_HEADER));
			m_SendStr = Talk::s_Talk.GetTalkActMsg(strAct.c_str(), TALK_ACT_NOBODY);
			if(m_SendStr.empty())
			{
				m_ErrStr = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_Act_Error");
				return;
			}

			m_SendStr = CHAT_ACT_HEADER + strAct;

		}
        
		//填加操作对象到发送字串中
		if(m_StrType == STRINGTYPE_ACT || m_StrType == STRINGTYPE_TXTACT)
		{
			STRING::size_type sF = m_PrevResult.find_first_of(" ");
			if(sF != STRING::npos)
			{
				STRING strOp = m_PrevResult.substr(sF);
				if(strOp == " u")
				{
					CObject* pObj = (CObject*)CObjectManager::GetMe()->GetMainTarget();
					if(pObj && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
					{
						m_SendStr += " " + STRING(((CObject_Character*)pObj)->GetCharacterData()->Get_Name());
					}
				}
				else if(strOp == " i")
				{
					CObject* pObj = (CObject*)CObjectManager::GetMe()->GetMySelf();
					m_SendStr += " " + STRING(((CObject_Character*)pObj)->GetCharacterData()->Get_Name());
				}
			}
		}

		//m_SendStr = RemoveInvalidControlChar(m_SendStr);
		m_SendStrBak = m_SendStr;
		m_SendStr = ReplaceItemElementInfo1( m_SendStr );

	}

	VOID SendTalkAnalyze::GenHistoryStr()
	{
		if(m_StrType == STRINGTYPE_NORMAL || m_StrType == STRINGTYPE_ACT || m_StrType == STRINGTYPE_TXTACT )
		{
			m_HistoryStr = m_HeaderStr + m_PrevResult;
		}
	}

	BOOL SendTalkAnalyze::IsOk()
	{
		TalkAnalyze::IsOk();
		if(m_Ok)
		{
			//文字表情、人物动作必须在“当前”频道中使用
			if(getStrType() == STRINGTYPE_ACT || getStrType() == STRINGTYPE_TXTACT)
			{
				if(getChannelType() != CHAT_TYPE_NORMAL)
				{
					if(getChannelType() == getOrChannelType())
					{
						if(CGameProcedure::s_pEventSystem)
						{
							CGameProcedure::s_pEventSystem->PushEvent(GE_CHAT_CHANNEL_CHANGED, "force_near");
						}
						m_ChannelType = CHAT_TYPE_NORMAL;
					}
					else
					{
						m_ErrStr = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_CantUseInThisChannel");
						m_Ok = FALSE;
					}
				}
			}

			//密聊却没有玩家名称
			if(getStrType() == STRINGTYPE_NORMAL && getStr(PRIVATE_STR).empty())
			{
				if(getChannelType() == CHAT_TYPE_TELL)
				{
					if(CGameProcedure::s_pEventSystem)
					{
						CGameProcedure::s_pEventSystem->PushEvent(GE_CHAT_CHANNEL_CHANGED);
					}
					m_ChannelType = CHAT_TYPE_NORMAL;
				}
			}

			//密聊玩家的名称里含有非法字符
			if(getStrType() == STRINGTYPE_NORMAL && !(getStr(PRIVATE_STR).empty()))
			{
				if(getChannelType() == CHAT_TYPE_TELL)
				{
					if(!KLU_CheckStringValid(getStr(PRIVATE_STR).c_str()))
					{
						m_ErrStr = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_InvalidTargetUser");
						m_Ok = FALSE;
					}

					if(0 == strcmp(getStr(PRIVATE_STR).c_str(),CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name()))
					{
						m_ErrStr = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_InvalidCantMySelf");
						m_Ok = FALSE;
					}

					if(!CGameProcedure::s_pUISystem->CheckStringFilter(getStr(PRIVATE_STR), FT_CHAT))
					{
						m_ErrStr = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_InvalidContent");
						m_Ok = FALSE;
					}
				}
			}


			//只有颜色控制符的字串不发送
			if(m_StrType == STRINGTYPE_NORMAL || m_StrType == STRINGTYPE_ACT || m_StrType == STRINGTYPE_TXTACT )
			{
				STRING strNoColor;
				if(CGameProcedure::s_pUISystem)		CGameProcedure::s_pUISystem->ParserString_NoColor(m_SendStr, strNoColor, TRUE);
				if(strNoColor.empty())
				{
					m_ErrStr = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_InvalidTalkContent");
					m_Ok = FALSE;
				}
			}

			//只有空格的字串不发送
			if(m_StrType == STRINGTYPE_NORMAL || m_StrType == STRINGTYPE_ACT || m_StrType == STRINGTYPE_TXTACT )
			{
				STRING strNoColor = RemoveSpaceChar(m_SendStr);
				
				if(strNoColor.empty())
				{
					m_ErrStr = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_InvalidTalkAllSpace");
					m_Ok = FALSE;
				}
			}
		}

		return m_Ok;
	}
// ---------------------------------------------------------------------------------------------------------------
	//接收信息时生成
	VOID RecvTalkAnalyze::doAnalyze(STRING& strTalker)
	{
		//m_PrevResult = m_OrInputStr;
		m_PrevResult = CreateItemElementInfo1( m_OrInputStr );
		m_ChannelType = m_OrChannelTye;

		m_Talker = strTalker;
		JudgeStringType();
		GenAllStr();

		m_PrevResult.erase();
	}

	VOID RecvTalkAnalyze::GenTalkStr()
	{
		//普通类型字串
		if(m_StrType == STRINGTYPE_NORMAL)
		{
			m_TalkStr = m_PrevResult;
		}

		//文字表情&休闲动作
		if(m_StrType == STRINGTYPE_TXTACT || m_StrType == STRINGTYPE_ACT)
		{
			m_TalkStr = m_PrevResult;
		}

		//目标不在线
		if(m_StrType == STRINGTYPE_NOUSER)
		{
			m_TalkStr = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_Obj_Error", m_PrevResult.c_str()+2);
		}
	}

	VOID RecvTalkAnalyze::GenPaoPaoStr()
	{
		if(CHAT_TYPE_NORMAL == m_ChannelType || CHAT_TYPE_TEAM == m_ChannelType)
		{
			m_PaoPaoStr = (CHAT_TYPE_TEAM == m_ChannelType)?COLORMSGFUNC("GMGameInterface_Script_Talk_Info_SetPaoPaoTxt"):"";

			if(CHAT_TYPE_TEAM == m_ChannelType)
			{
				if(CTransferItemSystem::GetMe())
				{
					CTransferItemSystem::Element* pEl = CTransferItemSystem::GetMe()->AddChannel(CHAT_TYPE_TEAM, COLORMSGFUNC("GMGameInterface_Script_Talk_Info_SetPaoPaoTxt"));

					CHAR id[64] = {0};
					_snprintf(id,63,"#{_INFOID%d}", pEl->nID);

					m_PaoPaoStr = Talk::s_Talk.GetColorByChannelType(CHAT_TYPE_TEAM, TRUE);
					m_PaoPaoStr += id;
					m_PaoPaoStr += Talk::s_Talk.GetColorByChannelType(CHAT_TYPE_TEAM, FALSE);
				}
			}

			//普通类型字串
			if(m_StrType == STRINGTYPE_NORMAL)
			{
				m_PaoPaoStr += m_PrevResult;
			}

			//文字表情
			if(m_StrType == STRINGTYPE_TXTACT)
			{
				m_PaoPaoStr = Talk::s_Talk.GetTalkTemplateString(m_PrevResult, m_Talker);
			}

			//休闲动作
			if(m_StrType == STRINGTYPE_ACT)
			{
				m_PaoPaoStr = Talk::s_Talk.GetTalkActString(m_PrevResult, m_Talker);
			}
		}
	}

	VOID RecvTalkAnalyze::GenHistoryStr()
	{
		//普通类型字串&文字表情&休闲动作
		if(m_StrType == STRINGTYPE_NORMAL || m_StrType == STRINGTYPE_TXTACT || m_StrType == STRINGTYPE_ACT)
		{
			m_HistoryStr = m_PrevResult;
		}

		//目标不在线
		if(m_StrType == STRINGTYPE_NOUSER)
		{
			m_HistoryStr = COLORMSGFUNC("GMGameInterface_Script_Talk_Info_Obj_Error", m_PrevResult.c_str()+2);
		}
	}
// ---------------------------------------------------------------------------------------------------------------
	//Lua脚本要求时生成
	VOID LuaTalkAnalyze::doAnalyze(STRING& strTalker)
	{
		m_PrevResult = m_OrInputStr;
		m_ChannelType = m_OrChannelTye;

		m_Talker = strTalker;
		JudgeStringType();
		GenAllStr();

		m_PrevResult.erase();
	}

	VOID LuaTalkAnalyze::GenTalkStr()
	{
		//普通类型
		if(m_StrType == STRINGTYPE_NORMAL)
		{
			m_TalkStr = m_PrevResult;
		}

		//文字表情
		if(m_StrType == STRINGTYPE_TXTACT)
		{
			STRING myName = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name();

			m_TalkStr = Talk::s_Talk.GetTalkTemplateString(m_PrevResult, m_Talker);
			if(m_TalkStr.empty() && m_Talker != myName && CTransferItemSystem::GetMe() )
			{
				CTransferItemSystem::Element* pEl = CTransferItemSystem::GetMe()->AddHyperLink(m_Talker, m_Talker,m_ChannelType);
				CHAR id[64] = {0};
				_snprintf(id,63,"#{_INFOID%d}", pEl->nID);
				m_TalkStr = id + m_PrevResult;
				//m_TalkStr = "#c00CCFF[#aB{" + m_Talker + "}" + m_Talker + "#aE]：" + m_PrevResult;
			}
			else if(m_TalkStr.empty() && m_Talker == myName)
			{
				m_TalkStr = "#W[" + m_Talker + "]：" + m_PrevResult;
			}

		}

		//休闲动作
		if(m_StrType == STRINGTYPE_ACT)
		{
			STRING myName = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name();

			m_TalkStr = Talk::s_Talk.GetTalkActString(m_PrevResult, m_Talker);
			if(m_TalkStr.empty() && m_Talker != myName)
			{
				m_TalkStr = "#c00CCFF[#aB{" + m_Talker + "}" + m_Talker + "#aE]：" + m_PrevResult;
			}
			else if(m_TalkStr.empty() && m_Talker == myName)
			{
				m_TalkStr = "#W[" + m_Talker + "]：" + m_PrevResult;
			}
		}
	}

	BOOL LuaTalkAnalyze::IsOk()
	{
		if(!m_Ok && CHAT_TYPE_INVALID == m_ChannelType)
		{
			m_Ok = TRUE;
		}

		return m_Ok;
	}
// ---------------------------------------------------------------------------------------------------------------
	//客户端添加聊天消息的封装
	VOID Global_Msg_Func::addTalkMsg(LPCTSTR szMsg, ENUM_CHAT_TYPE chatType, CHAR* file, INT line)
	{
		if(szMsg)
		{
			return addTalkMsg(STRING(szMsg), chatType, file, line);
		}
	}

	VOID Global_Msg_Func::addTalkMsg(STRING szMsg, ENUM_CHAT_TYPE chatType, CHAR* file, INT line)
	{
		if(szMsg.empty())	return;

		STRING strType = Talk::s_Talk.ChannelType2String(chatType);
		STRING strName = "";

		TalkAnalyze taN(chatType, szMsg);
		taN.CheckItemElementInfo();

		Talk::HistoryMsg tlkmsg(chatType); \
		tlkmsg.SetMsgName(strName); \
		tlkmsg.SetMsgData(szMsg, file, line); \
		tlkmsg.SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
		tlkmsg.SetItemCollect( taN.GetItemCollect() );

		Talk::s_Talk.AddToRecvHistoryQue(tlkmsg);
/*
		std::vector< STRING > strParam;
		strParam.push_back(strType);
		strParam.push_back(tlkmsg.GetMsgName());
		strParam.push_back(tlkmsg.GetMsgData());

		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_MESSAGE, strParam);
		}*/
	}

	//消息长度检查函数
	INT Global_Msg_Func::caculateMsgLength(STRING &szMsg)
	{
		ENUM_CHAT_TYPE ty = CHAT_TYPE_SELF;
		
		TalkAnalyze taN( ty, szMsg );
		STRING strAnalyze = taN.ReplaceItemElementInfo1( szMsg );
		return (INT)strAnalyze.length();
	}

	INT Global_Msg_Func::caculateMsgLength(tTransferItemSystem::Element* pElement)
	{
		//检查消息字符串加上这个字符后是否长度超出
		STRING strNow = CGameProcedure::s_pUISystem->getIMEEditboxString( "MainMenuBar_EditBox" );
		STRING strTotal = strNow + CTransferItemSystem::GetMe()->GetElementContents(pElement->nID) + "#{_INFOSG}";
		return caculateMsgLength( strTotal );
	}
}

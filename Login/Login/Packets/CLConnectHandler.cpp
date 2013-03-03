#include "stdafx.h" 
#include "CLConnect.h"
#include "LCRetConnect.h"
#include "LoginPlayerManager.h"
#include "ServerManager.h"
#include "LoginPlayer.h"
#include "LCRetConnect.h"
#include "Log.h"
#include "TimeManager.h"

UINT CLConnectHandler::Execute(CLConnect* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		//检查线程执行资源是否正确
		if(MyGetCurrentThreadID()!= g_pLoginPlayerManager->m_ThreadID)
		{
			return PACKET_EXE_CONTINUE;
		}

		LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);

		Assert(pLoginPlayer);
		
		//代理服务器的使用方式[2007/09/03 YangJun]
		pLoginPlayer->SetProxyType((BYTE)pPacket->GetNetProvider());

		//检查Login 服务器繁忙程度，返回一个相对负载小的IP 和Port
		LCRetConnect	Msg;
		Msg.SetResult(LOGINCR_SUCCESS);
		/*
		_SERVER_DATA* pLoginInfo	= g_pServerManager->GetCurrentServerInfo();
		Assert(pLoginInfo);

		Msg.SetLoginIP(pLoginInfo->m_IP0);
		Msg.SetLoginPort(pLoginInfo->m_Port0);
		*/
		
		INT nEnable = g_Config.m_LoginInfo.ProxyConnect;
		if(nEnable == PERMIT_EDU_PROXY_CONNECT)
		{
			INT nIsp = pLoginPlayer->WhereThisPlayerFrom();
			if(INVALID_ISP != nIsp && ISP_CHINA_EDU != nIsp)
			{
				g_pLog->FastSaveLog(LOG_FILE_0, "ERROR:CLConnectHandler::Execute(...)...Only EDU Proxy Permit!!! IP = %s,Port = %u",
					pLoginPlayer->GetSocket()->m_Host,pLoginPlayer->GetSocket()->m_Port);
				//非法登入，直接断开
				return PACKET_EXE_ERROR;
			}

			Msg.SetLoginIP(ProxyTool_GetCorrespondingIP(pLoginPlayer, g_Config.m_LoginInfo.m_LoginID));
			Msg.SetLoginPort(ProxyTool_GetCorrespondingPort(pLoginPlayer, g_Config.m_LoginInfo.m_LoginID));
		}
		else if(nEnable == FORBIT_PROXY_CONNECT)
		{
			INT nIsp = pLoginPlayer->WhereThisPlayerFrom();
			if(INVALID_ISP != nIsp)
			{
				g_pLog->FastSaveLog(LOG_FILE_0, "ERROR:CLConnectHandler::Execute(...)...Proxy Not Permit!!! IP = %s,Port = %u",
					pLoginPlayer->GetSocket()->m_Host,pLoginPlayer->GetSocket()->m_Port);
				//非法登入，直接断开
				return PACKET_EXE_ERROR;
			}

			//这里必须返回直连IP
			Msg.SetLoginIP((g_pServerManager->GetCurrentServerInfo())->m_IP0);
			Msg.SetLoginPort((g_pServerManager->GetCurrentServerInfo())->m_Port0);
		}
		else/* if(nEnable == PERMIT_PROXY_CONNECT)*/
		{
			Msg.SetLoginIP(ProxyTool_GetCorrespondingIP(pLoginPlayer, g_Config.m_LoginInfo.m_LoginID));
			Msg.SetLoginPort(ProxyTool_GetCorrespondingPort(pLoginPlayer, g_Config.m_LoginInfo.m_LoginID));
		}

		if(pPacket->GetUseMiBao() > 0)
		{
			pLoginPlayer->m_bEnableMiBao		= TRUE;
			pLoginPlayer->m_MiBaoInputBeginTime = g_pTimeManager->CurrentTime();
			for(INT i = 0; i < MIBAOUNIT_NUMBER; ++i)
			{
				Msg.SetMiBaoKey(i,pLoginPlayer->m_MiBaoGroup.unit[i].key);
			}
		}
		pLoginPlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog(LOG_FILE_0, "CLConnectHandler::Execute(...)...OK,IP = %s,Port = %d",Msg.GetLoginIP(),Msg.GetLoginPort()) ;

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;

}

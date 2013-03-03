
#include "stdafx.h"
#include "LoginPlayer.h"
#include "PacketFactoryManager.h"
#include "PlayerPool.h"
#include "TimeManager.h"
#include "Log.h"
#include "Config.h"
#include "ServerManager.h"
#include "TurnPlayerQueue.h"
#include "LCStatus.h"
//#include "captcha_generator.h"
//#include "captcha_font_file.h"
#include "LCRetCreateCode.h"
#include "LCRetLoginCode.h"

//using namespace CAPTCHA;
//extern Generator* g_pGenerator ;

LoginPlayer::LoginPlayer( )
{
__ENTER_FUNCTION

	m_Status = PS_LOGIN_EMPTY ;
	
	m_LastSendTime	 = 0 ;
	m_ConnectTime	 = 0 ;
	m_KickTime		 = 0 ;
	m_CurrentTime	 = 0 ;
	m_LeftTimeToQuit = 0 ;
	m_AccountGuid	 = 0 ;	
	
	m_Version		 = 0;
	m_LastSendProcessTurn = 0;
	m_QueuePos		 = 0;
	m_LastDBOpTime	 = 0;

	m_CharNumber	 = -1;
	memset(m_GUIDList,-1,sizeof(GUID_t)*DB_CHAR_NUMBER);
	m_uKey			= -1;
	m_bDBOprating	= FALSE;
	m_ReadyKickCount = 0;
	m_WrongPWCount	 = 0;
	m_SceneID		 = 0;
	m_WorldPos.CleanUp();
	m_bEnableMiBao	 = FALSE;
	m_MiBaoGroup.CleanUp();
	m_MiBaoInputBeginTime	= 0;
	m_FatigueSign	 = FALSE;
	m_AccountTotalOnlineTime	= 0;
	m_ProxyType		 = CL_NP_UNKNOWN;
	m_bAlreadyCheckCreateCode = FALSE;
	m_bAlreadyCheckLoginCode = FALSE;
	m_CreateCode.CleanUp();
	m_LoginCode.CleanUp();
	m_AccountSafeSign.CleanUp();

	ResetDisconnectCountDown();

__LEAVE_FUNCTION
}

LoginPlayer::~LoginPlayer( )
{
__ENTER_FUNCTION
__LEAVE_FUNCTION
}

VOID LoginPlayer::CleanUp( )
{
__ENTER_FUNCTION

	m_Status = PS_LOGIN_EMPTY ;
	m_KickTime		 =  0;
	m_LastSendTime   =  0;
	m_ConnectTime	 =	0;
	m_CurrentTime	 =	0;
	m_LeftTimeToQuit =	0;
	m_AccountGuid	 =	0;
	m_Version		 =	0;
	m_LastSendProcessTurn = 0;
	m_QueuePos			=	0;
	m_LastDBOpTime		=	0;
	m_CharNumber		=  -1;
	memset(m_GUIDList,-1,sizeof(GUID_t)*DB_CHAR_NUMBER);
	m_uKey				= -1;
	m_bDBOprating		= FALSE;
	m_ReadyKickCount	= 0;
	m_WrongPWCount		= 0;
	m_SceneID			= 0;
	m_WorldPos.CleanUp();
	m_bEnableMiBao		= FALSE;
	m_MiBaoGroup.CleanUp();
	m_MiBaoInputBeginTime	= 0;
	m_FatigueSign	 = FALSE;
	m_AccountTotalOnlineTime	= 0;
	m_ProxyType			= CL_NP_UNKNOWN;
	m_bAlreadyCheckCreateCode = FALSE;
	m_bAlreadyCheckLoginCode = FALSE;
	m_CreateCode.CleanUp();
	m_LoginCode.CleanUp();
	m_AccountSafeSign.CleanUp();

	ResetDisconnectCountDown();

	ResetKick( ) ;
	return Player::CleanUp( ) ;

__LEAVE_FUNCTION
}

BOOL LoginPlayer::ProcessCommand( BOOL Option )
{
__ENTER_FUNCTION

	return Player::ProcessCommand( Option ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LoginPlayer::ProcessInput( )
{
__ENTER_FUNCTION

	return Player::ProcessInput( ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LoginPlayer::ProcessOutput( )
{
__ENTER_FUNCTION

	return Player::ProcessOutput( ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LoginPlayer::SendPacket( Packet* pPacket )
{
__ENTER_FUNCTION

	return Player::SendPacket( pPacket ) ;

__LEAVE_FUNCTION

	return FALSE ;
}
//是否当前玩家处于断开连接倒计时状态
BOOL LoginPlayer::IsDisconnectCountDown(VOID) const
{
	return m_bNeedDisconnect || 
		m_DisconnectCountDown.IsSetTimer();
}
//触发断开连接倒计时，参数为倒计时毫秒数
VOID LoginPlayer::TrigerDisconnectCountDown(UINT time)
{
	if(IsDisconnectCountDown())
	{
		ResetDisconnectCountDown();
	}
	UINT now = g_pTimeManager->CurrentTime();
	m_DisconnectCountDown.BeginTimer(time,now);
}
//复位断开连接倒计时
VOID LoginPlayer::ResetDisconnectCountDown(VOID)
{
	m_DisconnectCountDown.CleanUp();
	m_bNeedDisconnect	= FALSE;
}
//如果启动了断开连接倒计时，执行倒计时，倒计时结束返回FALSE，否则返回TRUE
BOOL LoginPlayer::DisconnectCountDown(UINT now)
{
	//倒计时时间已到，直接要求断开连接
	if(m_bNeedDisconnect)
		return FALSE;
	if(m_DisconnectCountDown.IsSetTimer())
	{
		//如果已经开始倒计时，执行计时
		if(m_DisconnectCountDown.CountingTimer(now))
		{
			//倒计时时间到，标记为断开连接，并停止计时器
			m_bNeedDisconnect	= TRUE;
			m_DisconnectCountDown.CleanUp();
			return FALSE;
		}
	}
	//倒计时时间未到或者未启用断开连接倒计时
	return TRUE;
}

BOOL LoginPlayer::HeartBeat( UINT uTime )
{
__ENTER_FUNCTION

	/**	doing 29929，2007-12-25，alan
	*	退出倒计时。
	*/	
	if(!DisconnectCountDown(uTime))
	{
		g_pLog->FastSaveLog(LOG_FILE_0, "LoginPlayer::HeartBeat Disconnect because over turn !" ) ;
		return FALSE;
	}

	if( IsDisconnect())
	{
		m_LeftTimeToQuit -= (uTime-m_CurrentTime) ;
		if( m_LeftTimeToQuit<0 )
		{//真正执行退出处理
  		  return FALSE ;
		}
	}

	m_CurrentTime =uTime ;
	//准备登录的人数过多的时排队
	if(GetPlayerStatus() == PS_LOGIN_PROCESS_TURN)  //排队状态的玩家，每格MAX_TRUN_MESSAGE_TIME
													//时间向客户端发送排队消息
	{
		if(uTime>m_LastSendProcessTurn+MAX_TRUN_MESSAGE_TIME)
		{
			m_LastSendProcessTurn = uTime;
			
			if(m_QueuePos > 0)
			{
				LCStatus Msg;
				Msg.SetClientStatus(CTS_TURN);
				Msg.SetTurnNumber(m_QueuePos);
				SendPacket(&Msg);

				ResetKick();
			}
		}

		//排队玩家永远排队，不断线。
	}
	//World人满或Scene人满时排队
	else if(GetPlayerStatus() == PS_LOGIN_WORLD_PROCESS_TURN)
	{
		if(uTime>m_LastSendProcessTurn+MAX_TRUN_MESSAGE_TIME)
		{
			m_LastSendProcessTurn = uTime;

			if(m_QueuePos > 0)
			{
				LCStatus Msg;
				Msg.SetClientStatus(CTS_TURN);
				Msg.SetTurnNumber(m_QueuePos);
				SendPacket(&Msg);

				ResetKick();
			}
		}

		//排队玩家永远排队，不断线。
	}
	else
	{
		//如果Player在一定时间内没有收到任何消息，则断开客户端连接
		//在Player处理消息前会执行ResetKick函数修正m_KickTime信息
		if(uTime>m_KickTime+MAX_KICK_TIME )
		{
			g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: Player::HeartBeat Didn't recv message for too long time. Kicked!" ) ;
			return FALSE ;
		}
	}

	return Player::HeartBeat(uTime) ;

__LEAVE_FUNCTION

	return FALSE ;
}


BOOL LoginPlayer::FreeOwn( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

	g_pPlayerPool->DelPlayer( PlayerID() ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}



VOID LoginPlayer::Init( )
{
__ENTER_FUNCTION

	SetDisconnect( FALSE ) ;
	ResetKick() ;//开始踢出计时
	//MIBAO::InitMiBaoGroupKey(m_MiBaoGroup);

__LEAVE_FUNCTION
}




VOID LoginPlayer::Disconnect( )
{
__ENTER_FUNCTION

	_MY_TRY
	{
		m_LeftTimeToQuit = g_Config.m_ConfigInfo.m_DisconnectTime ;
		SetDisconnect( TRUE ) ;
	}
	_MY_CATCH
	{
	}

	Player::Disconnect( ) ;

__LEAVE_FUNCTION
}

VOID LoginPlayer::ResetKick( )
{
__ENTER_FUNCTION

	m_KickTime = g_pTimeManager->CurrentSavedTime() ;

	Player::ResetKick( ) ;

__LEAVE_FUNCTION
}

const CHAR*	LoginPlayer::GetAccount()
{
	return szAccount;
}
VOID	LoginPlayer::SetAccount(const CHAR* pAccount)
{
	strncpy(szAccount,pAccount,MAX_ACCOUNT);
	szAccount[MAX_ACCOUNT] = '\0';
}

UINT	LoginPlayer::GetVersion()
{
	return m_Version;
}
VOID	LoginPlayer::SetVersion(UINT uVersion)
{
	m_Version = uVersion;
}

UINT	LoginPlayer::GetQueuePos()
{
	return m_QueuePos;
}

VOID	LoginPlayer::SetQueuePos(UINT QueuePos)
{
	m_QueuePos = QueuePos;
}

VOID		LoginPlayer::SetCharNumber(INT CharNumber)
{
	m_CharNumber = CharNumber;
}

INT	LoginPlayer::GetCharNumber()
{
	return m_CharNumber;
}

VOID LoginPlayer::SetCharGUID(GUID_t guid,UINT uIndex)
{
	Assert(uIndex<DB_CHAR_NUMBER);
	m_GUIDList[uIndex] = guid;
}

BOOL LoginPlayer::IsGUIDOwner(GUID_t guid)
{
	for(INT i=0;(i<m_CharNumber) && (i<DB_CHAR_NUMBER) ;i++)
	{
		if((m_GUIDList[i] == guid) && (guid != INVALID_ID))
		{
			return TRUE;
		}
	}
	return FALSE;
}

VOID	LoginPlayer::SetUserKey(UINT key)
{
	m_uKey = key;
}

UINT	LoginPlayer::GetUserKey()
{
	return m_uKey;
}

VOID	LoginPlayer::SetDBOperating(BOOL bOp)
{
	m_bDBOprating = bOp;
	
}

BOOL	LoginPlayer::GetDBOperating()
{
	return m_bDBOprating;
}

VOID LoginPlayer::SetReadyKickCount(INT KickCount)
{
	m_ReadyKickCount = KickCount;
}

INT	LoginPlayer::GetReadyKickCount()
{
	return m_ReadyKickCount;
}

UINT LoginPlayer::GetLastSendTurnTime()
{
	return m_LastSendProcessTurn;
}

VOID LoginPlayer::SetLastSendTurnTime(UINT uTime)
{
	m_LastSendProcessTurn = uTime;
}

VOID LoginPlayer::SetWrongPWCount(INT Count)
{
	m_WrongPWCount = Count;
}

INT LoginPlayer::GetWrongPWCount()
{
	return m_WrongPWCount;
}

SceneID_t	LoginPlayer::GetChooseSceneID() const
{
	return m_SceneID;
}

VOID		LoginPlayer::SetChooseSceneID(SceneID_t sid)
{
	m_SceneID = sid;
}

WORLD_POS	LoginPlayer::GetWorldPos()	const
{
	return m_WorldPos;
}

VOID		LoginPlayer::SetWorldPos(WORLD_POS& pos)
{
	m_WorldPos = pos;
}

VOID		LoginPlayer::SetAccFatigueSign(CHAR cSign)
{
	m_FatigueSign = (cSign == 'Y')?TRUE:FALSE;
}

VOID		LoginPlayer::SetAccOnlineTime(UINT nTime)
{
	m_AccountTotalOnlineTime = nTime/**1000*/;	//不将秒转换为毫秒
}


BOOL		LoginPlayer::GetPhoneBind()
{
__ENTER_FUNCTION
	return m_AccountSafeSign.IsBindSafeSign(ASF_BIND_MOBILE_PHONE);
__LEAVE_FUNCTION
	return FALSE;
}

BOOL		LoginPlayer::GetIPBind()
{
__ENTER_FUNCTION
	return m_AccountSafeSign.IsBindSafeSign(ASF_BIND_IP);
__LEAVE_FUNCTION
	return FALSE;
}

BOOL		LoginPlayer::GetMiBaoBind()
{
__ENTER_FUNCTION
	return m_AccountSafeSign.IsBindSafeSign(ASF_BIND_MIBAOKA);
__LEAVE_FUNCTION
	return FALSE;
}

BOOL		LoginPlayer::GetMacBind()
{
__ENTER_FUNCTION
	return m_AccountSafeSign.IsBindSafeSign(ASF_BIND_MAC);
__LEAVE_FUNCTION
	return FALSE;
}

BOOL		LoginPlayer::GetRealNameBind()
{
__ENTER_FUNCTION
	return m_AccountSafeSign.IsBindSafeSign(ASF_BIND_REALNAME);
__LEAVE_FUNCTION
	return FALSE;
}

BOOL		LoginPlayer::GetInputNameBind()
{
__ENTER_FUNCTION
	return m_AccountSafeSign.IsBindSafeSign(ASF_BIND_INPUTNAME);
__LEAVE_FUNCTION
	return FALSE;
}

VOID		LoginPlayer::SetPhoneBind(CHAR cSign)
{
__ENTER_FUNCTION
	if('Y' == cSign)	
		m_AccountSafeSign.SetBindSafeSign(ASF_BIND_MOBILE_PHONE);
	else
		m_AccountSafeSign.SetBindSafeSign(ASF_BIND_MOBILE_PHONE,FALSE);
__LEAVE_FUNCTION
}

VOID		LoginPlayer::SetIPBind(CHAR cSign)
{
__ENTER_FUNCTION
	//if('Y' == cSign)
	//	m_AccountSafeSign.SetBindSafeSign(ASF_BIND_IP);
	//else
	//	m_AccountSafeSign.SetBindSafeSign(ASF_BIND_IP,FALSE);
__LEAVE_FUNCTION
}

VOID		LoginPlayer::SetMiBaoBind(CHAR cSign)
{
__ENTER_FUNCTION
	//if('Y' == cSign)	
	//	m_AccountSafeSign.SetBindSafeSign(ASF_BIND_MIBAOKA);
	//else
	//	m_AccountSafeSign.SetBindSafeSign(ASF_BIND_MIBAOKA,FALSE);
__LEAVE_FUNCTION
}

VOID		LoginPlayer::SetMacBind(CHAR cSign)
{
__ENTER_FUNCTION
	if('Y' == cSign)	
		m_AccountSafeSign.SetBindSafeSign(ASF_BIND_MAC);
	else
		m_AccountSafeSign.SetBindSafeSign(ASF_BIND_MAC,FALSE);
__LEAVE_FUNCTION
}

VOID		LoginPlayer::SetRealNameBind(CHAR cSign)
{
__ENTER_FUNCTION
	if('Y' == cSign)	
		m_AccountSafeSign.SetBindSafeSign(ASF_BIND_REALNAME);
	else
		m_AccountSafeSign.SetBindSafeSign(ASF_BIND_REALNAME,FALSE);
__LEAVE_FUNCTION
}

VOID		LoginPlayer::SetInputNameBind(CHAR cSign)
{
__ENTER_FUNCTION
	if('Y' == cSign)	
		m_AccountSafeSign.SetBindSafeSign(ASF_BIND_INPUTNAME);
	else
		m_AccountSafeSign.SetBindSafeSign(ASF_BIND_INPUTNAME,FALSE);
__LEAVE_FUNCTION
}


CHAR* ProxyTool_GetIPByNetProvider(LoginPlayer const* pPlayer,ID_t ServerID)
{
__ENTER_FUNCTION
	Assert(pPlayer);
	_SERVER_DATA* pData = g_pServerManager->FindServerInfo( ServerID );
	if(NULL==pData)
	{
		CHAR szLog[1024];
		tsnprintf(szLog, sizeof(szLog), "[ProxyTool_GetIPByNetProvider] Error: Can't found the specific server(%d).", ServerID);
		szLog[sizeof(szLog)-1] = '\0';
		AssertEx(pData, szLog);
	}
	INT nIsp = INVALID_ISP;

	switch(pPlayer->GetProxyType())
	{
	case CL_NP_TEL:
		nIsp = ISP_CHINA_TELE_COM;
		break;
	case CL_NP_CNC:
		nIsp = ISP_CHINA_NET_COM;
		break;
	case CL_NP_EDU:
		nIsp = ISP_CHINA_EDU;
		break;
	case CL_NP_DEF:
	case CL_NP_UNKNOWN:
	default:
		break;
	}

	if(INVALID_ISP==nIsp)
	{
		return pData->m_IP0;
	}
	else
	{
		//if(TRUE==RangeCheckForIndex_Assert(nIsp, 0, NUM_OF_ISP-1, "ProxyTool_GetIPByNetProvider"))
		//{
		//	PROXY_DATA& rProxy = pData->m_aProxy[nIsp];
		//	if(TRUE==rProxy.m_bEnabled)
		//	{
		//		return rProxy.m_szIP;
		//	}
		//	return pData->m_IP0;
		//}
		//else
		//{
		//	return pData->m_IP0;
		//}
	}

__LEAVE_FUNCTION
	return NULL;
}
UINT  ProxyTool_GetPortByNetProvider(LoginPlayer const* pPlayer,ID_t ServerID)
{
__ENTER_FUNCTION
	Assert(pPlayer);
	_SERVER_DATA* pData = g_pServerManager->FindServerInfo( ServerID );
	if(NULL==pData)
	{
		CHAR szLog[1024];
		tsnprintf(szLog, sizeof(szLog), "[ProxyTool_GetPortByNetProvider] Error: Can't found the specific server(%d).", ServerID);
		szLog[sizeof(szLog)-1] = '\0';
		AssertEx(pData, szLog);
	}
	INT nIsp = INVALID_ISP;

	switch(pPlayer->GetProxyType())
	{
	case CL_NP_TEL:
		nIsp = ISP_CHINA_TELE_COM;
		break;
	case CL_NP_CNC:
		nIsp = ISP_CHINA_NET_COM;
		break;
	case CL_NP_EDU:
		nIsp = ISP_CHINA_EDU;
		break;
	case CL_NP_DEF:
	case CL_NP_UNKNOWN:
	default:
		break;
	}

	if(INVALID_ISP==nIsp)
	{
		return pData->m_Port0;
	}
	else
	{
		if(TRUE==RangeCheckForIndex_Assert(nIsp, 0, NUM_OF_ISP-1, "ProxyTool_GetPortByNetProvider"))
		{
			PROXY_DATA& rProxy = pData->m_aProxy[nIsp];
			if(TRUE==rProxy.m_bEnabled)
			{
				return rProxy.m_nPort;
			}
			return pData->m_Port0;
		}
		else
		{
			return pData->m_Port0;
		}
	}

__LEAVE_FUNCTION
	return NULL;
}

VOID		LoginPlayer::SendCreateCode()
{
__ENTER_FUNCTION
	//if(!g_pGenerator)	return;

	//CAPTCHA_DATA data ;
	//g_pGenerator->generatorImage( data, true, true ) ;

	////记住正确的验证码
	//switch(data.nCorrectAnswer) {
	//case 0:
	//	memcpy( m_CreateCode.code, data.wAnswer0, sizeof(USHORT)*ANASWER_LENGTH_1);
	//	break;
	//case 1:
	//	memcpy( m_CreateCode.code, data.wAnswer1, sizeof(USHORT)*ANASWER_LENGTH_1);
	//	break;
	//case 2:
	//	memcpy( m_CreateCode.code, data.wAnswer2, sizeof(USHORT)*ANASWER_LENGTH_1);
	//	break;
	//case 3:
	//	memcpy( m_CreateCode.code, data.wAnswer3, sizeof(USHORT)*ANASWER_LENGTH_1);
	//	break;
	//default:
	//	Assert(FALSE);
	//}
	//
	////将图形发送到客户端
	//LCRetCreateCode msg;
	//msg.SetRetType(RETCREATECODE_IMAGE);
	//memcpy(msg.GetImageBuf(), data.imageBuf, sizeof(UCHAR)*IMAGE_BUF_LENGTH_1);
	//SendPacket(&msg);

	m_bAlreadyCheckCreateCode = FALSE;
__LEAVE_FUNCTION
}


VOID		LoginPlayer::SendLoginCode()
{
	__ENTER_FUNCTION
	//	if(!g_pGenerator)	return;

	//CAPTCHA_DATA data ;
	//g_pGenerator->generatorImage( data, true, true ) ;

	////记住正确的验证码
	//switch(data.nCorrectAnswer) {
	//case 0:
	//	memcpy( m_LoginCode.code, data.wAnswer0, sizeof(USHORT)*ANASWER_LENGTH_1);
	//	break;
	//case 1:
	//	memcpy( m_LoginCode.code, data.wAnswer1, sizeof(USHORT)*ANASWER_LENGTH_1);
	//	break;
	//case 2:
	//	memcpy( m_LoginCode.code, data.wAnswer2, sizeof(USHORT)*ANASWER_LENGTH_1);
	//	break;
	//case 3:
	//	memcpy( m_LoginCode.code, data.wAnswer3, sizeof(USHORT)*ANASWER_LENGTH_1);
	//	break;
	//default:
	//	Assert(FALSE);
	//}

	////将图形发送到客户端
	//LCRetLoginCode msg;
	//msg.SetRetType(RETLOGINCODE_IMAGE);
	//memcpy(msg.GetImageBuf(), data.imageBuf, sizeof(UCHAR)*IMAGE_BUF_LENGTH_1);
	//SendPacket(&msg);

	m_bAlreadyCheckLoginCode = FALSE;
	__LEAVE_FUNCTION
}
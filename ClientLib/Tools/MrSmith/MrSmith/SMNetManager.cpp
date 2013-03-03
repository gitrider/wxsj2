#include "StdAfx.h"
#include "SMNetManager.h"
#include "SMAgentManager.h"
#include "SMAgent.h"
#include "SMNetSend.h"
#include "SMUtil.h"
#include "SMInfo.h"
#include "SMVariable.h"

#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

#include <mmsystem.h>

#include "Type.h"
#include "Socket.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"
#include "Packet.h"
#include "PacketDefine.h"
#include "PacketFactoryManager.h"

namespace SMITH
{

struct NetImplData
{
	//Read Handle
	fd_set					m_ReadFD;
	//Write Handle
	fd_set					m_WriteFD;
	//Excetption Handle
	fd_set					m_ExceptFD;
	//SOCKET
	Socket					m_Socket;
	//Input Stream
	SocketInputStream		m_SocketInputStream;
	//Output Stream
	SocketOutputStream		m_SocketOutputStream;
	//SOCKET Manager
	PacketFactoryManager	m_PacketFactoryManager;

	NetImplData() : m_SocketInputStream(&m_Socket), m_SocketOutputStream(&m_Socket){}
	~NetImplData() {}
};

void Str2StrArr( string strText, vector<string>& vecParts, char div = '\n' )
{
	while( true )
	{
		size_t ret = strText.find( div );
		if( ret != -1 )
		{
			string strTemp = strText.substr( 0, ret );
			strText = strText.substr( ret + 1 );
			vecParts.push_back( strTemp );
		}
		else
		{
			break;
		}
	}
	if( strText.empty() == false ) vecParts.push_back( strText );
}

const char* GetPacketDefine( PacketID_t pd )
{
	static bool Init = false;
	static vector<string> vresult;
	if( Init == false ) {
		Init = true;
		char* p = 0;
		FILE* fp = fopen( "../../../Common/PacketDefine.h", "r" );
		if( fp ) {
			fseek( fp, 0, SEEK_END );
			long l = ftell( fp );
			p = new char[l+1];
			fseek( fp, 0, SEEK_SET );
			fread( p, 1, l, fp );
			p[l] = 0;
			fclose( fp );
		}
		vector<string> vstr;
		if( p ) {
			Str2StrArr( p, vstr );
			delete [] p ;
			bool Start = false;
			int PacketIndex = 0;
			for( int i = 0; i < (int)vstr.size(); i ++ ) {
				if( vstr[i].find( "PACKET_NONE" ) != string::npos ) {
					Start = true;
				}
				else if( vstr[i].find( "PACKET_MAX" ) != string::npos ) {
					Start = false;
				}
				if( Start == false ) continue;

				PacketIndex ++;
				string des;
				int dot = (int)vstr[i].find( "," );
				int desc = (int)vstr[i].find( "//" );
				if( dot != string::npos ) {
					if( desc != string::npos ) {
						desc += 2;
						des = vstr[i].substr( desc );
					}
					else {
						char buf[10];
						des = string("~~~没有找到该命令的解释(") + itoa(PacketIndex, buf, 10) + ")";
					}
					vresult.push_back( des );
				}
				else {
					if( desc != string::npos ) {
						desc += 2;
						des = vstr[i].substr( desc );
					}
					if( vresult.size() > 0 ) {
						string tmp = vresult[vresult.size()-1];
						tmp += des;
						vresult[vresult.size()-1] = tmp;
					}
				}
			}
		}
	}
	if( size_t(pd) < vresult.size() ) {
		return vresult[size_t(pd)].c_str();
	}
	return "";
}

/*************************************************************************
	Constructor
*************************************************************************/
NetManager::NetManager(Agent* pAgent)
	: LuaExport< NetManager >("NetManager", pAgent->getLuaState())
	, m_pAgent(pAgent) 
	, m_pNetImplData(0)
	, m_nPacketIndex(0)
	, m_nLastSendHeart(0)
{
	assert(pAgent);

	m_pNetImplData = new NetImplData();

	m_pDHKey = new CDHKey(KEY_SIZE_IN_BIT);
	m_pDHKey->CreateKeyPair(m_InterKey);

	m_PrivateKey[0]='\0';
}

/*************************************************************************
	Destructor
*************************************************************************/
NetManager::~NetManager()
{
	delete m_pNetImplData; m_pNetImplData = 0;

	SAFE_DELETE(m_pDHKey);
}

/*************************************************************************
	Init netmanager
*************************************************************************/
void NetManager::init(void)
{
	//Init winsocket
	WSADATA wsaData;
	if(0!=WSAStartup( MAKEWORD( 2, 2 ), &wsaData)  ||
		(LOBYTE( wsaData.wVersion ) != 2 ||	HIBYTE( wsaData.wVersion ) != 2 ) )
	{
		WSACleanup();
		Util::throwException("Could not find a usable WinSock DLL!");
		return;
	}

	m_pNetImplData->m_PacketFactoryManager.Init();

	// register lua function
	RegisterFunction("ConnectToServer", Lua_ConnectServer);
	RegisterFunction("SendMessage", Lua_SendMessage);
}

/*************************************************************************
	Init netmanager
*************************************************************************/
void NetManager::release(void)
{
	m_pNetImplData->m_Socket.close();
	WSACleanup();
}

/*************************************************************************
	Init netmanager
*************************************************************************/
void NetManager::tick(float fElapsTime)
{
	//process stream
	processSocketStream();

	//process input packet
	processInputPackets();

	//send heart beat
	_sendHeartBeat();
}

/*************************************************************************
	Process socket stream
*************************************************************************/
void NetManager::processSocketStream(void)
{
	if(!(m_pNetImplData->m_Socket.isValid()))
	{
		return;
	}

	if( m_pNetImplData->m_Socket.isSockError() )
	{
		if( m_pAgent->getVariable() )
		{
			Util::throwException(m_strNetError.c_str());
		}
	}

	//socket stream
	if(!_select() || !_processExcept() || !_processInput() || !_processOutput())
	{
		Util::throwException(m_strNetError.c_str());
	}
}

/*************************************************************************
	socket select
*************************************************************************/
bool NetManager::_select(void)
{
	FD_ZERO(&(m_pNetImplData->m_ReadFD));
	FD_ZERO(&(m_pNetImplData->m_WriteFD));
	FD_ZERO(&(m_pNetImplData->m_ExceptFD));

	FD_SET(m_pNetImplData->m_Socket.getSOCKET(), &(m_pNetImplData->m_ReadFD));
	FD_SET(m_pNetImplData->m_Socket.getSOCKET(), &(m_pNetImplData->m_WriteFD));
	FD_SET(m_pNetImplData->m_Socket.getSOCKET(), &(m_pNetImplData->m_ExceptFD));
	
	timeval	Timeout ;
	Timeout.tv_sec = 0 ;
	Timeout.tv_usec = 0 ;

	if(SOCKET_ERROR == SocketAPI::select_ex(0 , 
										&(m_pNetImplData->m_ReadFD), 
										&(m_pNetImplData->m_WriteFD), 
										&(m_pNetImplData->m_ExceptFD), 
										&Timeout ) )
	{
		m_strNetError = "Socket Select error";
		return false;
	}
	return true;
}

/*************************************************************************
	Fill Input socket stream
*************************************************************************/
bool NetManager::_processInput(void)
{
	if(FD_ISSET(m_pNetImplData->m_Socket.getSOCKET(), &(m_pNetImplData->m_ReadFD)))
	{
		unsigned int ret = m_pNetImplData->m_SocketInputStream.Fill();
		if((int)ret <= SOCKET_ERROR)
		{
			char szTemp[MAX_PATH];
			_snprintf(szTemp, MAX_PATH, "m_SocketInputStream.Fill ret %d", (int)ret);
			m_strNetError = szTemp;

			m_pNetImplData->m_Socket.close() ;
			return false;
		}
	}
	return true;
}

/*************************************************************************
	Send Out socket stream
*************************************************************************/
bool NetManager::_processOutput(void)
{
	if(FD_ISSET(m_pNetImplData->m_Socket.getSOCKET(), &(m_pNetImplData->m_WriteFD)))
	{
		unsigned int ret = m_pNetImplData->m_SocketOutputStream.Flush();
		if((int)ret <= SOCKET_ERROR)
		{
			char szTemp[MAX_PATH];
			_snprintf(szTemp, MAX_PATH, "m_SocketOutputStream.Flush ret %d", (int)ret);
			m_strNetError = szTemp;

			m_pNetImplData->m_Socket.close();
			return false;
		}
	}
	return true;
}

/*************************************************************************
	Check is any exception
*************************************************************************/
bool NetManager::_processExcept(void)
{
	if(FD_ISSET(m_pNetImplData->m_Socket.getSOCKET(), &(m_pNetImplData->m_ExceptFD)))
	{
		m_pNetImplData->m_Socket.close() ;
		return false;
	}
	return true;
}

/*************************************************************************
	Process input Packets
*************************************************************************/
void NetManager::processInputPackets(void)
{
	char header[PACKET_HEADER_SIZE];
	PacketID_t packetID;
	uint packetuint, packetSize, packetIndex;
	Packet* pPacket = NULL;

	{
		while(true)
		{
			if(!(m_pNetImplData->m_SocketInputStream.Peek(&header[0], PACKET_HEADER_SIZE)))
			{
				// head baked
				break;
			}

			memcpy(&packetID, &header[0], sizeof(PacketID_t) );	
			memcpy(&packetuint, &header[sizeof(PacketID_t)], sizeof(uint) );
			packetSize = GET_PACKET_LEN(packetuint);
			packetIndex = GET_PACKET_INDEX(packetuint);

			if( packetID >= (PacketID_t)Packets::PACKET_MAX )
			{
				//Invalid socket id
				Util::throwException("Invalid Packet[%d]!", packetID);

				return;
			}

			{
				if(m_pNetImplData->m_SocketInputStream.Length()<PACKET_HEADER_SIZE+packetSize)
				{
					//msg baked
					break;
				}

				UINT nSize = m_pNetImplData->m_PacketFactoryManager.GetPacketMaxSize(packetID);
				if( packetSize > nSize )
				{
					//Msg size error
					Util::throwException("Packet size error![ID=%d](%d>%d)", 
						packetID, packetSize, m_pNetImplData->m_PacketFactoryManager.GetPacketMaxSize(packetID));
					return;
				}

#ifdef _DEBUG
				//const char* pPacketDesc = GetPacketDefine( packetID );
				//if( pPacketDesc && strlen( pPacketDesc ) > 0 ) {
				//	m_pAgent->getInfo()->addLogFmt( "接收<<<<[%s]", pPacketDesc );
				//}
#endif // _DEBUG

				Packet* pPacket = m_pNetImplData->m_PacketFactoryManager.CreatePacket(packetID);
				assert(pPacket);
				if(pPacket==NULL)
				{
					//Memory alloc error
					Util::throwException("Create Packet error[%d]!", packetID);

					return;
				}

				//Packet index
				pPacket->SetPacketIndex(packetIndex);

				BOOL ret = m_pNetImplData->m_SocketInputStream.ReadPacket(pPacket);
				if(ret==FALSE)
				{
					//read packet 
					Util::throwException("Read Packet error!");
					continue;
				}

				//execute packet
				ret = pPacket->Execute((Player*)m_pAgent);
				if(ret != PACKET_EXE_NOTREMOVE)
				{
					m_pNetImplData->m_PacketFactoryManager.RemovePacket(pPacket);
				}
			}
		}
	}
}

/*************************************************************************
	Connect to a server
*************************************************************************/
void NetManager::sendPacket(Packet* pPacket)
{
	if(!pPacket) return;

	if(m_pNetImplData->m_Socket.isValid())
	{
		unsigned int nSizeBefore = m_pNetImplData->m_SocketOutputStream.Length();
		pPacket->SetPacketIndex(m_nPacketIndex++);
		m_pNetImplData->m_SocketOutputStream.WritePacket( pPacket ) ;
		unsigned int nSizeAfter = m_pNetImplData->m_SocketOutputStream.Length();

		if(pPacket->GetPacketSize() != nSizeAfter-nSizeBefore-6)
		{
			Util::throwException("Packet size error[id=%d, stream=%d, size=%d]",
				pPacket->GetPacketID(), nSizeAfter-nSizeBefore-6, pPacket->GetPacketSize());
		}

		if( m_pNetImplData->m_Socket.isSockError() )
		{
			if( m_pAgent->getVariable() )
			{
				Util::throwException(m_strNetError.c_str());
			}
		}
	}
}

/*************************************************************************
	Send heart beat on time
*************************************************************************/
void NetManager::_sendHeartBeat(void)
{
	const UINT HEART_BEAT_RATE = 60*1000;		// 1 Minutes

	unsigned int nTimeNow = timeGetTime();
	if(nTimeNow-m_nLastSendHeart < HEART_BEAT_RATE) return;

	m_nLastSendHeart = nTimeNow;

	std::string strServer;
	m_pAgent->getVariable()->getVariable( "ConnectServer", strServer );
	if(strServer == "GameServer")
	{
		MsgSend_HeartBeat(m_pAgent);
	}
}

/*************************************************************************
	Connect to a server
*************************************************************************/
int NetManager::Lua_ConnectServer(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		Util::throwException("NetManager::ConnectServer[2] param error");
	}
	if(!(args[3].IsInteger()))
	{
		Util::throwException("NetManager::ConnectServer[3] param error (%s)",
			args[2].GetString());
	}
	if(!(args[4].IsString()))
	{
		Util::throwException("NetManager::ConnectServer[4] param error (%s,%d)",
			args[2].GetString(), args[3].GetInteger());
	}

	//Close SOCKET
	m_pNetImplData->m_Socket.close();
	m_pAgent->getVariable()->setVariable("ConnectServer", "NONE" );

	//Create new SOCKET
	if(!(m_pNetImplData->m_Socket.create()))
	{
		state->PushInteger(-1);
		return 1;
	}

	//Connect to server(Block)
	std::string strServer = args[2].GetString();
	int nPort = args[3].GetInteger();
	if(!(m_pNetImplData->m_Socket.connect(strServer.c_str(), nPort)))
	{
		m_pNetImplData->m_Socket.close();
		state->PushInteger(-2);
		return 1;
	}

	//reset packet index
	m_nPacketIndex = 123;

	strServer = args[4].GetString();
	m_pAgent->getVariable()->setVariable( "ConnectServer", strServer.c_str() );

	state->PushInteger(0);
	return 1;
}

/*************************************************************************
	Connect to a server
*************************************************************************/
int NetManager::Lua_SendMessage(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	static int delete_char = 0;

	if(!(args[2].IsString()))
	{
		Util::throwException("NetManager::SendMessage[2] param parameter error");
	}

	const char* szMsgName = args[2].GetString();
	PacketID_t packetID;

	std::string strServer;
	m_pAgent->getVariable()->getVariable( "ConnectServer", strServer );
	if( strServer.at(0) != szMsgName[1] )
	{
		Util::throwException("NetManager::SendMessage Send %s to %s", szMsgName, strServer.c_str());
	}

	if(strcmp(szMsgName, "CGHeartBeat") == 0)
	{
		packetID = MsgSend_HeartBeat( m_pAgent );
	}
	else if(strcmp(szMsgName, "CLConnect") == 0)
	{
		packetID = MsgSend_CLConnect(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CLAskLogin") == 0)
	{
		packetID = MsgSend_CLAskLogin(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CLAskCharList") == 0)
	{
		packetID = MsgSend_CLAskCharList(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CLAskCharLogin") == 0)
	{
		packetID = MsgSend_CLAskCharLogin(m_pAgent);
	}
	else if(strcmp(szMsgName, "CLAskCreateChar") == 0)
	{
		packetID = MsgSend_CLAskCreateChar(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CLAskDeleteChar") == 0)
	{
		packetID = MsgSend_CLAskDeleteChar(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGConnect") == 0)
	{
		packetID = MsgSend_CGConnect(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGEnterScene") == 0)
	{
		packetID = MsgSend_CGEnterScene(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGAskChangeScene") == 0)
	{
		packetID = MsgSend_CGAskChangeScene(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGEnvRequest") == 0)
	{
		packetID = MsgSend_CGEnvRequest(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGCharAskBaseAttrib") == 0)
	{
		packetID = MsgSend_CGCharAskBaseAttrib(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGAskDetailAttrib") == 0)
	{
		packetID = MsgSend_CGAskDetailAttrib(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGAskEquipment") == 0)
	{
		packetID = MsgSend_CGAskEquipment(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGAskDetailAbilityInfo") == 0)
	{
		packetID = MsgSend_CGAskDetailAbilityInfo(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGAskDetailXinFaList") == 0)
	{
		packetID = MsgSend_CGAskDetailXinFaList(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGAskDetailSkillList") == 0)
	{
		packetID = MsgSend_CGAskDetailSkillList(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGAskTeamInfo") == 0)
	{
		packetID = MsgSend_CGAskTeamInfo(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGAskCityAttr") == 0)
	{
		packetID = MsgSend_CGAskCityAttr(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGAskMinorPasswd") == 0)
	{
		packetID = MsgSend_CGAskMinorPasswd(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGAskDetailEquipList") == 0)
	{
		packetID = MsgSend_CGAskDetailEquipList(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGAskMyBagList") == 0)
	{
		packetID = MsgSend_CGAskMyBagList(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGAskMissionList") == 0)
	{
		packetID = MsgSend_CGAskMissionList(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGAskCharAllTitles") == 0)
	{
		packetID = MsgSend_CGAskCharAllTitles(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGAskSetting") == 0)
	{
		packetID = MsgSend_CGAskSetting(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGAskRelation") == 0)
	{
		packetID = MsgSend_CGAskRelation(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGAskMail") == 0)
	{
		packetID = MsgSend_CGAskMail(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGAskBankMoney") == 0)
	{
		packetID = MsgSend_CGAskBankMoney(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGStallApply") == 0)
	{
		packetID = MsgSend_CGStallApply(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGStallEstablish") == 0)
	{
		packetID = MsgSend_CGStallEstablish(m_pAgent, args);
	}
	else if(strcmp(szMsgName, "CGCommand") == 0)
	{
		packetID = MsgSend_CGCommand(m_pAgent, args);
	}

#ifdef _DEBUG
	//const char* pPacketDesc = GetPacketDefine( packetID );
	//if( pPacketDesc && strlen( pPacketDesc ) > 0 ) {
	//	m_pAgent->getInfo()->addLogFmt( "发送>>>>[%s]", pPacketDesc );
	//}
#endif // _DEBUG

	return 0;
}
BOOL NetManager::EnableEncrypto(VOID)
{
	Assert(m_pNetImplData);
	
	m_pNetImplData->m_SocketInputStream.SetKey(m_PrivateKey);
	m_pNetImplData->m_SocketInputStream.SetKeySize(KEY_SIZE_IN_BYTE);
	m_pNetImplData->m_SocketInputStream.SetEncoderID(0);
	
	m_pNetImplData->m_SocketOutputStream.SetKey(m_PrivateKey);
	m_pNetImplData->m_SocketOutputStream.SetKeySize(KEY_SIZE_IN_BYTE);
	m_pNetImplData->m_SocketOutputStream.SetEncoderID(0);
	
	return TRUE;
}
VOID NetManager::KeyAgreement(UCHAR* pInterKey, INT nInterKeySize)
{
	Assert(m_pDHKey);
	Assert(pInterKey);
	Assert(KEY_SIZE_IN_BYTE==nInterKeySize);
	CHAR szBuff[512];
	tsnprintf(szBuff, 
		sizeof(szBuff), 
		"InterKey: %2X,%2X,%2X,%2X,%2X,%2X,%2X,%2X\n",
		pInterKey[0],
		pInterKey[1],
		pInterKey[2],
		pInterKey[3],
		pInterKey[4],
		pInterKey[5],
		pInterKey[6],
		pInterKey[7]
		);
	OutputDebugString(szBuff);
	
	m_pDHKey->Agree(m_PrivateKey, pInterKey);

	tsnprintf(szBuff, 
		sizeof(szBuff), 
		"PrivateKey: %2X,%2X,%2X,%2X,%2X,%2X,%2X,%2X\n",
		m_PrivateKey[0],
		m_PrivateKey[1],
		m_PrivateKey[2],
		m_PrivateKey[3],
		m_PrivateKey[4],
		m_PrivateKey[5],
		m_PrivateKey[6],
		m_PrivateKey[7]
		);
	OutputDebugString(szBuff);
}

}

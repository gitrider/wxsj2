/****************************************\
*										*
*		   NetWorldManager				*
*										*
*						 by jinchao		*
\****************************************/

#pragma once
#include "SMExportEnable.h"
#include "dhkey.h"

//Forward reference declarations
class Packet;
namespace SMITH
{
//Forward reference declarations
class Agent;
struct NetImplData;

class NetManager : public LuaExport< NetManager >
{
public:
	/*************************************************************************
		Public Interface
	*************************************************************************/

	/*!
	\brief
		Init manager
	\return
	*/
	void init(void);
	/*!
	\brief
		Release manager
	\return
	*/
	void release(void);
	/*!
	\brief
		Tickwork
	\return
	*/
	void tick(float fElapsTime);
	/*!
	\brief
		Send msg packet.
	\return
	*/
	void sendPacket(Packet* pPacket);

public:
	/*************************************************************************
		Lua Interface
	*************************************************************************/
	/*!
	\brief
		Connect to a server

		param0[string]	- server address.
		param1[int]		- server port.

		return - error code.
	\return
		None.
	*/
	int Lua_ConnectServer(LuaPlus::LuaState* state);

	/*!
	\brief
		Send a message

		param0[string]	- Msg name
		...				- Extra param

	\return
		None.
	*/
	int Lua_SendMessage(LuaPlus::LuaState* state);

protected:
	/*************************************************************************
		Implementation Interface
	*************************************************************************/
	
	/*!
	\brief
		socket select
	\return 
		Is success
	*/
	bool _select(void);

	/*!
	\brief
		Check is any exception
	\return 
		Is success
	*/
	bool _processExcept(VOID);
	/*!
	\brief
		Fill Input socket stream
	\return 
		Is success
	*/
	bool _processInput(void);

	/*!
	\brief
		Send Out socket stream
	\return 
		Is success
	*/
	bool _processOutput(VOID);

	/*!
	\brief
		Process SocketStream(Input and Output)
	\return
	*/
	void processSocketStream(void);

	/*!
	\brief
		Process Input packet(Input and Output)
	\return
	*/
	void processInputPackets(void);

	/*!
	\brief
		Send heart beat on time

	\return
	*/
	void _sendHeartBeat(void);

protected:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	Agent*					m_pAgent;			//!< Owner.
	NetImplData*			m_pNetImplData;		//!< Impldata about net(SOCKET)
	std::string				m_strNetError;		//!< NetError message;
	int						m_nPacketIndex;		//!< Packet index fill to packet head.
	unsigned int			m_nLastSendHeart;	//!< The time of last send heart beat.

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	NetManager(Agent* pAgent);
	~NetManager();
public: //encrypt and decrypt
	enum
	{
		KEY_SIZE_IN_BIT = 64,		//密钥长度
		KEY_SIZE_IN_BYTE = 8,		//密钥长度
	};
	UCHAR const* GetInterKey(VOID) const {return m_InterKey;}
	UCHAR const* GetPrivateKey(VOID) const {return m_PrivateKey;}
	BOOL EnableEncrypto(VOID);
	VOID KeyAgreement(UCHAR* pInterKey, INT nInterKeySize);
private :
	UCHAR					m_InterKey[KEY_SIZE_IN_BYTE];
	UCHAR					m_PrivateKey[KEY_SIZE_IN_BYTE];
	CDHKey*					m_pDHKey;
};


}
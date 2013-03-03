// CGCExchange.h
// 
// 密钥交换
// 
//////////////////////////////////////////////////////

#ifndef __KEYEXCHANGE_H__
#define __KEYEXCHANGE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
class KeyExchange : public Packet
{
public:
	enum
	{
		MAX_BUFFER_SIZE =1024,
	};
	KeyExchange() : m_nKeySize(0)
	{
		m_szKey[0] = '\0';
	}
	virtual ~KeyExchange(){}

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_KEY_EXCHANGE; }
	virtual UINT			GetPacketSize()const {return sizeof(m_nKeySize)+sizeof(UCHAR)*m_nKeySize;}
	
public:
	//使用数据接口
	VOID Reset(VOID) {m_nKeySize = 0; m_szKey[0] = '\0';}
	UCHAR* GetKey(VOID) {return m_szKey;}
	VOID SetKey(UCHAR const* szKey, INT nKeySize);
	INT GetKeySize(VOID) const {return m_nKeySize;}
private:
	//数据
	UCHAR m_szKey[MAX_BUFFER_SIZE];
	SHORT m_nKeySize;
};

class KeyExchangeFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new KeyExchange() ; }
	PacketID_t	GetPacketID()const { return PACKET_KEY_EXCHANGE; }
	UINT		GetPacketMaxSize()const { return sizeof(SHORT) + sizeof(UCHAR)* KeyExchange::MAX_BUFFER_SIZE; }
};

class KeyExchangeHandler 
{
public:
	static UINT Execute( KeyExchange* pPacket, Player* pPlayer ) ;
};

}

using namespace Packets;



#endif //__KEYEXCHANGE_H__
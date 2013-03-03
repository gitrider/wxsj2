

#ifndef __CGPACKUPPACKET_H__
#define __CGPACKUPPACKET_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{

class CGPackUpPacket : public Packet 
{
public:
	CGPackUpPacket( ){} ;
	virtual ~CGPackUpPacket( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_PACKUP_PACKET ; }
	virtual UINT			GetPacketSize()const { return	sizeof(UINT); }
	
public:
	//ʹ�����ݽӿ�
	INT			GetConIndex() {return m_nConIndex;}
	VOID		SetConIndex(INT nConIndex) {m_nConIndex = nConIndex;}
		
private:
	//����
	INT			m_nConIndex;

};


class CGPackUpPacketFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGPackUpPacket() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_PACKUP_PACKET ; }
	UINT		GetPacketMaxSize()const { return	sizeof(UINT); }
};


class CGPackUpPacketHandler 
{
public:
	static UINT Execute( CGPackUpPacket* pPacket, Player* pPlayer ) ;
};


};

using namespace Packets ;



#endif

// GCAskActiveTimeUpdate.h
// 
// ��ǰ���ǵļ�����Чʱ������
// 
//////////////////////////////////////////////////////

#ifndef __GCASKACTIVETIMEUPDATE_H__
#define __GCASKACTIVETIMEUPDATE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
class CGAskActiveTimeUpdate : public Packet
{
public:
	CGAskActiveTimeUpdate( ):m_nActiveTime(INVALID_ID)	{};
	virtual ~CGAskActiveTimeUpdate( ){};

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASK_ACTIVETIME_UPDATE ; }
	virtual UINT			GetPacketSize()const { return	sizeof(SHORT);}

public:
	SHORT			ActiveTimeID(VOID) const {return m_nActiveTime;};
	VOID			ActiveTimeID(SHORT const nID) {m_nActiveTime = nID;};
private:
	SHORT			m_nActiveTime;
};


class CGAskActiveTimeUpdateFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGAskActiveTimeUpdate() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_ASK_ACTIVETIME_UPDATE; };
	UINT		GetPacketMaxSize()const { return	sizeof(SHORT);};
};

class CGAskActiveTimeUpdateHandler 
{
public:
	static UINT Execute( CGAskActiveTimeUpdate* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif



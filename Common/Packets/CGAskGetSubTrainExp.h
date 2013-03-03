

#ifndef __CGASKGETSUBTRAINEXP_H__
#define __CGASKGETSUBTRAINEXP_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGAskGetSubTrainExp : public Packet 
	{
	public:
		CGAskGetSubTrainExp( ){} ;
		virtual ~CGAskGetSubTrainExp( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASKGETSUBTRAINEXP ; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE); }

	public :
		BYTE	m_nMode;
	};


	class CGAskGetSubTrainExpFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGAskGetSubTrainExp() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_ASKGETSUBTRAINEXP ; }
		UINT		GetPacketMaxSize()const { return sizeof(BYTE); }
	};


	class CGAskGetSubTrainExpHandler 
	{
	public:
		static UINT Execute( CGAskGetSubTrainExp* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif
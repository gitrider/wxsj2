

#ifndef __CGASKBEGINSUBTRAIN_H__
#define __CGASKBEGINSUBTRAIN_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGAskBeginSubTrain: public Packet 
	{
	public:
		CGAskBeginSubTrain( ){} ;
		virtual ~CGAskBeginSubTrain( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASKBEGINSUBTRAIN ; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE); }

	public :
		//使用数据接口
		BYTE					getMode(VOID) { return m_nMode; }
		VOID					setMode(BYTE Mode) { m_nMode = Mode; }

	private:
		BYTE	m_nMode;
	};


	class CGAskBeginSubTrainFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGAskBeginSubTrain() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_ASKBEGINSUBTRAIN ; }
		UINT		GetPacketMaxSize()const { return sizeof(BYTE) ; }
	};


	class CGAskBeginSubTrainHandler 
	{
	public:
		static UINT Execute( CGAskBeginSubTrain* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
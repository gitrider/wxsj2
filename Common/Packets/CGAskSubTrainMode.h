

#ifndef __CGASKSUBTRAINMODE_H__
#define __CGASKSUBTRAINMODE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGAskSubTrainMode : public Packet 
	{
	public:
		CGAskSubTrainMode( ){} ;
		virtual ~CGAskSubTrainMode( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASKSUBTRAINMODE ; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE); }

	public :
		//使用数据接口
		BYTE					getMode(VOID) { return m_nMode; }
		VOID					setMode(BYTE Mode) { m_nMode = Mode; }

	private:
		BYTE	m_nMode;
	};


	class CGAskSubTrainModeFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGAskSubTrainMode() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_ASKSUBTRAINMODE ; }
		UINT		GetPacketMaxSize()const { return sizeof(BYTE); }
	};


	class CGAskSubTrainModeHandler 
	{
	public:
		static UINT Execute( CGAskSubTrainMode* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif
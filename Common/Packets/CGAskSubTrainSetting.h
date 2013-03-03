

#ifndef __CGASKSUBTRAINSETTING_H__
#define __CGASKSUBTRAINSETTING_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGAskSubTrainSetting : public Packet 
	{
	public:
		CGAskSubTrainSetting( ){} ;
		virtual ~CGAskSubTrainSetting( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASKSUBTRAINSETTING ; }
		virtual UINT			GetPacketSize()const { return 0 ; }

	public :
	};


	class CGAskSubTrainSettingFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGAskSubTrainSetting() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_ASKSUBTRAINSETTING ; }
		UINT		GetPacketMaxSize()const { return 0 ; }
	};


	class CGAskSubTrainSettingHandler 
	{
	public:
		static UINT Execute( CGAskSubTrainSetting* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
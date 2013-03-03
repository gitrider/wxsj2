/////////////////////////////////////////////////////////////////////////////////
//
//	文件名：
//		WLChangeName.h
//	功能描述：
//		角色、帮会等更名操作相关
//	修改记录：
//		Aug.13 2007 - Create by Lybin Lee
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __WLCHANGENAME_H__
#define __WLCHANGENAME_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "PacketStruct.h"

namespace Packets
{
	class WLChangeName : public Packet 
	{
	public:
		WLChangeName( )
		{
		}
		virtual ~WLChangeName( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_WL_CHANGENAME ; }
		virtual UINT			GetPacketSize() const
		{
			return m_sPacketUnit.GetPacketSize() ;
		}

	public:
		SChgNamePacketUnit		m_sPacketUnit ;
	};

	class WLChangeNameFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WLChangeName() ; }
		PacketID_t	GetPacketID() const { return PACKET_WL_CHANGENAME ; }
		UINT		GetPacketMaxSize() const
		{
			return SChgNamePacketUnit::GetPacketSize() ;
		}
	};

	class WLChangeNameHandler 
	{
	public:
		static UINT Execute( WLChangeName* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif

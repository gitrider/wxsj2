// CGCityAskAttr.h
// 
// ������������������
// 
//////////////////////////////////////////////////////

#ifndef __CGCITYASKATTR_H__
#define __CGCITYASKATTR_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGCityAskAttr : public Packet 
	{
	public:
		CGCityAskAttr( ){} ;
		virtual ~CGCityAskAttr( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_CITYASKATTR; }
		virtual UINT			GetPacketSize() const { return 0; }

	public:
		//ʹ�����ݽӿ�

	private:

	};

	class CGCityAskAttrFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGCityAskAttr() ; }
		PacketID_t	GetPacketID() const { return PACKET_CG_CITYASKATTR ; }
		UINT		GetPacketMaxSize() const { return 0; }
	};

	class CGCityAskAttrHandler 
	{
	public:
		static UINT Execute( CGCityAskAttr* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif

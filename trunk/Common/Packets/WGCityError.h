// WGCityError.h
// 
// ���г�����Ϣ
// 
//////////////////////////////////////////////////////

#ifndef __WGCITYERROR_H__
#define __WGCITYERROR_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGCityError : public Packet 
	{
	public:
		WGCityError( ){} ;
		virtual ~WGCityError( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_WG_CITYERROR; }
		virtual UINT			GetPacketSize() const { return sizeof(BYTE); }

	public:
		//ʹ�����ݽӿ�
		BYTE					GetErrorID(){return m_ErrorID;}
		VOID					SetErrorID(BYTE nErrorID){m_ErrorID = nErrorID;}

	private:
		//����
		BYTE					m_ErrorID;

	};

	class WGCityErrorFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WGCityError() ; }
		PacketID_t	GetPacketID() const { return PACKET_WG_CITYERROR; }
		UINT		GetPacketMaxSize() const { return sizeof(BYTE); }
	};

	class WGCityErrorHandler 
	{
	public:
		static UINT Execute( WGCityError* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif

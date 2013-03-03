/////////////////////////////////////////////////////////////////////////////////
//
//	�ļ�����
//		LWChangeName.h
//	����������
//		��ɫ�����ȸ����������
//	�޸ļ�¼��
//		Aug.13 2007 - Create by Lybin Lee
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __LWCHANGENAME_H__
#define __LWCHANGENAME_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "PacketStruct.h"

namespace Packets
{
	class LWChangeName : public Packet 
	{
	public:
		LWChangeName( )
		{
		}
		virtual ~LWChangeName( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LW_CHANGENAME ; }
		virtual UINT			GetPacketSize() const
		{
			return m_sPacketUnit.GetPacketSize() ;
		}

	public:
		SChgNamePacketUnit		m_sPacketUnit ;
	};

	class LWChangeNameFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LWChangeName() ; }
		PacketID_t	GetPacketID() const { return PACKET_LW_CHANGENAME ; }
		UINT		GetPacketMaxSize() const
		{
			return SChgNamePacketUnit::GetPacketSize() ;
		}
	};

	class LWChangeNameHandler 
	{
	public:
		static UINT Execute( LWChangeName* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif

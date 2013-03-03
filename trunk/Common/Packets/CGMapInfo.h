/********************************************************************************
 *	�ļ�����	CGMapInfo.h

 *	�����ˣ�	�����
 *	����ʱ�䣺	2008 �� 4 �� 16 ��	
 *
 *	����˵����	�ͻ������������˷��͵������ͼ(������ת)��Ϣ
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef __CG_MAPINFO_H__
#define __CG_MAPINFO_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGMapInfo : public Packet
	{
	public:
		CGMapInfo(){}
		virtual					~CGMapInfo(){}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_MAP_INFO; }

		virtual UINT			GetPacketSize() const
		{
			UINT uSize = sizeof(m_Index);
			return uSize;
		}

		VOID					SetIndex(INT index){ m_Index = index; }
		INT						GetIndex()			{ return m_Index; }

	private:
		INT						m_Index;		//������תʱ������

	};

	class CGMapInfoFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new CGMapInfo(); }
		PacketID_t				GetPacketID() const { return PACKET_CG_MAP_INFO; }
		UINT					GetPacketMaxSize() const { return sizeof(INT); }
	};

	class CGMapInfoHandler
	{
	public:
		static UINT Execute( CGMapInfo* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif // __CG_GUILD_H__

/********************************************************************************
 *	�����ˣ�	���ı�
 *	����ʱ�䣺	2008 �� 5 �� 20 ��	
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef __CGQueryHorseFindMateInfo_H__
#define __CGQueryHorseFindMateInfo_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGQueryHorseFindMateInfo : public Packet
	{
	public:
		CGQueryHorseFindMateInfo(){}
		virtual					~CGQueryHorseFindMateInfo(){}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_QUERYHORSEFINDMATEINFO; }

		virtual UINT			GetPacketSize() const
		{
			return sizeof(m_iMaxPostion);
		}

		INT					GetMaxPostion(){return m_iMaxPostion;}
		VOID				SetMaxPostion(INT value){m_iMaxPostion = value;}

	private:
		
		INT	m_iMaxPostion; //�ϴβ�ѯʱ��Users�е����Index
	};

	class CGQueryHorseFindMateInfoFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new CGQueryHorseFindMateInfo(); }
		PacketID_t				GetPacketID() const { return PACKET_CG_QUERYHORSEFINDMATEINFO; }
		UINT					GetPacketMaxSize() const { return sizeof(INT); }
	};

	class CGQueryHorseFindMateInfoHandler
	{
	public:
		static UINT Execute( CGQueryHorseFindMateInfo* pPacket, Player* pPlayer );
	};

}

#endif // __CG_GUILD_H__

/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 20 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#ifndef __CGCancelHorseFindMateInfo_H__
#define __CGCancelHorseFindMateInfo_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGCancelHorseFindMateInfo : public Packet
	{
	public:
		CGCancelHorseFindMateInfo(){}
		virtual					~CGCancelHorseFindMateInfo(){}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_CANCELHORSEFINDMATEINFO; }

		virtual UINT			GetPacketSize() const
		{
			return sizeof(m_GUID);
		}

		PET_GUID_t		GetGUID( VOID )const{ return m_GUID; }
		VOID			SetGUID( PET_GUID_t GUID ){ m_GUID = GUID; }

	private:
		PET_GUID_t		m_GUID;							// ID

	};

	class CGCancelHorseFindMateInfoFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new CGCancelHorseFindMateInfo(); }
		PacketID_t				GetPacketID() const { return PACKET_CG_CANCELHORSEFINDMATEINFO; }
		UINT					GetPacketMaxSize() const { return sizeof(PET_GUID_t); }
	};

	class CGCancelHorseFindMateInfoHandler
	{
	public:
		static UINT Execute( CGCancelHorseFindMateInfo* pPacket, Player* pPlayer );
	};

}


#endif // __CG_GUILD_H__

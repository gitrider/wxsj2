/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 20 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/


#ifndef __CGIssueHorseFindMateInfo_H__
#define __CGIssueHorseFindMateInfo_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGIssueHorseFindMateInfo : public Packet
	{
	public:
		CGIssueHorseFindMateInfo(){}
		virtual					~CGIssueHorseFindMateInfo(){}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_ISSUEHORSEFINDMATEINFO; }

		virtual UINT			GetPacketSize() const
		{
			return sizeof(m_GUID) + sizeof(m_szLeaveWords);
		}

		PET_GUID_t		GetGUID( VOID )const{ return m_GUID; }
		VOID			SetGUID( PET_GUID_t GUID ){ m_GUID = GUID; }
		VOID			SetLeaveWords( const CHAR *pszName ) {
			tsnprintf_s(m_szLeaveWords, MAX_BBS_MESSAGE_LENGTH-1, "%s", pszName);
		}
		const CHAR		*GetLeaveWords( VOID )const{ return m_szLeaveWords; }

	private:
		PET_GUID_t		m_GUID;							// ID
		CHAR			m_szLeaveWords[MAX_BBS_MESSAGE_LENGTH];

	};

	class CGIssueHorseFindMateInfoFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new CGIssueHorseFindMateInfo(); }
		PacketID_t				GetPacketID() const { return PACKET_CG_ISSUEHORSEFINDMATEINFO; }
		UINT					GetPacketMaxSize() const { 
			return sizeof(PET_GUID_t) + sizeof(CHAR)*MAX_BBS_MESSAGE_LENGTH; 
		}
	};

	class CGIssueHorseFindMateInfoHandler
	{
	public:
		static UINT Execute( CGIssueHorseFindMateInfo* pPacket, Player* pPlayer );
	};

}


#endif // __CG_GUILD_H__

//by soongxl
#ifndef _CG_MODIFY_PKMODE_H_
#define _CG_MODIFY_PKMODE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameStruct_Country.h"

namespace Packets
{
	class CGModifyPKMode:	public Packet
	{
	public:
		CGModifyPKMode() { mCampMode = 0xFF; }
		virtual ~CGModifyPKMode() {}

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_MODIFY_PK_MODE; }
		virtual UINT			GetPacketSize() const
		{
			return sizeof(BYTE);
		}

	public :
		VOID					SetPKMode( CAMP_ENUM mode ) { mCampMode = (BYTE)mode; }
		CAMP_ENUM				GetPKMode( ) { return (CAMP_ENUM)mCampMode; }
	public :
		BYTE mCampMode;
	};

	class CGModifyPKModeFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGModifyPKMode(); }
		PacketID_t	GetPacketID()const { return PACKET_CG_MODIFY_PK_MODE; }
		UINT		GetPacketMaxSize()const { return sizeof(BYTE); }
	};

	class CGModifyPKModeHandler
	{
	public:
		static UINT Execute(CGModifyPKMode* pPacket,Player* pPlayer) ;
	};

}

using namespace Packets;

#endif // _WG_TEAM_LIST_H_

// GCCharMoveResult.h
// 
//////////////////////////////////////////////////////////

#ifndef __GCCHARMOVERESULT_H__
#define __GCCHARMOVERESULT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCCharMoveResult : public Packet
	{
	public:
		GCCharMoveResult( ){} ;
		virtual ~GCCharMoveResult( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHARMOVERESULT; }
		virtual UINT			GetPacketSize()const
		{
			return sizeof(m_nHandleID) + sizeof(m_nResult);
		}

	public:
		VOID				setHandleID( INT nID ) { m_nHandleID = nID; }
		INT					getHandleID( VOID )const{ return m_nHandleID; }

		VOID				setResult( INT nResult ) { m_nResult = nResult; }
		INT					getResult( VOID )const{ return m_nResult; }

	private:
		INT					m_nHandleID;	// 操作ID
		INT					m_nResult;		// ORESULT
	};


	class GCCharMoveResultFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCCharMoveResult() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CHARMOVERESULT; }
		UINT		GetPacketMaxSize()const { return sizeof(GCCharMoveResult) - sizeof(Packet); }
	};

	class GCCharMoveResultHandler 
	{
	public:
		static UINT Execute( GCCharMoveResult* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif // __GCCHARMOVERESULT_H__

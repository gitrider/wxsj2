#ifndef _CG_SOUXIA_DISCARD_H_
#define _CG_SOUXIA_DISCARD_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class CGSouXiaDiscard:public Packet
	{
	public:
		enum
		{
			FromBag = 0,
			FromBank,
		};

	public:
		CGSouXiaDiscard()
		{
			m_Opt	=	FromBag;
			m_BagIndex	=	0;
		};
		virtual				~CGSouXiaDiscard(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_SOUXIADISCARD; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)*2+ sizeof(UINT);
		}

	public:

		VOID			SetBagIndex(BYTE index){m_BagIndex = index;}
		BYTE			GetBagIndex(){return m_BagIndex;}

		VOID			SetOpt(BYTE Opt){m_Opt = Opt;}
		BYTE			GetOpt(){return m_Opt;}

		VOID			SetUniqueNpc(UINT objId){m_UniqueNPCId = objId;}
		UINT			GetUniqueNpc(){return m_UniqueNPCId;}

	private:
		BYTE			m_Opt;
		BYTE			m_BagIndex;
		UINT			m_UniqueNPCId;
	};


	class CGSouXiaDiscardFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGSouXiaDiscard(); }
		PacketID_t	GetPacketID() const { return PACKET_CG_SOUXIADISCARD; }
		UINT		GetPacketMaxSize() const { return sizeof(BYTE)*2 + sizeof(UINT); }
	};


	class CGSouXiaDiscardHandler
	{
	public:
		static UINT	Execute( CGSouXiaDiscard* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif //_CG_SOUXIA_DISCARD_H_

#ifndef __CGDISCLOSURETOGM_H__
#define __CGDISCLOSURETOGM_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGDisclosureToGM : public Packet 
	{
	public:
		CGDisclosureToGM( ){} ;
		virtual ~CGDisclosureToGM( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_DISCLOSURETOGM ; }
		virtual UINT			GetPacketSize() const 
		{ 
			UINT size =	m_disMsg.GetSize();
			return size;
		}

	public:
		//使用数据接口
		VOID		SetDisclosureMsg( DisclosureMsg* pDis ){ m_disMsg = *pDis ; }
		DisclosureMsg*		GetDisclosureMsg( ){ return &m_disMsg ; }

	private:
		//数据
		DisclosureMsg		m_disMsg ;

	};


	class CGDisclosureToGMFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGDisclosureToGM() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_DISCLOSURETOGM ; }
		UINT		GetPacketMaxSize()const { return	sizeof(DisclosureMsg) ; }
	};


	class CGDisclosureToGMHandler 
	{
	public:
		static UINT Execute( CGDisclosureToGM* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
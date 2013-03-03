

#ifndef __CGCHARALLTITLES_H__
#define __CGCHARALLTITLES_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class CGCharAllTitles : public Packet 
	{
	public:
		CGCharAllTitles( ){} ;
		virtual ~CGCharAllTitles( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_CHARALLTITLES ; }
		virtual UINT			GetPacketSize()const { return sizeof(ObjID_t) ; }

	public:
		//ʹ�����ݽӿ�
		ObjID_t					getTargetID(VOID) { return m_ObjID; }
		VOID					setTargetID(ObjID_t idTarget) { m_ObjID = idTarget; }

	private:
		//����
		ObjID_t					m_ObjID;	//�Է���ObjID
	};


	class CGCharAllTitlesFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGCharAllTitles() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_CHARALLTITLES ; }
		UINT		GetPacketMaxSize()const { return sizeof(ObjID_t) ; }
	};


	class CGCharAllTitlesHandler 
	{
	public:
		static UINT Execute( CGCharAllTitles* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif

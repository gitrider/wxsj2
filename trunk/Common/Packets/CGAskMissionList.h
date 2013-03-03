// CGAskMissionList.h
// 
// ����ˢ�������б�
// 
//////////////////////////////////////////////////////

#ifndef __CGASKMISSIONLIST_H__
#define __CGASKMISSIONLIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGAskMissionList : public Packet
	{
	public:
		CGAskMissionList( ){}
		virtual ~CGAskMissionList( ){}

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASKMISSIONLIST ; }
		virtual UINT			GetPacketSize()const { return sizeof(ObjID_t) ; }

	public:
		//ʹ�����ݽӿ�
		ObjID_t					getTargetID(VOID) { return m_ObjID; }
		VOID					setTargetID(ObjID_t idTarget) { m_ObjID = idTarget; }

	private:
		//����
		ObjID_t					m_ObjID;
	};


	class CGAskMissionListFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGAskMissionList() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_ASKMISSIONLIST; }
		UINT		GetPacketMaxSize()const { return sizeof(ObjID_t) ; }
	};

	class CGAskMissionListHandler 
	{
	public:
		static UINT Execute( CGAskMissionList* pPacket, Player* pPlayer ) ;
	};
}

#endif

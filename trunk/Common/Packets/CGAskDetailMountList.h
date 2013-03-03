// [7/8/2010 陈军龙]
#ifndef _CG_ASKMYMOUNT_LIST_H_
#define _CG_ASKMYMOUNT_LIST_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	enum ASK_MOUNT_MODE
	{
		ASK_MOUNT_ALL,
		ASK_MOUNT_SET
	};

	class CGAskDetailMountList : public Packet
	{
	public:
		CGAskDetailMountList()	{};
		virtual	~CGAskDetailMountList(){};


		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_ASKDETAILMOUNTLIST ; }
		virtual UINT			GetPacketSize() const { return	sizeof(ObjID_t); }

	public:
		ObjID_t					getObjID(VOID) { return m_ObjID; }
		VOID					setObjID(ObjID_t idObj) { m_ObjID = idObj; }

	private:
		ObjID_t					m_ObjID;	//对方的ObjID

	};

	class CGAskDetailMountListFactory:public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGAskDetailMountList() ; }
		PacketID_t	GetPacketID() const { return PACKET_CG_ASKDETAILMOUNTLIST; }
		UINT		GetPacketMaxSize() const { return	sizeof(ObjID_t); }
	};

	class CGAskDetailMountListHandler
	{
	public:
		static UINT	Execute( CGAskDetailMountList* pPacket, Player* pPlayer );
	};
}

using namespace Packets;
#endif


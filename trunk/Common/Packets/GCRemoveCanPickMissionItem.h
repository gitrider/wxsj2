// GCRemoveCanPickMissionItem.h
// 
// ɾ��һ������ʰȡ��������Ʒ
// 
//////////////////////////////////////////////////////

#ifndef __GCREMOVECANPICKMISSIONITEM_H__
#define __GCREMOVECANPICKMISSIONITEM_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class GCRemoveCanPickMissionItem :
		public Packet
	{
	public:
		GCRemoveCanPickMissionItem( ){
		}
		virtual ~GCRemoveCanPickMissionItem( ){}

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_REMOVECANPICKMISSIONITEM; }
		virtual UINT			GetPacketSize()const{ return sizeof( UINT ); }

	public:
		VOID			SetItemDataID( UINT uItemDataID ){ m_uItemDataID =uItemDataID; }
		UINT			GetItemDataID( VOID )const{ return m_uItemDataID; }

	private:
		UINT			m_uItemDataID;
	};

	class GCRemoveCanPickMissionItemFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCRemoveCanPickMissionItem() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_REMOVECANPICKMISSIONITEM; }
		UINT		GetPacketMaxSize()const { return sizeof(UINT); }
	};

	class GCRemoveCanPickMissionItemHandler 
	{
	public:
		static UINT Execute( GCRemoveCanPickMissionItem* pPacket, Player* pPlayer ) ;
	};
}

#endif // __GCREMOVECANPICKMISSIONITEM_H__

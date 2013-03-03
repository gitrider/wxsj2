#ifndef _GC_UNEQUIP_RESULT_H_
#define _GC_UNEQUIP_RESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"





namespace Packets
{
	class GCUnEquipResult:public Packet
	{

	public:
		GCUnEquipResult(){};
		virtual				~GCUnEquipResult(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_UNEQUIPRESULT; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)*3+sizeof(_ITEM_GUID)+sizeof(UINT);
		}

	public:
		VOID			setEquipPoint(BYTE index){m_EquipPoint = index;}
		BYTE			getEquipPoint(){return m_EquipPoint;}

		VOID			setBagIndex(BYTE index){m_BagIndex = index;}
		BYTE			getBagIndex(){return m_BagIndex;}

		VOID			setItemTableIndex(UINT index){m_ItemTableIndex = index;}
		UINT			getItemTableIndex(){return m_ItemTableIndex;}

		VOID			setItemID(_ITEM_GUID id){m_ItemId	=	id;}
		_ITEM_GUID		getItemID(){return m_ItemId;}

		VOID			setResult(BYTE	Ret){ m_Result = Ret;}
		BYTE			getResult(){return m_Result;}

	private:
		//by Jinchao.
		BYTE				m_EquipPoint;			//װ����

		BYTE				m_BagIndex;				//ж������װ����Bag�д�ŵ�λ��
		UINT				m_ItemTableIndex;		//BagIndex��װ������Դ��
		_ITEM_GUID			m_ItemId;				//BagIndex��װ����GUID
		BYTE				m_Result;				//�Ƿ�ɹ������ɹ����Ǵ����		

	};


	class GCUnEquipResultFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCUnEquipResult(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_UNEQUIPRESULT; }
		UINT		GetPacketMaxSize() const { return sizeof(BYTE)*3+sizeof(_ITEM_GUID)+sizeof(UINT); }
	};


	class GCUnEquipResultHandler
	{
	public:
		static UINT Execute( GCUnEquipResult* pPacket, Player* pPlayer );
	};



}


using namespace Packets;

#endif
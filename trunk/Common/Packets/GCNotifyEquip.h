#ifndef _GC_NOTIFYEQUIP_H_
#define _GC_NOTIFYEQUIP_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
	class GCNotifyEquip:public Packet
	{
    public:
        enum    
        {
            OPERATOR_BAG = 0,   //普通背包操作
            OPERATOR_BUY_BACK,  //回购栏操作
        };

	public:
		GCNotifyEquip()
        {
            m_OpType = 0;
        };
		virtual				~GCNotifyEquip(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_NOTIFYEQUIP; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(WORD) + GetItemVarSize(m_Item) + sizeof(INT) ;
		}

	public:
		VOID				SetBagIndex( WORD index ){ m_BagIndex = index ; } ;
		WORD				GetBagIndex( ){ return m_BagIndex ; } ;

		VOID				SetItem( const	_ITEM* pItem )
		{ 
			m_Item = *pItem ; 
		} ;
		_ITEM*				GetItem( ){ return &m_Item ; } ;

        VOID				SetOpType( INT op ){ m_OpType = op ; } ;
        WORD				GetOpType( ){ return m_OpType ; } ;

	private:
		WORD				m_BagIndex ;
		_ITEM				m_Item ;
        INT                 m_OpType;   //操作类型

	};


	class GCNotifyEquipFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCNotifyEquip(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_NOTIFYEQUIP; }
		UINT		GetPacketMaxSize() const { return	sizeof(WORD) + 
														sizeof(_ITEM) +
                                                        sizeof(INT); }
	};


	class GCNotifyEquipHandler
	{
	public:
		static UINT Execute( GCNotifyEquip* pPacket, Player* pPlayer );
	};



};

using namespace Packets ;


#endif
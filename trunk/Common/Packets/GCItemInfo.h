#ifndef _GC_ITEM_INFO_H_
#define _GC_ITEM_INFO_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
	class GCItemInfo : public Packet
	{
    public:
        enum    
        {
            OPERATOR_BAG = 0,   //��ͨ��������
            OPERATOR_BUY_BACK,  //�ع�������
			OPERATOR_BODY,		//������ϵ���Ʒ����
        };

	public:
		GCItemInfo()
        {
            m_OpType = 0;
        };
		virtual	~GCItemInfo(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_ITEMINFO; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(WORD)+GetItemVarSize(m_ITEM)+sizeof(BOOL)+sizeof(INT);
		}

	public:

		VOID			setID(WORD id)			{ m_BagIndex = id;}
		WORD			getID(VOID)				{ return m_BagIndex;}
		
		_ITEM*			getItem()				{return &m_ITEM;}
		VOID			setItem(const	_ITEM* pItem)	
		{
			Assert(pItem);
			memcpy(&m_ITEM,pItem,sizeof(_ITEM));
		}

		VOID			setIsNull(BOOL	bNull)		{m_nsNull	= bNull;}
		BOOL			getIsNull()	{return m_nsNull;}

        VOID            setOpType(INT op) { m_OpType = op; };
        INT             getOpType() { return m_OpType; };
	private:
		
		WORD			m_BagIndex;		//item ��BagIndex
		BOOL			m_nsNull;		//��Ʒ�Ƿ�Ϊ��		TRUE ����û��Item,FALSE ������Item	
		_ITEM			m_ITEM;
        INT             m_OpType;   //��������

	};


	class GCItemInfoFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCItemInfo(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_ITEMINFO; }
		UINT		GetPacketMaxSize() const { return	sizeof(WORD)+sizeof(_ITEM)+sizeof(BOOL)+sizeof(INT); }
	};


	class GCItemInfoHandler
	{
	public:
		static UINT Execute( GCItemInfo* pPacket, Player* pPlayer );
	};

}

using namespace Packets;


#endif
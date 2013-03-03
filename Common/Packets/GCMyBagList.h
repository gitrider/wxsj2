#ifndef _GC_MYBAG_LIST_H_
#define _GC_MYBAG_LIST_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "CGAskMyBagList.h"

struct  _BAG_ITEM
{
	BYTE			m_nndex;
	_ITEM			m_nItemData;

	UINT	GetSize() const 
	{
		int nItemSize = GetItemVarSize(m_nItemData);
		if(nItemSize == 0)
			return 0;
		return	sizeof(m_nndex)+nItemSize;
	}
};


namespace Packets
{
	class GCMyBagList:public Packet
	{
	public:
		GCMyBagList(){};
		virtual	~GCMyBagList(){};


		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_MYBAGLIST ; }
		virtual UINT			GetPacketSize() const 
		{ 
			int nSize = sizeof(ASK_BAG_MODE) + sizeof(BYTE);
			for(int i=0; i<m_nItemCount; ++i)
			{
				nSize = m_ItemData[i].GetSize() + nSize;
			}
			return nSize;
		}



	public:
		VOID					SetAskMode(ASK_BAG_MODE mode){m_Mode	=	mode;}
		VOID					SetAskItemData(_BAG_ITEM* pAskData,BYTE index)
		{
			memcpy(&m_ItemData[index], pAskData, sizeof(_BAG_ITEM));
		}
		VOID					SetItemCount(BYTE nCount){m_nItemCount	=	nCount;}
		BYTE					GetItemCount(){ return m_nItemCount;}

		ASK_BAG_MODE			GetAskMode(){return m_Mode;}
		_BAG_ITEM*				GetItemData(){return m_ItemData;}


	private:
		ASK_BAG_MODE		m_Mode;
		BYTE				m_nItemCount;
		_BAG_ITEM			m_ItemData[MAX_BAG_SIZE];

	};

	class GCMyBagListFactory:public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCMyBagList() ; }
		PacketID_t	GetPacketID() const { return PACKET_GC_MYBAGLIST; }
		UINT		GetPacketMaxSize() const { return	
			sizeof(ASK_BAG_MODE)+
			sizeof(BYTE)+
			sizeof(_BAG_ITEM)*MAX_BAG_SIZE;}
	};

	class GCMyBagListHandler
	{
	public:
		static UINT Execute( GCMyBagList* pPacket, Player* pPlayer );
	};



}






#endif
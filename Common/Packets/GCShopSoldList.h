// GCShopSoldList.h
// 
// 商人发给客户端的商品列表
// 
//////////////////////////////////////////////////////

#ifndef __GCSHOPSOLDLIST_H__
#define __GCSHOPSOLDLIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCShopSoldList : public Packet
	{
	public:
		//商人所卖商品结构
		struct _MERCHANDISE_ITEM
		{
			_ITEM		item_data;
			UINT		iPrice;
            INT         iIndex;         //卖出物品的位置

			_MERCHANDISE_ITEM()
			{
				iPrice = 0;
                iIndex = INVALID_ID;
			}

			VOID	Read( SocketInputStream& iStream )
			{
                //iStream.Read( (CHAR*)(&item_data), sizeof(_ITEM));
				item_data.ReadItemVarAttr(iStream);
				iStream.Read( (CHAR*)(&iIndex), sizeof(INT));
				iStream.Read( (CHAR*)(&iPrice), sizeof(UINT));
			}
			VOID	Write( SocketOutputStream& oStream )const
			{
                //oStream.Write( (CHAR*)(&item_data), sizeof(_ITEM));
				item_data.WriteItemVarAttr(oStream);
                oStream.Write( (CHAR*)(&iIndex), sizeof(INT));
				oStream.Write( (CHAR*)(&iPrice), sizeof(UINT));
			}
		};

	public:
		GCShopSoldList( )
		{
			m_nMerchadiseNum	=	0;
		};
		virtual ~GCShopSoldList( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_SHOPSOLDLIST; }
		virtual UINT			GetPacketSize()const
        {
            UINT iSize = sizeof(BYTE);
            for (INT i=0; i<MAX_BOOTH_SOLD_NUMBER; ++i)
            {
                iSize += GetItemVarSize(m_MerchadiseList[i].item_data);
                iSize += sizeof(UINT);
                iSize += sizeof(INT);
            }
            return	iSize;
        }

	public:
		BYTE			GetMerchadiseNum(VOID) const {return m_nMerchadiseNum;};
		VOID			SetMerchadiseNum(BYTE nNumber) {m_nMerchadiseNum = nNumber;};

		_MERCHANDISE_ITEM*	GetMerchadiseList(VOID) { return m_MerchadiseList; }
        VOID                SetMerchadiseData(const _MERCHANDISE_ITEM* pSour, INT iIndex)
        {
            Assert(iIndex>=0 && iIndex<MAX_BOOTH_SOLD_NUMBER);
            m_MerchadiseList[iIndex].iIndex = pSour->iIndex;
            m_MerchadiseList[iIndex].iPrice = pSour->iPrice;
            memcpy((void*)&m_MerchadiseList[iIndex].item_data, &pSour->item_data, sizeof(_ITEM) );
        }

		VOID				SetMerchadiseList(_MERCHANDISE_ITEM* pSour)
		{
			memcpy(m_MerchadiseList, pSour, MAX_BOOTH_SOLD_NUMBER*sizeof(_MERCHANDISE_ITEM));
		}


	private:
		//商品个数
		BYTE						m_nMerchadiseNum;
		//所有商品列表
		_MERCHANDISE_ITEM			m_MerchadiseList[MAX_BOOTH_SOLD_NUMBER];
	};


	class GCShopSoldListFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCShopSoldList() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_SHOPSOLDLIST; };
		UINT		GetPacketMaxSize()const { return sizeof(GCShopSoldList::_MERCHANDISE_ITEM)*MAX_BOOTH_SOLD_NUMBER + sizeof(BYTE);};
	};

	class GCShopSoldListHandler 
	{
	public:
		static UINT Execute( GCShopSoldList* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif



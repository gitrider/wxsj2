#ifndef _CG_SOUXIA_SEW_H_
#define _CG_SOUXIA_SEW_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGSouXiaSew : public Packet
	{
	public:
		CGSouXiaSew(VOID);
		virtual  ~CGSouXiaSew(VOID);

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		VOID					SetSouXiaData(SOUXIA_DATA* pData)
		{
			Assert(pData);
			memcpy(&m_SouXiaData, pData, sizeof(SOUXIA_DATA));
		}

		SOUXIA_DATA* 			GetSouXiaData()
		{
			return	&m_SouXiaData;
		}

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_SOUXIASEW; }
		virtual UINT			GetPacketSize() const 
		{
			//UINT size = sizeof(BYTE)+sizeof(UINT);
			//for (int i=0; i<4; ++i)
			//{
				//for (int j=0; j<)
				//{
				//	//size += 
				//}
			//}
			return sizeof(BYTE)+sizeof(BYTE)+sizeof(BYTE);
		}
		VOID					SetSewSouXiaIndex(BYTE index)		{ m_nSewSouXia = index;}
		BYTE					GetSewSouXiaIndex()					{ return m_nSewSouXia;}

		VOID					SetSewItemIndex(BYTE index)			{ m_nSewItem = index;}
		BYTE					GetSewItemIndex()					{ return m_nSewItem;}

		VOID					SetRelicItemIndex(BYTE index)		{ m_nRelicItem = index;}
		BYTE					GetRelicItemIndex()					{ return m_nRelicItem;}

	private:
		BYTE				m_nSewSouXia; // 要缝合的捜侠录索引
		BYTE				m_nSewItem;	  // 缝合道具背包索引
		BYTE				m_nRelicItem; // 残页道具背包索引

		SOUXIA_DATA			m_SouXiaData;	

	};

	class CGSouXiaSewFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGSouXiaSew(); }
		PacketID_t	GetPacketID() const { return PACKET_CG_SOUXIASEW; }
		UINT		GetPacketMaxSize() const {return sizeof(BYTE)+sizeof(BYTE)+sizeof(BYTE); }
	};

	class CGSouXiaSewHandler
	{
	public:
		static UINT Execute( CGSouXiaSew* pPacket, Player* pPlayer );
	};
}

using namespace Packets ;

#endif // _CG_SOUXIA_SEW_H_

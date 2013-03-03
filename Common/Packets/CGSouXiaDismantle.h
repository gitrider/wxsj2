#ifndef _CG_SOUXIA_DISMANTLE_H_
#define _CG_SOUXIA_DISMANTLE_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGSouXiaDismantle : public Packet
	{
	public:
		CGSouXiaDismantle(VOID);
		virtual ~CGSouXiaDismantle(VOID);

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_SOUXIADISMANTLE ; }
		virtual UINT			GetPacketSize() const 
		{ 
			return  sizeof(BYTE) + sizeof(BYTE);
		}

		VOID					SetSouXiaData(SOUXIA_DATA* pData,BYTE index)
		{
			memcpy(&m_SouXiaData[index], pData, sizeof(SOUXIA_DATA));
		}

		SOUXIA_DATA* 			GetSouXiaData(BYTE index)
		{
			return	&m_SouXiaData[index];
		}

		VOID					SetSouXiaItemIndex(BYTE index)	{ m_nDismantleSouXia = index;}
		BYTE					GetSouXiaItemIndex(){ return m_nDismantleSouXia;}

		VOID					SetDismantleItemIndex(BYTE index)	{ m_nDismantleItem = index;}
		BYTE					GetDismantleItemIndex(){ return m_nDismantleItem;}
	private:
		BYTE				m_nDismantleSouXia; // 要拆解的捜侠录索引
		BYTE				m_nDismantleItem; // 拆解道具背包索引

		BYTE				m_nSouXiaCount; // 学习过的捜侠录总数
		SOUXIA_DATA			m_SouXiaData[MAX_SOUXIA_CONTAINER];

	};

	class CGSouXiaDismantleFactory:public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGSouXiaDismantle() ; }
		PacketID_t	GetPacketID() const { return PACKET_CG_SOUXIADISMANTLE; }
		UINT		GetPacketMaxSize() const { return	sizeof(BYTE)+sizeof(BYTE);}
	};

	class CGSouXiaDismantleHandler
	{
	public:
		static UINT Execute( CGSouXiaDismantle* pPacket, Player* pPlayer );
	};
}
using namespace Packets ;
#endif // _CG_SOUXIA_DISMANTLE_H_


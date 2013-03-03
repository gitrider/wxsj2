#ifndef _CG_SOUXIA_REFINING_H_
#define _CG_SOUXIA_REFINING_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGSouXiaRefining : public Packet
	{
	public:
		CGSouXiaRefining(VOID);
		virtual ~CGSouXiaRefining(VOID);

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_SOUXIAREFINING ; }
		virtual UINT			GetPacketSize() const 
		{ 
			return  sizeof(BYTE) + sizeof(BYTE) + sizeof(BYTE);
		}

		VOID					SetSouXiaData(SOUXIA_DATA* pData,BYTE index)
		{
			memcpy(&m_SouXiaData[index], pData, sizeof(SOUXIA_DATA));
		}

		SOUXIA_DATA* 			GetSouXiaData(BYTE index)
		{
			return	&m_SouXiaData[index];
		}

		VOID					SetRefiningSouXiaIndex(BYTE index)		{ m_nRefiningSouXia1 = index;}
		BYTE					GetRefiningSouXiaIndex()				{ return m_nRefiningSouXia1;}

		VOID					SetRefiningSouXia2Index(BYTE index)		{ m_nRefiningSouXia2 = index;}
		BYTE					GetRefiningSouXia2Index()				{ return m_nRefiningSouXia2;}

		VOID					SetRefiningItemIndex(BYTE index)		{ m_nRefiningItem = index;}
		BYTE					GetRefiningItemIndex()					{ return m_nRefiningItem;}

	private:
		BYTE				m_nRefiningSouXia1; // 要炼化的第一本捜侠录索引
		BYTE				m_nRefiningSouXia2;	// 要炼化的第二本捜侠录索引
		BYTE				m_nRefiningItem;	// 炼化道具背包索引

		BYTE				m_nSouXiaCount; // 学习过的捜侠录总数
		SOUXIA_DATA			m_SouXiaData[MAX_SOUXIA_CONTAINER];

	};

	class CGSouXiaRefiningFactory:public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGSouXiaRefining() ; }
		PacketID_t	GetPacketID() const { return PACKET_CG_SOUXIAREFINING; }
		UINT		GetPacketMaxSize() const { return	sizeof(BYTE)+sizeof(BYTE) + sizeof(BYTE);}
	};

	class CGSouXiaRefiningHandler
	{
	public:
		static UINT Execute( CGSouXiaRefining* pPacket, Player* pPlayer );
	};
}
using namespace Packets ;
#endif // _CG_SOUXIA_REFINING_H_


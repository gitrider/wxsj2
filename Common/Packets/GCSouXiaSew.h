#ifndef _GC_SOUXIA_SEW_H_
#define _GC_SOUXIA_SEW_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCSouXiaSew : public Packet
	{
	public:
		GCSouXiaSew(VOID);
		virtual ~GCSouXiaSew(VOID);

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_SOUXIASEW ; }
		virtual UINT			GetPacketSize() const 
		{ 
			int nSize =0;
			nSize += sizeof(SHORT);
			nSize += sizeof(UINT);

			nSize += sizeof(BYTE);
			for (int j=0; j<GetSouXiaSkillCount(); ++j)
			{
				nSize += sizeof(SkillID_t);
			}

			nSize += sizeof(BYTE);
			for (int j=0; j<GetSouXiaProductCount(); ++j)
			{
				nSize += sizeof(SkillID_t);
			}

			nSize += sizeof(BYTE);
			for (int j=0; j<GetSouXiaPetZhaoHuanCount(); ++j)
			{
				nSize += sizeof(SkillID_t);
				nSize += sizeof(BYTE);
			}

			nSize += sizeof(BYTE);
			for (int j=0; j<GetSouXiaZuoJiZhaoHuanCount(); ++j)
			{
				nSize += sizeof(SkillID_t);
				nSize += sizeof(BYTE);
			}
			nSize += sizeof(BYTE); // result
			return nSize;
		}

		VOID	SetSouXiaData(SOUXIA_DATA* pData)
		{
			memcpy(&m_SouXiaData, pData, sizeof(SOUXIA_DATA));
		}

		SOUXIA_DATA* 			GetSouXiaData()						{ return	&m_SouXiaData; }
		VOID					SetSewSouXiaIndex(BYTE index)		{ m_nSewSouXia = index;}
		BYTE					GetSewSouXiaIndex()					{ return m_nSewSouXia;}

		VOID					SetSewItemIndex(BYTE index)			{ m_nSewItem = index;}
		BYTE					GetSewItemIndex()					{ return m_nSewItem;}

		VOID					SetRelicItemIndex(BYTE index)		{ m_nRelicItem = index;}
		BYTE					GetRelicItemIndex()					{ return m_nRelicItem;}


		VOID					SetResult(BYTE Result)				{ m_nResult = Result; }
		BYTE					GetResult()							{ return m_nResult; }

		// SouXiaData
		SHORT	GetSouXiaPos()const { return m_SouXiaData.m_CurPos;}
		UINT	GetSouXiaID()	const { return m_SouXiaData.m_SouXiaID;}

		BYTE	GetSouXiaSkillCount()   const	{ return m_SouXiaData.m_Skill.StudyCount;}
		BYTE	GetSouXiaProductCount() const	{ return m_SouXiaData.m_Product.StudyCount;}

		SkillID_t GetSouXiaSkillID(BYTE index)		const	{ return m_SouXiaData.m_Skill.StudySkillId[index];}
		SkillID_t GetSouXiaSProductID(BYTE index)	const	{ return m_SouXiaData.m_Product.StudyProductId[index];}

		BYTE	GetSouXiaPetZhaoHuanCount()	const	{ return m_SouXiaData.m_Pet.StudyCount;}
		BYTE	GetSouXiaZuoJiZhaoHuanCount() const	{ return m_SouXiaData.m_ZuoJi.StudyCount;}

		ZhaoHuan GetSouXiaPetZhaoHuan(BYTE index)	const	{ return m_SouXiaData.m_Pet.StudyPet[index];}
		ZhaoHuan GetSouXiaZuoJiZhaoHuan(BYTE index)	const	{ return m_SouXiaData.m_ZuoJi.StudyZuoji[index];}

	private:
		BYTE				m_nSewSouXia; // 要缝合的捜侠录索引
		BYTE				m_nSewItem;		// 缝合道具背包索引
		BYTE				m_nRelicItem;	// 残页道具背包索引
		SOUXIA_DATA			m_SouXiaData;	
		BYTE				m_nResult;		//炼化结果
	};

	class GCSouXiaSewFactory:public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCSouXiaSew() ; }
		PacketID_t	GetPacketID() const { return PACKET_GC_SOUXIASEW; }
		UINT		GetPacketMaxSize() const { return	sizeof(SOUXIA_DATA)+sizeof(BYTE);}
	};

	class GCSouXiaSewHandler
	{
	public:
		static UINT Execute( GCSouXiaSew* pPacket, Player* pPlayer );
	};
}
using namespace Packets ;
#endif // _GC_SOUXIA_SEW_H_


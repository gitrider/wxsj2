#ifndef _GC_SOUXIA_REFINING_H_
#define _GC_SOUXIA_REFINING_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCSouXiaRefining : public Packet
	{
	public:
		GCSouXiaRefining(VOID);
		virtual  ~GCSouXiaRefining(VOID);

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

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_SOUXIAREFINING; }
		virtual UINT			GetPacketSize() const 
		{
			int nSize =  sizeof(BYTE);//bagindex

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
			nSize += sizeof(BYTE); // type
			return nSize;
		}

		VOID	SetRefiningSouXiaIndex(BYTE index)		{ m_nRefiningSouXia1 = index;}
		BYTE	GetRefiningSouXiaIndex()				{ return m_nRefiningSouXia1;}

		VOID	SetRefiningSouXia2Index(BYTE index)		{ m_nRefiningSouXia2 = index;}
		BYTE	GetRefiningSouXia2Index()				{ return m_nRefiningSouXia2;}

		VOID	SetRefiningItemIndex(BYTE index)		{ m_nRefiningItem = index;}
		BYTE	GetRefiningItemIndex()					{ return m_nRefiningItem;}

		VOID	SetNewItemBagIndex(BYTE index)			{ m_nNewItemBagIndex = index;}
		BYTE	GetNewItemBagIndex()					{ return m_nNewItemBagIndex;}

		VOID	SetResult(BYTE Result)					{ m_nResult = Result; }
		BYTE	GetResult()								{ return m_nResult; }

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

		BYTE	GetCurRefiningType()				const	{ return m_nType; }	//残页的类型
		VOID	SetCurRefiningType(BYTE type) 				{ m_nType = type; } 
	private:
		BYTE			m_nRefiningSouXia1; // 要炼化的第一本捜侠录索引
		BYTE			m_nRefiningSouXia2;	// 要炼化的第二本捜侠录索引
		BYTE			m_nRefiningItem;	// 炼化道具背包索引

		BYTE			m_nNewItemBagIndex;	// 新道具背包索引
		SOUXIA_DATA		m_SouXiaData;	
		BYTE			m_nResult;//炼化结果
		BYTE			m_nType;//残页内容的类型1.技能2.配方3.神兽召唤技能4.坐骑召唤技能 每次只能有一种

	};

	class GCSouXiaRefiningFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCSouXiaRefining(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_SOUXIAREFINING; }
		UINT		GetPacketMaxSize() const {return sizeof(BYTE)+sizeof(SOUXIA_DATA)+sizeof(BYTE)+sizeof(BYTE); }
	};

	class GCSouXiaRefiningHandler
	{
	public:
		static UINT Execute( GCSouXiaRefining* pPacket, Player* pPlayer );
	};
}

using namespace Packets ;

#endif // _GC_SOUXIA_REFINING_H_

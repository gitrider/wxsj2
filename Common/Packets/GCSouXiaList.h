#ifndef _GC_SOUXIA_LIST_H_
#define _GC_SOUXIA_LIST_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCSouXiaList:public Packet
	{
	public:
		GCSouXiaList(VOID)			
		{
			m_nSouXiaCount = 0; 
		}
		virtual	~GCSouXiaList(VOID)	{};


		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_SOUXIA_LIST ; }
		virtual UINT			GetPacketSize() const 
		{ 
			int nSize =  sizeof(BYTE) ;//count
			for(int i=0; i<m_nSouXiaCount; ++i)
			{
				nSize += sizeof(SHORT);
				nSize += sizeof(UINT);

				nSize += sizeof(BYTE);
				for (int j=0; j<GetSouXiaSkillCount(i); ++j)
				{
					nSize += sizeof(SkillID_t);
				}

				nSize += sizeof(BYTE);
				for (int j=0; j<GetSouXiaProductCount(i); ++j)
				{
					nSize += sizeof(SkillID_t);
				}

				nSize += sizeof(BYTE);
				for (int j=0; j<GetSouXiaPetZhaoHuanCount(i); ++j)
				{
					nSize += sizeof(SkillID_t);
					nSize += sizeof(BYTE);
				}

				nSize += sizeof(BYTE);
				for (int j=0; j<GetSouXiaZuoJiZhaoHuanCount(i); ++j)
				{
					nSize += sizeof(SkillID_t);
					nSize += sizeof(BYTE);
				}
			}
			return nSize;
		}

		VOID			SetSouXiaData(SOUXIA_DATA* pData,BYTE subIndex)
		{
			Assert(pData);
			memcpy(&m_SouXiaData[subIndex], pData, sizeof(SOUXIA_DATA));
		}

		SOUXIA_DATA* 			GetSouXiaData(BYTE subIndex)
		{
			return	&m_SouXiaData[subIndex];
		}
		VOID			SetSouXiaCount(BYTE nCount)	{m_nSouXiaCount = nCount;}
		BYTE			GetSouXiaCount() const { return m_nSouXiaCount;}

		SHORT			GetSouXiaPos(SHORT subIndex)const { return m_SouXiaData[subIndex].m_CurPos;}
		UINT			GetSouXiaID(SHORT subIndex)	const { return m_SouXiaData[subIndex].m_SouXiaID;}

		BYTE			GetSouXiaSkillCount(SHORT subIndex)   const	{ return m_SouXiaData[subIndex].m_Skill.StudyCount;}
		BYTE			GetSouXiaProductCount(SHORT subIndex) const	{ return m_SouXiaData[subIndex].m_Product.StudyCount;}

		SkillID_t		GetSouXiaSkillID(SHORT subIndex, BYTE index)	const	{ return m_SouXiaData[subIndex].m_Skill.StudySkillId[index];}
		SkillID_t		GetSouXiaSProductID(SHORT subIndex, BYTE index)	const	{ return m_SouXiaData[subIndex].m_Product.StudyProductId[index];}

		BYTE			GetSouXiaPetZhaoHuanCount(SHORT subIndex)	const	{ return m_SouXiaData[subIndex].m_Pet.StudyCount;}
		BYTE			GetSouXiaZuoJiZhaoHuanCount(SHORT subIndex) const	{ return m_SouXiaData[subIndex].m_ZuoJi.StudyCount;}

		ZhaoHuan		GetSouXiaPetZhaoHuan(SHORT subIndex, BYTE index)	const	{ return m_SouXiaData[subIndex].m_Pet.StudyPet[index];}
		ZhaoHuan		GetSouXiaZuoJiZhaoHuan(SHORT subIndex, BYTE index)	const	{ return m_SouXiaData[subIndex].m_ZuoJi.StudyZuoji[index];}

	private:
		BYTE			m_nSouXiaCount; // 学习过的捜侠录总数
		SOUXIA_DATA		m_SouXiaData[MAX_SOUXIA_CONTAINER];//已经不和人身上的序号一样了

	};

	class GCSouXiaListFactory:public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCSouXiaList() ; }
		PacketID_t	GetPacketID() const { return PACKET_GC_SOUXIA_LIST; }
		UINT		GetPacketMaxSize() const { return	sizeof(BYTE)+sizeof(SOUXIA_DATA)*MAX_SOUXIA_CONTAINER;}
	};
 
	class GCSouXiaListHandler
	{
	public:
		static UINT Execute( GCSouXiaList* pPacket, Player* pPlayer );
	};

}

#endif //_GC_SOUXIA_LIST_H_

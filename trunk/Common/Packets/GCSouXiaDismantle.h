#ifndef _GC_SOUXIA_DISMANTLE_H_
#define _GC_SOUXIA_DISMANTLE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	#define MAX_SOUXIA_RELIC	80 //ע��Խ��,��ʱ�޸�

	class GCSouXiaDismantle : public Packet
	{
	public:
		GCSouXiaDismantle(VOID);
		virtual  ~GCSouXiaDismantle(VOID);

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		VOID					SetSouXiaData(SOUXIA_DATA* pData, BYTE index)
		{
			Assert(pData);
			memcpy(&m_SouXiaData[index], pData, sizeof(SOUXIA_DATA));
		}

		SOUXIA_DATA* 			GetSouXiaData(BYTE index)
		{
			return	&m_SouXiaData[index];
		}

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_SOUXIADISMANTLE; }
		virtual UINT			GetPacketSize() const 
		{
			int nSize =  sizeof(BYTE) + sizeof(BYTE) + sizeof(BYTE)*m_nSouXiaRelicCount;//count,type,bagindex
			for(int i=0; i<m_nSouXiaRelicCount; ++i)
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

			nSize += sizeof(BYTE);//result
			return nSize;
		}

		VOID	SetResult(BYTE Result)	{ m_nResult = Result; }
		BYTE	GetResult()	const { return m_nResult; }

		BYTE	GetNewSouXiaRelicCount() const	{ return m_nSouXiaRelicCount; }
		VOID	SetNewSouXiaRelicCount(BYTE relicCount)	{ m_nSouXiaRelicCount = relicCount; } 

		SHORT	GetSouXiaPos(SHORT subIndex)const { return m_SouXiaData[subIndex].m_CurPos;}
		UINT	GetSouXiaID(SHORT subIndex)	const { return m_SouXiaData[subIndex].m_SouXiaID;}

		BYTE	GetSouXiaSkillCount(SHORT subIndex)   const	{ return m_SouXiaData[subIndex].m_Skill.StudyCount;}
		BYTE	GetSouXiaProductCount(SHORT subIndex) const	{ return m_SouXiaData[subIndex].m_Product.StudyCount;}

		SkillID_t GetSouXiaSkillID(SHORT subIndex, BYTE index)		const	{ return m_SouXiaData[subIndex].m_Skill.StudySkillId[index];}
		SkillID_t GetSouXiaSProductID(SHORT subIndex, BYTE index)	const	{ return m_SouXiaData[subIndex].m_Product.StudyProductId[index];}

		BYTE	GetSouXiaPetZhaoHuanCount(SHORT subIndex)	const	{ return m_SouXiaData[subIndex].m_Pet.StudyCount;}
		BYTE	GetSouXiaZuoJiZhaoHuanCount(SHORT subIndex) const	{ return m_SouXiaData[subIndex].m_ZuoJi.StudyCount;}

		ZhaoHuan GetSouXiaPetZhaoHuan(SHORT subIndex, BYTE index)	const	{ return m_SouXiaData[subIndex].m_Pet.StudyPet[index];}
		ZhaoHuan GetSouXiaZuoJiZhaoHuan(SHORT subIndex, BYTE index)	const	{ return m_SouXiaData[subIndex].m_ZuoJi.StudyZuoji[index];}

		BYTE	GetCurBagIndex(BYTE subIndex)					{ return m_nCurBagIndex[subIndex]; }		//ѭ�����±�
		VOID	SetCurBagIndex(BYTE subIndex , BYTE bagIndex)	{ m_nCurBagIndex[subIndex] = bagIndex; } //�����±�

		BYTE	GetCurRelicType(BYTE subIndex)	const	{ return m_nType; }	//��ҳ������
		VOID	SetCurRelicType(BYTE type) 				{ m_nType = type; } 

	private:
		BYTE	m_nSouXiaRelicCount;
		BYTE	m_nResult;//�Ƿ�ɹ������ɹ�����������Ϣ
		BYTE	m_nType;//��ҳ���ݵ�����1.����2.�䷽3.�����ٻ�����4.�����ٻ����� ÿ��ֻ����һ��
		BYTE    m_nCurBagIndex[MAX_SOUXIA_RELIC];//��ǰ�ı�������
		SOUXIA_DATA	m_SouXiaData[MAX_SOUXIA_RELIC];	
	};

	class GCSouXiaDismantleFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCSouXiaDismantle(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_SOUXIADISMANTLE; }
		UINT		GetPacketMaxSize() const {return sizeof(BYTE) + sizeof(BYTE) + sizeof(BYTE)*MAX_SOUXIA_RELIC+sizeof(SOUXIA_DATA)*MAX_SOUXIA_RELIC+sizeof(BYTE); }
	};

	class GCSouXiaDismantleHandler
	{
	public:
		static UINT Execute( GCSouXiaDismantle* pPacket, Player* pPlayer );
	};
}

using namespace Packets ;

#endif // _GC_SOUXIA_DISMANTLE_H_

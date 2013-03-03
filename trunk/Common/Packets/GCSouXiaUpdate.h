#ifndef _GC_SOUXIA_UPDATE_H_
#define _GC_SOUXIA_UPDATE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCSouXiaUpdate:public Packet
	{
	public:
		GCSouXiaUpdate(VOID)
		{
			m_nType = 0;
			m_nSkillCount = 0; 
			m_nProductCount = 0;
			m_nPetZhaoHuanCount = 0;
			m_nZuoJiZhaoHuanCount = 0;

		}
		virtual	~GCSouXiaUpdate(VOID){}

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_SOUXIA_UPDATE ; }
		virtual UINT			GetPacketSize() const 
		{ 
			return	sizeof(SHORT) + sizeof(BYTE) + 
					sizeof(BYTE) + sizeof(SkillID_t) * m_nSkillCount +
					sizeof(BYTE) + sizeof(SkillID_t) * m_nProductCount + 
					sizeof(BYTE) + sizeof(SouXia_PetZhaoHuan) + 
					sizeof(BYTE) + sizeof(SouXia_ZuojiZhaoHuan);
		}

		VOID		SetCurSouXiaPos(SHORT pos){m_nCurPos = pos;}
		SHORT		GetCurSouXiaPos(){ return m_nCurPos;}

		VOID		SetSouXiaType(BYTE type){m_nType = type;}
		BYTE		GetSouXiaType(){ return m_nType;}

		VOID		SetSouXiaSkillCount(BYTE skillCount){m_nSkillCount = skillCount;}
		BYTE		GetSouXiaSkillCount(){ return m_nSkillCount;}

		VOID		SetSouXiaSkillID(SkillID_t*  skillIndex)
		{
			for (INT i=0; i<m_nSkillCount; ++i)
			{
				m_nSkillIndex[i] = skillIndex[i];
			}
		}
		SkillID_t*	GetSouXiaSkillID(){ return m_nSkillIndex;}

		VOID		SetSouXiaProductCount(BYTE productCount){m_nProductCount = productCount;}
		BYTE		GetSouXiaProductCount(){ return m_nProductCount;}

		VOID		SetSouXiaProductID(SkillID_t*  skillIndex)
		{
			for (INT i=0; i<m_nProductCount; ++i)
			{
				m_nProductIndex[i] = skillIndex[i];
			}
		}

		SkillID_t*	GetSouXiaProductID() { return m_nProductIndex; }

		VOID		SetSouXiapetZhaoHuanCount(BYTE petZhaoHuanCount){m_nPetZhaoHuanCount = petZhaoHuanCount;}
		BYTE		GetSouXiapetZhaoHuanCount(){ return m_nPetZhaoHuanCount;}

		VOID		SetSouXiaPetZhaoHuan(const SouXia_PetZhaoHuan& petZhaoHuan)
		{
			m_nPetZhaoHuan = petZhaoHuan;
		}
		SouXia_PetZhaoHuan*	GetSouXiaPetZhaoHuan(){	return &m_nPetZhaoHuan; }

		VOID		SetSouXiaZuoJiZhaoHuanCount(BYTE zuoJiZhaoHuanCount){m_nZuoJiZhaoHuanCount = zuoJiZhaoHuanCount;}
		BYTE		GetSouXiaZuoJiZhaoHuanCount(){ return m_nZuoJiZhaoHuanCount; }

		VOID		SetSouXiaZuoJiZhaoHuan(const SouXia_ZuojiZhaoHuan& zuoJiZhaoHuan)
		{
			m_nZuoJiZhaoHuan = zuoJiZhaoHuan;
		}
		SouXia_ZuojiZhaoHuan*	GetSouXiaZuoJiZhaoHuan(){ return &m_nZuoJiZhaoHuan;}

	private:
		SHORT		m_nCurPos;   // 要更新的捜侠录数据所在位置
		BYTE		m_nType;// 捜侠录的总的类型个数最大为5（全部都有学习）可以不用校验用1.技能2.生产配方3.神兽召唤4.坐骑召唤5.全部都有

		BYTE		m_nSkillCount; // 当前搜侠具有的技能总数
		SkillID_t   m_nSkillIndex[MAX_SKILL_COUNT];// 技能索引

		BYTE		m_nProductCount; // 当前搜侠具有的武器配方总数
		SkillID_t   m_nProductIndex[MAX_PRODUCT_COUNT];// 武器配方索引

		BYTE		m_nPetZhaoHuanCount; // 当前捜侠具有的神兽召唤技能总数
		SouXia_PetZhaoHuan   m_nPetZhaoHuan;// 神兽召唤技能索引

		BYTE		 m_nZuoJiZhaoHuanCount; // 当前捜侠具有的坐骑召唤技能总数
		SouXia_ZuojiZhaoHuan   m_nZuoJiZhaoHuan;    // 坐骑召唤技能索引和还有的使用次数

	};

	class GCSouXiaUpdateFactory : public PacketFactory
	{
	public:
		Packet* CreatePacket()	 { return new GCSouXiaUpdate() ; }
		PacketID_t	GetPacketID()	const { return PACKET_GC_SOUXIA_UPDATE; }
		UINT		GetPacketMaxSize()const 
		{
			return	sizeof(SHORT) + sizeof(BYTE) + 
				sizeof(BYTE) + sizeof(SkillID_t) * MAX_SKILL_COUNT +
				sizeof(BYTE) + sizeof(SkillID_t) * MAX_PRODUCT_COUNT + 
				sizeof(BYTE) + sizeof(SouXia_PetZhaoHuan) + 
				sizeof(BYTE) + sizeof(SouXia_ZuojiZhaoHuan);}
	};
 
	class GCSouXiaUpdateHandler
	{
	public:
		static UINT Execute( GCSouXiaUpdate* pPacket, Player* pPlayer );
	};

}

#endif //_GC_SOUXIA_UPDATE_H_

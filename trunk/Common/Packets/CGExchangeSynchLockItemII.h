// CGExchangeSynchLockItemII.h
// 
// 玩家确定选中(点击交易物品后的挂锁按钮)一件交易物品时发送
// 
//////////////////////////////////////////////////////

#ifndef __CGEXCHANGESYNCHLOCKITEMII_H__
#define __CGEXCHANGESYNCHLOCKITEMII_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "ExchangeMsgDefine.h"

namespace Packets
{
	class CGExchangeSynchLockItemII : public Packet
	{
	public:
		CGExchangeSynchLockItemII( )
		{
			m_IsMyself  = FALSE;
			m_Type   = 0;
			m_bLock  = FALSE;
			m_Index  = -1;
		};

		virtual ~CGExchangeSynchLockItemII( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_EXCHANGESYNCHLOCKITEMII; }
		virtual UINT			GetPacketSize()const 
		{ 			
			if ( m_Type == EXCHANGE_MSG::POS_BAG )
			{
				return sizeof(BOOL)*2 + sizeof(BYTE)*2;
			}
			else if ( m_Type == EXCHANGE_MSG::POS_PET )
			{
				return sizeof(BOOL)*2 + sizeof(BYTE)*2 + sizeof(PET_GUID_t);
			}
			else if ( m_Type == EXCHANGE_MSG::POS_FOLLOWPET )	// 待加 跟随宠物用到包大小...
			{
				return 0;
			}
			else
				return 0;
		}

	public:
		BOOL					GetIsMyself(VOID) const {return m_IsMyself;};
		VOID					SetIsMyself(BOOL IsMyself) {m_IsMyself = IsMyself;};

		BYTE					GetType(VOID) const {return m_Type;};
		VOID					SetType(BYTE itemType) {m_Type = itemType;};

		BYTE					GetIndex(VOID) const {return m_Index;};
		VOID					SetIndex(BYTE nIndex) {m_Index = nIndex;};

		BOOL					GetLock(VOID) const {return m_bLock;};
		VOID					SetLock(BOOL bLock) {m_bLock = bLock;};

		PET_GUID_t				GetPetGuid(VOID) const {return m_PetGuid;};
		VOID					SetPetGuid(PET_GUID_t PetGuid) {m_PetGuid = PetGuid;};

		// 待加 跟随宠物设置获取关键字函数...

	private:
		BOOL					m_IsMyself;	 // 物品是否属于自己交易栏 True：属于玩家自己， False：属于对方 
		BOOL					m_bLock;	 // 是否确定选中物品
		BYTE					m_Index;	 // 物品所在队列序号
		BYTE					m_Type;		 // 物品类型 POS_BAG,POS_PET,POS_FOLLOWPET
		PET_GUID_t				m_PetGuid;

		// 待加 跟随宠物用到关键字...
	};

	class CGExchangeSynchLockItemIIFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGExchangeSynchLockItemII() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_EXCHANGESYNCHLOCKITEMII; };
		UINT		GetPacketMaxSize()const 
		{ 	
			return sizeof(BOOL)*2 + sizeof(BYTE)*2 + sizeof(PET_GUID_t);	
		} // 待加 跟随宠物确定的包最大值...
	};

	class CGExchangeSynchLockItemIIHandler 
	{
	public:
		static UINT Execute( CGExchangeSynchLockItemII* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
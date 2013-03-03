// GCExchangeSynchLockItemII.h
// 
// ���ȷ��ѡ��(���������Ʒ��Ĺ�����ť)һ��������Ʒ
// 
//////////////////////////////////////////////////////

#ifndef __GCEXCHANGESYNCHLOCKITEMII_H__
#define __GCEXCHANGESYNCHLOCKITEMII_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "ExchangeMsgDefine.h"

namespace Packets
{
	class GCExchangeSynchLockItemII : public Packet
	{
	public:
		GCExchangeSynchLockItemII( )
		{
			m_IsMyself = FALSE;
			m_bLock  = FALSE;
			m_Type	 = 0;
			m_Index  = -1;
		}

		virtual ~GCExchangeSynchLockItemII( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_EXCHANGESYNCHLOCKITEMII; }
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
			else if ( m_Type == EXCHANGE_MSG::POS_FOLLOWPET )	// ���� ��������õ�����С...
			{
				return 0;
			}
			else
				return 0;
		}

	public:
		BYTE					GetIsMyself(VOID) const {return m_IsMyself;};
		VOID					SetIsMyself(BYTE IsMyself) {m_IsMyself = IsMyself;};

		BYTE					GetType(VOID) const {return m_Type;};
		VOID					SetType(BYTE itemType) {m_Type = itemType;};

		BYTE					GetIndex(VOID) const {return m_Index;};
		VOID					SetIndex(BYTE Index) {m_Index = Index;};

		BOOL					GetLock(VOID) const {return m_bLock;};
		VOID					SetLock(BOOL bLock) {m_bLock = bLock;};

		PET_GUID_t				GetPetGuid(VOID) const {return m_PetGuid;};
		VOID					SetPetGuid(PET_GUID_t PetGuid) {m_PetGuid = PetGuid;};

		// ���� �����������/��ȡ�ؼ��ֺ���...

	private:
		BOOL					m_IsMyself;	 // ��Ʒ�Ƿ������Լ������� True����������Լ��� False�����ڶԷ� 
		BOOL					m_bLock;	 // �Ƿ�ȷ��ѡ����Ʒ
		BYTE					m_Index;	 // ��Ʒ���ڶ������
		BYTE					m_Type;		 // ��Ʒ���� POS_BAG,POS_PET,POS_FOLLOWPET
		PET_GUID_t				m_PetGuid;

		// ���Ӹ�������õ��ؼ���...
	};

	class GCExchangeSynchLockItemIIFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCExchangeSynchLockItemII() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_EXCHANGESYNCHLOCKITEMII; };
		UINT		GetPacketMaxSize()const { return sizeof(BOOL)*2 + sizeof(BYTE)*2 + sizeof(PET_GUID_t);}; // ���� �������ȷ���İ����ֵ...
	};

	class GCExchangeSynchLockItemIIHandler 
	{
	public:
		static UINT Execute( GCExchangeSynchLockItemII* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
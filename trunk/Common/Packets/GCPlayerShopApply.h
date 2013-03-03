// GCPlayerShopApply.h
// 
// ������֪ͨ�ͻ��˴�������̵�������ж���Ϣ
// 
//////////////////////////////////////////////////////

#ifndef __GCPLAYERSHOPAPPLY_H__
#define __GCPLAYERSHOPAPPLY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class GCPlayerShopApply : public Packet
	{
	public:
		GCPlayerShopApply( )
		{
			m_ComFactor	=	0;		//��ǰ��ҵָ��
			m_Cost		=	0;		//������
			m_Type		=	0;		//�ɿ�������
			m_NpcId		=	0;		//�̵�����NpcId

		}
		virtual ~GCPlayerShopApply( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_PLAYERSHOPAPPLY; }
		virtual UINT			GetPacketSize()const { return	sizeof(FLOAT)+sizeof(UINT)+sizeof(BYTE)+sizeof(ObjID_t);}

	public:
		FLOAT					GetComFactor() const {return m_ComFactor;}
		VOID					SetComFactor(FLOAT ComFactor) {m_ComFactor = ComFactor;}

		UINT					GetCost() const {return m_Cost;}
		VOID					SetCost(UINT Cost) {m_Cost = Cost;}

		BYTE					GetType() const {return m_Type;}	
		VOID					SetType(BYTE Type) {m_Type = Type;}
		
		ObjID_t					GetNpcId(){return m_NpcId;}
		VOID					SetNpcId(ObjID_t NpcId){m_NpcId = NpcId;}

	private:
		FLOAT					m_ComFactor;	//��ǰ��ҵָ��
		UINT					m_Cost;			//������
		BYTE					m_Type;			//�ɿ�������
		ObjID_t					m_NpcId;		//Npc Id
	};

	class GCPlayerShopApplyFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCPlayerShopApply() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_PLAYERSHOPAPPLY; };
		UINT		GetPacketMaxSize()const { return sizeof(FLOAT)+sizeof(UINT)+sizeof(BYTE)+sizeof(ObjID_t);};
	};

	class GCPlayerShopApplyHandler 
	{
	public:
		static UINT Execute( GCPlayerShopApply* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif

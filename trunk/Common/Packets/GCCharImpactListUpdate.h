// GCCharImpactListUpdate.h
// 
// Ч�������
// 
//////////////////////////////////////////////////////

#ifndef __GCCHARIMPACTLISTUPDATE_H__
#define __GCCHARIMPACTLISTUPDATE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCCharImpactListUpdate : public Packet
	{
	public:
		typedef struct BriefImpact
		{
			BriefImpact()
			{
				CleanUp();
			}
			VOID CleanUp(VOID)
			{
				m_nBuffID		= INVALID_ID;
				m_nSN			= INVALID_ID;
				m_nNumOfLayer	= 1;				
				m_nDelayTime	= 0;
				m_nTimeCount	= 0;
			};
			ImpactID_t		m_nBuffID;		// ��Ч���ݵ�ID(����)
			UINT			m_nSN;			// Ч�����к�
			INT				m_nNumOfLayer;	// buff�ۻ�����
			Time_t			m_nDelayTime;

			INT				m_nTimeCount;	// ����Ч������ʱ�� 20100531 BLL
		} ImpactStruct_T;
		GCCharImpactListUpdate(VOID)
		{
			CleanUp();
		};
		VOID CleanUp(VOID)
		{
			m_nNumOfImpacts = 0;
		};
		virtual ~GCCharImpactListUpdate(VOID){};
	
		//���ü̳нӿ�
		virtual BOOL			Read(SocketInputStream& iStream);
		virtual BOOL			Write(SocketOutputStream& oStream)const;
		virtual UINT			Execute(Player* pPlayer);
	
		virtual PacketID_t		GetPacketID(VOID)const {return PACKET_GC_CHAR_IMPACT_LIST_UPDATE;}
		virtual UINT			GetPacketSize(VOID)const {return sizeof(m_nOwnerID) + sizeof(m_nNumOfImpacts) + sizeof(ImpactStruct_T) * m_nNumOfImpacts;}
	
	public:
		VOID					SetOwnerID(ObjID_t const nID) {m_nOwnerID=nID;}
		ObjID_t					GetOwnerID(VOID) const {return m_nOwnerID;}
		VOID					SetImpactList(_IMPACT_LIST const& rList);
		SHORT					NumOfImpacts(VOID) const {return m_nNumOfImpacts;};
		GCCharImpactListUpdate::ImpactStruct_T const*	GetImpactList(VOID) const {return m_aImpact;};
	private:
		ObjID_t			m_nOwnerID;
		SHORT			m_nNumOfImpacts;
		ImpactStruct_T	m_aImpact[MAX_IMPACT_NUM];
	};
	
	class GCCharImpactListUpdateFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() {return new GCCharImpactListUpdate();}
		PacketID_t	GetPacketID()const {return PACKET_GC_CHAR_IMPACT_LIST_UPDATE;}
		UINT		GetPacketMaxSize()const {return	sizeof(ObjID_t)+ sizeof(SHORT) + sizeof(GCCharImpactListUpdate::ImpactStruct_T)* MAX_IMPACT_NUM;}
	};
	
	class GCCharImpactListUpdateHandler 
	{
	public:
		static UINT Execute( GCCharImpactListUpdate* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;



#endif


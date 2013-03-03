// GCSpecialObj_ActNow.h
// 
// ����û����
// 
//////////////////////////////////////////////////////

#ifndef __GCSPECIALOBJ_ACTNOW_H__
#define __GCSPECIALOBJ_ACTNOW_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameStruct.h"

namespace Packets
{
	class GCSpecialObj_ActNow : public Packet
	{
	public:
		GCSpecialObj_ActNow() : m_nObjID(INVALID_ID), m_nLogicCount(0)
		{
			m_TargetList.CleanUp();
		}
		virtual ~GCSpecialObj_ActNow( ){}

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_SPECIAL_OBJ_ACT_NOW; }
		virtual UINT			GetPacketSize()const 
								{
									return 	sizeof(m_nObjID)
										+	sizeof(m_nLogicCount)
										+	sizeof(m_TargetList.m_nCount)
										+	sizeof(m_TargetList.m_aIDs[0])*m_TargetList.m_nCount;
								}
	public:

	public:
		//ʹ�����ݽӿ�
		ObjID_t				GetObjID(VOID) const {return m_nObjID;}
		VOID				SetObjID(ObjID_t nObjID) {m_nObjID=nObjID;}
		
		ObjID_List const&	GetTargetList(VOID) const {return m_TargetList;}
		VOID				SetTargetList(ObjID_List const& rList) 
		{
			memcpy((VOID*)&m_TargetList, (VOID const*)&rList, sizeof(m_TargetList));
			if(m_TargetList.MAX_LIST_SIZE<m_TargetList.m_nCount)
			{
				m_TargetList.m_nCount=m_TargetList.MAX_LIST_SIZE;
			}
		}
		
		INT					GetLogicCount(VOID) const {return m_nLogicCount;}
		VOID				SetLogicCount(INT nID) {m_nLogicCount=nID;}
	private:
		ObjID_t				m_nObjID;	// ��������ID
		INT					m_nLogicCount; //�߼����� 
		ObjID_List			m_TargetList; // Ŀ���б�.
	};

	class GCSpecialObj_ActNowFactory : public PacketFactory 
	{
	public:
		Packet*			CreatePacket() { return new GCSpecialObj_ActNow() ; }
		PacketID_t		GetPacketID()const { return PACKET_GC_SPECIAL_OBJ_ACT_NOW; }
		UINT			GetPacketMaxSize()const 
						{ 
							return 	sizeof(ObjID_t)
								+	sizeof(INT)
								+	sizeof(ObjID_List);
						}
	};

	class GCSpecialObj_ActNowHandler 
	{
	public:
		static UINT Execute( GCSpecialObj_ActNow* pPacket, Player* pPlayer ) ;
	};
}

#endif

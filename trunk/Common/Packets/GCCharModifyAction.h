// GCCharModifyAction.h
// 
// ���Ķ���
// 
//////////////////////////////////////////////////////

#ifndef __GCCHARMODIFYACTION_H__
#define __GCCHARMODIFYACTION_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCCharModifyAction :
		public Packet
	{
	public:
		GCCharModifyAction( )
		{
			m_ObjID			= INVALID_ID;
			m_nLogicCount	= -1;
			m_nModifyTime	= -1;
		}
		virtual ~GCCharModifyAction( )
		{
		}

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHARMODIFYACTION; }
		virtual UINT			GetPacketSize()const
		{
			return sizeof(m_ObjID)
				+ sizeof(m_nLogicCount)
				+ sizeof(m_nModifyTime);
		}

	public:
		//ʹ�����ݽӿ�
		VOID				setObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t				getObjID(VOID)const { return m_ObjID; }

		VOID				setLogicCount(INT nCount) { m_nLogicCount = nCount; }
		INT					getLogicCount(VOID)const { return m_nLogicCount; }

		VOID				setModifyTime( INT nModifyTime ) { m_nModifyTime = nModifyTime; }
		INT					getModifyTime( VOID )const { return m_nModifyTime; }

	private:
		ObjID_t				m_ObjID;			// ObjID
		INT					m_nLogicCount;		// �߼�����
		INT					m_nModifyTime;		// �Ķ���ʱ��ֵ
	};

	class GCCharModifyActionFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCCharModifyAction() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CHARMODIFYACTION; }
		UINT		GetPacketMaxSize()const { return sizeof(GCCharModifyAction) - sizeof(Packet); }
	};

	class GCCharModifyActionHandler 
	{
	public:
		static UINT Execute( GCCharModifyAction* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif // __GCCHARMODIFYACTION_H__

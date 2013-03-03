// CGCharMove.h
// 
// �ƶ�
// 
//////////////////////////////////////////////////////

#ifndef __CGCHARMOVE_H__
#define __CGCHARMOVE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGCharMove :
		public Packet
	{
	public:
		CGCharMove( )
		{
			m_yNumTargetPos = 0;
		}
		virtual ~CGCharMove( )
		{
		}

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const
		{
			return PACKET_CG_CHARMOVE ;
		}

		virtual UINT			GetPacketSize()const
		{
			return sizeof(ObjID_t) +
				sizeof(INT) +
				sizeof(WORLD_POS) +
				sizeof(BYTE) +
				sizeof(WORLD_POS) * m_yNumTargetPos;
		}

	public:
		//ʹ�����ݽӿ�
		VOID				setObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t				getObjID(VOID)const { return m_ObjID; }

		VOID				setHandleID( INT nID ) { m_nHandleID = nID; }
		INT					getHandleID( VOID )const{ return m_nHandleID; }

		VOID				setWorldPos(WORLD_POS& pos) { m_posWorld = pos; }
		const WORLD_POS&	getWorldPos(VOID)const { return m_posWorld; }

		BYTE				getNumTargetPos( VOID )const{ return m_yNumTargetPos; }
		VOID				addTargetPos( const WORLD_POS *pPos )
		{
			if ( m_yNumTargetPos < MAX_CHAR_PATH_NODE_NUMBER )
			{
				m_aTargetPos[m_yNumTargetPos] = *pPos;
				m_yNumTargetPos++;
			}
		}
		const WORLD_POS		*getTargetPos(VOID)const { return m_aTargetPos; }

	private:
		ObjID_t				m_ObjID;			// ObjID
		INT					m_nHandleID;		// ����ID
		WORLD_POS			m_posWorld;			// ��ǰλ��
		BYTE				m_yNumTargetPos;
		WORLD_POS			m_aTargetPos[MAX_CHAR_PATH_NODE_NUMBER];		// Ŀ��λ��

	};


	class CGCharMoveFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGCharMove() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_CHARMOVE; }
		UINT		GetPacketMaxSize()const { return	sizeof(CGCharMove) - sizeof(Packet); }
	};

	class CGCharMoveHandler 
	{
	public:
		static UINT Execute( CGCharMove* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif

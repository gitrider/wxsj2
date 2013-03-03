// CGCharPositionWarp.h
// 
// λ��ƫ���������
// 
//////////////////////////////////////////////////////

#ifndef __CGCHARPOSITIONWARP_H__
#define __CGCHARPOSITIONWARP_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGCharPositionWarp :
		public Packet
	{
	public:
		CGCharPositionWarp( )
		{
		}
		virtual ~CGCharPositionWarp( )
		{
		}

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const
		{
			return PACKET_CG_CHARPOSITIONWARP ;
		}

		virtual UINT			GetPacketSize()const
		{
			return sizeof(m_ObjID) + sizeof(m_posServer) + sizeof(m_posClient);
		}

	public:
		//ʹ�����ݽӿ�
		VOID				setObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t				getObjID(VOID)const { return m_ObjID; }

		VOID				setServerPos(const WORLD_POS& pos) { m_posServer = pos; }
		const WORLD_POS&	getServerPos(VOID)const { return m_posServer; }

		VOID				setClientPos(const WORLD_POS& pos) { m_posClient = pos; }
		const WORLD_POS&	getClientPos(VOID)const { return m_posClient; }

	private:
		ObjID_t				m_ObjID;			// ObjID
		WORLD_POS			m_posServer;		// ��������ǰλ��
		WORLD_POS			m_posClient;		// �ͻ��˵�ǰλ��
	};


	class CGCharPositionWarpFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGCharPositionWarp() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_CHARPOSITIONWARP; }
		UINT		GetPacketMaxSize()const { return	sizeof(CGCharPositionWarp) - sizeof(Packet); }
	};

	class CGCharPositionWarpHandler 
	{
	public:
		static UINT Execute( CGCharPositionWarp* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif // __CGCHARPOSITIONWARP_H__

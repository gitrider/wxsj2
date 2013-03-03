/********************************************************************************
 *	�ļ�����	GWLevelUp.h
 *	ȫ·����	d:\Prj\Common\Packets\GWLevelUp.h
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 11 �� 9 ��	14:25
 *
 *	����˵����	�� World �������������Ϣ���Ա���
 *	�޸ļ�¼��
*********************************************************************************/
#ifndef _GW_LEVEL_UP_H_
#define _GW_LEVEL_UP_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWLevelUp:public Packet
	{
	public:
		GWLevelUp(){};
		virtual					~GWLevelUp(){};

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GW_LEVEUP; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(GUID_t) + sizeof(INT) + sizeof(UINT);
		}

	public:

		VOID			SetGUID(GUID_t guid) { m_GUID = guid; }
		GUID_t			GetGUID() { return m_GUID; }

		VOID			SetLevel(INT lvl) { m_Level = lvl; }
		INT				GetLevel() { return m_Level; }

		VOID			SetExp(UINT val) { m_m_Exp = val; }
		UINT			GetExp()		 { return m_m_Exp ; }

	private:
		GUID_t			m_GUID;			
		INT				m_Level;			
		UINT			m_m_Exp;			//��Ҿ���

	};


	class GWLevelUpFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GWLevelUp(); }
		PacketID_t	GetPacketID() const { return PACKET_GW_LEVEUP; }
		UINT		GetPacketMaxSize() const { return sizeof(GUID_t) + sizeof(INT) + sizeof(UINT); }
	};


	class GWLevelUpHandler
	{
	public:
		static UINT Execute( GWLevelUp* pPacket, Player* pPlayer );
	};

}

#endif // _GW_LEVEL_UP_H_

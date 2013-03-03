/********************************************************************************
 *	�ļ�����	GCNotifyGoodBad.h
 *	ȫ·����	d:\PrjMaintance\Common\Packets\GCNotifyGoodBad.h
 *	�����ˣ�	����
 *	����ʱ�䣺	2006 �� 5 �� 8 ��	16:35
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef _GC_NOTIFY_GOODBAD_H_
#define _GC_NOTIFY_GOODBAD_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	enum NOTIFY_GOODBAD_TYPE
	{
		NOTIFY_GOODBAD_HELPNEWBIE		= 0,		// �������ֻ�ý���
		NOTIFY_GOODBAD_NEWBIEDIE,					// ����������óͷ�
		NOTIFY_GOODBAD_GMCOMMAND,					// GM ָ������
	};

	class GCNotifyGoodBad : public Packet
	{
	public:
		GCNotifyGoodBad() {}
		virtual	~GCNotifyGoodBad() {}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_NOTIFYGOODBAD; }
		virtual UINT			GetPacketSize() const { return sizeof(m_uMode) + sizeof(m_nValue); }



	public:
		UCHAR					GetNotifyMode() { return m_uMode; }
		VOID					SetNotifyMode(UCHAR uMode) { m_uMode = uMode; }

		INT						GetValue() { return m_nValue; }
		VOID					SetValue(INT nValue) { m_nValue = nValue; }

	private:
		UCHAR					m_uMode;
		INT						m_nValue;

	};

	class GCNotifyGoodBadFactory:public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCNotifyGoodBad(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_NOTIFYGOODBAD; }
		UINT		GetPacketMaxSize() const { return sizeof(GCNotifyGoodBad) - sizeof(Packet); }
	};

	class GCNotifyGoodBadHandler
	{
	public:
		static UINT	Execute( GCNotifyGoodBad* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif // _GC_NOTIFY_GOODBAD_H_

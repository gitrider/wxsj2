/********************************************************************************
 *	文件名：	GCNotifyGoodBad.h
 *	全路径：	d:\PrjMaintance\Common\Packets\GCNotifyGoodBad.h
 *	创建人：	胡繁
 *	创建时间：	2006 年 5 月 8 日	16:35
 *
 *	功能说明：	
 *	修改记录：
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
		NOTIFY_GOODBAD_HELPNEWBIE		= 0,		// 帮助新手获得奖励
		NOTIFY_GOODBAD_NEWBIEDIE,					// 新手死亡获得惩罚
		NOTIFY_GOODBAD_GMCOMMAND,					// GM 指令设置
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

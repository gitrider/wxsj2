#ifndef __GC_NEWITEM_INCLUDE__
#define __GC_NEWITEM_INCLUDE__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCNewItem : public Packet
{
public:
	GCNewItem( ){} ;
	virtual ~GCNewItem( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_NEWITEM ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t)*2 +
															sizeof(UINT) +
															sizeof(WORLD_POS); }

public:
	//ʹ�����ݽӿ�
	VOID			setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t			getObjID(VOID)const { return m_ObjID; }

	VOID			setAmount(UINT am) { m_uAmount = am; }
	UINT			getAmount(VOID)const { return m_uAmount; }

	VOID			setOwner(GUID_t id) { m_idOwner = id; }
	GUID_t			getOwner(VOID)const { return m_idOwner; }

	VOID				setWorldPos(WORLD_POS& pos) { m_posWorld = pos; }
	const WORLD_POS&	getWorldPos(VOID)const { return m_posWorld; }

private:
	//����

	ObjID_t			m_ObjID;		//��Ʒ��ObjID
	UINT			m_uAmount;		//��Ʒ����ֵ������������
	GUID_t			m_idOwner;		//��Ʒ���˵�GUID_t
	WORLD_POS		m_posWorld;		//��Ʒ��λ��

};


class GCNewItemFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCNewItem() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_NEWITEM; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t)*2 +
													sizeof(UINT) + 
													sizeof(WORLD_POS); }
};

class GCNewItemHandler 
{
public:
	static UINT Execute( GCNewItem* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif
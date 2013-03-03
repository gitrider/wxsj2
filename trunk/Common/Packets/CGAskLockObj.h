// CGAskLockObj.h
// 
// ����������Ʒ
// 
//////////////////////////////////////////////////////

#ifndef __CGASKLOCKOBJ_H__
#define __CGASKLOCKOBJ_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
class CGAskLockObj : public Packet
{
public:
	enum
	{
		LOCK_ITEM	=0,
		LOCK_PET,
	};
	enum
	{
		OPR_LOCK	=0,
		OPR_UNLOCK,
	};

	CGAskLockObj()
	{
	}
	virtual ~CGAskLockObj(){}

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_CGASKLOCKOBJ ; }
	virtual UINT			GetPacketSize()const { return sizeof(BYTE)*2 + sizeof(_ITEM_GUID) + sizeof(PET_GUID_t) ; }
	
public:
	//ʹ�����ݽӿ�
	_ITEM_GUID				GetItemGUID(VOID) const {return m_ItemGuid;}
	VOID					SetItemGUID(_ITEM_GUID Guid) {m_ItemGuid = Guid;}

	PET_GUID_t				GetPetGUID(VOID) const {return m_PetGuid;}
	VOID					SetPetGUID(PET_GUID_t PetGuid) {m_PetGuid = PetGuid;}

	BYTE					GetLockObj(){return m_bLockObj;}
	VOID					SetLockObj(BYTE bLockObj) {m_bLockObj=bLockObj;}

	BYTE					GetLockType(){return m_bLockType;}
	VOID					SetLockType(BYTE bLockType) {m_bLockType=bLockType;}

private:
	//����
	BYTE					m_bLockObj;		// ��ʲô���͵Ķ�������
	BYTE					m_bLockType;	// �Ǽ������ǽ���
	_ITEM_GUID				m_ItemGuid;
	PET_GUID_t				m_PetGuid;

};

class CGAskLockObjFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGAskLockObj() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_CGASKLOCKOBJ; }
	UINT		GetPacketMaxSize()const { return sizeof(BYTE)*2 + sizeof(_ITEM_GUID) + sizeof(PET_GUID_t) ; }
};

class CGAskLockObjHandler 
{
public:
	static UINT Execute( CGAskLockObj* pPacket, Player* pPlayer ) ;
};

}

using namespace Packets;



#endif
// GCDetailSubEffectList.h
// 
// ��������
// 
//////////////////////////////////////////////////////

#ifndef __GCDETAILSUBEFFECTLIST_H__
#define __GCDETAILSUBEFFECTLIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
#define MAX_PLAYER_BUFF_NUM		(16)
class GCDetailSubEffectList : public Packet
{
public:
	enum ENUM_BUFF_HANDLE
	{
		BUFF_HANDLE_INVALID	= -1,
		BUFF_HANDLE_ADD,			// ���
		BUFF_HANDLE_REMOVE,			// ɾ��
		BUFF_HANDLE_RESET,			// ���ò����
	};

	struct _BUFF_EFFECT
	{
		UINT	m_uID;					// Buff ID
		UINT	m_uResiduaryTime;		// ʣ��ʱ��
	};
public:
	GCDetailSubEffectList( ){}
	virtual ~GCDetailSubEffectList( ){}

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_DETAILSUBEFFECTLIST; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t); }

public:
	//VOID SetHandleType( BYTE yT
private:
	BYTE					m_nHandleType;		// �������� ENUM_BUFF_HANDLE
	BYTE					m_yEffectCount;
	_BUFF_EFFECT			m_aBuffEffect[MAX_PLAYER_BUFF_NUM];
};


class GCDetailSubEffectListFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCDetailSubEffectList() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_DETAILSUBEFFECTLIST; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t); }
};

class GCDetailSubEffectListHandler 
{
public:
	static UINT Execute( GCDetailSubEffectList* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif

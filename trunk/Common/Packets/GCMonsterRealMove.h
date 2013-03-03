#ifndef __GC_MONSTERREALMOVE_INCLUDE__
#define __GC_MONSTERREALMOVE_INCLUDE__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCMonsterRealMove : public Packet
{
public:
	GCMonsterRealMove( ){} ;
	virtual ~GCMonsterRealMove( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_MONSTERREALMOVE ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) +
															sizeof(WORLD_POS)*2 +
															sizeof(WORD); }

public:
	//ʹ�����ݽӿ�
	VOID			setObjID(ObjID_t id) { m_ObjID = id; }
	GUID_t			getObjID(VOID)const { return m_ObjID; }

	VOID			setRace(WORD race) { m_wRace = race; }
	WORD			getRace(VOID)const { return m_wRace; }

	VOID					setCurrentPos(WORLD_POS& pos) { m_posCur = pos; }
	const WORLD_POS&		getCurrentPos(VOID)const { return m_posCur; }

	VOID					setTargetPos(WORLD_POS& pos) { m_posTar = pos; }
	const WORLD_POS&		getTargetPos(VOID)const { return m_posTar; }


private:
	//����

	ObjID_t		m_ObjID;			//�����ID
	WORLD_POS	m_posCur;		//���ﵱǰ��λ��
	WORLD_POS	m_posTar;		//��������ǰ����λ��
	WORD		m_wRace;		//���������
};


class GCMonsterRealMoveFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCMonsterRealMove() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_MONSTERREALMOVE; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) +
													sizeof(WORLD_POS)*2 +
													sizeof(WORD); }
};

class GCMonsterRealMoveHandler 
{
public:
	static UINT Execute( GCMonsterRealMove* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif
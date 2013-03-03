#ifndef __GC_PLAYERREALMOVE_INCLUDE__
#define __GC_PLAYERREALMOVE_INCLUDE__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCPlayerRealMove : public Packet
{
public:
	GCPlayerRealMove( ){} ;
	virtual ~GCPlayerRealMove( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_PLAYERREALMOVE ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) +
															sizeof(WORLD_POS)*2 +
															sizeof(BYTE)*2 +
															sizeof(WORD); }

public:
	//ʹ�����ݽӿ�
	VOID			setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t			getObjID(VOID)const { return m_ObjID; }

	BYTE	getRace( ){ return m_byRace ;} ;
	VOID	setRace( BYTE race ){ m_byRace = race ; } ;

	BYTE	getStateFlag( ){ return m_byStateFlag ;} ;
	VOID	setStateFlag( BYTE flag ){ m_byStateFlag = flag ; } ;

	WORD	getEquipVer( ){ return m_wEquipVer ;} ;
	VOID	setEquipVer( WORD ver ){ m_wEquipVer = ver ; } ;

	VOID					setCurrentPos(WORLD_POS& pos) { m_posCur = pos; }
	const WORLD_POS&		getCurrentPos(VOID)const { return m_posCur; }

	VOID					setTargetPos(WORLD_POS& pos) { m_posTar = pos; }
	const WORLD_POS&		getTargetPos(VOID)const { return m_posTar; }


private:
	//����

	ObjID_t		m_ObjID;			//��ҵ�ObjID
	WORLD_POS	m_posCur;		//��ҵ�ǰ��λ��
	WORLD_POS	m_posTar;		//�������ǰ����λ��
	BYTE		m_byRace;		//��ҵ�����
	BYTE		m_byStateFlag;	//��ҵ�״̬��־
	WORD		m_wEquipVer;	//��ҵĵ�ǰװ���汾��

};


class GCPlayerRealMoveFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCPlayerRealMove() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_PLAYERREALMOVE; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) +
													sizeof(WORLD_POS)*2 +
													sizeof(BYTE)*2 +
													sizeof(WORD); }
};

class GCPlayerRealMoveHandler 
{
public:
	static UINT Execute( GCPlayerRealMove* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif
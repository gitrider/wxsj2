#ifndef __CGENTERSCENE_H__
#define __CGENTERSCENE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

#define ENTER_TYPE_FIRST		(0)	//0 ��¼���һ�ν��볡��
#define ENTER_TYPE_FROM_OTHER	(1)	//1 ����������ת��

class CGEnterScene : public Packet
{
public:
	CGEnterScene( ){} ;
	virtual ~CGEnterScene( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_ENTERSCENE ; }
	virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+ 
															sizeof(SceneID_t)+ 
															sizeof(WORLD_POS); }
	
public:
	//ʹ�����ݽӿ�
	VOID			setEnterType(BYTE byEnterType) { m_byEnterType = byEnterType; }
	BYTE			getEnterType(VOID)const { return m_byEnterType; }
	
	VOID			setSceneID(SceneID_t nSceneID) { m_nSceneID = nSceneID; }
	SceneID_t		getSceneID(VOID)const { return m_nSceneID; }

	VOID			setEnterPos(const WORLD_POS& posWorld) { m_posWorld = posWorld; }
	WORLD_POS*		getEnterPos(VOID) { return &m_posWorld; }

private:
	//����
	BYTE			m_byEnterType;	//��������
									//#define ENTER_TYPE_FIRST		(0)	 ��¼���һ�ν��볡��
									//#define ENTER_TYPE_FROM_OTHER	(1)	 ����������ת��
									//...
	SceneID_t		m_nSceneID;		//����ID
	WORLD_POS		m_posWorld;		//������X,Z�����
};

class CGEnterSceneFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGEnterScene() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_ENTERSCENE ; }
	UINT		GetPacketMaxSize()const { return	sizeof(BYTE)+ 
													sizeof(SceneID_t)+ 
													sizeof(WORLD_POS); }
};

class CGEnterSceneHandler 
{
public:
	static UINT Execute( CGEnterScene* pPacket, Player* pPlayer ) ;
};

}

using namespace Packets;

#endif
#ifndef __GCENTERSCENE_H__
#define __GCENTERSCENE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{


class GCEnterScene : public Packet
{
public:
	GCEnterScene( )
	{
		m_byRet			= 0;
		m_bIsCity		= 0;
		m_nCityLevel	= 0;
	}
	virtual ~GCEnterScene( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_ENTERSCENE ; }
	virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+ 
															sizeof(SceneID_t)+ 
															sizeof(ID_t)+															
															sizeof(WORLD_POS)+
															sizeof(ObjID_t) +
															sizeof(BYTE)*2; }
	
public:
	//ʹ�����ݽӿ�
	
	inline VOID			setReturn(BYTE byReturn)		{ m_byRet = byReturn; }
	inline BYTE			getReturn(VOID)const			{ return m_byRet; }

	inline VOID			setSceneID(SceneID_t nSceneID)	{m_nSceneID = nSceneID; }
	inline SceneID_t	getSceneID(VOID)const			{ return m_nSceneID; }

	VOID				setResID( ID_t resID ){ m_nResID = resID ; }
	ID_t				getResID( ){ return m_nResID ; }

	inline VOID			setEnterPos(const WORLD_POS& posWorld) { m_posWorld = posWorld; }
	inline WORLD_POS	getEnterPos(VOID)const { return m_posWorld; }

	VOID				setObjID( ObjID_t id ){ m_ObjID = id ; } ;
	ObjID_t				getObjID( ){ return m_ObjID ; } ;

	VOID				setIsCity( BYTE bIsCity ){ m_bIsCity = bIsCity; }
	BYTE				getIsCity( ){ return m_bIsCity; }

	VOID				setCityLevel( BYTE nCityLevel ){ m_nCityLevel = nCityLevel; }
	BYTE				getCityLevel( ){ return m_nCityLevel; }

private:
	//����
	BYTE				m_byRet;		//���������ؽ��
										//	0 - ȷ�Ͽ��Խ���ó���
										//  1 - ���û�н���ó��������
										//  2 - �Ƿ��ĳ���ID
										//  3 - ���������Ѿ�����
										//  ....
	SceneID_t			m_nSceneID;		//����ID
	ID_t				m_nResID;		//�ͻ�����Դ����
	WORLD_POS			m_posWorld;		//������X,Z�����
	ObjID_t				m_ObjID ;
	BYTE				m_bIsCity;		//�Ƿ��ǳ���
	BYTE				m_nCityLevel;	//���м���

};

//// To Try ...
//#define DECLEAR_FACTORY( PacketName, PacketID, PacketSize) \
//class PacketName##Factory : public PacketFactory \
//{ \
//public:\
//	Packet*		CreatePacket() { return new PacketName() ; } \
//	PacketID_t	GetPacketID()const { return Packet::PacketID ; } \
//	UINT		GetPacketMaxSize()const { return PacketSize; } \
//}; 
//
//DECLEAR_FACTORY( GCEnterScene, PACKET_GC_ENTERSCENE, (sizeof(UINT)+ sizeof(SceneID_t)+2*sizeof(Coord_t)) );


class GCEnterSceneFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCEnterScene() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_ENTERSCENE ; }
	UINT		GetPacketMaxSize()const { return	sizeof(BYTE)+ 
													sizeof(SceneID_t)+ 
													sizeof(WORLD_POS)+
													sizeof(BYTE)*2+
													sizeof(ObjID_t) +
													sizeof(ID_t); }
};

class GCEnterSceneHandler 
{
public:
	static UINT Execute( GCEnterScene* pPacket, Player* pPlayer ) ;
};

}

using namespace Packets;

#endif

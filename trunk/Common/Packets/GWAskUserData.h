

#ifndef __GWASKUSERDATA_H__
#define __GWASKUSERDATA_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class GWAskUserData : public Packet 
{
public:
	GWAskUserData( ){} ;
	virtual ~GWAskUserData( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream ) const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID() const { return PACKET_GW_ASKUSERDATA ; }
	virtual UINT			GetPacketSize() const { return	sizeof(PlayerID_t)+
															sizeof(GUID_t)+
															sizeof(CHAR)*MAX_ACCOUNT+
															sizeof(INT)+
															sizeof(INT)+
															sizeof(INT)+
															sizeof(INT)+
															sizeof(UINT) ; }
	
public:
	//ʹ�����ݽӿ�
	VOID					SetPlayerID( PlayerID_t pid ){ m_PlayerID = pid ; } ;
	PlayerID_t				GetPlayerID( ){ return m_PlayerID ; } ;

	GUID_t					GetGUID( ){ return m_GUID ; } ;
	VOID					SetGUID( GUID_t guid ){ m_GUID = guid ; } ;

	CHAR*					GetAccount( ){ return m_szAccount ; } ;
	VOID					SetAccount( CHAR* szAccount ){ 
		strncpy( m_szAccount, szAccount, MAX_ACCOUNT ) ;
		m_szAccount[MAX_ACCOUNT] = 0 ;
	}

	INT						GetGender(VOID) { return m_nGender; }
	VOID					SetGender(INT nGender) { m_nGender = nGender; }
	
	INT						GetCountry(VOID) { return m_nCountry; }
	VOID					SetCountry(INT nCountry) { m_nCountry = nCountry; }

	INT						GetProfession(VOID) { return m_nProfession; }
	VOID					SetProfession(INT nProfession) { m_nProfession = nProfession; }

	INT						GetFaceModel(VOID) { return m_nFaceModel; }
	VOID					SetFaceModel(INT nFaceModel) { m_nFaceModel = nFaceModel; }

	UINT					GetKey( ){ return m_uKey ; } ;
	VOID					SetKey( UINT key ){ m_uKey = key ; } ;


private:
	//����
	PlayerID_t				m_PlayerID ;	//��ǰ���ӵ���Һ�
	GUID_t					m_GUID ;		//��ҵ�GUID
	UINT					m_uKey ;		//��֤��

	//������
	CHAR					m_szAccount[MAX_ACCOUNT+1] ;
	INT						m_nGender;			//�Ա�
	INT                     m_nCountry;         //����
	INT                     m_nProfession;      //ְҵ
	INT                     m_nFaceModel;       //����
};


class GWAskUserDataFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GWAskUserData() ; }
	PacketID_t	GetPacketID() const { return PACKET_GW_ASKUSERDATA ; }
	UINT		GetPacketMaxSize() const { return	sizeof(PlayerID_t)+
													sizeof(GUID_t)+
													sizeof(CHAR)*MAX_ACCOUNT+
													sizeof(INT)+
													sizeof(INT)+
													sizeof(INT)+
													sizeof(INT)+
													sizeof(UINT) ; }
};


class GWAskUserDataHandler 
{
public:
	static UINT Execute( GWAskUserData* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif

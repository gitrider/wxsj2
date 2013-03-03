

#ifndef __GCRETSETTING_H__
#define __GCRETSETTING_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCRetSetting : public Packet 
	{
	public:
		GCRetSetting( ){} ;
		virtual ~GCRetSetting( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_RETSETTING ; }
		virtual UINT			GetPacketSize()const { return sizeof(_OWN_SETTING)*SETTING_TYPE_NUMBER + 
			sizeof(_SUIT_SETTING)*MAX_SUIT_SETTING; }

	public :

	public:
		//ʹ�����ݽӿ�
		VOID					SetSetting( _OWN_SETTING* pSetting ){
			memcpy( m_aSetting, pSetting, sizeof(_OWN_SETTING)*SETTING_TYPE_NUMBER ) ;
		};
		_OWN_SETTING*			GetSetting( ){ return m_aSetting ; } ;

		VOID					SetSuitSetting( _SUIT_SETTING* pSetting ){
			memcpy( m_SuitSetting, pSetting, sizeof(_SUIT_SETTING)*MAX_SUIT_SETTING ) ;
		};
		_SUIT_SETTING*			GetSuitSetting( ){ return m_SuitSetting ; } ;

	private:
		//����
		_OWN_SETTING			m_aSetting[SETTING_TYPE_NUMBER] ;
		_SUIT_SETTING			m_SuitSetting[MAX_SUIT_SETTING];
	};


	class GCRetSettingFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCRetSetting() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_RETSETTING ; }
		UINT		GetPacketMaxSize()const { return sizeof(_OWN_SETTING)*SETTING_TYPE_NUMBER +
			sizeof(_OWN_SETTING)*SETTING_TYPE_NUMBER + sizeof(_SUIT_SETTING)*MAX_SUIT_SETTING; }
	};


	class GCRetSettingHandler 
	{
	public:
		static UINT Execute( GCRetSetting* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif

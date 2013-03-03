

#ifndef __GCRETSUBTRAINSETTING_H__
#define __GCRETSUBTRAINSETTING_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCRetSubTrainSetting : public Packet 
	{
	public:
		GCRetSubTrainSetting( ){} ;
		virtual ~GCRetSubTrainSetting( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_RETSUBTRAINSETTING ; }
		virtual UINT			GetPacketSize()const { return sizeof(_SUBTRAIN_SETTING) ; }

	public :

	public:
		//使用数据接口
		VOID					SetSetting( _SUBTRAIN_SETTING* pSetting ){
			memcpy(&m_SubTrainSetting, pSetting, sizeof(_SUBTRAIN_SETTING) ) ;
		};
		_SUBTRAIN_SETTING&			GetSetting( ){ return m_SubTrainSetting ; } ;

	private:
		//数据
		_SUBTRAIN_SETTING			m_SubTrainSetting;
	};


	class GCRetSubTrainSettingFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCRetSubTrainSetting() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_RETSUBTRAINSETTING ; }
		UINT		GetPacketMaxSize()const { return sizeof(_SUBTRAIN_SETTING); }
	};


	class GCRetSubTrainSettingHandler 
	{
	public:
		static UINT Execute( GCRetSubTrainSetting* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif
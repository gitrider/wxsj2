#ifndef _GC_OPENBOXMANIPULATE_H_
#define _GC_OPENBOXMANIPULATE_H_
#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

	class GCOpenBoxManipulate : public Packet
	{
	public:
		enum
		{
			MANIPULATE_GET_DIRECTLY = 1,		//直接领取  
			MANIPULATE_ONCE_MORE,				//再来一次
			MANIPULATE_GET,						//领取
		};

		enum
		{
			MANIPULATE_NORMAL = 1,				//操作过程正常
			MANIPULATE_ERROR = -1,				//操作过程错误，客户端发的消息有问题
			MANIPULATE_GET_DIRECTLY_ERROR = -2,	//直接领取出错误
			MANIPULATE_ONCE_MORE_ERROR = -3,	//再来一次出错误	
			MANIPULATE_GET_ERROR = -4,			//领取出错误	
			MANIPULATE_BAG_FULL = -5,			//背包已满
		

		};

	public:
		GCOpenBoxManipulate(void)
			:
		m_nManipulateType(0),
		m_nRet(0)
		{

		}

		virtual ~GCOpenBoxManipulate(void){};

	public:
		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;//虚函数加const限制可能有问题
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_OPENBOXMANIPULATE; }
		virtual UINT			GetPacketSize() const { return sizeof(INT)
																+ sizeof(INT)
																+ sizeof(_ITEM_GUID); }

		VOID					SetManipulate( INT nManipulateType ) { m_nManipulateType = nManipulateType; };	
		INT						GetManipulate( VOID ) const { return m_nManipulateType; };
		VOID					SetRet( INT nRet ) { m_nRet = nRet; };
		INT						GetRet( VOID )	const	{ return m_nRet; };
		VOID					SetGUID( _ITEM_GUID GUID ) { m_GUID = GUID; };
		_ITEM_GUID				GetGUID( VOID )	const { return m_GUID; };

	private:
		INT		m_nManipulateType;		//操作的类型
		INT		m_nRet;
		_ITEM_GUID		m_GUID;


	};

	class GCOpenBoxManipulateFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCOpenBoxManipulate() ; }
		PacketID_t	GetPacketID() const { return PACKET_GC_OPENBOXMANIPULATE; }
		UINT		GetPacketMaxSize() const { return sizeof(INT)
														+ sizeof(INT)
														+ sizeof(_ITEM_GUID); }
	};

	class GCOpenBoxManipulateHandler 
	{
	public:
		static UINT Execute( GCOpenBoxManipulate* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;


#endif
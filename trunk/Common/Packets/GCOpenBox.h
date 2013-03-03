#ifndef _GCOPENBOX_H_
#define _GCOPENBOX_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class GCOpenBox : public Packet
	{
	public:
		enum			//返回状态
		{
			OPENBOX_NORMAL = 1,
			OPENBOX_ERROR = INVALID_ID,
			OPENBOX_HAVE_OPENED = -2,	//宝箱已经开启
		};
		enum
		{
			OPENBOX_ITEM_SIZE = 21
		};
	public:
		GCOpenBox( VOID )
			: m_nRet(0)
		{
			memset( m_Item, 0, sizeof(m_Item) );
		}

		virtual ~GCOpenBox( VOID ) { };

	public:
		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_OPENBOX; }
		virtual UINT			GetPacketSize() const 
		{ 
			UINT uSize = sizeof(INT);
			for( INT i=0; i<OPENBOX_ITEM_SIZE; ++i )
			{
				uSize += GetItemVarSize( m_Item[i] );
			}
			return  uSize;
		}

		VOID					SetRet( INT nRet ) { m_nRet = nRet; };
		INT						GetRet( VOID ) const { return m_nRet; };

		_ITEM*				GetItem( UINT uIndex )
		{
			if ( uIndex>=0 && uIndex<OPENBOX_ITEM_SIZE )
			{
				return &(m_Item[uIndex]) ;
			}
			else
			{
				Assert(FALSE);
			}
			return NULL;
		} 
	public:

	private:
		INT					m_nRet;
		_ITEM				m_Item[OPENBOX_ITEM_SIZE];
	};

	class GCOpenBoxFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCOpenBox() ; }
		PacketID_t	GetPacketID() const { return PACKET_GC_OPENBOX; }
		UINT		GetPacketMaxSize() const 
		{ 
			return sizeof(GCOpenBox) - sizeof(Packet);
		}
	};

	class GCOpenBoxHandler 
	{
	public:
		static UINT Execute( GCOpenBox* pPacket, Player* pPlayer ) ;
	};


};

using namespace Packets ;

#endif
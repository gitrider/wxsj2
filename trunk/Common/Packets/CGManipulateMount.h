// [7/8/2010 陈军龙]
#ifndef _CG_MANIPULATEMOUNT_H_
#define _CG_MANIPULATEMOUNT_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{

	class CGManipulateMount:public Packet
	{
	public:
		enum MANIPULATE_MOUNT_TYPE
		{ 
			MANIPULATE_EQUIP_MOUNT,     // 装备坐骑
			MANIPULATE_CALL_MOUNT,      // 召唤
			MANIPULATE_RECALL_MOUNT,    // 召回
			MANIPULATE_DISCARD_MOUNT,   // 丢弃
			MANIPULATE_DESTROY_MOUNT,   // 直接销毁
		};	

		enum MOUNT_BIND_TYPE
		{
			GETUP_BIND = 0,  		// 获得绑定
			EQUIP_BIND,				// 装备绑定  
		};

	public:
		CGManipulateMount(){};
		virtual				~CGManipulateMount(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_MANIPULATEMOUNT; }

		virtual UINT			GetPacketSize() const { return sizeof(BYTE) * 2 + sizeof(INT) + sizeof(MANIPULATE_MOUNT_TYPE); }

	public:
		VOID			        setMountIndex(BYTE index){m_MountIndex = index;}
		BYTE			        getMountIndex(){return m_MountIndex;}

		VOID			        setBagIndex( WORD index ){ m_BagIndex = index ; } ;
		BYTE			        getBagIndex( ){ return m_BagIndex ; } ;

		VOID			        setMountID( INT id ){ m_MountID = id ; } ;
		INT			            getMountID( ){ return m_MountID ; } ;

		VOID			        setMountOperator(MANIPULATE_MOUNT_TYPE op){m_MountOperator = op;}
		MANIPULATE_MOUNT_TYPE	getMountOperator(){return m_MountOperator;}


	private:
		
		BYTE			        m_MountIndex;	   // 使用Mount表中的位置存放的位置
		BYTE			        m_BagIndex;	       // 在背包中的位置
        INT                     m_MountID;         // charmount.tab 索引ID
        MANIPULATE_MOUNT_TYPE   m_MountOperator;   // 坐骑操作
	};


	class CGManipulateMountFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGManipulateMount(); }
		PacketID_t	GetPacketID() const { return PACKET_CG_MANIPULATEMOUNT; }
		UINT		GetPacketMaxSize() const { return	sizeof(BYTE) * 2  + sizeof(INT) + sizeof(CGManipulateMount::MANIPULATE_MOUNT_TYPE); }
	};


	class CGManipulateMountHandler
	{
	public:
		static UINT	Execute( CGManipulateMount* pPacket, Player* pPlayer );
	};

}


#endif
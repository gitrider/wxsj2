// [7/8/2010 �¾���]
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
			MANIPULATE_EQUIP_MOUNT,     // װ������
			MANIPULATE_CALL_MOUNT,      // �ٻ�
			MANIPULATE_RECALL_MOUNT,    // �ٻ�
			MANIPULATE_DISCARD_MOUNT,   // ����
			MANIPULATE_DESTROY_MOUNT,   // ֱ������
		};	

		enum MOUNT_BIND_TYPE
		{
			GETUP_BIND = 0,  		// ��ð�
			EQUIP_BIND,				// װ����  
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
		
		BYTE			        m_MountIndex;	   // ʹ��Mount���е�λ�ô�ŵ�λ��
		BYTE			        m_BagIndex;	       // �ڱ����е�λ��
        INT                     m_MountID;         // charmount.tab ����ID
        MANIPULATE_MOUNT_TYPE   m_MountOperator;   // �������
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
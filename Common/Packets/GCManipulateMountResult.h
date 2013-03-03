// [7/8/2010 陈军龙]
#ifndef _GC_MANIPULATEMOUNT_RESULT_H_
#define _GC_MANIPULATEMOUNT_RESULT_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
	class GCManipulateMountResult:public Packet
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
		GCManipulateMountResult(){};
		virtual				~GCManipulateMountResult(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_MANIPULATEMOUNTRESULT; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)*2 + sizeof(_ITEM_GUID)*2 + sizeof(UINT)*2 + sizeof(BOOL)*2  + sizeof(MANIPULATE_MOUNT_TYPE);
		}

	public:
		VOID			        setMountIndex(BYTE index){m_MountIndex = index;}
		BYTE			        getMountIndex(){return m_MountIndex;}

		VOID			        setBagIndex( WORD index ){ m_BagIndex = index ; } ;
		BYTE			        getBagIndex( ){ return m_BagIndex ; } ;

		VOID			        setBagItemId(_ITEM_GUID itemID){m_ItemID	= itemID;}
		_ITEM_GUID		        getItemId(){return m_ItemID;}

		VOID			        setMountGUID(_ITEM_GUID itemID){m_MountGUID = itemID;}
		_ITEM_GUID		        getMountGUID(){return m_MountGUID;}

		VOID			        setResult(BOOL Result){m_Result = Result;}
		BOOL			        getResult(){return m_Result;}

		VOID			        setMountObtainDate(UINT date){m_ObtainDate = date;}
		UINT			        getMountObtainDate(){return m_ObtainDate;}

		VOID			        setMountRemainTime(UINT remain){m_RemainTime = remain;}
		UINT			        getMountRemainTime(){return m_RemainTime;}

		VOID			        setMountIsBinded(BOOL bind ){m_IsBinded = bind;}
		BOOL			        getMountIsBinded(){return m_IsBinded;}

		VOID			        setMountOperator(MANIPULATE_MOUNT_TYPE op){m_MountOperator = op;}
		MANIPULATE_MOUNT_TYPE	getMountOperator(){return m_MountOperator;}

	private:
		BOOL			        m_Result;		  // 结果信息
		BYTE			        m_BagIndex;	      // 在背包中的位置
		BYTE			        m_MountIndex;	  // 在坐骑列表中的位置
		_ITEM_GUID		        m_ItemID;		 
		UINT			        m_ObtainDate;	  // 获取坐骑日期
		UINT                    m_RemainTime;     // 坐骑剩余使用时间
		_ITEM_GUID		        m_MountGUID;
		BOOL			        m_IsBinded;       // 是否已绑定
		MANIPULATE_MOUNT_TYPE   m_MountOperator;  // 坐骑操作
	};


	class GCManipulateMountResultFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket()      { return new GCManipulateMountResult(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_MANIPULATEMOUNTRESULT; }
		UINT		GetPacketMaxSize() const 
		{ 
			return sizeof(BYTE)*2 + sizeof(_ITEM_GUID)*2 + sizeof(UINT)*2 + sizeof(BOOL)*2 + sizeof(GCManipulateMountResult::MANIPULATE_MOUNT_TYPE);
		}
	};


	class GCManipulateMountResultHandler
	{
	public:
		static UINT Execute( GCManipulateMountResult* pPacket, Player* pPlayer );
	};
}



#endif
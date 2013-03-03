// GCDetailMountList.h  [7/8/2010 陈军龙]
// 
///////////////////////////////////////////////////////
#ifndef _GCDETAILMOUNTLIST_H_
#define _GCDETAILMOUNTLIST_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "CGAskDetailMountList.h"

namespace Packets
{
	class GCDetailMountList : public Packet
	{
	public:
		GCDetailMountList(){}
		virtual	~GCDetailMountList(){};


		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_DETAILMOUNTLIST ; }
		virtual UINT			GetPacketSize() const 
		{ 
			UINT	uSize = sizeof(ObjID_t) + sizeof(UINT);
			for(INT	i=0; i<MAX_MOUNT_SIZE; i++)
			{
				if( m_ItemIndexFlags & (1<<i) ) uSize += GetItemVarSize(m_ItemList[i]);
			}
			return uSize;
		}

	public:
		ObjID_t					getObjID(VOID) { return m_ObjID; }
		VOID					setObjID(ObjID_t idObj) { m_ObjID = idObj; }

		UINT					getMountIndexFlag(VOID) { return m_ItemIndexFlags; }
		VOID					setMountIndexFlag(UINT flag) { m_ItemIndexFlags = flag; }

		INT						getItemCount(VOID) 
		{
			return MAX_MOUNT_SIZE;
		}

		VOID					setMountData(INT nIndex,const	_ITEM* pMount)
		{
			memcpy(&m_ItemList[nIndex],pMount,sizeof(_ITEM));
			m_ItemIndexFlags |= (1 << nIndex);
		}

		_ITEM*					getMountData(VOID)
		{
			return	m_ItemList;
		}
		_ITEM*					getMountData(INT nIndex)
		{
			return  &m_ItemList[nIndex];
		}


	private:
		ObjID_t		    m_ObjID;	                      // 对方的ObjID
		UINT			m_ItemIndexFlags;		          // 每个位表示对应位置是否有坐骑，使用后五位
		_ITEM			m_ItemList[MAX_MOUNT_SIZE];
	};

	class GCDetailMountListFactory:public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCDetailMountList() ; }
		PacketID_t	GetPacketID() const { return PACKET_GC_DETAILMOUNTLIST; }
		UINT		GetPacketMaxSize() const { return	sizeof(ObjID_t) + sizeof(UINT) + sizeof(_ITEM)*MAX_MOUNT_SIZE; }
	};

	class GCDetailMountListHandler
	{
	public:
		static UINT	Execute( GCDetailMountList* pPacket, Player* pPlayer );
	};

}


#endif // _GCDETAILMOUNTLIST_H_

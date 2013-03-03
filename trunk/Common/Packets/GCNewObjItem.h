#ifndef _GC_NEW_OBJITEM_H_
#define _GC_NEW_OBJITEM_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

	class GCNewObjItem:public Packet
	{
	public:
		GCNewObjItem()
		{
			CleanUp();
		};
		VOID CleanUp()
		{
			m_nOwnerCount = 0;
			m_ObjID		= INVALID_ID;
			m_DropObjID = INVALID_ID;
			memset(m_idOwner, 0, sizeof(GUID_t)*MAX_TEAM_MEMBER);
		}
		virtual ~GCNewObjItem(){ CleanUp(); };

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_NEWOBJITEM ; }
		virtual UINT			GetPacketSize() const 
		{ 
			return	sizeof(BYTE)+sizeof(INT)+
					sizeof(ObjID_t)*2 +
					sizeof(UINT)*3+
					sizeof(GUID_t)*m_nOwnerCount+
					sizeof(WORLD_POS);
		}

	public:
		//使用数据接口
		VOID			setObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t			getObjID(VOID) const { return m_ObjID; }
		
		VOID			setMonsterID(ObjID_t id) { m_MonsterID = id; }
		ObjID_t			getMonsterID(VOID) const { return m_MonsterID; }

		VOID			setProtectTime(UINT nTime) { m_ProtectTime = nTime; }
		UINT			getProtectTime(VOID) const { return m_ProtectTime; }

		VOID			setItemIndex(UINT nItemIndex){ m_nItemIndex = nItemIndex;}
		UINT			getItemIndex(){ return m_nItemIndex; }
	
		VOID			setItemCount(BYTE nItemCount){ m_nItemCount = nItemCount;}
		BYTE			getItemCount(){ return m_nItemCount; }

		UINT			getItem() { return getItemIndex(); }
		VOID			setItem(UINT nItemIndex) { setItemIndex(nItemIndex); }

		VOID			setOwner(GUID_t Id) 
		{ 
			if ((Id == INVALID_ID)||(m_nOwnerCount==MAX_TEAM_MEMBER))
			{
				return;
			}
			m_idOwner[m_nOwnerCount] = Id;
			++m_nOwnerCount;
		}
		GUID_t			getOwner(INT nPos) const { Assert(nPos<MAX_TEAM_MEMBER); return m_idOwner[nPos]; }

		VOID			setOwnerCount(INT nCount){ Assert(nCount<=MAX_TEAM_MEMBER); m_nOwnerCount = nCount;}
		INT				getOwnerCount(){ return m_nOwnerCount;}

		VOID			setDropObjID(ObjID_t id){m_DropObjID = id;}
		ObjID_t			getDropObjID(){return m_DropObjID;}

		VOID				setWorldPos(const WORLD_POS& pos) { m_posWorld = pos; }
		const WORLD_POS&	getWorldPos(VOID) const { return m_posWorld; }

		VOID			setObjType(INT objType)
		{
			m_OBJType	=	objType;
		}

		const INT		getObjType()
		{
			return m_OBJType;
		}

	private:
		INT				m_OBJType;		//ObjItem的类型
		ObjID_t			m_ObjID;		//物品的ObjID
		UINT			m_ProtectTime;	//物品保护时间
		UINT			m_nItemIndex;
		BYTE			m_nItemCount;
		UINT			m_nOwnerCount;	//拥有者数量
		GUID_t			m_idOwner[MAX_TEAM_MEMBER];		//物品主人的ObjID,或者组队的ID
		ObjID_t			m_DropObjID;	//产生掉落包的objid
		WORLD_POS		m_posWorld;		//物品的位置
		ObjID_t			m_MonsterID;
	};


	class GCNewObjItemFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCNewObjItem() ; }
		PacketID_t	GetPacketID() const { return PACKET_GC_NEWOBJITEM; }
		UINT		GetPacketMaxSize() const 
		{ 
			return	sizeof(BYTE)+
					sizeof(INT) +
					sizeof(ObjID_t)*2 +
					sizeof(UINT)*3+
					sizeof(GUID_t)*MAX_TEAM_MEMBER+
					sizeof(WORLD_POS); 
		}
	};

	class GCNewObjItemHandler 
	{
	public:
		static UINT Execute( GCNewObjItem* pPacket, Player* pPlayer ) ;
	};


};

using namespace Packets;

#endif
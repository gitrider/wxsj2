// GCMissionList.h
// 
// ���������б�
// 
//////////////////////////////////////////////////////

#ifndef __GCMISSIONLIST_H__
#define __GCMISSIONLIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCMissionList : public Packet
	{
	public:

	public:
		GCMissionList( ){
			m_ObjID					= INVALID_ID;

			m_uMissionListFlags	= 0;
		}
		virtual ~GCMissionList( ){}

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_MISSIONLIST ; }
		virtual UINT			GetPacketSize()const 
		{
			UINT uSize = sizeof(m_ObjID);
			uSize += sizeof( m_uMissionListFlags );
			uSize += sizeof( _MISSION_LIST );
			return uSize;
		}

	public:
		VOID			SetObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t			GetObjID(VOID)const { return m_ObjID; }

		UINT GetMissionListFlags( VOID )const{
			return m_uMissionListFlags;
		}

		VOID AddMission( BYTE yIndex, const _OWN_MISSION *pMission ){
			if ( yIndex < MAX_CHAR_MISSION_NUM )
			{
				m_uMissionListFlags |= (0x00000001 << yIndex);
				m_listMission.m_aMission[yIndex]	= *pMission;
			}
		}

		const _OWN_MISSION *GetMission( BYTE yIndex )const{
			if ( yIndex < MAX_CHAR_MISSION_NUM )
				return &m_listMission.m_aMission[yIndex];
			else
				return NULL;
		}

		VOID *GetMissionBuf( VOID ){
			return (VOID*)&(m_listMission.m_aMission[0]);
		}


		VOID	SetMissionHaveDone(const UINT* pHaveDone)
		{
			memcpy(m_listMission.m_aMissionHaveDoneFlags,pHaveDone,sizeof(MAX_CHAR_MISSION_FLAG_LEN));
		}

		const UINT *GetMissionHaveDone(  )const
		{
				return m_listMission.m_aMissionHaveDoneFlags;
		}

		VOID				SetMissionList(_MISSION_LIST* pList)
		{
			memcpy(&m_listMission,pList,sizeof(_MISSION_LIST));
		
		}
		_MISSION_LIST*		GetMissionList(){ return &m_listMission; }

	private:
		ObjID_t				m_ObjID;

		UINT				m_uMissionListFlags;		// 1λ��ʾ1��Ҫˢ�µ�
		_MISSION_LIST		m_listMission;
	};

	class GCMissionListFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCMissionList() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_MISSIONLIST; }
		UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) + sizeof ( UINT )  + sizeof(_MISSION_LIST); }
	};

	class GCMissionListHandler 
	{
	public:
		static UINT Execute( GCMissionList* pPacket, Player* pPlayer ) ;
	};
}

#endif

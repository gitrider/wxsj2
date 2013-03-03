

#ifndef __GWASKSCENEDATA_H__
#define __GWASKSCENEDATA_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

#define MAX_EXTRAINFO_LENGTH 256
namespace Packets
{
	class GWAskSceneData : public Packet 
	{
	public:
		struct CITY_DATA
		{
			SceneID_t	m_PortSceneID;
			GuildID_t	m_GuildID;
			INT GetSize()
			{
				return sizeof(SceneID_t) + sizeof(GuildID_t);
			}
		};

	public:
		enum
		{
			TYPE_COPY = 0,
			TYPE_CITY,
			TYPE_BATTLE,
		};

	public:
		GWAskSceneData( )
		{
			m_SceneID	=	INVALID_ID;
			m_ClientRes	= 	INVALID_ID;
			m_Type		=	TYPE_COPY;
			m_bExtraLength = 0;
			memset(m_ExtraData, 0, MAX_EXTRAINFO_LENGTH);
		}
		virtual ~GWAskSceneData( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GW_ASKSCENEDATA ; }
		virtual UINT			GetPacketSize()const 
		{
			return sizeof(SceneID_t) + sizeof(ID_t) + sizeof(BYTE)*2 + m_bExtraLength; 
		}

	public:
		//ʹ�����ݽӿ�
		VOID			SetSceneID(SceneID_t SceneID){ m_SceneID = SceneID; }
		SceneID_t		GetSceneID(VOID) const { return m_SceneID; }

		VOID			SetClientRes(ID_t ClientRes){ m_ClientRes = ClientRes; }
		ID_t			GetClientRes(VOID) const { return m_ClientRes; }

		VOID			SetType(BYTE bType){ m_Type = bType; }
		BYTE			GetType(VOID) const { return m_Type; }

		VOID			SetExtraDataLength(BYTE bExtraLength){ m_bExtraLength = bExtraLength; }
		BYTE			GetExtraDataLength(VOID) const { return m_bExtraLength; }

		VOID			SetExtraData(CHAR* pExtraData)
		{
			memcpy(m_ExtraData, pExtraData, m_bExtraLength);
		}
		CHAR*			GetExtraData(VOID){ return m_ExtraData; }

	private:
		//����
		SceneID_t				m_SceneID;
		ID_t					m_ClientRes;
		BYTE					m_Type;
		BYTE					m_bExtraLength;
		CHAR					m_ExtraData[MAX_EXTRAINFO_LENGTH];
	};

	class GWAskSceneDataFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GWAskSceneData() ; }
		PacketID_t	GetPacketID()const { return PACKET_GW_ASKSCENEDATA ; }
		UINT		GetPacketMaxSize()const { return sizeof(SceneID_t) + MAX_EXTRAINFO_LENGTH + sizeof(BYTE)*2 + sizeof(ID_t); }
	};

	class GWAskSceneDataHandler 
	{
	public:
		static UINT Execute( GWAskSceneData* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif

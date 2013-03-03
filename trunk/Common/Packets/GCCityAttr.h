// GCCityAttr.h
// 
// �������뽨���µĳ���
// 
//////////////////////////////////////////////////////

#ifndef __GCCITYATTR_H__
#define __GCCITYATTR_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCCityAttr : public Packet 
	{
	public:
		GCCityAttr( )
		{
			m_uFlags	=	0;								//ÿ��λ��ʾһ�������Ƿ�Ҫˢ�� ENUM_UPDATE_CITY_ATT
			m_Level		=	0;								//���м���
			memset(m_szCityName, 0, MAX_CITY_NAME_SIZE);	//��������
			m_BuildingIndex	=	0;							//��������
			m_BuildingData.CleanUp();						//����������
		}
		virtual ~GCCityAttr( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_CITYATTR; }
		virtual UINT			GetPacketSize() const 
		{
			UINT uAttribSize = sizeof(UINT);

			if ( m_uFlags & (1 << CITY_UPDATE_CITY_LEVEL) )
				uAttribSize += sizeof(BYTE);

			if ( m_uFlags & (1 << CITY_UPDATE_CITY_NAME) )
				uAttribSize += sizeof(CHAR)*MAX_CITY_NAME_SIZE;

			if ( m_uFlags & (1 << CITY_UPDATE_BUILDING) )
			{
				uAttribSize += sizeof(BYTE);
				uAttribSize += sizeof(CityBuilding_t);
			}
			return uAttribSize; 
		}

	public:
		UINT					getFlags( VOID )const{ return m_uFlags; }

		BOOL					IsUpdateAttrib( ENUM_UPDATE_CITY_ATT eAttrib )const{ return (m_uFlags & (1<<(INT)eAttrib))?(TRUE):(FALSE); }
		VOID					SetUpdateAttrib( ENUM_UPDATE_CITY_ATT eAttrib, BOOL bUpdate )
		{
			if ( bUpdate )
				m_uFlags |= (1<<(INT)eAttrib);
			else
				m_uFlags &= (~(1<<(INT)eAttrib));
		}

		BYTE					GetSceneID(){return m_Level;}
		VOID					SetSceneID(BYTE nLevel){m_Level = nLevel;}

		CHAR*					GetCityName(){return m_szCityName;}
		VOID					SetCityName(CHAR* pszCityName)
		{
			memcpy(m_szCityName, pszCityName, MAX_CITY_NAME_SIZE);
		}

		BYTE					GetBuildingIndex(){return m_BuildingIndex;}
		VOID					SetBuildingIndex(BYTE nBuildingIndex){m_BuildingIndex = nBuildingIndex;}

		CityBuilding_t			GetBuildingData(){return m_BuildingData;}
		VOID					SetBuildingData(CityBuilding_t BuildingData){m_BuildingData = BuildingData;}

	private:
		UINT					m_uFlags;							//ÿ��λ��ʾһ�������Ƿ�Ҫˢ�� ENUM_UPDATE_CITY_ATT
		BYTE					m_Level;							//���м���
		CHAR					m_szCityName[MAX_CITY_NAME_SIZE];	//��������
		BYTE					m_BuildingIndex;					//��������
		CityBuilding_t			m_BuildingData;						//����������
	};

	class GCCityAttrFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCCityAttr() ; }
		PacketID_t	GetPacketID() const { return PACKET_GC_CITYATTR; }
		UINT		GetPacketMaxSize() const { return	sizeof(UINT)+
														sizeof(BYTE)+
														sizeof(CHAR)*MAX_CITY_NAME_SIZE+
														sizeof(BYTE)+
														sizeof(CityBuilding_t);}
	};

	class GCCityAttrHandler 
	{
	public:
		static UINT Execute( GCCityAttr* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif

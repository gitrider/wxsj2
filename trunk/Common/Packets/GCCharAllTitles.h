#ifndef __GCCHARALLTITLES_H__
#define __GCCHARALLTITLES_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class	GCCharAllTitles : public Packet 
	{
    public:
        enum
        {
            UPDATE_ALL = 0,                 //�������гƺ���Ϣ
            UPDATE_BANGPAI_TITLE_NAME,      //���°��ɳƺ�����
            UPDATE_CUR_BANGPAI_TITLE,       //���µ�ǰ���ɳƺ�ID
            UPDATE_CUR_GUOJIA_TITLE,        //���µ�ǰ���ҳƺ�ID
            UPDATE_CUR_WANFA_TITLE,         //���µ�ǰ�淨�ƺ�ID
        };

	public:
		GCCharAllTitles( )
        {
            m_cType = 0;
        } ;
		virtual ~GCCharAllTitles( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHARALLTITLES ; }
		virtual UINT			GetPacketSize()const 
		{
            return sizeof(_TITLE) + sizeof(BYTE);
		}

	public:
		//ʹ�����ݽӿ�
		_TITLE*     			GetTitleInfo(VOID) { return &m_TitleInfo; }
		VOID					SetTitleInfo(_TITLE* pTitle)
		{
			memcpy(&m_TitleInfo, pTitle, sizeof(_TITLE));
		}

        BYTE                    GetType() { return m_cType; };
        VOID                    SetType(BYTE cType) { m_cType = cType; };
	private:
        
        BYTE        m_cType;
        _TITLE      m_TitleInfo;
	};


	class GCCharAllTitlesFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCCharAllTitles() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CHARALLTITLES ; }
		UINT		GetPacketMaxSize()const { return sizeof(_TITLE) + sizeof(BYTE);  }
	};


	class GCCharAllTitlesHandler 
	{
	public:
		static UINT Execute( GCCharAllTitles* pPacket, Player* pPlayer ) ;
	};
};
using namespace Packets ;

#endif

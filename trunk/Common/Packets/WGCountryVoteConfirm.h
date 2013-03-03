/********************************************************************************
*	�ļ�����	WGCountryVoteConfirm.h
*	ȫ·����	d:\Prj\Server\Common\Packets\WGCountryVoteConfirm.h
*	�����ˣ�	��һ��
*	����ʱ�䣺	2008 �� 5 �� 30 ��	13:09
*
*	����˵����	
*	�޸ļ�¼��
*********************************************************************************/


#ifndef __WGCOUNTRYVOTECONFIRM_H__
#define __WGCOUNTRYVOTECONFIRM_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class WGCountryVoteConfirm : public Packet 
    {
    public:
        WGCountryVoteConfirm( ){} ;
        virtual ~WGCountryVoteConfirm( ){} ;

        //���ü̳нӿ�
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_WG_COUNTRY_VOTE_CONFIRM ; }
        virtual UINT			        GetPacketSize()const { return sizeof(GUID_t) + sizeof(m_PlayerID) + sizeof(INT)*2 + sizeof(BYTE); }

        VOID					        SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
        PlayerID_t				        GetPlayerID( ) { return m_PlayerID; }

        VOID                            SetGuid(GUID_t guid) { m_Guid = guid; };
        GUID_t                          GetGuid() { return m_Guid; };

        VOID                            SetCurNum (BYTE cNum) { m_cCurNum = cNum; };
        BYTE                            GetCurNum () { return m_cCurNum; };

        VOID                            SetTime(INT nTime) { m_nLeftTime = nTime; };
        INT                             GetTime() { return m_nLeftTime; };

        VOID                            SetPercent(INT nPercent) { m_nPercent = nPercent; };
        INT                             GetPercent() { return m_nPercent; };

    private:
        //����
        PlayerID_t				        m_PlayerID;	//������
        GUID_t                          m_Guid;     //У��
        BYTE                            m_cCurNum;  //��ǰƱ��
        INT                             m_nLeftTime;
        INT                             m_nPercent;
    };


    class WGCountryVoteConfirmFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new WGCountryVoteConfirm() ; }
        PacketID_t	GetPacketID()const { return PACKET_WG_COUNTRY_VOTE_CONFIRM ; }
        UINT		GetPacketMaxSize()const { return sizeof(GUID_t) + sizeof(PlayerID_t) + sizeof(INT)*2 + sizeof(BYTE); }
    };


    class WGCountryVoteConfirmHandler 
    {
    public:
        static UINT Execute( WGCountryVoteConfirm* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif

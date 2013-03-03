/********************************************************************************
*	�ļ�����	GCCountryVoteShow.h
*	ȫ·����	d:\Prj\Server\Common\Packets\GCCountryVoteShow.h
*	�����ˣ�	ZG
*	����ʱ�䣺	[ 2008-5-30 11:00 ]	
*
*	����˵����	
*	�޸ļ�¼��
*********************************************************************************/


#ifndef __GCCOUNTRYVOTESHOW_H__
#define __GCCOUNTRYVOTESHOW_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class GCCountryVoteShow : public Packet 
    {
    public:
        GCCountryVoteShow( ){} ;
        virtual ~GCCountryVoteShow( ){} ;

        //���ü̳нӿ�
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_GC_COUNTRY_VOTE_SHOW ; }
        virtual UINT			        GetPacketSize()const { return  sizeof(UINT)*2 + sizeof(float); }

        VOID                            SetCurVotes (UINT nNum) { m_nCurVotes = nNum ;}
        BYTE                            GetCurVotes () { return m_nCurVotes; }

		VOID							SetLeftTime( UINT nTime ) { m_nLeftTime = nTime; }
		UINT                            GetLeftTime() { return m_nLeftTime; }

		VOID                            SetPercentVote( float fPer ){ m_fPercentVote = fPer; }
		float							GetPercentVote(){ return m_fPercentVote; }

    private:
        UINT                            m_nCurVotes;   //��ǰƱ����
        float                           m_fPercentVote;  //��ǰͶƱ��
		UINT                            m_nLeftTime;     //��ǰʣ��ʱ��
   
    };


    class GCCountryVoteShowFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GCCountryVoteShow() ; }
        PacketID_t	GetPacketID()const { return PACKET_GC_COUNTRY_VOTE_SHOW ; }
        UINT		GetPacketMaxSize()const { return sizeof(UINT)*2 + sizeof(float); }
    };


    class GCCountryVoteShowHandler 
    {
    public:
        static UINT Execute( GCCountryVoteShow* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif

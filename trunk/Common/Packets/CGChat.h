

#ifndef __CGCHAT_H__
#define __CGCHAT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

#define MAX_CHAT_SIZE 256

namespace Packets
{


	class CGChat : public Packet 
	{
	public:
		CGChat( ){} ;
		virtual ~CGChat( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_CHAT ; }
		virtual UINT			GetPacketSize()const 
		{
			Assert( m_ContexSize<=MAX_CHAT_SIZE ) ;
			INT iSize = sizeof(BYTE)+sizeof(BYTE)+m_ContexSize*sizeof(CHAR) ;
			switch( m_ChatType )
			{
			case CHAT_TYPE_NEAR:
			case CHAT_TYPE_WORLD:
			case CHAT_TYPE_SYSTEM:
				break ;
			case CHAT_TYPE_TEAM:
				iSize += sizeof(TeamID_t) ;
				break ;
			case CHAT_TYPE_USER:
				Assert( m_TargetSize<=MAX_CHARACTER_NAME ) ;
				iSize += (sizeof(BYTE)+m_TargetSize*sizeof(CHAR)) ;
				break ;
			case CHAT_TYPE_GUILD:
				iSize += sizeof(GuildID_t) ;
				break ;
			case CHAT_TYPE_JIAZU:
				iSize += sizeof(GuildID_t) ;
				iSize += sizeof(JiaZuID_t) ;
				break ;
			case CHAT_TYPE_COUNTRY:
				iSize += sizeof(CountryID_t);
				break;
			case CHAT_TYPE_DMA_AGREE:
			case CHAT_TYPE_DMA_REQUEST:
			case CHAT_TYPE_DMA_CANCEL:
			case CHAT_TYPE_DMA_FORCE:
			case CHAT_TYPE_DMA_REFUSE:
				iSize += sizeof(ObjID_t);
				break;
			default :
				Assert(FALSE) ;
				break ;
			}

			return (UINT)iSize ; 
		}

	public:
		//ʹ�����ݽӿ�
		VOID					SetChatType( BYTE bType ){ m_ChatType = bType ; } ;
		BYTE					GetChatType( ){ return m_ChatType ; } ;

		VOID					SetContexSize( BYTE bSize ){ m_ContexSize = bSize ; } ;
		BYTE					GetContexSize( ){ return m_ContexSize ; } ;

		VOID					SetContex( const CHAR* pContex ){
			strncpy( m_Contex, pContex, MAX_CHAT_SIZE-1 ) ;
		};
		CHAR*					GetContex(){ return (CHAR*)m_Contex ; } ;

		VOID					SetTargetSize( BYTE bSize ){ m_TargetSize = bSize ; } ;
		BYTE					GetTargetSize( ){ return m_TargetSize ; } ;

		VOID					SetTargetName( const CHAR* pTargetName ){
			strncpy( m_TargetName, pTargetName, MAX_CHARACTER_NAME-1 ) ;
		};
		CHAR*					GetTargetName(){ return (CHAR*)m_TargetName ; } ;

		VOID					SetTeamID( TeamID_t tid ){ m_TeamID = tid ; } ;
		TeamID_t				GetTeamID(){ return m_TeamID ; } ;

		VOID					SetChannelID( ChannelID_t cid ){ m_ChannelID = cid ; };
		ChannelID_t				GetChannelID( ){ return m_ChannelID ; } ;

		VOID					SetGuildID( GuildID_t tid ){ m_GuildID = tid ; } ;
		GuildID_t				GetGuildID(){ return m_GuildID ; } ;

		VOID					SetMenpaiID( MenPaiID_t menpai ){ m_MenpaiID = menpai ; } ;
		MenPaiID_t					GetMenpaiID(){ return m_MenpaiID ; } 

		VOID					SetJiaZuID( JiaZuID_t tid ){ m_JiaZuID = tid ; } ;
		JiaZuID_t				GetJiaZuID(){ return m_JiaZuID ; } ;

		VOID					SetCountryID( CountryID_t country ){ m_CountryID = country ; } ;
		CountryID_t					GetCountryID(){ return m_CountryID ; } ;

		VOID					SetDestID(ObjID_t value) {m_DestID = value;}
		ObjID_t					GetDestID() const {return m_DestID;}

	private:
		//����
		BYTE					m_ChatType ;//������Ϣ���� enum CHAT_TYPE
		
		//������������
		BYTE					m_ContexSize ;
		CHAR					m_Contex[MAX_CHAT_SIZE] ;
		
		//˽�Ķ���Ľ�ɫ���֣�����CHAT_TYPE_TELLʱ��Ч
		BYTE					m_TargetSize ;
		CHAR					m_TargetName[MAX_CHARACTER_NAME] ;
		//����ţ����ڶ�������ʱ��Ч
		TeamID_t				m_TeamID ;
		//Ƶ���ţ������Խ�����Ƶ������ʱ��Ч
		ChannelID_t				m_ChannelID ;
		//���ɺţ������ڴ˰��ɵĳ�Ա��Ч
		GuildID_t				m_GuildID ;
		//����ֵ�����������ڵĳ�Ա��Ч
		MenPaiID_t					m_MenpaiID ;
		//����
		JiaZuID_t				m_JiaZuID;
		//����
		CountryID_t			m_CountryID;

		ObjID_t				m_DestID; //Ŀ����ҵ�ObjID

	};


	class CGChatFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGChat() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_CHAT ; }
		UINT		GetPacketMaxSize()const { return	sizeof(BYTE)+
														sizeof(BYTE)+
														sizeof(CHAR)*MAX_CHAT_SIZE+
														sizeof(BYTE)+
														sizeof(CHAR)*MAX_CHARACTER_NAME+
														sizeof(TeamID_t)+
														sizeof(ChannelID_t)+
														sizeof(GuildID_t)+
														sizeof(BYTE) +
														sizeof(ObjID_t); }
	};


	class CGChatHandler 
	{
	public:
		static UINT Execute( CGChat* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif

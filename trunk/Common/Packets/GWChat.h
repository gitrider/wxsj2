

#ifndef __GWChat_H__
#define __GWChat_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "CGChat.h"


namespace Packets
{
	class GWChat : public Packet 
	{
	public:
		GWChat( ){ m_ChatShowPos = CHAT_MAIN_WINDOW; } ;
		virtual ~GWChat( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GW_CHAT ; }
		virtual UINT			GetPacketSize()const 
		{ 
			Assert( m_ContexSize<=MAX_CHAT_SIZE ) ;
			INT iSize = sizeof(GUID_t)+
						sizeof(BYTE)+
						sizeof(BYTE)+
						sizeof(CHAR)*m_ContexSize +
						sizeof(BYTE);

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
				iSize += sizeof(JiaZuID_t) ;
				break ;
			case CHAT_TYPE_COUNTRY:
				iSize += sizeof(CountryID_t) ;
				break ;

			default :
				Assert(FALSE) ;
				break ;
			}

			return (UINT)iSize ; 
		}

	public :

	public:
		//ʹ�����ݽӿ�
		VOID					SetSourGUID( GUID_t guid ){ m_SourGUID = guid ; } ;
		GUID_t					GetSourGUID( ){ return m_SourGUID ; } ;

		VOID					SetChatType( BYTE bType ){ m_ChatType = bType ; } ;
		BYTE					GetChatType( ){ return m_ChatType ; } ;

		VOID					SetContexSize( BYTE bSize ){ m_ContexSize = bSize ; } ;
		BYTE					GetContexSize( ){ return m_ContexSize ; } ;

		VOID					SetContex(const CHAR* pContex ){
			strncpy( m_Contex, pContex, m_ContexSize ) ;
		};
		CHAR*					GetContex(){ return (CHAR*)m_Contex ; } ;

		VOID					SetTargetSize( BYTE bSize ){ m_TargetSize = bSize ; } ;
		BYTE					GetTargetSize( ){ return m_TargetSize ; } ;

		VOID					SetTargetName( CHAR* pTargetName ){
			strncpy( m_TargetName, pTargetName, MAX_CHARACTER_NAME-1 ) ;
		};
		CHAR*					GetTargetName(){ return (CHAR*)m_TargetName ; } ;

		VOID					SetTeamID( TeamID_t tid ){ m_TeamID = tid ; } ;
		TeamID_t				GetTeamID(){ return m_TeamID ; } ;

		VOID					SetGuildID( GuildID_t tid ){ m_GuildID = tid ; } ;
		GuildID_t				GetGuildID(){ return m_GuildID ; } ;
	
		VOID					SetJiaZuID( JiaZuID_t tid ){ m_JiaZuID = tid ; } ;
		JiaZuID_t				GetJiaZuID(){ return m_JiaZuID ; } ;

		VOID					SetCountryID( CountryID_t country ){ m_JiaZuID = country ; } ;
		CountryID_t					GetCountryID(){ return m_CountryID ; } ;

		VOID					SetChatShowPos( BYTE ShowPos ){ m_ChatShowPos = ShowPos ; } ;
		BYTE					GetChatShowPos( ){ return m_ChatShowPos ; } ;

	private:
		//˵�������
		GUID_t					m_SourGUID ;

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

		//���ɺţ������ڴ˰��ɵĳ�Ա��Ч
		GuildID_t				m_GuildID ;

		//����
		JiaZuID_t				m_JiaZuID;
		//����
		CountryID_t				m_CountryID;
			
		BYTE					m_ChatShowPos;	//ָ���ͻ�����ʾ������Ϣ��λ��

	};


	class GWChatFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GWChat() ; }
		PacketID_t	GetPacketID()const { return PACKET_GW_CHAT ; }
		UINT		GetPacketMaxSize()const { return	sizeof(GUID_t)+
														sizeof(BYTE)+
														sizeof(BYTE)+
														sizeof(CHAR)*MAX_CHAT_SIZE+
														sizeof(BYTE)+
														sizeof(CHAR)*MAX_CHARACTER_NAME+
														sizeof(TeamID_t)+
														sizeof(GuildID_t)+
														sizeof(JiaZuID_t)+
														sizeof(CountryID_t)+
														sizeof(BYTE) +
														sizeof(BYTE); }
	};


	class GWChatHandler 
	{
	public:
		static UINT Execute( GWChat* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif

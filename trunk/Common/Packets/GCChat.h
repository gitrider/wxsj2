

#ifndef __GCCHAT_H__
#define __GCCHAT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "CGChat.h"


namespace Packets
{

#define MAX_CHAT_SIZE 256

	class GCChat : public Packet 
	{
	public:
		GCChat( ){ Reset() ; } ;
		virtual ~GCChat( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHAT ; }
		virtual UINT			GetPacketSize()const 
		{
			UINT uSize = 0;
			switch(m_ChatType)
			{
			case CHAT_TYPE_DMA_FORCE:
			case CHAT_TYPE_DMA_REQUEST:
			case CHAT_TYPE_DMA_AGREE:
			case CHAT_TYPE_DMA_REFUSE:
			case CHAT_TYPE_DMA_CANCEL:
				uSize = sizeof(BYTE) +
					sizeof(BYTE) + 
					m_ContexSize*sizeof(CHAR)+
					sizeof(ObjID_t) +
					sizeof(ObjID_t);
				break;
			case CHAT_TYPE_NEAR:
				uSize = sizeof(BYTE)+
					sizeof(BYTE)+
					m_ContexSize*sizeof(CHAR)+
					sizeof(BYTE)+
					m_SourNameSize*sizeof(CHAR)+
					sizeof(ObjID_t)+
					sizeof(UINT) +
					sizeof(BYTE); 
				break;
			default:
				uSize = sizeof(BYTE)+
					sizeof(BYTE)+
					m_ContexSize*sizeof(CHAR)+
					sizeof(BYTE)+
					m_SourNameSize*sizeof(CHAR)+
					sizeof(UINT) +
					sizeof(BYTE); 
				break;
			}
			return uSize;
		}

		VOID	Reset()
		{
			m_ChatType = 0 ;
			m_ContexSize = 0 ;
			m_SourNameSize = 0 ;
			memset( m_Contex, 0, MAX_CHAT_SIZE ) ;
			memset( m_SourName, 0, MAX_CHARACTER_NAME ) ;
			m_ChatShowPos = CHAT_MAIN_WINDOW;
		};

	public:
		//使用数据接口

		//设置、读取聊天类型 见：CGChat::CHAT_TYPE
		VOID					SetChatType( BYTE bType ){ m_ChatType = bType ; } ;
		BYTE					GetChatType( ){ return m_ChatType ; } ;

		//设置、读取聊天内容
		VOID					SetContexSize( BYTE bSize ){ m_ContexSize = bSize ; } ;
		BYTE					GetContexSize( ){ return m_ContexSize ; } ;

		VOID					SetContex( CHAR* pContex ){
			strncpy( m_Contex, pContex, MAX_CHAT_SIZE-1 ) ;
		};
		CHAR*					GetContex(){ return (CHAR*)m_Contex ; } ;

		//设置、读取说话人的名字
		VOID					SetSourNameSize( BYTE bSize ){ m_SourNameSize = bSize ; } ;
		BYTE					GetSourNameSize( ){ return m_SourNameSize ; } ;

		VOID					SetSourName( const	CHAR* pSourName ){
			strncpy( m_SourName, pSourName, MAX_CHARACTER_NAME-1 ) ;
		};
		CHAR*					GetSourName(){ return (CHAR*)m_SourName ; } ;

		VOID					SetSourID( ObjID_t id ){ m_SourID = id ; }
		ObjID_t					GetSourID( ){ return m_SourID ; }

		VOID					SetWorldChatID( UINT uid ){ m_uWorldChatID = uid ; }
		UINT					GetWorldChatID( ){ return m_uWorldChatID ; }

		VOID					SetChatShowPos( BYTE ShowPos ){ m_ChatShowPos = ShowPos ; } ;
		BYTE					GetChatShowPos( ){ return m_ChatShowPos ; } ;
		VOID					SetDestID(ObjID_t value) {m_DestID = value;}
		ObjID_t					GetDestID() const {return m_DestID;}

	private:
		//数据
		BYTE					m_ChatType ;//聊天消息类型 enum CGChat::CHAT_TYPE
		
		//聊天内容数据
		BYTE					m_ContexSize ;
		CHAR					m_Contex[MAX_CHAT_SIZE] ;

		//说话者名字
		BYTE					m_SourNameSize ;
		CHAR					m_SourName[MAX_CHARACTER_NAME] ;

		//说话者的ObjID, 普通说话中有效
		ObjID_t					m_SourID ;

		UINT					m_uWorldChatID ; //聊天消息的序列号，用于纠正玩家转移场景消息丢失问题
		
		BYTE					m_ChatShowPos;	//指定客户端显示聊天消息的位置
		ObjID_t				m_DestID; // 目标玩家的ObjID,跟接受消息的玩家的ObjID是两码事 [6/5/2008 wuwenbin]
	};


	class GCChatFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCChat() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CHAT ; }
		UINT		GetPacketMaxSize()const { return 	sizeof(BYTE)+
														sizeof(BYTE)+
														sizeof(CHAR)*MAX_CHAT_SIZE+
														sizeof(BYTE)+
														sizeof(CHAR)*MAX_CHARACTER_NAME+
														sizeof(ObjID_t)+
														sizeof(ObjID_t)+
														sizeof(UINT) +
														sizeof(BYTE); }
	};


	class GCChatHandler 
	{
	public:
		static UINT Execute( GCChat* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif

#ifndef _GC_OPENBOXMANIPULATE_H_
#define _GC_OPENBOXMANIPULATE_H_
#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

	class GCOpenBoxManipulate : public Packet
	{
	public:
		enum
		{
			MANIPULATE_GET_DIRECTLY = 1,		//ֱ����ȡ  
			MANIPULATE_ONCE_MORE,				//����һ��
			MANIPULATE_GET,						//��ȡ
		};

		enum
		{
			MANIPULATE_NORMAL = 1,				//������������
			MANIPULATE_ERROR = -1,				//�������̴��󣬿ͻ��˷�����Ϣ������
			MANIPULATE_GET_DIRECTLY_ERROR = -2,	//ֱ����ȡ������
			MANIPULATE_ONCE_MORE_ERROR = -3,	//����һ�γ�����	
			MANIPULATE_GET_ERROR = -4,			//��ȡ������	
			MANIPULATE_BAG_FULL = -5,			//��������
		

		};

	public:
		GCOpenBoxManipulate(void)
			:
		m_nManipulateType(0),
		m_nRet(0)
		{

		}

		virtual ~GCOpenBoxManipulate(void){};

	public:
		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;//�麯����const���ƿ���������
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_OPENBOXMANIPULATE; }
		virtual UINT			GetPacketSize() const { return sizeof(INT)
																+ sizeof(INT)
																+ sizeof(_ITEM_GUID); }

		VOID					SetManipulate( INT nManipulateType ) { m_nManipulateType = nManipulateType; };	
		INT						GetManipulate( VOID ) const { return m_nManipulateType; };
		VOID					SetRet( INT nRet ) { m_nRet = nRet; };
		INT						GetRet( VOID )	const	{ return m_nRet; };
		VOID					SetGUID( _ITEM_GUID GUID ) { m_GUID = GUID; };
		_ITEM_GUID				GetGUID( VOID )	const { return m_GUID; };

	private:
		INT		m_nManipulateType;		//����������
		INT		m_nRet;
		_ITEM_GUID		m_GUID;


	};

	class GCOpenBoxManipulateFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCOpenBoxManipulate() ; }
		PacketID_t	GetPacketID() const { return PACKET_GC_OPENBOXMANIPULATE; }
		UINT		GetPacketMaxSize() const { return sizeof(INT)
														+ sizeof(INT)
														+ sizeof(_ITEM_GUID); }
	};

	class GCOpenBoxManipulateHandler 
	{
	public:
		static UINT Execute( GCOpenBoxManipulate* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;


#endif
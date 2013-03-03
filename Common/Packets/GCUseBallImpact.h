/********************************************************************
	created:	2007/11/27
	created:	27:11:2007   10:10
	filename: 	D:\project\projectNew\Common\Packets\GCUseBallImpact.h
	file path:	D:\project\projectNew\Common\Packets
	file base:	GCUseBallImpact 
	file ext:	h
	author:		Richard
	
	purpose:	先知或同类技能在创建时需要通知客户端创建特定效果此消息会包含: 

				技能ID, 小球的数量，客户端应根据技能ID判断应创建何种效果
*********************************************************************/

#ifndef GCUseBallImpact_h__27_11_2007_10_10
#define GCUseBallImpact_h__27_11_2007_10_10

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCUseBallImpact:	public Packet
	{
	public:
		GCUseBallImpact()
		{
			m_nSkillID		= INVALID_ID;
			m_nCharacterID	= INVALID_ID;
			m_uBallNum		= 0;
		}
		virtual		~GCUseBallImpact(){}

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_USEBALLIMPACT; }
		virtual UINT			GetPacketSize()const
		{ 
			return sizeof(SkillID_t) + sizeof(UINT) + sizeof(ObjID_t); 
		}

	public :
		//参考属性注释
		VOID		SetSkillID( SkillID_t nSkillID ) { m_nSkillID = nSkillID; }
		//参考属性注释
		SkillID_t	GetSkillID( ) { return m_nSkillID; }
		//参考属性注释
		VOID		SetBallNum( UINT uBallNum ) { m_uBallNum = uBallNum; }
		//参考属性注释
		UINT		GetBallNum( ) { return m_uBallNum; }
		//参考属性注释
		VOID		SetCharacterID( ObjID_t uID ) { m_nCharacterID = uID; }
		//参考属性注释
		ObjID_t		GetCharacterID( ) { return m_nCharacterID; }

		


	private :
		SkillID_t	m_nSkillID;			//产生此效果的技能ID
		UINT		m_uBallNum;			//产生的小球数量
		ObjID_t		m_nCharacterID;		//技能使用者的ID
	};

	class GCUseBallImpactFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCUseBallImpact(); }
		PacketID_t	GetPacketID() const	 { return PACKET_GC_USEBALLIMPACT; }
		UINT		GetPacketMaxSize() const { return sizeof(SkillID_t) + sizeof(UINT) + sizeof(ObjID_t); }			
	};

	class GCUseBallImpactHandler
	{
	public:
		static UINT Execute(GCUseBallImpact* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif //GCUseBallImpact_h__27_11_2007_10_10

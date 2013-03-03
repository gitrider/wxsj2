// GCDetailAbilityInfo.h
// 
// ��ǰ���ǵ���ϸ���ܱ�
// 
//////////////////////////////////////////////////////

#ifndef __GCDETAILABILITYINFO_H__
#define __GCDETAILABILITYINFO_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameStruct_Skill.h"

namespace Packets
{
class GCDetailAbilityInfo : public Packet
{
public:
	GCDetailAbilityInfo( ){}
	virtual ~GCDetailAbilityInfo( ){}

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_DETAILABILITYINFO ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) + sizeof(BYTE)
														+	sizeof(UCHAR) * m_wNumAbility
														+	sizeof(_OWN_ABILITY) * m_wNumAbility
														+	sizeof(CHAR) * MAX_CHAR_PRESCRIPTION_BYTE; }

public:
	VOID					setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t					getObjID(VOID)const { return m_ObjID; }

	BYTE					getAbilityNum( VOID ) const{ return m_wNumAbility; }
	const UCHAR				getAbilityID( INT idx ) const { return m_uAbilityIDList[idx]; }
	const _OWN_ABILITY*		getAbility( INT idx ) const{ return &m_aAbility[idx]; }
	const _OWN_ABILITY*		getAbilityList( VOID )const{ return &m_aAbility[0]; }
	const CHAR*				getPrescrList(VOID) const { return &m_aPrescr[0]; }

	VOID					setAbilityList(const _OWN_ABILITY *paAbility, const CHAR *paPrescr )
	{
		m_wNumAbility = 0;

		for( INT i=0; i<MAX_CHAR_ABILITY_NUM; ++i )
		{
			if( paAbility[i].m_Level < 1 )
			{
				continue;
			}

			m_uAbilityIDList[m_wNumAbility] = i;
			m_aAbility[m_wNumAbility] = paAbility[i];
			++m_wNumAbility;
		}

		memcpy( m_aPrescr, paPrescr, sizeof(CHAR) * MAX_CHAR_PRESCRIPTION_BYTE );
	}
private:
	ObjID_t					m_ObjID;	// ����Obj���͵�ObjID
	BYTE					m_wNumAbility;
	UCHAR					m_uAbilityIDList[MAX_CHAR_ABILITY_NUM];
	_OWN_ABILITY			m_aAbility[MAX_CHAR_ABILITY_NUM];
	CHAR					m_aPrescr[MAX_CHAR_PRESCRIPTION_BYTE];
};


class GCDetailAbilityInfoFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCDetailAbilityInfo() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_DETAILABILITYINFO; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) + sizeof(BYTE)
												+	sizeof(UCHAR) * MAX_CHAR_ABILITY_NUM
												+	sizeof(_OWN_ABILITY) * MAX_CHAR_ABILITY_NUM
												+	sizeof(CHAR) * MAX_CHAR_PRESCRIPTION_BYTE; }
};

class GCDetailAbilityInfoHandler 
{
public:
	static UINT Execute( GCDetailAbilityInfo* pPacket, Player* pPlayer ) ;
};

}

using namespace Packets;

#endif // __GCDETAILABILITYINFO_H__

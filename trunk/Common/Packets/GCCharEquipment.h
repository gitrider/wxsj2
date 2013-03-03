#ifndef __GCCHAREQUIPMENT_H__
#define __GCCHAREQUIPMENT_H__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCCharEquipment : public Packet
{
public:
	GCCharEquipment( ){
		Reset( );
	}
	virtual ~GCCharEquipment( ){}

	VOID Reset( VOID ){
		m_ObjID			= UINT_MAX;
		m_wFlags		= 0;
	}

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHAREQUIPMENT; }
	virtual UINT			GetPacketSize()const {
								UINT uAttribSize = 0;
								UINT i;
								for ( i = 0; i < HEQUIP_NUMBER; i++ )
								{
									if ( m_wFlags & (1<<i) )
									{
										uAttribSize += sizeof( UINT );
									}
								}
								uAttribSize +=	sizeof(ObjID_t) + sizeof(UINT);
								return uAttribSize;
							}

public:
	//ʹ�����ݽӿ�
	VOID			setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t			getObjID(VOID)const { return m_ObjID; }

	UINT			getFlags( VOID )const{ return (UINT)(m_wFlags); }

	BOOL			IsUpdateAttrib( HUMAN_EQUIP eAttrib )const{ return (m_wFlags & (1<<(INT)eAttrib))?(TRUE):(FALSE); }
	VOID			SetUpdateAttrib( HUMAN_EQUIP eAttrib, BOOL bUpdate ){
						if ( bUpdate )
							m_wFlags |= (1<<(INT)eAttrib);
						else
							m_wFlags &= (~(1<<(INT)eAttrib));
					}

	VOID			setMainWeapon( UINT ID){ m_MainWeapon = ID;  SetUpdateAttrib(HEQUIP_MAINHAND,TRUE); }
	UINT			getMainWeaponID( VOID )const{ return m_MainWeapon; }

	VOID			setAssiWeapon( UINT ID){ m_AssiWeapon = ID; SetUpdateAttrib(HEQUIP_ASSIHAND,TRUE); }
	UINT			getAssiWeaponID( VOID )const{ return m_AssiWeapon; }
	
	VOID			setShoulder( UINT ID){ m_Shoulder = ID; SetUpdateAttrib(HEQUIP_SHOULDER,TRUE); }
	UINT			getShoulderID( VOID )const{ return m_Shoulder; }
	
	VOID			setCap( UINT ID){ m_Cap = ID;  SetUpdateAttrib(HEQUIP_HEAD,TRUE); }
	UINT			getCapID( VOID )const{ return m_Cap; }

	VOID			setArmour( UINT ID){ m_Armour = ID;  SetUpdateAttrib(HEQUIP_BACK,TRUE); }
	UINT			getArmourID( VOID )const{ return m_Armour; }

	VOID			setCuff( UINT ID){ m_Hand = ID;  SetUpdateAttrib(HEQUIP_HAND,TRUE); }
	UINT			getCuffID( VOID )const{ return m_Hand; }

	VOID			setFoot( UINT ID){ m_Foot = ID;  SetUpdateAttrib(HEQUIP_FEET,TRUE); }
	UINT			getFootID( VOID )const{ return m_Foot; }

	VOID			setSuit( UINT ID){ m_Suit = ID;  SetUpdateAttrib(HEQUIP_SUIT,TRUE); }
	UINT			getSuitID( VOID )const{ return m_Suit; }

	VOID			setRider( UINT ID){ m_Rider = ID;  SetUpdateAttrib(HEQUIP_RIDER,TRUE); }
	UINT			getRiderID( VOID )const{ return m_Rider; }

	VOID			setSouxia( UINT ID ){ m_Souxia = ID; SetUpdateAttrib(HEQUIP_SOUXIA,TRUE); }
	UINT			getSouxiaID( VOID )const{ return m_Souxia; }

	VOID			setID(HUMAN_EQUIP EquipPoint,UINT ID)
	{
			switch(EquipPoint) {
			case HEQUIP_MAINHAND:
				{
					setMainWeapon(ID);
					break;
				}
			case HEQUIP_ASSIHAND:
				{
					setAssiWeapon(ID);
					break;
				}
			case HEQUIP_SHOULDER:
				{
					setShoulder(ID);
					break;
				}
			case HEQUIP_HEAD:
				{
					setCap(ID);
					break;
				}
			case HEQUIP_BACK:
				{
					setArmour(ID);
					break;
				}
			case HEQUIP_HAND:
				{
					setCuff(ID);
					break;
				}
			case HEQUIP_FEET:
				{
					setFoot(ID);
					break;
				}	
			case HEQUIP_SUIT:
				{
					setSuit(ID);
					break;
				}
			case HEQUIP_RIDER:
				{
					setRider(ID);
					break;
				}
			case HEQUIP_SOUXIA:
				{
					setSouxia(ID);
					break;
				}
			default:
				break;
			}
	}

	VOID FillParamBuf( VOID *pBuf );

private:
	ObjID_t			m_ObjID;		// ObjID

	/*
	|   m_ItemList [HEQUIP_NUMBER]
	| xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
	|				  | |||||||| ||||||||___  ����  HEQUIP_MAINHAND
	|				  | |||||||| |||||||____  ����  HEQUIP_ASSIHAND
	|				  | |||||||| ||||||_____  ͷ    HEQUIP_HEAD
	|				  | |||||||| |||||______  ��    HEQUIP_NECK
	|             	  |	|||||||| ||||_______  ��    HEQUIP_SHOULDER
	|				  |	|||||||| |||_________ ����  HEQUIP_BACK
	|                 | |||||||| ||_________  ��    HEQUIP_WRIST
	|                 | |||||||| |_________   ��    HEQUIP_HAND

	|                 | ||||||||___________   ��    HEQUIP_WAIST
	|             	  |	|||||||______________ �� 	HEQUIP_FEET
	|             	  |	||||||_______________ ��ָ1 HEQUIP_RING1
	|             	  |	|||||________________ ��ָ2 HEQUIP_RING2
	|             	  |	||||_________________ ��Ʒ1 HEQUIP_ADORN1
	|             	  |	|||__________________ ��Ʒ2	HEQUIP_ADORN2
	|             	  |	||___________________ ��װ	HEQUIP_SUIT
	|             	  |	|____________________ ����	HEQUIP_RIDER
	|				  |______________________ ����  HEQUIP_SOUXIA
	*/
	UINT	m_wFlags;		// ÿ��λ��ʾһ�������Ƿ�Ҫˢ�� HUMAN_EQUIP

	UINT	m_MainWeapon;		// �������� - ��Դ��ID
	UINT	m_AssiWeapon;		// �������� - ��Դ��ID
	UINT	m_Cap;			// ñ�� - ��Դ��ID
	UINT	m_Shoulder;		// �ļ�	- ��Դ��ID
	UINT	m_Armour;			// �·� - ��Դ��ID
	UINT	m_Hand;			// �� - ��Դ��ID
	UINT	m_Foot;			// ѥ�� - ��Դ��ID
	UINT	m_Suit;				// ��װ - ��Դ��ID
	UINT	m_Rider;			// ���� - ��Դ��ID
	UINT	m_Souxia;			// ����¼-��Դ��ID
};


class GCCharEquipmentFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCCharEquipment() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_CHAREQUIPMENT; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) +
													sizeof(UINT) +
													sizeof(UINT) * HEQUIP_NUMBER; }
};

class GCCharEquipmentHandler 
{
public:
	static UINT Execute( GCCharEquipment* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif
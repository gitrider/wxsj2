
#ifndef __GAMESTRUCT_ITEM_H__
#define __GAMESTRUCT_ITEM_H__

#include "Type.h"

class SocketInputStream;
class SocketOutputStream;

#define MAX_ITEM_STRING_LENGTH	255


BYTE		GetSerialType(UINT Serial);
BYTE		GetSerialClass(UINT Serial);
UINT		GetSerialIndex(UINT Serial);
BOOL        ISCommonEquip(UINT Serial);
UINT        GenMaterialIndex(UINT nMaterialType, UINT nMaterialLevel );

template <typename T> inline unsigned int ConvertUCharNew(T x, BOOL bLog, char* file, int line)
{
	unsigned int rt = ((unsigned int)((x%300000000)*13>>3));
	if(bLog) printf("%s:%d(%ld) crc=%d\n", file,line,x,rt);
	//g_pLog->SaveLog("./Log/Crc32.log","%s:%d(%ld) crc=%d",file,line,x,rt);
	return rt;
};

#define ConvertUChar(x,bLog)	ConvertUCharNew(x,bLog,__FILE__,__LINE__);





//��Χ
struct _RANGE_VALUE
{
	WORD				m_MinValue ;
	WORD				m_MaxValue ;
};

struct _ITEM_SKILL 
{
	BOOL				m_bActive;
	WORD				m_SkillID;
	WORD				m_SkillRate;

	BOOL				IsActive()
	{
		return m_bActive;
	}

	VOID				SetActive(BOOL bActive)
	{	
		m_bActive = bActive;
	}

	VOID	CleanUp()
	{
		memset(this,0,sizeof(_ITEM_SKILL));
	}

	_ITEM_SKILL()
	{
		CleanUp();
	}
};

struct _ITEM_IMPACT 
{
	WORD				m_ImpactID;
	WORD				m_ImpactSum;

	VOID	CleanUp()
	{
		memset(this,0,sizeof(_ITEM_IMPACT));
	}

	_ITEM_IMPACT()
	{
		CleanUp();
	}
};

struct _ITEM_EFFECT_SKILL 
{
	WORD				m_nSkillID;
	BYTE				m_nLevel;

	VOID	CleanUp()
	{
		m_nSkillID = -1;
		m_nLevel   = 0;
	}
};

struct _ITEM_AFFIX_SKILL 
{
	WORD				m_nSkillID;
	BYTE				m_nLevel;

	VOID	CleanUp()
	{
		m_nSkillID = -1;
		m_nLevel   = -1;
	}
};

//��Ʒ����ֵ
struct _ITEM_VALUE
{
	SHORT			m_Value ;
	VOID	CleanUp( )
	{
		memset( this, 0, sizeof(*this) ) ;
	}

	BOOL	operator == (_ITEM_VALUE& iV)
	{
		return iV.m_Value = m_Value;
	}
};

//��Ʒ����
struct _ITEM_ATTR
{
	INT					m_AttrType ;
	_ITEM_VALUE			m_Value ;

	VOID	CleanUp( )
	{
		m_AttrType	= -1;
		m_Value.CleanUp();
	}
	
	BOOL	operator==(_ITEM_ATTR& iA)
	{
		return	(iA.m_AttrType == m_AttrType) 
			   &&(iA.m_Value == m_Value);

	}
};


//��Ʒ����
struct _GEM_REQ_ATT
{
	BYTE				m_GemType ;
	_ITEM_VALUE			m_Value ;

	VOID	CleanUp( )
	{
		m_GemType	= -1;
		m_Value.CleanUp();
	}

	BOOL	operator==(_GEM_REQ_ATT& iA)
	{
		return	(iA.m_GemType == m_GemType) 
			&&(iA.m_Value == m_Value);

	}
};


//��ƷΨһID
struct _ITEM_GUID
{
	BYTE	m_World ;		//�����: (��)101
	BYTE	m_Server ;		//����˳���ţ�(��)5
	INT		m_Serial ;		//��Ʒ���кţ�(��)123429
	
	_ITEM_GUID()
	{
		m_Serial = 0;
		m_Server = 0;
		m_World	 = 0;	
	}
	VOID Reset(VOID)
	{
		m_Serial = 0;
		m_Server = 0;
		m_World	 = 0;	
	}
	_ITEM_GUID& operator=(_ITEM_GUID const& rhs)
	{
		m_Serial = rhs.m_Serial;
		m_Server = rhs.m_Server;
		m_World  = rhs.m_World;
		return *this;
	}
	BOOL	operator ==(_ITEM_GUID& Ref)	const
	{
		return (Ref.m_Serial==m_Serial)&&(Ref.m_Server==m_Server)&&(Ref.m_World==m_World);
	}

	BOOL	operator ==(const _ITEM_GUID& Ref)	const
	{
		return (Ref.m_Serial==m_Serial)&&(Ref.m_Server==m_Server)&&(Ref.m_World==m_World);
	}

	BOOL	isNull() const
	{
		return (m_World ==0)&&(m_Serial==0)&&(m_Server == 0);
	}
	
};

//-------------------------------------------------------------------------------
struct _ITEM_TYPE
{

	struct  
	{
		UINT	m_Class : 7 ;		//�������ͣ����磺װ���ࡢ����Ʒ����
		UINT	m_Type: 7 ;		//��𣬱��磺�󵶡���ǹ����
		UINT	m_Index: 14 ;		//��Ʒ�ţ����磺��Ҷ����ԧ�쵶����
	};

	UINT	ToUINT()
	{
		UINT	uid = 0 ;

		uid	+= m_Class;
		uid= uid<<7;
		uid	+= m_Type;
		uid= uid<<14;
		uid += m_Index;

		return uid;

	}

	UINT	ToSerial()	const
	{
		UINT Serial;
		Serial = m_Class;
		Serial	= Serial*100+m_Type;
		Serial	= Serial*10000+m_Index;
		return Serial;
	}



	BOOL	isNull() const
	{
		return (m_Class==0)&&(m_Type==0)&&(m_Index == 0);
	}

	BOOL	operator==(_ITEM_TYPE& Ref) const
	{
		return (m_Class==Ref.m_Class)&&(m_Type==Ref.m_Type)&&(m_Index==Ref.m_Index);
	}

	BOOL	operator>(_ITEM_TYPE& Ref) const
	{
		return ToSerial()>Ref.ToSerial();
	}

	BOOL	operator<(_ITEM_TYPE& Ref) const
	{
		return ToSerial()<Ref.ToSerial();		
	}
	VOID	CleanUp()
	{
		m_Class		=	0;
		m_Type		=	0;
		m_Index		=	0;
	}
};
//-------------------------------------------------------------------------------

_ITEM_TYPE	ConvertSerial2ItemType(UINT Serial);


struct _ITEM_RULER
{
	BOOL	m_Discard;		//����
	BOOL	m_Tile;			//�ص�
	BOOL	m_ShortCut;		//���
	BOOL	m_CanSell;		//����
	BOOL	m_CanExchange;	//����
	BOOL	m_CanUse;		//ʹ��
	BOOL	m_PickBind;		//ʰȡ��
	BOOL	m_EquipBind;	//װ����
	BOOL	m_Unique;		//�Ƿ�Ψһ

	BOOL	m_Ident;		//�Ƿ���Ҫ����	
	BOOL	m_DummyItem;	//�Ƿ��������
	BOOL	m_Bank;			//�Ƿ���Դ�������
	BOOL	m_Consume;		//�Ƿ�����	

	BOOL	CanDiscard()	{return m_Discard;}		//����
	BOOL	CanTile()		{return m_Tile;}		//�ص�
	BOOL	CanShortCut()	{return m_ShortCut;}	//���
	BOOL	CanSell()		{return m_CanSell;}		//����
	BOOL	CanExchange()	{return m_CanExchange;}	//����
	BOOL	CanUse()		{return m_CanUse;}		//ʹ��
	BOOL	isPickBind()	{return m_PickBind;}	//ʰȡ��
	BOOL	isEquipBind()	{return m_EquipBind;}	//װ����
	BOOL	isUnique()		{return m_Unique;}		//�Ƿ�Ψһ


	BOOL	isIdent()		{return m_Ident;}		//�Ƿ���Ҫ����	
	BOOL	isDummyItem()	{return m_DummyItem;}		//�Ƿ��������
	BOOL	isBack()		{return m_Bank;}		//�Ƿ���Դ�������
	BOOL	isConsume()		{return m_Consume;}		//�Ƿ�����	
};


enum	ITEM_RULER_LIST
{
	IRL_DISCARD,			//����
	IRL_TILE,				//�ص�
	IRL_SHORTCUT,			//���
	IRL_CANSELL,			//����
	IRL_CANEXCHANGE,		//����
	IRL_CANUSE,				//ʹ��
	IRL_PICKBIND,			//ʰȡ��
	IRL_EQUIPBIND,			//װ����
	IRL_UNIQUE,				//�Ƿ�Ψһ

	IRL_IDENT,				//�Ƿ���Ҫ����	��
	IRL_DUMMYITEM,			//�Ƿ��������	��
	IRL_BANK,				//�Ƿ���Դ�������	��
	IRL_CONSUME,			//�Ƿ�����	
};



//��Ƕ��ʯ��Ϣ
struct _ITEM_GEMINFO
{
	UINT	m_GemIndex;
	BOOL	m_bEnable;
	VOID	CleanUp()
	{
		m_GemIndex	= INVALID_ITEM_ID;
		m_bEnable   = FALSE;
	}
};




struct GEM_INFO
{
	UINT					m_nPrice;
	UINT					m_nSellPrice;
	BOOL					m_bBroadCast;
	UINT					m_EquipType[MAX_BASE_EQUIP_COUNT];
	UINT					m_AttCount;
	_ITEM_ATTR				m_GenAttr[MAX_GEM_ATT];
	UINT					m_GemReqCount;
	_GEM_REQ_ATT			m_GemReqAttr[MAX_GEM_REQ];

	VOID	CleanUp()
	{
		m_nPrice		=0;
		m_nSellPrice	=0;
		m_bBroadCast	=FALSE;
		m_AttCount		=0;
		m_GemReqCount	=0;
		for(INT i=0; i<MAX_BASE_EQUIP_COUNT; ++i)
			m_EquipType[i] = 0;
		for(int i=0; i<MAX_GEM_ATT; ++i)
			m_GenAttr[i].CleanUp();
		for(int i=0; i<MAX_GEM_REQ; ++i)
			m_GemReqAttr[i].CleanUp();
	}
};


struct STORE_MAP_INFO 
{
	INT						m_nLevel;
	FLOAT					m_xPos;
	FLOAT					m_zPos;
	INT						m_SceneID;
	INT						m_GrowType;
	
};

struct MEDIC_INFO
{
	
	BYTE					m_nCount;	   //��ǰ����
	BYTE					m_nLevel;
	BYTE					m_nLayedNum;   //��������
	BYTE					m_nReqSkillLevel;

	UINT					m_nBasePrice;
	UINT					m_nSellPrice;
	INT						m_nScriptID;
	INT						m_nSkillID;
	INT						m_nReqSkill;
	BYTE					m_TargetType;  //��Ʒѡ������
	BOOL					m_bBroadcast;	//�Ƿ�㲥
	
	INT GetTileNum()		{return m_nCount;}

	INT GetMaxTileNum()		{return m_nLayedNum;}

	VOID	CleanUp()
	{
		m_nLevel			=	0;
		m_nBasePrice		=	0;
		m_nSellPrice		=   0;
		m_nLayedNum			=	0;
		m_nScriptID			=	INVALID_ID;
		m_nSkillID			=	0;
		m_nCount			=	0;
		m_nReqSkill			=	-1;
		m_nReqSkillLevel	=	-1;
		m_TargetType		=	0;
		m_bBroadcast			=   FALSE;
	}
};

struct EXTRABAG_INFO
{
	UINT					m_nBasePrice;
	UINT					m_nSellPrice;
	UINT					m_nValidTime;
	UINT					m_nBeginTime;
	UINT					m_nSize;
	VOID	CleanUp()
	{
		m_nBasePrice		=	0;
		m_nSellPrice		=   0;
		m_nValidTime		=   0;
		m_nBeginTime		=	0;
		m_nSize				=   0;
	}
};
// add by gh for ���� 2010/06/24
struct	SOUL_BEAD_INFO
{
	UINT					m_nSellPrice;//�۳��۸�û�л����۸���Ϊ�̵겻��
	//BYTE					m_nLevel;	//���ߵȼ�
	BYTE					m_nNeedLevel;//��Ҫ�ȼ�

	BYTE					m_nAttrCount;//��������
	union
	{
		_ITEM_ATTR			m_pWuSoulBeadAttr[MAX_WU_SOUL_BEAD_ATTR];	//���������
		_ITEM_ATTR			m_pQiSoulBeadAttr[MAX_QI_SOUL_BEAD_ATTR];	//����������
	};

	VOID	CleanUp()
	{
		memset(this, 0, sizeof(SOUL_BEAD_INFO));
	}
};
// end of add
//-------------------------------------------------------------------------------------
enum	VAR_ATT
{
	ATT_CUR_DUR,
	ATT_CUR_DAMAGE,
};

//Ϊ�˼���װ����������Ŀǰ��װ��ϵͳ���ݽṹ���м�
//�򻯷���
//1 �� EQUIP_INFO �й̶����ԣ��˻�Ϊͨ������ȡ��

struct EQUIP_INFO 
{
	CHAR					m_RulerID;									//������
	UINT					m_BasePrice;								//�۳��۸�
	INT						m_MaxDurPoint;								//����;�
	BYTE                    m_BaseGemHole;								//��Ȼ��
	//////////////////////////////////////////////////////////////////////////
	//����Ϊ�̶�����
	BYTE					m_Bind;										//�����
	BYTE					m_PaiType;									//װ������
	UINT					m_EquipScore;								//װ���÷�
	BYTE					m_EquipType;								//װ������
	BYTE					m_AttrCount;								//���Ե�����
	_ITEM_ATTR				m_pAttr[MAX_ITEM_ATTR];						//��Ʒ����
	BYTE					m_NeedLevel;								//��Ҫ�ȼ�
	//INT						m_nAttSet;									//������
	BYTE					m_Level;									//�Ǽ�
	INT						m_CurMaxDur;								//��ǰ����;�
	INT						m_CurDurPoint;								//��ǰ�;�ֵ
	USHORT					m_CurDamagePoint;							//��ǰ���˶�
	BYTE					m_CurGemHole;								//��ǰ��ʯ��
	BYTE					m_StoneCount;								//��Ƕ��ʯ������
	_ITEM_GEMINFO			m_pGemInfo[MAX_ITEM_GEM] ;					//��Ʒ����Ƕ�ı�ʯ
	INT                     m_nAttSet;
	
	BYTE					m_SoulBeadType;							//��ħ�Ļ�������add by gh for Soul_bead 2010/06/30
	BYTE					m_SoulBeadAttrCount;						//�������Ե�����
	union
	{
		_ITEM_ATTR			m_pWuSoulBeadAttr[MAX_WU_SOUL_BEAD_ATTR];	//���������
		_ITEM_ATTR			m_pQiSoulBeadAttr[MAX_QI_SOUL_BEAD_ATTR];	//����������
	};

	VOID CleanUp()
	{
		memset(this,0,sizeof(EQUIP_INFO));
	};
};

// [7/8/2010 �¾���]
struct MOUNT_INFO
{
	VOID CleanUp()
	{
		memset(this,0,sizeof(MOUNT_INFO));
	};

	UINT             m_nNeedLevel;     // ��Ҫ�ﵽ�ĵȼ�
	INT		         m_nAddtionSpeed;  // ���ӵ��ٶ�
	INT              m_nLife;          // ����ʱ�ޣ���λ���죩
	INT              m_nBindType;      // ������ MOUNT_BIND_TYPE
	INT              m_nPrice;         // �����Ǯ

	BOOL             m_bIsBinded;      // ��ǰ�Ƿ��Ѱ�
	UINT             m_nObtainDate;    // ��ȡ����ʱ��ʱ��
	UINT             m_nRemainTime;    // ����ʣ��ʹ��ʱ��
};

//-------------------------------------------------------------------------------------

#define MAX_ITEM_LIST_COUNT			8
#define MAX_ITEM_STRING_LENGTH		255
struct _ITEM_LIST 
{
	INT				m_ListCount;
	_ITEM_TYPE		m_ListType[MAX_ITEM_LIST_COUNT];
	INT				m_TypeCount[MAX_ITEM_LIST_COUNT];

	_ITEM_LIST()
	{
		memset(this,0,sizeof(_ITEM_LIST));
	}
	
	VOID	AddType(_ITEM_TYPE it,INT Count)
	{
		Assert(Count>0);
		Assert(!it.isNull());
		
		BOOL bFindSame = FALSE;
		for(INT i=0;i<m_ListCount;i++)
		{
			if(m_ListType[i]==it)
			{
				m_TypeCount[i]+=Count;
				bFindSame = TRUE;
				break;
			}
		}
		if(m_ListCount>=MAX_ITEM_LIST_COUNT)
		{
			AssertEx(FALSE,"�����Ʒ����̫��!");
		}
		if(!bFindSame)
		{
			m_TypeCount[m_ListCount] = Count;
			m_ListType[m_ListCount]  = it;	
			m_ListCount++;
		}
	}
	
	VOID Init()
	{
		memset(this,0,sizeof(_ITEM_LIST));
	}
};


//��Ʒ��Ϣ, ����������Ʒ����������
#define	ITEMISVALID(ITEM) (ITEM.m_ItemIndex != 0 )//��Ʒ�Ƿ�Ϸ�
#define	ITEMPTRISVALID(PITEM) ( (PITEM == NULL) ? FALSE:PITEM->m_ItemIndex )//��Ʒָ���Ƿ�Ϸ�
#define	ITEMREFPTRISVALID(PITEM) ( (PITEM == NULL) ? FALSE:PITEM->GetItemTableIndex() )//��Ʒָ���Ƿ�Ϸ�

#define MAX_FIX_ATTR_LENGTH		1000
#define MAX_PET_SKILL_LENGTH	50
#define MAX_ITEM_CREATOR_NAME	13
#define MAX_ITEM_PARAM			3


enum ItemParamValue
{
	IPV_CHAR	=	0,
	IPV_SHORT	=	1,
	IPV_INT		=	2	
};

struct _ITEM
{
	_ITEM_GUID			m_ItemGUID ;				//��Ʒ�̶���Ϣ�����ܸı䣩
	UINT				m_ItemIndex;				//��Ʒ����
													/*
													|	    1 ~ 10000		��ɫװ��
													|	10001 ~ 20000		��ɫװ��	
													|	20001 ~ 30000		��ɫװ��
													|	30001 ~ 35000		ҩƿ
													|	35001 ~ 40000		��ʯ
													|						...
													*/

	CHAR				m_RulerID;
	CHAR				m_nsBind;
	CHAR				m_Creator[MAX_ITEM_CREATOR_NAME];
	INT					m_Param[MAX_ITEM_PARAM];// ���ڒ���¼�������װ��0����Ò���¼������,1����Ò���¼ռ��λ�����1λ��ֵ��0��û��ռ 2��������������ݣ�����ID�����߲�ҳ��¼������
	union 
	{
		EQUIP_INFO			m_Equip;
		GEM_INFO			m_Gem;
		MEDIC_INFO			m_Medic;
		EXTRABAG_INFO		m_ExtraBag;
		STORE_MAP_INFO		m_StoreMap;
		SOUL_BEAD_INFO		m_SoulBead;//��������
		MOUNT_INFO          m_Mount;   //����        [7/8/2010 �¾���]
	};

	_ITEM( )
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		memset( this, 0, sizeof(*this) ) ;
	}
	
	BYTE		ItemClass( )	const
	{
			return GetSerialClass(m_ItemIndex);
	};

	BYTE		ItemType()		const
	{
		return GetSerialType(m_ItemIndex);
	}

	BYTE		GetIndex()	const
	{
		return GetSerialIndex(m_ItemIndex);
	}

	EQUIP_INFO*	   GetEquipData() const
	{
		return	(EQUIP_INFO*)(&m_Equip);
	}

	GEM_INFO*	  GetGemData()	const
	{
		return	(GEM_INFO*)(&m_Gem);
	}

	MEDIC_INFO*		GetMedicData()	const
	{
		return (MEDIC_INFO*)(&m_Medic);
	}
	
	EXTRABAG_INFO*	GetExtraBagData()	const
	{
		return (EXTRABAG_INFO*)(&m_ExtraBag);
	}

	STORE_MAP_INFO*	GetStoreMapData()	const
	{
		return (STORE_MAP_INFO*)(&m_StoreMap);
	}
	// add by gh 2010/06/28
	SOUL_BEAD_INFO*	   GetSoulBeadData() const
	{
		return	(SOUL_BEAD_INFO*)(&m_SoulBead);
	}

    // [7/8/2010 �¾���]
	MOUNT_INFO*	   GetMountData() const
	{
		return	(MOUNT_INFO*)(&m_Mount);
	}

	//////////////////////////////////////////////////////////////////////////
	// ��Ʒ��������
	// �ܵ�����Ʒ		���� ʵ�ʴ�С
	// ���ܵ�����Ʒ		���� 1
	//
	BYTE	GetItemCount()		const;

	VOID	SetItemCount(INT nCount);

	BYTE	GetItemTileMax()	const;
	
	BOOL	isFullTile()		const;


	//���ӵ�������
	BOOL	IncCount(UINT nCount =1);	
	//���ٵ�������
	BOOL	DecCount(UINT nCount =1);


	BOOL	IsNullType()	const
	{
		return m_ItemIndex == 0;
	}

	VOID	ReadItemVarAttr(SocketInputStream& iStream);
	VOID	WriteItemVarAttr(SocketOutputStream& oStream) const;
	
	//VOID	ReadNewItemVarAttr(SocketInputStream& iStream);
	//VOID	WriteNewItemVarAttr(SocketOutputStream& oStream) const;

	INT		WriteFixAttr(CHAR* pIn,INT BuffLength);
	BOOL	ReadFixAttr(CHAR* pOut,INT& OutLength,INT BuffLength);

	INT		WriteVarAttr(CHAR* pIn,INT BuffLength);
	BOOL	ReadVarAttr (CHAR* pOut,INT& OutLength,INT BuffLength);

	BOOL	DBWriteAttr(CHAR* pIn,INT BuffLength);
	BOOL	DBReadAttr (CHAR* pOut,INT& OutLength,INT BuffLength);

	BOOL	IsFixAttr(BYTE attrType);
	BYTE	GetFixAttrCount();
		
	BOOL	GetItemBind() const;
	VOID	SetItemBind(BOOL bBind);

	BOOL	GetItemIdent() const;
	VOID	SetItemIdent(BOOL bIdent);

	BOOL	GetItemPLock() const;	//��������
	VOID	SetItemPLock(BOOL bIdent);
	
	//������
	BOOL	GetCreatorVar() const;
	VOID	SetCreatorVar(BOOL bCreator);

	INT		GetItemParamValue(UINT Start,ItemParamValue& ipv);
	VOID	SetItemParam(UINT start,ItemParamValue& ipv,INT value);
};

//pItem			��Ҫת����_ITEM
//pOut			ת�������������
//OutLength		������ݵĳ���
//BuffLength	pOut ��ʵ�ʳ���
BOOL		Item2String(_ITEM* pItem,CHAR* pOut,INT& OutLength,INT BuffLength);
BOOL		String2Item(const CHAR* pIn,INT BuffLength,_ITEM* pItem);


//A,B ����
VOID	SwapItem(_ITEM* pItemA,_ITEM* pItemB);
//A ����B �ռ�
//ͬʱA ���
VOID	OverWriteItem(_ITEM* pItemA,_ITEM* pItemB);

UINT    GetItemVarSize(const _ITEM& it);

UINT    GetNewItemVarSize(const _ITEM& it);


struct  ItemBoxContaner
{

	ItemBoxContaner(){
		memset(m_Item,0,sizeof(_ITEM)*MAX_BOXITEM_NUMBER);
		memset(m_nQuality,0,sizeof(INT)*MAX_BOXITEM_NUMBER);
		m_nCount = 0;
	}

	VOID	AddItemType(_ITEM_TYPE& it,INT iQuality)
	{
		m_Item[m_nCount].m_ItemIndex = it.ToSerial();
		m_nQuality[m_nCount]	=	iQuality;
		m_nCount++;
	}
	_ITEM				m_Item[MAX_BOXITEM_NUMBER]; 
	INT					m_nQuality[MAX_BOXITEM_NUMBER];
	INT					m_nCount;
	UINT				m_uDropType;
};

//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------

#endif

/********************************************************************
	��������:	2005��11��2��
	����ʱ��:	16:54
	�ļ�����:	ItemTable.h
	�ļ�·��:	d:\Prj\Server\Server\Item\ItemTable.h
	������:		������
	
	�ļ����ܣ� ��Ʒ�������ݱ����
	�޸ļ�¼��
	
*********************************************************************/

#ifndef _ITEM_TABLE_H_
#define _ITEM_TABLE_H_

#include "ItemTypes.h"
#include "Type.h"
#include "GameStruct_Item.h"
#include "GameDefine2.h"

#define SELF_CONS(x)	x(){memset(this,0,sizeof(x));}
#define DEFAULT_CONS(theClass,parentClass)	theClass(){memset(this,0,sizeof(theClass)); parentClass();}


//������ṹ
struct DROP_BOX_TB {

	SELF_CONS(DROP_BOX_TB);
	uint				m_DropBoxId;						//DropBox�ı�ű�
	INT					m_DropValue;						//DropBox��ֵ������������
	_ITEM_TYPE			m_DropItem[MAX_DROPBOX_CARRAGE];	//DropBoxЯ����Ʒ��
	INT					m_Quality[MAX_DROPBOX_CARRAGE];		//��ƷƷ�ʱ�	
};

struct MINMAX_VALUE 
{
	SELF_CONS(MINMAX_VALUE)
	INT			m_minValue;
	INT			m_maxValue;
};


enum  MINMAX_TYPE
{
	VT_MIN,
	VT_MAX
};
//��Ʒ��ֵ�νṹ��
struct ITEM_VALUE_TB 
{
	SELF_CONS(ITEM_VALUE_TB)
	uint					m_ValueType;
	MINMAX_VALUE			m_Values[MAX_ITEM_TABLE_VALUES];
};

//----------------------------------------------------------------------------------------
//��Ʒ���Եȼ��ṹ��
struct EQUIP_ATT_LEVEL_TB 
{
	SELF_CONS(EQUIP_ATT_LEVEL_TB)
	INT						m_AttID;
	INT						m_Attr[MAX_EQUIP_LEVEL];			//װ��������ֵ
};

//��Ʒ��ֵ�νṹ��
struct EQUIP_ATT_SET_TB 
{
	SELF_CONS(EQUIP_ATT_SET_TB);
	INT						m_IndexID;
	CHAR					m_SetName[100];
	INT                     m_AttSetRate;
	INT						m_ReqLevel;
	INT						m_SumRate;
	INT						m_AttCount;
	INT						m_Attr[MAX_EQUIP_EXT_ATTR];		//װ������
};


//װ�����еĻ�������
struct EQUIP_TB 
{
	SELF_CONS(EQUIP_TB);
	INT						m_IndexID;
	_ITEM_TYPE				m_ItemType;
	BYTE					m_EquipPoint;					//װ���
	CHAR					m_RulerID;						//������
	INT						m_EquipSetNum;					//��װ

	INT						m_ReqLevelCount;
	BYTE					m_ReqLevel[MAX_RAND_LEVEL_NUM];	//��Ҫ�ȼ�
	UINT					m_BasePrice;					//�۳��۸�
	INT						m_MaxDur;						//����;�
	INT						m_Level;						//�Ǽ�
	BYTE                    m_BaseGemHole;					//��Ȼ��
	BYTE					m_MAXGemHole;					//�����Ƕ��
	BYTE					m_PrefessionReq[MATTRIBUTE_NUMBER];	//ְҵ����	��������֧��ȫ����ְҵ
	BYTE					m_PaiType;						//����
	INT						m_EquipScore;					//װ����

	INT                     m_NearAttack;					//���̹���
	INT						m_FarAttack;					//Զ�̹���

	INT						m_MagicNearAttack;				//�����ڹ�����
	INT						m_MagicFarAttack;				//�����⹦����

	INT                     m_NearDefence;					//���̷���
	INT						m_FarDefence;					//Զ�̷���

	INT						m_MagicNearDefense;				//�����ڹ�����
	INT						m_MagicFarDefense;				//�����⹦����

	INT						m_EquipSetMaxNum;				//�����װ��Ŀ
	BYTE					m_AttCount;						//װ�����Ը���
	BYTE					m_AffSkillCount;				//���Ӽ��ܸ���
	BYTE					m_EffSkillCount;				//Ӱ�켼�ܸ���
	_ITEM_ATTR				m_Attr[MAX_EQUIP_EXT_ATTR];		//װ������
	_ITEM_AFFIX_SKILL		m_AffixSkill[MAX_EQUIP_AFFIX_SKILL];		//���Ӽ���
	_ITEM_EFFECT_SKILL		m_EffectSkill[MAX_EQUIP_EFFECT_SKILL];		//װ��Ӱ��ļ���
};
//----------------------------------------------------------------------------------------

//���������ṹ��
struct MONSTER_DROP_TB 
{
	SELF_CONS(MONSTER_DROP_TB);
	uint					m_DropId;
	uint					m_ValidCount;
	MONSTER_DROPCONTENT		m_Drops[MAX_MONSTER_DROPBOX];
};

//��ʯ��ṹ��
struct GEMINFO_TB 
{
	GEMINFO_TB()
	{
		m_nTableIndex	=	INVALID_ID;
		m_nRulerID		=	INVALID_RULERID;
		m_nPrice		=	INVALID_PRICE;
		m_nSellPrice	=	INVALID_PRICE;
		m_bBroadCast	=   FALSE;
		m_AttCount		=	0;
		m_GemReqCount	=	0;
		m_ItemType.CleanUp();
		for(INT i=0; i<MAX_BASE_EQUIP_COUNT; ++i)
			m_EquipType[i] = 0;
		for(INT i=0; i<MAX_GEM_ATT; ++i)
			m_GenAttr[i].CleanUp();
		for(INT i=0; i<MAX_GEM_REQ; ++i)
			m_GemReqAttr[i].CleanUp();
	}

	UINT					m_nTableIndex;
	_ITEM_TYPE				m_ItemType;
	CHAR					m_nRulerID;
	UINT					m_EquipType[MAX_BASE_EQUIP_COUNT];
	UINT					m_nPrice;
	UINT					m_nSellPrice;
	BOOL					m_bBroadCast;
	UINT					m_AttCount;
	_ITEM_ATTR				m_GenAttr[MAX_GEM_ATT];
	UINT					m_GemReqCount;
	_GEM_REQ_ATT			m_GemReqAttr[MAX_GEM_REQ];
};

struct EQUIPHOLE_INFO_TB
{
	SELF_CONS(EQUIPHOLE_INFO_TB);
	UINT					m_ReqItem;
	UINT					m_ReqItemNum;
	UINT					m_ReqMoney;
	UINT					m_Rate;
};

struct EQUIPCHAIN_INFO_TB 
{
	EQUIPCHAIN_INFO_TB()
	{
		m_ChainID	= INVALID_PAI_TYPE;
		m_ReqPai1	= INVALID_PAI_TYPE;
		m_ReqPai2	= INVALID_PAI_TYPE;
		m_ReqPai3	= INVALID_PAI_TYPE;
		m_Att.CleanUp();
		m_Flat.CleanUp();
	}
	UINT					m_ChainID;
	UINT					m_ReqPai1;
	UINT					m_ReqPai2;
	UINT					m_ReqPai3;
	Flag128					m_Flat;
	_ITEM_ATTR				m_Att;
};

struct EQUIPSOUL_INFO_TB 
{
	EQUIPSOUL_INFO_TB()
	{
		m_SoulID			= INVALID_SOUL_TYPE;
		m_ChainID1			= INVALID_PAI_TYPE;
		m_ChainID2			= INVALID_PAI_TYPE;
		m_ChainID3			= INVALID_PAI_TYPE;
		m_ChainID4			= INVALID_PAI_TYPE;
		m_ChainID5			= INVALID_PAI_TYPE;
		memset(SoulType, 0, sizeof(UINT)*1000);
		memset(ValidSoul, 0, sizeof(UINT)*4);
		ValidSoulNum		= 0;
		m_EffectAttCount	= 0;
		m_AnyShunNum		= 0;
		m_AnyKeNum			= 0;
	}
	UINT					m_SoulID;
	UINT					m_ChainID1;
	UINT					m_ChainID2;
	UINT					m_ChainID3;
	UINT					m_ChainID4;
	UINT					m_ChainID5;
	UINT					m_AnyShunNum;
	UINT					m_AnyKeNum;
	UINT					SoulType[1000];
	UINT					ValidSoul[4];
	UINT					ValidSoulNum;
	UINT					m_EffectAttCount;
	_ITEM_ATTR				m_Att[MAX_SOUL_EFFECTATT];
};

struct CHANGEPAI_INFO_TB 
{
	UINT					m_nTableIndex;
	UINT					m_DestPaiType;					
};
;
//��ͨ��Ʒ��ṹ��
struct COMMITEM_INFO_TB 
{
	COMMITEM_INFO_TB()
	{
		m_nTableIndex		=	0;
		m_nLevel			=	0;
		m_nBasePrice		=	0;
		m_nSellPrice		=   0;
		m_nLayedNum			=	0;
		m_nScriptID			=	0;
		m_nSkillID			=	0;
		m_nRulerID			=	0;
		m_nReqSkill			=	-1;
		m_nReqSkillLevel	=	-1;
		m_HoldNum			=   -1;
		m_TargetType		=	0;
		m_bBroadcast		=   FALSE;
		m_ItemType.CleanUp();
		
	}

	INT					m_nTableIndex;
	_ITEM_TYPE			m_ItemType;
	INT					m_nLevel;
	INT					m_nUseLeve;
	UINT				m_nBasePrice;
	UINT				m_nSellPrice;
	CHAR				m_nRulerID;	 //����ID
	INT					m_nLayedNum; //��������
	INT					m_nScriptID;
	INT					m_nSkillID;
	INT					m_nReqSkill;
	INT					m_nReqSkillLevel;
	INT					m_HoldNum;
	BYTE				m_TargetType;
	BOOL				m_bBroadcast;
};

//������Ʒ��ṹ��
struct TASKITEM_INFO_TB 
{
	TASKITEM_INFO_TB()
	{
		m_nTableIndex		=	0;
		m_nLevel			=	0;
		m_nBasePrice		=	0;
		m_nSellPrice		=   0;
		m_nLayedNum			=	0;
		m_nScriptID			=	0;
		m_nSkillID			=	0;
		m_nRulerID			=	0;
		m_nReqSkill			=	-1;
		m_nReqSkillLevel	=	-1;
		m_HoldNum			=   -1;
		m_TargetType		=	0;
		m_bBroadcast		=   FALSE;
		m_ItemType.CleanUp();

	}

	INT					m_nTableIndex;
	_ITEM_TYPE			m_ItemType;
	INT					m_nLevel;
	INT					m_nUseLeve;
	UINT				m_nBasePrice;
	UINT				m_nSellPrice;
	CHAR				m_nRulerID;	 //����ID
	INT					m_nLayedNum; //��������
	INT					m_nScriptID;
	INT					m_nSkillID;
	INT					m_nReqSkill;
	INT					m_nReqSkillLevel;
	INT					m_HoldNum;
	BYTE				m_TargetType;
	BOOL				m_bBroadcast;
};

//�ر�ͼ�ṹ��
struct STORE_MAP_INFO_TB 
{
	STORE_MAP_INFO_TB()
	{
		m_nTableIndex	=	0;
		m_ItemType.CleanUp();
		m_ResourceID	=	0;
		m_nRulerID		=	0;
		m_nLevel		=	0;
		m_nBasePrice	=	0;
		m_xPos			=	-1.0f;
		m_zPos			=	-1.0f;
		m_SceneID		=	-1;
		m_GrowPointType	=	-1;

	}
	INT						m_nTableIndex;;
	_ITEM_TYPE				m_ItemType;
	WORD					m_ResourceID;
	CHAR					m_nRulerID;
	INT						m_nLevel;
	UINT					m_nBasePrice;
	FLOAT					m_xPos;
	FLOAT					m_zPos;
	INT						m_SceneID;
	INT						m_GrowPointType;
	INT						m_ScriptID;
	
};
//add by gh for souxia 2010/05/05
struct SOUXIA_INFO_TB
{
	SOUXIA_INFO_TB()
	{
		memset(this, -1, sizeof(SOUXIA_INFO_TB));
	}
	INT					m_nTableIndex;
	INT					m_nNeedLevel;	//���Ƶȼ�
	INT					m_nUseType;		//��Ӧ�Ų�ͬ��ʹ������,��ʾ������ֵ�����ö���ͬ.0:����¼����,1:����¼����� 2:��ϵ��� 3:��������4:��ʾ��ϵ���-1:�õ�����Ч
	INT					m_nSouXiaLevel;	//��������Ϊ0�ģ������������ã���д����ֵ����������¼�ĵȼ��������ڸ�����д0����ʾ������¼Ϊ��������¼����д1��ʾΪ��������¼����д2��ʾΪ�м�����¼����д3��ʾΪ�߼�����¼����д4��ʾ������������¼����д-1��ʾ������Ч

	INT					m_nMaxPages[5];	//��¼�����ҳ�� 0Ϊ�����ҳ����1�Ǽ������ҳ��...
	INT					m_nMemoryType;	//����������дΪ1�ģ������������á���д����ֵ����������¼��������¼�ĵ������ࡣ��д1��ʾ��¼���Ǽ��ܣ���д2��ʾ��Ϊ��¼���������䷽����д3��ʾ��¼���������ٻ����ߣ���д4��ʾ��������ٻ����ߡ���д-1��ʾ������Ч
	
	INT					m_nMemoryContent[4];// ������1����ֵ������£��ò���0λ�����á���ʾ��������������¼����ʲô���ܣ���ʾ�õ�����Ҫ�ļ��ܱ�ţ�Ҫ��֧�֡��ͣ�������д���򣮹������£�������������������ʾ����ƽ�������ڣ�����������������֮�����ѡȡ1�����룮��������������������ʾ�ڷ�������ֵ֮�����ѡȡ�����еļ��ܣɣİ���һ�����ü��ܣɣĵĹ���
	//�ò���1λ�����á���ʾ������¼�������䷽�ģɣģ�Ҫ��֧�ֽ�����д��ʽ���ò���2λ�����ñ�ʾ�������ٻ��ļ��ܵģɣģ��ãɣ��Ǽ��ܱ���ģɣģ�Ҫ��֧�ֽ�����ʽ���ò���3λ�����ã���ʾ������¼�����ٻ����ܵģɣģ��ãɣ��Ǽ��ܱ���ģɣģ�Ҫ��֧�ֽ�����ʽ

	INT					m_nDismantleLevel;		//����������дΪ2�ģ������������á���д����ֵ�����Ų�ж���ߵ�Ʒ�ʡ�������д0����ʾ�ͼ���ж���ߣ���д1����ʾ������ж���ߣ���д2��ʾ�м���ж���ߣ���д3��ʾ�߼���ж���ߣ���д4��ʾ�������Ͳ�ж���ߡ���д-1��ʾ��Ч
	INT					m_nDismantleRule[4][2];	//������2����ֵ������£��ò��������á���ʾ��ж���ߵĹ��ܡ�����0��ʾΪ����������ҳ�����ݵĲ�ж��1��ʾ�������1������ҳ�Ĳ�ж��2��ʾ�漴����2������ҳ�Ĳ�ж����д3��ʾ���3������ҳ�Ĳ�ж����д4��ʾ��ȫ�����Ĳ�ж 
	
	INT					m_nRefiningLevel;		//����������дΪ3�ģ������������á���д����ֵ�������������ߵ�Ʒ�ʡ�������д0����ʾ�ͼ��������ߣ���д1����ʾ�����������ߣ���д2��ʾ�м��������ߣ���д����ʾ�߼���������д4��ʾ���������������ߡ���д-1��ʾ��Ч
	INT					m_nRefiningRule[4][2];	//������2����ֵ������£��ò��������á���ʾ��ж���ߵĹ��ܡ�����0��ʾΪ����������ҳ�����ݵĲ�ж��1��ʾ�������1������ҳ�Ĳ�ж��2��ʾ�漴����2������ҳ�Ĳ�ж����д3��ʾ���3������ҳ�Ĳ�ж����д4��ʾ��ȫ�����Ĳ�ж 
	//�����ࣳ����ֵ������£��ò��������á���ʾ�������ߵĹ��ܡ�����0��ʾΪ����������ҳ�����ݵ�������1��ʾ�ڣ�������¼���������1������ҳ��������2��ʾ�ڣ�������¼�����������������ҳ��������д3��ʾ�ڣ�������¼�����������������ҳ����������д4��ʾ��ȫ���������������ͨ���գ�ѡ����Ҫ�����������ҳ����
	
	INT					m_nSewRate;	//����������дΪ���ģ������������á���д����ֵ�����ŷ�ϵ��߳ɹ���ϵĸ�����ֵ��д�����ǣ�����������������������ֵԽС��ʾ�ɹ��ĸ���ԽС
	INT					m_nUseTime; //��ʾ��Ӧ�����ޣ�����ٻ�����ʹ�õĴ�������ֵ��ΧΪ1-99��Ч����д-1�ʹ�����Ч

};

struct	SOUL_BEAD_INFO_TB
{
	SOUL_BEAD_INFO_TB()
	{
		memset(this, 0, sizeof(SOUL_BEAD_INFO_TB));
	}
	INT					m_nTableIndex;
	BYTE				m_nRuleID;
	INT					m_nValidTime;
	INT					m_nImpactId;
};
// end of add
//����˥����ṹ��
struct DROP_ATT_TB 
{
	SELF_CONS(DROP_ATT_TB);
	INT				m_DeltaLevel;
	FLOAT			m_AttValue;
	FLOAT			m_ExpValue;
	FLOAT			m_TeamAttValue;
	FLOAT			m_TeamExpValue;
};

//����λ�ýṹ��
struct DROP_POS_TB 
{
	SELF_CONS(DROP_POS_TB);
	FLOAT			m_DropX;
	FLOAT			m_DropZ;
};

struct EQUIP_LEVELUP_TB
{
	SELF_CONS(EQUIP_LEVELUP_TB);
	UINT	m_LowLevelGemID;
	UINT	m_HighLevelGemID;
	UINT	m_OtherLevelGemID;
	UINT	m_SuccessRate[MAX_LEVELUP_GEMREQ_NUM];
	UINT	m_Punishment;
	UINT	m_ReqMoney;
	UINT	m_EffectRate;
};

struct EQUIP_BIND_TB
{
	SELF_CONS(EQUIP_BIND_TB);
	UINT	m_GemID;
	UINT	m_EffectRate;
};

struct EQUIP_SPECIALREPAIR_TB
{
	SELF_CONS(EQUIP_SPECIALREPAIR_TB);
	UINT	m_ItemID;
	UINT	m_RecoverPoint;
};

struct  CREATEITEM_INFO
{
	UINT	m_MaterialType;	//��������
	BYTE	m_MaterialNum;	//��������	
	BYTE	m_NeedLevel;	//������͵ȼ�	
	BYTE	m_SplitRate;	//�����ı���	
};

struct REMOULDITEM_INFO
{
	UINT	m_MaterialType;	//��������
	BYTE	m_MaterialNum;	//��������	
	BYTE	m_NeedLevel;	//������͵ȼ�	
};

struct  ITEM_PRODUCT_INFO_TB
{
	SELF_CONS(ITEM_PRODUCT_INFO_TB);
	UINT				m_ProductID;		//��ƷID	
	UINT				m_PrescriptionID;	//�䷽��	
	BYTE				m_ProductNum;		//��Ʒ����
	UINT				m_nCreateInfoCount;	
	CREATEITEM_INFO		m_CreateInfo[MAX_CREATE_MATERIAL_NUM];		//������1
	UINT				m_nRemouldInfoCount;
	REMOULDITEM_INFO	m_RemouldInfo[MAX_REMOULD_MATERIAL_NUM];
	UINT				m_OperateTime;
	UINT				m_ScriptID;
	UINT				m_CreateMoney;
	UINT				m_RemouldMoney;
	BYTE				m_NeedNPC;
};


struct MATERIAL_LEVEL_INFO_TB
{
	MATERIAL_LEVEL_INFO_TB()
	{
		m_MaterialValue = 0;
	}
	UINT m_MaterialValue;
};

struct  EQUIP_EXTRAATT_SET
{
	INT				m_AttrCount;
	INT				m_Attr[10];
};

struct EQUIPEXTRATT
{
	UINT					m_ValidCount;
	EQUIP_EXTRAATT_SET		m_GenAttr[MAX_ITEM_NATUREATTR];
};

//װ����������
struct EQUIPEXTRATT_TB 
{
	SELF_CONS(EQUIPEXTRATT_TB);

	UINT					m_nEquipScore;
	EQUIPEXTRATT			m_EquipExtrAtt[MAX_JOB_SUM];
};

struct EXTRABAG_INFO_TB 
{
	EXTRABAG_INFO_TB()
	{
		m_nTableIndex	=	0;
		m_ItemType.CleanUp();
		m_nRulerID		=	0;
		m_nBasePrice	=	0;
		m_nSellPrice	=   0;
		m_nValidTime	=   0;
		m_nSize			=   0;
	}	
	UINT					m_nTableIndex;
	_ITEM_TYPE				m_ItemType;
	CHAR					m_nRulerID;	 //����ID
	UINT					m_nBasePrice;
	UINT					m_nSellPrice;
	UINT					m_nValidTime;
	UINT					m_nSize;
};

struct  PAI_INFO
{
	PAI_INFO()
	{
		m_nPaiID = INVALID_PAI_TYPE;
		m_nRate  = 0;
		m_Att.CleanUp();
	}
	UINT					m_nPaiID;
	_ITEM_ATTR				m_Att;
	UINT					m_nRate;
};

//װ�����Ͻṹ��
struct EQUIP_SET_TB
{
	SELF_CONS(EQUIP_SET_TB);
	INT				m_nEquipSetSerial;
	INT				m_nEquipSetCount;
	INT				m_nAttrCount;
	_ITEM_ATTR		m_ItemAttr[MAX_ITEM_SET_ATTR];
	INT				m_ActiveAttrNeedNum[MAX_ITEM_SET_ATTR];
};


//��Ʒ����ṹ��
struct ITEM_RULER_TB :public _ITEM_RULER{
	
	SELF_CONS(ITEM_RULER_TB);
	INT		m_RulerIndex;
	
};

//����ṹ��  [7/8/2010 �¾���]
struct MOUNT_INFO_TB 
{
	MOUNT_INFO_TB()
	{
		m_nTableIndex	=	0;
		m_nRule	        =	0;
		m_nNeedLevel    =   0;
		m_nAddtionSpeed	=   0;
		m_nLife     	=   0;
        m_nBindType     =   0;
        m_nPrice        =   0;
	}	
	UINT	         m_nTableIndex;    //id
	CHAR	         m_nRule;		   //��Ӧ����
	UINT             m_nNeedLevel;     //��Ҫ�ﵽ�ĵȼ�
	INT	             m_nAddtionSpeed;  //���ӵ��ٶ�
	INT              m_nLife;          //����ʱ�ޣ���λ���죩
	INT              m_nBindType;      //������ 
	INT              m_nPrice;         //�����Ǯ
};

/*
 *��Ʒ��ر���Դ������	
 */
class ItemTable
{
	public:
		ItemTable() ;
		~ItemTable() ;

		VOID	CleanUp( ) ;
		BOOL	Init();


	public:
		MONSTER_DROP_TB*			GetMonsterDropTB(INT iMonsterType);
		
		//------------------------------------------------------------------------
		DROP_POS_TB*				GetDropPosTB(INT nDropPoint);

		EQUIP_TB*					GetEquipTB(UINT itemSerial);			//��ȡװ����Ϣ
		UINT						GetEquipPointValue(HUMAN_EQUIP equipPoint);
		PAI_INFO*					GetPaiInfo(UINT nType);
		UINT						GetZiPai(UINT nSeed);
		UINT						GetZiPaiCount();
		UINT						GetNoZiPai(UINT nSeed);
		UINT						GetNoZiPaiCount();

		EQUIP_ATT_SET_TB*			GetAttSetTB(INT nSetID);				//��ȡ��װ��Ϣ
		EQUIP_ATT_SET_TB*			GetAttSetTBByRate(INT nRate);
		UINT						GetSumRateByReqLevel(INT nLevel);
		EQUIP_ATT_LEVEL_TB*			GetAttLevelValue(ITEM_ATTRIBUTE iAtt);	//��ȡ���Եȼ���Ϣ
		//------------------------------------------------------------------------
		MINMAX_VALUE				GetItemValue(ITEM_ATTRIBUTE iAtt,INT QualityLevel);

		DROP_ATT_TB*				GetDropAttTB(INT iDeltaLevel);

		GEMINFO_TB*					GetGemInfoTB(UINT	itemSerial);
		EQUIPHOLE_INFO_TB*			GetEquipHoleInfoTB(UINT	nHoleNum);
		EQUIPSOUL_INFO_TB*			GetEquipSoulInfoTB(UINT	nIndex);
		EQUIPCHAIN_INFO_TB*			GetEquipChainInfoTB(UINT nIndex);
		EQUIP_LEVELUP_TB*			GetEquipLeveupTB(UINT nLevel);
		EQUIP_BIND_TB*				GetEquipBindTB(UINT nLevel);
		EQUIP_SPECIALREPAIR_TB*		GetSpecialRepairTB(/*UINT itemSerial*/);
		ITEM_PRODUCT_INFO_TB*		GetItemProductInfoTB(UINT nProductID);
		MATERIAL_LEVEL_INFO_TB*		GetMaterialInfoTB(UINT nLevel);
		EQUIPEXTRATT_TB*			GetEquipExtraAttInfoTB(UINT nScore);
		EXTRABAG_INFO_TB*			GetExtraBagInfoTB(UINT	itemSerial);
		CHANGEPAI_INFO_TB*			GetChangePaiInfoTB(UINT	itemSerial);
		MOUNT_INFO_TB*	            GetMountInfoTB(UINT	itemSerial);           // [7/8/2010 �¾���]

		COMMITEM_INFO_TB*			GetCommItemInfoTB(UINT	itemSerial);
		TASKITEM_INFO_TB*			GetTaskItemInfoTB(UINT	itemSerial);
		EQUIP_SET_TB*				GetEquipSetTB(INT EquipSetSerial);
		
		ITEM_RULER_TB*				GetRuleValueByID(INT& iIndex);

		STORE_MAP_INFO_TB*			GetStoreMapTB(UINT	itemSerial);;
		SOUXIA_INFO_TB*				GetSouXiaTB(UINT	itemSerial);	// add by gh for souxia_item
		SOUL_BEAD_INFO_TB*			GetSoulBeadTB(UINT	itemSerial);
		INT							GetAttCount(){ return m_nAttLevelCount;}
		INT							GetAttSetCount(){ return m_nEquipAttSetCount;}
		INT							GetEquipChainInfoCount(){ return m_nEquipChainInfoCount;}
		INT							GetEquipSoulInfoCount(){ return m_nEquipSoulInfoCount;}
	protected:

		//---------------------------------------------------------------------------
		VOID						InitEquipTable();
		VOID						InitAttLevelTable();
		VOID						InitEquipATTSetTable();
		
		//---------------------------------------------------------------------------
		VOID						InitMonsterDropTable();
		VOID						InitGemInfoTable();
		VOID						InitCommItemInfoTable();
		VOID						InitEquipHoleInfoTable();
		VOID						InitEquipLevelupInfoTable();
		VOID						InitEquipBindInfoTable();
		VOID						InitSpecialRepairInfoTable();
		VOID						InitProductInfoTable();
		VOID						InitMaterialLevelInfoTable();
		VOID						InitEquipExtraInfoTable();
		VOID						InitEquipPointInfoTable();
		VOID						InitExtraBagInfoTable();
		VOID						InitChangePaiInfoTable();
		VOID						InitPaiInfoTable();
		VOID						InitEquipChainInfoTable();
		VOID						InitEquipSoulInfoTable();

		VOID						InitTaskItemInfoTable();
		VOID						InitDropAttTable();
		VOID						InitItemDropPos();
		VOID						InitEquipSetAttrTable();
		VOID						InitItemRulerTable();
		VOID						InitStoreMapTable();
		VOID						InitSouXiaTable();//��ʼ������¼���
		VOID						InitSoulBeadTable();//��ʼ��������ṹ
		VOID	                    InitMountAttrTable();// [7/14/2010 �¾���]
	private:
		//-----------------------------------------------------------------------------------
		uint						m_nAttLevelCount;
		EQUIP_ATT_LEVEL_TB*         m_pAttLevelTableData;

		uint						m_nEquipAttSetCount;
		EQUIP_ATT_SET_TB*			m_pEquipAttSetTableData;

		uint						m_nEquipCount;
		EQUIP_TB*					m_pEquipTableData;
		//------------------------------------------------------------------------------------
		uint						m_nDropBoxCount;
		DROP_BOX_TB*				m_pDropBoxTableData;

		uint						m_nMonsterDropCount;
		MONSTER_DROP_TB*			m_pMonsterDropTableData;

		uint						m_nGemCount;
		GEMINFO_TB*					m_pGemInfoData;

		uint						m_nEquipHoleInfoCount;
		EQUIPHOLE_INFO_TB*			m_EquipHoleInfoData;	
		
		uint						m_nLeveupInfoCount;
		EQUIP_LEVELUP_TB*			m_EquipLeveupInfoData;
		
		uint						m_nEquipBindInfoCount;
		EQUIP_BIND_TB*				m_EquipBindInfoData;

		uint						m_nSpecialRepairCount;
		EQUIP_SPECIALREPAIR_TB*		m_EquipSpecialRepairData;

		uint						m_nProductCount;
		ITEM_PRODUCT_INFO_TB*		m_pProductInfoData;
		
		uint						m_nMaterialLevelInfoCount;
		MATERIAL_LEVEL_INFO_TB*		m_MaterialLevelInfoData;

		uint						m_nEquipExtraInfoCount;
		EQUIPEXTRATT_TB*			m_EquipExtraInfoData;
		
		uint						m_nExtraBagInfoCount;
		EXTRABAG_INFO_TB*			m_pExtraBagInfoData;
		
		uint						m_nEquipChainInfoCount;
		EQUIPCHAIN_INFO_TB*			m_EquipChainInfoData;
		
		uint						m_nEquipSoulInfoCount;
		EQUIPSOUL_INFO_TB*			m_EquipSoulInfoData;
		
		uint						m_nChangePaiInfoCount;
		CHANGEPAI_INFO_TB*			m_pChangePaiInfoData;
		
		uint						m_nCommItemCount;
		COMMITEM_INFO_TB*			m_pCommItemInfoData;

		uint						m_nTaskItemCount;
		TASKITEM_INFO_TB*			m_pTaskItemInfoData;	

		uint						m_nDropAttCount;
		DROP_ATT_TB*				m_pDropAttData;

		uint						m_nDropPosCount;
		DROP_POS_TB*				m_pDropPosData;

		uint						m_nEquipSetCount;
		EQUIP_SET_TB*				m_pEquipSetData;

		uint						m_nItemRulerCount;
		INT							m_nHashOffSet;
		ITEM_RULER_TB*				m_pItemRulerData;

		uint						m_nStoreMapCount;
		STORE_MAP_INFO_TB*			m_pStoreMapData;

		uint						m_nSouXiaCount;		// add by gh 2010/05/05
		SOUXIA_INFO_TB*				m_pSouXiaData;		// add by gh 2010/05/05

		UINT						m_nSoulBeadCount;
		SOUL_BEAD_INFO_TB*			m_pSoulBeadData;	
		uint						m_nMountAttInfoCount;  // [7/14/2010 �¾���]
		MOUNT_INFO_TB*			    m_pMountAttInfoData;   // [7/14/2010 �¾���]
		UINT						m_nEquipPointInfo[HEQUIP_NUMBER];
		PAI_INFO					m_nPaiInfo[EQUIP_PAI_NUM];
		UINT						m_nZiPaiCount;
		UINT						m_nZIPai[EQUIP_PAI_NUM];
		UINT						m_nNoZIPaiCount;
		UINT						m_nNoZIPai[EQUIP_PAI_NUM];
};

BYTE	GetItemTileMax(_ITEM_TYPE& it);


extern ItemTable g_ItemTable ;

#endif

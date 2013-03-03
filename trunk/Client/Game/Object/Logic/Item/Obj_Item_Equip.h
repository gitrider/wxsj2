
#pragma once


#include "Obj_Item.h"
#include "GameDefine2.h"
#include "Obj_Item_Equip.h"
#include "GIDBC_Struct.h"
#include "../../ObjectDef.h"
#include "GameStruct_Item.h"


/*
|
|	װ������Ʒ
|
*/


class CObject_Item_Equip : public CObject_Item
{
public:
	//�õ���Ʒ�����
	virtual ITEM_CLASS		GetItemClass(VOID) const;
	//��Ʒ������
	virtual LPCTSTR			GetName(VOID) const;
	//��Ʒ����
	virtual LPCTSTR			GetDesc(VOID) const;
	//ͼ����
	virtual LPCTSTR			GetIconName(VOID) const;
	//�õ�װ������
	INT                     GetEquipCard(VOID) ;

	// ����װ������
	BOOL					SwapEquipCard(CObject_Item_Equip &ep);

	//����װ������
	VOID					SetEquipCard( INT setNum );  
	//��ϸ����
	virtual LPCTSTR			GetExtraDesc(VOID);
	//�õ��Ǽ�
	INT						GetStartLevel(VOID) const;	
	//������ϸ����
	virtual VOID			SetExtraInfo(const _ITEM * pItemInfo);
	virtual VOID			SetExtraInfo( CHAR* pstrIn );
	virtual LPCTSTR	GetExtranInfo();
	//�õ�װ����
	HUMAN_EQUIP				GetItemType(VOID)const ;

	INT						GetEquipScore();		

	//�õ�װ���Ƿ񱻸�ħ
	BOOL		GetIsFumo();


	/** 
	 *	�õ���ʾ����ID ====== �߼�
	 */
	

	virtual INT				GetVisualID(VOID)	const;

	//�õ���ʾ����ID ����ʱ������
	virtual LPCTSTR			GetDropVisualID(VOID)	const;

	//��ʾ������ɫ
	virtual LPCTSTR			GetDropVisColor(VOID) const ;

	//��ѯ�߼�����,һ�����ڽű�����
	virtual STRING			GetAttributeValue(LPCTSTR szValueName);

	//��¡��ϸ��Ϣ
	virtual VOID			Clone(const CObject_Item * pItemSource);
	//�õ���װ���
	INT						GetSetID(VOID) const;
	//�õ���������(����Ƿ�����������WEAPON_TYPE_NONE)
	ENUM_WEAPON_TYPE		GetWeaponType(VOID) const;
	//����װ����ǰ�;öȶ���һ���Ѿ����ɵ�װ����ֻ�����ֵ�ᱻ�ı�
	VOID					SetDur(INT Dur);
	//��õ�ǰ�;ö�
	INT						GetDur(VOID) const;
	//����װ����ǰ����;ö�
	VOID					SetMaxDur(INT Dur);
	//��õ�ǰ����;ö�
	INT						GetMaxDur(VOID) const;
	//�������;ö�
	INT						GetMaxDurFromTable(VOID) const;
	
	//��¡��ϸ��Ϣ
	VOID					ClonetExtraInfo(CObject_Item_Equip * pItemSource);
	//�����ϸ��Ϣ
	VOID*					GetExtraInfo() const{return m_pExtraDefine;}

	//�õ�װ������ͨ�Ļ��Ǹ߼���
	INT                     GetItemEquipQua(){ return m_eEquipStyle;}

    //���װ����Ч
	 const  EQUIP_EFFECT*            GetEffect();	 


	//--------------------------------------------------------------------------------------------------------------
	//-- supertooltip ʹ��
	//

	// �õ����ʹ�������Ʒ��Ҫ�ĵȼ�
	virtual INT				GetNeedLevel();

	// �õ���Ʒ�;ö�
	virtual INT				GetItemDur();

	// �õ���Ʒ����;ö�
	virtual INT				GetItemMaxDur();

	// �õ���Ʒ�İ���Ϣ 
	virtual INT				GetItemBindInfo();
	// �õ���Ʒ��2������Ϣ 
	virtual INT				GetItemSecLevelBindInfo();


	//�õ�װ������
	VOID					GetEquipCardForTip(STRING& strValue) ;
	//�󶨼ӳ���Ϣ
	virtual LPCTSTR			GetItemBindAddInfo();

	// �õ���Ʒ����npc�ļ۸�
	virtual INT				GetItemPrice();

	//�õ���Ʒְҵ
	virtual INT             GetItemProfession();

	//�õ�װ��Ŀǰ��׵���Ŀ
	INT						GetEquipHoleNum();

	//�õ�װ��Ŀǰ���׵���Ŀ
	INT						GetEquipHoleMaxNum();

	//�õ�װ�����ͼ������Ϣ
	LPCSTR						GetEquipSpritePaiInfo();

	//�õ��������װ��С�������Ϣ
	LPCSTR						GetOtherEquipSpriteChainInfo();
	//�õ�װ��С�������Ϣ
	LPCSTR						GetEquipSpriteChainInfo();

	//�õ��������װ���󼤻����Ϣ
	LPCSTR						GetOtherEquipSpriteAllActiveInfo();
	//�õ�װ���󼤻����Ϣ
	LPCSTR						GetEquipSpriteAllActiveInfo();

	//�õ�װ����װ��Ϣ
	LPCTSTR					GetEquipSetInfo();
	//�õ�װ����װ��Ϣ
	LPCTSTR					GetOtherEquipSetInfo();

	//�õ�װ����װ�����������Ϣ
	LPCTSTR					GetEquipSetActiveAttr();
	//�õ�װ����װ�����������Ϣ
	LPCTSTR					GetOtherEquipSetActiveAttr();

	//����װ��Ŀǰ��׵���Ŀ

	VOID					SetEquipHoleNum( INT num);
	// �õ���Ʒ��������
	virtual LPCTSTR			GetManufacturer();

	// �õ�װ����������
	LPCTSTR                 GetEquipBaseAttrInfo();
	 
	// �õ�װ����������
	LPCTSTR			        GetEquipAddAttrInfo();


	// �õ�װ����������
	LPCTSTR			        GetEquipSkillInfo();

	// �õ���Ʒ���������� 
	virtual LPCTSTR			GetItemTableTypeDesc();

	// ��ʽ�������ַ���
	void					SetAttri(const _ITEM_ATTR& att, char* const pszBuf);

	
	// �õ���ʯ�ĸ���.
	INT						GetGemMaxCount(VOID) const;

	//�õ���ʯ 
	BOOL					GetEquipGem( INT  index ,_ITEM_GEMINFO &gem);

	// �õ���ʯ��ͼ��
	VOID					GetGemIcon(INT iGemIndex, STRING& strIconName);

	// �õ���ʯ�ĸ��ӹ���
	VOID					GetGemExtAttr(INT iGemIndex, STRING& strGemAttr);

/*
	CObject_Item*			GetAppendItem(INT nIndex) const;

	VOID					SetAppendItem(CObject_Item* pItem,INT nIndex);
*/


public:

	//���Ժͱ�ʯ
	typedef std::vector< _ITEM_ATTR >		EQUIP_ATTRIBUTES;
	typedef std::vector< _ITEM_GEMINFO >	EQUIP_ATTACH_GEM;

	//���Ӽ��ܺ�Ӱ�켼��
	typedef std::vector< _ITEM_AFFIX_SKILL >  EQUIP_AFFIX_SKILL_VEC;  
	typedef std::vector< _ITEM_EFFECT_SKILL > EQUIP_EFFECT_SKILL_VEC;

public:
	CObject_Item_Equip(INT id);
	virtual ~CObject_Item_Equip();


	//����һ��װ��
	VOID		AsEquip( const  _DBC_ITEM_EQUIP* pDefine);

   //�߼�װ��
	VOID		AsAdvEquip( const _DBC_ITEM_EQUIP* pDefine ); 


   //����װ���������� 

	VOID		GetAllEquipBaseAttribute(STRING& strValue);
	//����װ������ 

	VOID		GetAllEquipAddAttribute(STRING& strValue);

	//����װ���������� 

	VOID		GetAllEquipSkillAttr(STRING& strValue);

	//�õ���Ƕ�ı�ʯ����.
	INT 		GetGemCount();

	// �õ���Ʒ����typeһ�� 
	virtual INT	GetItemTableType(VOID);


public:

	//--------------------------------------------------------------
	

	struct   Base_Attr
	{

		INT  m_nBase_Atack_Near;
		INT  m_nBase_Atack_Far;
		INT  m_nBase_Atack_Magic;

		INT  m_nBase_Def_Near;
		INT  m_nBase_Def_Far;
		INT  m_nBase_Def_Magic;

		Base_Attr()
		{
			m_nBase_Atack_Near = -1;
			m_nBase_Atack_Far  = -1;
			m_nBase_Atack_Magic = -1;

			m_nBase_Def_Near  = -1;
			m_nBase_Def_Far   = -1;
			m_nBase_Def_Magic = -1;

		}

		~Base_Attr()
		{
			m_nBase_Atack_Near = -1;
			m_nBase_Atack_Far  = -1;
			m_nBase_Atack_Magic = -1;

			m_nBase_Def_Near  = -1;
			m_nBase_Def_Far   = -1;
			m_nBase_Def_Magic = -1;

		}
	};

	//�������Զ���
	struct BASE_DEFINE
	{
		//װ�����ݱ�
		const	 _DBC_ITEM_EQUIP*          pDefineEquip	; 
		//union
		//{   //�߼�װ������
		//	const _DBC_ITEM_ADVANCED_EQUIP_VISUAL*      pAdvVisual;
		//	//��ͨװ������
		//	const _DBC_ITEM_COMMON_EQUIP_VISUAL*		pCommonVisual;
		//};
		//װ������
		const _DBC_ITEM_EQUIP_VISUAL*      pEquipVisual ;

		Base_Attr			m_Base;
		
		BASE_DEFINE()
		{
			pEquipVisual = NULL;
			pDefineEquip = NULL;
		}

		~BASE_DEFINE()
		{
			pEquipVisual = NULL;
			pDefineEquip = NULL;
		}

		
	};



	//  ������ͨ�͸߼�װ��
	enum   EQUIP_ENUM
	{
		 INVAILD_EQUIP = -1,	//��Ч
		 COMMON_EQUIP ,			//��ͨ 
		 ADVANCED_EQUIP,		//�߼�
			
	};

	//  ������ͨ�͸߼�װ��
	enum   EQUIP_ENUM_STYLE
	{
		INVAILD_EQUIP_STYLE = -1,	//��Ч
		IB_EQUIP_STYLE ,			//ib 
		MANUFACTURE_EQUIP_STYLE,	//���������װ��


	};

 

	enum EQUIP_BIND_INFO
	{
		FREE_BIND = 0,			// �ް�����
		BINDED,					// �Ѿ���
		GETUP_BIND,				// ʰȡ��
		EQUIP_BIND,				// װ����  
	 	EQUIP_BIND_TWO  = 1,        // ʹ�ò��Ͻ��еİ󶨣�ĿǰΪ2���� ���Ժ�����и��߼��İ�    
	};
	//װ����type��
	enum EQUIP_ID_TYPE
	{
		EQ_TY_WEAPON  = 0,  //����
		EQ_TY_WEAPON_MAXNUM =6 , //��������
		EQ_TY_DEFENCE = 7,	//���� ����
		EQ_TY_RING = 28,	//��ָ
        EQ_TY_ADORN = 29	//��Ʒ

	};


	//��չ���Զ���
	struct EXTRA_DEFINE
	{
		//---���շ���������
		 
		INT					m_nAttSet;					//������
		BYTE			    m_StartLevel;				//�Ǽ�
		INT					m_CurDurPoint;				//��ǰ�;�ֵ                 
		USHORT				m_CurDamagePoint;		    //��ǰ���˶�
		BYTE				m_GemCount;					//��Ƕ�ı�ʯ����
		EQUIP_BIND_INFO		m_nEquipBindInfo;			//װ���Ƿ��
		EQUIP_BIND_INFO     m_nEquipBindInfo2;          // װ���Ƿ�ǿ���󶨣�2���󶨣�
		
		BYTE								m_nEquipAttachGemNum;	//���ӵı�ʯ������
		EQUIP_ATTACH_GEM	m_vEquipAttachGem;			//���ӵı�ʯ
		INT					m_nEquipHoleNum;			//װ���Ѿ��еĿ׶�����Ŀ

		INT					m_nEquipScore;				//װ���ķ������÷�������ĳֵ��װ��������������Լ�����װ��������
	
       //--------------------------------------------------------------

		BYTE							m_nEquipAttributesNum;		//װ�������Ե�����
		EQUIP_ATTRIBUTES	m_vEquipAttributes;			//װ������
		
		INT					m_nSellPrice;				//�۳��۸�
		INT					m_nLevelNeed;				//����ȼ� 
		INT					m_nMaxDurPoint;				//����;�ֵ
		INT					m_SetNum;					//��װid

		BYTE					m_nAffix_SkillNum;			//���ӵļ��ܵ�����
		EQUIP_AFFIX_SKILL_VEC   m_Affix_Skill;			//���Ӽ���
		
		BYTE					m_nEffect_SkillNum;			//Ӱ��ļ��ܵ�����
		EQUIP_EFFECT_SKILL_VEC  m_Effect_Skill;         //Ӱ�켼��
		
		BYTE					m_nszAttrGroupNum;			//���������Ĵ�С
		STRING				m_szAttrGroup;				//��������

		CHAR				m_RulerID;            //�¹���
		BYTE				m_EquipType;		  //װ������
		BYTE				m_AttrCount;		  //���Ե�����
		INT					m_PaiXing;			  //װ��������	

		BYTE					m_SoulBeadType;							//��ħ�Ļ�������add by gh for soul_bead 2010/06/30
		BYTE					m_SoulBeadAttrCount;						//�������Ե�����

   
		EXTRA_DEFINE()
		{
     		 		
			m_nAttSet = 0;				
			m_StartLevel  = 0;				
			m_CurDurPoint = 0;		
			m_CurDamagePoint = 0;				
			m_GemCount   = 0;		
			m_nEquipBindInfo = FREE_BIND;	
			m_nEquipBindInfo2 =FREE_BIND;

			m_nEquipHoleNum = 0;
			m_nEquipScore  = 0;
			m_nSellPrice = 0;
			m_nLevelNeed = 0;
			m_nMaxDurPoint = 0;
			m_SetNum = -1;
			m_szAttrGroup = "";
			m_RulerID = -1;
			m_EquipType = -1;
			m_AttrCount =0;
			m_PaiXing = -1;

			m_nEquipAttachGemNum = 0;
			m_nEquipAttributesNum = 0;
			m_nAffix_SkillNum = 0;
			m_nEffect_SkillNum = 0;
			m_nszAttrGroupNum = 0;
		}

		INT	WriteVarAttr( CHAR* pIn );
		BOOL	ReadVarAttr( CHAR* pOut, INT& OutLength );
	};


public:

	// �õ�װ����������
	BASE_DEFINE	GetEquipBaseDefine() { return m_theBaseDef;}

public:

	enum EQUIP_ATTRIB
	{
		EQUIP_ATTRIB_UNIDENTIFY,	// װ��δ������
		EQUIP_ATTRIB_IDENTIFY,		// װ���Ѽ�����
	};


	//����
	EQUIP_ATTRIB	m_EquipAttrib;
	EQUIP_ATTRIB	GetEquipAttrib();

	// �Ƿ�������
	BOOL IsWeapon();

	// �Ƿ��Ƿ���
	BOOL IsDefence();

	// �Ƿ���װ����
	BOOL IsAppend();

protected:
	
	//������ͨװ���͸߼�װ��
     EQUIP_ENUM			m_eEquipStyle;
	 

	//--------------------------------------------------------------
	//��������
	BASE_DEFINE			m_theBaseDef;			//�������Զ���(���ݱ�)

	//----------------------------------------------------------------
	//��չ����(��Ҫ����������)
	EXTRA_DEFINE*		m_pExtraDefine;
	STRING				m_strExtraDesc;

//	std::vector< CObject_Item* >	m_vAppendItem;
};


//װ�������Ƿ�
BOOL					Equip_Card_Is_Jiang( INT  c1 ,INT c2, BOOL isSpecial = FALSE);
//װ�������Ƿ����
BOOL					Equip_Card_Is_Equal( INT  c1 ,INT c2,INT c3);
//װ�������Ƿ�����
BOOL					Equip_Card_Is_Series( INT  c1 ,INT c2,INT c3);	
//����װ��������С��
INT					    Equip_Card_Min( INT  c1 ,INT c2, INT c3);
const  INT  max_sprite_chain_num  =   1000;   //�������id
const  INT  max_sprite_all_active_num  =  2000;   //�������id
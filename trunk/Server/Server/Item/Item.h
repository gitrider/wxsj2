/////////////////////////////////////////////////////////////////////////////////
//�ļ�����Item.h
//������������Ʒ���ݵķ�װ�࣬ͨ����������ȡ������һ����Ʒ����Ϣ
//�����ߣ�jack
//�޸ļ�¼��2005-11-2����
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef __ITEM_H__
#define __ITEM_H__

#include "Type.h"

struct _PET_DB_LOAD;
struct  PET_GUID_t;
struct _PET_SKILL;


struct _CREATEITEM_INIT
{
	UINT					m_nItemIndex ;//
	BYTE					m_nEquipFromType;
};

struct _HUMAN_EQUIPCREATE_INIT: 
	public _CREATEITEM_INIT
{
	UINT					m_nEquipScore;
	UINT					m_nPlayerJob;
};

struct _HUMAN_EQUIPDROP_INIT: 
	public _CREATEITEM_INIT
{
	UINT					m_nMonsterLevel;
	UINT					m_nMonsterType;
};

class _ITEM_INIT
{
public:
	_ITEM_INIT(const _ITEM*	pItem)
	{
		m_pItem = pItem ;
		m_pPet  = NULL;
	}

	_ITEM_INIT(const _PET_DB_LOAD* pPet)
	{
		m_pPet	= pPet;
		m_pItem = NULL;
	}
	const	_ITEM*				m_pItem ;//��Ʒ��ϸ����
	const	_PET_DB_LOAD*		m_pPet;	 //������ϸ����
};

enum ITEM_DATA_TYPE
{
	IDT_ITEM,
	IDT_PET,
};

struct _CAMP_DATA;
class Item
{
friend class ItemOperator ;
friend class ItemContainer ;
friend struct Obj_ItemContaner;
friend class Obj_ItemManager;

public :
	Item( ) ;
	~Item( ) ;

	ITEM_DATA_TYPE			GetDataType(){ return m_Type;}//ȡ����������

	VOID					SaveValueTo(_ITEM* pItem);
	VOID					SaveValueTo(_PET_DB_LOAD* pPet);
	inline  _ITEM_GUID		GetGUID( ) ;					//ȡ����ƷGUID
	inline  UINT			GetItemTableIndex( ) const; 	//ȡ����Ʒ�������Ʒ�������
	inline	UCHAR			GetItemClass()		 const;		//ȡ����Ʒ��	enum ITEM_CLASS
	
	inline	UCHAR			GetItemType()		 const;		//ȡ����Ʒ��С��WEAPON_TYPE DEFENCE_TYPE ��
	inline	UCHAR			GetItemIndex()		 const;		//ȡ����Ʒ����ˮ��

	BOOL					IsCanEquipBind()	const;		//ȡ����Ʒ�Ƿ���а�����
	BOOL					IsRuler(ITEM_RULER_LIST	ruler)	const;			//ȡ����Ʒ�Ƿ����ִ��ָ������
	BOOL					IsBind()			const;		//��ѯ�Ƿ��
	
	UINT					GetPrice()	const;
	UINT					GetSellPrice() const;
	INT						GetItemParam(UINT start,ItemParamValue& ipv);

	BOOL					GetItemTransferString(CHAR* pOutBuf, UINT nOutBufLength);		//ȡ����Ʒ����ϵͳ����Ʒ�ִ�	20100706 BLL
	BOOL					GetPetTransferString(CHAR* pOutBuf, UINT nOutBufLength);		//ȡ����Ʒ����ϵͳ�������ִ�	20100706 BLL

	/*=============================================
	 *	װ������
	 =============================================*/
	inline _ITEM*			GetItemData()				const;			//���װ������������
	inline INT				GetSetNum()					const;			//��װ���
	inline BYTE				GetMaxSetCount()			const;			//��װ����
	inline BYTE				GetEquipPoint()				const;			//��Ʒװ���
	inline INT				GetMaxDurPoint()			const ;			//����;�ֵ
	inline BYTE				GetRequireLevel()			const;			//����ȼ�
	inline BYTE				GetCurLevel()				const;			//����ȼ�
	inline BYTE				GetGemSlotMax()				const;			//���ʯ����
	inline BYTE				GetModelID()				const;			//ģ��
	inline INT				GetDurPoints()				const; 			//��ǰ�;�ֵ
	inline INT				GetCurMaxDurPoint()			const; 			//��ǰ�;�����
	inline BYTE				GetAttrCount()				const;			//���Ե�����
	inline USHORT			GetDamagePoint()			const;			//��Ʒ�����ۼ�ֵ
	inline BOOL				GetItemBind()				const;			//��Ʒ�Ƿ��
	inline BOOL				GetItemIdent()				const;			//��Ʒ�Ƿ����
	inline BYTE				GetEquipGemCount()			const;			//��Ƕ��ʯ������
	inline BYTE				GetAffixSkillCount()		const;			//��ø��Ӽ��ܸ���
	inline BYTE				GetEffectSkillCount()		const;			//���װ��Ӱ�켼�ܸ���
	inline _ITEM_EFFECT_SKILL*	GetEffectSkill(UINT pos)		const;			//���װ��Ӱ�켼�ܸ���
	inline BOOL				IsWuMenPai()				const;			//���װ���Ƿ�����������
	inline BOOL				InReqJob(MENPAI_ATTRIBUTE menpaiID)const;	//ָ���������Ƿ���װ������������һ��
	inline BYTE				GetCurGemHole()				const;			//
	inline BOOL				AddHole();									//װ���ӿ�
	inline BOOL				EquipLevelup(INT nLevel);								//װ������
	inline VOID				EquipBind(UINT nBindLevel);					//װ����
	inline BOOL				AddGem(INT nHolePos, INT GemId);			//�ӱ�ʯ
	inline BYTE				GetEquipBindLevel();						//װ����
	inline VOID				UpDataEquipGem()			const;			//���¼�������
	inline UINT				GetEquipGem(INT nPos)		const;			//��Ƕ�ı�ʯ
	
	inline UINT				GetEquipScore()				const;			//���װ����
	inline BYTE				GetEquipFromType()		    const;			//���װ����Դ����

	inline VOID				SetEquipScore(UINT nScore)	const;			//װ����
	inline VOID				SetEquipFromType(BYTE nType)const;			//װ����Դ����

	inline const _ITEM_ATTR		GetEquipAttr(UCHAR	nIndex)		const;		//���װ������
	inline VOID					SetEquipAttr(UCHAR	nIndex, _ITEM_ATTR ItemAttr);
	inline const _ITEM_GEMINFO	GetEquipGemInfo(UCHAR nIndex)	const;		//���װ���ϱ�ʯ�ı����Ϣ

	inline BYTE				GetEquipPaiType()		    const;			//���װ������
	inline VOID				SetEquipPaiType(BYTE nPaiType);
	inline BOOL				IsAdsorbMagic()				const;			//װ���Ƿ�ħ
	inline BOOL				AdsorbMagic(_ITEM* Bead, BYTE Kind );// װ�����ӻ��������
	inline BOOL				CancelMagic();						 // װ����ȥ��ħ������
	/*==========================================
	 *	��ͨ��Ʒ����
	 ============================================*/		
	inline UINT				GetItemSellPrice()		const;				
	inline BYTE				GetItemLevel()			const;				//��Ʒ����,�߻�����
	inline BYTE				GetReqSkillLevel()		const;				//��ü��ܼ�������
	inline INT				GetReqSkill()			const;				//��Ҫӵ�еļ��ܺ�
	inline INT				GetScriptID()			const;				//�����ؽű����
	inline INT				GetSkillID()			const;				//�����ؼ��ܱ��
	inline BYTE				GetTargetType()			const;
	inline BOOL				IsBroadCast()			const;				//
	/*==============================================
	 *��ʯ����	
	 ===============================================*/
	inline const _ITEM_ATTR		GetGemAttr(UINT i)			const;				//��ʯ����
	
	inline UINT				GetGemReqNum()			const;					//ǰ����Ƕ�ı�ʯ

	inline const _GEM_REQ_ATT			GetReqGemAtt(INT nPos)	const;
	inline UINT				GetReqEquip(INT nEquipType)const;				//װ������
	/*==============================================
	*�����������	
	===============================================*/
	inline UINT				GetExtraBagSellPrice()		const;				
	inline UINT				GetExtraBagBasePrice()		const;			
	inline UINT				GetExtraBagValidTime()		const;				//
	inline UINT				GetExtraBagRemainTime()		const;	
	inline BOOL				IsExtraBagCanUse()		const;				//
	inline UINT				GetExtraBagSize()			const;					//
	inline UINT				GetExtraBagBeginTime()		const;	
	inline VOID				SetExtraBagBeginTime();
	/*==========================================
	 *�ر�ͼ����	
	============================================*/

	inline INT				GetMapLevel()			const;				//�ر�ͼ����
	inline FLOAT			GetXPos()				const;				//x����
	inline FLOAT			GetZPos()				const;				//z����
	inline INT				GetSceneID()			const;				//�������
	inline INT				GetGrowType()			const;				//��Ӧ����������

	/*==========================================
	 *��������	add by gh 2010/06/28
	============================================*/
	VOID					SetWuSoulBeadAttr(const BYTE attrCount, const _ITEM_ATTR* tmpItemAttr);//�����������������
	VOID					SetQiSoulBeadAttr(const BYTE attrCount, const _ITEM_ATTR* tmpItemAttr);//������������������
	inline VOID				SetSoulBeadPrice(UINT price);	//���û�����������
	inline VOID				SetSoulBeadNeedLevel(BYTE needLevel);//���û�������ȼ�
	inline UINT				GetSoulBeadPrice()		const;	//��û�������
	inline BYTE				GetSoulBeadNeedLevel()	const;	//��û�������ȼ�
	
	inline BYTE				GetSoulBeadAttrCount()	const;			//�������Ե�����
	inline const _ITEM_ATTR		GetWuSoulBeadAttr(UCHAR	nIndex)		const;		//������������
	inline const _ITEM_ATTR		GetQiSoulBeadAttr(UCHAR	nIndex)		const;		//�������������

private:

	inline VOID				SetWuSoulBeadAttr(const _ITEM_ATTR&  ItemAttr);
	inline VOID				SetQiSoulBeadAttr(const _ITEM_ATTR&  ItemAttr);

public:
	//ITEM ��PET ���ú���
	
	BOOL					Init( const _ITEM_INIT* pInit );//��ʼ����Ʒ
	BOOL					IsEmpty( ) ;					//�ж���Ʒ�Ƿ�Ϊ�գ���û����Ʒ����
	BOOL					IsEquip( ) const;				//�жϴ���Ʒ�Ƿ�Ϊһ��װ��
	BOOL					IsLock( ){ return m_bLock ; } ; //�жϴ���Ʒ�Ƿ�����
	BOOL					IsCanLay( )			const;		//ȡ����Ʒ�Ƿ���Ե���
	inline	INT				GetLayedNum( )  const;			//ȡ�õ�����Ʒ������
	inline	INT				GetMaxLayedNum( ) const;		//ȡ���ܹ�������Ʒ���������
	inline  BOOL			IsFull()	const;				//�Ƿ��Ѿ��ﵽ��������Ŀ

	inline  BOOL			IsPWLock()	const;				//�����뱣��
public:
	//���ﺯ��

	inline  PET_GUID_t			GetPetGUID( ) ;								//ȡ�ó���GUID
	inline  PET_GUID_t			GetSpouseGUID( ) ;							//ȡ�ó�����żGUID
	inline	INT					GetDataID();								//ȡ�ó���ģ��		
	inline  const CHAR*			GetName()	const;							//ȡ������
	inline  const CHAR*			GetNick() const;							//ȡ���ǳ�
	inline  INT					GetLevel() const;							//ȡ�õȼ�
	inline  INT					GetTakeLevel() const;						//ȡ��Я���ȼ�
	inline  INT					GetAttackType() const;						//ȡ�ý������ͣ���/����
	inline  INT					GetAIType() const;							//ȡ��AI����
	inline  const _CAMP_DATA	*GetCampData() const;						//ȡ����Ӫ
	inline  INT					GetHP() const{return 0;};					//ȡ������ֵ
	inline  INT					GetBaseMaxHP() const{return 0;};			//ȡ������ֵ����
	inline  INT					GetLife() const {return 0;};				//ȡ�õ�ǰ����
	inline  BYTE				GetPetType() const;							//���������죬Ұ��
	inline  BYTE				GetGeneration() const;						//������
	inline  BYTE				GetHappiness() const;						//���ֶ�		
	//�������
	inline  INT					GetStrengthPer() const;						// ��������	
	inline  INT					GetSmartnessPer() const;					// ��������	
	inline  INT 				GetMindPer() const;							// ��������
	inline  INT 				GetConstitutionPer() const;					// ��������
	//�����������
	inline	INT					GetGenGu() const;							//����
	inline  FLOAT				GetGrowRate() const ;						//�ɳ���
	inline  INT					GetSavvy() const;							//����
	inline  INT					IsCheckup();								//�Ƿ��Ѽ���
	inline  INT					GetRemainPoint() const;						//һ������ʣ�����
	inline  INT					GetExp() const;								//����ֵ
	inline  INT					GetLvl1Attr(CHAR_ATTR_LEVEL1 type)const;	// ����һ��ս�����ԣ����������ܺ�װ�����ӵĲ��֣�
	inline  _PET_SKILL			GetSkill(UINT SkillIndex);					// ���＼��
	inline  VOID				SetCooldown(CooldownID_t nID, Time_t nTime);
	inline	BOOL				IsCooldowned(CooldownID_t nID) const;
	inline	VOID				HeartBeat_Cooldown(Time_t nDeltaTime);
	inline	Time_t				GetCooldownRemained(CooldownID_t nID) const;
	inline	BOOL				IsMating() const;
	inline  INT					GetSex() const;
	/*==============================================
	*���ܽӿ�
	===============================================*/
	//����
	BOOL						IsInExchange(){return m_bIsInExchange;}
	VOID						SetInExchange(BOOL bIsInExchange){m_bIsInExchange = bIsInExchange;}

protected :
	VOID					Lock( ){ m_bLock = TRUE ; }
	VOID					Unlock( ){ m_bLock = FALSE ; }
	VOID					SetItemValue(_ITEM* pItem);
	VOID					CleanUp( ) ;//�����Ʒ����

private :
	ITEM_DATA_TYPE			m_Type;
	_ITEM*					m_pItem ;					//��Ʒ��ϸ����
	_PET_DB_LOAD*			m_pPet;						//
	BOOL					m_bLock ;
	BOOL					m_bIsInExchange;			//��Ǵ���Ʒ���ڱ��ŵ���������
};

#include "Item.inl"

struct _EXCHANGE_ITEM_LIST 
{
	INT				m_ListCount;
	Item*			m_ListItem[EXCHANGE_BOX_SIZE+EXCHANGE_PET_BOX_SIZE];

	_EXCHANGE_ITEM_LIST()
	{
		memset(this,0,sizeof(_EXCHANGE_ITEM_LIST));
	}

	VOID	AddItem(Item* it)
	{
		INT i;

		for(i=0;i<EXCHANGE_BOX_SIZE+EXCHANGE_PET_BOX_SIZE;i++)
		{
			if(m_ListItem[i] == NULL)
			{
				break;
			}
		}

		if(i == EXCHANGE_BOX_SIZE+EXCHANGE_PET_BOX_SIZE)
		{
			Assert(0);
			return;
		}

		m_ListCount++;
		m_ListItem[i] = it;
	}

	VOID Init()
	{
		memset(this,0,sizeof(_EXCHANGE_ITEM_LIST));
	}
};


struct  Obj_ItemContaner
{
	Obj_ItemContaner(){
		CleanUp();
	}
	
	VOID    CleanUp()
	{
		memset(m_Item,0,sizeof(_ITEM)*MAX_DROP_ITEM_NUMBER);
		m_nCount = 0;
	}

	VOID	AddItem(_ITEM& item)
	{
		Assert(!item.IsNullType());
		memcpy(&m_Item[m_nCount], &item, sizeof(_ITEM));
		m_nCount++;
	}
	_ITEM				m_Item[MAX_DROP_ITEM_NUMBER]; 
	INT					m_nCount;
	UINT				m_uDropType;
};
#endif

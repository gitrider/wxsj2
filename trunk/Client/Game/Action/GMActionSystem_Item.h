
/** ����������-��Ʒ
 */

#pragma once


#include "GMActionSystem.h"


class CActionItem_Item : public CActionItem
{
public:

	//�õ���������
	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_ITEM;	}
	//�����ַ���
	virtual	LPCTSTR			GetType_String(VOID);
	//�õ�����ID
	virtual INT				GetDefineID(VOID);
	//�õ�����
	virtual INT				GetNum(VOID);
	//�õ��ڲ�����
	virtual VOID*			GetImpl(VOID)			{ return (VOID*)GetItemImpl();	}
	//�õ�����
	virtual LPCTSTR			GetDesc(VOID);
	//�õ���ȴ״��ID
	virtual INT				GetCoolDownID(VOID);


	//----------------------------------------------------------------------------------------------------------
	//-- super tool tip ʹ��
	//--

	// �õ�ʹ����Ʒ�ĵȼ�
	virtual INT				GetItemLevel();
	// �õ���Ʒ���;�
	virtual INT				GetItemDur();
	// �õ���Ʒ������;�
	virtual INT				GetItemMaxDur();
	// �õ���Ʒ���������
	virtual INT				GetItemRepairCount();
	// �õ���Ʒ�İ���Ϣ 
	virtual INT				GetItemBindInfo();

	// �õ�װ����2�ΰ���Ϣ 
	virtual INT				GetItemSecLevelBindInfo();

	//	�õ�װ����2�ΰ󶨼ӳ�
	virtual LPCTSTR			GetItemBindAddInfo();

	//	�õ�װ������ɫ
	virtual LPCTSTR			GetDropVisColor();

	// 20100408 AddCodeBegin
	// �õ���Ʒ����ļ۸�
	virtual INT	GetItemBasePrice();
	// 20100408 AddCodeEnd

	// �õ���Ʒ����npc�ļ۸�
	virtual INT				GetItemPrice();
	//�õ���Ʒ�Ƿ�Ψһ
	virtual	INT				GetItemRuleOnly();
	//�õ���Ʒְҵ
	virtual INT				GetItemProfession();
	//�õ�װ������
	virtual INT				GetEquipCard();

	// �õ���Ʒ��������
	virtual LPCTSTR			GetManufacturer();
	//�õ�װ���Ŀ�����
	virtual	INT				GetEquipHoleNum();
	//�õ�װ������װ��Ϣ
	virtual LPCTSTR			GetEquipSetInfo();

	//�õ�װ�����ͼ������Ϣ 
	virtual LPCTSTR			GetEquipSpritePaiInfo();

	//�õ��������װ��С�������Ϣ
	virtual LPCTSTR			GetOtherEquipSpriteChainInfo();
 
	//�õ�װ��С�������Ϣ
	virtual	LPCTSTR			GetEquipSpriteChainInfo();

	//�õ��������װ���󼤻����Ϣ
	virtual	LPCTSTR			GetOtherEquipSpriteAllActiveInfo();

	//�õ�װ���󼤻����Ϣ
	virtual	LPCTSTR			GetEquipSpriteAllActiveInfo();
 
	//�õ�װ������װ��Ϣ
	virtual LPCTSTR			GetOtherEquipSetInfo();
	

	//�õ�װ����װ�����������Ϣ

	virtual LPCTSTR			GetEquipSetActiveAttr();
	//�õ�װ����װ�����������Ϣ

	virtual LPCTSTR			GetOtherEquipSetActiveAttr();


	
	//�õ�װ�����Ǽ�
	virtual	INT				GetStartLevel();
	 
	// �õ���ɫ����
     
	virtual LPCTSTR			GetBaseWhiteAttrInfo();
	// �õ���չ��ɫ����
//	virtual LPCTSTR			GetExtBlueAttrInfo(); //remove
	// �õ�action�����õ�item ������
	virtual INT				GetItemType();

	// �õ�װ������
	LPCTSTR	GetEquipBaseAttrInfo();
	// �õ�װ������
	LPCTSTR	GetEquipAddAttrInfo();

	//�õ�װ����������
	LPCTSTR GetEquipSkillInfo();

	
	// �õ���������
	virtual LPCTSTR			GetTypeDesc();



	//�õ���������������
	//	�����е���Ʒ	- �������е�λ��
	//	�����ϵ���Ʒ	- ������λ��
	//
	virtual INT				GetPosIndex(VOID);
	//�Ƿ��ܹ��Զ���������
	virtual BOOL			AutoKeepOn(VOID)		{ return FALSE; }
	//�����
	virtual VOID			DoAction(VOID);
	//�����(������)
	virtual VOID			DoSubAction(VOID);
	//�Ƿ���Ч
	virtual BOOL			IsValidate(VOID);
	//ˢ��
	virtual VOID			Update(tActionReference* pRef);
	//�����ȴ�Ƿ����
	virtual BOOL			CoolDownIsOver(VOID);
	//�϶�����
	virtual VOID			NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName);
	//��ѯ�߼�����
	virtual STRING			GetAttributeValue(LPCTSTR szAttributeName);

	//�������������Ŀ�ʼ����ֵ
	virtual INT				GetCurBankRentBegin(VOID);


public:

	// �õ�װ��������(��, ��, ��)
	//INT GetEquipQuantity(); //remove

	// �õ�װ���ϵı�ʯ����
	INT GetGemInEquipCount();

	// �õ�װ��������;�
	//INT GetItemMaxRepair();

	// �õ�װ���ϵı�ʯ��ͼ��
	VOID GetGemIcon(INT iGemIndex, STRING& strIconName);

	// �õ�װ���ϵı�ʯ����
	VOID GetGemInEquipDesc(STRING& strIconName);

	// �õ����ڵڼ�������
	INT	GetQuestIndex(VOID)	{return m_Quest_Number;}

	// �õ�װ���ļ�����Ϣ
	INT GetEquipIdentify();


public:

	//-----------------------------------------------------------------------------------------------------------------------
	//
	// ��ʯ��Ϣ
	//

	// �õ���ʯ�ȼ�
	INT GetGemLevel();

	// �õ���ʯ������Ϣ
	LPCTSTR	GetGemArribInfo();

	
	
	//-----------------------------------------------------------------------------------------------------------------------
	//
	// ������Ϣ
	//

	// �õ��������������
	LPCTSTR GetFecthHeadInfo_TypeDesc( INT Index );

	// �õ����������ֵ
	INT GetFecthHeadInfo_Value( INT Index );

	//�õ���������Ե�����
	INT GetFecthHeadInfo_Num();

	//-----------------------------------------------------------------------------------------------------------------------
	//
	// ԭ�Ϻ�ҩƷ
	//

	// �õ�ԭ�ϻ�ҩƷ������
	//INT GetItemQuality(); //remove

	//�õ�װ������ͨ�Ļ��Ǹ߼���
	INT  GetItemEquipQua();
	// �õ�����Ʒ�ڱ��е�����
	INT GetMedicineItemTableType();
	// �õ��䷽��Ҫ�ļ������ơ�
	LPCTSTR GetPrescrNeedSkillName();
	// �õ��䷽��Ҫ�ļ��ܵȼ���
	int GetPrescrNeedSkillLevel();
	// �õ��䷽��Ҫ���ܵĵ�ǰ�ȼ���
	int GetPrescrNeedSkillCurLevel();


	// ʹ�ü������� 
	VOID UseIdentify();

	// ׼��ʹ�ü������� 
	VOID PrepareUseIdentify();

	// ȡ������״̬ 
	VOID CancelIdentify();

	// �õ����������ٶ�         [7/21/2010 �¾���]
	INT	 GetMountAddtionSpeed();

	// �õ��������ʱ��         [7/21/2010 �¾���]
	INT  GetMountLife();

	// �õ������ȡʱ��         [7/21/2010 �¾���]
	INT	GetMountObtainDate();

	// �õ�����ʣ��ʹ��ʱ��     [7/21/2010 �¾���]
	INT  GetMountRemainTime();

	// �õ����ﵱǰ�Ƿ��Ѱ�   [7/21/2010 �¾���]
	BOOL GetMountIsBinded();

	// �õ����������         [7/21/2010 �¾���]
	BOOL  GetMountBindType();

public:

	CActionItem_Item(INT nID);
	virtual ~CActionItem_Item();

	// 
	VOID	Update_Item(tObject_Item* pItem);

	// ����������ϸ������Ϣ
	void SendAskItemInfoMsg();

	//�õ���Ʒ
	tObject_Item*			GetItemImpl(VOID);
	

protected:

	//������Ʒ����ʱ����ƷID(ָ��tObject_Item)
	INT		m_idItemImpl;

	//���ڵڼ�������
	INT		m_Quest_Number;

private:

	// ��װ��������ж��
	VOID	UnUseEquip();
	//DoAction_Packet
	VOID	DoAction_Packet(VOID);
	//������Ʒ�Ĳ���
	VOID	DestoryItem(LPCSTR szSourceName);

	//Ctrl+Click
	BOOL InjectItemInfo( BOOL bSubAction );

};

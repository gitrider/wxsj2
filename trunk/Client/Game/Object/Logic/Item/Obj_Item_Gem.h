
#pragma once


#include "Obj_Item.h"
#include "GameDefine2.h"
#include "Obj_Item_Equip.h"
#include "GIDBC_Struct.h"
#include "../../ObjectDef.h"
#include "GameStruct_Item.h"


/*
|
|	��ʯ����Ʒ
|
*/
class CObject_Item_Gem : public CObject_Item
{
public:

	//�õ���Ʒ�����
	virtual ITEM_CLASS		GetItemClass(VOID) const { return (ITEM_CLASS)m_theBaseDef->nClass; }
	//��Ʒ������
	virtual LPCTSTR			GetName(VOID) const			{ return m_theBaseDef->szName; }
	//��Ʒ����
	virtual LPCTSTR			GetDesc(VOID) const			{ return m_theBaseDef->szDesc; }
	//ͼ����
	virtual LPCTSTR			GetIconName(VOID) const;
	//��ϸ����(������Ҫ������)
	virtual LPCTSTR			GetExtraDesc(VOID);
	//������ϸ����
	virtual VOID			SetExtraInfo(const _ITEM * pItemInfo);
	virtual VOID			SetExtraInfo( CHAR* pstrIn ) {};
	/*virtual LPCTSTR	GetExtranInfo(){ return GetExtraDesc(); };*/
	//��¡��ϸ��Ϣ
	virtual VOID			Clone(const CObject_Item * pItemSource);
	
	 
	//��ʯ����
	INT						GetGemType(VOID) const			{ return m_theBaseDef->nType; }	

	//��������
	 
	virtual LPCTSTR				GetDropVisualID(VOID)	const;

	virtual LPCTSTR				GetDropVisColor(VOID) const ;

	// 20100408 AddCodeBegin
	// �õ���Ʒ����ļ۸�
	virtual INT				GetItemBasePrice();
	// 20100408 AddCodeEnd


	// �õ���Ʒ����npc�ļ۸�
	virtual INT				GetItemPrice();

	// �õ���ʯ�ĵȼ� 
	INT						GetGemLevel();

	// �õ���ʯ������Ϣ
	LPCTSTR					GetGemArribInfo();

	
	// �õ�type��Ϣ
	virtual INT				GetItemTableType(VOID);

	// �õ���Ʒ���������� 
	LPCTSTR	GetItemTableTypeDesc();
/*
	CObject_Item*			GetHost(VOID) const				{ return m_HostOfAppend; }

	VOID					SetHost(CObject_Item* HoseItem) { m_HostOfAppend = HoseItem;}
*/
public:
	CObject_Item_Gem(INT id);
	virtual ~CObject_Item_Gem();
	//����һ����ʯ
	VOID					AsGem(const _DBC_ITEM_GEM* pGemDefine);

protected:
	//--------------------------------------------------------------
	//��������
	const _DBC_ITEM_GEM*	m_theBaseDef;			//�������Զ���(���ݱ�)

	STRING					m_strExtraDesc;

//	CObject_Item*			m_HostOfAppend;			//�������ļ�װ���ϡ�
};

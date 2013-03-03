
#pragma once


#include "Obj_Item.h"
#include "GameDefine2.h"
#include "GameDefine_Item.h"
#include "Obj_Item_Equip.h"
#include "GIDBC_Struct.h"
#include "../../ObjectDef.h"
#include "GameStruct_Item.h"

/*
|
|	��������Ʒ
|
*/
#define SOULBEAD_PARAMNUM 4
class CObject;
struct PET_GUID_t;
class CObject_Item_SoulBead : public CObject_Item
{
public:

	//�õ���Ʒ�����
	virtual ITEM_CLASS		GetItemClass(VOID) const ;
	// �õ�type��Ϣ
	virtual INT				GetItemTableType(VOID);

	//ͼ����
	virtual LPCTSTR			GetIconName(VOID) const;

	//��Ʒ������
	virtual LPCTSTR			GetName(VOID) const			{ return m_theBaseDef->szName; }
	//��Ʒ����
	virtual LPCTSTR			GetDesc(VOID) const			{ return m_theBaseDef->szDesc; }

	//�õ���Чʱ��
	INT		GetEffectTime();

	//�õ��Ƿ�㲥
	INT		GetIsRadio();

	// �õ���Ʒ���������� 
	virtual LPCTSTR					GetItemTableTypeDesc();


	//�õ�ѡ���������
	INT		GetSelectObjType();

	//�õ�����ʱ��ģ��id
	virtual LPCTSTR				GetDropVisualID(VOID)	const;
	//��ʾ������ɫ
	virtual LPCTSTR				GetDropVisColor(VOID) const ;


	//������ϸ����
	virtual VOID			SetExtraInfo(const _ITEM * pItemInfo);
	virtual VOID			SetExtraInfo( CHAR* pstrIn ){};

	//��ϸ����(������Ҫ������)
	virtual LPCTSTR			GetExtraDesc(VOID);

	//INT	GetFecthHeadInfo_Type( INT Index );
	INT	GetFecthHeadInfo_Value( INT Index );

	LPCTSTR GetFecthHeadInfo_TypeDesc( INT Index );

	//�õ��������������
	INT	GetFecthHeadInfo_Num(){ return m_soulbead.m_nAttrCount; };


public:
	CObject_Item_SoulBead(INT id);
	virtual ~CObject_Item_SoulBead();
	//������չ����
	VOID					AsSoulBead(const _DBC_ITEM_SOULBEAD* pMedicDefine);

protected:
	//--------------------------------------------------------------
	//��������
	const _DBC_ITEM_SOULBEAD*	m_theBaseDef;			//�������Զ���(���ݱ�)
	SOUL_BEAD_INFO				m_soulbead;			//��������

	std::map<ITEM_ATTRIBUTE, std::string> m_soulbeadTypeName;		//�������������

	STRING					m_strExtraDesc;
};

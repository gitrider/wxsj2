
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
|	��չ��������Ʒ
|
*/
class CObject;
struct PET_GUID_t;
class CObject_Item_ExtraBag : public CObject_Item
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

	// �õ���Ʒ����ļ۸�
	virtual INT				GetItemBasePrice();

	// �õ�������
	virtual INT				GetItemPrice();

	//�õ���Чʱ��
	INT		GetEffectTime();

	//�õ���Ч������
	INT		GetEffectGridNum();

	// �õ���Ʒ���������� 
	virtual LPCTSTR					GetItemTableTypeDesc();

	//�õ�����ʱ��ģ��id
	virtual LPCTSTR				GetDropVisualID(VOID)	const;
	//��ʾ������ɫ
	virtual LPCTSTR				GetDropVisColor(VOID) const ;


	//������ϸ����
	virtual VOID			SetExtraInfo(const _ITEM * pItemInfo);
	virtual VOID			SetExtraInfo( CHAR* pstrIn ){};

	//��ϸ����(������Ҫ������)
	virtual LPCTSTR			GetExtraDesc(VOID);


public:
	CObject_Item_ExtraBag(INT id);
	virtual ~CObject_Item_ExtraBag();
	//������չ����
	VOID					AsExtraBag(const _DBC_ITEM_EXTRABAG* pMedicDefine);

protected:
	//--------------------------------------------------------------
	//��������
	const _DBC_ITEM_EXTRABAG*	m_theBaseDef;			//�������Զ���(���ݱ�)

	STRING					m_strExtraDesc;
};

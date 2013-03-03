
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
|	ҩƿ����Ʒ
|
*/
class CObject;
struct PET_GUID_t;
class CObject_Item_Task : public CObject_Item
{

 
 
public:
	//�õ���Ʒ�����
	virtual ITEM_CLASS		GetItemClass(VOID) const ;
	// �õ�type��Ϣ
	virtual INT				GetItemTableType(VOID);
	//�õ����еĶ���
	const _DBC_ITEM_TASK*	GetBaseDefine(void) const { return m_theBaseDef; }
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
	//�õ��Ƿ���Թ㲥
     BOOL				    GetBroadcast(VOID)const ;

	// �õ���Ʒ����ļ۸�
	virtual INT				GetItemBasePrice();
   // �õ�������
	virtual INT				GetItemPrice();

	// �õ���Ʒ������ļ���ID
	INT						GetItemSkillID();
	//�õ���Ʒ��ѡ���������
	virtual INT				GetItemTargetType();
	// �õ����ʹ�������Ʒ��Ҫ�ĵȼ�
	virtual INT				GetNeedLevel();

	//�Ƿ�������Χʹ����Ʒ
	bool					IsAreaTargetType(void) const;
	//�Ƿ�Ŀ������Ǿ���Ψһ��(�Լ����Լ��ĳ���)
	bool					IsTargetOne(void) const;

	// �õ���Ʒ���������� 
	LPCTSTR	GetItemTableTypeDesc();

	//��������

	virtual LPCTSTR				GetDropVisualID(VOID)	const;

	virtual	LPCTSTR				GetDropVisColor(VOID)	const;

	//��ȡ��������
	virtual INT				GetLayNum(VOID) ;

public:
	// �õ�����Ʒ�ڱ��е�����
	INT GetTaskItemTableType();

public:
	CObject_Item_Task(INT id);
	virtual ~CObject_Item_Task();
	//����һ��������Ʒ
	VOID					AsTask(const _DBC_ITEM_TASK* pTaskDefine);

protected:
	//--------------------------------------------------------------
	//��������
	const _DBC_ITEM_TASK*	m_theBaseDef;			//�������Զ���(���ݱ�)
	INT	WriteVarAttr( CHAR* pIn );
	BOOL	ReadVarAttr( CHAR* pOut, INT& OutLength );

	STRING					m_strExtraDesc;
};

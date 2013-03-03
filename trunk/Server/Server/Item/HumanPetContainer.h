
#ifndef _HUMAN_PET_CONTAINER_H_
#define _HUMAN_PET_CONTAINER_H_
#include "ItemContainer.h"
#include "DB_Struct.h"

class HumanDB;
struct _PET_DB_LOAD;

struct _HUMAN_PETCONTAINER_INIT: 
	public _ITEMCONTAINER_INIT
{
	HumanDB*				m_pHumanDB;
	ITEM_CONTAINER_TYPE		m_ICTType;
};

class Obj_Human;

class HumanPetContainer:public ItemContainer
{
	HumanDB*				m_pHumanDB;
	ITEM_CONTAINER_TYPE		m_ICTType;
public:
	virtual		BOOL		Init( const _ITEMCONTAINER_INIT* pInit ) ;//��ʼ������

	virtual		INT			GetContainerType();
protected :
	/*
	*	��Ʒ ʹ�÷���
	*/
	//������Ʒ������
	virtual		BOOL		SetItem( const INT nIndex, const Item* pItem );
	//������Ʒ�ص�����
	virtual		BOOL		SetItemLayCount(INT nIndex, INT nCount) ;
	//������Ʒ�ص�����
	virtual		BOOL		DecItemLayCount(INT nIndex, INT nCount=1) ;
	//������Ʒ�ص�����
	virtual		BOOL		IncItemLayCount(INT nIndex, INT nCount=1);
	//ɾ����Ʒ
	virtual		BOOL		EraseItem(UINT uIndex);
	//������Ʒ�;ö�
	virtual	BOOL			SetItemDur(INT nIndex, INT nDur);
	//������Ʒ���˵�
	virtual	BOOL			SetItemDamagePoint(INT nIndex, INT nPoint);
	//������Ʒ����;ö�
	virtual	BOOL			SetItemMaxDur(INT nIndex, INT nDur);

	//������Ʒ�;ö�
	virtual		BOOL		SetItemCurMaxDur(INT nIndex, INT nDur);

	//������Ʒ����
	virtual		BOOL		SetItemValue(INT nIndex,_ITEM*	pItem);
	//������Ʒ�ﶨ
	virtual		BOOL		SetItemBind(INT nIndex);
	//������Ʒ������Ϣ
	virtual		BOOL		SetItemIdent(INT nIndex);
	//������������Ϣ
	virtual		BOOL		SetItemCreator(INT nIndex,const CHAR* CreatorName);
	//������Ʒ����
	virtual		BOOL		AddItemAttr(INT nIndex,_ITEM_ATTR iA);
	//ɾ����Ʒ����
	virtual		BOOL		DelItemAttr(INT nIndex,_ITEM_ATTR iA);
	//ɾ����ʯ��Ϣ
	virtual		BOOL		DelGemInfo(INT nIndex,INT GemIndex);
	//��ӱ�ʯ��Ϣ
	virtual		BOOL		AddGemInfo(INT nIndex,INT& GemIndex,UINT GemType);
protected :
	/*
	*	PET ʹ�÷���
	*/
	//������Ʒ����
	virtual	BOOL			SetItemValue(INT nIndex,const _PET_DB_LOAD* pPet);

	virtual BOOL  			SetPetGUID(INT nIndex,PET_GUID_t GUID) ;				//���ó���GUID
	virtual BOOL  			SetSpouseGUID(INT nIndex,PET_GUID_t GUID) ;				//���ó�����żGUID
	virtual BOOL			SetDataID(INT nIndex,INT ID);							//���ó���ģ��		
	virtual BOOL 			SetName(INT nIndex, const CHAR* pName);					//��������
	virtual BOOL   			SetNick(INT nIndex, const CHAR* pNick);					//�����ǳ�
	virtual BOOL  			SetLevel(INT nIndex,INT level);							//���õȼ�
	virtual BOOL  			SetTakeLevel(INT nIndex,INT takeLevel);					//����Я���ȼ�
	virtual BOOL  			SetAttackType(INT nIndex,INT attackType);				//���ý������ͣ���/����
	virtual BOOL  			SetAIType(INT nIndex,INT AIType);						//����AI����
	virtual BOOL  			SetCampData(INT nIndex,const _CAMP_DATA* pCamp);		//������Ӫ
	virtual BOOL  			SetGeneration(INT nIndex,BYTE Gen);						//������
	virtual BOOL  			SetHappiness(INT nIndex,BYTE byHappiness);				//���ֶ�		

	virtual BOOL  			SetStrengthPer(INT nIndex,INT strper);					//��������	
	virtual BOOL  			SetSmartnessPer(INT nIndex,INT conper);					//��������	
	virtual BOOL   			SetMindPer(INT nIndex,INT dexper);						//��������
	virtual BOOL   			SetConstitutionPer(INT nIndex,INT intper);				//��������

	virtual BOOL			SetGenGu(INT nIndex,INT gengu);							//����

	virtual BOOL  			SetGrowRate(INT nIndex,FLOAT rate);						//�ɳ���
	virtual BOOL  			SetRemainPoint(INT nIndex,INT rPoint);					//һ������ʣ�����
	virtual BOOL  			SetExp(INT nIndex,INT exp) ;							//����ֵ
	virtual BOOL  			SetLvl1Attr(INT nIndex,CHAR_ATTR_LEVEL1 type,INT value);//����һ��ս�����ԣ����������ܺ�װ�����ӵĲ��֣�
	virtual BOOL  			SetSkill(INT nIndex,UINT SkillIndex,_PET_SKILL skill);	//���＼��
	virtual BOOL			SetMatingFlag( INT iIndex, BOOL bFlag );				//������
	virtual BOOL			SetMatingStartTime( INT nIndex, UINT uTime );			//���俪ʼʱ��
	virtual BOOL			SetMatingFinishFlag( INT nIndex, BOOL bFlag );			//������ɱ��
	virtual BOOL			SetMatingLevel( INT nIndex, INT iLevel );				//���һ�ν���ɹ�ʱ�ĵȼ�
	virtual BOOL			SetPetSex( INT nIndex, INT iSex );						//�Ա�
	virtual BOOL			SetCheckUpFlag( INT nIndex, BOOL bCheckup );			//�Ƿ񾭹�����

private:
	BOOL					SetDBDirty(INT index);
};



#endif
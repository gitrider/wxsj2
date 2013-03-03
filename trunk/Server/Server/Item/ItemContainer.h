/////////////////////////////////////////////////////////////////////////////////
//�ļ�����ItemContainer.h
//������������Ʒ�����ķ�װ�࣬ͨ����������ȡ�����ö����Ʒ�������Ϣ
//�����ߣ�jack
//�޸ļ�¼��2005-11-2����
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __ITEMCONTAINER_H__
#define __ITEMCONTAINER_H__

#include "Type.h"
#include "Item.h"

#define MAX_ITEMCONTAINER_SIZE 185

struct _ITEMCONTAINER_INIT
{
	INT						m_nContainerSize ;//������С
	
};

class ItemContainer
{
friend class ItemOperator ;
	//ItemContainerType
	
public :
	ItemContainer( ) ;
	virtual ~ItemContainer( ) ;
	/*
	 *	PET �� ITEM ���÷���
	 */

	virtual BOOL		Init( const _ITEMCONTAINER_INIT* pInit ) ;//��ʼ������

	//������ŵ�Bag���
	virtual		UINT	ConIndex2BagIndex(UINT uConIndex){return uConIndex;}
	//������ŵ��������
	virtual		UINT	BagIndex2ConIndex(UINT uBagIndex){return uBagIndex;}

	//����һ��nIndex�ǲ��������Container��
	virtual		BOOL	IsInContainer(UINT uBagIndex);

	//���Ը������Ƿ����
	virtual		BOOL	IsCanUse();

	//�����Ƿ���Ч
	virtual		BOOL	IsValid();

	//��ȡ��������
	virtual	INT			GetContainerType();


	VOID				CleanUp( ) ;//��������ڵ�������Ʒ����

	//���������ո����
	virtual		UINT	CalcItemSpace();	

	//�ж������Ƿ��ǿ�
	virtual		BOOL	IsEmpty();

	//�����м��������ո����
	//����ʱ����Ʒ����������ס������ռ�ʱ���Ա���������ס����Ʒ��������Ʒ�Ϳ��Խ����ˡ�
	virtual		UINT	CalcExchangeItemSpace();	

	//ȡ��һ���յ���Ʒλ��, �������INVALID_INDEX��������
	virtual		INT		GetEmptyItemIndex( ) ;

	//ȡ��������С
	virtual		INT		GetContainerSize( ){ return m_nContainerSize ; }

	virtual		BOOL	SetItemPWLock(INT nIndex,BOOL bLock);

    //��չContainer ��С
    virtual BOOL        ExpandSize(const INT nSize);
    //����Container ��С
    virtual BOOL        SetSize(const INT nSize);

	//ͨ����Ʒ��GUIDȡ�ô���Ʒ�������е�λ��
	virtual	INT			GetIndexByGUID( const _ITEM_GUID* pGuid ) ;

	//ͨ����Ʒ������ȡ�ô���Ʒ�������е�λ��
	virtual	INT			GetIndexByType( const UINT uType ) ;

	//ͨ����Ʒ������ȡ�ô���Ʒ�������е�λ��
	virtual	INT			GetIndexByType( const UINT uType ,UINT emptyCount) ;

	//ͨ��������λ��ȡ����Ʒ
	virtual	Item*		GetItem( const INT nIndex ) ;

	//ȡ��һ��û������Ʒλ��,���INVALID_INDEX ���ʾû�ҵ�
	virtual	INT			GetNoFullItemIndex( UINT uItemIndex);

	virtual		INT		GetIndexByGUID( const PET_GUID_t* pGuid );

	//�ı�Container �Ĵ�С
	virtual		BOOL	ChangeSize(const INT nSize);

	virtual     BOOL	ResetSize(const INT nSize);
protected :
	/*
	*	ITEM ʹ�÷���
	*/
	//����Ʒ���õ���Ӧ����λ��
	virtual BOOL		SetItem( const INT nIndex, const Item* pItem );

	//������Ʒ�ص�����
	virtual BOOL		SetItemLayCount(INT nIndex, INT nCount) ;
	//������Ʒ�ص�����
	virtual BOOL		DecItemLayCount(INT nIndex, INT nCount=1) ;
	//������Ʒ�ص�����
	virtual BOOL		IncItemLayCount(INT nIndex, INT nCount=1);
	//ɾ����Ʒ
	virtual	BOOL		EraseItem(UINT uIndex);
	//������Ʒ�;ö�
	virtual	BOOL		SetItemDur(INT nIndex, INT nDur);
	//������Ʒ���˵�
	virtual	BOOL		SetItemDamagePoint(INT nIndex, INT nPoint);
	//������Ʒ�;ö�
	virtual	BOOL		SetItemMaxDur(INT nIndex, INT nDur);
	//������Ʒ�;ö�
	virtual	BOOL		SetItemCurMaxDur(INT nIndex, INT nDur);
	
	//������Ʒ����
	virtual	BOOL		SetItemValue(INT nIndex,_ITEM*	pItem);
	//������Ʒ�ﶨ
	virtual	BOOL		SetItemBind(INT nIndex);
	//������Ʒ������Ϣ
	virtual BOOL		SetItemIdent(INT nIndex);
	//������ƷGUID
	virtual BOOL		SetItemGuid(INT nIndex,_ITEM_GUID* pGUID);
	//������
	virtual	BOOL		SetItemCreator(INT nIndex,const CHAR* CreatorName);
	//��Ʒ����
	virtual BOOL		SetItemParam(INT nIndex,UINT start,INT type,INT value);

	//������Ʒ����
	virtual BOOL		AddItemAttr(INT nIndex,_ITEM_ATTR iA);
	//ɾ����Ʒ����
	virtual	BOOL		DelItemAttr(INT nIndex,_ITEM_ATTR iA);
	//ɾ����ʯ��Ϣ
	virtual BOOL		DelGemInfo(INT nIndex,INT GemIndex);
	//��ӱ�ʯ��Ϣ
	virtual	BOOL		AddGemInfo(INT nIndex, INT GemIndex,UINT GemHolePos);
	
protected :
	/*
	*	PET ʹ�÷���
	*/
	
	//������ƷGUID
	virtual		BOOL		SetItemGuid(INT nIndex,PET_GUID_t* pGUID);
	//����PET ����
	//������Ʒ����
	virtual	BOOL		SetItemValue(INT nIndex,const _PET_DB_LOAD*	pPet);

	virtual BOOL  		SetPetGUID(INT nIndex,PET_GUID_t GUID) ;				//���ó���GUID
	virtual BOOL  		SetSpouseGUID(INT nIndex,PET_GUID_t GUID) ;				//���ó�����żGUID
	virtual BOOL		SetDataID(INT nIndex,INT ID);							//���ó���ģ��		
	virtual BOOL 		SetName(INT nIndex,const CHAR* pName);					//��������
	virtual BOOL   		SetNick(INT nIndex,const CHAR* pNick);					//�����ǳ�
	virtual BOOL  		SetLevel(INT nIndex,INT level);							//���õȼ�
	virtual BOOL  		SetTakeLevel(INT nIndex,INT takeLevel);					//����Я���ȼ�
	virtual BOOL  		SetAttackType(INT nIndex,INT attackType);				//���ý������ͣ���/����
	virtual BOOL  		SetAIType(INT nIndex,INT AIType);						//����AI����
	virtual BOOL  		SetCampData(INT nIndex,const _CAMP_DATA* pCamp);		//������Ӫ
	virtual BOOL  		SetPetType(INT nIndex,BYTE PetType);					//���������죬Ұ��
	virtual BOOL  		SetGeneration(INT nIndex,BYTE Gen);						//������
	virtual BOOL  		SetHappiness(INT nIndex,BYTE byHappiness);				//���ֶ�		
	virtual BOOL  		SetStrengthPer(INT nIndex,INT strper);					//��������	
	virtual BOOL  		SetSmartnessPer(INT nIndex,INT conper);					//��������	
	virtual BOOL   		SetMindPer(INT nIndex,INT dexper);						//��������
	virtual BOOL   		SetConstitutionPer(INT nIndex,INT intper);				//��������
	virtual BOOL		SetGenGu(INT nIndex,INT gengu);							//����
	virtual BOOL		SetSavvy(INT nIndex,INT iSavvy);						//����
	virtual BOOL  		SetGrowRate(INT nIndex,FLOAT rate);						//�ɳ���
	virtual BOOL  		SetRemainPoint(INT nIndex,INT rPoint);					//һ������ʣ�����
	virtual BOOL  		SetExp(INT nIndex,INT exp) ;							//����ֵ
	virtual BOOL  		SetLvl1Attr(INT nIndex,CHAR_ATTR_LEVEL1 type,INT value);//����һ��ս�����ԣ����������ܺ�װ�����ӵĲ��֣�
	virtual BOOL  		SetSkill(INT nIndex,UINT SkillIndex,_PET_SKILL skill);	//���＼��
	virtual BOOL		SetMatingFlag( INT iIndex, BOOL bFlag );				//�Ƿ��ڽ���״̬
	virtual BOOL		SetMatingStartTime( INT nIndex, UINT uTime );			//���俪ʼʱ��
	virtual BOOL		SetMatingFinishFlag( INT nIndex, BOOL bFlag );			//������ɱ��
	virtual BOOL		SetMatingLevel( INT nIndex, INT iLevel );				//���һ�ν���ɹ�ʱ�ĵȼ�
	virtual BOOL		SetPetSex( INT nIndex, INT iSex );						//�Ա�
	virtual BOOL		SetCheckUpFlag( INT nIndex, BOOL bCheckup );			//�Ƿ񾭹�����
	
	INT					m_nContainerSize ;					//������С
private :
	
	Item				m_aItem[MAX_ITEMCONTAINER_SIZE] ;	//��������Ʒ�ռ�
	
};

#endif

/////////////////////////////////////////////////////////////////////////////////
//�ļ�����ItemOperator.h
//������������Ʒ����Ʒ�����Ĳ�����װ�࣬ͨ������������������������Ʒ��ת�ƣ�����
//			���ɡ��Լ�������Ʒ���ݵȹ��ܡ�
//�����ߣ�jack
//�޸ļ�¼��2005-11-2����
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __ITEMOPERATOR_H__
#define __ITEMOPERATOR_H__

#include "Type.h"
#include "ItemContainer.h"
#include "DB_Struct.h"


enum ITEM_OPERATOR_ERROR//��ö�ٽṹ�ڵ����ݱ��붼Ϊ����
{
	ITEMOE_SUCCESS				=  0,	//�ɹ�
	ITEMOE_UNKNOW				= -1 ,	//δ֪���쳣����
	ITEMOE_DESTOPERATOR_HASITEM = -2 ,  //������Ŀ��λ���Ѿ�����Ʒ��
	ITEMOE_SOUROPERATOR_LOCK	= -3 ,	//Դ�����е���Ʒ��������
	ITEMOE_DESTOPERATOR_LOCK	= -4 ,	//Ŀ�������е���Ʒ��������
	ITEMOE_DESTOPERATOR_FULL	= -5 ,	//Ŀ�������Ѿ�����
	ITEMOE_SOUROPERATOR_EMPTY	= -6 ,	//Դ���������Ʒ�ǿյ�
	ITEMOE_DESTOPERATOR_EMPTY   = -7 ,	//Ŀ����������Ʒ�ǿյ�
	ITEMOE_CREATEITEM_FAIL		= -8,	//��Ʒ����ʧ��
	ITEMOE_CREATEGUID_FAIL		= -9,	//��ƷGUID����ʧ��
	ITEMOE_DIFF_ITEM_DATA		= -10,	//��ͬ������Ʒ���ݲ��ܲ���
	ITEMOE_DESTOPERATOR_FAIL	= -11,	//Ŀ��������Ʒ��������
	ITEMOE_DESTOPUSE_FAIL		= -12,  //Ŀ������ʧЧ
	ITEMOE_SOUROPUSE_FAIL		= -13,  //Դ����ʧЧ
	ITEMOE_DESTOPERATOR_INVALID = -14,	//Ŀ��λ����Ч
	ITEMOE_SOUROPERATOR_INVALID = -15,	//Դλ����Ч
};


struct _ITEMOPERATOR_INIT
{

};

struct ITEM_LOG_PARAM;

struct ItemBoxContaner;

class ItemOperator
{
public :
	ItemOperator( ) ;
	~ItemOperator( ) ;

	BOOL				Init( _ITEMOPERATOR_INIT* pInit ) ;//��ʼ����Ʒ����ϵͳ

	VOID				CleanUp( ) ;//���

public :
	//////////////////////////////
	//��������Ʒ������Ĳ����ӿ�//
	//////////////////////////////



	/*
	 *	������pDestContainer�д���һ����Ʒ,��Ʒ���͸���uItemIndex,uQuality ���涨
	 *  uQuality 
	 *  ����Ѿ��д�����Ʒ����е���,������Ʒ��λ�÷��ڿ�λ����
	 *  
	 *	uConIndex �����Ʒ���õ�λ��
	 *
	 *	�˲�������һ������Ψһ(GUID) ������Ʒ
	 */

	BOOL				CreateItem(ITEM_LOG_PARAM*	pLogParam,
								   UINT uItemIndex,
								   UINT	uQuality,
								   ItemContainer*	pDestContainer,
								   BOOL&			bNewEmptyPos,
								   UINT&			uConIndex,			
								   INT nDestIndex = INVALID_INDEX);


	/*
	 *	��Ʒ�����г�ʼ��ItemBox ��һ�ַ�ʽ
	 *	��pSourceContainer �� nSourceIndex λ�õ���Ʒ
	 *  copy �� pDestContainer �� nDestIndex λ��
	 *	���nDestIndex δָ����Ѱ��һ���ո�,����δ���ĸ�
	 */

	INT					CopyItem(ItemBoxContaner*	pSourceContainer,
								 INT				nSourceIndex,
								 BOOL&				bNewPos,
								 ItemContainer*		pDestContainer,
								 INT	nDestIndex	=INVALID_INDEX);
								

	


	//��pSourContainer�����ڵ�nSourIndexλ�õ���Ʒת�Ƶ�pDestContainer�����ڵ�
	//nDestIndexλ��
	//���nDestIndex��ֵΪINVALID_INDEX������Ʒת�Ƶ�pDestContainer�����ڵ���
	//���ҵ���һ����λ����
	//����ֵ��ִ�гɹ����򷵻���Ʒ��pDestContainer�����ڵ�λ��
	//		  ִ��ʧ�ܣ��򷵻�ITEM_OPERATOR_ERROR��������ֵ
	//ע�⣺�˲������������µ���Ʒ����
	INT					MoveItem( ItemContainer* pSourContainer,
							      INT			 nSourIndex ,
								  BOOL&			 bNewPos,
								  ItemContainer* pDestContainer,
								  INT			 nDestIndex=INVALID_INDEX ) ;

	//��pSourContainer�����ڵ�nSourIndexλ�õ���Ʒת�Ƶ�pSourContainer�����ڵ�
	//nDestIndexλ��
	//����ֵ��ִ�гɹ����򷵻���Ʒ��pSourContainer�����ڵ�λ��
	//		  ִ��ʧ�ܣ��򷵻�ITEM_OPERATOR_ERROR��������ֵ
	//ע�⣺�˲������������µ���Ʒ����
	//��Ӧ��дLog �����ڵ��ƶ�
	INT					MoveItem( ItemContainer* pSourContainer,
								  INT nSourIndex,
								  INT nDestIndex);



	
	
	

	//�����Ʒ����
	//ע��:�˲������ܲ����µ�GUID
	INT					SplitItem(ITEM_LOG_PARAM* pLogParam,
								  ItemContainer* pSourContainer,	//ԭ����
								  UCHAR uSourIndex,					//ԭ������λ��
								  UCHAR uCount,						//�������
								  ItemContainer* pDestContainer,	//Ŀ������
                                  INT nDestIndex =INVALID_INDEX );  //Ŀ��λ��

	//�ϲ���Ʒ����
	//ע��:�˲����������µ�GUID
	INT					SpliceItem(	ItemContainer* pSourContainer,	//ԭ����
									UCHAR uSourIndex,				//ԭ������λ��
									ItemContainer* pDestContainer,	//Ŀ������
									INT nDestIndex  );				//Ŀ��λ��

	
	//�ϲ���Ʒ����
	//ע��:�˲����������µ�GUID
	INT					MoveSpliceItem(ItemContainer* pSourContainer,	//ԭ����
									UCHAR uSourIndex,					//ԭ������λ��
									ItemContainer* pDestContainer,		//Ŀ������
									INT nDestIndex);					//Ŀ��λ��




	


public :
	////////////////
	//������Ʒ�ӿ�//
	////////////////

	
	//������Ʒ�ص�����
	BOOL	SetItemLayCount(ItemContainer* pContainer, INT nIndex, INT nCount) ;
	//������Ʒ�ص�����
	BOOL	DecItemLayCount(ItemContainer* pContainer,INT nIndex, INT nCount=1) ;
	//������Ʒ�ص�����
	BOOL	IncItemLayCount(ItemContainer* pContainer,INT nIndex, INT nCount=1);
	//������Ʒ�;ö�
	BOOL	SetItemDur(ItemContainer*	pContainer,INT nIndex, INT nDur);
	//������Ʒ��������
	BOOL	SetItemIdent(ItemContainer*	pContainer,INT nIndex);
	//������Ʒ���˶�
    BOOL	SetItemDamagePoint(ItemContainer*	pContainer,INT nIndex, INT nPoint);
	//������Ʒ����;ö�
	BOOL	SetItemMaxDur(ItemContainer*	pContainer,INT nIndex, INT nDur);
	//������Ʒ����;ö�
	BOOL	SetItemCurMaxDur(ItemContainer*	pContainer,INT nIndex, INT nDur);
	
	//������Ʒ������
	BOOL	SetItemBind(ItemContainer*	pContainer,INT nIndex);
	
	//������Ʒ�Ƿ������뱣��
	BOOL	SetItemPWLock(ItemContainer*	pContainer,INT nIndex, BOOL bLock);
	//������Ʒ��������Ϣ
	BOOL	SetItemCreator(ItemContainer* pContainer,INT nIndex,const CHAR* CreatorName);
	//������Ʒ����
	BOOL	SetItemParam(ItemContainer* pContainer,INT nIndex,UINT start,INT type,INT value);

	//������Ʒ����
	BOOL			AddItemAttr(ItemContainer*	pContainer,INT nIndex,_ITEM_ATTR	iA);
	//ɾ����Ʒ����
	BOOL			DelItemAttr(ItemContainer*	pContainer,INT nIndex,_ITEM_ATTR	iA);
	//ɾ����ʯ��Ϣ
	BOOL			DelGemInfo(ItemContainer* pContainer,INT nIndex,INT GemIndex);
	//��ӱ�ʯ��Ϣ
	BOOL			AddGemInfo(ItemContainer* pContainer,INT nIndex,INT GemIndex,UINT GemHolePos);

	////////////////
	//��������ӿ�//
	////////////////

	BOOL						SetItemValue(ItemContainer*	pContainer,INT nIndex,const _PET_DB_LOAD*	pPet);
	BOOL  						SetPetGUID(ItemContainer*	pContainer,INT nIndex,PET_GUID_t GUID) ;				//���ó���GUID
	BOOL  						SetSpouseGUID(ItemContainer*	pContainer,INT nIndex,PET_GUID_t GUID) ;			//���ó�����żGUID
	BOOL						SetDataID(ItemContainer*	pContainer,INT nIndex,INT ID);							//���ó���ģ��		
	BOOL 						SetName(ItemContainer*	pContainer,INT nIndex,const CHAR* pName);					//��������
	BOOL   						SetNick(ItemContainer*	pContainer,INT nIndex,const CHAR* pNick);					//�����ǳ�
	BOOL  						SetLevel(ItemContainer*	pContainer,INT nIndex,INT level);							//���õȼ�
	BOOL  						SetTakeLevel(ItemContainer*	pContainer,INT nIndex,INT takeLevel);					//����Я���ȼ�
	BOOL  						SetAttackType(ItemContainer*	pContainer,INT nIndex,INT attackType);				//���ý������ͣ���/����
	BOOL  						SetAIType(ItemContainer*	pContainer,INT nIndex,INT AIType);						//����AI����
	BOOL  						SetCampData(ItemContainer*	pContainer,INT nIndex,const _CAMP_DATA* pCamp);			//������Ӫ
	BOOL  						SetHP(ItemContainer*	pContainer,INT nIndex,INT hp);								//��������ֵ
	BOOL  						SetLife(ItemContainer*	pContainer,INT nIndex,INT Life);							//���õ�ǰ����
	BOOL  						SetPetType(ItemContainer*	pContainer,INT nIndex,BYTE PetType);					//���������죬Ұ��
	BOOL  						SetGeneration(ItemContainer*	pContainer,INT nIndex,BYTE Gen);					//������
	BOOL  						SetHappiness(ItemContainer*	pContainer,INT nIndex,BYTE byHappiness);				//���ֶ�		
	BOOL  						SetStrPer(ItemContainer*	pContainer,INT nIndex,INT strper);						//��������	
	BOOL  						SetSmartnessPer(ItemContainer*	pContainer,INT nIndex,INT conper);					//��������	
	BOOL   						SetMindPer(ItemContainer*	pContainer,INT nIndex,INT dexper);						//��������
	BOOL   						SetConstitutionPer(ItemContainer*	pContainer,INT nIndex,INT intper);				//��������
	BOOL						SetGenGu(ItemContainer*	pContainer,INT nIndex,INT gengu);							//����
	BOOL  						SetGrowRate(ItemContainer*	pContainer,INT nIndex,FLOAT rate);						//�ɳ���
	BOOL  						SetRemainPoint(ItemContainer*	pContainer,INT nIndex,INT rPoint);					//һ������ʣ�����
	BOOL  						SetExp(ItemContainer*	pContainer,INT nIndex,INT exp) ;							//����ֵ
	BOOL  						SetLvl1Attr(ItemContainer*	pContainer,INT nIndex,CHAR_ATTR_LEVEL1 type,INT value);	//����һ��ս�����ԣ����������ܺ�װ�����ӵĲ��֣�
	BOOL  						SetSkill(ItemContainer*	pContainer,INT nIndex,UINT SkillIndex,_PET_SKILL skill);	//���＼��
	BOOL  						SetSavvy(ItemContainer*	pContainer,INT nIndex,INT iSavvy);							//��������
	BOOL						SetMatingFlag( ItemContainer* pContainer,INT nIndex, BOOL bFlag );
	BOOL						SetMatingStartTime( ItemContainer* pContainer,INT nIndex, UINT uTime );
	BOOL						SetMatingFinishFlag( ItemContainer* pContainer,INT nIndex, BOOL bFlag );
	BOOL						SetMatingLevel( ItemContainer* pContainer,INT nIndex, INT iLevel );
	BOOL						SetPetSex( ItemContainer* pContainer,INT nIndex, INT iSex );
	BOOL						SetCheckUpFlag(  ItemContainer* pContainer,INT nIndex, BOOL bCheckup );
public:
	/*
	 *	ITEM �� PET ���������ӿ�
	 */
	//��������pContainer��nIndexλ�õ���Ʒ
	VOID	LockItem(ItemContainer* pContainer, INT nIndex ) ;
	//��������pContainer��nIndexλ�õ���Ʒ
	VOID	UnlockItem(ItemContainer* pContainer, INT nIndex ) ;
	

	//������pDestContainer��nDestIndexλ��������һ����Ʒ������Ʒ��pTemplateItem����
	//��ͬ���߼����ݼ����ԣ���GUID��ͬ��
	//���nDestIndex��ֵΪINVALID_INDEX������Ʒ���ɵ�pDestContainer����������ҵ�
	//��һ����λ����
	//����ֵ��ִ�гɹ����򷵻���Ʒ��pDestContainer�����ڵ�λ��
	//		  ִ��ʧ�ܣ��򷵻�ITEM_OPERATOR_ERROR��������ֵ
	//
	//ע�⣺�˲���������һ������Ψһ��ʶ��GUID��������Ʒ
	//
	INT					CreateItem( ITEM_LOG_PARAM*	pLogParam,
									Item* pTemplateItem ,
									ItemContainer* pDestContainer,
									INT nDestIndex=INVALID_INDEX ) ;


	//������pDestContainer����һ����Ʒ������Ʒ����pTemplateItem�߼����ݼ�����
	//��Ʒ���ɵ�pDestContainer����������ҵ�
	//��һ����λ����
	//uConIndex �����Ʒ���õ�λ��
	//ע�⣺�˲�������һ������Ψһ(GUID) ��pTemplateItem��Ʒ
	BOOL				CreateItem( ITEM_LOG_PARAM* pLogParam,
									UINT uQuality,
									ItemContainer* pDestContainer,
									UINT&	uConIndex,
									_CREATEITEM_INIT* pInit=NULL);

	//ɾ����Ʒ����
	//ɾ�� pDestContainer ����uIndexλ�õ���Ʒ
	//ע��:�˲���������Ŀ��������uIndex λ�õ���Ʒ
	BOOL				EraseItem(	ItemContainer* pDestContainer, 
									UCHAR uIndex );
	

	//��pSourContainer�����ڵ�nSourIndexλ�õ���Ʒ��pDestContainer�����ڵ�
	//nDestIndexλ�õ���Ʒ���н�������
	//����ֵ��ִ�гɹ����򷵻���Ʒ��pDestContainer�����ڵ�λ��
	//		  ִ��ʧ�ܣ��򷵻�ITEM_OPERATOR_ERROR��������ֵ
	//ע�⣺�˲������������µ���Ʒ����
	INT					ExchangeItem( ItemContainer* pSourContainer,
									  INT nSourIndex ,
									  ItemContainer* pDestContainer,
									  INT nDestIndex ) ;
	

	//��pSourContainer�����ڵ�nSourIndexλ�õ���Ʒ���Ƶ�pDestContainer�����ڵ�
	//nDestIndexλ��
	//���nDestIndex��ֵΪINVALID_INDEX������Ʒ���Ƶ�pDestContainer�����ڵ���
	//���ҵ���һ����λ����
	//����ֵ��ִ�гɹ����򷵻���Ʒ��pDestContainer�����ڵ�λ��
	//		  ִ��ʧ�ܣ��򷵻�ITEM_OPERATOR_ERROR��������ֵ
	//
	//ע�⣺�˲����Ḵ�Ƴ�һ����Ʒ,�¾���Ʒ����ͬ��GUID
	//
	INT					CopyItem( ItemContainer* pSourContainer,
								  INT			 nSourIndex ,
								  ItemContainer* pDestContainer,
								  INT nDestIndex = INVALID_INDEX ) ;


	//��ָ����Ʒ�ƶ���ָ������nDestIndexλ��
	//���nDestIndex��ֵΪINVALID_INDEX������Ʒת�Ƶ�pDestContainer�����ڵ���
	//���ҵ���һ����λ����
	INT					MoveItem(ItemContainer* pDestContainer, 
								 _ITEM* pSourItem, 
								 INT nDestIndex = INVALID_INDEX );


	//��pSourContainer�����ڵ�nSourIndexλ�õ���Ʒת�Ƶ�pDestContainer�����ڵ�
	//nDestIndexλ��
	//���nDestIndex��ֵΪINVALID_INDEX������Ʒת�Ƶ�pDestContainer�����ڵ���
	//���ҵ���һ����λ����
	//����ֵ��ִ�гɹ����򷵻���Ʒ��pDestContainer�����ڵ�λ��
	//		  ִ��ʧ�ܣ��򷵻�ITEM_OPERATOR_ERROR��������ֵ
	//ע�⣺�˲������������µ���Ʒ����
	INT					MoveItem( ItemContainer* pSourContainer,
								  INT nSourIndex ,
								  ItemContainer* pDestContainer,
								  INT nDestIndex=INVALID_INDEX ) ;

	
	INT					MoveExtraBagItem( ItemContainer* pSourContainer,
										  INT nSourIndex ,
										  INT nDestIndex=INVALID_INDEX ) ;
public:

	/*
	 *	PET ר�õĲ����ӿ�
	 */
	
	

protected :
	//�ڲ���������

	//������Ʒ��GUID
	VOID			SetItemGUID( ItemContainer* pContainer, INT nIndex, _ITEM_GUID* pGuid ) ;
	VOID			SetItemGUID( ItemContainer* pContainer, INT nIndex, PET_GUID_t* pGuid ) ;
	
	
	BOOL			GenItemGUID(_ITEM_GUID& ig) ;//����һ���µ�GUID
	BOOL			GenItemGUID(PET_GUID_t& ig) ;//����һ���µ�GUID


	
};

extern ItemOperator	g_ItemOperator;





#endif

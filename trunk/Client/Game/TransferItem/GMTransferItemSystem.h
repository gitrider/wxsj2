/****************************************\
*										*
* 			  ��Ʒ��Ϣ����ϵͳ			*
*										*
*						 by fangyuan		*
\****************************************/

#pragma once
#include "GITransferItemSystem.h"
#include "../DataPool/GMDP_Struct_Pet.h"

struct SDATA_PET;
class tObject_Item;

class ItemElement : public tTransferItemSystem::Element
{
public:
	ItemElement();
	virtual ~ItemElement(){}

	tObject_Item*	pData;
public:
	virtual STRING		GetDisplayColor();
	virtual STRING		GetDisplayPretxt();
public:
	//��ѯTooltip����Ʒ����ɫ����
	//
	//
	//   
	// ��������1��
	//   0[str] - "": �����Ʒû���������
	//   
	//
	STRING		GetItemExtBlueAttrInfo();

	// 
	// �õ���ɫװ���Ƿ����
	// 
	INT	IsBlueEquipIdentify();
};

class HyperLinkElement : public tTransferItemSystem::Element
{
public:
	virtual STRING		GetDisplayColor();
	virtual STRING		GetDisplayPretxt(){return "";}

	HyperLinkElement();
	virtual ~HyperLinkElement(){}
public:
	INT		m_nChannelType;				//��Ӧ��Ƶ������
};

class ChannelElement : public tTransferItemSystem::Element
{
public:
	virtual STRING		GetDisplayColor(){return "";}
	virtual STRING		GetDisplayPretxt(){return "";}

	ChannelElement();
	virtual ~ChannelElement(){}
public:
	INT		m_nChannelType;				//��Ӧ��Ƶ������
};

class PetElement : public tTransferItemSystem::Element
{
public:
	virtual STRING		GetDisplayColor();
	virtual STRING		GetDisplayPretxt();

	PetElement();
	virtual ~PetElement(){}
public:
	SDATA_PET		m_Pet;
};

class BossElement : public tTransferItemSystem::Element
{
public:
	virtual STRING		GetDisplayColor(){return "";}
	virtual STRING		GetDisplayPretxt(){return "";}

	BossElement();
	virtual ~BossElement(){}
public:
	INT		m_nBossID;					//��Ӧ��BOSS��ID
};

class CTransferItemSystem : public tTransferItemSystem
{
	/******************************************
		���ܽӿ�
	*******************************************/
public:
	//��һ����Ʒת��Ϊ�ַ���
	virtual Element*		ConvertFromItem(tObject_Item* pItem);
	//��һ������ת��Ϊ����
	virtual Element*		ConvertFromPet(INT nIdx);
	//���ַ���ת��Ϊ��Ʒ
	virtual Element*		ConvertFromString(const STRING& strIn, const STRING& strContents);
	
	//���һ��HyperLink
	virtual Element*		AddHyperLink(const STRING& strName, const STRING& strContex, INT nCh = -1/*CHAT_TYPE_INVALID*/);
	//���һ��Ƶ��
	virtual Element*		AddChannel(INT nType, const STRING& strContex);
	//����һ�����ݽṹ
	virtual void			DeleteElement(int nID);

	//����Element
	virtual void			DoElement(int nID, int Action,int msgID = -1);
	//�ж�Element�Ƿ����
	virtual BOOL			IsElementExist(int nID);
	//���Element������
	virtual int				GetElementType(int nID);
	//���Element����ʾ����
	virtual STRING			GetElementDisplayName(int nID);
	//���Element��ԭʼ����
	virtual STRING			GetOrgElementDisplayName(int nID);
	//���Element������
	virtual STRING			GetElementContents(int nID);
	//����Element��ʹ�ü���
	virtual VOID			AddElementUseCount(int nID);
	//��õ�ǰѡ�е�Element
	virtual Element*		GetCurElement(){return m_pCurElement;}
protected:
	
	//���ִ�ת��Ϊ��Ʒ
	Element*				ConvertItemFromString(const STRING& strIn, const STRING& strContents);

	//���ִ�ת��Ϊ����
	Element*				ConvertPetFromString(const STRING& strIn, const STRING& strContents);

	//���ִ�ת��ΪBOSS������Ϣ
	Element*				ConvertBossFromString(const STRING& strIn, const STRING& strContents);

	/******************************************
		�ڲ�����
	*******************************************/
private:
	typedef std::map< int, Element* > ElementMap;

	ElementMap				m_mapElements;	//��������

	int						m_nIdFactory;

	Element*				m_pCurElement;	//��ǰѡ�е�Element
	/******************************************
	�ڲ�����
	*******************************************/
	Element*				GetElement(int nID);

	STRING					GetDisplayColour(Element& el);
	STRING					GetDisplayPrefix(Element& el);

	// ����Ʒϵͳ����޸�[12/7/2006]
	static BOOL				GenItemBinStr( tObject_Item* pItem, STRING& strContents);
	static BOOL				GenBinStrItem(tObject_Item*& pItem, const STRING& strContents);
	/******************************************
		���ļ̳�
	*******************************************/
public:
	virtual VOID	Initial(VOID*);
	virtual VOID	Release(VOID);
	virtual VOID	ReSet(VOID);
public:
	CTransferItemSystem();
	virtual ~CTransferItemSystem();
	static CTransferItemSystem* GetMe(VOID) { return s_pMe; }

protected:
	static CTransferItemSystem*		s_pMe;

	KL_DECLARE_DYNAMIC(CTransferItemSystem);
};

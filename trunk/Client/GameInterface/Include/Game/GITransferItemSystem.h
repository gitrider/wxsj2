#pragma once
#include "GINode.h"
/*
|
|	��Ʒ��Ϣ����ϵͳ,����Ʒת��Ϊ������Ϣͨ������������
|
*/
enum ElementType
{
	ELEMENT_INVALID = -1,

	ELEMENT_ITEMINFO,			//��Ʒ��Ϣ
	ELEMENT_HYPERLINK,			//����
	ELEMENT_CHANNEL,			//Ƶ���л�
	ELEMENT_PET,				//����
	ELEMENT_BOSS,				//BOSS
};

enum ElementAction
{
	ELEMENT_R_CLICK,			//�Ҽ����
	ELEMENT_L_CLICK,			//������
	ELEMENT_MOVE_IN,			//����ƽ�
	ELEMENT_MOVE_OUT,			//����Ƴ�
};

class tObject_Item;
class KYLIN_API tTransferItemSystem : public tNode
{
public:
	//�������ݽṹ
	struct Element
	{
		int nID;			//ID
		int nType;			//����

		int nObjID;				//��ƷID
		STRING strDisplay;		//��ʾ����

		STRING strOrgDisplay;	//û�о����޸ĵ�ԭʼ�ִ�
		STRING strContents;		//���ڴ�����ַ���

		int useCount;		//ʹ�ü�����������0��ʱ�����Elementɾ��

		Element():nType(ELEMENT_INVALID){}
		virtual ~Element(){}
		BOOL				IsType(int nT){return (nType == nT)?TRUE:FALSE;}

		// ʹ��_DBC_SYSTEM_COLOR�е�����
		virtual STRING	GetDisplayColor() = 0;
		virtual STRING	GetDisplayPretxt() = 0;
	};

public:
	//��һ����Ʒת��Ϊ�ַ���
	virtual Element*		ConvertFromItem(tObject_Item* pItem) = 0;
	//��һ������ת��Ϊ����
	virtual Element*		ConvertFromPet(INT nIdx) = 0;
	//���ַ���ת��Ϊ��Ʒ
	virtual Element*		ConvertFromString(const STRING& strDisplay, const STRING& strContents) = 0;

	//���һ��HyperLink
	virtual Element*		AddHyperLink(const STRING& strName, const STRING& strContex,INT nCh = -1/*CHAT_TYPE_INVALID*/) = 0;
	//���һ��Ƶ��
	virtual Element*		AddChannel(INT nType, const STRING& strContex) = 0;

	//����һ�����ݽṹ
	virtual void			DeleteElement(int nID) = 0;
	//����Element
	virtual void			DoElement(int nID, int Action,int msgID=-1) = 0;
	//�ж�Element�Ƿ����
	virtual BOOL			IsElementExist(int nID) = 0;
	//���Element������
	virtual int				GetElementType(int nID) = 0;
	//���Element����ʾ����
	virtual STRING			GetElementDisplayName(int nID) = 0;
	//���Element��ԭʼ����
	virtual STRING			GetOrgElementDisplayName(int nID) = 0;
	//���Element������
	virtual STRING			GetElementContents(int nID) = 0;
	//����Element��ʹ�ü���
	virtual VOID			AddElementUseCount(int nID)	= 0;
	//��õ�ǰѡ�е�Element
	virtual Element*		GetCurElement() = 0;

	virtual VOID	ReSet() = 0;
	KL_DECLARE_DYNAMIC(tTransferItemSystem);
};
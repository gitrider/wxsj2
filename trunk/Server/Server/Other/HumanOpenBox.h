#ifndef _OPENBOXCONTAINER_H_
#define _OPENBOXCONTAINER_H_
#include "Type.h"
#include "ItemContainer.h"
#include "HumanOpenBox.h"
#include "HumanItemLogic.h"
#include "ItemOperator.h"

template< int OPEN_BOX_SIZE >			//��������Ʒ�б�Ĵ�С
class OpenBoxContainer
{
public:
	enum 
	{
		CENTER_ITEM_INDEX = 0,			//������Ʒ�������е�����
		RAND_ITEM_INDEX = 1,			//�����Ʒ������������
	};

public:
	OpenBoxContainer();

	~OpenBoxContainer();

public:
	VOID				Init();
	VOID				CleanUp();



public:
	BOOL				GetState(VOID) const { return m_IsOpen; };
	VOID				SetState( BOOL bState ) { m_IsOpen = bState; };
	ItemContainer*		GetContainer( VOID ) { return  &m_Container; };
public:
	ItemContainer		m_Container;
	_ITEM				m_ItemList[OPEN_BOX_SIZE];
private:
	BOOL				m_IsOpen;		//�����ǲ��Ǳ���
};


#include "HumanOpenBox.cpp"

typedef OpenBoxContainer< OPENBOX_CONTAINER_SIZE > HumanOpenBox;
typedef OpenBoxContainer< TMP_OPENBOX_CONTAINER_SIZE > TmpOpenBoxContainer;


#endif
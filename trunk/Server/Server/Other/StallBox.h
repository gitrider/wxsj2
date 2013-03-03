#ifndef _STALL_BOX_H_
#define _STALL_BOX_H_

#include "Type.h"
#include "ItemContainer.h"
#include "BBS.h"
class Obj_Human;
class StallInfoManager;
struct _ITEM;
/*
��������̯λ�нṹ.
*/
class ServerStallBox
{
public:
	enum STALL_STATUS
	{
		STALL_CLOSE,
		STALL_READY,
		STALL_OPEN,
	};
public:
	ServerStallBox()
	{
		m_pMySelf = NULL;
	}
	~ServerStallBox()
	{
	}

public:
	/*
	��ʼ���Լ�̯λ��
	*/
	VOID Init(Obj_Human* pHuman);

	/*
	���
	*/
	VOID CleanUp();

	/*
	̯���Լ�,һ����ʼ���겻���ٸ�
	*/
	Obj_Human*	GetMySelf(){return m_pMySelf;}

	/*
	��������
	*/
	VOID	UnLockAllItem();

	/*
	λ���ͷ�
	*/
	VOID	UnLockMapPos();
public:

	BYTE	GetStallStatus(){ return m_StallStatus; }
	VOID	SetStallStatus(BYTE StallStatus){m_StallStatus = StallStatus;}

	BOOL	GetStallIsOpen(){ return m_IsOpen; }
	VOID	SetStallIsOpen(BOOL IsOpen){m_IsOpen = IsOpen;}

	// ���û�ȡ���̯λ���б��		20100706 BLL
	BOOL	GetStallOpenPublic(){ return m_bOpenPublic; }
	VOID	SetStallOpenPublic(BOOL bOpenPublic){m_bOpenPublic = bOpenPublic;}

	UINT	GetPosTax(){ return m_PosTax; }
	VOID	SetPosTax(UINT PosTax){ m_PosTax = PosTax; }

	UINT	GetTradeTax(){ return m_TradeTax; }
	VOID	SetTradeTax(UINT TradeTax){ m_TradeTax = TradeTax; }

	CHAR*	GetStallName(){ return m_szStallName; }
	VOID	SetStallName(const CHAR *pszTitle, BYTE nSize );	

	UINT	GetFirstPage(){return m_nFirstPage;}
	VOID	SetFirstPage(UINT nFirstPage){m_nFirstPage = nFirstPage;}

	//��Ʒ��Ϣ����
	UINT	GetSerialByIndex(UINT index)
	{
		if(index>=0 && index<STALL_BOX_SIZE)
			return m_ItemSerial[index];
		else
			return 0;
	}
	VOID	SetSerialByIndex(UINT index, UINT nSerial)
	{
		if(index>=0 && index<STALL_BOX_SIZE)
			m_ItemSerial[index] = nSerial;
	}
	VOID	IncSerialByIndex(UINT index)
	{
		if(index>=0 && index<STALL_BOX_SIZE)
			m_ItemSerial[index] += m_SerialIncStep;
	}

	VOID	SetSerialIncStep(UINT SerialIncStep) {m_SerialIncStep = SerialIncStep;}
	UINT	GetSerialIncStep() {return m_SerialIncStep;}

	UINT	GetPriceByIndex(UINT index)
	{
		if(index>=0 && index<STALL_BOX_SIZE)
			return m_ItemPrice[index];
		else
			return 0;
	}
	VOID	SetPriceByIndex(UINT index, UINT nPrice)
	{
		if(index>=0 && index<STALL_BOX_SIZE)
			m_ItemPrice[index] = nPrice;
	}

	//������Ϣ����
	UINT	GetPetSerialByIndex(UINT index)
	{
		if(index>=0 && index<STALL_PET_BOX_SIZE)
			return m_PetSerial[index];
		else
			return 0;
	}

	VOID	SetPetSerialByIndex(UINT index, UINT nSerial)
	{
		if(index>=0 && index<STALL_PET_BOX_SIZE)
			m_PetSerial[index] = nSerial;
	}

	VOID	IncPetSerialByIndex(UINT index)
	{
		if(index>=0 && index<STALL_PET_BOX_SIZE)
			m_PetSerial[index] += m_SerialIncStep;
	}

	UINT	GetPetPriceByIndex(UINT index)
	{
		if(index>=0 && index<STALL_PET_BOX_SIZE)
			return m_PetPrice[index];
		else
			return 0;
	}

	VOID	SetPetPriceByIndex(UINT index, UINT nPrice)
	{
		if(index>=0 && index<STALL_PET_BOX_SIZE)
			m_PetPrice[index] = nPrice;
	}

	VOID	SetStallPos(UINT Pos_x, UINT Pos_z)
	{
		m_PosX	=	Pos_x;	
		m_PosZ	=	Pos_z;	
	}

	ItemContainer*	GetContainer() {return &m_Container;}

	ItemContainer*	GetPetContainer() {return &m_PetContainer;}

	ServerBBS*		GetBBS()	{return &m_StallBBS;}
private:
	Obj_Human*		m_pMySelf;				// ָ���Լ���ָ�룬����������ҵ��Լ����ϵĶ���
	BOOL			m_IsOpen;				// ̯λ�Ƿ��Ѿ���
	BYTE			m_StallStatus;			// ��ǰ̯λ�Ƿ��ڱ���
	BOOL			m_bOpenPublic;			// ̯λ�Ƿ���(������ʾ̯λ�Ƹ��������)	20100706 BLL
	UINT			m_PosTax;				// ̯λ��
	UINT			m_TradeTax;				// ÿ�ν���˰��
	CHAR			m_szStallName[MAX_STALL_NAME];			//��ʾ̯λ��
	UINT			m_nFirstPage;				//��ʼҳ
	UINT			m_ItemSerial[STALL_BOX_SIZE];//ÿ����Ʒ�����к�
	UINT			m_ItemPrice[STALL_BOX_SIZE];//ÿ����Ʒ�ļ۸�

	ItemContainer	m_Container;					//CONTAINER
	_ITEM			m_ItemList[STALL_BOX_SIZE];		//��Ʒ����ʵ����

	UINT			m_PetSerial[STALL_PET_BOX_SIZE];//ÿ����������к�
	UINT			m_PetPrice[STALL_PET_BOX_SIZE];	//ÿ������ļ۸�

	ItemContainer	m_PetContainer;						//̯λ���еĳ�������
	_PET_DB_LOAD	m_PetItemList[STALL_PET_BOX_SIZE];	//�������ʵ����

	UINT			m_SerialIncStep;		//���кŵ�������		
	ServerBBS		m_StallBBS;				//̯λ���԰�

	StallInfoManager* m_pStallInfoMgr;			//̯λ��Ϣ������
	UINT			m_PosX;					//̯λ��λ�ü�¼����,��CleanUpʹ��
	UINT			m_PosZ;					//̯λ��λ�ü�¼����,��CleanUpʹ��

};

#endif
#ifndef _DB_CHARCREATE_H_
#define _DB_CHARCREATE_H_

#include "Type.h"
#include "ODBCBase.h"

class DBCharCreate:public ODBCBase
{
	UINT			m_AID;
	CHAR			m_Account[MAX_ACCOUNT+1];		//�û�����
	GUID_t			m_CharGuid;
	CHAR			m_CharName[MAX_CHARACTER_NAME];
	BOOL			m_Sex;
	BYTE			m_HairColor;					//�½�ɫͷ����ɫ	
	BYTE			m_FaceColor;					//�½�ɫ������ɫ
	BYTE			m_HairModel;					//�½�ɫͷ��ģ��
	BYTE			m_FaceModel;					//�½�ɫ����ģ��
	INT				m_DefEquip;						//�½�ɫװ��
	BYTE			m_HeadID;						//ͷ�����
	BYTE			m_Country;					//����
public:
	DBCharCreate(ODBCInterface* pInterface);

	virtual BOOL	Load();
	virtual BOOL	AddNew();
	virtual BOOL	ParseResult(VOID* pResult);

public:
	const	CHAR*	GetAccount()	const;
	VOID			SetAccount(const CHAR*	pAccount);

	VOID			SetCharName(const CHAR* pName);
	const CHAR*		GetCharName();
	
	VOID			SetSex(BOOL bSex);
	BOOL			GetSex();

	VOID			SetHairColor(BYTE Color);
	BYTE			GetHairColor();

	VOID			SetFaceColor(BYTE Color);
	BYTE			GetFaceColor();

	VOID			SetHairModel(BYTE Model);
	BYTE			GetHairModel();

	VOID			SetFaceModel(BYTE Model);
	BYTE			GetFaceModel();


	VOID			SetHeadID(BYTE Head);
	BYTE			GetHeadID();

	VOID			SetDefEquip(INT defEquip);
	INT				GetDefEquip();

	VOID			SetCountry(INT Country);
	INT				GetCountry();

	GUID_t			GetCharGuid();				
};


#endif
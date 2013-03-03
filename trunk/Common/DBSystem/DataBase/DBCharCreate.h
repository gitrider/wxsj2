#ifndef _DB_CHARCREATE_H_
#define _DB_CHARCREATE_H_

#include "Type.h"
#include "ODBCBase.h"

class DBCharCreate:public ODBCBase
{
	UINT			m_AID;
	CHAR			m_Account[MAX_ACCOUNT+1];		//用户名称
	GUID_t			m_CharGuid;
	CHAR			m_CharName[MAX_CHARACTER_NAME];
	BOOL			m_Sex;
	BYTE			m_HairColor;					//新角色头发颜色	
	BYTE			m_FaceColor;					//新角色脸形颜色
	BYTE			m_HairModel;					//新角色头发模型
	BYTE			m_FaceModel;					//新角色脸形模型
	INT				m_DefEquip;						//新角色装备
	BYTE			m_HeadID;						//头部编号
	BYTE			m_Country;					//国家
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
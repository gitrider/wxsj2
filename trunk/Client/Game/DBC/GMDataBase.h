
/** GMDataBase.h
 *	
 *	���ܣ�	���ݿ�ϵͳ�� �ͻ���������ȡ���ݱ�
 *	
 *	�޸ļ�¼��	
 *			
 *			
 */

#pragma once


#include "GIDataBase.h"
#include "GIDBC_Struct.h"


//----------------------------------------------------------
//���ݿ�
class CDataBase : public tDataBase
{
public:

	//������������
	virtual const DBC_FIELD*	Search_Index_EQU(INT iIndexValue) const;
	//������
	virtual const DBC_FIELD*	Search_LineNum_EQU(INT iLineNum) const;
	//����ĳ�е���ָ��ֵ�ĵ�һ��
	virtual const DBC_FIELD*	Search_First_Column_Equ(INT nColumnNum, const DBC_FIELD& value) const;

public:

	virtual	const DBC::DBCFile*	GetDBCFile( VOID ) const		{ return m_pFileDBC; }
	//ȡ��ID
	virtual INT					GetID(VOID)const;
	//ȡ������
	virtual UINT				GetFieldsNum(VOID)const;
	//ȡ�ü�¼������
	virtual UINT				GetRecordsNum(VOID)const;
	//ֱ�Ӵ�һ��dbc�ļ�
	static BOOL					Util_OpenDBCFromTxt(LPCTSTR szFileName, DBC::DBCFile* pDBCFile);

	// �и��ַ��������á�|���ָ���ַ���˳���ֳ����������������ַ���
	// ������(��Ҫ�������ַ����� �ڼ������ݣ� ������ַ���)
	static BOOL					GetSplitData( const CHAR* szData, UINT nCount, CHAR* szOutStr );

protected:
	// �����ַ����� ָ���ַ��� ��ĳ��λ��, ����Ϊ�ַ����±�
	static INT					GetCharOff( const CHAR* szStr, const CHAR ch, UINT nCount);


public:
	CDataBase(INT id);
	virtual ~CDataBase();

protected:

	//���ı��ļ�������һ�����ݿ�
	BOOL					OpenFromTXT(LPCTSTR szFileName);

protected:
	DBC::DBCFile*			m_pFileDBC;

	friend class CDataBaseSystem;
};

//----------------------------------------------------------
//���ݿ������
class CDataBaseSystem : public tDataBaseSystem
{
public:
	//��ϵͳ���������ݿ�
	virtual VOID				OpenAllDataBase(VOID);
	//�ر�ϵͳ���������ݿ�
	virtual VOID				CloseAllDataBase(VOID);
	//�õ�һ���ѽ��򿪵����ݿ⣬�������ݿ�ָ��
	virtual const tDataBase*	GetDataBase(INT idDataBase) const;

public:
	virtual VOID				Initial(VOID*);
	virtual VOID				Release(VOID);

protected:

protected:
	//���ݿ�
	std::map< INT, CDataBase* >	m_mapDataBaseBuf;

public:
	CDataBaseSystem();
	virtual ~CDataBaseSystem();
	static CDataBaseSystem* GetMe(VOID) { return s_pMe; }

protected:
	static CDataBaseSystem* s_pMe;

	KL_DECLARE_DYNAMIC(CDataBaseSystem);
};



//---------------------------------------------------

//�������ݿ��ݷ�ʽ
#define DBC_DEFINEHANDLE(nameDBC, idDBC) \
	const static tDataBase* nameDBC = NULL; \
	if(!nameDBC) nameDBC = CGameProcedure::s_pDataBaseSystem->GetDataBase(idDBC); \
	KLAssert(nameDBC);


//ת��title�ַ���
LPCTSTR TransTitleStr(LPCTSTR szTitle);
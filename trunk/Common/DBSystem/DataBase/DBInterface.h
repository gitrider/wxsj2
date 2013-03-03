#ifndef		_DB_INTERFACE_H_
#define		_DB_INTERFACE_H_
#include	"Type.h"
#include	"DBTypes.h"

class Connection;
class Statement;


//���ݲ�ѯ���
/*
 *	���ݲ�ѯ�����
 *  
 *		 field 0 field 1 .....	field n
 *
 *	Row 0
 *	Row 1
 *  |
 *	|
 *  |
 *  Row n
 *
 */



class SqlResult
{

	friend class Statement;

public:
	SqlResult();
	~SqlResult();
	

	BOOL			Next();		//ȡ��һ������

	CHAR*			GetField(UINT	fieldIndex); //ȡField n ��Buffer

	CHAR			GetChar(UINT    fieldIndex)  //ȡCHAR Field 
	{
		return(GetField(fieldIndex))[0];
	}
	INT				GetInt(UINT fieldIndex)      //ȡINT Field
	{
		return	atoi(GetField(fieldIndex));
	}
	UINT			GetUInt(UINT fieldIndex)	//ȡ UINT Field 
	{
		return(UINT)atoi(GetField(fieldIndex));
	}
	UCHAR			GetBYTE(UINT fieldIndex)	//ȡBYTE Field
	{
		return(UCHAR)atoi(GetField(fieldIndex));
	}
	WORD			GetWORD(UINT fieldIndex)	//ȡWORD Field
	{
		return(WORD)atoi(GetField(fieldIndex));
	}
	SHORT			GetShort(UINT fieldIndex)	//ȡShort Field
	{
		return (SHORT)atoi(GetField(fieldIndex));
	}
	const CHAR*		GetString(UINT fieldIndex); //ȡCHAR* Field

	UINT			GetRowCount() const			//ȡRow ����
	{
		return m_RowCount;
	}
	UINT			GetFieldCount() const		//ȡField ����
	{
		return m_FieldCount;
	}
	VOID			FreeResult();				//�ͷŵ�ǰResult �Ļ�����
	
private:
	VOID*			m_pVarData;
	UINT			m_RowCount;
	UINT			m_FieldCount;
};


//Sql ״̬,����һ���ض���SQL
class Statement
{

private:

public:
	Statement();
	~Statement();
	

	//��ν������ѯ
	SqlResult *				ExecuteQuery(CHAR *fmt,...);


	 //ָ��SQL ���Ľ����ѯ
	SqlResult *				ExecuteQuery(const CHAR* sqlStatement);


	SqlResult*				ExecuteQuery();					//ִ��Ĭ�ϲ�ѯ

	 //ȡ��ǰStatement �İ�����
	Connection *			GetConnection() const;

	VOID					SetConnection(Connection * pConnection)
	{
		m_pConnection = pConnection;
	}
	
	//���ش���
	const	CHAR*			GetError() const;
	
	//ȡ�ò�ѯִ�к�Ӱ�������
	UINT					GetAffectedRowCount() const;

	DB_QUERY&				GetDBQuery();

private:

	Connection*				m_pConnection;					//���ݿ�����
	SqlResult				m_Result;						//��ѯ���
	UINT					m_nAffectedRows;				//Ӱ������
	DB_QUERY				m_Query;						//��ѯ�ṹ��
	
};


//
// ���ݿ�������
//
class Connection
{
public:
	
	Connection();
	//�����Ӳ����Ĺ���
	Connection( const CHAR* host,
				const CHAR* db,
				const CHAR* user,
				const CHAR* password,
				const UINT port);

	~Connection();
	
	//�رյ�ǰ����
	VOID					Close();

	
	VOID					Connect(const CHAR* host,
									const CHAR* db,
									const CHAR* user,
									const CHAR* password,
									UINT		port );

	VOID					Connect() ;
	BOOL					IsConnected()		const;
	Statement *				GetDefStatement();
	const CHAR*				GetHost() const
	{
		return m_Host;
	}
	UINT					GetPort() const
	{
		return m_Port;
	}
	const CHAR*				GetDatabase() const
	{
		return m_Database;
	}
	const CHAR*				GetUser() const
	{
		return m_User;
	}
	const CHAR*				GetPassword() const
	{
		return m_Password;
	}
	
	const CHAR*				GetName() const
	{
		return m_Name;
	}
	VOID					SetName(const	CHAR* strName)
	{
		strncpy(m_Name,strName,CONNECTION_NAME_LEN);
	}
	BOOL					IsBusy(void) const
	{
		return m_bBusy;
	}
	VOID					SetBusy(bool busy=true)
	{
		m_bBusy = busy;
	}
	const CHAR*				GetError();
	DBHandle				GetHandle();
private:
	BOOL					m_bConnected;					//�Ѿ������ϵ�״̬���
	CHAR					m_Host[HOST_STR_LEN];			//���ӶԶ�IP
	UINT					m_Port;							//���ӶԶ˶˿�
	CHAR					m_Database[DATABASE_STR_LEN];	//���ݿ�����
	CHAR					m_User[DB_USE_STR_LEN];			//�û�����
	CHAR					m_Password[DB_PASSWORD_STR_LEN];//����
	CHAR					m_Name[CONNECTION_NAME_LEN];	//��������
	BOOL					m_bBusy;						//�Ƿ�æ
	Statement				m_Statement;					//Ĭ��Sql״̬
	DBHandle				m_Handle;						//���ݿ���


};



#endif
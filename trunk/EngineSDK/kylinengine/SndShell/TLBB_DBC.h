/*
|==========================================
|	DBC���ݿ��ļ���
|		(������/�ͻ���ͨ��)
|==========================================
|
|		--------------------
|		|  ���ݿ��ļ���ʽ  |
|		--------------------
|
|		Offset |	Type  |  Description  
|		-------+----------+------------------
|	Head
|		0X000	  UINT		DBC File Identity�� always 0XDDBBCC00
|		0X004	  UINT      Number of records in the file 
|		0X008     UINT      Number of 4-byte fields per record
|		0X010     UINT      String block size 
|   FieldType
|		0X014     UINT[FieldNum]   
|							  The type fo fields(0-INT, 1-FLOAT, 2-string)
|   FieldBlock
|				  UINT[FieldNum*RecordNum]
|							  DataBlock
|	StringBlock
|				  CHAR[StringSize]
|							  StringBlock
|
*/
#ifndef _TLBB_DB_H_
#define _TLBB_DB_H_

#include <vector>

#include <windows.h>
#include <hash_map>
#include <string>

//#include "Type.h"
namespace DBC
{
    using namespace std;

	class DBCFile	
	{
	public:
		//�ļ�ͷ
		struct FILE_HEAD
		{
			UINT		m_Identify;				//��ʾ	0XDDBBCC00
			INT			m_nFieldsNum;			//����
			INT			m_nRecordsNum;			//����
			INT			m_nStringBlockSize;		//�ַ�������С
		};

		//�ֶ���������
		enum FIELD_TYPE
		{
			T_INT		= 0,	//����
			T_FLOAT		= 1,	//������
			T_STRING	= 2,	//�ַ���
		};

		//���ݿ��ʽ����
		typedef vector< FIELD_TYPE >	FILEDS_TYPE;

		//���ݶ�
		union FIELD
		{
			FLOAT		fValue;
			INT			iValue;
			const CHAR*	pString;	// Just for runtime!

			//Construct
			FIELD() {}
			FIELD(INT value) { iValue = value; }
			FIELD(FLOAT value) { fValue = value; }
			FIELD(const CHAR* value) { pString = value; }
		};
		//������
		typedef vector< FIELD >		DATA_BUF;

	public:
		//���ı��ļ�������һ�����ݿ�
		BOOL					OpenFromTXT(const CHAR* szFileName, INT nColumToRead=0);
		//�����ڴ��е��ļ���
		BOOL					OpenFromMemory(const CHAR* pMemory, const CHAR* pDeadEnd, const CHAR* szFileName=0, INT nColumToRead=0);
	protected:
		//��ȡ�ı���ʽ����
		BOOL					OpenFromMemoryImpl_Text(const CHAR* pMemory, const CHAR* pDeadEnd, const CHAR* szFileName=0, INT nColumToRead=0);
		//��ȡ�����Ƹ�ʽ����
		BOOL					OpenFromMemoryImpl_Binary(const CHAR* pMemory, const CHAR* pDeadEnd, const CHAR* szFileName=0, INT nColumToRead=0);

	public:
		//����������(��һ��Ϊ����)
		virtual const FIELD*	Search_Index_EQU(INT nValue) const;
		//����λ�ò���
		virtual const FIELD*	Search_Posistion(INT nRecordLine, INT nColumNum) const;
		//����ĳ�е���ָ��ֵ�ĵ�һ��
		virtual const FIELD*	Search_First_Column_Equ(INT nColumnNum, const FIELD& value) const;

	public:
		//ȡ��ID
		UINT GetID(VOID) const				{ return m_ID; }
		//ȡ������
		INT	GetFieldsNum(VOID) const	    { return m_nFieldsNum; }
		//ȡ�ü�¼������
		INT GetRecordsNum(VOID) const		{ return m_nRecordsNum; }
		//����������
		VOID CreateIndex(INT nColum = 0, const CHAR* szFileName=0);

	protected:
		typedef stdext::hash_map< INT, FIELD*>	FIELD_HASHMAP;		  
		
		//���ݿ��ʽ�ļ���
		UINT			m_ID;
		//���ݿ��ʽ����
		FILEDS_TYPE				m_theType;
		//����
		INT						m_nRecordsNum;
		//����
		INT						m_nFieldsNum;
		//������
		DATA_BUF				m_vDataBuf;		//size = m_nRecordsNum*m_nFieldsNum
		//�ַ�����
		CHAR*					m_pStringBuf;
		//�ַ�������С
		INT						m_nStringBufSize;
		//������
		FIELD_HASHMAP			m_hashIndex;
		//������
		INT						m_nIndexColum;

	public:

		static INT			_ConvertStringToVector(const CHAR* strStrINTgSource, vector< std::string >& vRet, const CHAR* szKey, BOOL bOneOfKey, BOOL bIgnoreEmpty);
		//���ڴ����ַ�����ȡһ���ı�(���ջ��з�)
		static const CHAR*	_GetLineFromMemory(CHAR* pStringBuf, INT nBufSize, const CHAR* pMemory, const CHAR* pDeadEnd);
		//�Ƚ�����ֵ�Ƿ����
		template < FIELD_TYPE T>	
		static bool			_FieldEqu(const FIELD& a, const FIELD& b);

	public:
		DBCFile(UINT id);
		virtual ~DBCFile();
	};
}

#endif
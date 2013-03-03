#include "stdafx.h"
#include <string>
#include <map>
#include <assert.h>
#include "WXSJ_DBC.h"
#include <exception>

namespace DBC
{

INT	DBCFile::_ConvertStringToVector(const CHAR* strStrINTgSource, vector< std::string >& vRet, const CHAR* szKey, BOOL bOneOfKey, BOOL bIgnoreEmpty)
{
	vRet.clear();
	
	std::string strSrc = strStrINTgSource;
	if(strSrc.empty())
	{
		return 0;
	}

	std::string::size_type nLeft = 0;
	std::string::size_type nRight;
	if(bOneOfKey)
	{
		nRight = strSrc.find_first_of(szKey);
	}
	else
	{
		nRight = strSrc.find(szKey);
	}

	if(nRight == std::string::npos)
	{
		nRight = strSrc.length();
	}
	while(TRUE)
	{
		std::string strItem = strSrc.substr(nLeft, nRight-nLeft);
		if(strItem.length() > 0 || !bIgnoreEmpty)
		{
			vRet.push_back(strItem);
		}

		if(nRight == strSrc.length())
		{
			break;
		}

		nLeft = nRight + (bOneOfKey ? 1 : strlen(szKey));
		
		if(bOneOfKey)
		{
			std::string strTemp = strSrc.substr(nLeft);
			nRight = strTemp.find_first_of(szKey);
			if(nRight != std::string::npos) nRight += nLeft;
		}
		else
		{
			nRight = strSrc.find(szKey, nLeft);
		}

		if(nRight == std::string::npos)
		{
			nRight = strSrc.length();
		}
	}

	return (INT)vRet.size();
}

template < DBCFile::FIELD_TYPE T>
bool DBCFile::_FieldEqu(const FIELD& a, const FIELD& b)
{
	if(T == T_INT)
	{
		return a.iValue==b.iValue;
	}
	else if(T == T_FLOAT)
	{
		return a.fValue==b.fValue;
	}
	else
	{
		try
		{
			return (strcmp(a.pString, b.pString)==0);
		}
		catch(...)
		{
			return false;
		}
	}
}

DBCFile::DBCFile(UINT id)
{
	m_ID = id;
	m_pStringBuf = NULL;
	m_nIndexColum = -1;
}

DBCFile::~DBCFile()
{
	if(m_pStringBuf) SAFE_DELETE_ARRAY(m_pStringBuf);
	m_pStringBuf = NULL;
}

//���ڴ����ַ�����ȡһ���ı�(���ջ��з�)
const CHAR* DBCFile::_GetLineFromMemory(CHAR* pStringBuf, INT nBufSize, const CHAR* pMemory, const CHAR* pDeadEnd)
{
	//Ѱ����һ�����з�
	register const char* pMem = pMemory;
	if(pMem >= pDeadEnd || *pMem==0) return 0;

	while(pMem < pDeadEnd && pMem-pMemory+1<nBufSize && 
			*pMem != 0 && *pMem != '\r' && *pMem != '\n') *(pStringBuf++) = *(pMem++);
	//add 'null' end
	*pStringBuf = 0;

	//skip all next \r and \n
	while(pMem < pDeadEnd && *pMem != 0 && (*pMem=='\r' || *pMem=='\n')) pMem++;

	return pMem;
}

BOOL DBCFile::OpenFromMemory(const CHAR* pMemory, const CHAR* pDeadEnd, const CHAR* szFileName)
{
	assert(pMemory && pDeadEnd);
	//----------------------------------------------------
	//�ж��Ƿ��Ƕ����Ƹ�ʽ
	if(pDeadEnd - pMemory >=sizeof(FILE_HEAD) && *((UINT*)pMemory)==0XDDBBCC00)
	{
		return OpenFromMemoryImpl_Binary(pMemory, pDeadEnd, szFileName);
	}
	else
	{
		return OpenFromMemoryImpl_Text(pMemory, pDeadEnd, szFileName);
	}
}


//
INT Decrypt( const char* cSrc, char* cDest )
{
	if( 0 == strcmp( cSrc, "" ) )
		return 1;

	int h, l;
	int m, n;
	int j = 0;

	for( size_t i = 0; i < strlen(cSrc); i = i + 2 )
	{
		h = ( cSrc[ i ]		- 'x' );
		l = ( cSrc[ i + 1 ] - 'z' );
		
		m = ( h << 4);
		n = ( l & 0xf);

		cDest[j] = m + n;

		j++;
	}

	cDest[j]='\0';

	return 0;
} 


BOOL DBCFile::OpenFromMemoryImpl_Binary(const CHAR* pMemory, const CHAR* pDeadEnd, const CHAR* szFileName)
{
	register const char* pMem = pMemory;

	//----------------------------------------------------
	//Read Head
	FILE_HEAD theHead;
	memcpy(&theHead, pMem, sizeof(FILE_HEAD));
	if(theHead.m_Identify != 0XDDBBCC00 )
	{
		return FALSE;
	}
	//check memory size
	if(	sizeof(FILE_HEAD) + 
		sizeof(UINT)*theHead.m_nFieldsNum + 
		sizeof(FIELD)*theHead.m_nRecordsNum * theHead.m_nFieldsNum +
		theHead.m_nStringBlockSize > (DWORD)(pDeadEnd-pMemory))
	{
		return FALSE;
	}

	pMem += sizeof(FILE_HEAD);

	//----------------------------------------------------
	//Init 
	m_nRecordsNum = theHead.m_nRecordsNum;
	m_nFieldsNum  = theHead.m_nFieldsNum;
	m_nStringBufSize = theHead.m_nStringBlockSize;

	//---------------------------------------------
	//Create String Blok
	m_pStringBuf = new CHAR[theHead.m_nStringBlockSize];
	if(!m_pStringBuf) return FALSE;

	//------------------------------------------------------
	// Read Field Types
	vector< UINT > vFieldType;
	vFieldType.resize(theHead.m_nFieldsNum);
	memcpy(&(vFieldType[0]), pMem, sizeof(UINT)*theHead.m_nFieldsNum);
	pMem += sizeof(UINT)*theHead.m_nFieldsNum;

	//Check it!
	m_theType.resize(theHead.m_nFieldsNum);
	for(INT i=0; i<(INT)theHead.m_nFieldsNum; i++)
	{
		switch(vFieldType[i])
		{
		case T_INT:
		case T_FLOAT:
		case T_STRING:
			m_theType[i] = (FIELD_TYPE)vFieldType[i];
			break;

		default:
			SAFE_DELETE_ARRAY(m_pStringBuf);
			return FALSE;
		}
	}

	//------------------------------------------------------
	// Read All Field
	m_vDataBuf.resize(theHead.m_nRecordsNum * theHead.m_nFieldsNum);
	memcpy(&(m_vDataBuf[0]), pMem, sizeof(FIELD)*theHead.m_nRecordsNum * theHead.m_nFieldsNum);
	pMem += sizeof(FIELD)*theHead.m_nRecordsNum * theHead.m_nFieldsNum;

	//------------------------------------------------------
	// Read String Block
	memcpy( m_pStringBuf, pMem, m_nStringBufSize );
	m_pStringBuf[ m_nStringBufSize - 1 ] = 0;



	//------------------------------------------------------
	// TO runtime address
	for( INT j = 0; j < (INT)theHead.m_nFieldsNum; j++ )
	{
		if( vFieldType[j] != T_STRING ) 
			continue;

		string str;
		INT nFieldsNum;
		for( INT i = 0; i < (INT)theHead.m_nRecordsNum; i++ )
		{
			nFieldsNum = GetFieldsNum();

			m_vDataBuf[ i * nFieldsNum + j ].pString += reinterpret_cast<unsigned long long>( m_pStringBuf );

//			str = m_vDataBuf[ i * nFieldsNum + j ].pString;
//			Decrypt( str.c_str(), const_cast< char* >( m_vDataBuf[ i * nFieldsNum + j ].pString ) );
		}
	}


	//------------------------------------------------------
	//����������
	CreateIndex(0, szFileName);

	return TRUE;
}

BOOL DBCFile::OpenFromMemoryImpl_Text(const CHAR* pMemory, const CHAR* pDeadEnd, const CHAR* szFileName)
{
	//----------------------------------------------------
	//��������������
	CHAR szLine[1024*10] = {0};
	//����һ��
	register const char* pMem = pMemory;
	pMem = _GetLineFromMemory(szLine, 1024*10, pMem, pDeadEnd);
	if(!pMem) return FALSE;

	//�ֽ�
	vector< std::string > vRet;
	_ConvertStringToVector(szLine, vRet, "\t", TRUE, TRUE);
	if(vRet.empty()) return FALSE;
	//����Field Types
	FILEDS_TYPE vFieldsType;
	vFieldsType.resize(vRet.size());

	for(INT i=0; i<(INT)vRet.size(); i++) 
	{
		if(vRet[i] == "INT") vFieldsType[i] = T_INT;
		else if(vRet[i] == "FLOAT") vFieldsType[i] = T_FLOAT;
		else if(vRet[i] == "STRING") vFieldsType[i] = T_STRING;
		else
		{
			return FALSE;
		}
	}

	//--------------------------------------------------------------
	//��ʼ��
	INT nRecordsNum	= 0;
	INT nFieldsNum	= (INT)vFieldsType.size();

	//��ʱ�ַ�����
	vector< std::pair< std::string, INT > >	vStringBuf;
	//������
	std::map< std::string, INT >					mapStringBuf;

	//--------------------------------------------------------------
	//��ʼ��ȡ

	//�ն�һ��
	pMem = _GetLineFromMemory(szLine, 1024*10, pMem, pDeadEnd);
	if(!pMem) return FALSE;

	INT nStringBufSize = 0;
	do
	{
		//��ȡһ��
		pMem = _GetLineFromMemory(szLine, 1024*10, pMem, pDeadEnd);
		if(!pMem) break;;

		//�Ƿ���ע����
		if(szLine[0] == '#') continue;

		//�ֽ�
		_ConvertStringToVector(szLine, vRet, "\t", TRUE, FALSE);

		//��������
		if(vRet.empty()) continue;
        if(vRet.size() != nFieldsNum) 
		{
			//���Ͽո�
			if((INT)vRet.size() < nFieldsNum)
			{
				INT nSubNum = nFieldsNum-(INT)vRet.size();
				for(INT i=0; i<nSubNum; i++)
				{
					vRet.push_back("");
				}
			}
		}

		//��һ�в���Ϊ��
		if(vRet[0].empty()) continue;

		for(register INT i=0; i<nFieldsNum; i++)
		{
			FIELD newField;
			switch(vFieldsType[i])
			{
			case T_INT:
				newField.iValue = atoi(vRet[i].c_str());
				m_vDataBuf.push_back(newField);
				break;

			case T_FLOAT:
				newField.fValue = (FLOAT)atof(vRet[i].c_str());
				m_vDataBuf.push_back(newField);
				break;

			case T_STRING:
				if(vRet[i].empty())
				{
					newField.iValue = 0;
				}
				else
				{
					const CHAR * p = vRet[i].c_str();
					std::map< std::string, INT >::iterator it = mapStringBuf.find(vRet[i]);
					if(it == mapStringBuf.end())
					{
						vStringBuf.push_back(std::make_pair(vRet[i], nStringBufSize));
						mapStringBuf.insert(std::make_pair(vRet[i], (INT)vStringBuf.size()-1));
						newField.iValue = nStringBufSize + 1; // first CHAR is '\0' for blank string
	
						nStringBufSize += (INT)strlen(vRet[i].c_str()) + 1;
					}
					else
					{
						newField.iValue = vStringBuf[it->second].second + 1;
					}
				}

				m_vDataBuf.push_back(newField);
				break;
			}
		}

		nRecordsNum++;
	}while(TRUE);

	//--------------------------------------------------------
	//������ʽ���ݿ�
	m_nRecordsNum = nRecordsNum;
	m_nFieldsNum  = nFieldsNum;
	m_nStringBufSize = nStringBufSize+1;

	//Create String Blok
	m_pStringBuf = new CHAR[m_nStringBufSize];
	if(!m_pStringBuf) return FALSE;

	//------------------------------------------------------
	// Create Field Types
	m_theType = vFieldsType;

	//------------------------------------------------------
	// Create String Block
	UCHAR byBlank = '\0';
	m_pStringBuf[0] = '\0';

	register CHAR* p = m_pStringBuf + 1;
	for(INT i=0; i<(INT)vStringBuf.size(); i++)
	{
		memcpy(p, vStringBuf[i].first.c_str(), vStringBuf[i].first.size());	
		p += vStringBuf[i].first.size();

		*(p++) = '\0';
	}

	//------------------------------------------------------
	// Relocate String Block
	for(register INT i=0; i<nFieldsNum; i++)
	{
		if(vFieldsType[i] != T_STRING) continue;

		for(register INT j=0; j<nRecordsNum; j++)
		{
			FIELD& theField = m_vDataBuf[j*nFieldsNum+i];
			theField.pString = m_pStringBuf + theField.iValue;
		}
	}

	//------------------------------------------------------
	//����������
	CreateIndex(0, szFileName);

	return TRUE;
}

BOOL DBCFile::OpenFromTXT(const CHAR* szFileName)
{
	assert(szFileName);

	//----------------------------------------------------
	//���ļ�
	FILE* fp = fopen(szFileName, "rb");
	if(NULL == fp) return FALSE;

	fseek(fp, 0, SEEK_END);
	int nFileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	//�����ڴ�
	char* pMemory = new char[nFileSize+1];
	fread(pMemory, 1, nFileSize, fp);
	pMemory[nFileSize] = 0;

	BOOL bRet = OpenFromMemory(pMemory, pMemory+nFileSize+1, szFileName);

	SAFE_DELETE_ARRAY(pMemory); pMemory = 0;
	return bRet;
}

VOID DBCFile::CreateIndex(INT nColum, const CHAR* szFileName)
{
	if(nColum <0 || nColum >= m_nFieldsNum || m_nIndexColum==nColum) return;

	m_hashIndex.clear();

	for(INT i=0; i<(INT)m_nRecordsNum; i++)
	{
		FIELD* p = &(m_vDataBuf[i*m_nFieldsNum]);

		FIELD_HASHMAP::iterator itFind = m_hashIndex.find(p->iValue);
		if(itFind != m_hashIndex.end())
		{
			CHAR szTemp[260];
			tsnprintf(szTemp, 260, "[%s]Multi index at line %d(SAME:value=%d)", szFileName, i+1, p->iValue);
#ifdef GAME_CLIENT
			throw std::string(szTemp);
#else
			AssertEx(FALSE,szTemp);
#endif
			return;
		}
		m_hashIndex.insert(std::make_pair(p->iValue, p));
	}
}

const DBCFile::FIELD* DBCFile::Search_Index_EQU(INT iIndex) const
{
	FIELD_HASHMAP::const_iterator itFind = m_hashIndex.find(iIndex);
	if(itFind == m_hashIndex.end()) return NULL;

	return itFind->second;
}

const DBCFile::FIELD* DBCFile::Search_Posistion(INT nRecordLine, INT nColumNum) const
{
	INT nPosition = nRecordLine*GetFieldsNum() + nColumNum;

	if(nPosition <0 || nColumNum >= (INT)m_vDataBuf.size()) 
	{
		char szTemp[260];
		tsnprintf(szTemp, MAX_FILE_PATH, "DBC:Invalid search request[rec=%d, col=%d]", nRecordLine, nColumNum);
#ifdef GAME_CLIENT
		throw std::string(szTemp);
#else
		AssertEx(FALSE, szTemp);
#endif
		return NULL;
	}

	return &(m_vDataBuf[nPosition]);
}

//����ĳ�е���ָ��ֵ�ĵ�һ��
const DBCFile::FIELD* DBCFile::Search_First_Column_Equ(INT nColumnNum, const FIELD& value) const
{
	if(nColumnNum < 0 || nColumnNum >= m_nFieldsNum) return 0;
	FIELD_TYPE type = m_theType[nColumnNum];

	for(register int i=0; i<m_nRecordsNum; i++)
	{
		const FIELD& theField = m_vDataBuf[m_nFieldsNum*i + nColumnNum];
		
		bool bRet;
		if(T_INT == type) 
			bRet = _FieldEqu<T_INT>(theField, value);
		else if(T_FLOAT == type)
			bRet = _FieldEqu<T_FLOAT>(theField, value);
		else
			bRet = _FieldEqu<T_STRING>(theField, value);

		if(bRet) return &(m_vDataBuf[m_nFieldsNum*i]);
	}

	return 0;
}

}
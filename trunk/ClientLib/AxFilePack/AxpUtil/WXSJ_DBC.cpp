#include "stdafx.h"
#include <string>
#include <map>
#include <assert.h>
#include "WXSJ_DBC.h"
#include <exception>

namespace DBC
{

	INT	DBCFile::_ConvertStringToVector(const CHAR* strStrINTgSource, std::vector< std::string >& vRet, const CHAR* szKey, BOOL bOneOfKey, BOOL bIgnoreEmpty)
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
		m_nRecordsNum = 0;
		m_nFieldsNum = 0;
	}

	DBCFile::~DBCFile()
	{
		if(m_pStringBuf) delete[] m_pStringBuf;
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

	BOOL DBCFile::WriteToBinFile(const CHAR* szFileName)
	{
		assert(szFileName);

		FILE* fp = fopen(szFileName, "wb");
		if(fp == 0)
		{
			return FALSE;
		}

		//write file head
		FILE_HEAD theHead;
		theHead.m_Identify = 0XDDBBCC00;
		theHead.m_nFieldsNum = GetFieldsNum();
		theHead.m_nRecordsNum = GetRecordsNum();
		theHead.m_nStringBlockSize = m_nStringBufSize;

		if(1 != fwrite(&theHead, sizeof(FILE_HEAD), 1, fp))
		{
			fclose(fp);
			return FALSE;
		}

		//write field type
		if(m_theType.size() !=
			fwrite((const void*)(&(m_theType[0])), sizeof(FIELD_TYPE), m_theType.size(), fp))
		{
			fclose(fp);
			return FALSE;
		}

		DATA_BUF bufTemp = m_vDataBuf;

		//convert string runtime to offset
		for(int i=0; i<(int)bufTemp.size(); i++)
		{
			if(m_theType[i%m_nFieldsNum] == T_STRING)
			{
				bufTemp[i].iValue -= (INT)(INT_PTR)m_pStringBuf;
			}
		}

		//write data block
		if(m_vDataBuf.size() !=
			fwrite((const void*)(&(bufTemp[0])), sizeof(FIELD), bufTemp.size(), fp))
		{
			fclose(fp);
			return FALSE;
		}

		//write string block
		if(1 != fwrite(m_pStringBuf, m_nStringBufSize, 1, fp))
		{
			fclose(fp);
			return FALSE;
		}

		fclose(fp);
		return TRUE;
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
		std::vector< UINT > vFieldType;
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
				delete[] (m_pStringBuf);
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
		memcpy(m_pStringBuf, pMem, m_nStringBufSize);
		m_pStringBuf[m_nStringBufSize-1]=0;

		//------------------------------------------------------
		// TO runtime address
		for(INT j=0; j<(INT)theHead.m_nFieldsNum; j++)
		{
			if(vFieldType[j] != T_STRING) continue;

			for(INT i=0; i<(INT)theHead.m_nRecordsNum; i++)
			{

				m_vDataBuf[i*GetFieldsNum()+j].pString += reinterpret_cast<unsigned long long>(m_pStringBuf);
			}
		}

		//------------------------------------------------------
		//����������
		CreateIndex(0, szFileName);

		return TRUE;
	}

	BOOL DBCFile::OpenFromMemoryImpl_Text(const CHAR* pMemory, const CHAR* pDeadEnd, const CHAR* szFileName)
	{
		const int MAX_LINE_BYTES = 1024*64;

		//----------------------------------------------------
		//��������������
		CHAR szLine[MAX_LINE_BYTES] = {0};
		//����һ��
		register const char* pMem = pMemory;
		pMem = _GetLineFromMemory(szLine, MAX_LINE_BYTES, pMem, pDeadEnd);
		if(!pMem) return FALSE;

		//�ֽ�
		std::vector< std::string > vRet;
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
		std::vector< std::pair< std::string, INT > >	vStringBuf;
		//������
		std::map< std::string, INT >					mapStringBuf;

		//--------------------------------------------------------------
		//��ʼ��ȡ

		//�ն�һ��
		pMem = _GetLineFromMemory(szLine, MAX_LINE_BYTES, pMem, pDeadEnd);
		if(!pMem) return FALSE;

		INT nStringBufSize = 0;
		do
		{
			//��ȡһ��
			pMem = _GetLineFromMemory(szLine, MAX_LINE_BYTES, pMem, pDeadEnd);
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
		fclose(fp); fp=0;

		BOOL bRet = OpenFromMemory(pMemory, pMemory+nFileSize+1, szFileName);

		delete[] pMemory; pMemory = 0;
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
				_snprintf(szTemp, 260, "[%s]Multi index at line %d(SAME:value=%d)", szFileName, i+1, p->iValue);
#ifdef GAME_CLIENT
				throw std::exception(szTemp);
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
			_snprintf(szTemp, MAX_PATH, "DBC:Invalid search request[rec=%d, col=%d]", nRecordLine, nColumNum);
#ifdef GAME_CLIENT
			throw std::exception(szTemp);
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
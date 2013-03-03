// AxCryptoMath.cpp: implementation of the CAxCryptoMath class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AxCryptoMath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*/////////////////////////////////////////////////////////////////////

//解密及验证步骤

//  1. 定义共钥数据
BYTE g_CryptoPubKey[] = 
{
"\x06\x02\x00\x00\x00\x24\x00\x00\x52\x53\x41\x31\x00\x04\x00\x00"
"\x01\x00\x01\x00\xE3\x7C\xC6\x75\x42\xF2\xAB\xB4\xFB\x31\x19\x04"
"\xE8\x36\xBB\x83\x83\x61\x01\x57\x91\xC4\x70\x3E\xCA\xD0\x08\xA5"
"\x8C\x82\xED\xE4\xB4\x74\x38\xEA\x7D\xAC\x27\xFC\xF4\xF7\x8D\x8F"
"\xE2\xE5\xD3\x82\x8A\xD9\xF7\x41\x61\x9F\x14\xB1\xBD\xD3\xDD\x96"
"\x69\x81\xDD\x2E\x97\xA4\xF2\xCB\x66\x20\x96\x1F\x7A\x2F\xE8\x58"
"\x6B\x3F\xC2\xE4\x71\x1D\xE4\x75\x5E\x75\x75\x59\x97\x6D\xBD\x8A"
"\x9D\xA6\x5D\x06\xE3\x16\xCA\xA8\xD2\x94\x0A\x07\x61\x7E\xB0\x1A"
"\x36\xBC\x70\x3E\x88\xC3\x25\x18\xCA\x04\xEF\x86\x24\xA3\xF5\x1E"
"\x03\x15\xA9\xB5"
};


BOOL DeCryptoIniFile(LPCTSTR szIniFileName)
{
//	2. 生成解压算法对象
	CAxCryptoMath	theCryptoCenter;
	HRESULT hRet;
//	3. 导入共钥	
	if(S_OK != (hRet = theCryptoCenter.ImportPubKey(g_CryptoPubKey, sizeof(g_CryptoPubKey)/sizeof(BYTE)-1)))
	{
		return FALSE;
	}
//	4. 解压并验证签名是否正确
	if(S_OK != (hRet = theCryptoCenter.DecryptFileWithSign(szIniFileName, szIniFileName)))
	{
		return FALSE;
	}

	return TRUE;
}


/////////////////////////////////////////////////////////////////////*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAxCryptoMath::CAxCryptoMath()
{
	m_hProv = NULL;
	m_hPubKey = NULL;
#ifdef AX_CRYPTOMATH_MAKER
	m_hPairKey = NULL;
#endif
}

CAxCryptoMath::~CAxCryptoMath()
{
	if(m_hProv) 
		CryptReleaseContext(m_hProv, 0);

	if(m_hPubKey)
		CryptDestroyKey(m_hPubKey);

#ifdef AX_CRYPTOMATH_MAKER
	if(m_hPairKey)
		CryptDestroyKey(m_hPairKey);
#endif

}

BOOL CAxCryptoMath::Init(void)
{
	//--------------------------------------------------------------------
	// 获得CSP句柄,密钥容器名为登陆用户名	
	if(CryptAcquireContext(
		&m_hProv, 
		NULL, 
		NULL, 
		PROV_RSA_FULL, 
		0)) 
	{
		//CSP context acquired
	}
	else	//密钥容器不存在创建之
	{
		if(CryptAcquireContext(
			&m_hProv, 
			NULL, 
			NULL, 
			PROV_RSA_FULL, 
			CRYPT_NEWKEYSET)) 
		{
			//A new key container has been created
		}
		else
		{
			// Error during CryptAcquireContext
			return FALSE;
		}
	}

	return TRUE;
}

//输入公钥
HRESULT	 CAxCryptoMath::ImportPubKey(BYTE* pBuf, DWORD nBufLen)
{
	if(m_hProv == NULL) 
	{
		if(!Init()) return GetLastError();
	}

	if(m_hPubKey)
	{
		CryptDestroyKey(m_hPubKey);
	}

	if(CryptImportKey(
		m_hProv,
		pBuf,
		nBufLen,
		0,
		0,
		&m_hPubKey))
	{
		
	}
	else
	{
		//Public key import failed.
		return GetLastError();
	}

	return S_OK;
}

//验证签名
HRESULT	CAxCryptoMath::VerifySign(BYTE* pBuf, DWORD nBufLen, BYTE* pSignCode, DWORD nSignBufLen)
{
	//必须已经倒入公钥
	if(m_hProv == NULL || m_hPubKey == NULL) return E_FAIL;

	//--------------------------------------------------------------------
	// 创建哈希对象

	HCRYPTHASH hHash = NULL;
	HRESULT hRet = S_OK;
	if(CryptCreateHash(
		m_hProv, 
		CALG_MD5, 
		0, 
		0, 
		&hHash)) 
	{
		//The hash object has been recreated.
	}
	else
	{
		//Error during CryptCreateHash.
		hRet = GetLastError();
		if(hHash) CryptDestroyHash(hHash);
		return hRet;
	}

	//--------------------------------------------------------------------
	// 跟生成时一样对数据进行hash运算
	if(CryptHashData(
		hHash, 
		pBuf, 
		nBufLen, 
		0)) 
	{
		//The new has been created.
	}
	else
	{
		//Error during CryptHashData.
		hRet = GetLastError();
		if(hHash) CryptDestroyHash(hHash);
		return hRet;
	}

	//--------------------------------------------------------------------
	// 验证数字签名
	if(CryptVerifySignature(
		hHash, 
		pSignCode,		//数字签名数据
		nSignBufLen, 
		m_hPubKey,		//签名者的公钥
		NULL, 
		0)) 
	{
		//The signature has been verified.
	}
	else
	{
		//Signature not validated!
		if(hHash) CryptDestroyHash(hHash);
		return E_FAIL;
	}

	//验证成功！
	if(hHash) CryptDestroyHash(hHash);
	return S_OK;
}

//解密加密后的文件并验证签名
HRESULT CAxCryptoMath::UnSignFile(LPCTSTR szInputFileName, LPCTSTR szOutputFileName, BOOL bEncrypt, BOOL bTextSign)
{
	//必须已经倒入公钥
	if(m_hProv == NULL || m_hPubKey == NULL) return E_FAIL;

	HRESULT hRet = S_OK;

	//读取文件
	FILE* fp = fopen(szInputFileName, "rb");
	if(fp == NULL) return ERROR_FILE_NOT_FOUND;
	
	fseek(fp, 0, SEEK_END);
	INT nFileLen = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	BYTE* pBuf = new BYTE[nFileLen];
	if(pBuf == NULL) return ERROR_NOT_ENOUGH_MEMORY;
	fread(pBuf, 1, nFileLen, fp);
	fclose(fp); fp = NULL;

	//签名
	BYTE pSignBuf[256] = {0};
	int nSignSize = 256;

	//原文件大小
	INT nOrgFileLen = 0;

	//取得签名
	if(bTextSign)
	{
		const INT nTextSignSize = 180;
		if(nTextSignSize >= nFileLen)
		{
			delete[] pBuf;
			return E_FAIL;
		}

		BYTE szTextSign[256];
		memcpy(szTextSign, pBuf+nFileLen-nTextSignSize, nTextSignSize);
		szTextSign[nTextSignSize] = 0;

		S_UnUUECodeBuf(szTextSign, nTextSignSize, pSignBuf, nSignSize);
		nOrgFileLen = nFileLen-nTextSignSize;
	}
	else
	{
		const INT nBinSignSize = 128;
		if(nBinSignSize >= nFileLen)
		{
			delete[] pBuf;
			return E_FAIL;
		}

		memcpy(pSignBuf, pBuf+nFileLen-nBinSignSize, nBinSignSize);
		nSignSize = nBinSignSize;
		nOrgFileLen = nFileLen-nBinSignSize;
	}

	//验证签名是否正确
	hRet = VerifySign(pBuf, 
		nOrgFileLen, 
		pSignBuf, 
		nSignSize);

	if(hRet != S_OK)
	{
		delete[] pBuf;
		return E_FAIL;
	}

	//验证成功,解密
	if(bEncrypt)
	{
		S_EncryptBuffer(pBuf + sizeof(INT), nOrgFileLen, 0XC0DE);
	}
	
	//生成原始文件
	fp = fopen(szOutputFileName, "wb");
	if(fp == NULL)
	{
		delete[] pBuf;
		return ERROR_ACCESS_DENIED;
	}

	fwrite(pBuf, 1, nOrgFileLen, fp);
	fclose(fp);

	return S_OK;
}

#ifdef AX_CRYPTOMATH_MAKER
//创建出一对共钥和私钥
HRESULT CAxCryptoMath::CreatePairKey(BYTE* pBuf, DWORD& nBufLen)
{
	if(m_hProv == NULL) 
	{
		if(!Init()) return GetLastError();
	}

	if(m_hPairKey)
	{
		CryptDestroyKey(m_hPairKey);
	}

	//--------------------------------------------------------------------
	//创建共钥/密钥对
	if(CryptGenKey(
		m_hProv,			//CSP句柄
		AT_SIGNATURE,		//创建的密钥对类型为signature key pair
		CRYPT_EXPORTABLE,	//key类型，这里用默认值
		&m_hPairKey)) 		//创建成功返回新创建的密钥对的句柄
	{
		//Created a signature key pair
	}
	else
	{
		//Error occurred creating a signature key
		return GetLastError();
	}
	
	//--------------------------------------------------------------------
	// 真正导出公钥数据
	if(CryptExportKey(   
		m_hPairKey, 
		NULL,    
		PRIVATEKEYBLOB,    
		0,    
		pBuf,
		&nBufLen))
	{
		//Contents have been written to the BLOB
	}
	else
	{
		//Error during CryptExportKey.
		return GetLastError();
	}

	return S_OK;
}

//输入公钥/私钥对
HRESULT	 CAxCryptoMath::ImportPairKey(BYTE* pBuf, DWORD nBufLen)
{
	if(m_hProv == NULL) 
	{
		if(!Init()) return GetLastError();
	}

	if(m_hPairKey)
	{
		CryptDestroyKey(m_hPairKey);
	}

	if(CryptImportKey(
		m_hProv,
		pBuf,
		nBufLen,
		0,
		0,
		&m_hPairKey))
	{
		
	}
	else
	{
		//Public key import failed.
		return GetLastError();
	}

	//生成PubKey
	BYTE pTempBuf[1024] = {0};
	DWORD dwSize = 1024;
	ExportPubKey(pTempBuf, dwSize);
	ImportPubKey(pTempBuf, dwSize);

	return S_OK;
}

HRESULT	CAxCryptoMath::ExportPubKey(BYTE* pBuf, DWORD& nBufLen)
{
	//必须已经输入PairKey或者创建PairKey
	if(m_hProv==NULL || m_hPairKey==NULL) return E_FAIL;
	
	if(CryptExportKey(   
		m_hPairKey, 
		NULL,    
		PUBLICKEYBLOB,    
		0,    
		pBuf,
		&nBufLen))
	{
		//Contents have been written to the BLOB
	}
	else
	{
		//Error during CryptExportKey.
		return GetLastError();
	}

	return S_OK;
}

//对数据进行签名
HRESULT	CAxCryptoMath::SignBuf(BYTE* pBuf, DWORD nBufLen, BYTE* pSignCode, DWORD& nSignBufLen)
{
	//必须已经导入或创建公钥/私钥对
	if(m_hProv == NULL || m_hPairKey == NULL) return E_FAIL;

	HCRYPTHASH hHash = NULL;
	HRESULT hRet = S_OK;
	//--------------------------------------------------------------------
	// 创建hash对象
	if(CryptCreateHash(
		m_hProv, 
		CALG_MD5, 
		0, 
		0, 
		&hHash)) 
	{
		// Hash object created.
	}
	else
	{
		//Error during CryptCreateHash.
		hRet = GetLastError();
		if(hHash) CryptDestroyHash(hHash);
		return hRet;
	}

	//--------------------------------------------------------------------
	// 对数据进行hash运算
	if(CryptHashData(
		hHash, 
		pBuf, 
		nBufLen, 
		0)) 
	{
		//The data buffer has been hashed.
	}
	else
	{
		//Error during CryptHashData.
		hRet = GetLastError();
		if(hHash) CryptDestroyHash(hHash);
		return hRet;
	}

	//--------------------------------------------------------------------
	// 使用signature key pair的私钥对hash数据签名

	if(CryptSignHash(
		hHash, 
		AT_SIGNATURE, 
		NULL, 
		0, 
		pSignCode, //这里将返回数字签名，同被签名的数据一起发送给接收方
		&nSignBufLen)) 
	{
		//pbSignature is the hash signature.
	}
	else
	{
		//Error during CryptSignHash.
		hRet = GetLastError();
		if(hHash) CryptDestroyHash(hHash);
		return hRet;
	}

	//--------------------------------------------------------------------
	// Destroy the hash object.
	if(hHash) 
		CryptDestroyHash(hHash);
	
	return S_OK;
}

HRESULT CAxCryptoMath::SignFile(LPCTSTR szInputFileName, LPCTSTR szOutputFileName, BOOL bEncrypt, BOOL bTextSign)
{
	//必须已经导入或声称公钥/私钥对
	if(m_hProv == NULL || m_hPairKey == NULL) return E_FAIL;
	
	HRESULT hRet = S_OK;

	//读取文件
	FILE* fp = fopen(szInputFileName, "rb");
	if(fp == NULL) return ERROR_FILE_NOT_FOUND;
	
	fseek(fp, 0, SEEK_END);
	INT nFileLen = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	BYTE* pBuf = new BYTE[nFileLen];
	if(pBuf == NULL) return ERROR_NOT_ENOUGH_MEMORY;
	fread(pBuf, 1, nFileLen, fp);
	fclose(fp); fp = NULL;

	//加密
	if(bEncrypt)
	{
		S_EncryptBuffer(pBuf, nFileLen, 0XC0DE);
	}

	//签名
	BYTE pSign[1024] = {0};
	DWORD nSignLen = 1024;
	hRet = SignBuf(pBuf, nFileLen, pSign, nSignLen);
	if(hRet != S_OK)
	{
		if(pBuf) delete[] pBuf;
		return hRet;
	}

	//以文本形式保存签名
	BYTE szTextSign[1024] = {0};
	int nTextSignLen = 1024;
	if(bTextSign)
	{
		S_UUECodeBuf(pSign, nSignLen, szTextSign, nTextSignLen);
	}

	//保存加密后文件以及签名
	fp = fopen(szOutputFileName, "wb");
	if(fp == NULL) return ERROR_ACCESS_DENIED;
	fwrite(pBuf, 1, nFileLen, fp);
	if(bTextSign)
	{
		fwrite(szTextSign, 1, nTextSignLen, fp);
	}
	else
	{
		fwrite(pSign, 1, nSignLen, fp);
	}
	fclose(fp);

	delete[] pBuf;

	return S_OK;
}

#endif

/*Uuencode编码*/
/* 
chasc：未编码的二进制代码
chuue：编码过的Uue代码
*/
void CAxCryptoMath::S_UUECode(BYTE chasc[3], BYTE chuue[4])
{
	int i,k=2;
	BYTE t=NULL;

	for(i=0; i<3; i++)
	{
		*(chuue+i)=*(chasc+i)>>k;
		*(chuue+i)|=t;
		if(*(chuue+i)==NULL) *(chuue+i)+=96;
		else *(chuue+i)+=32;
		t=*(chasc+i)<<(8-k);
		t>>=2;
		k+=2;
	}
	
	*(chuue+3)=*(chasc+2)&63;
	if(*(chuue+3)==NULL) *(chuue+3)+=96;
	else *(chuue+3)+=32;
}

/*Uuencode解码*/
/* 
chuue：未解码的Uue代码
chasc：解码过的二进制代码
*/
void CAxCryptoMath::S_UnUUECode(BYTE chuue[4], BYTE chasc[3])
{
	int i,k=2;
	BYTE t=NULL;
	if(*chuue==96) *chuue=NULL;
	else *chuue-=32;
	for(i=0;i<3;i++)
	{
		*(chasc+i)=*(chuue+i)<<k;
		k+=2;
		if(*(chuue+i+1)==96) *(chuue+i+1)=NULL;
		else *(chuue+i+1)-=32;
		t=*(chuue+i+1)>>(8-k);
		*(chasc+i)|=t;
	}
}

int CAxCryptoMath::S_UUECodeBuf(BYTE *pOrg, int nOrgLen, BYTE* pUUE, int& nUUELen)
{

	int nShouldLen;
	if((nOrgLen / 3) * 3 == nOrgLen) nShouldLen = (nOrgLen / 3)*4;
	else nShouldLen = (nOrgLen/3+1)*4;

	nShouldLen += 8;

	if(pUUE == NULL || nUUELen < nShouldLen)
	{
		nUUELen = nShouldLen;
		return nUUELen;
	}

	BYTE szOrgTemp[3];
	BYTE szUUETemp[4];

	BYTE* pNow = pUUE;
	sprintf((CHAR*)pNow, "%08X", nOrgLen);
	pNow += 8;

	int i = 0;
	for(i=0; i<nOrgLen/3; i++)
	{
		memcpy(szOrgTemp, pOrg+i*3, 3);

		S_UUECode(szOrgTemp, szUUETemp);

		memcpy(pNow, szUUETemp, 4);
		pNow += 4;
	}

	//补零
	if(i*3 < nOrgLen)
	{
		memcpy(szOrgTemp, pOrg+i*3, nOrgLen - i*3);
		memset(szOrgTemp+(nOrgLen - i*3), 0, 3-(nOrgLen - i*3));

		S_UUECode(szOrgTemp, szUUETemp);
		memcpy(pNow, szUUETemp, 4);
		pNow += 4;
	}
	

	nUUELen = nShouldLen;
	return nUUELen;
}

int CAxCryptoMath::S_UnUUECodeBuf(BYTE *pUUE, int nUUELen, BYTE* pOrg, int& nOrgLen)
{
	INT nShouldLen;

	CHAR szOrgLenBuf[9];
	memcpy(szOrgLenBuf, pUUE, 8);
	szOrgLenBuf[8] = 0;

	sscanf(szOrgLenBuf, "%08X", &nShouldLen);
	if(pOrg == NULL || nShouldLen > nOrgLen)
	{
		nOrgLen = nShouldLen;
		return nShouldLen;
	}

	nUUELen -= 8;

	BYTE szOrgTemp[3];
	BYTE szUUETemp[4];

	BYTE* pNow = pOrg;

	for(int i=0; i<nUUELen/4; i++)
	{
		memcpy(szUUETemp, pUUE + 8 + i*4, 4);
		
		S_UnUUECode(szUUETemp, szOrgTemp);

		memcpy(pNow, szOrgTemp, 3);
		pNow += 3;
	}

	nOrgLen = nShouldLen;
	return nShouldLen;
}

int myrand(unsigned int* pSeed)
{
	return ((( *pSeed = (*pSeed) * 214013L + 2531011L) >> 16) & 0x7fff);
}

void CAxCryptoMath::S_EncryptBuffer(BYTE* pBuf, int nLen, DWORD dwKey)
{
	#define BASE_XOR_VALUE	(0x04020626)
	#define BASE_PLUS_VALUE	(0x00970702)
	#define DEF_ENCRYPT_KEY	(0x03070201)

	int i;
	unsigned int nSeed;
	nSeed=dwKey;

	for(i=0;i<nLen;i++)
		pBuf[i] = (BYTE)(pBuf[i] ^ ((BYTE)(myrand(&nSeed)+BASE_PLUS_VALUE)) ^ BASE_XOR_VALUE);
}


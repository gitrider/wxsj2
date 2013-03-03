// AxCryptoMath.h: interface for the CAxCryptoMath class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AXCRYPTOMATH_H__6E51E2D1_A77A_4A44_893E_C1EF01EB432D__INCLUDED_)
#define AFX_AXCRYPTOMATH_H__6E51E2D1_A77A_4A44_893E_C1EF01EB432D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <wincrypt.h>

class CAxCryptoMath  
{
public:
	CAxCryptoMath();
	virtual ~CAxCryptoMath();

public:
	BOOL				Init(void);
	HRESULT				ImportPubKey(BYTE* pBuf, DWORD nBufLen);
	HRESULT				VerifySign(BYTE* pBuf, DWORD nBufLen, BYTE* pSignCode, DWORD nSignBufLen);
	HRESULT				UnSignFile(LPCTSTR szInputFileName, LPCTSTR szOutputFileName, BOOL bEncrypt, BOOL bTextSign);

public:
	static int			S_UUECodeBuf(BYTE *pOrg, int nOrgLen, BYTE* pUUE, int& nUUELen);
	static int			S_UnUUECodeBuf(BYTE *pUUE, int nUUELen, BYTE* pOrg, int& nOrgLen);
	static void			S_EncryptBuffer(BYTE *pBuf, int nLen, DWORD dwKey);

protected:
	static void			S_UUECode(BYTE chasc[3], BYTE chuue[4]);
	static void			S_UnUUECode(BYTE chuue[4], BYTE chasc[3]);

private:
	HCRYPTPROV			m_hProv;
	HCRYPTKEY			m_hPubKey;

//以下函数用于加密
#ifdef AX_CRYPTOMATH_MAKER

public:
	HRESULT				CreatePairKey(BYTE* pBuf, DWORD& nBufLen);
	HRESULT				ImportPairKey(BYTE* pBuf, DWORD nBufLen);
	HRESULT				ExportPubKey(BYTE* pBuf, DWORD& nBufLen);
	HRESULT				SignBuf(BYTE* pBuf, DWORD nBufLen, BYTE* pSignCode, DWORD& nSignBufLen);
	HRESULT				SignFile(LPCTSTR szInputFileName, LPCTSTR szOutputFileName, BOOL bEncrypt, BOOL bTextSign);

private:
	HCRYPTKEY			m_hPairKey;

#endif

};

#endif // !defined(AFX_AXCRYPTOMATH_H__6E51E2D1_A77A_4A44_893E_C1EF01EB432D__INCLUDED_)

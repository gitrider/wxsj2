#include "Encoder_XOR.h"

namespace Encode_Decode
{
	BOOL EncoderXOR_T::Encode(Encoder_T::Parameters_T& rParams) const
	{
		UCHAR const* pPlainText = rParams.GetConstPlainText();
		if(NULL==pPlainText)
		{
			return FALSE;
		}
		INT nPlainTextSize = rParams.GetPlainTextSize();
		if(0>=nPlainTextSize)
		{
			return FALSE;
		}
		UCHAR * pCipher = rParams.GetAlterableCripher();
		if(NULL==pCipher)
		{
			return FALSE;
		}
		INT nCipherSize = rParams.GetCripherSize();
		if(0>=nCipherSize || nCipherSize<nPlainTextSize)
		{
			return FALSE;
		}
		UCHAR const* pKey = rParams.GetKey();
		if(NULL==pKey)
		{
			return FALSE;
		}
		INT nKeySize = rParams.GetKeySize();
		if(0>=nKeySize)
		{
			return FALSE;
		}
		INT nIdxInKey = rParams.GetParam0();
		for(INT nIdx=0; nPlainTextSize>nIdx; ++nIdx)
		{
			pCipher[nIdx] = pPlainText[nIdx]^pKey[nIdxInKey];
			++nIdxInKey;
			if(nIdxInKey>=nKeySize)
			{
				nIdxInKey -=nKeySize;
			}
		}
		rParams.SetParam0(nIdxInKey);
		return TRUE;
	}
	BOOL EncoderXOR_T::Decode(Encoder_T::Parameters_T& rParams) const
	{
		UCHAR* pPlainText = rParams.GetAlterablePlainText();
		if(NULL==pPlainText)
		{
			return FALSE;
		}
		INT nPlainTextSize = rParams.GetPlainTextSize();
		if(0>=nPlainTextSize)
		{
			return FALSE;
		}
		UCHAR const* pCipher = rParams.GetConstCripher();
		if(NULL==pCipher)
		{
			return FALSE;
		}
		INT nCipherSize = rParams.GetCripherSize();
		if(0>=nCipherSize || nCipherSize>nPlainTextSize)
		{
			return FALSE;
		}
		UCHAR const* pKey = rParams.GetKey();
		if(NULL==pKey)
		{
			return FALSE;
		}
		INT nKeySize = rParams.GetKeySize();
		if(0>=nKeySize)
		{
			return FALSE;
		}
		INT nIdxInKey = rParams.GetParam0();
		for(INT nIdx=0; nPlainTextSize>nIdx; ++nIdx)
		{
			pPlainText[nIdx] = pCipher[nIdx]^pKey[nIdxInKey];
			++nIdxInKey;
			if(nIdxInKey>=nKeySize)
			{
				nIdxInKey -=nKeySize;
			}
		}
		rParams.SetParam0(nIdxInKey);
		return TRUE;
	}
	BOOL EncoderXOR_T::Skip(Encoder_T::Parameters_T& rParams, INT nSize) const
	{
		INT nKeySize = rParams.GetKeySize();
		if(0>=nKeySize)
		{
			return FALSE;
		}
		INT nIdxInKey = rParams.GetParam0();
		for(INT nIdx=0; nSize>nIdx; ++nIdx)
		{
			++nIdxInKey;
			if(nIdxInKey>=nKeySize)
			{
				nIdxInKey -=nKeySize;
			}
		}
		rParams.SetParam0(nIdxInKey);
		return TRUE;
	}

}

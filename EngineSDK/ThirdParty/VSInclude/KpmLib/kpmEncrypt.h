#ifndef _KPM_ENCRYPT_H_
#define _KPM_ENCRYPT_H_
#include "kpmLib.h"

namespace KPM
{
#define  HELPER_BUFFER_SIZE   0x500
	
			
	int   			PerpareBuffers();

	void  			Encrypt(DWORD * pdwPtr, BYTE * pbKey, DWORD dwLength);
	void  			Decrypt(DWORD * pdwPtr, BYTE * pbKey, DWORD dwLength);
	
	void  			EncryptBlockTable(DWORD * pdwTable, BYTE * pbKey, DWORD dwLength);
	void  			DecryptBlockTable(DWORD * pdwTable, BYTE * pbKey, DWORD dwLength);
	
	DWORD 			DecryptHashIndex(TKPMArchive * hArchive, const char * szFileName);
	DWORD 			DetectFileSeed(DWORD * block, DWORD decrypted);
	DWORD 			DetectFileSeed2(DWORD * block, UINT nDwords, ...);
	void  			EncryptBlock(DWORD * pdwBlock, DWORD dwLength, DWORD dwSeed1);
	void  			DecryptBlock(DWORD * pdwBlock, DWORD dwLength, DWORD dwSeed1);
	
	DWORD 			DecryptName1    (const char * szFileName);
	DWORD 			DecryptName2    (const char * szFileName);
	DWORD 			DecryptFileSeed (const char * szFileName);

	TKPMHashKey *	GetHashEntry  (TKPMArchive * pArch, const char * szFileName);
	
	TKPMHashKey *	FindFreeHashEntry(TKPMArchive * pArch, const char * szFileName);

	extern BOOL		bBufferCreated;
	extern DWORD	mBuffer[HELPER_BUFFER_SIZE];

		
}

#endif
//---------------------------------------------------------------------------
// Script Engine by FireFox
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef FoxMemBase_H
#define FoxMemBase_H
//---------------------------------------------------------------------------
ENGINE_API	void	g_MemInfo(void);
ENGINE_API	void*	g_MemAlloc(DWORD dwSize);
ENGINE_API	void	g_MemFree(LPVOID lpMem);
ENGINE_API	void	g_MemCopy(LPVOID lpDest, LPVOID lpSrc, DWORD dwLen);
ENGINE_API	void	g_MemCopyMmx(LPVOID lpDest, LPVOID lpSrc, DWORD dwLen);
ENGINE_API	BOOL	g_MemComp(LPVOID lpDest, LPVOID lpSrc, DWORD dwLen);
ENGINE_API	void	g_MemFill(LPVOID lpDest, DWORD dwLen, BYTE byFill);
ENGINE_API	void	g_MemFill(LPVOID lpDest, DWORD dwLen, WORD wFill);
ENGINE_API	void	g_MemFill(LPVOID lpDest, DWORD dwLen, DWORD dwFill);
ENGINE_API	void	g_MemZero(LPVOID lpDest, DWORD dwLen);
ENGINE_API	void	g_MemXore(LPVOID lpDest, DWORD dwLen, DWORD dwXor);
//---------------------------------------------------------------------------
#endif

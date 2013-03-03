//---------------------------------------------------------------------------
// Script Engine by FireFox
// Desc:	Memory base functions
//---------------------------------------------------------------------------
#include "FoxWin32.h"
#include "FoxDebug.h"
#include "FoxNode.h"
#include "FoxList.h"
#include "FoxMemBase.h"
#include <string.h>
//---------------------------------------------------------------------------
class FoxMemNode : public FoxNode
{
public:
	DWORD	m_dwMemSize;//�ڴ��С
	DWORD	m_dwMemSign;//�ڴ��־
};
//---------------------------------------------------------------------------
class FoxMemList : public FoxList
{
public:
	~FoxMemList()
	{
		FoxMemNode* pNode = (FoxMemNode*)GetHead();
		while (pNode)
		{
//			g_DebugLog("KMemList::Leak Detected, Size = %d", pNode->m_dwMemSize);
			pNode = (FoxMemNode*)pNode->GetNext();
		}
	};
	void ShowUsage()
	{
		FoxMemNode* pNode = (FoxMemNode*)GetHead();
		DWORD dwMemSize = 0;
		while (pNode)
		{
			dwMemSize += pNode->m_dwMemSize;
			pNode = (FoxMemNode*)pNode->GetNext();
		}
//		g_DebugLog("Memory Usage Size = %d KB", dwMemSize / 1024);
	}
};
static FoxMemList m_MemList;
//---------------------------------------------------------------------------
#define MEMSIGN 1234567890
//---------------------------------------------------------------------------
// ����:	g_MemInfo
// ����:	Memory Infomation
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
ENGINE_API void g_MemInfo()
{
//	MEMORYSTATUS stat;
	
//	GlobalMemoryStatus(&stat);
	
//	g_DebugLog("Total Physical Memory = %d MB", stat.dwTotalPhys >> 20);
//	g_DebugLog("Total Virtual Memory = %d MB", stat.dwTotalVirtual >> 20);
//	g_DebugLog("%d percent of memory is in use.", stat.dwMemoryLoad);
}
//---------------------------------------------------------------------------
// ����:	g_MemAlloc
// ����:	�����ڴ�
// ����:	dwSize		�ڴ���С
// ����:	lpMem (lpMem = NULL ��ʾ����ʧ��)
//---------------------------------------------------------------------------
ENGINE_API LPVOID g_MemAlloc(DWORD dwSize)
{
//	HANDLE hHeap = GetProcessHeap();
	PBYTE  lpMem = NULL;
	DWORD  dwHeapSize = dwSize + sizeof(FoxMemNode);

//	lpMem = (PBYTE)HeapAlloc(hHeap, 0, dwHeapSize);
	lpMem = (PBYTE)new char[dwHeapSize];
	if (NULL == lpMem)
	{
//		g_MessageBox("g_MemAlloc() Failed, Size = %d", dwSize);
		return NULL;
	}

	FoxMemNode* pNode = (FoxMemNode*)lpMem;
	pNode->m_pPrev = NULL;
	pNode->m_pNext = NULL;
	pNode->m_dwMemSize = dwSize;
	pNode->m_dwMemSign = MEMSIGN;
	m_MemList.AddHead(pNode);
	
	return (lpMem + sizeof(FoxMemNode));
//	return 0;
}
//---------------------------------------------------------------------------
// ����:	g_MemFree
// ����:	�ͷ��ڴ�
// ����:	lpMem		Ҫ�ͷŵ��ڴ�ָ��
// ����:	void
//---------------------------------------------------------------------------
ENGINE_API void g_MemFree(LPVOID lpMem)
{
//	HANDLE hHeap = GetProcessHeap();
	if (lpMem == NULL)
		return;
	lpMem = (PBYTE)lpMem - sizeof(FoxMemNode);
	FoxMemNode* pNode = (FoxMemNode *)lpMem;
	if (pNode->m_dwMemSign != MEMSIGN)
	{
//		g_MessageBox("g_MemFree() Failed, Size = %d", pNode->m_dwMemSize);
		return;
	}
	pNode->Remove();
//	HeapFree(hHeap, 0, lpMem);
	delete[] lpMem;
}
//---------------------------------------------------------------------------
// ����:	MemoryCopy
// ����:	�ڴ濽��
// ����:	lpDest	:	Ŀ���ڴ��
//			lpSrc	:	Դ�ڴ��
//			dwLen	:	��������
// ����:	void
//---------------------------------------------------------------------------
ENGINE_API void g_MemCopy(PVOID lpDest, PVOID lpSrc, DWORD dwLen)
{	
#ifdef WIN32
	__asm
	{
		mov		edi, lpDest
		mov		esi, lpSrc
		mov		ecx, dwLen
		mov     ebx, ecx
		shr     ecx, 2
		rep     movsd
		mov     ecx, ebx
		and     ecx, 3
		rep     movsb
	}
#else
     memcpy(lpDest, lpSrc, dwLen);
#endif
}
//---------------------------------------------------------------------------
// ����:	MemoryCopyMmx
// ����:	�ڴ濽����MMX�汾��
// ����:	lpDest	:	Ŀ���ڴ��
//			lpSrc	:	Դ�ڴ��
//			dwLen	:	��������
// ����:	void
//---------------------------------------------------------------------------
ENGINE_API void g_MemCopyMmx(PVOID lpDest, PVOID lpSrc, DWORD dwLen)
{
#ifdef WIN32
	__asm
	{
		mov		edi, lpDest
		mov		esi, lpSrc
		mov		ecx, dwLen
		mov     ebx, ecx
		shr     ecx, 3
		jcxz	loc_copy_mmx2

loc_copy_mmx1:

		movq	mm0, [esi]
		add		esi, 8
		movq	[edi], mm0
		add		edi, 8
		dec		ecx
		jnz		loc_copy_mmx1

loc_copy_mmx2:

		mov     ecx, ebx
		and     ecx, 7
		rep     movsb
		emms
	}
#else
     memcpy(lpDest, lpSrc, dwLen);
#endif
}
//---------------------------------------------------------------------------
// ����:	MemoryComp
// ����:	�ڴ�Ƚ�
// ����:	lpDest	:	�ڴ��1
//			lpSrc	:	�ڴ��2
//			dwLen	:	�Ƚϳ���
// ����:	TRUE	:	��ͬ
//			FALSE	:	��ͬ	
//---------------------------------------------------------------------------
ENGINE_API BOOL g_MemComp(PVOID lpDest, PVOID lpSrc, DWORD dwLen)
{	
#ifdef WIN32
	__asm
	{
		mov		edi, lpDest
		mov		esi, lpSrc
		mov		ecx, dwLen
		mov     ebx, ecx
		shr     ecx, 2
		rep     cmpsd
		jne		loc_not_equal
		mov     ecx, ebx
		and     ecx, 3
		rep     cmpsb
		jne		loc_not_equal
	};
	return TRUE;

loc_not_equal:

	return FALSE;
#else
     return (0 == memcmp(lpDest, lpSrc, dwLen));
#endif
}
//---------------------------------------------------------------------------
// ����:	MemoryFill
// ����:	�ڴ����
// ����:	lpDest	:	�ڴ��ַ
//			dwLen	:	�ڴ泤��
//			byFill	:	����ֽ�
// ����:	void
//---------------------------------------------------------------------------
ENGINE_API void g_MemFill(PVOID lpDest, DWORD dwLen, BYTE byFill)
{
#ifdef WIN32
	__asm
	{
		mov		edi, lpDest
		mov		ecx, dwLen
		mov		al, byFill
		mov		ah, al
		mov		bx, ax
		shl		eax, 16
		mov		ax, bx
		mov		ebx, ecx
		shr		ecx, 2
		rep     stosd
		mov     ecx, ebx
		and		ecx, 3
		rep     stosb
	}
#else
     memset(lpDest, byFill, dwLen);
#endif
}
//---------------------------------------------------------------------------
// ����:	MemoryFill
// ����:	�ڴ����
// ����:	lpDest	:	�ڴ��ַ
//			dwLen	:	�ڴ泤��
//			wFill	:	�����
// ����:	void
//---------------------------------------------------------------------------
ENGINE_API void g_MemFill(PVOID lpDest, DWORD dwLen, WORD wFill)
{	
#ifdef WIN32
	__asm
	{
		mov		edi, lpDest
		mov		ecx, dwLen
		mov		ax, wFill
		mov		bx, ax
		shl		eax, 16
		mov		ax, bx
		mov		ebx, ecx
		shr		ecx, 1
		rep     stosd
		mov     ecx, ebx
		and		ecx, 1
		rep     stosw
	}
#else
     memset(lpDest, wFill & 0xff, dwLen);
#endif
}
//---------------------------------------------------------------------------
// ����:	MemoryFill
// ����:	�ڴ����
// ����:	lpDest	:	�ڴ��ַ
//			dwLen	:	�ڴ泤��
//			dwFill	:	�����
// ����:	void
//---------------------------------------------------------------------------
ENGINE_API void g_MemFill(PVOID lpDest, DWORD dwLen, DWORD dwFill)
{	
#ifdef WIN32
	__asm
	{
		mov		edi, lpDest
		mov		ecx, dwLen
		mov		eax, dwFill
		rep     stosd
	}
#else
     memset(lpDest, dwFill & 0xff, dwLen);
#endif
}
//---------------------------------------------------------------------------
// ����:	MemoryZero
// ����:	�ڴ�����
// ����:	lpDest	:	�ڴ��ַ
//			dwLen	:	�ڴ泤��
// ����:	void
//---------------------------------------------------------------------------
ENGINE_API void g_MemZero(PVOID lpDest, DWORD dwLen)
{
#ifdef WIN32
	__asm
	{
		mov		ecx, dwLen
		mov		edi, lpDest
		xor     eax, eax
		mov		ebx, ecx
		shr		ecx, 2
		rep     stosd
		mov     ecx, ebx
		and		ecx, 3
		rep     stosb
	}
#else
     memset(lpDest, 0, dwLen);
#endif
}
//---------------------------------------------------------------------------
// ����:	MemoryXore
// ����:	�ڴ����
// ����:	lpDest	:	�ڴ��ַ
//			dwLen	:	�ڴ泤��
//			dwXor	:	����ֽ�
// ����:	void
//---------------------------------------------------------------------------
ENGINE_API void g_MemXore(PVOID lpDest, DWORD dwLen, DWORD dwXor)
{
#ifdef WIN32
	__asm
	{
		mov		edi, lpDest
		mov		ecx, dwLen
		mov		eax, dwXor
		shr		ecx, 2
		cmp		ecx, 0
		jle		loc_xor_exit
loc_xor_loop:
		xor		[edi], eax
		add		edi, 4
		dec		ecx
		jnz		loc_xor_loop
loc_xor_exit:
	}
#else
     unsigned long *ptr = (unsigned long *)lpDest;
     while((long)dwLen > 0) {
       *ptr++ ^= dwXor;
       dwLen -= sizeof(unsigned long);
     }
#endif
}

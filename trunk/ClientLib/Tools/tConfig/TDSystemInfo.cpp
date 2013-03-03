#include "StdAfx.h"
#include "TDSystemInfo.h"

#include <windows.h>
#include <mmsystem.h>
#include <string.h>
#include <stdio.h>
#pragma comment(lib,"Winmm.lib")
#include <D3d8.h>
#pragma comment(lib,"D3d8.lib")
#include <ddraw.h>
#pragma comment(lib,"ddraw.lib")
#pragma comment(lib,"dxguid.lib")


bool IfCPUID();
void CPUVendor(char *szVendor);
void CPUTFMS(int *pnType, int *pnFamily, int *pnModel, int *pnStepping);
bool CPUCache(int *pnL2, int *pnL3);
int CPUBrand();
int CPUSpeed();

void IntelCPU(char *szName, int nType, int nFamily, int nModel, int nStepping, int nL2, int nL3, int nBrand);
void AMDCPU(char *szName, int nType, int nFamily, int nModel, int nStepping);

bool IfCPUID()
{
	__try
	{
		_asm
		{
			xor eax, eax;
			cpuid;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}
    return true;
}

void CPUVendor(char *szVendor)
{
	union{
        char c[12 + 1];
        struct{
            DWORD dw0;
            DWORD dw1;
            DWORD dw2;
        }s;
    }szTemp;
	_asm
	{
		xor eax, eax;
		cpuid;
		mov szTemp.s.dw0, ebx;
		mov szTemp.s.dw1, edx;
		mov szTemp.s.dw2, ecx;
	}
	szTemp.c[12] = 0;
	if (strcmp(szTemp.c, "AuthenticAMD") == 0
		|| strcmp(szTemp.c, "AMD ISBETTER") == 0)
	{
		strcpy(szVendor, "AMD");
	}
	else if (strcmp(szTemp.c, "GenuineIntel") == 0)
	{
		strcpy(szVendor, "Intel");
	}
	else if (strcmp(szTemp.c, "CyrixInstead") == 0)
	{
		strcpy(szVendor, "Cyrix/VIA");
	}
	else if (strcmp(szTemp.c, "CentaurHauls") == 0)
	{
		strcpy(szVendor, "IDT/Centaur");
	}
	else if (strcmp(szTemp.c, "NexGenDriven") == 0)
	{
		strcpy(szVendor, "NexGen");
	}
	else if (strcmp(szTemp.c, "UMC UMC UMC ") == 0)
	{
		strcpy(szVendor, "UMC");
	}
	else if (strcpy(szTemp.c, "RiseRiseRise") == 0)
	{
		strcpy(szVendor, "Rise");
	}
	else if (strcpy(szTemp.c, "GenuineTMx86") == 0)
	{
		strcpy(szVendor, "Transmeta");
	}
	else
	{
		strcpy(szVendor, szTemp.c);
	}
}

void CPUTFMS(int *pnType, int *pnFamily, int *pnModel, int *pnStepping)
{
	DWORD dwTemp;
	_asm
	{
		mov eax, 1;
		cpuid;
		mov dwTemp, eax;
	}
	*pnType = (dwTemp >> 12) & 0x00000003;
	*pnFamily = (dwTemp >> 8) & 0x0000000f;
	if (*pnFamily == 15)
	{
		*pnFamily |= (dwTemp >> 16) & 0x00000ff0;
	}
	*pnModel = (dwTemp >> 4) & 0x0000000f;
	if (*pnModel == 15)
	{
		*pnModel |= (dwTemp >> 12) & 0x000000f0;
	}
	*pnStepping = dwTemp & 0x0000000f;
}

bool CPUCache(int *pnL2, int *pnL3)
{
	DWORD dwTemp;
	union
	{
		BYTE by[16];
		struct
		{
            DWORD dw0;
			DWORD dw1;
			DWORD dw2;
			DWORD dw3;
		}s;
	}uTemp;
	_asm
	{
		xor eax, eax;
		cpuid;
		mov dwTemp, eax;
        mov eax, 2;
		cpuid;
		mov uTemp.s.dw0, eax;
		mov uTemp.s.dw1, ebx;
		mov uTemp.s.dw2, ecx;
		mov uTemp.s.dw3, edx;
	}
	*pnL2 = -1;
	*pnL3 = -1;
	if (dwTemp < 2 || uTemp.by[0] != 1)
	{
        return false;
	}
	bool bNoL2L3 = false;
	for (int i = 1; i < 16; i++)
	{
		int nNewL2 = -1;
		int nNewL3 = -1;
		switch (uTemp.by[i])
		{
		case 0x1a: nNewL2 = 96; break;
		case 0x22: nNewL3 = 512; break;
		case 0x23: nNewL3 = 1024; break;
		case 0x25: nNewL3 = 2048; break;
		case 0x29: nNewL3 = 4096; break;
		case 0x39: nNewL2 = 128; break;
		case 0x3b: nNewL2 = 128; break;
		case 0x3c: nNewL2 = 256; break;
		case 0x40: bNoL2L3 = true; break;
		case 0x41: nNewL2 = 128; break;
        case 0x42: nNewL2 = 256; break;
        case 0x43: nNewL2 = 512; break;
        case 0x44: nNewL2 = 1024; break;
		case 0x45: nNewL2 = 2048; break;
		case 0x79: nNewL2 = 128; break;
        case 0x7a: nNewL2 = 256; break;
        case 0x7b: nNewL2 = 512; break;
        case 0x7c: nNewL2 = 1024; break;
		case 0x7e: nNewL2 = 256; break;
		case 0x81: nNewL2 = 128; break;
        case 0x82: nNewL2 = 256; break;
        case 0x83: nNewL2 = 512; break;
		case 0x84: nNewL2 = 1024; break;
        case 0x85: nNewL2 = 2048; break;
        case 0x86: nNewL2 = 512; break;
		case 0x87: nNewL2 = 1024; break;
		case 0x88: nNewL3 = 2048; break;
		case 0x89: nNewL3 = 4096; break;
		case 0x8a: nNewL3 = 8192; break;
		case 0x8d: nNewL3 = 4096; break;
		}
		if (nNewL2 != -1)
		{
			if (*pnL2 == -1)
			{
				*pnL2 = nNewL2;
			}
			else
			{
				*pnL2 = -2;
			}
			nNewL2 = -1;
		}
		if (nNewL3 != -1)
		{
			if (*pnL3 == -1)
			{
				*pnL3 = nNewL3;
			}
			else
			{
				*pnL3 = -2;
			}
			nNewL3 = -1;
		}
	}
	if (bNoL2L3)
	{
        if (*pnL2 == -1)
		{
			*pnL2 = 0;
		}
        else if (*pnL3 == -1)
		{
			*pnL3 = 0;
		}
        else
		{
			*pnL2 = -2;
			*pnL3 = -2;
		}
	}
	if (*pnL2 == -2 || *pnL3 == -2)
	{
		return false;
	}

	if (*pnL2 == -1)
	{
		*pnL2 = 0;
	}
	if (*pnL3 == -1)
	{
		*pnL3 = 0;
	}
    return true;
}

int CPUBrand()
{
	DWORD dwTemp;
	_asm
	{
		mov eax, 1;
		cpuid;
		mov dwTemp, ebx;
	}
	return dwTemp & 0x000000ff;
}

inline unsigned __int64 RDTSC()
{
	_asm rdtsc;
}

int CPUSpeed()
{
	__try
	{
		_asm rdtsc;
    }
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return 0;
	}
    
	__int64 qwBegin, qwEnd;
	DWORD dwBegin, dwEnd;
	dwBegin = timeGetTime();
	qwBegin = RDTSC();
	while (timeGetTime() < dwBegin + 1000);
	dwEnd = timeGetTime();
	qwEnd = RDTSC();
    
	return (int)((qwEnd - qwBegin) / (dwEnd - dwBegin) / 1000);
}

void IntelCPU(char *szName, int nType, int nFamily, int nModel, int nStepping, int nL2, int nL3, int nBrand)
{
	switch (nFamily)
	{
	case 5:
		switch (nModel)
		{
		case 0:
			strcpy(szName, "Pentium A-Step");
			break;
		case 1:
            strcpy(szName, "Pentium");
			break;
		case 2:
		case 7:
			strcpy(szName, "Pentium P54C");
			break;
		case 3:
			strcpy(szName, "Pentium P24T");
			break;
		case 4:
		case 5:
		case 6:
		case 8:
			strcpy(szName, "Pentium MMX");
			break;
		default:
            sprintf(szName, "Pentium Model %d\0", nModel);
		}
		break;
	case 6:
		switch (nModel)
		{
		case 0:
		case 1:
			strcpy(szName, "Pentium Pro");
			break;
		case 3:
		case 4:
			strcpy(szName, "Pentium II");
			break;
		case 5:
			if (nL2 == 0)
			{
				strcpy(szName, "Celeron");
			}
			else
			{
				strcpy(szName, "Pentium II / Xeon");
			}
			break;
		case 6:
			if (nL2 == 0)
			{
				strcpy(szName, "Celeron");
			}
			else if (nL2 <= 128)
			{
				strcpy(szName, "Celeron A");
			}
			else
			{
				strcpy(szName, "Pentium II PE");
			}
			break;
		case 7:
		case 8:
			if (nBrand == 1 || nBrand == 7 || (nBrand == 3 && nL2 < 256))
			{
				strcpy(szName, "PIII Celeron");
			}
			else if (nBrand == 3)
			{
                strcpy(szName, "PIII Xeon");
			}
			else
			{
				strcpy(szName, "Pentium III");
			}
			break;
		case 9:
			strcpy(szName, "PIII-M");
			break;
		case 0x0a:
			strcpy(szName, "PIII Xeon");
			break;
		case 0x0b:
			if (nBrand == 1 || nBrand == 7 || nBrand == 3)
			{
				strcpy(szName, "PIII Celeron");
			}
			else if (nBrand == 0x16)
			{
				strcpy(szName, "PIII-M");
			}
			else
			{
				strcpy(szName, "Pentium III");
			}
			break;
		default:
			sprintf(szName, "P6 Model %d\0", nModel);
		}
		break;
	case 15:
		switch (nBrand)
		{
		case 0x08:
		case 0x0f:
			if (nL2 < 256)
			{
				strcpy(szName, "P4 Celeron");
			}
			else
			{
				strcpy(szName, "Pentium 4");
			}
			break;
		case 0x0b:
			if (nModel == 1 && nStepping == 1)
			{
				strcpy(szName, "P4 Xeon MP");
			}
			else
			{
				strcpy(szName, "P4 Xeon");
			}
			break;
		case 0x0e:
			strcpy(szName, "Pentium 4 / Xeon");
			break;
		case 0x0a:
			strcpy(szName, "P4 Celeron");
			break;
		case 0x0c:
			strcpy(szName, "P4 Xeon MP");
			break;
		default:
			strcpy(szName, "Pentium 4");
		}
		break;
	default:
        sprintf(szName, "Family %d Model %d\0", nFamily, nModel);
	}
}

void AMDCPU(char *szName, int nType, int nFamily, int nModel, int nStepping)
{
	switch (nFamily)
	{
	case 5:
		switch (nModel)
		{
		case 0:
			strcpy(szName, "5k86");
			break;
		case 1:
		case 2:
		case 3:
            strcpy(szName, "K5");
			break;
		case 6:
		case 7:
            strcpy(szName, "K6");
			break;
		case 8:
			strcpy(szName, "K6-2");
			break;
		case 9:
			strcpy(szName, "K6-3");
			break;
		case 0x0d:
			strcpy(szName, "K6-2+ / K6-3+");
			break;
		default:
            sprintf(szName, "K5 / K6 Model %d\0", nModel);
		}
		break;
	case 6:
		switch (nModel)
		{
		case 1:
		case 2:
		case 4:
		case 6:
		case 8:
		case 0x0a:
			strcpy(szName, "Athlon");
			break;
		case 3:
		case 7:
			strcpy(szName, "Duron");
			break;
		default:
            sprintf(szName, "K7 Model %d\0", nModel);
		}
		break;
	case 15:
		if (nModel == 5)
		{
			strcpy(szName, "Opteron");
		}
		else
		{
            sprintf(szName, "K8 Model %d\0", nModel);
		}
		break;
	default:
        sprintf(szName, "Family %d Model %d\0", nFamily, nModel);
	}
}

void TDU_CPUInfo(char *szCPU)
{
	DWORD_PTR tam = SetThreadAffinityMask(GetCurrentThread(), 1);

	if (!IfCPUID())
	{
		strcpy(szCPU, "Î´Öª");
	}
	else
	{
        int nType, nFamily, nModel, nStepping;
		CPUVendor(szCPU + strlen(szCPU));
		CPUTFMS(&nType, &nFamily, &nModel, &nStepping);
        if (strcmp(szCPU, "Intel") == 0)
		{
			int nL2, nL3, nBrand;
			CPUCache(&nL2, &nL3);
			nBrand = CPUBrand();
			strcat(szCPU, " ");
			IntelCPU(szCPU + strlen(szCPU), nType, nFamily, nModel, nStepping, nL2, nL3, nBrand);
		}
		else if (strcmp(szCPU, "AMD") == 0)
		{
			strcat(szCPU, " ");
			AMDCPU(szCPU + strlen(szCPU), nType, nFamily, nModel, nStepping);
		}
		else
		{
			sprintf(szCPU + strlen(szCPU), "Family %d Model %d\0", nFamily, nModel);
		}
	}

	strcat(szCPU, "£¬Ö÷Æµ£º");
	int nSpeed = CPUSpeed();
	if (nSpeed != 0)
	{
		if (nSpeed >= 1000)
		{
			sprintf(szCPU + strlen(szCPU), "%.2fGHz\0", (float)nSpeed / 1000.0f);
		}
		else
		{
            sprintf(szCPU + strlen(szCPU), "%dMHz\0", nSpeed);
		}
	}
	else
	{
		strcat(szCPU, "Î´Öª");
	}

	SetThreadAffinityMask(GetCurrentThread(), tam);
}

void TDU_OSInfo(char *szOS)
{
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(osvi));
	osvi.dwOSVersionInfoSize = sizeof(osvi);
    GetVersionEx(&osvi);
	
	sprintf(szOS, "Windows %d.%d\0", osvi.dwMajorVersion, osvi.dwMinorVersion);
	if (osvi.dwPlatformId == VER_PLATFORM_WIN32s)
	{
		sprintf(szOS, "Win32s on Windows %d.%d\0", osvi.dwMajorVersion, osvi.dwMinorVersion);
	}
	else if (osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	{
		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
		{
			strcpy(szOS, "Windows 95");
		}
		else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
		{
			strcpy(szOS, "Windows 98");
		}
		else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
		{
			strcpy(szOS, "Windows ME");
		}
	}
	else if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		if (osvi.dwMajorVersion < 5)
		{
            sprintf(szOS, "Windows NT %d.%d\0", osvi.dwMajorVersion, osvi.dwMinorVersion);
		}
		else if (osvi.dwMajorVersion == 5)
		{
			if (osvi.dwMinorVersion == 0)
			{
				strcpy(szOS, "Windows 2000");
			}
			else if (osvi.dwMinorVersion == 1)
			{
				strcpy(szOS, "Windows XP");
			}
			else if (osvi.dwMinorVersion == 2)
			{
				strcpy(szOS, "Windows Server 2003 family");
			}
		}
	}
	strcat(szOS, " ");
	strcat(szOS, osvi.szCSDVersion);
}

void TDU_MemoryInfo(char *szMemory)
{
	MEMORYSTATUS ms;
	GlobalMemoryStatus(&ms);
	sprintf(szMemory, "%.1f MB\0", (float)ms.dwTotalPhys / 1024.0f / 1024.0f);
	strcat(szMemory, "£¬ÐéÄâÄÚ´æ£º");
	sprintf(szMemory + strlen(szMemory), "%.1f MB\0", (float)ms.dwTotalVirtual / 1024.0f / 1024.0f);	
}

void TDU_AdapterInfo(char *szDevice, char *szDriver)
{
	IDirect3D8 *lpD3D = NULL;
	D3DADAPTER_IDENTIFIER8 d3dad;
	if ((lpD3D = Direct3DCreate8(D3D_SDK_VERSION)) != NULL
		&& D3D_OK == lpD3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &d3dad))
	{
        strcpy(szDevice, d3dad.Description);
		sprintf(szDriver, "%s£¬ °æ±¾ºÅ£º%d\0", d3dad.Driver, d3dad.DriverVersion.LowPart & 0x0000ffff);
	}
	else
	{
		strcpy(szDevice, "Î´Öª");
		strcpy(szDriver, "Î´Öª£¬°æ±¾ºÅ£ºÎ´Öª");
	}
	if (lpD3D != NULL)
	{
		lpD3D->Release();
	}

	strcat(szDevice, "£¬ÏÔ´æ£º");
	IDirectDraw7 *lpDD = NULL;
	DDSCAPS2 DDSCaps;
	ZeroMemory(&DDSCaps, sizeof(DDSCaps));
    DDSCaps.dwCaps = DDSCAPS_VIDEOMEMORY;
    DWORD dwTotal, dwFree;
	if (DD_OK == DirectDrawCreateEx(NULL, (LPVOID*)&lpDD, IID_IDirectDraw7, NULL)
		&& DD_OK == lpDD->GetAvailableVidMem(&DDSCaps, &dwTotal, &dwFree))
	{
        sprintf(szDevice + strlen(szDevice), "%.1f MB\0", (float)dwTotal / 1024.0f / 1024.0f);
	}
	else
	{
	}
	if (lpDD != NULL)
	{
		lpDD->Release();
	}
}


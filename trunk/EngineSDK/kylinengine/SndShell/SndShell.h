
#pragma once

//�����ṩ���������Žӿ�,��������ԴHANDLE
typedef int (__stdcall* FUNC_PLAYSOUND)(const char* szSoundFile, float* fvPos, bool bLoop);
//�����ṩ������ֹͣ�ӿ�
typedef VOID (__stdcall* FUNC_STOPSOUND)(int);



bool __stdcall SSInit(const char* szSndPath, const char* szDBCFile);

bool __stdcall SSEnumSoundFile(int nIndex, char* szSoundFileBuf, int nBufSize, int& id);

int __stdcall SSPlaySoundFunc(const char* szSoundFile, float* fvPos, bool bLoop);

void __stdcall  SSStopSoundFunc(int nHandle);

void __stdcall  SSDeInit(void);

void __stdcall  SSSetSoundVolume(int handle, float gain);
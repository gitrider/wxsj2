#include "StdAfx.h"
#include "AXPPackFile.h"

namespace AXP
{

//hash��������
enum HASH_TYPE
{
	HT_A = 1,		//A ���㷽��
	HT_B,			//B ���㷽��
	HT_OFFSET,		//Offset ���㷽��

	HT_NUMBER,
};

//����hash��������ݱ�
unsigned int g_MathDataBuf[0X100*HT_NUMBER] = {0};

/**********************************************
����һ���ַ�����Hashֵ
***********************************************/
unsigned int _hashString(HASH_TYPE type, const char *pszString)
{
	unsigned int seed1 = 0x7FED7FED;
	unsigned int seed2 = 0xEEEEEEEE;

	while (*pszString != 0) 
	{
		unsigned int ch = *pszString++;
	
		seed1 = g_MathDataBuf[(((int)type)<<8) + ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
	}
	
	return seed1;
}

/**********************************************
	 �����ַ����õ�λ�ã���������ڣ�����-1
***********************************************/
int PackFile::getStringPosInHashTable(const char* szString) const
{
	assert(szString);
	if(!szString || szString[0]==0) return -1;

	unsigned int nHash = _hashString(HT_OFFSET, szString);		// for pos

	unsigned int nHashA = _hashString(HT_A, szString);	// for check
	unsigned int nHashB = _hashString(HT_B, szString);	// for check again

	unsigned int nHashStart = nHash%HASH_TABLE_SIZE;
	register unsigned int nHashPos = nHashStart;

	do
	{
		const FILE_HASHNODE& hashNode = m_hashTable[nHashPos];
	
		//У��ֵ��ȷ
		if(getHashNodeExists(hashNode) && hashNode.nHashA == nHashA && hashNode.nHashB == nHashB) 
			return (int)nHashPos;

		//������һ��
		nHashPos = (nHashPos+1)%HASH_TABLE_SIZE;
		if(nHashPos == nHashStart)break;	//�ص������,�˳�,��ֹ��ѭ��

	}while(true);

	// not find
	return -1;
}

/**********************************************
��Hash���д���һ���ַ�����λ�ã�
����Ѿ����ڣ���ᱻ�滻������Ҳ���λ�ã�����-1
***********************************************/
int PackFile::allocStringPosInHashTable(const char* szString)
{
	assert(szString);
	if(!szString || szString[0]==0 ) return -1;

	//�鿴�Ƿ��Ѿ�����
	int nOldPos = getStringPosInHashTable(szString);
	if(nOldPos >= 0) return nOldPos;

	unsigned int nHash = _hashString(HT_OFFSET, szString);		// for pos

	unsigned int nHashA = _hashString(HT_A, szString);	// for check
	unsigned int nHashB = _hashString(HT_B, szString);	// for check again

	unsigned int nHashStart = nHash%HASH_TABLE_SIZE;
	register unsigned int nHashPos = nHashStart;

	do
	{
		FILE_HASHNODE& hashNode = m_hashTable[nHashPos];

		//��λ��Ϊ��
		if(!getHashNodeExists(hashNode)) 
		{
			hashNode.nHashA = nHashA;
			hashNode.nHashB = nHashB;
			return (int)nHashPos;
		}

		//������һ��
		nHashPos = (nHashPos+1)%HASH_TABLE_SIZE;
		if(nHashPos == nHashStart)break;	//�ص������,�˳�,��ֹ��ѭ��

	}while(true);

	//Hash���Ѿ�����
	return -1;
}

/**********************************************
	hash����Զ���ʼ��
***********************************************/
struct MATH_TABLE_AUTO_INIT
{
	MATH_TABLE_AUTO_INIT()
	{
		unsigned int seed   = 0x00100001;

		memset(g_MathDataBuf, 0, sizeof(g_MathDataBuf));

		for(int index1 = 0; index1 < 0x100; index1++) 
		{
			int index2 = index1;
			for(int i = 0; i<HT_NUMBER; i++, index2+=0x100) 
			{
				unsigned int temp1, temp2;
				seed  = (seed * 125 + 3) % 0x2AAAAB;
				temp1 = (seed & 0xFFFF) << 0x10;

				seed  = (seed * 125 + 3) % 0x2AAAAB;
				temp2 = (seed & 0xFFFF);

				g_MathDataBuf[index2] = (temp1 | temp2);
			}
		}
	}
}_MATH_TABLE_AUTO_INIT;

}
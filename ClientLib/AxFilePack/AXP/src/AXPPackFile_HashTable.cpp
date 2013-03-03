#include "StdAfx.h"
#include "AXPPackFile.h"

namespace AXP
{

//hash计算类型
enum HASH_TYPE
{
	HT_A = 1,		//A 计算方法
	HT_B,			//B 计算方法
	HT_OFFSET,		//Offset 计算方法

	HT_NUMBER,
};

//用于hash计算的数据表
unsigned int g_MathDataBuf[0X100*HT_NUMBER] = {0};

/**********************************************
计算一个字符串的Hash值
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
	 根据字符串得到位置，如果不存在，返回-1
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
	
		//校验值正确
		if(getHashNodeExists(hashNode) && hashNode.nHashA == nHashA && hashNode.nHashB == nHashB) 
			return (int)nHashPos;

		//查找下一个
		nHashPos = (nHashPos+1)%HASH_TABLE_SIZE;
		if(nHashPos == nHashStart)break;	//回到了起点,退出,防止死循环

	}while(true);

	// not find
	return -1;
}

/**********************************************
在Hash表中创建一个字符串的位置，
如果已经存在，则会被替换，如果找不到位置，返回-1
***********************************************/
int PackFile::allocStringPosInHashTable(const char* szString)
{
	assert(szString);
	if(!szString || szString[0]==0 ) return -1;

	//查看是否已经存在
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

		//该位置为空
		if(!getHashNodeExists(hashNode)) 
		{
			hashNode.nHashA = nHashA;
			hashNode.nHashB = nHashB;
			return (int)nHashPos;
		}

		//查找下一个
		nHashPos = (nHashPos+1)%HASH_TABLE_SIZE;
		if(nHashPos == nHashStart)break;	//回到了起点,退出,防止死循环

	}while(true);

	//Hash表已经满了
	return -1;
}

/**********************************************
	hash表的自动初始化
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
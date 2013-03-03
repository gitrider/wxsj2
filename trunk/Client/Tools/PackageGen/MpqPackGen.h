
/**	MpqPackGen.h
 *	
 *	���ܣ�	
 *
 *	�޸ļ�¼��
 *			
 *			
 */

#ifndef MPQ_PACK_GEN_H
#define MPQ_PACK_GEN_H


#include <io.h>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <windows.h>
#include <shlwapi.h>
#include "kpmLib.h"

using namespace KPM;
using namespace std;

struct _MPQPackInfo
{
	string				strPackName;
	vector<string>		vecPackPath;		//һ�����е��ļ������ڲ�ͬ·����
	vector<string>		vecExt;				//��Ӧ��������չ�ļ�
};

const string PACK_EXT = ".mpq";				//����չ��


class CMpqPackGen
{
public:

	CMpqPackGen(void);
	~CMpqPackGen(void);

	void SetPackPotion(int val);
	void ShowPackInfo();
	void GetConfigInfo();
	void DoPacking();

	//pArchive:  MPQ��ָ��  strDir��ԭ��ԴĿ¼   VecExt����չ������
	int BrowseDirectory(TKPMArchive* pArchive,string  &strDir,vector<string> &VecExt,bool IsPack = true);

private:

	int						m_iPackTotal;	
	int						m_iPackPotion;	//�û������Ĵ��ѡ��,0Ϊȫ��,Ĭ��Ϊ0
	string					m_strBasePath;	//��Դ��Ŀ¼
	vector<_MPQPackInfo>	m_vecPackInfo;
	map<string,int>			m_mapCompressType;

	FILE*					m_pGenLogFile;
	FILE*					m_pErrorLogFile;
	int						m_iFileNum;

	vector<string>			m_vFilePath;
	BOOL					m_bTestVersion;	//�Ƿ�Ϊ�ڲ����԰�,Ĭ�ϲ���
//	vector<string>			m_vFileExt;

};

#endif
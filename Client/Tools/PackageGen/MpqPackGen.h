
/**	MpqPackGen.h
 *	
 *	功能：	
 *
 *	修改记录：
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
	vector<string>		vecPackPath;		//一个包中的文件可能在不同路径下
	vector<string>		vecExt;				//对应需打包的扩展文件
};

const string PACK_EXT = ".mpq";				//包扩展名


class CMpqPackGen
{
public:

	CMpqPackGen(void);
	~CMpqPackGen(void);

	void SetPackPotion(int val);
	void ShowPackInfo();
	void GetConfigInfo();
	void DoPacking();

	//pArchive:  MPQ包指针  strDir：原资源目录   VecExt：扩展名集合
	int BrowseDirectory(TKPMArchive* pArchive,string  &strDir,vector<string> &VecExt,bool IsPack = true);

private:

	int						m_iPackTotal;	
	int						m_iPackPotion;	//用户所做的打包选项,0为全打,默认为0
	string					m_strBasePath;	//资源根目录
	vector<_MPQPackInfo>	m_vecPackInfo;
	map<string,int>			m_mapCompressType;

	FILE*					m_pGenLogFile;
	FILE*					m_pErrorLogFile;
	int						m_iFileNum;

	vector<string>			m_vFilePath;
	BOOL					m_bTestVersion;	//是否为内部测试版,默认不是
//	vector<string>			m_vFileExt;

};

#endif
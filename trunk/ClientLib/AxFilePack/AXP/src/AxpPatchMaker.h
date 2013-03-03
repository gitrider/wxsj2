/*
		Axia|FilePack
			补丁制作者
*/

#pragma once

#include "AXP.h"

namespace AXP
{

//补丁制作者，用于根据两个版本之间差异制作补丁
class PatchMaker : public IPatchMaker
{
public:
	/********************************************
					外部接口
	*********************************************/
	static const char* VERSION_FILENAME;	//!< 版本文件名
	static const char* PATCH_COMMAND;		//!< 版本更新命令文件

	//分析目录, 如果参数szVersion存在，则会重新创建版本文件"(version)"
	virtual bool analysePath(const char* szPath, const char* szVersion);

	//得到某个目录的版本，该目录必须经过分析，否则会返回 ""
	virtual const char* getPathVersion(const char* szPath);

	//根据两个目录的分析结果，产生版本差异Patch
	virtual bool generatePatch(const char* szOldVersionPath, const char* szNewVersionpath, 
					const char* szPatchFile, AXP_PATCHMAKER_CALLBACK callBackFunc);

public:
	/********************************************
					构造/析构
	*********************************************/
	PatchMaker();
	virtual ~PatchMaker();

protected:
	/********************************************
					内部数据
	*********************************************/
	
	//文件描述
	struct FILE_INFO
	{
		std::string		strFile;		//文件名,相对于版本目录
		unsigned int	nFileSize;		//文件大小(bytes)
		unsigned int	nCRC;			//文件校验码
		std::string		strPakName;		//所属的axp包的名称，如果是散文件，则为空
		bool			bNeedDel;		//需要删除，在做Patch中使用
	};
	typedef std::vector< FILE_INFO > FILE_INFO_COLLECT;

	//pak包集合
	typedef std::set< std::string > PAKFILE_SET;

	/// 一个完整的版本目录数据
	struct VERSION_INFO
	{
		std::string			strVersion;		//版本号
		std::string			strPath;		//所在的磁盘目录
		FILE_INFO_COLLECT	theFiles;		//文件信息
		PAKFILE_SET			pakFiles;		//pak文件名集合
	};

	//已经分析过的目录数据, key=目录名(经过规范化)
	typedef std::map< std::string, VERSION_INFO > VERSION_INFO_COLLECT;
	VERSION_INFO_COLLECT	m_verCollects;

	//版本比较数据
	struct PATCH_FILE
	{
		UPDATE_PROCESS	process;		//文件操作类型
		std::string		strPakFileName;	//对应的包文件
		std::string		strFileName;	//对应的文件
		std::string		strSourcePath;	//新版本文件在磁盘上的路径
	};
	typedef std::vector< PATCH_FILE > PATCH_FILES;

	//一个完整的版本差异
	struct PATCH_INFO
	{
		std::string strOldVersion;
		std::string strNewVersion;
		PATCH_FILES	patchFiles;
	};

protected:
	/********************************************
					内部接口
	*********************************************/
	
	//通过Version文件收集数据
	bool _collectFromVersionFile(const char* szVersionFile, VERSION_INFO& verInfo);
	//通过目录收集数据,并且产生新的version文件
	bool _collectFromPath(const char* szPath, const char* szVersionFile, const char* szVersion, VERSION_INFO& verInfo);
	//递归收集路径
	bool _collectFromPathRecursive(const char* szPath, const char* szPathRelative, VERSION_INFO& verInfo);
	//加入单个文件,如果该文件为打包文件,则会加入其中的小文件
	bool _insertFile(const char* szFileName, const char* szRelativeName, VERSION_INFO& verInfo);


	//key=(pakName, fileName), value=index
	typedef std::map< std::pair< std::string, std::string >, unsigned int > FILE_SEARCH_MAP;
	//生成文件查找索引
	void _generateSearchMap(const VERSION_INFO& verInfo, FILE_SEARCH_MAP& mapSearch);
	//生成Patch包
	bool _generatePatchZip(const PATCH_INFO& patchInfo, const char* szPatchFile, AXP_PATCHMAKER_CALLBACK callBackFunc);
	//将Pak包中的文件加入到zip中
	bool _insertFileInPakToZip(const char* szPakFile, const char* szFileName, const char* szFileInZip, void* hZip);

};

}
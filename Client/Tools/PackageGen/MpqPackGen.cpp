//#include "StdAfx.h"
#include <iostream>
#include <tchar.h>
#include "mpqpackgen.h"

//HashTable上下限
#define MIN_HASH_TABLE_SIZE	100
#define MAX_HASH_TABLE_SIZE	30000


CMpqPackGen::CMpqPackGen(void)
{
	m_iPackTotal = 0;
	m_iPackPotion = 0;
	m_strBasePath = "";
	m_vecPackInfo.clear();

	m_pGenLogFile	= NULL;
	m_pErrorLogFile = NULL;
	m_iFileNum = 0;
	m_bTestVersion = FALSE;
}

CMpqPackGen::~CMpqPackGen(void)
{

	m_vecPackInfo.clear();

	if( m_pGenLogFile )
	{
		fclose( m_pGenLogFile );
		m_pGenLogFile = NULL;
	}
	if( m_pErrorLogFile )
	{
		fclose( m_pErrorLogFile );
		m_pErrorLogFile = NULL;
	}
}

void CMpqPackGen::ShowPackInfo()
{
	for ( int i=0;i<m_iPackTotal;i++ )
	{
		printf( "\n%d:%s",i+1,m_vecPackInfo[i].strPackName.c_str() );
	}
		
	printf( "\n0:All" );
}

void CMpqPackGen::SetPackPotion(int val)
{
	if ( val<0 || val> m_iPackTotal )
		val = 0;
	m_iPackPotion = val;
}


void CMpqPackGen::DoPacking()
{
	// 日志文件
	CHAR szTempFile[MAX_PATH] = {0};
	::GetCurrentDirectory( MAX_PATH, szTempFile );
	::PathAppend( szTempFile, "PackGen.log" );

	m_pGenLogFile = fopen( szTempFile, "w" );
	if( !m_pGenLogFile )
	{
		char szMsg[ MAX_PATH ];
		_snprintf(szMsg, MAX_PATH, "日志文件%s创建失败！", szTempFile);
		MessageBox(NULL, szMsg, "ERROR", MB_OK|MB_ICONSTOP);

		return;
	}

	CHAR szTempErrFile[MAX_PATH] = {0};
	::GetCurrentDirectory( MAX_PATH, szTempErrFile );
	::PathAppend( szTempErrFile, "PackGenError.log" );
	m_pErrorLogFile = fopen( szTempErrFile, "w" );
	if( !m_pErrorLogFile )
	{
		char szMsg[ MAX_PATH ];
		_snprintf(szMsg, MAX_PATH, "文件错误日志 %s创建失败！", szTempFile);
		MessageBox(NULL, szMsg, "ERROR", MB_OK|MB_ICONSTOP);

		return;
	}

	for( int i=0;i<m_iPackTotal;i++ )
	{
		if ( m_iPackPotion == 0 || m_iPackPotion == i+1 )
		{
			int FileNum = 0;
			//统计文件个数，以决定HashTable大小
			vector<string>::iterator it = m_vecPackInfo[i].vecPackPath.begin();
			for ( ;it!= m_vecPackInfo[i].vecPackPath.end();it++ )
			{
				int num = BrowseDirectory(NULL,*it, m_vecPackInfo[i].vecExt,false);			
				FileNum += num;
			}	

			FileNum = FileNum * 1.5;	//Hash表大小是文件实际个数的1.5倍

			if ( FileNum<MIN_HASH_TABLE_SIZE )
				FileNum = MIN_HASH_TABLE_SIZE;

			if (FileNum>MAX_HASH_TABLE_SIZE)
			{
				MessageBox(NULL, "Hash表太大，请检查文件个数", "ERROR", MB_OK|MB_ICONSTOP);
				return;
			}

			DWORD R = 0;
			string PackName = m_vecPackInfo[i].strPackName + ".mpq";
			TKPMArchive* pArchive = KPM_CreateArchive( PackName.c_str(), R,FileNum , USE_TYPE_PATCH);
			
			printf( "\n ==============================" );
			printf( "\n PackName: %s", PackName.c_str() );
			printf( "\n" );

			it = m_vecPackInfo[i].vecPackPath.begin();
			for ( ;it!= m_vecPackInfo[i].vecPackPath.end();it++ )
			{
				int num = BrowseDirectory(pArchive,*it, m_vecPackInfo[i].vecExt,true);			
				FileNum += num;
			}		

			KPM_CloseArchive(pArchive);
		}
	}

	printf( "\n PackGen OK！" );

}//DoPacking end


//递归遍历文件夹
//pArchive:  MPQ包指针  strDir：原资源目录   VecExt：扩展名集合
int CMpqPackGen::BrowseDirectory(TKPMArchive* pArchive,string  &strDir,vector<string> &VecExt,bool IsPack)
{
	int			FileNum = 0;
   	intptr_t	ptrFile;  
	struct		_finddata_t file;

	string		strFindTemp = "";
	if ( strDir == "./" || strDir =="/" || strDir == "../" )
		strFindTemp = strDir + "*.*";
	else
		strFindTemp = strDir + "/*.*";

	if ( strDir == "./" || strDir =="/" )
		strDir = ".";
	if ( strDir == "../" )
		strDir = "..";

	bool		bAddFileTag = false;

	if( (ptrFile = _findfirst( strFindTemp.c_str(), &file )) != -1L )
	{	
		while( _findnext( ptrFile, &file ) == 0 )
		{	
			if (file.name[0] == '.')
				continue;					//忽略

			_mbslwr((byte*)file.name);
			string FilePath = strDir + "/" + file.name;

			if( PathIsDirectory( FilePath.c_str() ) )
			{
				//如果是目录,进入此目录进行递归
				FileNum = FileNum + BrowseDirectory(pArchive,strDir+"/"+file.name,VecExt,IsPack);
			}
			else
			{

				//得到文件扩展名
				string::size_type ExtPos = FilePath.find_last_of(".");
				string::size_type TmpPos = FilePath.find_last_of("/");

				if ( ExtPos != string::npos )
				{
					string Ext = FilePath.substr(ExtPos,FilePath.size()-ExtPos);
					if (Ext == ".exe")
					{
						//
					}
									
					string tmp = FilePath.substr(TmpPos+1,tmp.size()-TmpPos);
					//转为小写

				
					if ( find(VecExt.begin(),VecExt.end(),Ext) != VecExt.end() || VecExt.at(0) == "*.*" ||
						 find(VecExt.begin(),VecExt.end(),tmp) != VecExt.end()  )
					{
						//指定KPM_FILE_IMPLODE/KPM_FILE_COMPRESS：表示使用压缩算法（同时指定速度较慢）
						//遇有已压缩的数据时，不要指定KPM_FILE_IMPLODE/KPM_FILE_COMPRESS
						//KPM_FILE_ENCRYPTED:表示加密
						//KPM_FILE_REPLACEEXISTING:表示有同名文件时替换,有同名文件但未指定此参数，则返回错误
						//FILE_TYPE_DATA:目前只支持两种格式：普通数据/WAVE数据，今后需扩展
						DWORD Flags = KPM_FILE_COMPRESS;
						DWORD FileType = FILE_TYPE_DATA;

						if( ".wav" == Ext )
						{
							FileType =  FILE_TYPE_WAVE;
						}
						else if( ".mp3" == Ext || 
							     ".zip" == Ext ||
								 ".rar" == Ext ||
								 ".mpq" == Ext ||
								 ".jpg" == Ext ||
								 ".dds" == Ext
								 )
						{
							Flags &= ~KPM_FILE_COMPRESS;
						}
						else if( ".db" == Ext )
						{
							continue;
						}


						FileNum++;

						if ( IsPack && pArchive )
						{
							m_iFileNum++;

							if ( !m_bTestVersion )		
							{
								//最终的发行版不带路径信息
								bAddFileTag = KPM_AddFile( pArchive, FilePath.c_str(), file.name, Flags, 0, FileType );
							}
							else							
							{		
								//内部调试版带路径信息
								bAddFileTag = KPM_AddFile( pArchive, FilePath.c_str(), FilePath.c_str(), Flags, 0, FileType );
							}

							if( bAddFileTag )
							{
								printf( "\n AddFile PackPath: %s", FilePath.c_str() );
								fprintf( m_pGenLogFile, "%d: AddFile ( %s ) \n", m_iFileNum, FilePath.c_str() );

								m_vFilePath.push_back( FilePath );

							}
							else
							{
								string str = "无";
								vector<string>::iterator it = m_vFilePath.begin();
								for( ; it != m_vFilePath.end(); it++ )
								{	
									if( string::npos != (*it).find( file.name ) )
									{
										str = (*it);
										break;
									}
								}

								fprintf( m_pErrorLogFile, "%d: Error ( %s 已经存在 %s ) \n", m_iFileNum, FilePath.c_str(), str.c_str() );

	//							assert(false);
							}						
						}

					}
         		}
			}
			
		}//while end

		//关闭查找句柄
		_findclose( ptrFile );				

	}//if end

	return FileNum;

}//BrowseDirectory end


void CMpqPackGen::GetConfigInfo()
{
	m_iPackTotal = ::GetPrivateProfileInt("PackInfo","PackTotal",0,"./PackageGenConfig.ini");
	for ( int i=1;i<=m_iPackTotal;i++ )
	{
		char Ext[10] =  {0};
		char Name[10] = {0};
		char Path[10] = {0};
		char Temp[500] = {0};

		_MPQPackInfo	 info;

		sprintf(Name,"Name%d",i);
		sprintf(Path,"Path%d",i);
		sprintf(Ext,"Ext%d",i);

		::GetPrivateProfileString("PackInfo",Name,NULL,Temp,500,"./PackageGenConfig.ini");
		_mbslwr((byte*)Temp);
		info.strPackName = Temp;

		::GetPrivateProfileString("PackInfo",Path,NULL,Temp,500,"./PackageGenConfig.ini");
		_mbslwr((byte*)Temp);
		string path = Temp;
		string::size_type tmpPos;
		while ( ( tmpPos = path.find(" ") )!= string::npos )
		{
			path.erase(tmpPos, 1);
		}
		
		replace(path.begin(),path.end(),'\\','/');

		string::size_type pos = path.find("&");
		while ( pos != string::npos )				//对应多个路径
		{
			string PathPart = path.substr(0,pos);
			info.vecPackPath.push_back(PathPart);
			path = path.substr(pos+1);
			pos = path.find("&");
		}
		info.vecPackPath.push_back(path);

		::GetPrivateProfileString("PackInfo",Ext,NULL,Temp,500,"./PackageGenConfig.ini");
		_mbslwr((byte*)Temp);
		string ext = Temp ;

		while ( ( tmpPos = ext.find(" ") )!= string::npos )
		{
			ext.erase(tmpPos, 1);
		}

		pos = ext.find("&");
		while ( pos != string::npos )				//对应多个扩展名
		{
			string ExtPart = ext.substr(0,pos);
			info.vecExt.push_back(ExtPart);
			ext = ext.substr(pos+1);
			pos = ext.find("&");
		}
		info.vecExt.push_back(ext);
	
		m_vecPackInfo.push_back(info);
		
	}//for end

   
	int CompressType = ::GetPrivateProfileInt("CompressType",".wav",0,"./PackageGenConfig.ini");
	m_mapCompressType.insert( map<string,int>::value_type(".wav ",CompressType) );

	CompressType = ::GetPrivateProfileInt("CompressType",".zip",0,"./PackageGenConfig.ini");
	m_mapCompressType.insert( map<string,int>::value_type(".zip",CompressType) );

	CompressType = ::GetPrivateProfileInt("CompressType",".rar",0,"./PackageGenConfig.ini");
	m_mapCompressType.insert( map<string,int>::value_type(".rar",CompressType) );

	CompressType = ::GetPrivateProfileInt("CompressType",".mpq",0,"./PackageGenConfig.ini");
	m_mapCompressType.insert( map<string,int>::value_type(".mpq",CompressType) );

	CompressType = ::GetPrivateProfileInt("CompressType","*.*",0,"./PackageGenConfig.ini");
	m_mapCompressType.insert( map<string,int>::value_type("*.*",CompressType) );

	m_bTestVersion = ::GetPrivateProfileInt("VersionType","Version",0,"./PackageGenConfig.ini");

}//GetConfigInfo end



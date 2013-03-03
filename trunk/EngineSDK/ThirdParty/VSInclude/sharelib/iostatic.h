#ifndef __IO_STATIC_H__
#define __IO_STATIC_H__

/*!
	Author : dfhu
	data: 2002-7
	des: the baic io function wrap
*/

#include <string>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include "iostatic_func.h"

#ifdef WIN32
#include <windows.h>
#include <direct.h>
#include "winver.h"
#include <tchar.h>
#include <io.h>
#define mkdir(a,b) mkdir(a)

#define S_IRUSR 0
#define S_IWUSR 1
#define S_IXUSR 2
#define S_IRGRP 4
#define S_IWGRP 8
#define S_IXGRP 16
#define S_IROTH 32
#define S_IWOTH 64
#define S_IXOTH 128
#endif

#pragma warning (disable : 4267 4018)

namespace IO_OP
{
	inline bool Exist(const char *name)
	{
		struct stat buf;
		if(stat(name,&buf)==0)
			return true;
		else
			return false;
	}

	inline bool Exist(std::string &name)
	{
		return Exist(name.c_str());
	}

	inline bool Rename( const char *oldName, const char *newName )
	{
		return rename(oldName, newName ) == 0;
	}

	namespace File
	{
		static inline bool IsFile( const char *name )
		{
			struct stat buf;
			if(stat(name,&buf)==0)
				if(S_IFREG&buf.st_mode)
					return true;
			return false;
		}

		static inline bool Delete(const char *name)
		{
#ifdef WIN32
			return DeleteFile(name)!=0;
#else
			unlink(name);
#endif
		}

		static inline int	GetFileLength( const char *name )
		{
			struct stat buf;
			if(stat(name,&buf)==0)
				return buf.st_size;
			else
				return 0;
		}

		static inline bool Delete(std::string  &name)
		{
			return Delete(name.c_str());
		}

		static inline bool DeleteAlways( const char *name )
		{
			_chmod( name, _S_IREAD | _S_IWRITE );
			return Delete(name);
		}

	

		static inline
			bool	CopyF( const char *src, const char *dst, bool bFailIfExist )
		{
#ifdef _WIN32
			return CopyFile( src, dst, bFailIfExist);
#else
			return false;
#endif
		}
	};

	namespace Path
	{
		static inline bool IsDir(const char *name)
		{
			struct stat buf;
			if(stat(name,&buf)==0)
				if(S_IFDIR&buf.st_mode)
					return true;
			return false;
		}

		static inline bool IsDir(std::string &name)
		{
			return IsDir(name.c_str());
		}


		static inline void Remove(const char *name)
		{
#ifdef WIN32
			RemoveDirectory(name);
#else
			unlink(name);
#endif
		}

	

		static inline void Remove(std::string &sDir)
		{
			Remove(sDir.c_str());
		}


		static inline bool MakeDir(const char *sDir,int priv =  S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH)
		{
			return mkdir(sDir,S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH)==0;
		}

		static inline bool MakeDir(std::string &sDir,int priv =  S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH)
		{
			return mkdir(sDir.c_str(),S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH)==0;
		}

	/*	static bool MakeTree(string &sDir)
		{
			if(Exist(sDir.c_str()))
				return true;
			else
			{
				string sParentDir = sDir;
				for(;;)
				{
					Parent(sParentDir);
					if(!Valid(sParentDir))
						break;
					if(MakeTree(sParentDir))
						break;
				}
				if(Valid(sParentDir))
					return MakeDir(sDir);
				else
					return false;
			}
		}*/

		static inline bool MakeTree(std::string &sDir)
		{
			if(Exist(sDir.c_str()))
				return true;
			else
			{
				std::string sParentDir = sDir;
			//	for(;;)
				{
					if(!Valid( sDir ))
						return false;
					if( MakeDir( sDir ) )
						return true;
					Parent(sParentDir);
					if(MakeTree(sParentDir))
						return MakeDir(sDir);
					else
						return false;
				}
			}
		}

	

		static inline void AllSub(std::string &sDir,std::vector<std::string> &sFileNameArray, bool bFullPathNameReturn=true)
		{
#ifdef WIN32
			std::string sFileName=sDir+"/*.*";
			WIN32_FIND_DATA FindFileData;
			HANDLE hFind=FindFirstFile(sFileName.c_str(),&FindFileData);
			if(hFind==INVALID_HANDLE_VALUE)
				return;

			BOOL bFind=TRUE;//FindNextFile(hFind,&FindFileData);
			while(bFind)
			{
				char *t=FindFileData.cFileName;
				if( bFullPathNameReturn )
					sFileName=sDir+'/'+t;
				else
					sFileName=t;
				sFileNameArray.push_back(sFileName);
				bFind=FindNextFile(hFind,&FindFileData);
			}
			FindClose(hFind);
#else
			std::string sFileName;
			struct dirent * *namelist;
			int n=scandir(sDir.c_str(),&namelist,NULL,alphasort);
			if(n>0)
			{
				while(n--)
				{
					struct dirent *name=namelist[n];
					if( bFullPathNameReturn )
						sFileName=sDir+'/'+name->d_name;
					else
						sFileName = name->d_name;
					sFileNameArray.push_back(sFileName);
					free(name);
				}
				free(namelist);
			}
#endif
		}

		static  inline void AllSubNoExtJudge(std::string &sDir,std::vector<std::string> &sFileNameArray, bool bFullPathNameReturn)
		{
			AllSub( sDir, sFileNameArray, bFullPathNameReturn );
		}

		static inline void AllSubWithSearchSubDir( const char *sSearchDir, const char *sExt, std::vector<std::string> &sFileNameArray )
		{
		#ifdef WIN32
			std::string sDir = sSearchDir;
			NoEndDir( sDir );
			
			std::string sFileName=sDir+"/*";//+sExt;			
			WIN32_FIND_DATA FindFileData;
			HANDLE hFind=FindFirstFile(sFileName.c_str(),&FindFileData);
			if(hFind==INVALID_HANDLE_VALUE)
				return;

			std::vector< std::string > sDirArray;
			BOOL bFind=TRUE;//FindNextFile(hFind,&FindFileData);
			while(bFind)
			{
				char *t=FindFileData.cFileName;

				if( (FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) )
				{
					if( (t[0] == '.' && t[1] == 0)
						|| (t[0] == '.' && t[1] == '.' && t[2] == 0) )
					{
					}
					else
					{
						sFileName=sDir+'/'+t;
						sDirArray.push_back( sFileName );
					}
				}
				else
				{
					bool bExt = false;
					if( !sExt )
						bExt = true;
					else
					{
						const char *ext = IO_OP::File::GetExt( t );
						if( ext && (ext+1) )
						{
							if( stricmp(ext+1,sExt) == 0)
								bExt = true;
						}
					}
					if(bExt )
					{
						sFileName=sDir+'/'+t;
						sFileNameArray.push_back(sFileName);
					}				
				}

				bFind=FindNextFile(hFind,&FindFileData);
			}
			FindClose(hFind);

			for( int i=0; i<sDirArray.size(); ++i )
				AllSubWithSearchSubDir( sDirArray[i].c_str(), sExt, sFileNameArray );
		#endif
		}

		static inline void AllSub(std::string &sDir, std::string &sExt, std::vector<std::string> &sFileNameArray, bool bFullPathNameReturn=true)
		{
#ifdef WIN32
			std::string sFileName=sDir+"/*."+sExt;
			WIN32_FIND_DATA FindFileData;
			HANDLE hFind=FindFirstFile(sFileName.c_str(),&FindFileData);
			if(hFind==INVALID_HANDLE_VALUE)
				return;

			BOOL bFind=TRUE;//FindNextFile(hFind,&FindFileData);
			while(bFind)
			{
				char *t=FindFileData.cFileName;
				if( bFullPathNameReturn )
					sFileName=sDir+'/'+t;
				else
					sFileName = t;
				sFileNameArray.push_back(sFileName);
				bFind=FindNextFile(hFind,&FindFileData);
			}
			FindClose(hFind);
#else
			std::string sFileName;
			struct dirent * *namelist;
			int n=scandir(sDir.c_str(),&namelist,NULL,alphasort);
			if(n>0)
			{
				while(n--)
				{
					struct dirent *name=namelist[n];
					if( strstr(name->d_name, sExt.c_str()) == NULL )
					{
						if( bFullPathNameReturn )
							sFileName=sDir+'/'+name->d_name;
						else
							sFileName = name->d_name;
						sFileNameArray.push_back(sFileName);
					}
					free(name);
				}
				free(namelist);
			}
#endif
		}

		static inline void AllSubWithExtJudge(std::string &sDir,std::string &sExt,std::vector<std::string> &sFileNameArray, bool bFullPathNameReturn)
		{
			AllSub(sDir, sExt, sFileNameArray, bFullPathNameReturn );
		}

		static inline int GetDirFileNumber( std::string &sDir )
		{
#ifdef WIN32
			std::string sFileName=sDir+"/*.*";
			WIN32_FIND_DATA FindFileData;
			HANDLE hFind=FindFirstFile(sFileName.c_str(),&FindFileData);
			if(hFind==INVALID_HANDLE_VALUE)
				return 0;

			int n = 0;
			BOOL bFind=TRUE;//FindNextFile(hFind,&FindFileData);
			while(bFind)
			{
				if( !(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) )
					++n;
				bFind=FindNextFile(hFind,&FindFileData);
			}
			FindClose(hFind);
			return n;
#else
			std::string sFileName;
			struct dirent * *namelist;
			int n=scandir(sDir.c_str(),&namelist,NULL,alphasort);
			return n;
#endif
		}

		static inline int GetDirFileNumber( std::string &sDir, std::string &sExt )
		{
#ifdef WIN32
			std::string sFileName=sDir+"/*."+sExt;
			WIN32_FIND_DATA FindFileData;
			HANDLE hFind=FindFirstFile(sFileName.c_str(),&FindFileData);
			if(hFind==INVALID_HANDLE_VALUE)
				return 0;

			int nNum = 0;
			BOOL bFind=TRUE;//FindNextFile(hFind,&FindFileData);
			while(bFind)
			{
				if( !(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) )
					++nNum;
				bFind=FindNextFile(hFind,&FindFileData);
			}
			FindClose(hFind);
			return nNum;
#else
			int nNum = 0;
			std::string sFileName;
			struct dirent * *namelist;
			int n=scandir(sDir.c_str(),&namelist,NULL,alphasort);
			if(n>0)
			{
				while(n--)
				{
					struct dirent *name=namelist[n];
					if( strstr(name->d_name, sExt.c_str()) == NULL )
					{
						nNum++;
					}
					free(name);
				}
				free(namelist);
			}
			return nNum;
#endif
		}

		static inline void AllSubDir(std::string &sDir,std::vector<std::string> &sDirNameArray, bool bFullPathNameReturn=true)
		{
#ifdef WIN32
			std::string sFileName=sDir;
			EndDir(sFileName);
			sFileName += "*";
			WIN32_FIND_DATA FindFileData;
			HANDLE hFind=FindFirstFile(sFileName.c_str(),&FindFileData);
			if(hFind==INVALID_HANDLE_VALUE)
				return;

			BOOL bFind=TRUE;//FindNextFile(hFind,&FindFileData);
			while(bFind)
			{
				if( (FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) )
				{
					char *t=FindFileData.cFileName;
					if( bFullPathNameReturn )
						sFileName=sDir+'/'+t;
					else
						sFileName = t;
					sDirNameArray.push_back(sFileName);
				}
				bFind=FindNextFile(hFind,&FindFileData);
			}
			FindClose(hFind);
#else
			std::string sFileName;
			struct dirent * *namelist;
			int n=scandir(sDir.c_str(),&namelist,NULL,alphasort);
			if(n>0)
			{
				while(n--)
				{
					struct dirent *name=namelist[n];
				//	if( strstr(name->d_name, sExt.c_str()) == NULL )
					{
						if( bFullPathNameReturn )
							sFileName=sDir+'/'+name->d_name;
						else
							sFileName = name->d_name;
						sDirNameArray.push_back(sFileName);
					}
					free(name);
				}
				free(namelist);
			}
#endif
		}

		static inline void	AllSubFile( std::string &sDir,std::vector<std::string> &sFileNameArray, bool bFullPathNameReturn=true)
		{
	#ifdef WIN32
			std::string sFileName=sDir;
			EndDir(sFileName);
			sFileName += "*.*";
			WIN32_FIND_DATA FindFileData;
			HANDLE hFind=FindFirstFile(sFileName.c_str(),&FindFileData);
			if(hFind==INVALID_HANDLE_VALUE)
				return;

			BOOL bFind=TRUE;//FindNextFile(hFind,&FindFileData);
			while(bFind)
			{
				if( !(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
					&& (!(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DEVICE)) )
				{
					char *t=FindFileData.cFileName;
					if( bFullPathNameReturn )
						sFileName=sDir+'/'+t;
					else
						sFileName = t;
					sFileNameArray.push_back(sFileName);
				}
				bFind=FindNextFile(hFind,&FindFileData);
			}
			FindClose(hFind);
#else
			std::string sFileName;
			struct dirent * *namelist;
			int n=scandir(sDir.c_str(),&namelist,NULL,alphasort);
			if(n>0)
			{
				while(n--)
				{
					struct dirent *name=namelist[n];
				//	if( strstr(name->d_name, sExt.c_str()) == NULL )
					{
						if( bFullPathNameReturn )
							sFileName=sDir+'/'+name->d_name;
						else
							sFileName = name->d_name;
						sFileNameArray.push_back(sFileName);
					}
					free(name);
				}
				free(namelist);
			}
#endif
		}

		static inline void	AllSubDirFile(std::string &sDir,std::vector<std::string> &sDirNameArray, std::vector<std::string> &sFilenameArray, bool bFullPathNameReturn = true )
		{
#ifdef WIN32
			std::string sFileName=sDir;
			EndDir(sFileName);
			sFileName += "*";
			WIN32_FIND_DATA FindFileData;
			HANDLE hFind=FindFirstFile(sFileName.c_str(),&FindFileData);
			if(hFind==INVALID_HANDLE_VALUE)
				return;

			BOOL bFind=TRUE;//FindNextFile(hFind,&FindFileData);
			while(bFind)
			{
				if( (FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) )
				{
					char *t=FindFileData.cFileName;
					if( bFullPathNameReturn )
						sFileName=sDir+'/'+t;
					else
						sFileName = t;
					sDirNameArray.push_back(sFileName);
				}
				else 
				{
					char *t=FindFileData.cFileName;
					if( bFullPathNameReturn )
						sFileName=sDir+'/'+t;
					else
						sFileName = t;
					sFilenameArray.push_back(sFileName);
				}
				bFind=FindNextFile(hFind,&FindFileData);
			}
			FindClose(hFind);
#else
			std::string sFileName;
			struct dirent * *namelist;
			int n=scandir(sDir.c_str(),&namelist,NULL,alphasort);
			if(n>0)
			{
				while(n--)
				{
					struct dirent *name=namelist[n];
				//	if( strstr(name->d_name, sExt.c_str()) == NULL )
					{
						if( bFullPathNameReturn )
							sFileName=sDir+'/'+name->d_name;
						else
							sFileName = name->d_name;
						sDirNameArray.push_back(sFileName);
					}
					free(name);
				}
				free(namelist);
			}
#endif
		}

		static inline void DelTree(std::string &sDir)
		{
			std::vector<std::string> tmpArray;
			AllSub(sDir,tmpArray);
			std::string name;
			_chmod( sDir.c_str(), _S_IREAD | _S_IWRITE );
			for(int i = 0 ;i < tmpArray.size();i++)
			{
				name = tmpArray[i];
				const char *pName = IO_OP::Path::GetName(name.c_str());
				if( pName[0] == '.' && pName[1] == 0 )
					continue;
				if( pName[0] == '.' && pName[2] == 0 && pName[1] == '.' )
					continue;
				if(IsDir(name))
				{
					DelTree(name);
				}
				else
				{
					///
					_chmod( name.c_str(), _S_IREAD | _S_IWRITE );
					File::Delete(name);
				}
			}
			Remove(sDir);
		}



		static inline std::string GetCurrentDir()
		{
			char dirBufL[266];

#if defined(_WIN32)
			GetCurrentDirectory(256, dirBufL);
#else
			getcwd(dirBufL, 256);
#endif
			return std::string(dirBufL);
		}
	};
};

#pragma warning (default  : 4267 4018)
#endif



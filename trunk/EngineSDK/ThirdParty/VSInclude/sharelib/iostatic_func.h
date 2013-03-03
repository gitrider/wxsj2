#ifndef IO_OP_STATIC_FUN_H__
#define IO_OP_STATIC_FUN_H__

#include <string>



namespace IO_OP
{
	namespace File
	{
		static inline
		const char * GetExt( const char * filename )
		{
			const char *p = strlen(filename) + filename;
			while( p > filename && (*p != '.') && (*p != '/') && (*p != '\\') ) p--;
			if ( *p != '.' )
				return NULL;
			else
				return p;
		}

		static inline
			std::string GetNameNoExt( const char *fileName )
		{
			const char *p = strlen(fileName) + fileName;
			while( p > fileName && (*p != '.') && (*p != '/') && (*p != '\\') ) p--;
			if ( *p != '.' )
				return fileName;
			else
			{
				std::string s;
				s.append( fileName, p-fileName );
				return s;
			}
		}

		static inline
		const char * GetExt(std::string &filename)
		{
			return GetExt(filename.c_str());
		}


		static inline
		const char *GetFilename(const char *sFilename)
		{
			int len =strlen(sFilename);
			int i = 0;
			for(i=len-2;i>-1;--i)
			{
				if( (sFilename[i] == '/') || (sFilename[i] == '\\') )
				{
					break;
				}
			}
			return sFilename+i+1;
		}
	};

	namespace Path
	{
		static inline int Parent( char *pDir, int nLen )
		{
			bool bFind=false;
			int i=0;
			int maxIndex = nLen - 1;
			while( (pDir[maxIndex] == '\\') || (pDir[maxIndex] == '/') )
			{
				--maxIndex;
			}
			for(i=maxIndex;i>-1;i--)
			{
				if(pDir[i]!='\\'&&pDir[i]!='/')
				{
					bFind=true;
				}
				if((pDir[i]=='\\'||pDir[i]=='/')&&bFind)
				{
					break;
				}
			}
			if(i>0)
			{
				pDir[i] = '\0';
				return i;
			}
			else
			{
				return 0;
			}
		}

		static inline int Parent(std::string &sDir)
		{
			bool bFind=false;
			int i=0;
			int maxIndex = sDir.size() - 1;
			while( (sDir[maxIndex] == '\\') || (sDir[maxIndex] == '/') )
			{
				--maxIndex;
			}
			for(i=maxIndex;i>-1;i--)
			{
				if(sDir[i]!='\\'&&sDir[i]!='/')
				{
					bFind=true;
				}
				if((sDir[i]=='\\'||sDir[i]=='/')&&bFind)
				{
					break;
				}
			}
			if(i>0)
			{
				sDir.resize(i);
				return i;
			}
			else
			{
				sDir.erase(sDir.begin(),sDir.end());
				return 0;
			}
		}

		static inline const char *GetName(const char *pDir)
		{
			int len =strlen(pDir);
			int i = 0;
			if( len < 1 )
				return pDir;
			int maxIndex = len-1;
			while( (pDir[maxIndex] == '\\') || (pDir[maxIndex] == '/') )
			{
				--maxIndex;
				if( maxIndex < 0 )
					break;
			}
			for(i=maxIndex;i>-1;--i)
			{
				if( (pDir[i] == '/') || (pDir[i] == '\\') )
				{
					break;
				}
			}
			return pDir+i+1;
		}

		static inline bool Valid(std::string &sDir)
		{
			if(sDir.empty())
				return false;
			else
				return true;
		}

		  	static inline void NoEndDir(std::string &sDir)
		{
			int nLength=sDir.size();
			int i=0;
			for(i=nLength-1;i>-1;i--)
			{
				if(sDir[i]!='\\'&&sDir[i]!='/')
					break;
			}
			sDir.resize(i+1);
		}

		static inline void	EndDir( std::string &sDir )
		{
			int nLength=sDir.size();
			if( sDir[nLength-1] != '/'
				&& sDir[nLength-1] != '\\' )
				sDir += '/';
		}

		static inline void NoBeginDir( std::string &sDir )
		{
			int nLength=sDir.size();
			int i=0;
			for(i=0;i<nLength;i++)
			{
				if(sDir[i]!='\\'&&sDir[i]!='/')
					break;
			}
			if( i > 0 )
				sDir.erase(sDir.begin(), sDir.begin()+i);
		}

			static inline
		bool SplitRootFromLayerDir( std::string &sDirBegin, std::string &sLeft, std::string &sSource )
		{
			int nBegin = 0;
			int nSplit = 0;

			bool bFindBegin = false;
			unsigned int i=0;
			unsigned int nSize = sSource.size();
			for( i=0; i<nSize; ++i )
			{
				if( sSource[i] == '/' || sSource[i] == '\\' )
				{
					if( !bFindBegin )
					{
						nBegin++;
					}
					else
					{
						nSplit = i;
						break;
					}
				}
				else
				{
					if( !bFindBegin )
						bFindBegin = true;
				}
			}

			if( nSplit > 0 )
			{
				sDirBegin = "";
				sLeft = "";
				sDirBegin.insert( sDirBegin.begin(), sSource.begin()+nBegin, sSource.begin()+nSplit );
				sLeft.insert( sLeft.begin(), sSource.begin()+nSplit+1, sSource.end() );
				return true;
			}
			return false;
		}
	};
}


#endif


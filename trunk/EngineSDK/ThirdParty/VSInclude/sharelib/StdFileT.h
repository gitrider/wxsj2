#pragma once

#include <io.h>

#include "utiltemplatepoint.h"

#pragma warning (disable : 4267)
template<typename T>
class CStdFileT : public FI_UTIL::ut_BaseTemplateRep
{
public:
	CStdFileT(void)
	{
		_FilePtr=NULL;
	}

	~CStdFileT(void)
	{
		Close();
	}
	
public:
	typedef int size_type;

private:
	typedef CStdFileT<T> _ThisClass;

private:
	FILE *_FilePtr;
	std::string _strFilename;

public:
	enum{begin=SEEK_SET,current=SEEK_CUR,end=SEEK_END,};
/*	enum OpenFlags {
		modeRead =         (int) 0x00000,
		modeWrite =        (int) 0x00001,
		modeReadWrite =    (int) 0x00002,
		shareCompat =      (int) 0x00000,
		shareExclusive =   (int) 0x00010,
		shareDenyWrite =   (int) 0x00020,
		shareDenyRead =    (int) 0x00030,
		shareDenyNone =    (int) 0x00040,
		modeNoInherit =    (int) 0x00080,
		modeCreate =       (int) 0x01000,
		modeNoTruncate =   (int) 0x02000,
		typeText =         (int) 0x04000, // typeText and typeBinary are
		typeBinary =       (int) 0x08000, // used in derived classes only
		osNoBuffer =       (int) 0x10000,
		osWriteThrough =   (int) 0x20000,
		osRandomAccess =   (int) 0x40000,
		osSequentialScan = (int) 0x80000,
		};

	size_type Open(char *filename,int mode)
	{
		if(mode&modeCreate)
		{
			if(mode&modeNoTruncate)
			{
				Open(filename,"a+");
				Close();
				return Open(filename,"r+");
			}
			else
				return Open(filename,"w+");
		}

		if(mode&modeRead)
		{
			return Open(filename,"r");
		}
		else
		{
			return Open(filename,"r+");
		}

		return Open(filename,"r+");
	}*/

	FILE * GetHandle()
	{
		return _FilePtr;
	}

	bool Open(const char *filename,const char *mode)
	{
		_strFilename=filename;
		_FilePtr=fopen(filename,mode);
		return _FilePtr==NULL?0:1;
	}

	void Close()
	{
		if(_FilePtr)
			fclose(_FilePtr),_FilePtr=NULL;
	}

	size_type Read(T *buf,size_type len)
	{
		return fread(buf,sizeof(T),len,_FilePtr);
	}

	size_type ReadData( void *buf, size_type len )
	{
		return fread(buf,1,len, _FilePtr );
	}

	size_type WriteData( const void *buf, size_type len )
	{
		return fwrite( buf, 1, len, _FilePtr );
	}

	size_type Write(const T *buf,size_type len)
	{
		return fwrite(buf,sizeof(T),len,_FilePtr);
	}

	size_type WriteInt( int &n )
	{
		return Write( (T*)&n, sizeof(int)/sizeof(T) );
	}

	size_type ReadInt( int &n )
	{
		return Read( (T*)&n, sizeof(int)/sizeof(T) );
	}

	size_type	WriteACIIString(  const char *p, ... )
	{
		va_list vl;
		va_start(vl, p);
	//	int len = _vscprintf( l, vl);
	//	char * p = (char *)malloc( len + 1 );
	//	vsprintf( p, l, vl );
	//	p[len] = '\0';
		int n = vfprintf( _FilePtr, p, vl );
		va_end(vl);
		return n;
	}

	size_type WriteString( const std::string &s )
	{
		int nSize = s.length();
		WriteInt( nSize );
		if( nSize > 0 )
		{
			Write( s.c_str(), nSize );
		}
		return nSize+4;
	}

	size_type ReadString( std::string &s )
	{
		int nSize = 0;
		ReadInt( nSize );
		if( nSize > 0 )
		{
			s.resize( nSize );
			Read( &s[0], nSize );
		}
		return nSize+4;
	}

	bool IsOpen()
	{
		return _FilePtr==NULL?0:1;
	}

	size_type Seek(size_type offset,size_type form)
	{
		return fseek(_FilePtr,offset,form);
	}

	size_type SeekToBegin()
	{
		return Seek(0,begin);
	}

	size_type SeekToEnd()
	{
		return Seek(0,end);
	}


	size_type SeekCurrent( int nOffset )
	{
		return fseek(_FilePtr, nOffset, current );
	}

	size_type	GetCurrentPosition()
	{
		return ftell(_FilePtr);
	}

	/// normal not use type end
	/// it's more slowly than the other two
	size_type SeekUseCurrent( int nOffset, int seekType )
	{
		if( seekType == current )
		{
			if( nOffset != 0 )
				return fseek( _FilePtr, nOffset, SEEK_CUR );
			else
				return 0;
		}
		else if( seekType == begin )
		{
			/// from begin cacl the cur offset
			size_type curPos = GetCurrentPosition();
			int nCurOffset = nOffset - curPos;
			if( nCurOffset != 0 )
				return fseek( _FilePtr, nCurOffset, SEEK_CUR);
			else
				return 0;
		}
		else if( seekType == end )
		{
			/// from end cacl the cur offset
			size_type curPos = GetCurrentPosition();
			int nCurOffset = nOffset - curPos + GetLength();
			if( nCurOffset != 0 )
				return fseek( _FilePtr, nCurOffset, SEEK_CUR);
			else
				return 0;
		}
		return 0;
	}

	size_type GetLength()
	{
		size_type pos=ftell(_FilePtr);
		fseek(_FilePtr,0,SEEK_END);
		size_type len=ftell(_FilePtr);
		fseek(_FilePtr,pos,SEEK_SET);
		return len;
	}

	std::string GetFileName()
	{
		return _strFilename;
	}

	void Flush()
	{
		if(_FilePtr)
			fflush(_FilePtr);
	}

	bool	IsEOF()
	{
		if( _FilePtr )
			return (feof(_FilePtr)!=0);
		return false;
	}

	int		GetError()
	{
		if( _FilePtr )
			return ferror(_FilePtr);
		return 0;
	}

	size_type SetLength(size_type newsize)
	{
#ifdef WIN32
		chsize(fileno(_FilePtr),newsize);
#else
		ftruncate(_strFilename.c_str(),newsize);
#endif
		return newsize;
	}

	size_type read(void* buffer, int sizeToRead) 
	{
		return fread( buffer, 1, sizeToRead, _FilePtr );
	}

		//! Changes position in file, returns true if successful.
		//! \param finalPos: Destination position in the file.
		//! \param relativeMovement: If set to true, the position in the file is
		//! changed relative to current position. Otherwise the position is changed 
		//! from begin of file.		
		//! \return Returns true if successful, otherwise false.
	bool seek(int finalPos, bool relativeMovement = false)
	{
		if( relativeMovement )
			return SeekCurrent( finalPos ) > 0;
		else
			return Seek( finalPos, SEEK_SET ) > 0;
	}

		//! Returns size of file.
		//! \return Returns the size of the file in bytes.
	size_type getSize() {return GetLength();}

		//! Returns the current position in the file.
		//! \return Returns the current position in the file in bytes.
	size_type getPos() {return GetCurrentPosition();}

		//! Returns name of file.
		//! \return Returns the file name as zero terminated character string.
	const char* getFileName() {return _strFilename.c_str();}
};

#pragma warning (default : 4267)

#ifndef WIN32
typedef CStdFileT<char> CFile;
typedef CStdFileT<char>	CMyFile;
typedef FI_UTIL::ut_TemplatePointer< CMyFile >	CMyFilePointer;
#else
typedef CStdFileT<char> CMyFile;
typedef FI_UTIL::ut_TemplatePointer< CMyFile >	CMyFilePointer;
#endif
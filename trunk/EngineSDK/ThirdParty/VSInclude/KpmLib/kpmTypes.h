
#ifndef _KPM_TYPES_H_
#define _KPM_TYPES_H_

namespace KPM
{

#define   KPM_MAGIC			0x1A4D504B					//head 'KPM\1A

#ifndef   KPM_ALLOC
#define   KPM_ALLOC(t, n)   (t*)malloc((n) * sizeof(t))
#define   KPM_FREE(ptr)		free(ptr)
#endif

#define KPM_FILE_IMPLODE         0x00000200 // Implode method (By PKWARE Data Compression Library)
#define KPM_FILE_COMPRESS        0x00000100 // Compress methods (My various methods)
#define KPM_FILE_COMPRESSED      0x0000FF00 // File is compressed
#define KPM_FILE_ENCRYPTED       0x00020000 // Indicates whether file is encrypted 
#define KPM_FILE_FIXSEED         0x00010000 // File decrypt seed has to be fixed
#define KPM_FILE_SINGLE_UNIT     0x02000000 // File is stored as a single unit, rather than split into sectors (Thx, Quantam)
#define KPM_FILE_EXISTS          0x80000000 // Set if file exists, reset when the file was deleted
#define KPM_FILE_REPLACEEXISTING 0x80000000 // Replace when the file exist (SFileAddFile)

#define KPM_FILE_VALID_FLAGS     (KPM_FILE_IMPLODE     |  \
	KPM_FILE_COMPRESS    |  \
	KPM_FILE_ENCRYPTED   |  \
	KPM_FILE_FIXSEED     |  \
	KPM_FILE_SINGLE_UNIT |  \
	KPM_FILE_EXISTS)


	// Compression types for multilpe compressions
#define KPM_COMPRESSION_HUFFMANN    0x01    // Huffmann compression (used on WAVE files only)
#define KPM_COMPRESSION_ZLIB        0x02    // ZLIB compression
#define KPM_COMPRESSION_PKWARE      0x08    // PKWARE DCL compression
#define KPM_COMPRESSION_BZIP2       0x10    // BZIP2 compression
#define KPM_COMPRESSION_WAVE_MONO   0x40    // 
#define KPM_COMPRESSION_WAVE_STEREO 0x80    // 

#define KPM_FLAG_CHANGED         0x00000001 // If set, the KPM has been changed
#define KPM_FLAG_PROTECTED       0x00000002 // Set on protected MPQs (like W3M maps)

	// Flags for MPQ attributes
#define KPM_ATTRIBUTE_CRC32     0x00000001  // The "(attributes)" contain array of CRC32s
#define KPM_ATTRIBUTE_FILETIME  0x00000002  // The "(attributes)" contain array of FILETIMEs
#define KPM_ATTRIBUTE_MD5       0x00000004  // The "(attributes)" contain array of MD5s

#define LISTFILE_NAME		"(list)"
#define SIGNATURE_NAME		"(sign)"		// Name of internal signature
#define ATTRIBUTES_NAME		"(attr)"		// Name of internal attributes file

#define LISTFILE_ENTRY_DELETED   (DWORD_PTR)(-2)
#define LISTFILE_ENTRY_FREE      (DWORD_PTR)(-1)

#define USE_TYPE_PATCH			 0x01
#define USE_TYPE_CLIENT			 0x02

#pragma pack(push, 1)

	struct	TKPMHeader
	{
		//the (KPM\1A) signature 
		DWORD		dwMagic; 
		//the KPM header size
		DWORD		dwHeaderSize;
		//the KPM archive size
		LONGLONG	dwArchiveSize;
		//KPM format version
		DWORD		dwVersion;
		//size of the file block
		WORD		wBlockSize;
		//哈希表偏移量
		LONGLONG	dwHashTablePos;
		//文件块表偏移量
		LONGLONG	dwBlockTablePos;
		//哈希表达小
		DWORD		dwHashTableSize;
		//文件块大小 
		DWORD		dwBlockTableSize;
		//文件用途
		DWORD		wUseType;
		//ListFileCache大小
		DWORD		dwMaxListFileCache;

	};

	enum BlockStatus
	{
		HASH_ENTRY_DELETED =      0xFFFFFFFE,			// Block index for deleted hash entry
		HASH_ENTRY_FREE    =      0xFFFFFFFF			// Block index for free hash entry
	};

	struct TKPMHashKey
	{
		// The hash of the file path, using method A.
		DWORD dwName1;

		// The hash of the file path, using method B.
		DWORD dwName2;

		// The blockindex of TKPMBlock
		DWORD dwBlockIndex;
	};


	struct TKPMBlock
	{
		//Offset of the beginning of the block, relative to the beginning of the archive.
		LONGLONG dwFilePos;


		//Compressed file size
		DWORD dwCSize;

		//UnCompressed file size;
		DWORD dwFSize;                      

		// Flags for the file. 
		DWORD dwFlags;  

		void  Clean()
		{
			dwFilePos	  = 0;
			dwCSize		  = 0;
			dwFSize		  = 0;
			dwFlags		  = 0;
		}
	};

	// CRC32 present in the (attr) file
	struct TKPMCRC32
	{
		DWORD dwValue;                      // Value of CRC32 for each block
	};


	// FILETIME present in the (attr) file
	struct TKPMFileTime
	{
		DWORD dwFileTimeLow;                // Low DWORD of the FILETIME
		DWORD dwFileTimeHigh;               // High DWORD of the FILETIME
	};

#define  MD5Length 16
	// MD5 present in the (attr) file
	struct TKPMMD5
	{
		BYTE Value[MD5Length];                   // 16 bytes of MD5
	};


	// Data from (attr) file
	struct TKPMAttr
	{
		DWORD			dwVersion;                    // Version of the (attr) file
		DWORD			dwFlags;                      // See KPM_ATTRIBUTE_XXXX
		TKPMCRC32		* pCrc32;					  // Array of CRC32 (NULL if none)
		TKPMFileTime	* pFileTime;				  // Array of FILETIME (NULL if not present)
		TKPMMD5			* pMd5;						  // Array of MD5 (NULL if none)
	};

#pragma pack(pop)


	struct TFileNode
	{
		DWORD dwRefCount;                   // Number of references
		// When a number of node references reaches zero, 
		// the node will be deleted
		// support multithread operations

		DWORD nLength;                     // File name length
		char  szFileName[1];                // File name, variable length
	};
	struct TKPMFile;

	struct TKPMBlockCache
	{
		TKPMFile*	pLastFile;
		DWORD		dwblockOffSet;
		BYTE*		pbBuffer;
		DWORD		BufferOffSet;

	};
	struct TKPMArchive
	{
		char			szFileName[MAX_PATH];	// Archive file path
		DWORD           dwPriority;				// Priority of the archive
		HANDLE			hArchiveFile;			// Archive File handle
		LONGLONG		HeaderOffset;           // File header offset (relative to the begin of the file)
		LARGE_INTEGER	HashTableOffset;        // Hash table offset (relative to the begin of the file)
		LARGE_INTEGER	BlockTableOffset;       // Block table offset (relative to the begin of the file)
		LONGLONG		CurrentOffset;			// Current file position (relative to begin of the file)
		LONGLONG		KpmSize;                // Size of KPM archive	

		DWORD           dwBlockSize;            // Size of file block
		TKPMBlockCache	BlockCache;
		TKPMHeader*		pHeader;				// Pointer to Header 
		TKPMHashKey*	pHashKeyTable;			// Pointer to HashKey table		
		TKPMBlock*		pBlockTable;			// Pointer to Block Table;

		TKPMHeader		Header;					// Header 

		TKPMAttr*		pFileAttr;				// Pointer to File attr in (attr) file
		TFileNode  **	pListFile;				// File name array
		DWORD			dwFlags;				// KPM flags;

		void Clean(char* name)
		{
			memset(this, 0, sizeof(*this));
		}
	};


	struct TKPMFile
	{
		HANDLE         hFile;					// File handle
		TKPMArchive*	ha;						//Pointer to Archive 
		TKPMHashKey*	pHash;					//Pointer to hash table
		TKPMBlock*		pBlock;					//Pointer to block table
		DWORD           dwSeed1;				//Seed used for file decrypt
		DWORD			FileOffSet;				//File offset from file begin
		LARGE_INTEGER	RawFileOffSet;          //Offset in KPM archive (relative to file begin)
		LARGE_INTEGER	MpqFileOffSet;          //Offset in KPM archive (relative to KPM header)

		LARGE_INTEGER*	pllBlockOffSet;			//Position of each file block
		DWORD			nBlocks;				//Block counts
		BOOL            bBlockPosLoaded;		// TRUE if block positions loaded
		BYTE*			pFileBuffer;			//file buffers;

		TKPMCRC32*		pCrc32;					// Pointer to CRC32 (NULL if none)
		TKPMFileTime*	pFileTime;				// Pointer to file's FILETIME (NULL if none)
		TKPMMD5*		pMD5;					// Pointer to file's MD5 (NULL if none)

		DWORD			dwHashIndex;			// Index to Hash table
		DWORD			dwBlockIndex;			// Index to Block table
		char			szFileName[1];			// File name (variable length)
	};

	enum
	{
		FILE_TYPE_DATA	=  0,              // Process the file as data file
		FILE_TYPE_WAVE	=  1,              // Process the file as WAVE file
	};

	// Used by searching in KPM archives
	struct TKPMSearch
	{
		TKPMArchive *	ha;							 // Handle to KPM, where the search runs
		DWORD			dwNextIndex;                 // The next searched hash index
		DWORD			dwName1;                     // Lastly found Name1
		DWORD			dwName2;                     // Lastly found Name2
		char			szSearchMask[1];             // Search mask (variable length)
	};


	struct FILE_FIND_DATA
	{
		char   cFileName[MAX_PATH];         // Full name of the found file
		char * szPlainName;                 // Pointer to file part
		DWORD  dwFileSize;                  // File size in bytes
		DWORD  dwFileFlags;                 // File flags (compressed or encrypted)
		DWORD  dwBlockIndex;                // Block index for the file
		DWORD  dwCompSize;                  // Compressed file size
	};
}

#endif
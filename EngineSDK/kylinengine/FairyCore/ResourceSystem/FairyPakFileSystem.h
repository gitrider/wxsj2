/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
#ifndef __PakFileSystem_H__
#define __PakFileSystem_H__

#include "OgrePrerequisites.h"

#include "OgreArchive.h"
#include "OgreArchiveFactory.h"

#include <map>
#include <vector>
#include <Windows.h>

using std::map;
using std::pair;
using std::vector;

namespace Ogre {

	typedef struct _FILE_DESC
	{
		char szFileName[_MAX_PATH];
		Ogre::ulong dwOffset;
		Ogre::ulong dwSize;
		Ogre::ulong dwCRC;
	} FILE_DESC, *LPFILE_DESC;

	typedef struct _PACKAGE_DESC
	{
		char szPackName[_MAX_PATH];
		Ogre::ulong dwFileSize;
		Ogre::ulong dwAvailSize;
		//FILE* lpFile;
		HANDLE lpFileMap;
		unsigned char* lpFileAddr;
	} PACKAGE_DESC, *LPPACKAGE_DESC;

	const Ogre::String WXFileVersion = "FairyV1"; //--Current WXFile Verison

	/** Specialisation of the Archive class to allow reading of files from 
        package files.
    */
	class _OgrePrivate PakFileSystemArchive : public Archive 
    {
	public:
		OGRE_AUTO_MUTEX
    protected:
		/// total file info length
		Ogre::ulong mdwHeadLength;

		/// total file data length
		Ogre::ulong mdwFileLength;

		/// file name map
		std::map<String, FILE_DESC > mPackedFileMap;
		typedef pair<String, FILE_DESC > mPackedFilePair;
		std::vector<PACKAGE_DESC > mPackedFiles;

        /// Base path; actually the same as mName, but for clarity
       Ogre::String mBasePath;

        /** Utility method to retrieve all files in a directory matching pattern.
        @param pattern File pattern
        @param recursive Whether to cascade down directories
        @param simpleList Populated if retrieving a simple list
        @param detailList Populated if retrieving a detailed list
        @param currentDir The current directory relative to the base of the 
            archive, for file naming
        */
        void findFiles(const Ogre::String& pattern, bool recursive,Ogre::StringVector* simpleList,
            FileInfoList* detailList, const Ogre::String& currentDir = "");

		char* ConvertNumber( unsigned long num );
		char* DecryptName(char* name, int len);
		void  DecryptOffset(unsigned long& dwOffset);
		void  DecryptSize(unsigned long& dwSize);

    public:
        PakFileSystemArchive(const Ogre::String& name, const Ogre::String& archType );
        ~PakFileSystemArchive();

        /// @copydoc Archive::isCaseSensitive
        bool isCaseSensitive(void) const;

        /// @copydoc Archive::load
        void load();
        /// @copydoc Archive::unload
        void unload();

        /// @copydoc Archive::open
        DataStreamPtr open(const Ogre::String& filename) const;

        /// @copydoc Archive::list
       Ogre::StringVectorPtr list(bool recursive = true,bool dirs = false);

        /// @copydoc Archive::listFileInfo
        FileInfoListPtr listFileInfo(bool recursive = true,bool dirs = false);

        /// @copydoc Archive::find
       Ogre::StringVectorPtr find(const Ogre::String& pattern, bool recursive = true,bool dirs = false);

        /// @copydoc Archive::findFileInfo
        FileInfoListPtr findFileInfo(const Ogre::String& pattern, bool recursive = true, bool dirs = false);

		time_t getModifiedTime(const String& filename);

        /// @copydoc Archive::exists
		bool exists(const Ogre::String& filename);

    };

    /** Specialisation of ArchiveFactory for FileSystem files. */
    class _OgrePrivate PakFileSystemArchiveFactory : public ArchiveFactory
    {
    public:
        virtual ~PakFileSystemArchiveFactory() {}
        /// @copydoc FactoryObj::getType
        const Ogre::String& getType(void) const;
        /// @copydoc FactoryObj::createInstance
        Archive *createInstance( const Ogre::String& name ) 
        {
            return new PakFileSystemArchive(name, "FairyFileSystem");
        }
        /// @copydoc FactoryObj::destroyInstance
        void destroyInstance( Archive* arch) { delete arch; }
    };


}

#endif

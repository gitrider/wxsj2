#include "FairyResourceSolverHelper.h"

#include <OgreResourceGroupManager.h>
#include <OgreCodec.h>

namespace Fairy
{

   Ogre::StringVectorPtr findResourceFilenames(const Ogre::String& group, const Ogre::String& filename)
    {
       StringSet filenames;

        // We must find use base name, otherwise, the relative path will lost
       Ogre::String basename, path;
        Ogre::StringUtil::splitFilename(filename, basename, path);
        Ogre::FileInfoListPtr fileInfos =
            Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(group, basename);
        if (!fileInfos.isNull())
        {
            for (Ogre::FileInfoList::const_iterator it = fileInfos->begin(); it != fileInfos->end(); ++it)
            {
                const Ogre::FileInfo& fileInfo = *it;
                if (fileInfo.archive->getType() == "FileSystem")
                {
                    if (fileInfo.filename == filename)
                    {
                        filenames.insert(fileInfo.archive->getName() + "/" + fileInfo.filename);
                    }
                }
            }
        }

        return toStringVectorPtr(filenames);
    }
    //-----------------------------------------------------------------------
   Ogre::StringVectorPtr findImageResources(const Ogre::String& group)
    {
       StringSet filenames;

        Ogre::Codec::CodecIterator it = Ogre::Codec::getCodecIterator();
        while (it.hasMoreElements())
        {
            const Ogre::String& ext = it.peekNextKey();
            Ogre::Codec* codec = it.getNext();
            if (codec->getDataType() != "ImageData")
                continue;

            Ogre::FileInfoListPtr fileInfoList =
                Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
                group,
                "*." + ext);
            for (Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
            {
                filenames.insert(it->filename);
            }
        }


        return toStringVectorPtr(filenames);
    }
    //-----------------------------------------------------------------------
    void moveMatchToFront(StringVector& names, const Ogre::String& name)
    {
       Ogre::StringVector::iterator it = std::find(names.begin(), names.end(), name);
        if (it != names.end() && it != names.begin())
        {
            std::swap(*it, names.front());
        }
    }

    //////////////////////////////////////////////////////////////////////////

    UnsolvedResourceBase::UnsolvedResourceBase(void)
        : UnsolvedResource()
        , origin()
        , description()
        , recommends()
        , possibles()
    {
    }
    //-----------------------------------------------------------------------
    UnsolvedResourceBase::~UnsolvedResourceBase()
    {
    }
    //-----------------------------------------------------------------------
    const Ogre::String& UnsolvedResourceBase::getOrigin(void) const
    {
        return origin;
    }
    //-----------------------------------------------------------------------
    const Ogre::String& UnsolvedResourceBase::getDescription(void) const
    {
        return description;
    }
    //-----------------------------------------------------------------------
    const Ogre::StringVector* UnsolvedResourceBase::getRecommendValues(void) const
    {
        return recommends.get();
    }
    //-----------------------------------------------------------------------
    const Ogre::StringVector* UnsolvedResourceBase::getPossibleValues(void) const
    {
        return possibles.get();
    }

    //////////////////////////////////////////////////////////////////////////

    UnsolvedResourceSimple::UnsolvedResourceSimple(void)
        : UnsolvedResourceBase()
        , references()
    {
    }
    //-----------------------------------------------------------------------
    UnsolvedResourceSimple::~UnsolvedResourceSimple()
    {
    }
    //-----------------------------------------------------------------------
    bool UnsolvedResourceSimple::setResolvedValue(const Ogre::String& value)
    {
        if (references.empty())
            return false;

        for (std::vector<String*>::const_iterator it = references.begin(); it != references.end(); ++it)
        {
            **it = value;
        }
        return true;
    }

    //////////////////////////////////////////////////////////////////////////

    ResourceSolverBase::ResourceSolverBase(void)
        : ResourceSolver()
        , unsolvedResources()
    {
    }
    //-----------------------------------------------------------------------
    ResourceSolverBase::~ResourceSolverBase()
    {
    }
    //-----------------------------------------------------------------------
    const UnsolvedResourceList& ResourceSolverBase::getUnsolvedResources(void) const
    {
        return unsolvedResources;
    }

}

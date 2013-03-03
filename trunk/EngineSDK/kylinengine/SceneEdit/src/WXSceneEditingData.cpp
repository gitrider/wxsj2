/********************************************************************
filename:   WXSceneEditingData.cpp

purpose:    ���泡���༭ʱ��Ҫ�����һЩ��Ϣ������reshape����ǰ���ε�
            ��С�������´δ�ͬһ����ʱ������ȷ����ʾreshape��������
            �ߡ�
*********************************************************************/

#include "WXSceneEditingData.h"

#include <OgreString.h>
#include <OgreStringConverter.h>
#include <OgreDataStream.h>

namespace Fairy {

    SceneEditingData::SceneEditingData() :
    mDataChanged(false)
    {
    }

    SceneEditingData::~SceneEditingData()
    {
    }

    void SceneEditingData::saveData(const String& fileName)
    {
        String baseName, path;
        Ogre::StringUtil::splitFilename(fileName, baseName, path);
        if (Ogre::StringUtil::endsWith(baseName, ".scene"))
            baseName.erase(baseName.length() - (sizeof(".scene") - 1));

        String editDataFileName = path + baseName + ".SED";

        std::ofstream of(editDataFileName.c_str());

        saveTerrainReshapeSize(of);

        of.close();
    }

    void SceneEditingData::readData(const String& fileName)
    {
        String baseName, path;
        Ogre::StringUtil::splitFilename(fileName, baseName, path);
        if (Ogre::StringUtil::endsWith(baseName, ".scene"))
            baseName.erase(baseName.length() - (sizeof(".scene") - 1));

        String editDataFileName = path + baseName + ".SED";

        std::ifstream stream;
        stream.open(editDataFileName.c_str());

        if (stream)
        {
            Ogre::DataStreamPtr ifStream( new Ogre::FileStreamDataStream(&stream, false) );

            String line;

            while (!ifStream->eof())
            {
                line = ifStream->getLine();

                Ogre::StringVector para = Ogre::StringUtil::split(line);

                if ( para[0] == "ReshapeLeft" )
                {
                    mTerrainReshapeSize.left = Ogre::StringConverter::parseInt(para[1]);
                }
                else if ( para[0] == "ReshapeRight" )
                {
                    mTerrainReshapeSize.right = Ogre::StringConverter::parseInt(para[1]);
                }
                else if ( para[0] == "ReshapeTop" )
                {
                    mTerrainReshapeSize.top = Ogre::StringConverter::parseInt(para[1]);
                }
                else if ( para[0] == "ReshapeBottom" )
                {
                    mTerrainReshapeSize.bottom = Ogre::StringConverter::parseInt(para[1]);
                }
            }
        }
    }

    void SceneEditingData::saveTerrainReshapeSize(std::ofstream& of)
    {
        of << "ReshapeLeft " << Ogre::StringConverter::toString(mTerrainReshapeSize.left) << std::endl;
        of << "ReshapeRight " << Ogre::StringConverter::toString(mTerrainReshapeSize.right) << std::endl;
        of << "ReshapeTop " << Ogre::StringConverter::toString(mTerrainReshapeSize.top) << std::endl;
        of << "ReshapeBottom " << Ogre::StringConverter::toString(mTerrainReshapeSize.bottom) << std::endl;
    }
}
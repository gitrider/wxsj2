#include "TerrainPaintInfoContainer.h"

#include <OgreStringConverter.h>
#include <OgreException.h>

namespace Fairy {

    TerrainPaintInfoContainer::TerrainPaintInfoContainer()
    {
        mStringTexTypeMap["�ڲ�����"]       = InnerTex;
        mStringTexTypeMap["�ڽ�"] = InnerCornerTex;
        mStringTexTypeMap["���"] = OuterCornerTex;
        mStringTexTypeMap["��Ե"]        = EdgeTex;	
        mStringTexTypeMap["�Խ�"]     = DuijiaoTex;

		mStringExtraTexTypeMap["���Ͻ�"]        = LT;
		mStringExtraTexTypeMap["���Ͻ�"]		= RT;
		mStringExtraTexTypeMap["���½�"]		= LB;
		mStringExtraTexTypeMap["���½�"]        = RB;	
		mStringExtraTexTypeMap["���Ե"]        = LE;
		mStringExtraTexTypeMap["�ұ�Ե"]        = RE;
		mStringExtraTexTypeMap["�ϱ�Ե"]        = TE;
		mStringExtraTexTypeMap["�±�Ե"]        = BE;
		mStringExtraTexTypeMap["��ͨ"]			= EOther;
    }

    TerrainPaintInfoContainer::~TerrainPaintInfoContainer()
    {
    }

    bool TerrainPaintInfoContainer::isTextureInfoOk( const TextureInfo &info )
    {
        return (info.brushName.empty() == false) &&
            (info.height > 0) &&
            (info.width > 0) &&
            (info.leftCorner >= 0) &&
            (info.topCorner >= 0) &&
            (info.ownerTextureName.empty() == false);
    }

    void TerrainPaintInfoContainer::addTextureInfo( TextureInfo &info )
    {
        assert ( isTextureInfoOk(info) );

        String brushName = info.brushName;

        String pathName = info.ownerTextureName;

		addTexNameAndBrushMapInfo(pathName,brushName);

        size_t pos = pathName.find_last_of('/');

        if (pos != String::npos)
        {
            pathName.erase(pos+1);
            pathName.append(brushName);

            brushName = pathName;
        }

        String texTypeStr;

        // �ҵ�������������ַ���
        StringTexTypeMap::iterator itForStringTexTypeMap = mStringTexTypeMap.begin();
        while ( itForStringTexTypeMap != mStringTexTypeMap.end() )
        {
            if (itForStringTexTypeMap->second == info.textureType)
            {
                texTypeStr = itForStringTexTypeMap->first;
                break;
            }
            ++itForStringTexTypeMap;
        }

        assert (itForStringTexTypeMap != mStringTexTypeMap.end());

        TextureInfoMap::iterator it = mTextureInfoMap.find(brushName);

        // �����ǰmap��û�����brush��˵����һ���µĻ�ˢ
        if ( it == mTextureInfoMap.end() )
        {
            TextureInfos newInfos;            

            // �Զ������������ƣ����Ƹ�ʽΪ ������|��ǰ����������|�������͡�
            String textureName = info.brushName + "|"
                + Ogre::StringConverter::toString(newInfos.size()) + "|" + texTypeStr;

            info.textureName = textureName;

            newInfos.push_back(info);

            std::pair<TextureInfoMap::iterator, bool> inserted = 
                mTextureInfoMap.insert( TextureInfoMap::value_type(brushName, newInfos) );

            assert (inserted.second);
        }
        else
        {
            // �Զ�������������
            String textureName = info.brushName + "|"
                + Ogre::StringConverter::toString(it->second.size()) + "|" + texTypeStr;

            info.textureName = textureName;

            it->second.push_back(info);
        }
    }

	void TerrainPaintInfoContainer::addTexNameAndBrushMapInfo( const String &texName, const String &brushName )
	{
		// ��Ѱ�������Ϣ�Ƿ���ӹ���
		TexNameAndBrushMap::const_iterator i = mTexNameAndBrushMap.find(texName);

		if ( i == mTexNameAndBrushMap.end() )
		{
			std::pair<TexNameAndBrushMap::iterator, bool> inserted = 
				mTexNameAndBrushMap.insert( TexNameAndBrushMap::value_type(texName, brushName) );
			assert (inserted.second);
		}
	}

	const String &TerrainPaintInfoContainer::findBrushNameByTexName(const String &texName) const
	{
		TexNameAndBrushMap::const_iterator i = mTexNameAndBrushMap.find(texName);

		if (i == mTexNameAndBrushMap.end())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "Can't found brush texture with name '" + texName + "'",
                "TerrainPaintInfoContainer::findBrushNameByTexName");
        }

        return i->second;		
	}

	const TextureInfo &TerrainPaintInfoContainer::findTextureInfoByTexName( const String &texName )
	{
		String brushName = findBrushNameByTexName(texName);

		size_t pos = texName.find_last_of('/');

		// ��������Ļ�ˢ����
		if (pos != String::npos)
		{
			brushName = texName.substr(0,pos+1) + brushName;
		}

		TextureInfoMap::iterator it = mTextureInfoMap.begin();

		while ( it != mTextureInfoMap.end() )
		{
			if ( it->first == brushName )
			{
				break;
			}
			++it;
		}

		if ( it != mTextureInfoMap.end() )
		{
			TextureInfos &infos = it->second;            

			for ( size_t count = 0; count < infos.size(); ++count )
			{
				if (infos[count].ownerTextureName == texName)
				{
					return infos[count];
				}
			}
		}

		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
			"Can't found texture with name '" + texName + "'",
			"TerrainPaintInfoContainer::findTextureInfoByTexName");
	}

}
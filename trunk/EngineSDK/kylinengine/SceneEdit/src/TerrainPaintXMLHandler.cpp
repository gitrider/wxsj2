#include "TerrainPaintXMLHandler.h"

#include "Core/FairyXMLAttributes.h"

#include <OgreException.h>
#include <OgreStringConverter.h>

namespace Fairy {

    TerrainPaintXMLHandler::TerrainPaintXMLHandler(TerrainPaintInfoContainer *container) : 
    mPaintInfoContainer(container), 
    mErrorInfoString(""),
    mCurrentElementWrong(false)
    {
        mCurrentTextureInfo.clear();       
    }

    /** Destructor for SceneXMLHandler objects */
    TerrainPaintXMLHandler::~TerrainPaintXMLHandler(void)
    {
    }

    /*************************************************************************
    SAX2 Handler overrides
    *************************************************************************/ 

    /** Overridden from XMLHandler */
    void TerrainPaintXMLHandler::characters(const String& chars)
    {
    }

    /** Overridden from XMLHandler */
    void TerrainPaintXMLHandler::startElement(const String& element, const XMLAttributes& attributes)
    {
        if (element == "��ͼ")
        {
            mCurrentTextureInfo.clear();
        }
        else if (element == "������ͼ����")
        {
            String attValue = attributes.getValue("����");
            
            if ( attValue.empty() )
            {
                _pushErrorInfoString(element + " " + attValue +
                    " the value can't be empty!");
                mCurrentElementWrong = true;
                return;
            }

            mCurrentTextureInfo.ownerTextureName = attValue;
        }
        else if (element == "��ͼ��С")
        {
            String attValue = attributes.getValue("��С");

            if ( attValue.empty() )
            {
                _pushErrorInfoString(element + " " + attValue +
                    " the value can't be empty!");
                mCurrentElementWrong = true;
                return;
            }

            Ogre::StringVector valueVector = Ogre::StringUtil::split(attValue,",");

            if ( valueVector.size() != 2 )
            {
                _pushErrorInfoString(element + " " + attValue +
                    " size of parameter must be two!");
                mCurrentElementWrong = true;
                return;
            }

            mCurrentTextureInfo.width = Ogre::StringConverter::parseUnsignedInt(valueVector[0]);
            mCurrentTextureInfo.height = Ogre::StringConverter::parseUnsignedInt(valueVector[1]);
        }
        else if (element == "��ͼ����")
        {
            String attValue = attributes.getValue("����");
            
            if ( attValue.empty() )
            {
                _pushErrorInfoString(element + " " + attValue +
                    " the value can't be empty!");
                mCurrentElementWrong = true;
                return;
            }

            Ogre::StringVector valueVector = Ogre::StringUtil::split(attValue,",");

            if ( valueVector.size() != 2 )
            {
                _pushErrorInfoString(element + " " + attValue +
                    " size of parameter must be two!");
                mCurrentElementWrong = true;
                return;
            }

            mCurrentTextureInfo.leftCorner = Ogre::StringConverter::parseUnsignedInt(valueVector[0]);
            mCurrentTextureInfo.topCorner = Ogre::StringConverter::parseUnsignedInt(valueVector[1]);
        }
        else if (element == "��ˢ����")
        {
            String attValue = attributes.getValue("����");

            if ( attValue.empty() )
            {
                _pushErrorInfoString(element + " " + attValue +
                    " the value can't be empty!");
                mCurrentElementWrong = true;
                return;
            }

            mCurrentTextureInfo.brushName = attValue;
        }
        else if (element == "��ͼ����")
        {
            String attValue = attributes.getValue("����");

            if ( attValue.empty() )
            {
                _pushErrorInfoString(element + " " + attValue +
                    " the value can't be empty!");
                mCurrentElementWrong = true;
                return;
            }

            TerrainPaintInfoContainer::StringTexTypeMap::const_iterator it = mPaintInfoContainer->mStringTexTypeMap.find(attValue);

            if  ( it == mPaintInfoContainer->mStringTexTypeMap.end() )
            {
                _pushErrorInfoString(element + " " + attValue + " can't find the texture type!");
                mCurrentElementWrong = true;
                return;
            }

            // �Ӷ�Ӧ���в��ҳ���Ӧ�������ַ���
            mCurrentTextureInfo.textureType = it->second;

			attValue = attributes.getValue("�����������Ϣ");

            if ( attValue.empty() )
            {
                _pushErrorInfoString(element + " " + attValue +
                    " the value can't be empty!");
                mCurrentElementWrong = true;
                return;
            }

			TerrainPaintInfoContainer::StringExtraTexTypeMap::const_iterator itt = mPaintInfoContainer->mStringExtraTexTypeMap.find(attValue);

			if ( itt == mPaintInfoContainer->mStringExtraTexTypeMap.end() )
            {
                _pushErrorInfoString(element + " " + attValue + " can't find the extra texture info type!");
                mCurrentElementWrong = true;
                return;
            }

			// �Ӷ�Ӧ���в��ҳ���Ӧ�������ַ���
			mCurrentTextureInfo.extraTexType = itt->second;
        }
		else if (element == "��ת����")
        {
            String attValue = attributes.getValue("��������");

            if ( attValue.empty() )
            {
                _pushErrorInfoString(element + " " + attValue +
                    " the value can't be empty!");
                mCurrentElementWrong = true;
                return;
            }

			mCurrentTextureInfo.rotateType = Ogre::StringConverter::parseUnsignedInt(attValue);

			// ��ƴ�ӵĴ���ͼ�з���ֻ����4��ѡ��
			if ( false == (mCurrentTextureInfo.rotateType >= 0 && mCurrentTextureInfo.rotateType <= 3) )
            {
                _pushErrorInfoString(element + " " + attValue + 
                    " rotate type must be between 0 and 3!");
                mCurrentElementWrong = true;
                return;
            }
		}
        else if (element == "��ͼ��Ϣ")
        {
        }
        // anything else is an error which *should* have already been caught by XML validation
        else
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                "Unexpected data was found while parsing the scene file: '" + element + "' is unknown.",
                "SceneXMLHandler::startElement");
        }
    }

    /** Overridden from XMLHandler */
    void TerrainPaintXMLHandler::endElement(const String& element)
    { 
        if (element == "��ͼ")
        {
            // ����ڽ�����һ����ͼ��Ϣʱ�����˴��󣬾Ͳ�������texture info
            if (mCurrentElementWrong)
            {
                // �Ѵ������ͼ��Ϣ��¼����
                _pushErrorInfoString( _getCurrentTextureInfo() );
                mCurrentElementWrong = false;
            }
            else
                // ��ÿ��texture���������ӵ�map��
                mPaintInfoContainer->addTextureInfo(mCurrentTextureInfo);
        }
    }

    void TerrainPaintXMLHandler::_pushErrorInfoString(const String& errorInfo)
    {
        mErrorInfoString += "wrong element : " + errorInfo;
        mErrorInfoString += "\n";
    }

    const String TerrainPaintXMLHandler::_getCurrentTextureInfo(void) const
    {       
        String textureInfoStr;

        textureInfoStr += "\n";
        textureInfoStr += "Owner Texture Name : " + mCurrentTextureInfo.ownerTextureName + "\n";
        textureInfoStr += "Brush Name : " + mCurrentTextureInfo.brushName + "\n";
        return textureInfoStr;
    }

}
/********************************************************************
filename:   WXMiniMapMaker.cpp

purpose:    ���ݳ������ݣ��Զ�����һ��mini map
*********************************************************************/

#include "WXMiniMapMaker.h"
#include "SceneManipulator.h"

#include <OgreRenderTexture.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreRenderSystem.h>
#include <OgreViewport.h>
#include <OgreColourValue.h>
#include <OgreStringConverter.h>
#include <OgreTextureManager.h>
#include <OgreEntity.h>
#include <OgreMath.h>
#include <OgreHardwarePixelBuffer.h>

#include "Core/TerrainData.h"
#include "math.h"
namespace Fairy {

    MiniMapMaker::MiniMapMaker( SceneManipulator* manipulator ) :
    mManipulator(manipulator),
    mRenderTexture(NULL),
    mCamera(NULL),
    mTexHeight(128),
    mTexWidth(128),
    mTileSize(800.0f),
    mPerAngle(90.0f),
    mTexExtension("jpg"),
    mOutFileName(""),
    mSceneBaseName(""),
    mPath(""),
    mNeedRecreate(true),
    mOutPutFormat(Ogre::PF_BYTE_RGB),
    mUseRealCameraAngle(false),
    mMoveZDir(Ogre::Vector3::ZERO),
    mCameraDir(Ogre::Vector3::ZERO),
    mCameraDirQuadrant(None),
    mCamDirAngle(0.0f),
    mInvertCameraDir(Ogre::Vector3::ZERO),
    mCameraDirectionType(CDT_MiniMap),
    mLastErrorString("")
    {
        mTempOutputFileNames.clear();
    }

    MiniMapMaker::~MiniMapMaker()
    {
        destroy();
    }

    void MiniMapMaker::init(void)
    {
        // ��������
        Ogre::TexturePtr pTexture = Ogre::TextureManager::getSingleton().createManual(
            "RttTex", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            Ogre::TEX_TYPE_2D,
            mTexWidth, mTexHeight, 1, 0, mOutPutFormat,
            Ogre::TU_RENDERTARGET, 0);

        Ogre::HardwarePixelBufferSharedPtr pBuffer = pTexture->getBuffer(0, 0);

        mRenderTexture = pBuffer->getRenderTarget(0);
        {
            mCamera = mManipulator->getSceneManager()->createCamera("RttCam");

            // ����������Ļ�������
            mCamera->setAspectRatio(1);
            mCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
            mCamera->setFOVy(Ogre::Degree(90));            

            _setCameraAttribute();

            Ogre::Viewport *v = mRenderTexture->addViewport( mCamera );
            v->setClearEveryFrame( true );
            v->setBackgroundColour( Ogre::ColourValue::Black );   
            v->setOverlaysEnabled(false);
            v->setSkiesEnabled(false);
            v->setShadowsEnabled(true);
        }
    }

    void MiniMapMaker::destroy(void)
    {
        if (mRenderTexture)
        {
            mManipulator->getSceneManager()->destroyCamera("RttCam");

            mRenderTexture->removeAllViewports();

            Ogre::TextureManager::getSingleton().remove("RttTex");

            mRenderTexture = NULL;
        }
    }

    bool MiniMapMaker::outputTextures(void)
    {
        // �����Ҫ�������С�ı��˻��һ������ļ�ʱ�������ؽ�render texture
        if (mNeedRecreate)
        {
            destroy();
            init();
        }

        mTempOutputFileNames.clear();

        static const String TEMP_GROUP_NAME = "#TEMP#";

        // ������ʱ����Դ��
        Ogre::ResourceGroupManager& rgm = Ogre::ResourceGroupManager::getSingleton();
        rgm.addResourceLocation(mPath, "FileSystem", TEMP_GROUP_NAME, false);

		mCamera->setOrthoWindowHeight(mTileSize);
		Ogre::Vector3 position = mCamera->getPosition();
        // ����������ĸ߶�
        Real yPos = mCamera->getNearClipDistance() + mManipulator->getSceneManager()->getSkyDomeNode()->getAttachedObject("SkyDomePlane0")->getWorldBoundingBox(true).getMaximum().y;  ;
	
        TerrainData* terrainData = mManipulator->getTerrainData();
        assert (terrainData);

        float terrainHeight = terrainData->mMaxZ - terrainData->mMinZ;
        float terrainWidth = terrainData->mMaxX - terrainData->mMinX;

        // ͶӰ���������
        Real projectSize = 0.0f;

        // �����и��С������Ŀ���
        int xIndex = 0;
        int zIndex = 0;

        Ogre::Vector3 originPoint(Ogre::Vector3::ZERO);

        if (mUseRealCameraAngle)
        {
            float outerSquareWidth = 0.0f;
            float outerSquareHeight = 0.0f;

            Ogre::Radian alphaAngle = Ogre::Math::ATan( Ogre::Math::Abs(mMoveZDir.z / mMoveZDir.x) );

            switch (mCameraDirQuadrant)
            {
            case WestNorth :
                {
                    float leftWidth = Ogre::Math::Sin(alphaAngle) * terrainHeight;
                    float rightWidth = Ogre::Math::Cos(alphaAngle) * terrainWidth;
                    outerSquareWidth = leftWidth + rightWidth;

                    float topHeight = Ogre::Math::Cos(alphaAngle) * terrainHeight;
                    float bottomHeight = Ogre::Math::Sin(alphaAngle) * terrainWidth;

                    outerSquareHeight = topHeight + bottomHeight;

                    originPoint = Ogre::Vector3(terrainData->mMinX,0,terrainData->mMinZ) +
                        (-mMoveZDir * leftWidth);

                    float projectOffset = yPos / Ogre::Math::Tan(mCamDirAngle);

                    originPoint.x += (mInvertCameraDir * projectOffset ).x;
                    originPoint.z += (mInvertCameraDir * projectOffset ).z;

                    break;
                }
            case EastNorth :
                {
                    float leftWidth = Ogre::Math::Cos(alphaAngle) * terrainWidth;
                    float rightWidth = Ogre::Math::Sin(alphaAngle) * terrainHeight;
                    outerSquareWidth = leftWidth + rightWidth;

                    float topHeight = Ogre::Math::Cos(alphaAngle) * terrainHeight;
                    float bottomHeight = Ogre::Math::Sin(alphaAngle) * terrainWidth;

                    outerSquareHeight = topHeight + bottomHeight;

                    originPoint = Ogre::Vector3(terrainData->mMaxX,0,terrainData->mMinZ) +
                        (-mMoveZDir * leftWidth);

                    float projectOffset = yPos / Ogre::Math::Tan(mCamDirAngle);

                    originPoint.x += (mInvertCameraDir * projectOffset ).x;
                    originPoint.z += (mInvertCameraDir * projectOffset ).z;

                    break;
                }
            case EastSouth :
                {
                    float leftWidth = Ogre::Math::Sin(alphaAngle) * terrainHeight;
                    float rightWidth = Ogre::Math::Cos(alphaAngle) * terrainWidth;
                    outerSquareWidth = leftWidth + rightWidth;

                    float topHeight = Ogre::Math::Sin(alphaAngle) * terrainWidth;
                    float bottomHeight = Ogre::Math::Cos(alphaAngle) * terrainHeight;

                    outerSquareHeight = topHeight + bottomHeight;

                    originPoint = Ogre::Vector3(terrainData->mMaxX,0,terrainData->mMaxZ) +
                        (-mMoveZDir * topHeight);

                    float projectOffset = yPos / Ogre::Math::Tan(mCamDirAngle);

                    originPoint.x += (mInvertCameraDir * projectOffset ).x;
                    originPoint.z += (mInvertCameraDir * projectOffset ).z;

                    break;
                }
            case WestSouth :
                {
                    float leftWidth = Ogre::Math::Sin(alphaAngle) * terrainHeight;
                    float rightWidth = Ogre::Math::Cos(alphaAngle) * terrainWidth;
                    outerSquareWidth = leftWidth + rightWidth;

                    float topHeight = Ogre::Math::Sin(alphaAngle) * terrainWidth;
                    float bottomHeight = Ogre::Math::Cos(alphaAngle) * terrainHeight;

                    outerSquareHeight = topHeight + bottomHeight;

                    originPoint = Ogre::Vector3(terrainData->mMinX,0,terrainData->mMaxZ) +
                        (-mMoveZDir * rightWidth);

                    float projectOffset = yPos / Ogre::Math::Tan(mCamDirAngle);

                    originPoint.x += (mInvertCameraDir * projectOffset ).x;
                    originPoint.z += (mInvertCameraDir * projectOffset ).z;

                    break;
                }

            default:
                {
                    OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        " wrong camera dir " + Ogre::StringConverter::toString(mCameraDir),
                        "MiniMapMaker::outputTextures");

                    break;
                }
            }

            // ����ͶӰ�ĳ���
            Real factor = Ogre::Math::Sin(mCamDirAngle);

            if (factor > 0.0f && factor != 1.0f)
                projectSize = mTileSize / factor;

            // ���ݵ�ǰ�����Ĵ�С��������Ҫ�ķֿ���
            xIndex = Ogre::Math::Ceil( (outerSquareWidth) / mTileSize ) + 1;
            zIndex = Ogre::Math::Ceil( (outerSquareHeight) / projectSize ) + 1;
        }
        else
        {
            xIndex = Ogre::Math::Ceil( (terrainData->mMaxX - terrainData->mMinX) / mTileSize ) + 1;
            zIndex = Ogre::Math::Ceil( (terrainData->mMaxZ - terrainData->mMinZ) / mTileSize ) + 1;

            originPoint.x = terrainData->mMinX;
            originPoint.z = terrainData->mMinZ;
        }

        // �������յ�mini map�Ĵ�С
        uint miniMapWidth = xIndex * mTexWidth;
        uint miniMapHeight = zIndex * mTexHeight;

        if ( miniMapWidth > 10000 || miniMapHeight > 10000 )
        {
            mLastErrorString = "texture size is out of range!";
            return false;
        }
        // ����mini map������ڴ�ռ�
        uchar* miniMapData = new uchar[miniMapWidth * miniMapHeight * Ogre::PixelUtil::getNumElemBytes(mOutPutFormat)];

        //// ��ʼ�������λ��
        Real xPos = originPoint.x;
        Real zPos = originPoint.z;

        for ( int i=0; i<xIndex; ++i )
        {
            for ( int j=0; j<zIndex; ++j )
            {
                // ���������λ�ã�������render texture������
                mCamera->setPosition(xPos, yPos, zPos);
                mRenderTexture->update();

                String fileName = mPath + mSceneBaseName + Ogre::StringConverter::toString(i)
                    + "_" + Ogre::StringConverter::toString(j) + "." + mTexExtension;

                // ���С�����ļ�
                mRenderTexture->writeContentsToFile(fileName);

                mTempOutputFileNames.push_back(fileName);

                // ��ȡ�մ���������
                Ogre::Image* tempImage = new Ogre::Image;
                
                tempImage->load(mSceneBaseName + Ogre::StringConverter::toString(i)
                    + "_" + Ogre::StringConverter::toString(j) + "." + mTexExtension, TEMP_GROUP_NAME);

                // ��ȡrender texture�е�����
                uchar* tempImageData = tempImage->getData();

                // ��λ��mini map�е����Ͻ�
                uint miniMapIndex = ( j * mTexHeight * miniMapWidth + i * mTexWidth ) * Ogre::PixelUtil::getNumElemBytes(mOutPutFormat);

                uchar* startData = miniMapData + miniMapIndex;

                for ( size_t height = 0; height < tempImage->getHeight(); ++height )
                {
                    for ( size_t width = 0; width < tempImage->getWidth(); ++width )
                    {
                        memcpy(startData, tempImageData, Ogre::PixelUtil::getNumElemBytes(mOutPutFormat));

                        startData += Ogre::PixelUtil::getNumElemBytes(mOutPutFormat);
                        tempImageData += Ogre::PixelUtil::getNumElemBytes( tempImage->getFormat() );
                    }
                    startData += (miniMapWidth - tempImage->getWidth()) * Ogre::PixelUtil::getNumElemBytes(mOutPutFormat);
                }

                delete tempImage;

                // �ƶ��������z����
                if (mUseRealCameraAngle)
                {
                    zPos += (mInvertCameraDir * (projectSize)).z;
                    xPos += (mInvertCameraDir * (projectSize)).x;
                }
                else
                   zPos += mTileSize;
            }
            
            if (mUseRealCameraAngle)
            {
                xPos = originPoint.x;
                zPos = originPoint.z;

                xPos += (mMoveZDir * (mTileSize) * (i+1)).x;
                zPos += (mMoveZDir * (mTileSize) * (i+1)).z;
            }
            else
            {
                // ������ÿһ��֮�������������z����
                zPos = terrainData->mMinZ;

                // �ƶ��������x����
                xPos += mTileSize;
            }
        } 

        // ����mini map�����
        Ogre::Image* miniMapImage = new Ogre::Image;
        miniMapImage->loadDynamicImage(miniMapData, miniMapWidth, miniMapHeight, 1, mOutPutFormat, false);

        miniMapImage->save(mPath + mOutFileName + "." + mTexExtension);

        delete miniMapImage;
		delete[] miniMapData;
        rgm.destroyResourceGroup(TEMP_GROUP_NAME);

        return true;
    }

    void MiniMapMaker::setCurrentSceneName(const String& sceneName)
    {
        // ��ȡ·���ͳ�������
        Ogre::StringUtil::splitFilename(sceneName, mSceneBaseName, mPath);

        if (Ogre::StringUtil::endsWith(mSceneBaseName, ".scene"))
            mSceneBaseName.erase(mSceneBaseName.length() - (sizeof(".scene") - 1));
    }

    void MiniMapMaker::setTexWidth(uint width)
    {
        mTexWidth = width;
        mNeedRecreate = true;
    }

    void MiniMapMaker::setTexHeight(uint height)
    {
        mTexHeight = height;
        mNeedRecreate = true;
    }

    void MiniMapMaker::setTexExtension( const String& extension )
    {
        mTexExtension = extension;

        if (mTexExtension == "png" || mTexExtension == "jpg")
            mOutPutFormat = Ogre::PF_BYTE_BGRA;
        else if (mTexExtension == "bmp")
            mOutPutFormat = Ogre::PF_BYTE_BGR;

        mNeedRecreate;
    }
    void MiniMapMaker::setCameraPerAngle(Real angle)
    {
        mPerAngle = angle;   

        /** dirΪĿ�귽��
        ��֪���� : dir.y == -sin(alpha) �� ԭ�з��� curDir
        /
        /
        /   
        alpha   \/angle
        -------------------------------
        ����Ҫ���dir��x��z��
        (curDir.x/s)^2 + dir.y^2 + (curDir.y/s)^2 == 1
        */
        Ogre::Vector3 direction = mCamera->getDirection();
        Real v = Ogre::Math::Sin(Ogre::Math::DegreesToRadians(angle));
        Real s = Ogre::Math::Sqrt((direction.x*direction.x + direction.z*direction.z) / (1-v*v));
        direction.x /= s;
        direction.z /= s;
        direction.y = -v;
        mCamera->setDirection(direction);

    }

    void MiniMapMaker::_computeCameraDirQuadrant(void)
    {
        float x = mCameraDir.x;
        float z = mCameraDir.z;

        if ( x < 0.0f && z < 0.0f )
            mCameraDirQuadrant = WestNorth;

        else if ( x > 0.0f && z < 0.0f )
            mCameraDirQuadrant = EastNorth;

        else if ( x < 0.0f && z > 0.0f )
            mCameraDirQuadrant = WestSouth;

        else if ( x > 0.0f && z > 0.0f )
            mCameraDirQuadrant = EastSouth;
        else
            mCameraDirQuadrant = None;
    }

    void MiniMapMaker::_setCameraAttribute(void)
    {
        switch (mCameraDirectionType)
        {
        case CDT_MiniMap :
            {
                // ������������ӽǴ�ֱ����
                Ogre::Quaternion orientation;
                orientation.FromAxes(Ogre::Vector3::UNIT_X, Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::UNIT_Y);
                mCamera->setOrientation(orientation);

                break;
            }

        case CDT_CameraDirection :
            {
                // ��ȡ��ǰ������ķ���
                mCameraDir = mManipulator->getCamera()->getDirection();

                // ������������������ɵĽǶ�
                float tanValue = 
                    Ogre::Math::Abs(mCameraDir.y) / Ogre::Math::Sqrt(mCameraDir.x*mCameraDir.x + mCameraDir.z*mCameraDir.z);

                mCamDirAngle = Ogre::Math::ATan(tanValue);

                // ����Ҫ����С��ͼ���յ�������ķ���
                mCamera->setDirection(mCameraDir);

                // ������������ʱ�ĺ����ƶ��ķ��򣨸÷�����������ķ����Ǵ�ֱ�ģ�
                mMoveZDir = mCameraDir.crossProduct(Ogre::Vector3::UNIT_Y).normalisedCopy();

                // ������������ʱ�������ƶ��ķ��򣨸÷�����������ķ������෴�ģ�
                mInvertCameraDir = -mCameraDir;
                // ��Ϊ����ֻ������xzƽ����ƶ�������y������0
                mInvertCameraDir.y = 0.0f;
                mInvertCameraDir.normalise();

                // �жϵ�ǰ�����������ָ���ĸ������
                _computeCameraDirQuadrant();

                mUseRealCameraAngle = true;

                break;
            }

        case CDT_North :
            {
                break;
            }

        case CDT_South :
            {
                break;
            }

        case CDT_West :
            {
                break;
            }

        case CDT_East :
            {
                break;
            }

        default :
            {
                break;
            }
        }        
    }
}
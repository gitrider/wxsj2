/********************************************************************
filename:   WXMiniMapMaker.h

purpose:    ���ݳ������ݣ��Զ�����һ��mini map
*********************************************************************/

#ifndef _MINIMAPMAKER_H_
#define _MINIMAPMAKER_H_

#include "Core/FairyPrerequisites.h"

#include <OgrePixelFormat.h>
#include <OgreVector3.h>

namespace Ogre  {
    class RenderTexture;
    class Camera;
}

namespace Fairy {

    class SceneManipulator;

    class MiniMapMaker
    {
    public:

        enum CameraDirectionType
        {
            CDT_MiniMap,
            CDT_CameraDirection,
            CDT_North,
            CDT_South,
            CDT_West,
            CDT_East,
        };

    public:

        MiniMapMaker( SceneManipulator* manipulator );

        ~MiniMapMaker();

        /// ��ʼ������������render texture��camera��
        void init(void);
        /// ����render texture
        void destroy(void);

        /// ���mini map
        bool outputTextures(void);

        /**
        ����ÿ��С�����ϸ��ǵı߳�
        @param tileSize �߳����������꣬��1600��ʾÿ��С�������µ��������1600*1600
        */
        void setTileSize(Real tileSize)
        {
            mTileSize = tileSize;
        }
        const Real getTileSize(void)
        {
            return mTileSize;
        }

        void setCameraPerAngle(Real angle);

        /// ��������ļ���չ�����硰png��
        void setTexExtension( const String& extension );
        const String& getTexExtension(void)
        {
            return mTexExtension;
        }

        /// ����С����Ŀ��
        void setTexWidth(uint width);
        const uint getTexWidth(void)
        {
            return mTexWidth;
        }

        /// ����С����ĸ߶�
        void setTexHeight(uint height);
        const uint getTexHeight(void)
        {
            return mTexHeight;
        }

        /**
            ��������ļ���
            Ĭ�������ǳ�������
        */
        void setOutputFileName(const String& fileName)
        {
            mOutFileName = fileName;
        }
        const String& getOutputFileName(void)
        {
            return mOutFileName;
        }

        /// ���õ�ǰ���������ƣ�����·����
        void setCurrentSceneName(const String& sceneName);

        /// ��ȡ��������
        const String& getBaseName(void)
        {
            return mSceneBaseName;
        }

        /// ��ȡ�м����ɵ�С������ļ���������ɾ��
        std::vector<String>& getTempFileNames(void)
        {
            return mTempOutputFileNames;
        }

        void setUseRealCameraAngle(bool use)
        {
            mUseRealCameraAngle = use;
        }

        void setCameraDirectionType(CameraDirectionType dirType)
        {
            mCameraDirectionType = dirType;
        }

        const String& getLastErrorString(void)
        {
            return mLastErrorString;
        }

    private:
        
        void _computeCameraDirQuadrant(void);

        void _setCameraAttribute(void);    

    private:

        Ogre::Camera* mCamera;
        Ogre::RenderTexture* mRenderTexture;

        uint mTexHeight;
        uint mTexWidth;

        SceneManipulator* mManipulator;

        Real mTileSize;
        Real mPerAngle;

        /// ��������������ļн�
        Ogre::Radian mCamDirAngle;

        String mTexExtension;
        String mOutFileName;

        String mSceneBaseName;
        String mPath;

        std::vector<String> mTempOutputFileNames;

        bool mNeedRecreate;

        Ogre::PixelFormat mOutPutFormat;

        bool mUseRealCameraAngle;

        Ogre::Vector3 mMoveZDir;

        /// ��ǰ������������ķ���
        Ogre::Vector3 mCameraDir;

        /// ���������ķ���
        Ogre::Vector3 mInvertCameraDir;

        enum CameraDirQuadrant
        {
            WestNorth,
            EastNorth,
            EastSouth,
            WestSouth,
            None,
        };

        CameraDirQuadrant mCameraDirQuadrant; 

        CameraDirectionType mCameraDirectionType;

        String mLastErrorString;
    };
}

#endif
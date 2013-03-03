#ifndef __TerrainSelectionAction_H__
#define __TerrainSelectionAction_H__

#include "Action.h"
#include <OgreVector3.h>

namespace Fairy {
    
	class TerrainSelectionGrids;

    class TerrainSelectionAction : public Action
    {
    public:
        TerrainSelectionAction(SceneManipulator* sceneManipulator);

        ~TerrainSelectionAction();

        virtual void setParameter(const String& name, const String& value);
        virtual String getParameter(const String& name) const;
    
        const String& getName(void) const;

        void CopyHeightInfo( bool copy = true )     {   mPasteHeightInfo = copy; }
		void CppyDiffuseInfo(bool copy = true )		{	mPasteDiffuseInfo = copy; }
        void CopyTextureInfo( bool copy = true )    {   mPasteTextureInfo = copy; }
        void CopyDiagonalInfo( bool copy = true )   {   mPasteDiagonalInfo = copy; }
       

    protected:
        virtual void _onActive(bool active);
        virtual void _onMove(const Point& pt);
        virtual void _onBegin(const Point& pt);
        virtual void _onDrag(const Point& pt);
        virtual void _onEnd(const Point& pt, bool canceled);

        void getJunctionInfoAsString(void) const;
        void getGridTextureInfoAsString(void) const;
        void getGridDiagonalInfoAsString(void) const;
		void getJunctionDiffuseInfoAsString(void) const;

        void pasteTerrainInfo(void);

		void beginRotateTerrain();

    protected:

        /// ѡ�����ʱ��ָʾ��
        TerrainSelectionGrids *mTerrainSelectionGrids;
        /// ��ǰ��ѡ��ģʽ����ѡ��ѡ��
        Ogre::String mCurrentMode;

        /// 4���ַ��������ڱ��浱ǰѡ����ε���Ϣ
        mutable Ogre::String mJunctionHeightInfoString;
		mutable Ogre::String mJunctionDiffuseInfoString;
        mutable Ogre::String mGridTextureInfoString;
        mutable Ogre::String mGridDiagonalInfoString;

        /// ��ǰҪ����ʱ�����λ��
        Ogre::Vector3 mCurrentMousePos;

        /// ��ǰ���ճ��Ҫճ����Щ��Ϣ
        bool mPasteHeightInfo;
		bool mPasteDiffuseInfo;
        bool mPasteTextureInfo;
        bool mPasteDiagonalInfo;

		/// �Ƿ��Ѿ�������ctrl+C�ˣ�Ҳ����˵�Ƿ����˿�����Ϣ������Ѿ������ˣ����ο�Ϳ��Ը����ƶ�
		/// Ҳ���ԶԵ��ο������ת
		bool mAfterCopy;
		int mRotateCount;

		/*typedef std::vector<float> HeightArray;
		mutable HeightArray mHeightArray;
		HeightArray mRotatedHeightArray;

		mutable int mMaxJunctionX;
		mutable int mMaxJunctionZ;*/
    };
}

#endif // __TerrainSelectionAction_H__
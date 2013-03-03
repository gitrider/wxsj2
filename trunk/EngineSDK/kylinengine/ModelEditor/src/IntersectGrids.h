#ifndef __IntersectGrids_H__
#define __IntersectGrids_H__

#include "HitIndicator.h"
#include "TerrainSelections.h"

#include <OgreSimpleRenderable.h>
#include <OgreVector2.h>
#include <OgreSceneNode.h>

#include <list>

namespace Fairy {
    
    class Projector;

    typedef std::vector<Ogre::Vector3> PositionArray;

    /** �������е�ָʾ���飬��������Ⱦ
    */
    class IntersectGrid : public Ogre::SimpleRenderable
    {
    public:
        IntersectGrid();
        ~IntersectGrid();

        /// ��䶥�㻺����
        void fillPosition( const PositionArray &posArray );

        Ogre::Real getBoundingRadius() const;

        Ogre::Real getSquaredViewDepth(const Ogre::Camera*) const;

        void setGridColour( const Ogre::ColourValue &colour )
        {
            mGridsColour = colour;
        }        

    protected:

        Ogre::HardwareVertexBufferSharedPtr vbuf;

        float mRadius;

        size_t mCurrentVertexCount;

        Ogre::ColourValue mGridsColour;

    protected:

        void _createBuffer(void);

        /// �����������õĲ��ʣ���͸���������ܹ��գ�
        Ogre::MaterialPtr _createIntersectGridMaterial(void);        
    };

    /** ����ָʾ���������������ʱ�ڵ����ϵ�ָʾ
    */
    class IntersectGrids : public HitIndicator
    {       

    protected:

        /// ���ڹҽӲ���ָʾ��Ƭ��SceneNode
        Ogre::SceneNode *mIntersectGridsNode;

        /// ����renderable
        IntersectGrid *intersectGrids;

        /// ���浱ǰˢ�ӷ�Χ�Ķ�����Ϣ
        PositionArray mPosArray;

    public:
        IntersectGrids(SceneManipulator* sceneManipulator);
        ~IntersectGrids();

        const String& getName(void) const;

        void hide(void);
        void refresh(void);

    protected:

        void _setHitPoint(Real x, Real y);
    };

    /** ��ת�Խ��߲�����ָʾ��
    */
    class FlipDiagonalGrids : public IntersectGrids
    {
    public:
        FlipDiagonalGrids(SceneManipulator* sceneManipulator);

        const String& getName(void) const;
    };  

    /** Disable Decal������ָʾ��
    */
    class DisableDecalGrids : public IntersectGrids
    {
    public:
        DisableDecalGrids(SceneManipulator* sceneManipulator);

        ~DisableDecalGrids();

        const String& getName(void) const;

        void hide(void);

    protected:

        virtual void _setHitPoint(Real x, Real y);

        void _createProjector(void);

        void _destroyProjector(void);

        void _setProjcetorSize(void);

    protected:

        Projector* mProjector;
        Ogre::SceneNode* mProjectorNode;
    };

    /** ���ڵ���ѡ���ָʾ�� 
        ����ͨ��ָʾ���Ĳ�����ڣ����Ը�ѡ�������
    */
    class TerrainSelectionGrids : public IntersectGrids
    {
    public:

        TerrainSelectionGrids(SceneManipulator* sceneManipulator);
        ~TerrainSelectionGrids();

        const String& getName(void) const;

        virtual void reshape(void);

        virtual void refresh(void);

        void setCurrentGridSelection( bool multiSelecting = true );

        void addCurrentGridSelection( bool multiSelecting  = true );

        GridSelection *getTotalGridSelection(void)
        {
            return mTotalGridSelection;
        }

        JunctionSelection *getTotalJunctionSelection(void)
        {
            return mTotalJunctionSelection;
        }

        void show(void);

		int getMinX(void)
		{
			return mMinX;
		}

		int getMinZ(void)
		{
			return mMinZ;
		}

		int getMaxX(void)	{	return mMaxX;	}
		int getMaxZ(void)	{	return mMaxZ;	}

		void setSceneNodePos(const Ogre::Vector3 &pos)
		{
			mIntersectGridsNode->setPosition(pos);
		}

		void rotateSceneNode(void)
		{
			mIntersectGridsNode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(90.0f));
		}

		void setOriginSceneNodeOrientation(void)
		{
			mIntersectGridsNode->setOrientation(Ogre::Quaternion::IDENTITY);
		}
        
    protected:

        void _setHitPoint(Real x, Real y);

        void _clearGridSelections(void);		

    protected:

        // ÿ�����ѡ�����ʱ�����ԭ��
        Ogre::Vector2 mOriginPos;

        // ����ѡ�����飬���ڱ���ͬʱѡ�еĶ������
        typedef std::list<GridSelection *> GridSelections;
        GridSelections mGridSelectionList;

        // ��ǰ����ѡ��ĵ��ο�       
        GridSelection *mCurrentSelection;

        GridSelection *mTotalGridSelection;
        JunctionSelection *mTotalJunctionSelection;

		int mMinX;
		int mMinZ;

		int mMaxX;
		int mMaxZ;
    };

}

#endif // __IntersectGrids_H__

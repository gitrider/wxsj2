#include "WXModifyTerrainPaintOperator.h"
#include "SceneManipulator.h"

#include "Core/Terrain.h"
#include "PaintAction.h"
#include <OgreTextureManager.h>

namespace Fairy
{    
    ModifyTerrainPaintOperator::ModifyTerrainPaintOperator(SceneManipulator* sceneManipulator)
        : mSceneManipulator(sceneManipulator)
        , mInfos()
    {
        assert(sceneManipulator);
    }
    //-----------------------------------------------------------------------
    ModifyTerrainPaintOperator::~ModifyTerrainPaintOperator()
    {
    }
    //-----------------------------------------------------------------------
    void ModifyTerrainPaintOperator::add(int x, int z, const TerrainData::GridInfo &oldGridInfo, const TerrainData::GridInfo &newGridInfo)
    {
        assert(mSceneManipulator->getTerrainData()->isValidGrid(x, z));

        if (mInfos.empty())
        {
            mMinX = mMaxX = x;
            mMinZ = mMaxZ = z;
        }
        else
        {
            if (mMinX > x)
                mMinX = x;
            else if (mMaxX < x)
                mMaxX = x;

            if (mMinZ > z)
                mMinZ = z;
            else if (mMaxZ < z)
                mMaxZ = z;
        }

        // �õ���ǰgridinfo�е���������

        Info info;
        info.x = x;
        info.z = z;
        fillUndoRedoInfo(oldGridInfo, info.oldGridInfo);
        fillUndoRedoInfo(newGridInfo, info.newGridInfo);

        mInfos.push_back(info);
    }
    //-----------------------------------------------------------------------
    bool ModifyTerrainPaintOperator::empty(void) const
    {
        return mInfos.empty();
    }
    //-----------------------------------------------------------------------
    const String& ModifyTerrainPaintOperator::getGroupName(void) const
    {
        static const String groupName = "TerrainPaint";
        return groupName;
    }
    //-----------------------------------------------------------------------
    String ModifyTerrainPaintOperator::getDescription(void) const
    {
        // TODO:
        return "";
    }
    //-----------------------------------------------------------------------
    String ModifyTerrainPaintOperator::getHelp(void) const
    {
        // TODO
        return "";
    }
    //-----------------------------------------------------------------------
    void ModifyTerrainPaintOperator::undo(void)
    {

		PaintAction* pAction = dynamic_cast<PaintAction*>(mSceneManipulator->getActiveAction());
		if(pAction)
			 pAction->resetHintModified();//��չ�굱ǰ��ʾ�ı�ˢ����ֹӰ��undo��������

        apply(&Info::oldGridInfo);
    }
    //-----------------------------------------------------------------------
    void ModifyTerrainPaintOperator::redo(void)
    {
		PaintAction* pAction = dynamic_cast<PaintAction*>(mSceneManipulator->getActiveAction());
		if(pAction)
			pAction->resetHintModified(); //��չ�굱ǰ��ʾ�ı�ˢ����ֹӰ��redo��������

        apply(&Info::newGridInfo);
    }
    //-----------------------------------------------------------------------
    void ModifyTerrainPaintOperator::fillUndoRedoInfo(const TerrainData::GridInfo &gridInfo, UndoRedoGridInfo &fillInfo)
    {
        // ����redo undo�������Ϣ
        TerrainData *pTerrainData = mSceneManipulator->getTerrainData();

        /** ԭ����ֻ���浱ǰ���ӵ������id������Ҫ����
        1 ��������
        2 ������������������
        3 ����ķ���
        */
        // �����һ���������Ϣ
        int id = gridInfo.layers[0].pixmapId;

        if ( id == 0 )
        {
            fillInfo.mPaintInfo[0].mTexName.clear();
            fillInfo.mPaintInfo[0].left = 0.0f;
            fillInfo.mPaintInfo[0].right = 1.0f;
            fillInfo.mPaintInfo[0].top = 0.0f;
            fillInfo.mPaintInfo[0].bottom = 1.0f;
            fillInfo.mPaintInfo[0].orientation = 0;
        }
        else
        {
            fillInfo.mPaintInfo[0].mTexName = pTerrainData->mTextures[ pTerrainData->mPixmaps[id-1].textureId ];
            fillInfo.mPaintInfo[0].left = pTerrainData->mPixmaps[id-1].left;
            fillInfo.mPaintInfo[0].right = pTerrainData->mPixmaps[id-1].right;
            fillInfo.mPaintInfo[0].top = pTerrainData->mPixmaps[id-1].top;
            fillInfo.mPaintInfo[0].bottom = pTerrainData->mPixmaps[id-1].bottom;
            fillInfo.mPaintInfo[0].orientation = gridInfo.layers[0].orientation;
        }
        // ����ڶ����������Ϣ������ڶ���������Ļ���
        id = gridInfo.layers[1].pixmapId;
        if ( id > 0 )
        {
            fillInfo.mPaintInfo[1].mTexName = pTerrainData->mTextures[ pTerrainData->mPixmaps[id-1].textureId ];
            fillInfo.mPaintInfo[1].left = pTerrainData->mPixmaps[id-1].left;
            fillInfo.mPaintInfo[1].right = pTerrainData->mPixmaps[id-1].right;
            fillInfo.mPaintInfo[1].top = pTerrainData->mPixmaps[id-1].top;
            fillInfo.mPaintInfo[1].bottom = pTerrainData->mPixmaps[id-1].bottom;
            fillInfo.mPaintInfo[1].orientation = gridInfo.layers[1].orientation;
        }

    }
    //-----------------------------------------------------------------------
    void ModifyTerrainPaintOperator::apply(UndoRedoGridInfo Info::* undoRedo)
    {
        if (!mInfos.empty())
        {
            for (InfoList::const_iterator i = mInfos.begin(); i != mInfos.end(); ++i)
            {
                const Info& info = *i;
                assert(mSceneManipulator->getTerrainData()->isValidGrid(info.x, info.z));

                // ȡ����ǰҪ����undo �� redo�ĸ��ӵ���Ϣ�������н��в����޸ģ���ǰ���ӵ���������ķ���
                const TerrainData::GridInfo &nowInfo = mSceneManipulator->getTerrainData()->getGridInfo(info.x, info.z);
                TerrainData::GridInfo newInfo = nowInfo;

                // ����ָ��ȡ���ṹ�е�����
                const UndoRedoGridInfo &undoRedoInfo = info.*undoRedo;

                // �����һ�����������Ϊ�գ�˵����û������
                if (undoRedoInfo.mPaintInfo[0].mTexName.empty())
                {
                    newInfo.layers[0].pixmapId = 0;
                }
                else
                {
                    int layerOnePixmapId = mSceneManipulator->getTerrainData()->_registerPixmap(
                        undoRedoInfo.mPaintInfo[0].mTexName,
                        undoRedoInfo.mPaintInfo[0].left,
                        undoRedoInfo.mPaintInfo[0].top,
                        undoRedoInfo.mPaintInfo[0].right,
                        undoRedoInfo.mPaintInfo[0].bottom);

                    newInfo.layers[0].pixmapId = layerOnePixmapId;    
                }  

                newInfo.layers[0].orientation = undoRedoInfo.mPaintInfo[0].orientation;

                if (undoRedoInfo.mPaintInfo[1].mTexName.empty())
                {
                    newInfo.layers[1].pixmapId = 0;
                }
                else
                {
                    int layerTwoPixmapId = mSceneManipulator->getTerrainData()->_registerPixmap(
                        undoRedoInfo.mPaintInfo[1].mTexName,
                        undoRedoInfo.mPaintInfo[1].left,
                        undoRedoInfo.mPaintInfo[1].top,
                        undoRedoInfo.mPaintInfo[1].right,
                        undoRedoInfo.mPaintInfo[1].bottom);

                    newInfo.layers[1].pixmapId = layerTwoPixmapId;    
                }

                newInfo.layers[1].orientation = undoRedoInfo.mPaintInfo[1].orientation;

                mSceneManipulator->getTerrainData()->setGridInfo(info.x, info.z, newInfo);
            }

            mSceneManipulator->getTerrain()->notifyGridInfoModified(mMinX, mMinZ, mMaxX+1, mMaxZ+1);
        }
    }
}

#include "WXModifyTerrainDiffuseOperator.h"
#include "SceneManipulator.h"

#include "Core/Terrain.h"
#include "DragTerrainColourAction.h"
#include <OgreTextureManager.h>

namespace Fairy
{    
    ModifyTerrainDiffuseOperator::ModifyTerrainDiffuseOperator(SceneManipulator* sceneManipulator)
        : mSceneManipulator(sceneManipulator)
        , mInfoList()
    {
        assert(sceneManipulator);
    }
    //-----------------------------------------------------------------------
    ModifyTerrainDiffuseOperator::~ModifyTerrainDiffuseOperator()
    {
    }
    //-----------------------------------------------------------------------
    void ModifyTerrainDiffuseOperator::add(int x, int z, const Ogre::ColourValue & oldDifffuse, const Ogre::ColourValue & newDifffuse)
    {
        assert(mSceneManipulator->getTerrainData()->isValidGrid(x, z));

        if (mInfoList.empty())
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

        // 得到当前gridinfo中的纹理名称

        DiffuseInfo info;
        info.x = x;
        info.z = z;
		info.oldDifffuse = oldDifffuse;
		info.newDifffuse = newDifffuse;

        mInfoList.push_back(info);
    }
    //-----------------------------------------------------------------------
    bool ModifyTerrainDiffuseOperator::empty(void) const
    {
        return mInfoList.empty();
    }
    //-----------------------------------------------------------------------
    const String& ModifyTerrainDiffuseOperator::getGroupName(void) const
    {
        static const String groupName = "TerrainDiffuse";
        return groupName;
    }
    //-----------------------------------------------------------------------
    String ModifyTerrainDiffuseOperator::getDescription(void) const
    {
        // TODO:
        return "";
    }
    //-----------------------------------------------------------------------
    String ModifyTerrainDiffuseOperator::getHelp(void) const
    {
        // TODO
        return "";
    }
    //-----------------------------------------------------------------------
    void ModifyTerrainDiffuseOperator::undo(void)
    {

		//DragTerrainColourAction* pAction = dynamic_cast<DragTerrainColourAction*>(mSceneManipulator->getActiveAction());
		
        apply(&DiffuseInfo::oldDifffuse);
    }
    //-----------------------------------------------------------------------
    void ModifyTerrainDiffuseOperator::redo(void)
    {
		//DragTerrainColourAction* pAction = dynamic_cast<DragTerrainColourAction*>(mSceneManipulator->getActiveAction());
		
        apply(&DiffuseInfo::newDifffuse);
    }
   
    //-----------------------------------------------------------------------
    void ModifyTerrainDiffuseOperator::apply(Ogre::ColourValue DiffuseInfo::* undoRedo)
    {
        if (!mInfoList.empty())
        {
            for (DiffuseInfoList::const_iterator i = mInfoList.begin(); i != mInfoList.end(); ++i)
            {
                const DiffuseInfo& info = *i;
                assert(mSceneManipulator->getTerrainData()->isValidGrid(info.x, info.z));
               
                // 根据指针取出结构中的内容
				const Ogre::ColourValue &undoRedoInfo = info.*undoRedo;
               
                mSceneManipulator->getTerrainData()->setDiffuseInfo(info.x, info.z, undoRedoInfo);
            }

            mSceneManipulator->getTerrain()->notifyColourModified(mMinX, mMinZ, mMaxX+1, mMaxZ+1);
        }
    }
}

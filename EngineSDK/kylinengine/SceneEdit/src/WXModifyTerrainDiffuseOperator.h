#ifndef __FAIRYMODIFYTERRAINDIFFUSEOPERATOR_H__
#define __FAIRYMODIFYTERRAINDIFFUSEOPERATOR_H__

#include "WXOperator.h"
#include "Core/TerrainData.h"

namespace Fairy
{
    class SceneManipulator;

    /**
    这个类用于记录对地形纹理进行的修改，包括第一层的橡皮擦，自动纹理拼接，简单画刷等操作（从PaintAction派生的操作），
    用于redo，undo，大部分内容参考了ModifyTerrainHeightOperator类
    */
    class ModifyTerrainDiffuseOperator : public Operator
    {
    public:       

        ModifyTerrainDiffuseOperator(SceneManipulator* sceneManipulator);
        ~ModifyTerrainDiffuseOperator();

		void add(int x, int z, const Ogre::ColourValue & oldDifffuse, const Ogre::ColourValue & newDifffuse);
        bool empty(void) const;

        /** @copydoc Operator::getGroupName */
        const String& getGroupName(void) const;

        /** @copydoc Operator::getDescription */
        String getDescription(void) const;

        /** @copydoc Operator::getHelp */
        String getHelp(void) const;

        /** @copydoc Operator::undo */
        void undo(void);

        /** @copydoc Operator::redo */
        void redo(void);

    protected:
        SceneManipulator* mSceneManipulator;

        // 每次进行画刷修改时要保存的信息
		struct DiffuseInfo
		{
			int x;
			int z;
			Ogre::ColourValue oldDifffuse;		// 当前格子中的Diffuse
			Ogre::ColourValue newDifffuse;		// 当前格子要修改的Diffuse
		};
		typedef std::vector<DiffuseInfo>  DiffuseInfoList;
		DiffuseInfoList mInfoList;				//操作信息列表
		int mMinX, mMaxX, mMinZ, mMaxZ;			// 记录修改区域的最大，最小坐标

		void apply(Ogre::ColourValue DiffuseInfo::* undoRedo);

    };
}

#endif 

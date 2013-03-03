#ifndef __TERRAINSELECTIONACTIONOPERATOR_H__
#define __TERRAINSELECTIONACTIONOPERATOR_H__

#include "WXOperator.h"

namespace Fairy {

    class FlipTerrainDiagonalOperator;
    class ModifyTerrainHeightOperator;
    class ModifyTerrainPaintOperator;
	class ModifyTerrainDiffuseOperator;

    class SceneManipulator;

    /** ���operator���ڵ��εĿ�����ճ����������4��operator
    */
    class TerrainSelectionActionOperator : public Operator
    {
    public:

        TerrainSelectionActionOperator(SceneManipulator* sceneManipulator);
        ~TerrainSelectionActionOperator();

        bool empty(void) const;

        const String& getGroupName(void) const;

        String getDescription(void) const;

        String getHelp(void) const;

        void undo(void);

        void redo(void);

    public:

        FlipTerrainDiagonalOperator *mFlipDiagonalOperator;
        ModifyTerrainHeightOperator *mModifyHeightOperator;
        ModifyTerrainPaintOperator *mModifyPaintOperator;
		ModifyTerrainDiffuseOperator * mModifyTerrainDiffuseOperator;
    };
}

#endif // __TerrainSelectionActionOperator_H__
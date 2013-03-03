#ifndef __FAIRYMODIFYTERRAINPAINTOPERATOR_H__
#define __FAIRYMODIFYTERRAINPAINTOPERATOR_H__

#include "WXOperator.h"
#include "Core/TerrainData.h"

namespace Fairy
{
    class SceneManipulator;

    /**
    ��������ڼ�¼�Ե���������е��޸ģ�������һ�����Ƥ�����Զ�����ƴ�ӣ��򵥻�ˢ�Ȳ�������PaintAction�����Ĳ�������
    ����redo��undo���󲿷����ݲο���ModifyTerrainHeightOperator��
    */
    class ModifyTerrainPaintOperator : public Operator
    {
    public:

        /** undo redo Ҫ�����������Ϣ
        */
        struct PaintInfo
        {
            String mTexName;
            Real left;
            Real top;
            Real right;
            Real bottom;
            uchar orientation;

            PaintInfo()
                : left(0)
                , top(0)
                , right(1)
                , bottom(1)
                , orientation(0)
            {                
            }

            PaintInfo(String textureName, Real left, Real top, Real right, Real bottom, uchar orientation)
                : mTexName(textureName)
                , left(left)
                , top(top)
                , right(right)
                , bottom(bottom)
                , orientation(orientation)
            {
            }

            explicit PaintInfo(String textureName)
                : mTexName(textureName)
                , left(0)
                , top(0)
                , right(1)
                , bottom(1)
                , orientation(0)
            {
            }

            bool operator== (const PaintInfo& rhs) const
            {
                return mTexName == rhs.mTexName &&
                    left == rhs.left && right == rhs.right &&
                    top == rhs.top && bottom == rhs.bottom && orientation == rhs.orientation;
            }

            bool operator!= (const PaintInfo& rhs) const
            {
                return !(*this == rhs);
            }
        };

        enum {
            NumLayers = 2,
        };

        struct UndoRedoGridInfo
        {
            // ÿ����Ҫ�������������������Ϣ
            PaintInfo mPaintInfo[NumLayers];
        };

        ModifyTerrainPaintOperator(SceneManipulator* sceneManipulator);
        ~ModifyTerrainPaintOperator();

        void add(int x, int z, const TerrainData::GridInfo &oldGridInfo, const TerrainData::GridInfo &newGridInfo);
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

        // ÿ�ν��л�ˢ�޸�ʱҪ�������Ϣ
        struct Info
        {
            int x;
            int z;
            UndoRedoGridInfo oldGridInfo; // ��ǰ�����е�������Ϣ 
            UndoRedoGridInfo newGridInfo; // �Ե�ǰ������Ҫ���������޸�
        };

        typedef std::vector<Info> InfoList;

        InfoList mInfos;
        int mMinX, mMaxX, mMinZ, mMaxZ;

        void apply(UndoRedoGridInfo Info::* undoRedo);

        void fillUndoRedoInfo(const TerrainData::GridInfo &gridInfo, UndoRedoGridInfo &fillInfo);
    };
}

#endif 

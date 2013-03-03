#ifndef __TerrainLayerOneEraserAction_H__
#define __TerrainLayerOneEraserAction_H__

#include "PaintAction.h"

namespace Fairy {

    class GridSelection;

    /** ������һ������ԭ���Ǵ�action�������ģ�����Ϊ������PaintAction���doFinish������redo��undo����
        ��PaintAction�������ˣ���Ҳ�Ǻ���ģ���Ϊ�����������Ҳ���޸ĵ�������
    */
    class TerrainLayerOneEraserAction : public PaintAction
    {
    public:
        TerrainLayerOneEraserAction(SceneManipulator* sceneManipulator);
        ~TerrainLayerOneEraserAction();

        const String& getName(void) const;

      //  void setParameter(const String& name, const String& value);
     //   String getParameter(const String& name) const;

		//��չ����ʾ��ˢ����
		void resetHintModified(bool bApply = true);
		void resetCurrentGrids();

    protected:
        GridSelection* mCurrentGrids;
        GridSelection* mHintModified;
        GridSelection* mModifiedGrids;

        void _buildHitIndicator(const Point& pt);
        void _doErase(GridSelection* modified);

        void _onActive(bool active);

        void _onMove(const Point& pt);
        void _onBegin(const Point& pt);
        void _onDrag(const Point& pt);
        void _onEnd(const Point& pt, bool canceled);   

    };

}

#endif // __TerrainLayerOneEraserAction_H__

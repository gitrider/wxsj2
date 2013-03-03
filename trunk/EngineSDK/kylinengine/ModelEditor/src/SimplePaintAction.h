#ifndef __SimplePaintAction_H__
#define __SimplePaintAction_H__

#include "PaintAction.h"
#include "Selection.h"

namespace Fairy {

    class GridSelection;

    class SimplePaintAction : public PaintAction
    {
    public:
        SimplePaintAction(SceneManipulator* sceneManipulator);
        ~SimplePaintAction();

        const String& getName(void) const;

		void setParameter(const String& name, const String& value);
		String getParameter(const String& name) const;

    protected:
        GridSelection* mCurrentGrids;
        GridSelection* mHintModified;
        GridSelection* mModifiedGrids;

        void _buildHitIndicator(const Point& pt);
        void _doPaint(GridSelection* modified);

        void _onActive(bool active);

        void _onMove(const Point& pt);
        void _onBegin(const Point& pt);
        void _onDrag(const Point& pt);
        void _onEnd(const Point& pt, bool canceled);

    protected:

        typedef std::vector< PaintPixmap > SimplePaintPixmapArray;

        SimplePaintPixmapArray mPixmapArray;

        /// ��ǰҪ��������Ĵ�СΪ64����׼��С��
        bool mbTexNormalSize;
        /// �Ƿ�ʹ���������
        bool mbUseRandomOrientation;

        /// ��ˢ�Ĵ�С����ͨ��ˢ��������64*64�������С����64��������ô�����Ҫͨ��setParameter��������
        /// ��ˢ�Ĵ�С�������
        int mXBrushSize;
        int mZBrushSize;

        
    };

}

#endif // __SimplePaintAction_H__

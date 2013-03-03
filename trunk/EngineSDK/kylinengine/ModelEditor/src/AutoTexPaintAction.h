#ifndef __AutoTexPaintAction_H__
#define __AutoTexPaintAction_H__

#include "PaintAction.h"
#include "TerrainPaintInfoContainer.h"

namespace Fairy {

    class GridSelection;

    class AutoTexPaintAction : public PaintAction
    {
        
    public:

        AutoTexPaintAction(SceneManipulator* sceneManipulator);
        ~AutoTexPaintAction();

        const String& getName(void) const;

        void setParameter(const String& name, const String& value);
        String getParameter(const String& name) const;

    protected:        

        /// �Ѳ�ͬ���͵�������ڲ�ͬ��AutoTexGenPixmapArray
        typedef std::vector< PaintPixmap > AutoTexGenPixmapArray;

        /// һ����ˢ�����������һ���������ڲ�ͬ��������
        struct Brush
        {
            AutoTexGenPixmapArray edgeTexArray;
			AutoTexGenPixmapArray lEdgeTexArray;
			AutoTexGenPixmapArray rEdgeTexArray;
			AutoTexGenPixmapArray tEdgeTexArray;
			AutoTexGenPixmapArray bEdgeTexArray;

            AutoTexGenPixmapArray innerCornerTexArray;
			AutoTexGenPixmapArray ltInnerCornerTexArray;
			AutoTexGenPixmapArray rtInnerCornerTexArray;
			AutoTexGenPixmapArray lbInnerCornerTexArray;
			AutoTexGenPixmapArray rbInnerCornerTexArray;

            AutoTexGenPixmapArray outerCornerTexArray;
			AutoTexGenPixmapArray ltOuterCornerTexArray;
			AutoTexGenPixmapArray rtOuterCornerTexArray;
			AutoTexGenPixmapArray lbOuterCornerTexArray;
			AutoTexGenPixmapArray rbOuterCornerTexArray;

            AutoTexGenPixmapArray innerTexArray;
            AutoTexGenPixmapArray duijiaoTexArray;
        };

        /// group brush map
        typedef std::map< String, Brush * > BrushMap;

        /// �ڲ�ʵ�������ѡ������
        GridSelection* mCurrentInnerGrids;
        /// ��߱�Ե��ѡ������
        GridSelection* mCurrentOuterGrids;

        GridSelection* mHintModified;
        GridSelection* mModifiedGrids;

        void _buildHitIndicator(const Point& pt);
        void _doPaint(GridSelection* modified);

        void _onActive(bool active);

        void _onMove(const Point& pt);
        void _onBegin(const Point& pt);
        void _onDrag(const Point& pt);
        void _onEnd(const Point& pt, bool canceled);

        /// �ж�ָ���������Ƿ��ڵ�ǰ����
        bool isTexInCurrentGroup( ushort id ) ;

        /// ����ָ����id���������������
        TexType findTexTypeByPixmapId( ushort id ); 

        /// ���浱ǰ�༭�������е�������
        BrushMap mBrushMap;

        /// ��ǰ���õ�������
        Brush *mCurrentBrush;
    
        /// ��ǰ�����������
        String mCurrentBrushName;

        /// ���ڱ��浱ǰ��ѡ�����������
        AutoTexGenPixmapArray *mCurrentInnerArray;

        AutoTexGenPixmapArray *mCurrentInnerCornerArray;
		AutoTexGenPixmapArray *mCurrentLTInnerCornerArray;
		AutoTexGenPixmapArray *mCurrentRTInnerCornerArray;
		AutoTexGenPixmapArray *mCurrentLBInnerCornerArray;
		AutoTexGenPixmapArray *mCurrentRBInnerCornerArray;

        AutoTexGenPixmapArray *mCurrentOuterCornerArray;
		AutoTexGenPixmapArray *mCurrentLTOuterCornerArray;
		AutoTexGenPixmapArray *mCurrentRTOuterCornerArray;
		AutoTexGenPixmapArray *mCurrentLBOuterCornerArray;
		AutoTexGenPixmapArray *mCurrentRBOuterCornerArray;

        AutoTexGenPixmapArray *mCurrentEdgeArray;
		AutoTexGenPixmapArray *mCurrentLEdgeArray;
		AutoTexGenPixmapArray *mCurrentREdgeArray;
		AutoTexGenPixmapArray *mCurrentTEdgeArray;
		AutoTexGenPixmapArray *mCurrentBEdgeArray;

        AutoTexGenPixmapArray *mCurrentDuijiaoArray;

        /// ��ǰ��ѡ��box��������Ͻǣ�������û�г�����Ե��������Ͻǳ����˱�Ե����ô���ֵ�ͻ��Ǹ��ģ�
        int mCurrentBoxSelectionMinX;
        int mCurrentBoxSelectionMinZ;

        /// ����ˢ�ӱ�Ե��selection
        // �������ֲ�������һ��brushshape��ʵ�ֵģ�������Ӱ�쵽��Ĳ�������������ߵȣ������Ծ͸ĳ�
        // һ��������
        // FIX IT �������ֻ����ʱ���ӵģ�ֻ������box�λ�ˢ��Ӧ�޸�Ϊͨ�õģ��������κ���״�Ļ�ˢ
        void buildBoxEdgeSelection(GridSelection& selection, Real xcentre, Real zcentre, int xsize, int zsize);

        /// ��ǰ�������Ƿ��ѳ�ʼ�����
        bool mInitCurrentBrush;
    };

}

#endif // __AutoTexPaintAction_H__

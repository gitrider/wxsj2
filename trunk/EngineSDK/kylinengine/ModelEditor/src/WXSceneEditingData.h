/********************************************************************
filename:   WXSceneEditingData.h

purpose:    ���泡���༭ʱ��Ҫ�����һЩ��Ϣ������reshape����ǰ���ε�
            ��С�������´δ�ͬһ����ʱ������ȷ����ʾreshape��������
            �ߡ�
            �����ȥ���ļ��ĺ�׺����.SED����ʾscene editing data
*********************************************************************/

#ifndef _SCENEEDITINGDATA_H_
#define _SCENEEDITINGDATA_H_

#include "Core/FairyPrerequisites.h"

namespace Fairy {
    
    /// �������reshape�Ĵ�С������˵left��С��3��right��С��4����Ϣ,���ڵ��������ߵ���ʾ
    struct TerrainReshapeSize
    {
        TerrainReshapeSize() :
        left(0), right(0), top(0), bottom(0)
        {
        }

        TerrainReshapeSize(int l, int r, int t, int b) :
        left(l), right(r), top(t), bottom(b)
        {
        }

        int left;
        int right;
        int top;
        int bottom;
    };

    class SceneEditingData
    {
    public:

        SceneEditingData();
        ~SceneEditingData();

        void saveData(const String& fileName);
        void readData(const String& fileName);

        void setTerrainReshapeSize(int left, int right, int top, int bottom)
        {
            mTerrainReshapeSize.left = left;
            mTerrainReshapeSize.right = right;
            mTerrainReshapeSize.top = top;
            mTerrainReshapeSize.bottom = bottom;

            mDataChanged = true;
        }

        const TerrainReshapeSize& getTerrainReshapeSize(void) const
        {
            return mTerrainReshapeSize;
        }

        bool isDataChanged(void)
        {
            return mDataChanged;
        }

    private:

        void saveTerrainReshapeSize(std::ofstream& of);

    private:

        TerrainReshapeSize mTerrainReshapeSize;

        /// �����Ƿ񱻸ı䣬���û���ı䣬���泡��ʱ�Ͳ���Ҫ����.SED�ļ�
        bool mDataChanged;
    };
}

#endif
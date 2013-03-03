#ifndef __FAIRYMODIFYTERRAINDIFFUSEOPERATOR_H__
#define __FAIRYMODIFYTERRAINDIFFUSEOPERATOR_H__

#include "WXOperator.h"
#include "Core/TerrainData.h"

namespace Fairy
{
    class SceneManipulator;

    /**
    ��������ڼ�¼�Ե���������е��޸ģ�������һ�����Ƥ�����Զ�����ƴ�ӣ��򵥻�ˢ�Ȳ�������PaintAction�����Ĳ�������
    ����redo��undo���󲿷����ݲο���ModifyTerrainHeightOperator��
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

        // ÿ�ν��л�ˢ�޸�ʱҪ�������Ϣ
		struct DiffuseInfo
		{
			int x;
			int z;
			Ogre::ColourValue oldDifffuse;		// ��ǰ�����е�Diffuse
			Ogre::ColourValue newDifffuse;		// ��ǰ����Ҫ�޸ĵ�Diffuse
		};
		typedef std::vector<DiffuseInfo>  DiffuseInfoList;
		DiffuseInfoList mInfoList;				//������Ϣ�б�
		int mMinX, mMaxX, mMinZ, mMaxZ;			// ��¼�޸�����������С����

		void apply(Ogre::ColourValue DiffuseInfo::* undoRedo);

    };
}

#endif 

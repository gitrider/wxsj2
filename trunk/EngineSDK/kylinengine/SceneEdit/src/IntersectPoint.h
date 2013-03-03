#ifndef __IntersectPoint_H__
#define __IntersectPoint_H__

#include "HitIndicator.h"

namespace Fairy {

    class IntersectPoint : public HitIndicator
    {
    protected:
        Ogre::SceneNode* mIntersectNode;
        Ogre::Entity* mIntersectEntity;

    public:
        IntersectPoint(SceneManipulator* sceneManipulator);
        ~IntersectPoint();

        const String& getName(void) const;

        void reshape(void);
        void hide(void);

		// ������ײС��ԭ���Ĳ���
		void SetMaterial();

		// ������ײС����²���.
		void SetMaterial(String& strMaterialName);
	
	

    protected:
        void _setHitPoint(Real x, Real y);
    };

}

#endif // __IntersectPoint_H__

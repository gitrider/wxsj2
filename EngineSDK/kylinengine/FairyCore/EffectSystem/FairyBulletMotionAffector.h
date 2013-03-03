#ifndef FairyBulletMotionAffector_h__
#define FairyBulletMotionAffector_h__

#include "OgreStringInterface.h"

namespace Fairy
{
	class BulletSystem;

	class BulletMotionAffector : public Ogre::StringInterface
	{
	public:
		BulletMotionAffector();
		virtual ~BulletMotionAffector();

		virtual void	affectBullet(BulletSystem* bulletSystem) = 0;

	protected:
	private:
	};
}
#endif // FairyBulletMotionAffector_h__
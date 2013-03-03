#include "Scorpio.h"
#include "ScorpioCloth.h"
#include "ScorpioFabric.h"

#include <OgreRoot.h>


namespace Ogre
{
    namespace
    {
        static std::vector<MovableObjectFactory*> movableObjectFactories;
        //-----------------------------------------------------------------------
        static void registerMovableObjectFactories(void)
        {
            // -- Create all new movable object factories --
            MovableObjectFactory* pFactory;

            // Cloth
            pFactory = new ClothFactory;
            movableObjectFactories.push_back(pFactory);
            Root::getSingleton().addMovableObjectFactory(pFactory);

            // Fabric
            pFactory = new FabricFactory;
            movableObjectFactories.push_back(pFactory);
            Root::getSingleton().addMovableObjectFactory(pFactory);
        }
        //-----------------------------------------------------------------------
        static void destroyMovableObjectFactories(void)
        {
            std::vector<MovableObjectFactory*>::const_iterator mi;
            for (mi = movableObjectFactories.begin(); mi != movableObjectFactories.end(); ++mi)
            {
                delete *mi;
            }
        }
    }
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    void  _StartScorpio(void) throw()
    {
        // Physics system
        registerMovableObjectFactories();
    }
    //-----------------------------------------------------------------------
    void  _StopScorpio(void) throw()
    {
        // Physics system
        destroyMovableObjectFactories();
    }

	//-----------------------------------------------------------------------
	extern "C" void _ScorpioExport dllStartPlugin(void) throw()
	{
		// Particle SFX
		_StartScorpio();
	}

	//-----------------------------------------------------------------------
	extern "C" void _ScorpioExport dllStopPlugin(void)
	{
		// Particle SFX
		_StopScorpio();

	}

	CollisionManager* _createCollisionManager(void);
	void  _destroyCollisionManager(CollisionManager* collisionManager);
	bool  _convertEntityToHardwareSkinning(Entity* entity, size_t maxSimultaneousLights);

	extern "C" _ScorpioExport CollisionManager* createCollisionManager(void)
	{
		return _createCollisionManager();
	}

	extern "C" _ScorpioExport void  destroyCollisionManager(CollisionManager* collisionManager)
	{
		_destroyCollisionManager( collisionManager);
	}
	
	extern "C" _ScorpioExport bool  convertEntityToHardwareSkinning(Entity* entity, size_t maxSimultaneousLights)
	{
		return _convertEntityToHardwareSkinning( entity, maxSimultaneousLights);
	}

}


namespace Ogre
{
    // Since VC CRT allow replacable operator new/new[], but not that
    // for operator delete/delete[]. Thus, if some source file which
    // used new[]/delete[] but never includes OgreMemoryMacros.h, and
    // all source files that included OgreMemoryMacros.h and then use
    // new/delete only, the problem occuring then: In this case, only
    // new/delete are instantiated, and then in link time, replacable
    // new[] supplied by CRT will linked, which call to ourself version
    // of new, and the CRT DLL version delete[] will linked too, which
    // call to CRT DLL version of delete!
    //
    // Force instantiate all four functions here to resolve this problem.
    //
    void __force_instantiate_global_overridden(void)
    {
        ::operator new(0);
        ::operator delete(0);
        ::operator new[](0);
        ::operator delete[](0);
    }
}

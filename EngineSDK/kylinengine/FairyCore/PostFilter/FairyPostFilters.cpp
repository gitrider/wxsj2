#include "FairyPostFilters.h"
#include "FairyPostFilterManager.h"

namespace WX {

extern void registerAllPostFilterFactories(PostFilterManager* postFilterManager)
{
    assert(postFilterManager);

    postFilterManager->registerFactory(getHeatVisionPostFilterFactory());
    postFilterManager->registerFactory(getBloomPostFilterFactory());
    postFilterManager->registerFactory(getFloodlightingPostFilterFactory());
    postFilterManager->registerFactory(getFadePostFilterFactory());
    postFilterManager->registerFactory(getMotionBlurPostFilterFactory());
}

}

#ifndef __FairyPostFilters_H__
#define __FairyPostFilters_H__

#include "FairyPostFilterFactory.h"

namespace Fairy {

extern PostFilterFactory* getHeatVisionPostFilterFactory(void);
extern PostFilterFactory* getBloomPostFilterFactory(void);
extern PostFilterFactory* getFloodlightingPostFilterFactory(void);
extern PostFilterFactory* getFadePostFilterFactory(void);
extern PostFilterFactory* getMotionBlurPostFilterFactory(void);

extern void registerAllPostFilterFactories(PostFilterManager* postFilterManager);

}

#endif // 

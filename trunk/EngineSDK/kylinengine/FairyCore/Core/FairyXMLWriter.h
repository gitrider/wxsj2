#ifndef __FAIRYXMLWRITER_H__
#define __FAIRYXMLWRITER_H__

#include "FairyPrerequisites.h"

namespace Fairy
{
    class XMLNode;

    class XMLWriter
    {
    public:
        XMLWriter(void);

        virtual ~XMLWriter();

        virtual void writeXMLFile(const XMLNode* root, const Ogre::String& filename) = 0;
    };

}

#endif  // end of guard 

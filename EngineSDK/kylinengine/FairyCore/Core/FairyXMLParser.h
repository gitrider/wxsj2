#ifndef __FAIRYXMLPARSER_H__
#define __FAIRYXMLPARSER_H__

#include "FairyPrerequisites.h"

namespace Fairy
{
    class XMLHandler;

    class XMLParser
    {
    public:
        XMLParser(void);

        virtual ~XMLParser();

        virtual void parseXMLFile(XMLHandler& handler, const Ogre::String& filename, const Ogre::String& resourceGroup, const Ogre::String& schemaName, const Ogre::String& schemaResourceGroup) = 0;
    };

} 

#endif  // end of guard 

#ifndef __FairyXercesParser_H__
#define __FairyXercesParser_H__

#include "FairyXMLParser.h"
#include "FairyXercesHelper.h"

// Start of Fairy namespace section
namespace Fairy
{
    /*!
    \brief
        Implementation of XMLParser using Xerces-C++
     */
    class XercesParser : public XMLParser, public XercesHelper
    {
    public:
        XercesParser(void);
        ~XercesParser();

        // Implementation of public abstract interface
        void parseXMLFile(XMLHandler& handler, const Ogre::String& xmlName, const Ogre::String& xmlResourceGroup, const Ogre::String& schemaName, const Ogre::String& schemaResourceGroup);
    };

} // End of Fairy namespace section

#endif  // end of guard __FairyXercesParser_H__

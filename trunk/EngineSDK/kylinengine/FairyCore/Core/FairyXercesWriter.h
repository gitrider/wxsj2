#ifndef __FAIRYXERCESWRITER_H__
#define __FAIRYXERCESWRITER_H__

#include "FairyXMLWriter.h"
#include "FairyXercesHelper.h"


namespace Fairy
{
    /*!
    \brief
        Implementation of XMLWriter using Xerces-C++
     */
    class XercesWriter : public XMLWriter, public XercesHelper
    {
    public:
        XercesWriter(void);
        ~XercesWriter();

        // Implementation of public abstract interface
        virtual void writeXMLFile(const XMLNode* root, const Ogre::String& filename);
    };

}

#endif  

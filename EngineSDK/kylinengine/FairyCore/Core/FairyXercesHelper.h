#ifndef __FAIRYXERCESHELPER_H__
#define __FAIRYXERCESHELPER_H__

#include "FairyPrerequisites.h"

// Xerces-C includes
#include <xercesc/util/XercesDefs.hpp>


namespace Fairy
{
    class XercesHelper
    {
    public:
        static std::basic_string<char> transcode(const XMLCh *toTranscode);
        static std::basic_string<XMLCh> transcode(const char *toTranscode);
        static std::basic_string<char> transcode(const std::basic_string<XMLCh>& toTranscode);
        static std::basic_string<XMLCh> transcode(const std::basic_string<char>& toTranscode);

        XercesHelper(void);
        ~XercesHelper();

    protected:
        bool mInitialised;
    };

} 

#endif  

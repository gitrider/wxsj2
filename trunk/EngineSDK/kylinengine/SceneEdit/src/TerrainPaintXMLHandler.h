#ifndef __TERRAINPAINTXMLHANDLER_H__
#define __TERRAINPAINTXMLHANDLER_H__

#include "Core/FairyXMLHandler.h"
#include "TerrainPaintInfoContainer.h"

namespace Fairy {

    /** ���������XML������
    */
    class TerrainPaintXMLHandler : public XMLHandler
    {
    public:
     
        explicit TerrainPaintXMLHandler(TerrainPaintInfoContainer *container);

        ~TerrainPaintXMLHandler(void);

        /*************************************************************************
        SAX2 Handler overrides
        *************************************************************************/ 

        /** Overridden from XMLHandler */
        virtual void characters(const String& chars);

        /** Overridden from XMLHandler */
        virtual void startElement(const String& element, const XMLAttributes& attributes);

        /** Overridden from XMLHandler */
        virtual void endElement(const String& element);

        void clearTextureInfos(void)
        {
            mPaintInfoContainer->clearTextureInfos();
        }

        const String& getErrorInfoString(void) const
        {
            return mErrorInfoString;
        }

    protected:

        void _pushErrorInfoString(const String& errorInfo);

        const String _getCurrentTextureInfo(void) const;

    protected:

        TerrainPaintInfoContainer *mPaintInfoContainer;

        TextureInfo mCurrentTextureInfo; /// ���浱ǰ���ڴ�����������Ϣ

        String mErrorInfoString;

        bool mCurrentElementWrong;
    };
}

#endif
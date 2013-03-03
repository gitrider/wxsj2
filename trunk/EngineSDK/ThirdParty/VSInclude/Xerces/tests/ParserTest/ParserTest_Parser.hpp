/*
 * Copyright 1999-2000,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Log$
 * Revision 1.9  2004/09/08 13:57:05  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.8  2002/11/05 21:47:36  tng
 * Explicit code using namespace in application.
 *
 * Revision 1.7  2002/02/01 22:45:54  peiyongz
 * sane_include
 *
 * Revision 1.6  2000/03/02 19:55:46  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.5  2000/02/06 07:48:37  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.4  2000/01/24 20:38:56  roddey
 * Fixed some small bugs introduced in the recent move away from the util/* streams.
 *
 * Revision 1.3  2000/01/21 23:58:06  roddey
 * Initial move away from util streams was bad. Wide char APIs didn't allow enough
 * control to do canonical output, so changed to use std short char APIs.
 *
 * Revision 1.1  1999/11/09 01:02:23  twl
 * Initial revision
 *
 * Revision 1.3  1999/11/08 20:42:25  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include    <xercesc/framework/XMLDocumentHandler.hpp>
#include    <xercesc/framework/XMLErrorReporter.hpp>
#include    <xercesc/validators/DTD/DocTypeHandler.hpp>

XERCES_CPP_NAMESPACE_USE


class TestParser :

    public XMLDocumentHandler, public XMLErrorReporter, public DocTypeHandler
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    TestParser();
    ~TestParser();


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setDoNamespaces(const bool state);
    void setScanner(XMLScanner* const state);
    void setOutputType(const OutputTypes outType);
    void setShowErrLoc(const bool state);
    void setShowIntDTD(const bool state);
    void setShowWarnings(const bool state);


    // -----------------------------------------------------------------------
    //  The XMLDocumentHandler interface
    // -----------------------------------------------------------------------
    virtual void docCharacters
    (
        const   XMLCh* const    chars
        , const unsigned int    length
        , const bool            cdataSection
    );

    virtual void docComment
    (
        const   XMLCh* const    comment
    );

    virtual void docPI
    (
        const   XMLCh* const    target
        , const XMLCh* const    data
    );

    virtual void endDocument();

    virtual void endElement
    (
        const   XMLElementDecl& elemDecl
        , const unsigned int    uriId
        , const bool            isRoot
    );

    virtual void endEntityReference
    (
        const   XMLEntityDecl&  entDecl
    );

    virtual void ignorableWhitespace
    (
        const   XMLCh* const    chars
        , const unsigned int    length
        , const bool            cdataSection
    );

    virtual void resetDocument();

    virtual void startDocument();

    virtual void startElement
    (
        const   XMLElementDecl&         elemDecl
        , const unsigned int            elemURIId
        , const XMLCh* const            elemPrefix
        , const RefVectorOf<XMLAttr>&   attrList
        , const unsigned int            attrCount
        , const bool                    isEmpty
        , const bool                    isRoot
    );

    virtual void startEntityReference
    (
        const   XMLEntityDecl&  entDecl
    );

    virtual void XMLDecl
    (
        const   XMLCh* const    versionStr
        , const XMLCh* const    encodingStr
        , const XMLCh* const    standaloneStr
        , const XMLCh* const    autoEncStr
    );


    // -----------------------------------------------------------------------
    //  Implementation of the XMLErrorReporter interface
    // -----------------------------------------------------------------------
    virtual void error
    (
        const   unsigned int                errCode
        , const XMLCh* const                msgDomain
        , const XMLErrorReporter::ErrTypes  type
        , const XMLCh* const                text
        , const XMLCh* const                systemId
        , const XMLCh* const                publicId
        , const unsigned int                lineNum
        , const unsigned int                colNum
    );

    virtual void resetErrors();


    // -----------------------------------------------------------------------
    //  The document type handler virtual handler interface
    // -----------------------------------------------------------------------
    virtual void attDef
    (
        const   DTDElementDecl&     elemDecl
        , const DTDAttDef&          attDef
        , const bool                ignoring
    );

    virtual void doctypeComment
    (
        const   XMLCh* const    comment
    );

    virtual void doctypeDecl
    (
        const   DTDElementDecl& elemDecl
        , const XMLCh* const    publicId
        , const XMLCh* const    systemId
        , const bool            hasIntSubset
    );

    virtual void doctypePI
    (
        const   XMLCh* const    target
        , const XMLCh* const    data
    );

    virtual void doctypeWhitespace
    (
        const   XMLCh* const    chars
        , const unsigned int    length
    );

    virtual void elementDecl
    (
        const   DTDElementDecl& decl
        , const bool            isIgnored
    );

    virtual void endAttList
    (
        const   DTDElementDecl& elemDecl
    );

    virtual void endIntSubset();

    virtual void endExtSubset();

    virtual void entityDecl
    (
        const   DTDEntityDecl&  entityDecl
        , const bool            isPEDecl
        , const bool            isIgnored
    );

    virtual void resetDocType();

    virtual void notationDecl
    (
        const   XMLNotationDecl&    notDecl
        , const bool                isIgnored
    );

    virtual void startAttList
    (
        const   DTDElementDecl& elemDecl
    );

    virtual void startIntSubset();

    virtual void startExtSubset();

    virtual void TextDecl
    (
        const   XMLCh* const    versionStr
        , const XMLCh* const    encodingStr
    );


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    TestParser(const TestParser&);
    void operator=(const TestParser&);


    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    void showChar(const char toShow, const bool doCan);
    void showChars
    (
        const   XMLCh* const    chars
        , const unsigned int    length
    );
    void showIds(const XMLCh* const publicId, const XMLCh* const systemId);
    void showString(const XMLCh* const toShow);


    // -----------------------------------------------------------------------
    //  Data members
    //
    //  fDoNamespaces
    //      Indicates whether the user wanted us to be namespace aware or
    //      not.
    //
    //  fInsideRoot
    //      This is set once the first start element event is seen, and cleared
    //      when the root element ends. Its used to support the canonical
    //      format used by the Bosak tests. Basically it allows us to supress
    //      the whitespace outside the internal subset but before the root
    //      element.
    //
    //  fInExtSubSet
    //      Indicates when we are in the external subset, so that we don't
    //      display that part.
    //
    //  fIntDTDOutput
    //      Indicates whether the data from the internal DTD subset is
    //      output or not (only used if fXMLOutput is true.)
    //
    //  fNestingLevel
    //      This is used to handle tabbing over nested elements. Each start
    //      element bumps it up, and each end element bumps it down.
    //
    //  fOutputType
    //      This flag controls the primary style of output used. It can
    //      be set to do James Clark type canonical output, Sun style
    //      canonical output, debug output, regular XML output, or none.
    //
    //  fScanner
    //      The scanner we created to do the scanning.
    //
    //  fShowErrLoc
    //      This flag turns on the special display mode that is used for
    //      negative test testing. It puts out a special, condensed display
    //      of error info that can be compared in subsequent runs to check
    //      for changes. If its turned on, it forces the output type to
    //      'none'.
    //
    //  fShowWarnings
    //      Indicates whether warning messages should be displayed or not.
    //
    //  fSurrogate
    //      Indicates that we got a surrogate char, so we have to wait for
    //      the next char before we can output it. Its zero when not.
    // -----------------------------------------------------------------------
    bool            fDoNamespaces;
    bool            fInExtSubset;
    bool            fInsideRoot;
    bool            fIntDTDOutput;
    unsigned int    fNestingLevel;
    OutputTypes     fOutputType;
    XMLScanner*     fScanner;
    bool            fShowErrLoc;
    bool            fShowWarnings;
    XMLCh           fSurrogate;
};


// ---------------------------------------------------------------------------
//  TestParser: Setter Methods
// ---------------------------------------------------------------------------
inline void TestParser::setDoNamespaces(const bool state)
{
    fDoNamespaces = state;
}

inline void TestParser::setScanner(XMLScanner* const ourScanner)
{
    fScanner = ourScanner;
}

inline void TestParser::setOutputType(const OutputTypes outType)
{
    fOutputType = outType;
}

inline void TestParser::setShowIntDTD(const bool state)
{
    fIntDTDOutput = state;
}

inline void TestParser::setShowWarnings(const bool state)
{
    fShowWarnings = state;
}

inline void TestParser::setShowErrLoc(const bool state)
{
    fShowErrLoc = state;
}

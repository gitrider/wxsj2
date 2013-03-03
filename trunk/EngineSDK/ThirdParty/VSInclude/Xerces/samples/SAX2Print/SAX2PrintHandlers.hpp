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
 * Revision 1.7  2004/09/08 13:55:33  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.6  2004/02/15 19:43:15  amassari
 * Removed cause for warnings in VC 7.1
 *
 * Revision 1.5  2002/11/05 21:46:20  tng
 * Explicit code using namespace in application.
 *
 * Revision 1.4  2002/02/01 22:40:44  peiyongz
 * sane_include
 *
 * Revision 1.3  2000/10/10 23:55:55  andyh
 * XMLFormatter patch, contributed by Bill Schindler.  Fix problems with
 * output to multi-byte encodings.
 *
 * Revision 1.2  2000/08/09 22:20:39  jpolast
 * updates for changes to sax2 core functionality.
 *
 * Revision 1.1  2000/08/02 19:16:14  jpolast
 * initial checkin of SAX2Print
 *
 *
 */

#include    <xercesc/sax2/DefaultHandler.hpp>
#include    <xercesc/framework/XMLFormatter.hpp>

XERCES_CPP_NAMESPACE_USE

class SAX2PrintHandlers : public DefaultHandler, private XMLFormatTarget
{
public:
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
    SAX2PrintHandlers
    (
        const   char* const                 encodingName
        , const XMLFormatter::UnRepFlags    unRepFlags
		, const bool						expandNamespaces
    );
    ~SAX2PrintHandlers();


    // -----------------------------------------------------------------------
    //  Implementations of the format target interface
    // -----------------------------------------------------------------------
    void writeChars
    (
        const   XMLByte* const  toWrite
    );

    void writeChars
    (
        const   XMLByte* const  toWrite
        , const unsigned int    count
        , XMLFormatter* const   formatter
    );


    // -----------------------------------------------------------------------
    //  Implementations of the SAX DocumentHandler interface
    // -----------------------------------------------------------------------
    void endDocument();

    void endElement( const XMLCh* const uri,
					 const XMLCh* const localname,
					 const XMLCh* const qname);

    void characters(const XMLCh* const chars, const unsigned int length);

    void ignorableWhitespace
    (
        const   XMLCh* const    chars
        , const unsigned int    length
    );

    void processingInstruction
    (
        const   XMLCh* const    target
        , const XMLCh* const    data
    );

    void startDocument();

    void startElement(	const   XMLCh* const    uri,
						const   XMLCh* const    localname,
						const   XMLCh* const    qname,
					    const   Attributes&		attributes);



    // -----------------------------------------------------------------------
    //  Implementations of the SAX ErrorHandler interface
    // -----------------------------------------------------------------------
    void warning(const SAXParseException& exc);
    void error(const SAXParseException& exc);
    void fatalError(const SAXParseException& exc);



    // -----------------------------------------------------------------------
    //  Implementation of the SAX DTDHandler interface
    // -----------------------------------------------------------------------
    void notationDecl
    (
        const   XMLCh* const    name
        , const XMLCh* const    publicId
        , const XMLCh* const    systemId
    );

    void unparsedEntityDecl
    (
        const   XMLCh* const    name
        , const XMLCh* const    publicId
        , const XMLCh* const    systemId
        , const XMLCh* const    notationName
    );

private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fFormatter
    //      This is the formatter object that is used to output the data
    //      to the target. It is set up to format to the standard output
    //      stream.
    // -----------------------------------------------------------------------
    XMLFormatter    fFormatter;
	bool			fExpandNS ;
};

/*
 * Copyright 1999-2001,2004 The Apache Software Foundation.
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
 * Revision 1.11  2005/01/12 20:43:21  cargilld
 * Remove warning messages.
 *
 * Revision 1.10  2004/09/08 13:55:33  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.9  2003/05/30 09:36:36  gareth
 * Use new macros for iostream.h and std:: issues.
 *
 * Revision 1.8  2003/03/17 21:03:52  peiyongz
 * Bug#17983
 *
 * Revision 1.7  2002/05/28 20:24:15  tng
 * XMLUni::fgEmptyString which is defined as "EMPTY" is incorrectly used as an empty string, should use XMLUni::fgZeroLenString instead
 *
 * Revision 1.6  2002/02/01 22:40:44  peiyongz
 * sane_include
 *
 * Revision 1.5  2001/05/11 13:24:57  tng
 * Copyright update.
 *
 * Revision 1.4  2001/05/03 16:00:03  tng
 * Schema: samples update with schema
 *
 * Revision 1.3  2000/10/10 23:55:53  andyh
 * XMLFormatter patch, contributed by Bill Schindler.  Fix problems with
 * output to multi-byte encodings.
 *
 * Revision 1.2  2000/08/09 22:20:38  jpolast
 * updates for changes to sax2 core functionality.
 *
 * Revision 1.1  2000/08/02 19:16:14  jpolast
 * initial checkin of SAX2Print
 *
 *
 */



// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include "SAX2Print.hpp"


// ---------------------------------------------------------------------------
//  Local const data
//
//  Note: This is the 'safe' way to do these strings. If you compiler supports
//        L"" style strings, and portability is not a concern, you can use
//        those types constants directly.
// ---------------------------------------------------------------------------
static const XMLCh  gEndElement[] = { chOpenAngle, chForwardSlash, chNull };
static const XMLCh  gEndPI[] = { chQuestion, chCloseAngle, chNull };
static const XMLCh  gStartPI[] = { chOpenAngle, chQuestion, chNull };
static const XMLCh  gXMLDecl1[] =
{
        chOpenAngle, chQuestion, chLatin_x, chLatin_m, chLatin_l
    ,   chSpace, chLatin_v, chLatin_e, chLatin_r, chLatin_s, chLatin_i
    ,   chLatin_o, chLatin_n, chEqual, chDoubleQuote, chDigit_1, chPeriod
    ,   chDigit_0, chDoubleQuote, chSpace, chLatin_e, chLatin_n, chLatin_c
    ,   chLatin_o, chLatin_d, chLatin_i, chLatin_n, chLatin_g, chEqual
    ,   chDoubleQuote, chNull
};

static const XMLCh  gXMLDecl2[] =
{
        chDoubleQuote, chQuestion, chCloseAngle
    ,   chLF, chNull
};




// ---------------------------------------------------------------------------
//  SAX2PrintHandlers: Constructors and Destructor
// ---------------------------------------------------------------------------
SAX2PrintHandlers::SAX2PrintHandlers( const   char* const              encodingName
                                    , const XMLFormatter::UnRepFlags unRepFlags
									, const bool					 expandNamespaces) :

    fFormatter
    (
        encodingName
        , 0
        , this
        , XMLFormatter::NoEscapes
        , unRepFlags
    ),
	fExpandNS ( expandNamespaces )
{
    //
    //  Go ahead and output an XML Decl with our known encoding. This
    //  is not the best answer, but its the best we can do until we
    //  have SAX2 support.
    //
    fFormatter << gXMLDecl1 << fFormatter.getEncodingName() << gXMLDecl2;
}

SAX2PrintHandlers::~SAX2PrintHandlers()
{
}


// ---------------------------------------------------------------------------
//  SAX2PrintHandlers: Overrides of the output formatter target interface
// ---------------------------------------------------------------------------
void SAX2PrintHandlers::writeChars(const XMLByte* const /* toWrite */)
{
}

void SAX2PrintHandlers::writeChars(const XMLByte* const toWrite,
                                   const unsigned int count,
                                   XMLFormatter* const /* formatter */)
{
    // For this one, just dump them to the standard output
    // Surprisingly, Solaris was the only platform on which
    // required the char* cast to print out the string correctly.
    // Without the cast, it was printing the pointer value in hex.
    // Quite annoying, considering every other platform printed
    // the string with the explicit cast to char* below.
    XERCES_STD_QUALIFIER cout.write((char *) toWrite, (int) count);
	XERCES_STD_QUALIFIER cout.flush();
}


// ---------------------------------------------------------------------------
//  SAX2PrintHandlers: Overrides of the SAX ErrorHandler interface
// ---------------------------------------------------------------------------
void SAX2PrintHandlers::error(const SAXParseException& e)
{
    XERCES_STD_QUALIFIER cerr << "\nError at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
}

void SAX2PrintHandlers::fatalError(const SAXParseException& e)
{
    XERCES_STD_QUALIFIER cerr << "\nFatal Error at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
}

void SAX2PrintHandlers::warning(const SAXParseException& e)
{
    XERCES_STD_QUALIFIER cerr << "\nWarning at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
}


// ---------------------------------------------------------------------------
//  SAX2PrintHandlers: Overrides of the SAX DTDHandler interface
// ---------------------------------------------------------------------------
void SAX2PrintHandlers::unparsedEntityDecl(const    XMLCh* const /* name */
                                          , const   XMLCh* const /* publicId */
                                          , const   XMLCh* const /* systemId */
                                          , const   XMLCh* const /* notationName */)
{
    // Not used at this time
}


void SAX2PrintHandlers::notationDecl(const  XMLCh* const /* name */
                                    , const XMLCh* const /* publicId */
                                    , const XMLCh* const /* systemId */)
{
    // Not used at this time
}


// ---------------------------------------------------------------------------
//  SAX2PrintHandlers: Overrides of the SAX DocumentHandler interface
// ---------------------------------------------------------------------------
void SAX2PrintHandlers::characters(const     XMLCh* const    chars
                                  , const   unsigned int    length)
{
    fFormatter.formatBuf(chars, length, XMLFormatter::CharEscapes);
}


void SAX2PrintHandlers::endDocument()
{
}


void SAX2PrintHandlers::endElement(const XMLCh* const uri,
								 const XMLCh* const localname,
								 const XMLCh* const qname)
{
    // No escapes are legal here
    fFormatter << XMLFormatter::NoEscapes << gEndElement ;
	if ( fExpandNS )
	{
		if (XMLString::compareIString(uri,XMLUni::fgZeroLenString) != 0)
				fFormatter  << uri << chColon;
		fFormatter << localname << chCloseAngle;
	}
	else
		fFormatter << qname << chCloseAngle;
}


void SAX2PrintHandlers::ignorableWhitespace( const   XMLCh* const chars
                                            ,const  unsigned int length)
{
    fFormatter.formatBuf(chars, length, XMLFormatter::NoEscapes);
}


void SAX2PrintHandlers::processingInstruction(const  XMLCh* const target
                                            , const XMLCh* const data)
{
    fFormatter << XMLFormatter::NoEscapes << gStartPI  << target;
    if (data)
        fFormatter << chSpace << data;
    fFormatter << XMLFormatter::NoEscapes << gEndPI;
}


void SAX2PrintHandlers::startDocument()
{
}


void SAX2PrintHandlers::startElement(const   XMLCh* const    uri,
									const   XMLCh* const    localname,
									const   XMLCh* const    qname,
                                    const   Attributes&		attributes)
{
    // The name has to be representable without any escapes
    fFormatter  << XMLFormatter::NoEscapes << chOpenAngle ;
	if ( fExpandNS )
	{
		if (XMLString::compareIString(uri,XMLUni::fgZeroLenString) != 0)
				fFormatter  << uri << chColon;
		fFormatter << localname ;
	}
	else
		fFormatter << qname ;

    unsigned int len = attributes.getLength();
    for (unsigned int index = 0; index < len; index++)
    {
        //
        //  Again the name has to be completely representable. But the
        //  attribute can have refs and requires the attribute style
        //  escaping.
        //
        fFormatter  << XMLFormatter::NoEscapes << chSpace ;
		if ( fExpandNS )
		{
			if (XMLString::compareIString(attributes.getURI(index),XMLUni::fgZeroLenString) != 0)
				fFormatter  << attributes.getURI(index) << chColon;
			fFormatter  << attributes.getLocalName(index) ;
		}
		else
			fFormatter  << attributes.getQName(index) ;

		fFormatter  << chEqual << chDoubleQuote
                    << XMLFormatter::AttrEscapes
		            << attributes.getValue(index)
                    << XMLFormatter::NoEscapes
                    << chDoubleQuote;
    }
    fFormatter << chCloseAngle;
}

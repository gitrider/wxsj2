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
 * Revision 1.1  2004/12/24 19:44:21  amassari
 * Add support for SAX2 filters (jira# 1133)
 *
 *
 */

#include    <xercesc/parsers/SAX2XMLFilterImpl.hpp>

XERCES_CPP_NAMESPACE_USE

class SAX2SortAttributesFilter : public SAX2XMLFilterImpl
{
public:
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
    SAX2SortAttributesFilter(SAX2XMLReader* parent);
    ~SAX2SortAttributesFilter();


    // -----------------------------------------------------------------------
    //  Implementations of the SAX2XMLFilter interface
    // -----------------------------------------------------------------------
    void startElement(	const   XMLCh* const    uri,
						const   XMLCh* const    localname,
						const   XMLCh* const    qname,
					    const   Attributes&		attributes);
};


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
 * $Id: XercesLibMachORelPrefix.h 176026 2004-09-08 13:57:07Z peiyongz $
 *
 */
 
#define TARGET_API_MAC_OSX 1

#include <MSL MacHeadersMach-O.h>
#include "XercesLibCommonPrefix.h"

#define PROJ_XMLUTIL 1
#define PROJ_XMLPARSER 1
#define PROJ_SAX4C 1
#define PROJ_SAX2 1
#define PROJ_DOM 1
#define PROJ_PARSERS 1
#define PROJ_VALIDATORS 1

//  A couple of defines to allow /usr/include/unistd.h to compile
#define __dead2
#define __pure2

/*
 * Copyright 2001-2002,2004 The Apache Software Foundation.
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
 * $Id: xerces_prefix.h 176026 2004-09-08 13:57:07Z peiyongz $
 */

//	Objective C only
#if __OBJC__
#endif

//	C++ only
#if defined(__cplusplus)
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <cstdio>
#include <memory>
#endif

//	Standard C headers
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

//	Carbon Headers
#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>

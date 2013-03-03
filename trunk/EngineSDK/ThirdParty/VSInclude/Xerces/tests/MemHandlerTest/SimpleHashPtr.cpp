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

#include "SimpleHashPtr.hpp"

// this is just a copy of HashPtr which is careful to rely on global new.


SimpleHashPtr::SimpleHashPtr()
{
}

SimpleHashPtr::~SimpleHashPtr()
{
}

unsigned int SimpleHashPtr::getHashVal(const void *const key, unsigned int mod)
{
 return ((long)key % (unsigned long)mod);
}

bool SimpleHashPtr::equals(const void *const key1, const void *const key2)
{
	return (key1 == key2);
}


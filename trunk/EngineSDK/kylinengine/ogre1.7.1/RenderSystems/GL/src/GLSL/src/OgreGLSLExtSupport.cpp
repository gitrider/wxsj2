/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org

Copyright (c) 2000-2009 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#include "OgreLogManager.h"
#include "OgreGLSLExtSupport.h"
#include "OgreGLSupport.h"

namespace Ogre
{

    //-----------------------------------------------------------------------------
    void checkForGLSLError(const String& ogreMethod, const String& errorTextPrefix, const GLhandleARB obj, const bool forceInfoLog, const bool forceException)
    {
		GLenum glErr;
		bool errorsFound = false;
		String msg = errorTextPrefix;

		// get all the GL errors
		glErr = glGetError();
		while (glErr != GL_NO_ERROR)
        {
			const char* glerrStr = (const char*)gluErrorString(glErr);
			if (glerrStr)
			{
				msg += String(glerrStr);
			}
			glErr = glGetError();
			errorsFound = true;
        }


		// if errors were found then put them in the Log and raise and exception
		if (errorsFound || forceInfoLog)
		{
			// if shader or program object then get the log message and send to the log manager
			msg += logObjectInfo( msg, obj );

            if (forceException) 
			{
				OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, msg, ogreMethod);
			}
		}
    }

    //-----------------------------------------------------------------------------
	String logObjectInfo(const String& msg, const GLhandleARB obj)
	{
		String logMessage = msg;

		if (obj > 0)
		{
			GLint infologLength = 0;

			glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infologLength);

			if (infologLength > 0)
			{
				GLint charsWritten  = 0;

				GLcharARB * infoLog = new GLcharARB[infologLength];

				glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
				logMessage += String(infoLog);
				LogManager::getSingleton().logMessage(logMessage);

				delete [] infoLog;
			}
		}

		return logMessage;

	}


} // namespace Ogre

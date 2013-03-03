/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

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
#include "OgreCgProgram.h"
#include "OgreGpuProgramManager.h"
#include "OgreHighLevelGpuProgramManager.h"
#include "OgreStringConverter.h"
#include "OgreLogManager.h"

namespace Ogre {
    //-----------------------------------------------------------------------
    CgProgram::CmdEntryPoint CgProgram::msCmdEntryPoint;
    CgProgram::CmdProfiles CgProgram::msCmdProfiles;
    CgProgram::CmdArgs CgProgram::msCmdArgs;
    //-----------------------------------------------------------------------
    void CgProgram::selectProfile(void)
    {
        mSelectedProfile.clear();
        mSelectedCgProfile = CG_PROFILE_UNKNOWN;

        StringVector::iterator i, iend;
        iend = mProfiles.end();
        GpuProgramManager& gpuMgr = GpuProgramManager::getSingleton();
        for (i = mProfiles.begin(); i != iend; ++i)
        {
            if (gpuMgr.isSyntaxSupported(*i))
            {
                mSelectedProfile = *i;
                mSelectedCgProfile = cgGetProfile(mSelectedProfile.c_str());
                // Check for errors
                checkForCgError("CgProgram::selectProfile", 
                    "Unable to find CG profile enum for program " + mName + ": ", mCgContext);
                break;
            }
        }
    }
    //-----------------------------------------------------------------------
    void CgProgram::buildArgs(void)
    {
        StringVector args;
        if (!mCompileArgs.empty())
            args = StringUtil::split(mCompileArgs);

        StringVector::const_iterator i;
        if (mSelectedCgProfile == CG_PROFILE_VS_1_1)
        {
            // Need the 'dcls' argument whenever we use this profile
            // otherwise compilation of the assembler will fail
            bool dclsFound = false;
            for (i = args.begin(); i != args.end(); ++i)
            {
                if (*i == "dcls")
                {
                    dclsFound = true;
                    break;
                }
            }
            if (!dclsFound)
            {
                args.push_back("-profileopts");
				args.push_back("dcls");
            }
        }
        // Now split args into that god-awful char** that Cg insists on
        freeCgArgs();
        mCgArguments = OGRE_ALLOC_T(char*, args.size() + 1, MEMCATEGORY_RESOURCE);
        int index = 0;
        for (i = args.begin(); i != args.end(); ++i, ++index)
        {
            mCgArguments[index] = OGRE_ALLOC_T(char, i->length() + 1, MEMCATEGORY_RESOURCE);
            strcpy(mCgArguments[index], i->c_str());
        }
        // Null terminate list
        mCgArguments[index] = 0;


    }
    //-----------------------------------------------------------------------
    void CgProgram::freeCgArgs(void)
    {
        if (mCgArguments)
        {
            size_t index = 0;
            char* current = mCgArguments[index];
            while (current)
            {
                OGRE_FREE(current, MEMCATEGORY_RESOURCE);
				mCgArguments[index] = 0;
                current = mCgArguments[++index];
            }
            OGRE_FREE(mCgArguments, MEMCATEGORY_RESOURCE);
            mCgArguments = 0;
        }
    }
    //-----------------------------------------------------------------------
    void CgProgram::loadFromSource(void)
    {
        // Create Cg Program
        selectProfile();
		if (mSelectedCgProfile == CG_PROFILE_UNKNOWN)
		{
			LogManager::getSingleton().logMessage(
				"Attempted to load Cg program '" + mName + "', but no suported "
				"profile was found. ");
			return;
		}
        buildArgs();
		// deal with includes
		String sourceToUse = resolveCgIncludes(mSource, this, mFilename);
        mCgProgram = cgCreateProgram(mCgContext, CG_SOURCE, sourceToUse.c_str(), 
            mSelectedCgProfile, mEntryPoint.c_str(), const_cast<const char**>(mCgArguments));

        // Test
        //LogManager::getSingleton().logMessage(cgGetProgramString(mCgProgram, CG_COMPILED_PROGRAM));

        // Check for errors
        checkForCgError("CgProgram::loadFromSource", 
            "Unable to compile Cg program " + mName + ": ", mCgContext);

    }
    //-----------------------------------------------------------------------
    void CgProgram::createLowLevelImpl(void)
    {
		// ignore any previous error
		if (mSelectedCgProfile != CG_PROFILE_UNKNOWN && !mCompileError)
		{

			if ( false
// the hlsl 4 profiles are only supported in OGRE from CG 2.2
#if(CG_VERSION_NUM >= 2200)
				|| mSelectedCgProfile == CG_PROFILE_VS_4_0
				 || mSelectedCgProfile == CG_PROFILE_PS_4_0
#endif
				 )
			{
				// Create a high-level program, give it the same name as us
				HighLevelGpuProgramPtr vp = 
					HighLevelGpuProgramManager::getSingleton().createProgram(
					mName, mGroup, "hlsl", mType);
				String hlslSourceFromCg = cgGetProgramString(mCgProgram, CG_COMPILED_PROGRAM);
				
				vp->setSource(hlslSourceFromCg);
				vp->setParameter("target", mSelectedProfile);
				vp->setParameter("entry_point", "main");

				vp->load();

				mAssemblerProgram = vp;
			}
			else
			{

				String shaderAssemblerCode = cgGetProgramString(mCgProgram, CG_COMPILED_PROGRAM);

                if (mType == GPT_FRAGMENT_PROGRAM) {
                    //HACK : http://developer.nvidia.com/forums/index.php?showtopic=1063&pid=2378&mode=threaded&start=#entry2378
                    //Still happens in CG 2.2. Remove hack when fixed.
                    shaderAssemblerCode = StringUtil::replaceAll(shaderAssemblerCode, "oDepth.z", "oDepth");
                }
				// Create a low-level program, give it the same name as us
				mAssemblerProgram = 
					GpuProgramManager::getSingleton().createProgramFromString(
					mName, 
					mGroup,
					shaderAssemblerCode,
					mType, 
					mSelectedProfile);
			}
			// Shader params need to be forwarded to low level implementation
			mAssemblerProgram->setAdjacencyInfoRequired(isAdjacencyInfoRequired());
		}
    }
    //-----------------------------------------------------------------------
    void CgProgram::unloadHighLevelImpl(void)
    {
        // Unload Cg Program
        // Lowlevel program will get unloaded elsewhere
        if (mCgProgram)
        {
            cgDestroyProgram(mCgProgram);
            checkForCgError("CgProgram::unloadImpl", 
                "Error while unloading Cg program " + mName + ": ", 
                mCgContext);
            mCgProgram = 0;
        }
    }
    //-----------------------------------------------------------------------
    void CgProgram::buildConstantDefinitions() const
    {
        // Derive parameter names from Cg
		createParameterMappingStructures(true);

		if (!mCgProgram)
			return;

		recurseParams(cgGetFirstParameter(mCgProgram, CG_PROGRAM));
        recurseParams(cgGetFirstParameter(mCgProgram, CG_GLOBAL));
	}
	//---------------------------------------------------------------------
	void CgProgram::recurseParams(CGparameter parameter, size_t contextArraySize) const
	{
		while (parameter != 0)
        {
            // Look for uniform (non-sampler) parameters only
            // Don't bother enumerating unused parameters, especially since they will
            // be optimised out and therefore not in the indexed versions
            CGtype paramType = cgGetParameterType(parameter);

            if (cgGetParameterVariability(parameter) == CG_UNIFORM &&
                paramType != CG_SAMPLER1D &&
                paramType != CG_SAMPLER2D &&
                paramType != CG_SAMPLER3D &&
                paramType != CG_SAMPLERCUBE &&
                paramType != CG_SAMPLERRECT &&
                cgGetParameterDirection(parameter) != CG_OUT && 
                cgIsParameterReferenced(parameter))
            {
				int arraySize;

				switch(paramType)
				{
				case CG_STRUCT:
					recurseParams(cgGetFirstStructParameter(parameter));
					break;
				case CG_ARRAY:
					// Support only 1-dimensional arrays
					arraySize = cgGetArraySize(parameter, 0);
					recurseParams(cgGetArrayParameter(parameter, 0), (size_t)arraySize);
					break;
				default:
					// Normal path (leaf)
					String paramName = cgGetParameterName(parameter);
					size_t logicalIndex = cgGetParameterResourceIndex(parameter);

					// Get the parameter resource, to calculate the physical index
					CGresource res = cgGetParameterResource(parameter);
					bool isRegisterCombiner = false;
					size_t regCombinerPhysicalIndex = 0;
					switch (res)
					{
					case CG_COMBINER_STAGE_CONST0:
						// register combiner, const 0
						// the index relates to the texture stage; store this as (stage * 2) + 0
						regCombinerPhysicalIndex = logicalIndex * 2;
						isRegisterCombiner = true;
						break;
					case CG_COMBINER_STAGE_CONST1:
						// register combiner, const 1
						// the index relates to the texture stage; store this as (stage * 2) + 1
						regCombinerPhysicalIndex = (logicalIndex * 2) + 1;
						isRegisterCombiner = true;
						break;
					default:
						// normal constant
						break;
					}

					// Trim the '[0]' suffix if it exists, we will add our own indexing later
					if (StringUtil::endsWith(paramName, "[0]", false))
					{
						paramName.erase(paramName.size() - 3);
					}


					GpuConstantDefinition def;
					def.arraySize = contextArraySize;
					mapTypeAndElementSize(paramType, isRegisterCombiner, def);

					if (def.constType == GCT_UNKNOWN)
					{
						LogManager::getSingleton().logMessage(
							"Problem parsing the following Cg Uniform: '"
							+ paramName + "' in file " + mName);
						// next uniform
						parameter = cgGetNextParameter(parameter);
						continue;
					}
					if (isRegisterCombiner)
					{
						def.physicalIndex = regCombinerPhysicalIndex;
					}
					else
					{
						// base position on existing buffer contents
						if (def.isFloat())
						{
							def.physicalIndex = mFloatLogicalToPhysical->bufferSize;
						}
						else
						{
							def.physicalIndex = mIntLogicalToPhysical->bufferSize;
						}
					}


					def.logicalIndex = logicalIndex;
					mConstantDefs->map.insert(GpuConstantDefinitionMap::value_type(paramName, def));

					// Record logical / physical mapping
					if (def.isFloat())
					{
						OGRE_LOCK_MUTEX(mFloatLogicalToPhysical->mutex)
						mFloatLogicalToPhysical->map.insert(
							GpuLogicalIndexUseMap::value_type(logicalIndex, 
								GpuLogicalIndexUse(def.physicalIndex, def.arraySize * def.elementSize, GPV_GLOBAL)));
						mFloatLogicalToPhysical->bufferSize += def.arraySize * def.elementSize;
						mConstantDefs->floatBufferSize = mFloatLogicalToPhysical->bufferSize;
					}
					else
					{
						OGRE_LOCK_MUTEX(mIntLogicalToPhysical->mutex)
						mIntLogicalToPhysical->map.insert(
							GpuLogicalIndexUseMap::value_type(logicalIndex, 
								GpuLogicalIndexUse(def.physicalIndex, def.arraySize * def.elementSize, GPV_GLOBAL)));
						mIntLogicalToPhysical->bufferSize += def.arraySize * def.elementSize;
						mConstantDefs->intBufferSize = mIntLogicalToPhysical->bufferSize;
					}

					// Deal with array indexing
					mConstantDefs->generateConstantDefinitionArrayEntries(paramName, def);

					break;
		
				}
					
            }
            // Get next
            parameter = cgGetNextParameter(parameter);
        }

        
    }
	//-----------------------------------------------------------------------
	void CgProgram::mapTypeAndElementSize(CGtype cgType, bool isRegisterCombiner, 
		GpuConstantDefinition& def) const
	{
		if (isRegisterCombiner)
		{
			// register combiners are the only single-float entries in our buffer
			def.constType = GCT_FLOAT1;
			def.elementSize = 1;
		}
		else
		{
			switch(cgType)
			{
			case CG_FLOAT:
			case CG_FLOAT1:
			case CG_HALF:
			case CG_HALF1:
				def.constType = GCT_FLOAT1;
				break;
			case CG_FLOAT2:
			case CG_HALF2:
				def.constType = GCT_FLOAT2;
				break;
			case CG_FLOAT3:
			case CG_HALF3:
				def.constType = GCT_FLOAT3;
				break;
			case CG_FLOAT4:
			case CG_HALF4:
				def.constType = GCT_FLOAT4;
				break;
			case CG_FLOAT2x2:
			case CG_HALF2x2:
				def.constType = GCT_MATRIX_2X2;
				break;
			case CG_FLOAT2x3:
			case CG_HALF2x3:
				def.constType = GCT_MATRIX_2X3;
				break;
			case CG_FLOAT2x4:
			case CG_HALF2x4:
				def.constType = GCT_MATRIX_2X4;
				break;
			case CG_FLOAT3x2:
			case CG_HALF3x2:
				def.constType = GCT_MATRIX_3X2;
				break;
			case CG_FLOAT3x3:
			case CG_HALF3x3:
				def.constType = GCT_MATRIX_3X3;
				break;
			case CG_FLOAT3x4:
			case CG_HALF3x4:
				def.constType = GCT_MATRIX_3X4;
				break;
			case CG_FLOAT4x2:
			case CG_HALF4x2:
				def.constType = GCT_MATRIX_4X2;
				break;
			case CG_FLOAT4x3:
			case CG_HALF4x3:
				def.constType = GCT_MATRIX_4X3;
				break;
			case CG_FLOAT4x4:
			case CG_HALF4x4:
				def.constType = GCT_MATRIX_4X4;
				break;
			case CG_INT:
			case CG_INT1:
				def.constType = GCT_INT1;
				break;
			case CG_INT2:
				def.constType = GCT_INT2;
				break;
			case CG_INT3:
				def.constType = GCT_INT3;
				break;
			case CG_INT4:
				def.constType = GCT_INT4;
				break;
			default:
				def.constType = GCT_UNKNOWN;
				break;
			}
			// Cg pads
			def.elementSize = GpuConstantDefinition::getElementSize(def.constType, true);
		}
	}
    //-----------------------------------------------------------------------
    CgProgram::CgProgram(ResourceManager* creator, const String& name, 
        ResourceHandle handle, const String& group, bool isManual, 
        ManualResourceLoader* loader, CGcontext context)
        : HighLevelGpuProgram(creator, name, handle, group, isManual, loader), 
        mCgContext(context), mCgProgram(0), 
        mSelectedCgProfile(CG_PROFILE_UNKNOWN), mCgArguments(0)
    {
        if (createParamDictionary("CgProgram"))
        {
            setupBaseParamDictionary();

            ParamDictionary* dict = getParamDictionary();

            dict->addParameter(ParameterDef("entry_point", 
                "The entry point for the Cg program.",
                PT_STRING),&msCmdEntryPoint);
            dict->addParameter(ParameterDef("profiles", 
                "Space-separated list of Cg profiles supported by this profile.",
                PT_STRING),&msCmdProfiles);
            dict->addParameter(ParameterDef("compile_arguments", 
                "A string of compilation arguments to pass to the Cg compiler.",
                PT_STRING),&msCmdArgs);
        }
        
    }
    //-----------------------------------------------------------------------
    CgProgram::~CgProgram()
    {
        freeCgArgs();
        // have to call this here reather than in Resource destructor
        // since calling virtual methods in base destructors causes crash
        if (isLoaded())
        {
            unload();
        }
        else
        {
            unloadHighLevel();
        }
    }
    //-----------------------------------------------------------------------
    bool CgProgram::isSupported(void) const
    {
        if (mCompileError || !isRequiredCapabilitiesSupported())
            return false;

		StringVector::const_iterator i, iend;
        iend = mProfiles.end();
        // Check to see if any of the profiles are supported
        for (i = mProfiles.begin(); i != iend; ++i)
        {
            if (GpuProgramManager::getSingleton().isSyntaxSupported(*i))
            {
                return true;
            }
        }
        return false;

    }
    //-----------------------------------------------------------------------
    void CgProgram::setProfiles(const StringVector& profiles)
    {
        mProfiles.clear();
        StringVector::const_iterator i, iend;
        iend = profiles.end();
        for (i = profiles.begin(); i != iend; ++i)
        {
            mProfiles.push_back(*i);
        }
    }
	//-----------------------------------------------------------------------
	String CgProgram::resolveCgIncludes(const String& inSource, Resource* resourceBeingLoaded, const String& fileName)
	{
		String outSource;
		// output will be at least this big
		outSource.reserve(inSource.length());

		size_t startMarker = 0;
		size_t i = inSource.find("#include");
		while (i != String::npos)
		{
			size_t includePos = i;
			size_t afterIncludePos = includePos + 8;
			size_t newLineBefore = inSource.rfind("\n", includePos);

			// check we're not in a comment
			size_t lineCommentIt = inSource.rfind("//", includePos);
			if (lineCommentIt != String::npos)
			{
				if (newLineBefore == String::npos || lineCommentIt > newLineBefore)
				{
					// commented
					i = inSource.find("#include", afterIncludePos);
					continue;
				}

			}
			size_t blockCommentIt = inSource.rfind("/*", includePos);
			if (blockCommentIt != String::npos)
			{
				size_t closeCommentIt = inSource.rfind("*/", includePos);
				if (closeCommentIt == String::npos || closeCommentIt < blockCommentIt)
				{
					// commented
					i = inSource.find("#include", afterIncludePos);
					continue;
				}

			}

			// find following newline (or EOF)
			size_t newLineAfter = inSource.find("\n", afterIncludePos);
			// find include file string container
			String endDelimeter = "\"";
			size_t startIt = inSource.find("\"", afterIncludePos);
			if (startIt == String::npos || startIt > newLineAfter)
			{
				// try <>
				startIt = inSource.find("<", afterIncludePos);
				if (startIt == String::npos || startIt > newLineAfter)
				{
					OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR,
						"Badly formed #include directive (expected \" or <) in file "
						+ fileName + ": " + inSource.substr(includePos, newLineAfter-includePos),
						"CgProgram::preprocessor");
				}
				else
				{
					endDelimeter = ">";
				}
			}
			size_t endIt = inSource.find(endDelimeter, startIt+1);
			if (endIt == String::npos || endIt <= startIt)
			{
				OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR,
					"Badly formed #include directive (expected " + endDelimeter + ") in file "
					+ fileName + ": " + inSource.substr(includePos, newLineAfter-includePos),
					"CgProgram::preprocessor");
			}

			// extract filename
			String filename(inSource.substr(startIt+1, endIt-startIt-1));

			// open included file
			DataStreamPtr resource = ResourceGroupManager::getSingleton().
				openResource(filename, resourceBeingLoaded->getGroup(), true, resourceBeingLoaded);

			// replace entire include directive line
			// copy up to just before include
			if (newLineBefore != String::npos && newLineBefore >= startMarker)
				outSource.append(inSource.substr(startMarker, newLineBefore-startMarker+1));

			size_t lineCount = 0;
			size_t lineCountPos = 0;
			
			// Count the line number of #include statement
			lineCountPos = outSource.find('\n');
			while(lineCountPos != String::npos)
			{
				lineCountPos = outSource.find('\n', lineCountPos+1);
				lineCount++;
			}

			// Add #line to the start of the included file to correct the line count
			outSource.append("#line 1 \"" + filename + "\"\n");

			outSource.append(resource->getAsString());

			// Add #line to the end of the included file to correct the line count
			outSource.append("\n#line " + Ogre::StringConverter::toString(lineCount) + 
				"\"" + fileName + "\"\n");

			startMarker = newLineAfter;

			if (startMarker != String::npos)
				i = inSource.find("#include", startMarker);
			else
				i = String::npos;

		}
		// copy any remaining characters
		outSource.append(inSource.substr(startMarker));

		return outSource;
	}
    //-----------------------------------------------------------------------
    const String& CgProgram::getLanguage(void) const
    {
        static const String language = "cg";

        return language;
    }


    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    String CgProgram::CmdEntryPoint::doGet(const void *target) const
    {
        return static_cast<const CgProgram*>(target)->getEntryPoint();
    }
    void CgProgram::CmdEntryPoint::doSet(void *target, const String& val)
    {
        static_cast<CgProgram*>(target)->setEntryPoint(val);
    }
    //-----------------------------------------------------------------------
    String CgProgram::CmdProfiles::doGet(const void *target) const
    {
        return StringConverter::toString(
            static_cast<const CgProgram*>(target)->getProfiles() );
    }
    void CgProgram::CmdProfiles::doSet(void *target, const String& val)
    {
        static_cast<CgProgram*>(target)->setProfiles(StringUtil::split(val));
    }
    //-----------------------------------------------------------------------
    String CgProgram::CmdArgs::doGet(const void *target) const
    {
        return static_cast<const CgProgram*>(target)->getCompileArguments();
    }
    void CgProgram::CmdArgs::doSet(void *target, const String& val)
    {
        static_cast<CgProgram*>(target)->setCompileArguments(val);
    }

}

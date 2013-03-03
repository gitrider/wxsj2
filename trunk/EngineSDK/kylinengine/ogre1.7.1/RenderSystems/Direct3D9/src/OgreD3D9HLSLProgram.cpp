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
#include "OgreD3D9HLSLProgram.h"
#include "OgreGpuProgramManager.h"
#include "OgreStringConverter.h"
#include "OgreD3D9GpuProgram.h"

namespace Ogre {
    //-----------------------------------------------------------------------
    D3D9HLSLProgram::CmdEntryPoint D3D9HLSLProgram::msCmdEntryPoint;
    D3D9HLSLProgram::CmdTarget D3D9HLSLProgram::msCmdTarget;
    D3D9HLSLProgram::CmdPreprocessorDefines D3D9HLSLProgram::msCmdPreprocessorDefines;
    D3D9HLSLProgram::CmdColumnMajorMatrices D3D9HLSLProgram::msCmdColumnMajorMatrices;
	D3D9HLSLProgram::CmdOptimisation D3D9HLSLProgram::msCmdOptimisation;
	D3D9HLSLProgram::CmdMicrocode D3D9HLSLProgram::msCmdMicrocode;
	D3D9HLSLProgram::CmdAssemblerCode D3D9HLSLProgram::msCmdAssemblerCode;

	class _OgreD3D9Export HLSLIncludeHandler : public ID3DXInclude
	{
	public:
		HLSLIncludeHandler(Resource* sourceProgram) 
			: mProgram(sourceProgram) {}
		~HLSLIncludeHandler() {}
		
		STDMETHOD(Open)(D3DXINCLUDE_TYPE IncludeType,
			LPCSTR pFileName,
			LPCVOID pParentData,
			LPCVOID *ppData,
			UINT *pByteLen
			)
		{
			// find & load source code
			DataStreamPtr stream = 
				ResourceGroupManager::getSingleton().openResource(
				String(pFileName), mProgram->getGroup(), true, mProgram);

			String source = stream->getAsString();
			// copy into separate c-string
			// Note - must NOT copy the null terminator, otherwise this will terminate
			// the entire program string!
			*pByteLen = static_cast<UINT>(source.length());
			char* pChar = new char[*pByteLen];
			memcpy(pChar, source.c_str(), *pByteLen);
			*ppData = pChar;

			return S_OK;
		}

		STDMETHOD(Close)(LPCVOID pData)
		{
			char* pChar = (char*)pData;
			delete [] pChar;
			return S_OK;
		}
	protected:
		Resource* mProgram;


	};

    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    void D3D9HLSLProgram::loadFromSource(void)
    {
        // Populate preprocessor defines
        String stringBuffer;

        vector<D3DXMACRO>::type defines;
        const D3DXMACRO* pDefines = 0;
        if (!mPreprocessorDefines.empty())
        {
            stringBuffer = mPreprocessorDefines;

            // Split preprocessor defines and build up macro array
            D3DXMACRO macro;
            String::size_type pos = 0;
            while (pos != String::npos)
            {
                macro.Name = &stringBuffer[pos];
                macro.Definition = 0;

				String::size_type start_pos=pos;

                // Find delims
                pos = stringBuffer.find_first_of(";,=", pos);

				if(start_pos==pos)
				{
					if(pos==stringBuffer.length())
					{
						break;
					}
					pos++;
					continue;
				}

                if (pos != String::npos)
                {
                    // Check definition part
                    if (stringBuffer[pos] == '=')
                    {
                        // Setup null character for macro name
                        stringBuffer[pos++] = '\0';
                        macro.Definition = &stringBuffer[pos];
                        pos = stringBuffer.find_first_of(";,", pos);
                    }
                    else
                    {
                        // No definition part, define as "1"
                        macro.Definition = "1";
                    }

                    if (pos != String::npos)
                    {
                        // Setup null character for macro name or definition
                        stringBuffer[pos++] = '\0';
                    }
                }
				else
				{
					macro.Definition = "1";
				}
				if(strlen(macro.Name)>0)
				{
					defines.push_back(macro);
				}
				else
				{
					break;
				}
            }

            // Add NULL terminator
            macro.Name = 0;
            macro.Definition = 0;
            defines.push_back(macro);

            pDefines = &defines[0];
        }

        // Populate compile flags
        DWORD compileFlags = 0;
        if (mColumnMajorMatrices)
            compileFlags |= D3DXSHADER_PACKMATRIX_COLUMNMAJOR;
        else
            compileFlags |= D3DXSHADER_PACKMATRIX_ROWMAJOR;

#if OGRE_DEBUG_MODE
		compileFlags |= D3DXSHADER_DEBUG;
#endif
		switch (mOptimisationLevel)
		{
		case OPT_DEFAULT:
			compileFlags |= D3DXSHADER_OPTIMIZATION_LEVEL1;
			break;
		case OPT_NONE:
			compileFlags |= D3DXSHADER_SKIPOPTIMIZATION;
			break;
		case OPT_0:
			compileFlags |= D3DXSHADER_OPTIMIZATION_LEVEL0;
			break;
		case OPT_1:
			compileFlags |= D3DXSHADER_OPTIMIZATION_LEVEL1;
			break;
		case OPT_2:
			compileFlags |= D3DXSHADER_OPTIMIZATION_LEVEL2;
			break;
		case OPT_3:
			compileFlags |= D3DXSHADER_OPTIMIZATION_LEVEL3;
			break;
		}


        LPD3DXBUFFER errors = 0;

		// include handler
		HLSLIncludeHandler includeHandler(this);

        // Compile & assemble into microcode
        HRESULT hr = D3DXCompileShader(
            mSource.c_str(),
            static_cast<UINT>(mSource.length()),
            pDefines,
            &includeHandler, 
            mEntryPoint.c_str(),
            mTarget.c_str(),
            compileFlags,
            &mpMicroCode,
            &errors,
            &mpConstTable);

        if (FAILED(hr))
        {
            String message = "Cannot assemble D3D9 high-level shader " + mName + " Errors:\n" +
                static_cast<const char*>(errors->GetBufferPointer());
            errors->Release();
            OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, message,
                "D3D9HLSLProgram::loadFromSource");
        }


    }
    //-----------------------------------------------------------------------
    void D3D9HLSLProgram::createLowLevelImpl(void)
    {
		if (!mCompileError)
		{
			// Create a low-level program, give it the same name as us
			mAssemblerProgram = 
				GpuProgramManager::getSingleton().createProgramFromString(
					mName, 
					mGroup,
					"",// dummy source, since we'll be using microcode
					mType, 
					mTarget);
			static_cast<D3D9GpuProgram*>(mAssemblerProgram.get())->setExternalMicrocode(mpMicroCode);
		}

    }
    //-----------------------------------------------------------------------
    void D3D9HLSLProgram::unloadHighLevelImpl(void)
    {
        SAFE_RELEASE(mpMicroCode);
        SAFE_RELEASE(mpConstTable);

    }
    //-----------------------------------------------------------------------
    void D3D9HLSLProgram::buildConstantDefinitions() const
    {
        // Derive parameter names from const table
        assert(mpConstTable && "Program not loaded!");
        // Get contents of the constant table
        D3DXCONSTANTTABLE_DESC desc;
        HRESULT hr = mpConstTable->GetDesc(&desc);

		createParameterMappingStructures(true);

        if (FAILED(hr))
        {
            OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, 
                "Cannot retrieve constant descriptions from HLSL program.", 
                "D3D9HLSLProgram::buildParameterNameMap");
        }
        // Iterate over the constants
        for (unsigned int i = 0; i < desc.Constants; ++i)
        {
            // Recursively descend through the structure levels
            processParamElement(NULL, "", i);
        }

        
    }
    //-----------------------------------------------------------------------
    void D3D9HLSLProgram::processParamElement(D3DXHANDLE parent, String prefix, 
        unsigned int index) const
    {
        D3DXHANDLE hConstant = mpConstTable->GetConstant(parent, index);

        // Since D3D HLSL doesn't deal with naming of array and struct parameters
        // automatically, we have to do it by hand

        D3DXCONSTANT_DESC desc;
        unsigned int numParams = 1;
        HRESULT hr = mpConstTable->GetConstantDesc(hConstant, &desc, &numParams);
        if (FAILED(hr))
        {
            OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, 
                "Cannot retrieve constant description from HLSL program.", 
                "D3D9HLSLProgram::processParamElement");
        }

        String paramName = desc.Name;
        // trim the odd '$' which appears at the start of the names in HLSL
        if (paramName.at(0) == '$')
            paramName.erase(paramName.begin());

		// Also trim the '[0]' suffix if it exists, we will add our own indexing later
		if (StringUtil::endsWith(paramName, "[0]", false))
		{
			paramName.erase(paramName.size() - 3);
		}


        if (desc.Class == D3DXPC_STRUCT)
        {
            // work out a new prefix for nested members, if it's an array, we need an index
            prefix = prefix + paramName + ".";
            // Cascade into struct
            for (unsigned int i = 0; i < desc.StructMembers; ++i)
            {
                processParamElement(hConstant, prefix, i);
            }
        }
        else
        {
            // Process params
            if (desc.Type == D3DXPT_FLOAT || desc.Type == D3DXPT_INT || desc.Type == D3DXPT_BOOL)
            {
                size_t paramIndex = desc.RegisterIndex;
                String name = prefix + paramName;
                
				GpuConstantDefinition def;
				def.logicalIndex = paramIndex;
				// populate type, array size & element size
				populateDef(desc, def);
				if (def.isFloat())
				{
					def.physicalIndex = mFloatLogicalToPhysical->bufferSize;
					OGRE_LOCK_MUTEX(mFloatLogicalToPhysical->mutex)
					mFloatLogicalToPhysical->map.insert(
						GpuLogicalIndexUseMap::value_type(paramIndex, 
						GpuLogicalIndexUse(def.physicalIndex, def.arraySize * def.elementSize, GPV_GLOBAL)));
					mFloatLogicalToPhysical->bufferSize += def.arraySize * def.elementSize;
					mConstantDefs->floatBufferSize = mFloatLogicalToPhysical->bufferSize;
				}
				else
				{
					def.physicalIndex = mIntLogicalToPhysical->bufferSize;
					OGRE_LOCK_MUTEX(mIntLogicalToPhysical->mutex)
					mIntLogicalToPhysical->map.insert(
						GpuLogicalIndexUseMap::value_type(paramIndex, 
						GpuLogicalIndexUse(def.physicalIndex, def.arraySize * def.elementSize, GPV_GLOBAL)));
					mIntLogicalToPhysical->bufferSize += def.arraySize * def.elementSize;
					mConstantDefs->intBufferSize = mIntLogicalToPhysical->bufferSize;
				}

                mConstantDefs->map.insert(GpuConstantDefinitionMap::value_type(name, def));

				// Now deal with arrays
				mConstantDefs->generateConstantDefinitionArrayEntries(name, def);
            }
        }
            
    }
	//-----------------------------------------------------------------------
	void D3D9HLSLProgram::populateDef(D3DXCONSTANT_DESC& d3dDesc, GpuConstantDefinition& def) const
	{
		def.arraySize = d3dDesc.Elements;
		switch(d3dDesc.Type)
		{
		case D3DXPT_INT:
			switch(d3dDesc.Columns)
			{
			case 1:
				def.constType = GCT_INT1;
				break;
			case 2:
				def.constType = GCT_INT2;
				break;
			case 3:
				def.constType = GCT_INT3;
				break;
			case 4:
				def.constType = GCT_INT4;
				break;
			} // columns
			break;
		case D3DXPT_FLOAT:
			switch(d3dDesc.Class)
			{
			case D3DXPC_MATRIX_COLUMNS:
			case D3DXPC_MATRIX_ROWS:
				{
					int firstDim, secondDim;
					firstDim = d3dDesc.RegisterCount / d3dDesc.Elements;
					if (d3dDesc.Class == D3DXPC_MATRIX_ROWS)
					{
						secondDim = d3dDesc.Columns;
					}
					else
					{
						secondDim = d3dDesc.Rows;
					}
					switch(firstDim)
					{
					case 2:
						switch(secondDim)
						{
						case 2:
							def.constType = GCT_MATRIX_2X2;
							def.elementSize = 8; // HLSL always packs
							break;
						case 3:
							def.constType = GCT_MATRIX_2X3;
							def.elementSize = 8; // HLSL always packs
							break;
						case 4:
							def.constType = GCT_MATRIX_2X4;
							def.elementSize = 8; 
							break;
						} // columns
						break;
					case 3:
						switch(secondDim)
						{
						case 2:
							def.constType = GCT_MATRIX_3X2;
							def.elementSize = 12; // HLSL always packs
							break;
						case 3:
							def.constType = GCT_MATRIX_3X3;
							def.elementSize = 12; // HLSL always packs
							break;
						case 4:
							def.constType = GCT_MATRIX_3X4;
							def.elementSize = 12; 
							break;
						} // columns
						break;
					case 4:
						switch(secondDim)
						{
						case 2:
							def.constType = GCT_MATRIX_4X2;
							def.elementSize = 16; // HLSL always packs
							break;
						case 3:
							def.constType = GCT_MATRIX_4X3;
							def.elementSize = 16; // HLSL always packs
							break;
						case 4:
							def.constType = GCT_MATRIX_4X4;
							def.elementSize = 16; 
							break;
						} // secondDim
						break;

					} // firstDim
				}
				break;
			case D3DXPC_SCALAR:
			case D3DXPC_VECTOR:
				switch(d3dDesc.Columns)
				{
				case 1:
					def.constType = GCT_FLOAT1;
					break;
				case 2:
					def.constType = GCT_FLOAT2;
					break;
				case 3:
					def.constType = GCT_FLOAT3;
					break;
				case 4:
					def.constType = GCT_FLOAT4;
					break;
				} // columns
				break;
			}
		default:
			// not mapping samplers, don't need to take the space 
			break;
		};

		// D3D9 pads to 4 elements
		def.elementSize = GpuConstantDefinition::getElementSize(def.constType, true);


	}

	LPD3DXBUFFER D3D9HLSLProgram::getMicroCode()
	{
		return mpMicroCode;
	}

    //-----------------------------------------------------------------------
    D3D9HLSLProgram::D3D9HLSLProgram(ResourceManager* creator, const String& name, 
        ResourceHandle handle, const String& group, bool isManual, 
        ManualResourceLoader* loader)
        : HighLevelGpuProgram(creator, name, handle, group, isManual, loader)
        , mTarget()
        , mEntryPoint()
        , mPreprocessorDefines()
        , mColumnMajorMatrices(true)
        , mpMicroCode(NULL), mpConstTable(NULL)
		, mOptimisationLevel(OPT_DEFAULT)
    {
        if (createParamDictionary("D3D9HLSLProgram"))
        {
            setupBaseParamDictionary();
            ParamDictionary* dict = getParamDictionary();

            dict->addParameter(ParameterDef("entry_point", 
                "The entry point for the HLSL program.",
                PT_STRING),&msCmdEntryPoint);
            dict->addParameter(ParameterDef("target", 
                "Name of the assembler target to compile down to.",
                PT_STRING),&msCmdTarget);
            dict->addParameter(ParameterDef("preprocessor_defines", 
                "Preprocessor defines use to compile the program.",
                PT_STRING),&msCmdPreprocessorDefines);
            dict->addParameter(ParameterDef("column_major_matrices", 
                "Whether matrix packing in column-major order.",
                PT_BOOL),&msCmdColumnMajorMatrices);
			dict->addParameter(ParameterDef("optimisation_level", 
				"The optimisation level to use.",
				PT_STRING),&msCmdOptimisation);
			dict->addParameter(ParameterDef("micro_code", 
				"the micro code.",
				PT_STRING),&msCmdMicrocode);
			dict->addParameter(ParameterDef("assemble_code", 
				"the assemble code.",
				PT_STRING),&msCmdAssemblerCode);
        }
        
    }
    //-----------------------------------------------------------------------
    D3D9HLSLProgram::~D3D9HLSLProgram()
    {
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
    bool D3D9HLSLProgram::isSupported(void) const
    {
        if (mCompileError || !isRequiredCapabilitiesSupported())
            return false;

        return GpuProgramManager::getSingleton().isSyntaxSupported(mTarget);
    }
    //-----------------------------------------------------------------------
    GpuProgramParametersSharedPtr D3D9HLSLProgram::createParameters(void)
    {
        // Call superclass
        GpuProgramParametersSharedPtr params = HighLevelGpuProgram::createParameters();

        // Need to transpose matrices if compiled with column-major matrices
        params->setTransposeMatrices(mColumnMajorMatrices);

        return params;
    }
    //-----------------------------------------------------------------------
    void D3D9HLSLProgram::setTarget(const String& target)
    {
        mTarget = target;
    }

    //-----------------------------------------------------------------------
    const String& D3D9HLSLProgram::getLanguage(void) const
    {
        static const String language = "hlsl";

        return language;
    }

    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    String D3D9HLSLProgram::CmdEntryPoint::doGet(const void *target) const
    {
        return static_cast<const D3D9HLSLProgram*>(target)->getEntryPoint();
    }
    void D3D9HLSLProgram::CmdEntryPoint::doSet(void *target, const String& val)
    {
        static_cast<D3D9HLSLProgram*>(target)->setEntryPoint(val);
    }
    //-----------------------------------------------------------------------
    String D3D9HLSLProgram::CmdTarget::doGet(const void *target) const
    {
        return static_cast<const D3D9HLSLProgram*>(target)->getTarget();
    }
    void D3D9HLSLProgram::CmdTarget::doSet(void *target, const String& val)
    {
        static_cast<D3D9HLSLProgram*>(target)->setTarget(val);
    }
    //-----------------------------------------------------------------------
    String D3D9HLSLProgram::CmdPreprocessorDefines::doGet(const void *target) const
    {
        return static_cast<const D3D9HLSLProgram*>(target)->getPreprocessorDefines();
    }
    void D3D9HLSLProgram::CmdPreprocessorDefines::doSet(void *target, const String& val)
    {
        static_cast<D3D9HLSLProgram*>(target)->setPreprocessorDefines(val);
    }
    //-----------------------------------------------------------------------
    String D3D9HLSLProgram::CmdColumnMajorMatrices::doGet(const void *target) const
    {
        return StringConverter::toString(static_cast<const D3D9HLSLProgram*>(target)->getColumnMajorMatrices());
    }
    void D3D9HLSLProgram::CmdColumnMajorMatrices::doSet(void *target, const String& val)
    {
        static_cast<D3D9HLSLProgram*>(target)->setColumnMajorMatrices(StringConverter::parseBool(val));
    }
	//-----------------------------------------------------------------------
	String D3D9HLSLProgram::CmdOptimisation::doGet(const void *target) const
	{
		switch(static_cast<const D3D9HLSLProgram*>(target)->getOptimisationLevel())
		{
		default:
		case OPT_DEFAULT:
			return "default";
		case OPT_NONE:
			return "none";
		case OPT_0:
			return "0";
		case OPT_1:
			return "1";
		case OPT_2:
			return "2";
		case OPT_3:
			return "3";
		}
	}
	void D3D9HLSLProgram::CmdOptimisation::doSet(void *target, const String& val)
	{
		if (StringUtil::startsWith(val, "default", true))
			static_cast<D3D9HLSLProgram*>(target)->setOptimisationLevel(OPT_DEFAULT);
		else if (StringUtil::startsWith(val, "none", true))
			static_cast<D3D9HLSLProgram*>(target)->setOptimisationLevel(OPT_NONE);
		else if (StringUtil::startsWith(val, "0", true))
			static_cast<D3D9HLSLProgram*>(target)->setOptimisationLevel(OPT_0);
		else if (StringUtil::startsWith(val, "1", true))
			static_cast<D3D9HLSLProgram*>(target)->setOptimisationLevel(OPT_1);
		else if (StringUtil::startsWith(val, "2", true))
			static_cast<D3D9HLSLProgram*>(target)->setOptimisationLevel(OPT_2);
		else if (StringUtil::startsWith(val, "3", true))
			static_cast<D3D9HLSLProgram*>(target)->setOptimisationLevel(OPT_3);
	}

    //-----------------------------------------------------------------------
    String D3D9HLSLProgram::CmdMicrocode::doGet(const void *target) const
    {
		D3D9HLSLProgram* program=const_cast<D3D9HLSLProgram*>(static_cast<const D3D9HLSLProgram*>(target));
		LPD3DXBUFFER buffer=program->getMicroCode();
		if(buffer)
		{
			char* str  =static_cast<Ogre::String::value_type*>(buffer->GetBufferPointer());
			size_t size=static_cast<size_t>(buffer->GetBufferSize());
			Ogre::String code;
			code.assign(str,size);
			return code;
		}
		else
		{
			return String();
		}
    }
    void D3D9HLSLProgram::CmdMicrocode::doSet(void *target, const String& val)
    {
		//nothing to do 
		//static_cast<D3D9HLSLProgram*>(target)->setColumnMajorMatrices(StringConverter::parseBool(val));
    }
    //-----------------------------------------------------------------------
    String D3D9HLSLProgram::CmdAssemblerCode::doGet(const void *target) const
    {
		D3D9HLSLProgram* program=const_cast<D3D9HLSLProgram*>(static_cast<const D3D9HLSLProgram*>(target));
		LPD3DXBUFFER buffer=program->getMicroCode();
		if(buffer)
		{
			CONST DWORD* code =static_cast<CONST DWORD*>(buffer->GetBufferPointer());
			LPD3DXBUFFER pDisassembly=0;
			HRESULT hr=D3DXDisassembleShader(code,FALSE,"// assemble code from D3D9HLSLProgram\n",&pDisassembly);
			if(pDisassembly)
			{
				char* str  =static_cast<Ogre::String::value_type*>(pDisassembly->GetBufferPointer());
				size_t size=static_cast<size_t>(pDisassembly->GetBufferSize());
				Ogre::String assemble_code;
				assemble_code.assign(str,size);
				pDisassembly->Release();
				return assemble_code;
			}
			return String();
		}
		else
		{
			return String();
		}
    }
    void D3D9HLSLProgram::CmdAssemblerCode::doSet(void *target, const String& val)
    {
		//nothing to do 
		//static_cast<D3D9HLSLProgram*>(target)->setColumnMajorMatrices(StringConverter::parseBool(val));
    }
}

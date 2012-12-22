#ifdef ASC_RENDERER_GLES2

#include "ASCGenInc.h"
#include "ASCGLES2Shader.h"
#include "ASCFileIO.h"
#include "ASCINIFile.h"
#include "ASCRenderer.h"
#include "../Ascension/Ascension.h"
#include "ASCCamera.h"

#ifdef ASC_IOS
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/EAGL.h>
#else
#include "EGL/egl.h"
#include "GLES2/gl2.h"
#endif

ASCGLES2Shader::ASCGLES2Shader(ASCString& strFileName)
: m_strFileName ( strFileName )
, m_uActiveTechinque ( 0 )
{
	Initialise();
}


ASCGLES2Shader::~ASCGLES2Shader(void)
{
	for(UINT32 i = 0; i < m_vecTechinques.size(); ++i)
	{
		ASC_Techinque& sTechnique = m_vecTechinques[i];
		glDeleteProgram(sTechnique.m_uTechniqueNum);
		glDeleteShader(sTechnique.m_uVertexShader);
		glDeleteShader(sTechnique.m_uPixelShader);

		SafeDelete( sTechnique.m_vecAttributeSize );

		SafeDelete( sTechnique.m_vecUniformTypes);

		SafeDelete( sTechnique.m_vecUniformLocation);

		SafeDelete( sTechnique.m_vecAttributeIDs );
		SafeDelete( sTechnique.m_vecTextureIDs);
		SafeDelete( sTechnique.m_vecUniformIDs);
	}
}

void 
ASCGLES2Shader::Initialise()
{
	m_strFileNameNoExt = m_strFileName;
	m_strFileName += ASCString(".ascshdr");

	//ASCDebuggingManager::OutputWindowLine("ASC INFO - Begun loading shader - ", ASCDebugString( m_strFileName.c_str() ));

	ASCINIFile INIFile;
	if(INIFile.Load(m_strFileName))
    {
        UINT32 uTechniqueCount = INIFile.GetUInt("General", "TechniqueCount");
        for(UINT32 i = 0; i < uTechniqueCount; ++i)
        {
            LoadTechnique(i);
		//ASCDebuggingManager::OutputWindowLine("ASC INFO - Loaded technique ", i+1);
        }
    }

	//ASCDebuggingManager::OutputWindowLine("ASC INFO - Finished loading shader - ", ASCDebugString( m_strFileName.c_str() ));
}
void 
ASCGLES2Shader::LoadTechnique(UINT32 uVal)
{
	m_vecTechinques.push_back( ASC_Techinque() );
	ASC_Techinque& sTechnique = m_vecTechinques[m_vecTechinques.size()-1];


	ASCString strSection = "Technique_";
	strSection.AddSmallInt( uVal );

	ASCString strFileContents = "";
	ASCString strFileName;
	ASCString strLine;
	ASCFileIO File;

	sTechnique.m_strTechniqueName = Ascension::INI().GetString(m_strFileName, strSection, "Name");

	//Pixel Shader
	sTechnique.m_uPixelShader = glCreateShader(GL_FRAGMENT_SHADER);
	strFileName = Ascension::INI().GetString(m_strFileName, strSection, "PixelShader");

	if(File.Open(strFileName, IO_IN))
	{
		while(File.GetNextLine(strLine))
		{
			strFileContents += strLine;
			strFileContents += ASCString("\n");
		}
		File.Close();
	}
	else
	{
		ASCString strDebug = "Guts, Can not find file ";
		strDebug += strFileName;

		assert_now( strDebug.c_str() );
	}

	CHAR8* pstr = strFileContents.c_str();
	const CHAR8** ppstrData = new const CHAR8*[1];
	ppstrData[0] = pstr;
	
	glShaderSource(sTechnique.m_uPixelShader, 1, ppstrData, NULL);

	SafeDelete( ppstrData );
	
	glCompileShader(sTechnique.m_uPixelShader);

	SINT32 sShaderCompiled;
    glGetShaderiv(sTechnique.m_uPixelShader, GL_COMPILE_STATUS, &sShaderCompiled);
	if (sShaderCompiled == NULL)
	{
		SINT32 iInfoLogLength;
		SINT32 iCharsWritten;
		glGetShaderiv(sTechnique.m_uPixelShader, GL_INFO_LOG_LENGTH, &iInfoLogLength);

		// Allocate enough space for the message and retrieve it
		CHAR8* pszInfoLog = new CHAR8[iInfoLogLength];
        glGetShaderInfoLog(sTechnique.m_uPixelShader, iInfoLogLength, &iCharsWritten, pszInfoLog);

		ASCString strDebug = "Guts, Pixel Shader Did Not Complie Because: ";
		strDebug += ASCString(pszInfoLog);
		
		SafeDelete( pszInfoLog );

		assert_now( strDebug.c_str() );
	}

	//Vertex Shader
	sTechnique.m_uVertexShader = glCreateShader(GL_VERTEX_SHADER);
	strFileName = Ascension::INI().GetString(m_strFileName, strSection, "VertexShader");

	strFileContents = "";
	if(File.Open(strFileName, IO_IN))
	{
		while(File.GetNextLine(strLine))
		{
			strFileContents += strLine;
			strFileContents += ASCString("\n");
		}
		File.Close();
	}
	else
	{
		ASCString strDebug = "Guts, Can not find file ";
		strDebug += strFileName;

		assert_now( strDebug.c_str() );
	}

	pstr = strFileContents.c_str();
	ppstrData = new const CHAR8*[1];
	ppstrData[0] = pstr;
	
	glShaderSource(sTechnique.m_uVertexShader, 1, ppstrData, NULL);

	SafeDelete( ppstrData );
	
	glCompileShader(sTechnique.m_uVertexShader);

    glGetShaderiv(sTechnique.m_uPixelShader, GL_COMPILE_STATUS, &sShaderCompiled);
	if (sShaderCompiled == NULL)
	{
		SINT32 iInfoLogLength;
		SINT32 iCharsWritten;
		glGetShaderiv(sTechnique.m_uVertexShader, GL_INFO_LOG_LENGTH, &iInfoLogLength);

		// Allocate enough space for the message and retrieve it
		CHAR8* pszInfoLog = new CHAR8[iInfoLogLength];
        glGetShaderInfoLog(sTechnique.m_uVertexShader, iInfoLogLength, &iCharsWritten, pszInfoLog);

		ASCString strDebug = "Guts, Vertex Shader Did Not Complie Because: ";
		strDebug += ASCString(pszInfoLog);

		SafeDelete( pszInfoLog );

		assert_now( strDebug.c_str() );
	}

	sTechnique.m_uTechniqueNum = glCreateProgram();
	
    glAttachShader(sTechnique.m_uTechniqueNum, sTechnique.m_uPixelShader);
    glAttachShader(sTechnique.m_uTechniqueNum, sTechnique.m_uVertexShader);

	
	
	UINT32 uAttributeCount = Ascension::INI().GetUInt(m_strFileName, strSection, "AttributeCount");
	ASCString strAttributeName;
	ASCString strAttributeID;
	ASCString strAttributeSize;
	sTechnique.m_vecAttributeCount = uAttributeCount;
	//sTechnique.m_vecAttributeNames = new ASCString[uAttributeCount];
	sTechnique.m_vecAttributeIDs = new UINT32[uAttributeCount];
	sTechnique.m_vecAttributeSize = new UINT32[uAttributeCount];
	for(UINT32 i = 0; i < uAttributeCount; ++i)
	{
		strAttributeID = "Attribute_";
		strAttributeID.AddSmallInt(i);
		strAttributeName = Ascension::INI().GetString(m_strFileName, strSection, strAttributeID);

		UINT32 uUniqueID = ASCGeneralFunctions::GenUniqueID();
		glBindAttribLocation(sTechnique.m_uTechniqueNum, i, strAttributeName.c_str());
		
		sTechnique.m_vecAttributeNames->push_back(strAttributeName);
		sTechnique.m_vecAttributeIDs[i] = uUniqueID;
		
		strAttributeSize = "AttributeSize_";
		strAttributeSize.AddSmallInt(i);
		sTechnique.m_vecAttributeSize[i] = Ascension::INI().GetUInt(m_strFileName, strSection, strAttributeSize);
	}
	
    glLinkProgram(sTechnique.m_uTechniqueNum);

    SINT32 sLinked;
    glGetProgramiv(sTechnique.m_uTechniqueNum, GL_LINK_STATUS, &sLinked);

	if (sLinked == NULL)
	{
		SINT32 iInfoLogLength;
		SINT32 iCharsWritten;
		glGetProgramiv(sTechnique.m_uTechniqueNum, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		char* pszInfoLog = new char[iInfoLogLength];
		glGetProgramInfoLog(sTechnique.m_uTechniqueNum, iInfoLogLength, &iCharsWritten, pszInfoLog);
		
		ASCString strDebug = "Guts, Shader Technique Did Not Link Because: ";
		strDebug += ASCString(pszInfoLog);

		SafeDelete( pszInfoLog );

		assert_now( strDebug.c_str() );
	}
	
	glUseProgram(sTechnique.m_uTechniqueNum);
	
	UINT32 uTextureCount = Ascension::INI().GetUInt(m_strFileName, strSection, "TextureSamplerCount");
	ASCString strTextureName;
	ASCString strTextureID;
	sTechnique.m_vecTextureCount = uTextureCount;
	sTechnique.m_vecTextureIDs = new UINT32[uTextureCount];
	for(UINT32 i = 0; i < uTextureCount; ++i)
	{
		strTextureID = "TextureSampler_";
		strTextureID.AddSmallInt(i);
		strTextureName = Ascension::INI().GetString(m_strFileName, strSection, strTextureID);

		glUniform1i(glGetUniformLocation(sTechnique.m_uTechniqueNum, strTextureName.c_str()), i);
		
		sTechnique.m_vecTextureNames->push_back(strTextureName);
		sTechnique.m_vecTextureIDs[i] = ASCGeneralFunctions::GenUniqueID();
	}
	
	
	UINT32 uUniformCount = Ascension::INI().GetUInt(m_strFileName, strSection, "UniformCount");
	ASCString strUniformName;
	ASCString strUniformType;
	ASCString strUniformID;
	sTechnique.m_uUniformCount = uUniformCount;
	sTechnique.m_vecUniformIDs = new UINT32[uUniformCount];
	sTechnique.m_vecUniformLocation = new UINT32[uUniformCount];
	sTechnique.m_vecUniformTypes = new ASC_GLES_UNIFORM_TYPES[uUniformCount];
	for(UINT32 i = 0; i < uUniformCount; ++i)
	{
		strUniformID = "Uniform_";
		strUniformID.AddSmallInt(i);
		strUniformName = Ascension::INI().GetString(m_strFileName, strSection, strUniformID);
		
		sTechnique.m_vecUniformNames->push_back(strUniformName);
		sTechnique.m_vecUniformIDs[i] = ASCGeneralFunctions::GenUniqueID();
		sTechnique.m_vecUniformLocation[i] = glGetUniformLocation(sTechnique.m_uTechniqueNum, strUniformName.c_str());
		
		strUniformID = "UniformType_";
		strUniformID.AddSmallInt(i);
		strUniformType = Ascension::INI().GetString(m_strFileName, strSection, strUniformID);
		sTechnique.m_vecUniformTypes[i] = ConvertStringToUniformType(strUniformType);
	}
    
    sTechnique.m_bUseCamera = Ascension::INI().GetBool(m_strFileName, strSection, "UseCamera");

	sTechnique.m_uID = ASCGeneralFunctions::GenUniqueID();
}
ASC_GLES_UNIFORM_TYPES 
ASCGLES2Shader::ConvertStringToUniformType(ASCString& str)
{
    static ASCString s_strMatrix = "mat4";
    static ASCString s_strFloat = "float";
	if(str == s_strMatrix)
	{
		return GLESUT_MAT4;
	}
	if(str == s_strFloat)
	{
		return GLESUT_FLOAT;
	}

	assert_nowex("Guts, Unknown uniform type of - ", str);
	return GLESUT_COUNT;
}
//Rendering
bool 
ASCGLES2Shader::Begin ( UINT32& uPassCount )
{
	uPassCount;
	assert_now("Guts, Not valid in GLES2");
	return false;
}
bool 
ASCGLES2Shader::Begin ( void )
{
	//glEnableVertexAttribArray(VERTEX_ARRAY);
	UINT32 uStride = Ascension::Renderer().CurrentVertexStride();
	ASC_Techinque& sTechnique = m_vecTechinques[m_uActiveTechinque];
	UINT32 uOffset = 0;
	for(UINT32 i = 0; i < sTechnique.m_vecAttributeCount; ++i)
	{
		//glVertexAttribPointer(sTechnique.m_vecAttributeIDs[i], sTechnique.m_vecAttributeSize[i], GL_FLOAT, GL_FALSE, uStride, reinterpret_cast<void*>(uOffset * sizeof(FLOAT32)));
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, sTechnique.m_vecAttributeSize[i], GL_FLOAT, GL_FALSE, uStride, reinterpret_cast<void*>(uOffset * sizeof(GL_FLOAT)));
		uOffset += sTechnique.m_vecAttributeSize[i];
	}
	for(UINT32 i = 0; i < sTechnique.m_vecTextureCount; ++i)
	{
		glUniform1i(glGetUniformLocation(sTechnique.m_uTechniqueNum, sTechnique.m_vecTextureNames->at(i).c_str()), i);
	}

#ifdef ASC_IOS
    ASC_Handle hRatio = GetNamedHandle("m_fScreenRatio");
    FLOAT32 fRatio = SC_FLOAT(Ascension::Width()) / SC_FLOAT(Ascension::Height());
#else
	Handle hRatio = GetNamedHandle("m_fScreenRatio");
    FLOAT32 fRatio = SC_FLOAT(Ascension::Height()) / SC_FLOAT(Ascension::Width());
#endif

    
	SetFVectors(hRatio, &fRatio, 0);
    
    if(sTechnique.m_bUseCamera)
    {
        ASC_Handle hCameraX = GetNamedHandle("m_fCameraX");
        ASC_Handle hCameraY = GetNamedHandle("m_fCameraY");
        
        
        FLOAT32 fTemp = Ascension::Camera().GetPos().X();
        SetFVectors(hCameraX, &fTemp, 0);
        
        fTemp = Ascension::Camera().GetPos().Y();
        SetFVectors(hCameraY, &fTemp, 0);
    }
	return true;
}
bool 
ASCGLES2Shader::BeginPass ( UINT32 uPassNum )
{
	assert_now("Guts, Not valid in GLES2");
	uPassNum;
	return false;
}
bool 
ASCGLES2Shader::EndPass ( void )
{
	assert_now("Guts, Not valid in GLES2");
	return false;
}
bool 
ASCGLES2Shader::End ( void )
{
	return true;
}

//Get handles
ASC_Handle 
ASCGLES2Shader::GetSemanticHandle( ASCString strSemantic )
{
	assert_now("Guts, Not GetSemanticHandle implemented");
	strSemantic;
	return NULL;
}
ASC_Handle 
ASCGLES2Shader::GetNamedHandle( ASCString strName )
{
    {
    ASC_Techinque& sTechnique = m_vecTechinques[m_uActiveTechinque];
    
    if(sTechnique.m_strTechniqueName == strName)
    {
        return &sTechnique.m_uID;
    }
    
    for(UINT32 j = 0; j < sTechnique.m_vecAttributeCount; ++j)
    {
        if(sTechnique.m_vecAttributeNames->at(j) == strName)
        {
            return &sTechnique.m_vecAttributeIDs[j];
        }
    }
    
    for(UINT32 j = 0; j < sTechnique.m_vecUniformNames->size(); ++j)
    {
        if(sTechnique.m_vecUniformNames->at(j) == strName)
        {
            return &sTechnique.m_vecUniformIDs[j];
        }
    }
    
    for(UINT32 j = 0; j < sTechnique.m_vecTextureCount; ++j)
    {
        if(sTechnique.m_vecTextureNames->at(j) == strName)
        {
            return &sTechnique.m_vecTextureNames[j];
        }
    }
    }
    
	for(UINT32 i = 0; i < m_vecTechinques.size(); ++i)
	{
        ASC_Techinque& sTechnique = m_vecTechinques[i];
		if(sTechnique.m_strTechniqueName == strName)
		{
			return &sTechnique.m_uID;
		}

		for(UINT32 j = 0; j < sTechnique.m_vecAttributeCount; ++j)
		{
			if(sTechnique.m_vecAttributeNames->at(j) == strName)
			{
				return &sTechnique.m_vecAttributeIDs[j];
			}
		}
		
		for(UINT32 j = 0; j < sTechnique.m_vecUniformNames->size(); ++j)
		{
			if(sTechnique.m_vecUniformNames->at(j) == strName)
			{
				return &sTechnique.m_vecUniformIDs[j];
			}
		}

		for(UINT32 j = 0; j < sTechnique.m_vecTextureCount; ++j)
		{
			if(sTechnique.m_vecTextureNames->at(j) == strName)
			{
				return &sTechnique.m_vecTextureNames[j];
			}
		}
	}
	return NULL;
}
bool 
ASCGLES2Shader::SetFVectors( ASC_Handle hTarget, const void* pSrc, UINT32 uQuadCount )
{
	UINT32 uID = *reinterpret_cast<const UINT32*>(hTarget);
	
	for(UINT32 j = 0; j < m_vecTechinques.size(); ++j)
	{
		ASC_Techinque& sTechnique = m_vecTechinques[j];
		for(UINT32 i = 0; i < sTechnique.m_uUniformCount; ++i)
		{
			if(sTechnique.m_vecUniformIDs[i] == uID)
			{
				if(sTechnique.m_vecUniformTypes[i] == GLESUT_MAT4)
				{
					glUniformMatrix4fv(sTechnique.m_vecUniformLocation[i], uQuadCount/4, GL_FALSE, reinterpret_cast<const FLOAT32*>(pSrc));
					return true;
				}
				else if(sTechnique.m_vecUniformTypes[i] == GLESUT_FLOAT )
				{
					FLOAT32 fVal = *reinterpret_cast<const FLOAT32*>(pSrc);
					glUniform1f(sTechnique.m_vecUniformLocation[i], fVal);
					return true;
				}
				else
				{
					assert_now("Guts, Invalid uniform type");
					return false;
				}
			}
		}
	}
	assert_now("Guts, Invalid ID");
	return false;
}
bool 
ASCGLES2Shader::SetTechnique( ASC_Handle hTarget )
{
	UINT32 uID = *reinterpret_cast<const UINT32*>(hTarget);
	
	for(UINT32 i = 0; i < m_vecTechinques.size(); ++i)
	{
		ASC_Techinque& sTechnique = m_vecTechinques[i];
		if(sTechnique.m_uID == uID)
		{
			m_uActiveTechinque = i;
			//Activates the shader
			glUseProgram(sTechnique.m_uTechniqueNum);
			
			//Binds all of the texture samplers
			for(UINT32 j = 0; j < sTechnique.m_vecTextureCount; ++j)
			{
				glUniform1i(glGetUniformLocation(sTechnique.m_vecTextureCount, sTechnique.m_vecTextureNames->at(j).c_str()), j);
			}
			return true;
		}
	}
	return false;
}

//Loading
void 
ASCGLES2Shader::Reload()
{
	assert_now("Guts, Not implemented");
}

//Get
ASCString 
ASCGLES2Shader::GetName()
{
	return m_strFileNameNoExt;
}

#endif
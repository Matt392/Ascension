#include "ASCGenInc.h"
#include "ASCFont.h"
#include "ASCFileIO.h"
#include "../Ascension/Ascension.h"
#include "ASCTextureManager.h"
#include "ASCRenderer.h"
#include "ASCTexture.h"
#include "ASCShader.h"
#include "ASCVertexDeclaration.h"
#include "ASCShaderManager.h"
#include "ASCVertexDeclaration.h"
#include "ASCVertexBuffer.h"

ASCFont::ASCFont( ASCString strFont )
: m_iCharacterCount ( 0 )
, m_pCharacters ( NULL )
, m_pFontTexture ( NULL )
, m_pVertDecl ( NULL )
{
    m_strID = strFont;
	ASCFileIO ioFile;
	if(ioFile.Open(m_strID, IO_IN))
	{
		m_pCharacters = new SCharacter[m_kiMaxCharacters];
		ASCString strLine;
		SINT32 iIsChar = 0;
		SINT32 iBitmap = 0;
		SINT32 i = 0;

		while(ioFile.GetNextLine(strLine))
		{
			//note: every line we want will have a '=' in it therfore check for it first
			i = strLine.find( '=' );
			if( i == -1 )
			{
				continue;
			}
			++i;
			//note: most of the lines we want will have "Char" so we check for it second
			iIsChar = strLine.find( "Char" );
			if(iIsChar >= 0)
			{
				ASCString strTemp;
				strLine.substr(i, strTemp);
				strLine = strTemp;
				ProcessLetterLine(strLine, m_iCharacterCount);
				++m_iCharacterCount;
				continue;
			}
			//note: only one line we want will have "Bitmap" so we check for it last
			iBitmap = strLine.find( "Bitmap" );
			if(iBitmap >= 0)
			{
				ASCString strTemp;
				strLine.substr(i, strTemp);
				strLine = strTemp;
				ProcessImageLine(strLine);
				continue;
			}
		}
	}
	else
	{
		assert_now("Failed to open font file");
	}
	m_uHighest = 0;
	for(UINT32 i = 0; i < m_iCharacterCount; ++i)
	{
		if(m_pCharacters[i].sRect.m_iHeight > m_uHighest)
		{
			m_uHighest = m_pCharacters[i].sRect.m_iHeight;
		}
	}

	m_pVertDecl = Ascension::Renderer().GetDefaultVertDecl();
	
	m_pRenderBuffer = Ascension::Renderer().CreateVertexBuffer(10*6, m_pVertDecl->GetStride());
}


ASCFont::~ASCFont(void)
{
	SafeDeleteArray( m_pCharacters );
}

void 
ASCFont::ProcessLetterLine( ASCString& strLine, UINT32 iCurCharacter )
{
	UINT32 i = 0;
	bool bHex = strLine[0] != '\"';
	if(bHex)
	{
		UINT32 iChar = 0;
		const char strHex[2] = { strLine[ 0 ], strLine[ 1 ] };
#ifdef ASC_IOS
		sscanf( strHex, "%2x", &iChar );
#else
		sscanf_s( strHex, "%2x", &iChar );
#endif
		m_pCharacters[ iCurCharacter ].cCharacter = static_cast< char >( iChar );
	}
	else
	{
		++i;
		m_pCharacters[ iCurCharacter ].cCharacter = strLine[ i ];
	}
	++i;

	ASCString strTemp;
	strLine.substr(i, strTemp);
	strLine = strTemp;

	ASCString strWorkWith;
	ASCString strTheRest;
	UINT32 iInfoBitsFound = 0;

	SINT32 iAt = strLine.find(',');
	if(iAt >= 0)
	{
		ASCString strTemp;
		strLine.substr(iAt+1, strTemp);
		strLine = strTemp;
	}

	while(iAt >= 0 && iInfoBitsFound < 6)
	{
		iAt = strLine.find(',');
		strLine.split(iAt+1, strWorkWith, strTheRest);
		ASCString strTemp;
		strLine.substr(iAt+1, strTemp);
		strLine = strTemp;
		if(strWorkWith[0] == ',')
		{
			continue;
		}

		switch( iInfoBitsFound )
		{
		case 0:
			{
				//m_pCharacters[ iCurCharacter ].sRect.iLeft = atoi( strWorkWith.c_str() );
				m_pCharacters[ iCurCharacter ].sRect.m_iX = atoi( strWorkWith.c_str() );
			}
			break;
		case 1:
			{
				//m_pCharacters[ iCurCharacter ].sRect.iTop = atoi( strWorkWith.c_str() );
				m_pCharacters[ iCurCharacter ].sRect.m_iY = atoi( strWorkWith.c_str() );
			}
			break;
		case 2:
			{
				//m_pCharacters[ iCurCharacter ].sRect.iRight = atoi( strWorkWith.c_str() );
				m_pCharacters[ iCurCharacter ].sRect.m_iWidth = atoi( strWorkWith.c_str() );
			}
			break;
		case 3:
			{
				//m_pCharacters[ iCurCharacter ].sRect.iBottom = atoi( strWorkWith.c_str() );
				m_pCharacters[ iCurCharacter ].sRect.m_iHeight = atoi( strWorkWith.c_str() );
			}
			break;
		case 4:
			{
				m_pCharacters[ iCurCharacter ].fPixelsBeforeChar = FLOAT32( atof( strWorkWith.c_str() ) );
			}
			break;
		case 5:
			{
				m_pCharacters[ iCurCharacter ].fPixelsAfterChar = FLOAT32( atof( strWorkWith.c_str() ) );
			}
			break;
		default:break;
		}
		++iInfoBitsFound;
		iAt = strLine.find(',');
		if(iAt < 0 && iInfoBitsFound == 5)
		{
			m_pCharacters[ iCurCharacter ].fPixelsAfterChar = FLOAT32( atof( strLine.c_str() ) );
		}
	}
}
void 
ASCFont::ProcessImageLine( ASCString& strLine )
{
	ASCTextureManager* pTextureManager = Ascension::Renderer().GetTextureManager();
	if(pTextureManager)
	{
		m_pFontTexture = pTextureManager->CreateTexture(strLine, true);
	}
	
	if(NULL == m_pFontTexture)
	{
		assert_now("Failed to load font texture");
	}
}
bool 
ASCFont::Render( FLOAT32 fX, FLOAT32 fY, ASCString& strMessage, UINT32 iColour, bool bCentered, bool bRight, UINT32 iColour2)
{
    if(strMessage.length() > 0)
    {
   // fX *= Ascension::ScaleX();
    //fY *= Ascension::ScaleY();
	FLOAT32 fRightAlignX = fX;
	FLOAT32 fOrigX = fX;
        //Ascension::Debugging().OutputWindowLine("FAC: ", bCentered);
       // Ascension::Debugging().OutputWindowLine("FAR: ", bRight);
	if(bCentered)
	{
        //Ascension::Debugging().OutputWindowLine("FC: ", ASCDebugString(strMessage.c_str()));
		fX -= GetLineWidth(strMessage, 0) * 0.5f;
	}
	else if(bRight)
	{
       // Ascension::Debugging().OutputWindowLine("FR: ", ASCDebugString(strMessage.c_str()));
		fX -=GetLineWidth(strMessage, 0);
	}
    else
    {
       // Ascension::Debugging().OutputWindowLine("FL: ", ASCDebugString(strMessage.c_str()));
    }
    
    
    
	UINT32 iLength = strMessage.length(); 

	SIRect sCharRect;
	FLOAT32 fCurX = fX;
	FLOAT32 fCurY = fY;
	FLOAT32 fULeft = 0.0f;
	FLOAT32 fVTop = 0.0f;
	FLOAT32 fURight = 0.0f;
	FLOAT32 fVBottom = 0.0f;
	FLOAT32 fTextureWidth = 0.0f;
	FLOAT32 fTextureHeight = 0.0f;
	if(m_pFontTexture)
	{
		fTextureWidth = (FLOAT32)m_pFontTexture->GetWidth();
		fTextureHeight = (FLOAT32)m_pFontTexture->GetHeight();
	}

	UINT32 uPrimCount = 0;

	if(iLength * 6 > m_pRenderBuffer->Size())
	{
		m_pRenderBuffer->Resize(iLength * 6);
	}
	SVertex* pData = reinterpret_cast<SVertex*>(m_pRenderBuffer->Lock());
	if(pData)
	{
		SVertex sCorners[4];
		for(UINT32 i = 0; i < iLength; ++i)
		{
			if(strMessage[i] == '\n')
			{
				fCurY += m_uHighest;
				fCurX = fX;
				if(bCentered)
				{
					fCurX = fOrigX - GetLineWidth(strMessage, i+1) * 0.5f;
				}
				else if(bRight)
				{
					fCurX = fRightAlignX - GetLineWidth(strMessage, i+1);
				}
				continue;
			}
			SCharacter& sCharData = GetCharacter(strMessage[i]);
			sCharRect = sCharData.sRect;

			fULeft = (FLOAT32)sCharRect.m_iX / fTextureWidth;
			fVTop = (FLOAT32)sCharRect.m_iY / fTextureHeight;
			fURight = ( (FLOAT32)sCharRect.m_iX + (FLOAT32)sCharRect.m_iWidth ) / fTextureWidth;
			fVBottom = ( (FLOAT32)sCharRect.m_iY + (FLOAT32)sCharRect.m_iHeight ) / fTextureHeight;
			
			// top left
#ifdef ASC_IOS
			sCorners[0].m_fX = (fCurX + sCharData.fPixelsBeforeChar) / Ascension::Height();
			sCorners[0].m_fY = (fCurY) / Ascension::Width();
#else
			sCorners[0].m_fX = (fCurX + sCharData.fPixelsBeforeChar) / Ascension::Width();
			sCorners[0].m_fY = (fCurY) / Ascension::Height();
#endif
			sCorners[0].m_fTU = fULeft;
			sCorners[0].m_fTV = fVTop;
			sCorners[0].SetColour(iColour);
			sCorners[0].SetSecondColour(iColour2);
			sCorners[0].m_fZ = 0.0f;
			sCorners[0].m_fW = 1.0f;
            sCorners[0].m_fScaleX = Ascension::ScaleX();
            sCorners[0].m_fScaleY = Ascension::ScaleY();

			// top right
#ifdef ASC_IOS
			sCorners[1].m_fX = (fCurX + (FLOAT32)sCharRect.m_iWidth + sCharData.fPixelsAfterChar) / Ascension::Height();
			sCorners[1].m_fY = (fCurY) / Ascension::Width();
#else
			sCorners[1].m_fX = (fCurX + (FLOAT32)sCharRect.m_iWidth + sCharData.fPixelsAfterChar) / Ascension::Width();
			sCorners[1].m_fY = (fCurY) / Ascension::Height();
#endif
			sCorners[1].m_fTU = fURight;
			sCorners[1].m_fTV = fVTop;
			sCorners[1].SetColour(iColour);
			sCorners[1].SetSecondColour(iColour2);
			sCorners[1].m_fZ = 0.0f;
			sCorners[1].m_fW = 1.0f;
            sCorners[1].m_fScaleX = Ascension::ScaleX();
            sCorners[1].m_fScaleY = Ascension::ScaleY();

			// bottom right
#ifdef ASC_IOS
			sCorners[2].m_fX = (fCurX + (FLOAT32)sCharRect.m_iWidth + sCharData.fPixelsAfterChar) / Ascension::Height();
			sCorners[2].m_fY = (fCurY + (FLOAT32)sCharRect.m_iHeight) / Ascension::Width();
#else
			sCorners[2].m_fX = (fCurX + (FLOAT32)sCharRect.m_iWidth + sCharData.fPixelsAfterChar) / Ascension::Width();
			sCorners[2].m_fY = (fCurY + (FLOAT32)sCharRect.m_iHeight) / Ascension::Height();
#endif
			sCorners[2].m_fTU = fURight;
			sCorners[2].m_fTV = fVBottom;
			sCorners[2].SetColour(iColour);
			sCorners[2].SetSecondColour(iColour2);
			sCorners[2].m_fZ = 0.0f;
			sCorners[2].m_fW = 1.0f;
            sCorners[2].m_fScaleX = Ascension::ScaleX();
            sCorners[2].m_fScaleY = Ascension::ScaleY();

			// bottom left
#ifdef ASC_IOS
			sCorners[3].m_fX = (fCurX + sCharData.fPixelsBeforeChar) / Ascension::Height();
			sCorners[3].m_fY = (fCurY + (FLOAT32)sCharRect.m_iHeight) / Ascension::Width();
#else
			sCorners[3].m_fX = (fCurX + sCharData.fPixelsBeforeChar) / Ascension::Width();
			sCorners[3].m_fY = (fCurY + (FLOAT32)sCharRect.m_iHeight) / Ascension::Height();
#endif
			sCorners[3].m_fTU = fULeft;
			sCorners[3].m_fTV = fVBottom;
			sCorners[3].SetColour(iColour);
			sCorners[3].SetSecondColour(iColour2);
			sCorners[3].m_fZ = 0.0f;
			sCorners[3].m_fW = 1.0f;
            sCorners[3].m_fScaleX = Ascension::ScaleX();
            sCorners[3].m_fScaleY = Ascension::ScaleY();
			
			pData[0] = sCorners[0];
			++pData;
			pData[0] = sCorners[1];
			++pData;
			pData[0] = sCorners[2];
			++pData;
			pData[0] = sCorners[0];
			++pData;
			pData[0] = sCorners[2];
			++pData;
			pData[0] = sCorners[3];
			++pData;

			uPrimCount += 2;
			fCurX += sCharData.fPixelsBeforeChar + (FLOAT32)sCharRect.m_iWidth + sCharData.fPixelsAfterChar;
		}
	}
	m_pRenderBuffer->UnLock();
	m_pRenderBuffer->SetSource();
		

	static ASCShader* pShader = Ascension::Renderer().GetShaderManager()->LoadShader("Shaders/SpriteShaders");

	static ASC_Handle m_TechniqueHandle = pShader->GetNamedHandle("Font");

	pShader->SetTechnique(m_TechniqueHandle);
	m_pFontTexture->Apply( 0 );
	if(m_pVertDecl)
	{
		m_pVertDecl->Apply();
	}


	pShader->Begin();

	Ascension::Renderer().DrawPrimitives( PT_TriList, uPrimCount);

	pShader->End();
    }

	return false;
}
SCharacter& 
ASCFont::GetCharacter(CHAR8 cChar)
{
	for(UINT32 i = 0; i < m_iCharacterCount; ++i)
	{
		if(m_pCharacters[i].cCharacter == cChar)
		{
			return m_pCharacters[i];
		}
	}
	//assert_now("Char does not exist in this font");
	return m_pCharacters[0];
}
FLOAT32 
ASCFont::GetStringWidth( ASCString& strMessage )
{
	UINT32 iWidth = 0;

	for( UINT32 uChar = 0; uChar < strMessage.length(); ++uChar )
	{
		SCharacter& CharData = GetCharacter(strMessage[uChar]);
			iWidth += 
				(UINT32)CharData.fPixelsBeforeChar 
				+ CharData.sRect.m_iWidth 
				+ (UINT32)CharData.fPixelsAfterChar;
	}

	return( FLOAT32( iWidth ) );
}
FLOAT32 
ASCFont::Width(ASCString& strString)
{
	UINT32 iWidth = 0;
	UINT32 uNewWidth = 0;

	for( UINT32 uChar = 0; uChar < strString.length(); ++uChar )
	{
		if(strString[uChar] == '\n')
		{
			if(uNewWidth > iWidth)
			{
				iWidth = uNewWidth;
			}
			uNewWidth = 0;
		}
		else
		{
			SCharacter& CharData = GetCharacter(strString[uChar]);
				uNewWidth += 
					(UINT32)CharData.fPixelsBeforeChar 
					+ CharData.sRect.m_iWidth 
					+ (UINT32)CharData.fPixelsAfterChar;
		}
	}
	if(uNewWidth > iWidth)
	{
		iWidth = uNewWidth;
	}

	return( FLOAT32( iWidth ) );
}
FLOAT32 
ASCFont::GetLineWidth( ASCString& strMessage, UINT32 uStart )
{
	UINT32 iWidth = 0;

	for( UINT32 uChar = uStart; uChar < strMessage.length(); ++uChar )
	{
		if(strMessage[uChar] == '\n')
		{
			break;
		}
		SCharacter& CharData = GetCharacter(strMessage[uChar]);
			iWidth += 
				(UINT32)CharData.fPixelsBeforeChar 
				+ CharData.sRect.m_iWidth 
				+ (UINT32)CharData.fPixelsAfterChar;
	}

	return( FLOAT32( iWidth ) );
}
UINT32 
ASCFont::GetHeight()
{
	return m_uHighest;
}
ASCString 
ASCFont::ID()
{
	return m_strID;
}
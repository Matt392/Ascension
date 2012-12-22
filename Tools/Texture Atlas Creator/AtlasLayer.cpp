#include "ASCGenInc.h"
#include "AtlasLayer.h"

#include "Ascension.h"
#include "ASCGUIStaticImage.h"
#include "ASCGUIButton.h"
#include "ASCGUIManager.h"
#include "ASCRenderer.h"
#include "ASCTextureManager.h"
#include "ASCTexture.h"
#include "ASCSprite.h"
#include "ASCFileIO.h"
#include "ASCINIFile.h"

#include <stdio.h>

UINT32 PowerOfTwoRoundUp(UINT32 uVal)
{
	--uVal;
	uVal |= uVal >> 1;
	uVal |= uVal >> 2;
	uVal |= uVal >> 4;
	uVal |= uVal >> 8;
	uVal |= uVal >> 16;
	++uVal;
	return uVal;
}

CAtlasLayer::CAtlasLayer(void)
: m_pLayerTexure ( NULL )
, m_pLayerIcon ( NULL )
, m_bActive ( false )
, m_uCurrentTexture ( 0 )
{
}


CAtlasLayer::~CAtlasLayer(void)
{
	for(UINT32 i = 0; i < m_vecTextures.size(); ++i)
	{
		SafeDelete(m_vecTextures[i]);
	}
}

UINT32 
CAtlasLayer::Initialise(UINT32 uWidth, UINT32 uHeight, UINT32 uLayerNum)
{
	m_uLayerNumber = uLayerNum;
	m_uWidth = uWidth;
	m_uHeight = uHeight;

	ASCString strName = "";
	m_pLayerTexure = Ascension::Renderer().GetTextureManager()->CreateTexture(m_uWidth, m_uHeight, strName);
	m_pLayerSaveTexure = Ascension::Renderer().GetTextureManager()->CreateTexture(m_uWidth, m_uHeight, strName);
	
	UINT32* pTexels = NULL;
	UINT32 uPicth = 0;
	if(m_pLayerTexure->LockTexels(pTexels, uPicth, false))
	{
		CHAR8* pColour = NULL;
		UINT32 uR = 0;
		UINT32 uG = 128;
		UINT32 uB = 0;
		UINT32 uA = 128;
		FLOAT32 fXDegree = 255.0f/m_uWidth;
		FLOAT32 fYDegree = 255.0f/m_uHeight;
		for(UINT32 i = 0; i < m_uWidth; ++i)
		{
			uR = SC_UINT( fXDegree * i );
			for(UINT32 j = 0; j < m_uHeight; ++j)
			{
				uB = SC_UINT( fYDegree * j );
				pColour = reinterpret_cast<CHAR8*>(&pTexels[(m_uWidth*i)+j]);
				pColour[0] = SC_CHAR( uB );
				pColour[1] = SC_CHAR( uG );
				pColour[2] = SC_CHAR( uR );
				pColour[3] = SC_CHAR( uA );
			}
		}

		m_pLayerTexure->UnlockTexels();
	}

	m_pLayerIcon = Ascension::GUI().AddGUIButton(m_pLayerTexure, m_uIconButton);
	m_pLayerIcon->SetPos(74.0f, 100.0f + ((ATLAS_ICON_SIZE+16.0f) * uLayerNum));
	m_pLayerIcon->SetSize(ATLAS_ICON_SIZE, ATLAS_ICON_SIZE);
	m_pLayerIcon->Sprite()->SetDepthLayer(0);

	return m_uIconButton;
}
void 
CAtlasLayer::Initialise()
{
	ASCString strName = "Atlas_Background";
	UINT32* pTexels = NULL;
	UINT32 uPicth = 0;
	m_pDisplayTexture = Ascension::Renderer().GetTextureManager()->CreateTexture(m_uWidth, m_uHeight, strName);
	if(m_pDisplayTexture->LockTexels(pTexels, uPicth, false))
	{
		CHAR8* pColour = NULL;
		UINT32 uR = 0;
		UINT32 uG = 128;
		UINT32 uB = 0;
		UINT32 uA = 128;
		FLOAT32 fXDegree = 255.0f/m_uWidth;
		FLOAT32 fYDegree = 255.0f/m_uHeight;
		for(UINT32 i = 0; i < m_uWidth; ++i)
		{
			uR = SC_UINT( fXDegree * i );
			for(UINT32 j = 0; j < m_uHeight; ++j)
			{
				uB = SC_UINT( fYDegree * j );
				pColour = reinterpret_cast<CHAR8*>(&pTexels[(m_uWidth*i)+j]);
				pColour[0] = SC_CHAR( uB );
				pColour[1] = SC_CHAR( uG );
				pColour[2] = SC_CHAR( uR );
				pColour[3] = SC_CHAR( uA );
			}
		}

		m_pDisplayTexture->UnlockTexels();
	}
	
	m_pEditDisplay = Ascension::GUI().AddGUIStaticImage(m_pDisplayTexture, ATLAS_CENTER_X, ATLAS_CENTER_Y);
	m_pEditDisplay->Sprite()->SetDepthLayer(0);

	m_pFont = Ascension::Renderer().LoadFont("Fonts/CourierNew.fnt");
	
	Ascension::EventSystem().AddEventListener(this, ET_GUI);

	Reset();
	Deactivate();
}
bool 
CAtlasLayer::IsValid()
{
	return m_vecTextures.size() > 0;
}
UINT32 
CAtlasLayer::TextureCount()
{
	return m_vecTextures.size();
}
void 
CAtlasLayer::Reset()
{
	UINT32* pTexels = NULL;
	UINT32 uPicth = 0;
	if(m_pLayerTexure->LockTexels(pTexels, uPicth, false))
	{
		CHAR8* pColour = NULL;
		UINT32 uR = 0;
		UINT32 uG = 128;
		UINT32 uB = 0;
		UINT32 uA = 255;
		FLOAT32 fXDegree = 255.0f/m_uWidth;
		FLOAT32 fYDegree = 255.0f/m_uHeight;
		for(UINT32 i = 0; i < m_uWidth; ++i)
		{
			uR = SC_UINT( fXDegree * i );
			for(UINT32 j = 0; j < m_uHeight; ++j)
			{
				uB = SC_UINT( fYDegree * j );
				pColour = reinterpret_cast<CHAR8*>(&pTexels[(m_uWidth*i)+j]);
				pColour[0] = SC_CHAR( uB );
				pColour[1] = SC_CHAR( uG );
				pColour[2] = SC_CHAR( uR );
				pColour[3] = SC_CHAR( uA );
			}
		}

		m_pLayerTexure->UnlockTexels();
	}

	Deactivate();
	for(UINT32 i = 0; i < m_vecTextures.size(); ++i)
	{
		SafeDelete(m_vecTextures[i]);
	}
	m_vecTextures.Clear();
	m_uCurrentTexture = 0;

	m_pEditDisplay->SetPos(ATLAS_CENTER_X, ATLAS_CENTER_Y);
}
void 
CAtlasLayer::Deactivate()
{
	m_bActive = false;
	m_pEditDisplay->Disable();
	for(UINT32 i = 0; i < m_vecTextures.size(); ++i)
	{
		m_vecTextures[i]->m_pOnScreenImage->Disable();
	}
}
void 
CAtlasLayer::Activate()
{
	m_bActive = true;
	m_pEditDisplay->Enable();
	for(UINT32 i = 0; i < m_vecTextures.size(); ++i)
	{
		m_vecTextures[i]->m_pOnScreenImage->Enable();
	}
}
void 
CAtlasLayer::UpdateCurrentTexture()
{
	if(m_vecTextures[m_uCurrentTexture]->m_strFilePath.length() > 0)
	{
		AtlasTexture* pAtlasTexture = m_vecTextures[m_uCurrentTexture];
		ASCTexture* pOldTexture = m_vecTextures[m_uCurrentTexture]->m_pOnScreenImage->Sprite()->Texture();
		ASCTexture* pTexture = Ascension::Renderer().GetTextureManager()->CreateTexture(pAtlasTexture->m_strFilePath, true);
		pTexture->UnLoad();
		pTexture->Reload();
		
		if(pTexture == pAtlasTexture->m_pTexture)
		{
			UINT32 uPitch = 0;
			UINT32* pTexels = NULL;

			if(pTexture->LockTexels(pTexels, uPitch, false))
			{
				UINT32 uWidth = pTexture->GetWidth();
				UINT32 uHeight = pTexture->GetHeight();

				for(UINT32 i = 0; i < uWidth * uHeight; ++i)
				{
					pAtlasTexture->m_pTextureData[i] = pTexels[i];
				}

				pTexture->UnlockTexels();
			}
		}
		else
		{
			UINT32 uPitch = 0;
			UINT32* pTexels = NULL;

			if(pTexture->LockTexels(pTexels, uPitch, false))
			{
				UINT32 uWidth = pTexture->GetWidth();
				UINT32 uHeight = pTexture->GetHeight();

				for(UINT32 i = 0; i < uWidth * uHeight; ++i)
				{
					pAtlasTexture->m_pTextureData[i] = pTexels[i];
				}

				pTexture->UnlockTexels();
			}

			if(pOldTexture->LockTexels(pTexels, uPitch, false))
			{
				UINT32 uWidth = pOldTexture->GetWidth();
				UINT32 uHeight = pOldTexture->GetHeight();

				for(UINT32 i = 0; i < uWidth * uHeight; ++i)
				{
					pTexels[i] = pAtlasTexture->m_pTextureData[i];
				}

				pOldTexture->UnlockTexels();
			}
			if(pOldTexture->LockTexels(pTexels, uPitch, false))
			{
				UINT32 uWidth = pOldTexture->GetWidth();
				UINT32 uHeight = pOldTexture->GetHeight();

				CHAR8* pColour = NULL;
				UINT32 uR = 0;
				UINT32 uG = 128;
				UINT32 uB = 0;
				UINT32 uA = 128;
		
				FLOAT32 fXDegree = 255.0f/uWidth;
				FLOAT32 fYDegree = 255.0f/uHeight;
		
				CHAR8 cNone = 0;
				unsigned char cFull = 255;

				for(UINT32 i = 0; i < uWidth; ++i)
				{
					uR = SC_UINT( fXDegree * (uWidth - i) );
					for(UINT32 j = 0; j < uHeight; ++j)
					{
						uB = SC_UINT( fYDegree * (uHeight - j) );
						pColour = reinterpret_cast<CHAR8*>(&pTexels[(uWidth*j)+i]);
						if(pColour[3] == cNone)
						{
							if(i == 0 || i == uWidth-1 || j == 0 || j == uHeight-1)
							{
								pColour[0] = SC_CHAR( cNone );
								pColour[1] = SC_CHAR( cNone );
								pColour[2] = SC_CHAR( cNone );
								pColour[3] = SC_CHAR( cFull );
							}
							else
							{
								pColour[0] = SC_CHAR( uB );
								pColour[1] = SC_CHAR( uG );
								pColour[2] = SC_CHAR( uR );
								pColour[3] = SC_CHAR( uA );
							}
						}
					}
				}

				pOldTexture->UnlockTexels();
			}
		}
	}
	CalculateTexture();
}
void 
CAtlasLayer::UpdateAllTextures()
{
	for(UINT32 k = 0; k < m_vecTextures.size(); ++k)
	{
		if(m_vecTextures[k]->m_strFilePath.length() > 0)
		{
			AtlasTexture* pAtlasTexture = m_vecTextures[k];
			ASCTexture* pOldTexture = m_vecTextures[k]->m_pOnScreenImage->Sprite()->Texture();
			ASCTexture* pTexture = Ascension::Renderer().GetTextureManager()->CreateTexture(pAtlasTexture->m_strFilePath, true);
			pTexture->UnLoad();
			pTexture->Reload();
		
			if(pTexture == pAtlasTexture->m_pTexture)
			{
				UINT32 uPitch = 0;
				UINT32* pTexels = NULL;

				if(pTexture->LockTexels(pTexels, uPitch, false))
				{
					UINT32 uWidth = pTexture->GetWidth();
					UINT32 uHeight = pTexture->GetHeight();

					for(UINT32 i = 0; i < uWidth * uHeight; ++i)
					{
						pAtlasTexture->m_pTextureData[i] = pTexels[i];
					}

					pTexture->UnlockTexels();
				}
			}
			else
			{
				UINT32 uPitch = 0;
				UINT32* pTexels = NULL;

				if(pTexture->LockTexels(pTexels, uPitch, false))
				{
					UINT32 uWidth = pTexture->GetWidth();
					UINT32 uHeight = pTexture->GetHeight();

					for(UINT32 i = 0; i < uWidth * uHeight; ++i)
					{
						pAtlasTexture->m_pTextureData[i] = pTexels[i];
					}

					pTexture->UnlockTexels();
				}

				if(pOldTexture->LockTexels(pTexels, uPitch, false))
				{
					UINT32 uWidth = pOldTexture->GetWidth();
					UINT32 uHeight = pOldTexture->GetHeight();

					for(UINT32 i = 0; i < uWidth * uHeight; ++i)
					{
						pTexels[i] = pAtlasTexture->m_pTextureData[i];
					}

					pOldTexture->UnlockTexels();
				}
				if(pOldTexture->LockTexels(pTexels, uPitch, false))
				{
					UINT32 uWidth = pOldTexture->GetWidth();
					UINT32 uHeight = pOldTexture->GetHeight();

					CHAR8* pColour = NULL;
					UINT32 uR = 0;
					UINT32 uG = 128;
					UINT32 uB = 0;
					UINT32 uA = 128;
		
					FLOAT32 fXDegree = 255.0f/uWidth;
					FLOAT32 fYDegree = 255.0f/uHeight;
		
					CHAR8 cNone = 0;
					unsigned char cFull = 255;

					for(UINT32 i = 0; i < uWidth; ++i)
					{
						uR = SC_UINT( fXDegree * (uWidth - i) );
						for(UINT32 j = 0; j < uHeight; ++j)
						{
							uB = SC_UINT( fYDegree * (uHeight - j) );
							pColour = reinterpret_cast<CHAR8*>(&pTexels[(uWidth*j)+i]);
							if(pColour[3] == cNone)
							{
								if(i == 0 || i == uWidth-1 || j == 0 || j == uHeight-1)
								{
									pColour[0] = SC_CHAR( cNone );
									pColour[1] = SC_CHAR( cNone );
									pColour[2] = SC_CHAR( cNone );
									pColour[3] = SC_CHAR( cFull );
								}
								else
								{
									pColour[0] = SC_CHAR( uB );
									pColour[1] = SC_CHAR( uG );
									pColour[2] = SC_CHAR( uR );
									pColour[3] = SC_CHAR( uA );
								}
							}
						}
					}

					pOldTexture->UnlockTexels();
				}
			}
		}
	}
	CalculateTexture();
}
void 
CAtlasLayer::Update()
{
	SFontData sFontData;
	sFontData.bCentered = false;
	sFontData.bRight = false;
	sFontData.fX = TEXTURE_LIST_X;
	sFontData.fY = TEXTURE_LIST_Y;
	sFontData.iColour = 0xFF000000;
	sFontData.strMessage = "Layer ";
	sFontData.strMessage.AddSmallInt(m_uLayerNumber+1);
	sFontData.strMessage += ASCString(" Texture List\n\n");

	for(UINT32 i = 0; i < m_vecTextures.size(); ++i)
	{
		if(i == m_uCurrentTexture)
		{
			sFontData.strMessage += ASCString("*");
		}
		sFontData.strMessage += m_vecTextures[i]->m_strName;
		sFontData.strMessage += ASCString("\n");
	}
	Ascension::Renderer().AddTextLineToFont(m_pFont, sFontData);

	
	sFontData.fX = INFO_ZONE_X;
	sFontData.fY = INFO_ZONE_Y;
	
	if(m_vecTextures.size() > 0)
	{
		ASCSprite* pSprite = m_vecTextures[m_uCurrentTexture]->m_pOnScreenImage->Sprite();
		
		FLOAT32 fPrevScale = m_pEditDisplay->Sprite()->GetScaleX();

		UINT32 uSrcWidth = SC_UINT(pSprite->GetWidth());
		UINT32 uSrcHeight = SC_UINT(pSprite->GetHeight());
		UINT32 uOffsetX = SC_UINT(((pSprite->PosX()) - m_pEditDisplay->Sprite()->PosX()) + (((m_uWidth/2) - (uSrcWidth/2)) * fPrevScale));
		UINT32 uOffsetY = SC_UINT(((pSprite->PosY()) - m_pEditDisplay->Sprite()->PosY()) + (((m_uHeight/2) - (uSrcHeight/2)) * fPrevScale));
		
		sFontData.strMessage = "Current Texture,  X: ";
		sFontData.strMessage.AddSmallInt(uOffsetX);
		
		sFontData.strMessage += ASCString("   Y: ");
		sFontData.strMessage.AddSmallInt(uOffsetY);
	
		Ascension::Renderer().AddTextLineToFont(m_pFont, sFontData);
	}
}
void 
CAtlasLayer::AddTexture(ASCString& strSection, ASCTexture* pSourceTexture, ASCINIFile& rFile)
{
	ASCString strTemp = "";
	AtlasTexture* pNewTexture = new AtlasTexture();
	
	UINT32 uSrcWidth = pSourceTexture->GetWidth();
	UINT32 uSrcHeight = pSourceTexture->GetHeight();
	uSrcHeight;

	UINT32 uOffsetX = rFile.GetUInt(strSection, "TL");
	UINT32 uOffsetY = rFile.GetUInt(strSection, "BL");
	UINT32 uFarX = rFile.GetUInt(strSection, "TR");
	UINT32 uFarY = rFile.GetUInt(strSection, "BR");
	
	UINT32 uWidth = uFarX - uOffsetX;
	UINT32 uHeight = uFarY - uOffsetY;
	pNewTexture->m_pTexture = Ascension::Renderer().GetTextureManager()->CreateTexture(uWidth, uHeight, strTemp);

	static UINT32 suCounter = 0;
	strTemp = "Temp_Img_";
	strTemp.AddSmallInt(suCounter);
	++suCounter;

	pNewTexture->m_pTexture->Save(strTemp);
	pNewTexture->m_pTexture->UnLoad();
	strTemp += ASCString(".png");
	pNewTexture->m_pTexture = Ascension::Renderer().GetTextureManager()->CreateTexture(strTemp, true);
	
	remove(strTemp.c_str());
	
	UINT32* pTexels = NULL;
	UINT32 uPicth = 0;
	UINT32* pSrcTexels = NULL;
	UINT32 uSrcPicth = 0;
	if(pNewTexture->m_pTexture->LockTexels(pTexels, uPicth, false) && pSourceTexture->LockTexels(pSrcTexels, uSrcPicth, false))
	{
		for(UINT32 x = 0; x < uWidth; ++x)
		{
			for(UINT32 y = 0; y < uHeight; ++y)
			{
				pTexels[(uWidth*y)+x] = pSrcTexels[(uSrcWidth*(uOffsetY+y))+x+uOffsetX];
			}
		}

		pNewTexture->m_pTexture->UnlockTexels();
		pSourceTexture->UnlockTexels();
	}



	pNewTexture->m_pOnScreenImage = Ascension::GUI().AddGUIButton(pNewTexture->m_pTexture, pNewTexture->m_uGUIID);
	
	FLOAT32 fPosX = m_pEditDisplay->Sprite()->PosX() + (SC_FLOAT(uOffsetX + (uWidth/2)) - SC_FLOAT(m_uWidth/2));
	FLOAT32 fPosY = m_pEditDisplay->Sprite()->PosY() + (SC_FLOAT(uOffsetY + (uHeight/2)) - SC_FLOAT(m_uHeight/2));


	pNewTexture->m_pOnScreenImage->SetPos( fPosX, fPosY );

	pNewTexture->m_strFilePath = rFile.GetString(strSection, "SourcePath");
	
	pNewTexture->m_strName = rFile.GetString(strSection, "Name");

	if(pNewTexture->m_pTexture->LockTexels(pTexels, uPicth, false))
	{
		UINT32 uWidth = pNewTexture->m_pTexture->GetWidth();
		UINT32 uHeight = pNewTexture->m_pTexture->GetHeight();

		pNewTexture->m_pTextureData = new UINT32[uWidth*uHeight];

		CHAR8* pColour = NULL;
		UINT32 uR = 0;
		UINT32 uG = 128;
		UINT32 uB = 0;
		UINT32 uA = 128;
		
		FLOAT32 fXDegree = 255.0f/uWidth;
		FLOAT32 fYDegree = 255.0f/uHeight;
		
		CHAR8 cNone = 0;
		unsigned char cFull = 255;

		for(UINT32 i = 0; i < uWidth; ++i)
		{
			uR = SC_UINT( fXDegree * (uWidth - i) );
			for(UINT32 j = 0; j < uHeight; ++j)
			{
				pNewTexture->m_pTextureData[(uWidth*j)+i] = pTexels[(uWidth*j)+i];
				uB = SC_UINT( fYDegree * (uHeight - j) );
				pColour = reinterpret_cast<CHAR8*>(&pTexels[(uWidth*j)+i]);
				if(pColour[3] == cNone)
				{
					if(i == 0 || i == uWidth-1 || j == 0 || j == uHeight-1)
					{
						pColour[0] = SC_CHAR( cNone );
						pColour[1] = SC_CHAR( cNone );
						pColour[2] = SC_CHAR( cNone );
						pColour[3] = SC_CHAR( cFull );
					}
					else
					{
						pColour[0] = SC_CHAR( uB );
						pColour[1] = SC_CHAR( uG );
						pColour[2] = SC_CHAR( uR );
						pColour[3] = SC_CHAR( uA );
					}
				}
			}
		}

		pNewTexture->m_pTexture->UnlockTexels();
	}
	
	m_vecTextures.push_back(pNewTexture);
	CalculateTexture();
}
void 
CAtlasLayer::AddTexture(ASCString& strTexture)
{
	ASCString strTemp;
	AtlasTexture* pNewTexture = new AtlasTexture();

	pNewTexture->m_pTexture = Ascension::Renderer().GetTextureManager()->CreateTexture(strTexture, true);

	pNewTexture->m_pOnScreenImage = Ascension::GUI().AddGUIButton(pNewTexture->m_pTexture, pNewTexture->m_uGUIID);
	pNewTexture->m_pOnScreenImage->SetPos( ATLAS_CENTER_X, ATLAS_CENTER_Y );

	pNewTexture->m_strFilePath = strTexture;
	strTexture.substrc('\\', strTemp, 1);
	pNewTexture->m_strName = strTexture;
	while(strTemp.length() < pNewTexture->m_strName.length())
	{
		pNewTexture->m_strName = strTemp;
		pNewTexture->m_strName.substrc('\\', strTemp, 1);
	}
	strTemp.prestrc('.', pNewTexture->m_strName);

	UINT32* pTexels = NULL;
	UINT32 uPicth = 0;
	if(pNewTexture->m_pTexture->LockTexels(pTexels, uPicth, false))
	{
		UINT32 uWidth = pNewTexture->m_pTexture->GetWidth();
		UINT32 uHeight = pNewTexture->m_pTexture->GetHeight();

		pNewTexture->m_pTextureData = new UINT32[uWidth*uHeight];

		CHAR8* pColour = NULL;
		UINT32 uR = 0;
		UINT32 uG = 128;
		UINT32 uB = 0;
		UINT32 uA = 128;
		
		FLOAT32 fXDegree = 255.0f/uWidth;
		FLOAT32 fYDegree = 255.0f/uHeight;
		
		CHAR8 cNone = 0;
		unsigned char cFull = 255;

		for(UINT32 i = 0; i < uWidth; ++i)
		{
			uR = SC_UINT( fXDegree * (uWidth - i) );
			for(UINT32 j = 0; j < uHeight; ++j)
			{
				pNewTexture->m_pTextureData[(uWidth*j)+i] = pTexels[(uWidth*j)+i];
				uB = SC_UINT( fYDegree * (uHeight - j) );
				pColour = reinterpret_cast<CHAR8*>(&pTexels[(uWidth*j)+i]);
				if(pColour[3] == cNone)
				{
					if(i == 0 || i == uWidth-1 || j == 0 || j == uHeight-1)
					{
						pColour[0] = SC_CHAR( cNone );
						pColour[1] = SC_CHAR( cNone );
						pColour[2] = SC_CHAR( cNone );
						pColour[3] = SC_CHAR( cFull );
					}
					else
					{
						pColour[0] = SC_CHAR( uB );
						pColour[1] = SC_CHAR( uG );
						pColour[2] = SC_CHAR( uR );
						pColour[3] = SC_CHAR( uA );
					}
				}
			}
		}

		pNewTexture->m_pTexture->UnlockTexels();
	}
	
	m_vecTextures.push_back(pNewTexture);
	CalculateTexture();
}
void 
CAtlasLayer::RemoveCurrentTexture()
{
	if(m_uCurrentTexture < m_vecTextures.size() )
	{
		m_vecTextures[m_uCurrentTexture]->m_pOnScreenImage->Disable();
		SafeDelete(m_vecTextures[m_uCurrentTexture]);
		m_vecTextures.remove(m_uCurrentTexture);
		CalculateTexture();
	}
}
void 
CAtlasLayer::MoveCurrentTexture(FLOAT32 fX, FLOAT32 fY)
{
	if(m_uCurrentTexture < m_vecTextures.size() && fX < 200.0f && fY < 200.0f)
	{
		m_vecTextures[m_uCurrentTexture]->m_pOnScreenImage->IncPosition(fX, fY);
		CalculateTexture();
	}
}
void 
CAtlasLayer::Zoom(FLOAT32 fVal)
{
	FLOAT32 fPrevScale = m_pEditDisplay->Sprite()->GetScaleX();
	m_pEditDisplay->Sprite()->SetScale(fVal, fVal);
	
	
	FLOAT32 fCenterX = m_pEditDisplay->Sprite()->PosX();
	FLOAT32 fCenterY = m_pEditDisplay->Sprite()->PosY();

	FLOAT32 fPosX = 0.0f;
	FLOAT32 fPosY = 0.0f;

	for(UINT32 i = 0; i < m_vecTextures.size(); ++i)
	{
		fPosX = fCenterX + (((fCenterX - m_vecTextures[i]->m_pOnScreenImage->Sprite()->PosX()) / fPrevScale) * fVal);
		fPosY = fCenterY + (((fCenterY - m_vecTextures[i]->m_pOnScreenImage->Sprite()->PosY()) / fPrevScale) * fVal);
		m_vecTextures[i]->m_pOnScreenImage->Sprite()->SetScale(fVal, fVal);
		m_vecTextures[i]->m_pOnScreenImage->Sprite()->SetPosition(fPosX, fPosY);
	}
}
void 
CAtlasLayer::PanAtlas(FLOAT32 fX, FLOAT32 fY)
{
	if(fX < 200.0f && fY < 200.0f)
	{
		m_pEditDisplay->IncPosition(fX, fY);

		for(UINT32 i = 0; i < m_vecTextures.size(); ++i)
		{
			m_vecTextures[i]->m_pOnScreenImage->IncPosition(fX, fY);
		}
		//CalculateTexture();
	}
}
void 
CAtlasLayer::CalculateTexture()
{
	UINT32* pDisplayTexels;
	UINT32* pSaveTexels;
	UINT32 uPitch;

	if(m_pLayerTexure->LockTexels(pDisplayTexels, uPitch, false) && m_pLayerSaveTexure->LockTexels(pSaveTexels, uPitch, false))
	{
		CHAR8* pColour = NULL;
		UINT32 uR = 0;
		UINT32 uG = 128;
		UINT32 uB = 0;
		UINT32 uA = 255;
		CHAR8 cNone = 0;

		FLOAT32 fXDegree = 255.0f/m_uWidth;
		FLOAT32 fYDegree = 255.0f/m_uHeight;

		for(UINT32 i = 0; i < m_uWidth; ++i)
		{
			uR = SC_UINT( fXDegree * i );
			for(UINT32 j = 0; j < m_uHeight; ++j)
			{
				uB = SC_UINT( fYDegree * j );
				pColour = reinterpret_cast<CHAR8*>(&pDisplayTexels[(m_uWidth*i)+j]);
				pColour[0] = SC_CHAR( uB );
				pColour[1] = SC_CHAR( uG );
				pColour[2] = SC_CHAR( uR );
				pColour[3] = SC_CHAR( uA );

				pColour = reinterpret_cast<CHAR8*>(&pSaveTexels[(m_uWidth*i)+j]);
				pColour[0] = SC_CHAR( cNone );
				pColour[1] = SC_CHAR( cNone );
				pColour[2] = SC_CHAR( cNone );
				pColour[3] = SC_CHAR( cNone );
			}
		}
		
		for(UINT32 i = 0; i < m_vecTextures.size(); ++i)
		{
			UINT32* pSourceTexels = m_vecTextures[i]->m_pTextureData;

			UINT32 uSrcWidth = m_vecTextures[i]->m_pTexture->GetWidth();
			UINT32 uSrcHeight = m_vecTextures[i]->m_pTexture->GetHeight();
			UINT32 uOffsetX = SC_UINT(m_vecTextures[i]->m_pOnScreenImage->Sprite()->PosX() - m_pEditDisplay->Sprite()->PosX()) + (m_uWidth/2) - (uSrcWidth/2);
			UINT32 uOffsetY = SC_UINT(m_vecTextures[i]->m_pOnScreenImage->Sprite()->PosY() - m_pEditDisplay->Sprite()->PosY()) + (m_uHeight/2) - (uSrcHeight/2);
			
			for ( UINT32 y = uOffsetY; y < m_uHeight && y < uOffsetY + uSrcHeight; y++ )
			{
				for ( UINT32 x = uOffsetX; x < m_uWidth && x < uOffsetX + uSrcWidth; x++ )
				{
					pDisplayTexels[(m_uWidth*y)+x] = pSourceTexels[(uSrcWidth*(y-uOffsetY))+(x-uOffsetX)];
					pSaveTexels[(m_uWidth*y)+x] = pSourceTexels[(uSrcWidth*(y-uOffsetY))+(x-uOffsetX)];
				}
			}
		}

		m_pLayerSaveTexure->UnlockTexels();
		m_pLayerTexure->UnlockTexels();
	}
	else
	{
		assert_now("Guts, Failed to lock display and save texture");
	}
}
void 
CAtlasLayer::Save(ASCFileIO& rGameFile, ASCFileIO& rEditFile, UINT32& ruTextureCount, UINT32 uAtlasNum, ASCString& strAtlasName)
{
	ASCString strTemp = "";
	for(UINT32 i = 0; i < m_vecTextures.size(); ++i)
	{
		strTemp = "<Texture_";
		strTemp.AddSmallInt(ruTextureCount);
		strTemp += ASCString(">");
		
		rGameFile.PrintLine(strTemp);
		rEditFile.PrintLine(strTemp);
		
		strTemp = "	<Name>";
		strTemp += m_vecTextures[i]->m_strName;
		strTemp += ASCString("</Name>");
		
		rGameFile.PrintLine(strTemp);
		rEditFile.PrintLine(strTemp);
		
		strTemp = "	<SourcePath>";
		strTemp += m_vecTextures[i]->m_strFilePath;
		strTemp += ASCString("</SourcePath>");
		
		rEditFile.PrintLine(strTemp);
		
		strTemp = "	<Texture>";
		strTemp.AddSmallInt(uAtlasNum);
		strTemp += ASCString("</Texture>");
		
		rGameFile.PrintLine(strTemp);
		rEditFile.PrintLine(strTemp);

		UINT32 uSrcWidth = SC_UINT(m_vecTextures[i]->m_pOnScreenImage->Sprite()->GetWidth());
		UINT32 uSrcHeight = SC_UINT(m_vecTextures[i]->m_pOnScreenImage->Sprite()->GetHeight());

		UINT32 uOffsetX = SC_UINT((m_vecTextures[i]->m_pOnScreenImage->Sprite()->PosX()) - m_pEditDisplay->Sprite()->PosX()) + (m_uWidth/2) - (uSrcWidth/2);
		UINT32 uOffsetY = SC_UINT((m_vecTextures[i]->m_pOnScreenImage->Sprite()->PosY()) - m_pEditDisplay->Sprite()->PosY()) + (m_uHeight/2) - (uSrcHeight/2);
				
		ASCString strTextureInfo;
		UINT32 uFarX = (uOffsetX+uSrcWidth);
		UINT32 uFarY = (uOffsetY+uSrcHeight);
				
		strTemp = "	<TL>";
		strTemp += uOffsetX;
		strTemp += ASCString("</TL>");

		rGameFile.PrintLine(strTemp);
		rEditFile.PrintLine(strTemp);
				
		strTemp = "	<TR>";
		strTemp += uFarX;
		strTemp += ASCString("</TR>");

		rGameFile.PrintLine(strTemp);
		rEditFile.PrintLine(strTemp);
				
		strTemp = "	<BL>";
		strTemp += uOffsetY;
		strTemp += ASCString("</BL>");

		rGameFile.PrintLine(strTemp);
		rEditFile.PrintLine(strTemp);
				
		strTemp = "	<BR>";
		strTemp += uFarY;
		strTemp += ASCString("</BR>");

		rGameFile.PrintLine(strTemp);
		rEditFile.PrintLine(strTemp);

		
		strTemp = "</Texture_";
		strTemp.AddSmallInt(ruTextureCount);
		strTemp += ASCString(">");
		
		rGameFile.PrintLine(strTemp);
		rEditFile.PrintLine(strTemp);

		++ruTextureCount;
	}
	ASCString strSaveName = strAtlasName;
	strSaveName += ASCString("_");
	strSaveName.AddSmallInt(uAtlasNum);

	m_pLayerSaveTexure->Save(strSaveName);
}
void 
CAtlasLayer::OnEvent(ASCIEvent* pEvent)
{
	if(m_bActive)
	{
		switch(pEvent->TypeID())
		{
		case ET_GUI:
			{
				UINT32 uEventID = pEvent->ID();
				for(UINT32 i = 0; i < m_vecTextures.size(); ++i)
				{
					if(uEventID == m_vecTextures[i]->m_uGUIID)
					{
						m_uCurrentTexture = i;
						break;
					}
				}
				break;
			}
		default:break;
		}
	}
}

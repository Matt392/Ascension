#include "ASCGenInc.h"
#include "MenuBackground.h"

#include "Ascension.h"
#include "ASCGUIManager.h"
#include "ASCGUIStaticImage.h"
#include "ASCSprite.h"
#include "ASCTextureManager.h"
#include "ASCTexture.h"
#include "ASCRenderer.h"
#include "ASCFolder.h"

#include "GameOfDeathInc.h"
#include "GameOfLifePattern.h"

CMenuBackground::CMenuBackground(void)
: m_pGameOfLfie ( NULL )
, m_fNewPatternTimer ( MENU_BACKGROUND_NEW_PATTERN )
, m_fResetTimer ( 0.0f )
{
}


CMenuBackground::~CMenuBackground(void)
{
	SafeDelete( m_pGameOfLfie );
	for(UINT32 i = 0; i < m_vecPatterns.size(); ++i)
	{
		SafeDelete(m_vecPatterns[i]);
	}
}

bool 
CMenuBackground::Initialise()
{
	m_pGameOfLfie = new CGameOfLifePattern();
	m_pGameOfLfie->InitialiseNULL(FIELD_TEXTURE_WIDTH, FIELD_TEXTURE_HEIGHT, 0xFF777777, 0xFF000000, MENU_BACKGROUND_UPDATE);

	
	ASCString strTemp = "Menu Background";
	m_pGUITexture = Ascension::Renderer().GetTextureManager()->CreateTexture(FIELD_TEXTURE_WIDTH, FIELD_TEXTURE_HEIGHT, strTemp);
	
	UINT32* pTexels;
	UINT32 uPitch;

	if(m_pGUITexture->LockTexels(pTexels, uPitch, false))
	{
		for(UINT32 y = 0; y < FIELD_TEXTURE_HEIGHT; ++y)
		{
			for(UINT32 x = 0; x < FIELD_TEXTURE_WIDTH; ++x)
			{
				pTexels[(uPitch*y)+x] = 0XFF000000;
			}
		}
		m_pGUITexture->UnlockTexels();
	}
	
	m_pGUIImage = Ascension::GUI().AddGUIStaticImage(m_pGUITexture, Ascension::Width()*0.5f, Ascension::Height()*0.5f, "GUI_No_Blend");
	m_pGUIImage->Sprite()->SetSize(Ascension::Width()*1.0f, Ascension::Height()*1.0f);

	ASCFolder Folder;
	if(Folder.Load("Templates/Patterns/Menu Background"))
	{
		ASCVector<ASCString>& rFiles = *Folder.Files();
		
		for(UINT32 i = 0; i < rFiles.size(); ++i)
		{
			CGameOfLifePattern* pPattern = new CGameOfLifePattern();
			if(pPattern->Initialise(rFiles[i], 0xFFFFFFFF, 0xFF000000, 0.25f))
			{
				m_vecPatterns.push_back(pPattern);
			}
			else
			{
				SafeDelete( pPattern );
			}
		}
	}

	return true;
}

bool 
CMenuBackground::Activate()
{
	m_pGUIImage->Enable();
	return true;
}
bool 
CMenuBackground::Deactivate()
{
	m_pGUIImage->Disable();
	return true;
}

bool 
CMenuBackground::Update()
{
	bool bForceUpdate = false;

	m_fResetTimer += Ascension::Delta();
	if(m_fResetTimer >= MENU_BACKGROUND_RESET)
	{
		m_fResetTimer = 0.0f;
		m_fNewPatternTimer = MENU_BACKGROUND_NEW_PATTERN;
		m_pGameOfLfie->Reset();
	}
	m_fNewPatternTimer += Ascension::Delta();
	if(m_fNewPatternTimer >= MENU_BACKGROUND_NEW_PATTERN)
	{
		m_fNewPatternTimer -= MENU_BACKGROUND_NEW_PATTERN;
		if(m_vecPatterns.size() > 0)
		{
			UINT32 uIndex = ASCRandNumGen::Rand(m_vecPatterns.size());

			SCoordinates sCoords;
			sCoords.m_sX = ASCRandNumGen::Rand(SC_UINT( FIELD_TEXTURE_WIDTH ));
			sCoords.m_sY = ASCRandNumGen::Rand(SC_UINT( FIELD_TEXTURE_HEIGHT ));

			m_pGameOfLfie->AddInto(m_vecPatterns[uIndex]->Pattern(), m_vecPatterns[uIndex]->Width(), m_vecPatterns[uIndex]->Height(), sCoords);

			bForceUpdate = true;
		}
	}

	if(m_pGameOfLfie->Update() || bForceUpdate)
	{
		UINT32* pTexels;
		UINT32 uPitch;

		if(m_pGUITexture->LockTexels(pTexels, uPitch, false))
		{
			m_pGameOfLfie->DrawInto(pTexels, uPitch, FIELD_TEXTURE_WIDTH, FIELD_TEXTURE_HEIGHT, SCoordinates(0, 0));

			m_pGUITexture->UnlockTexels();
		}
	}
	return true;
}

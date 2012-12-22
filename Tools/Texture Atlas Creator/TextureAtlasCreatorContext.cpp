#include "ASCGenInc.h"
#include "TextureAtlasCreatorContext.h"

#include "Ascension.h"
#include "ASCGUIManager.h"
#include "ASCGUIStaticImage.h"
#include "ASCGUIButton.h"

#include "ASCInputDevice.h"
#include "ASCInputEvent.h"

#include "ASCRenderer.h"
#include "ASCTextureManager.h"
#include "ASCTexture.h"
#include "ASCSprite.h"
#include "ASCSpriteManager.h"
#include "ASCSpriteVertex.h"

#include "ASCFileIO.h"
#include "ASCINIFile.h"

#include "AtlasLayer.h"

#include "resource.h"
#include <Windows.h>

bool g_bContinue = false;

BOOL CALLBACK ContinueYN(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

CTextureAtlasCreatorContext::CTextureAtlasCreatorContext(void)
: m_bQuit ( false )
, m_bMouseDown ( false )
, m_bMouseRightDown ( false )
, m_pMainButtonBanner ( NULL )
, m_pInfoZoneBanner ( NULL )
, m_fMouseScrollWheel ( 1.0f )
{
}


CTextureAtlasCreatorContext::~CTextureAtlasCreatorContext(void)
{
	for(UINT32 i = 0; i < ALTAS_MAX_LAYERS; ++i)
	{
		SafeDelete( m_pAtlasLayers[i] );
	}
}

//Creation
bool 
CTextureAtlasCreatorContext::Initialise()
{
	Ascension::SpriteManager().SetRenderOrderMode(ESROM_PRE_POST_BACKWARD);

	ASCString strName = "Background";
	UINT32 uBGWidth = 128;
	UINT32 uBGHeight = 128;
	UINT32 uBGColour1 = 0xFFFFFFFF;
	UINT32 uBGColour2 = 0xFFAAAAAA;
	ASCTexture* pBackgroundTexture = Ascension::Renderer().GetTextureManager()->CreateTexture(uBGWidth, uBGHeight, strName);
	UINT32* pTexels = NULL;
	UINT32 uPicth = 0;
	if(pBackgroundTexture->LockTexels(pTexels, uPicth, false))
	{
		for(UINT32 i = 0; i < uBGWidth; ++i)
		{
			if(i % 2 == 0)
			{
				for(UINT32 j = 0; j < uBGHeight; ++j)
				{
					if(j % 2 == 0)
					{
						pTexels[(uBGWidth*i)+j] = uBGColour1;
					}
					else
					{
						pTexels[(uBGWidth*i)+j] = uBGColour2;
					}
				}
			}
			else
			{
				for(UINT32 j = 0; j < uBGHeight; ++j)
				{
					if(j % 2 == 0)
					{
						pTexels[(uBGWidth*i)+j] = uBGColour2;
					}
					else
					{
						pTexels[(uBGWidth*i)+j] = uBGColour1;
					}
				}
			}
		}

		pBackgroundTexture->UnlockTexels();
	}

	const CHAR8* strNames[EMB_COUNT] = { "Sprites/New.ascspr", "Sprites/Load.ascspr", "Sprites/Save.ascspr", "Sprites/Add.ascspr", "Sprites/Remove.ascspr" };

	for(UINT32 i = 0; i < EMB_COUNT; ++i)
	{
		m_pMainButtonName[i] = Ascension::GUI().AddGUIStaticImage(strNames[i], 0.0f, 0.0f, "GUI_No_Blend");
	}
	
	for(UINT32 i = 0; i < ALTAS_MAX_LAYERS; ++i)
	{
		m_pAtlasLayers[i] = new CAtlasLayer();
		m_uLayerButtons[i] = m_pAtlasLayers[i]->Initialise(ATLAS_SIZE, ATLAS_SIZE, i);
	}

	m_pTextureListZone = Ascension::GUI().AddGUIStaticImage("Sprites/Texture_List_Zone.ascspr", 0.0f, 0.0f);
	m_pTextureListZone->SetPos(1280.0f-m_pTextureListZone->HalfWidth(), 512.0f);

	m_pInfoZoneBanner = Ascension::GUI().AddGUIStaticImage("Sprites/Top_Banner.ascspr", 0.0f, 0.0f);
	m_pInfoZoneBanner->SetPos(640.0f, m_pInfoZoneBanner->HalfHeight() + m_pInfoZoneBanner->Height());

	m_pLayerZone = Ascension::GUI().AddGUIStaticImage("Sprites/Layer_Zone.ascspr", 0.0f, 0.0f);
	m_pLayerZone->SetPos(m_pLayerZone->HalfWidth(), 512.0f);

	m_pMainButtonBanner = Ascension::GUI().AddGUIStaticImage("Sprites/Top_Banner.ascspr", 0.0f, 0.0f);
	m_pMainButtonBanner->SetPos(640.0f, m_pMainButtonBanner->HalfHeight());

	m_pMessageZoneBanner = Ascension::GUI().AddGUIStaticImage("Sprites/Bottom_Banner.ascspr", 0.0f, 0.0f);
	m_pMessageZoneBanner->SetPos(640.0f, 1024.0f-m_pInfoZoneBanner->HalfHeight());

	//const CHAR8* strNames[EMB_COUNT] = { "Sprites/New.ascspr", "Sprites/Load.ascspr", "Sprites/Save.ascspr", "Sprites/Add.ascspr", "Sprites/Remove.ascspr" };

	for(UINT32 i = 0; i < EMB_COUNT; ++i)
	{
		//m_pMainButtonName[i] = Ascension::GUI().AddGUIStaticImage(strNames[i], 0.0f, 0.0f, "GUI_No_Blend");

		m_pMainButtons[i] = Ascension::GUI().AddGUIButton("Sprites/Banner_Button_Norm.ascspr", "Banner_Button_Over", "Banner_Button_Down", 0.0f, 0.0f, m_uMainButtons[i]);
		m_pMainButtons[i]->SetPos((m_pMainButtons[i]->Width() * i) + m_pMainButtons[i]->HalfWidth(), m_pMainButtonBanner->HalfHeight());
		m_pMainButtonName[i]->SetPos((m_pMainButtons[i]->Width() * i) + m_pMainButtons[i]->HalfWidth(), m_pMainButtonBanner->HalfHeight());
	}

	const CHAR8* strBigNames[EBB_COUNT] = { "Sprites/Update All.ascspr", "Sprites/Update.ascspr" };

	for(UINT32 i = 0; i < EBB_COUNT; ++i)
	{
		m_pBigButtons[i] = Ascension::GUI().AddGUIButton("Sprites/Big_Button_Norm.ascspr", "Big_Button_Over", "Big_Button_Down", 0.0f, 0.0f, m_uBigButtons[i]);
		m_pBigButtons[i]->SetPos(1280.0f-m_pBigButtons[i]->HalfWidth(), 1024.0f - m_pMessageZoneBanner->Height() - m_pBigButtons[i]->HalfHeight() - (m_pBigButtons[i]->Height() * i));

		m_pBigButtonName[i] = Ascension::GUI().AddGUIStaticImage(strBigNames[i], 0.0f, 0.0f, "GUI_No_Blend");
		m_pBigButtonName[i]->SetPos(1280.0f-m_pBigButtons[i]->HalfWidth(), 1024.0f - m_pMessageZoneBanner->Height() - m_pBigButtons[i]->HalfHeight() - (m_pBigButtons[i]->Height() * i));
	}
	
	for(UINT32 i = 0; i < ALTAS_MAX_LAYERS; ++i)
	{
		m_pAtlasLayers[i]->Initialise();
	}

	m_pBackground = Ascension::GUI().AddGUIStaticImage(pBackgroundTexture, 640.0f, 512.0f, "GUI_No_Blend");
	m_pBackground->Sprite()->SetSize(1280.0f, 1280.0f);


	New(false);

	ASCVector<ASCSpriteVertex*>& rSpriteVertexesRenderOrderPre = Ascension::SpriteManager().SpriteVerticiesRenderOrderPre();
	ASCVector<ASCSpriteVertex*>& rSpriteVertexesRenderOrder = Ascension::SpriteManager().SpriteVerticiesRenderOrder();
	ASCVector<ASCSpriteVertex*>& rSpriteVertexesRenderOrderPost = Ascension::SpriteManager().SpriteVerticiesRenderOrderPost();

	rSpriteVertexesRenderOrderPre.Clear();
	rSpriteVertexesRenderOrderPost.Clear();

	for(UINT32 i = 0; i < rSpriteVertexesRenderOrder.size(); ++i)
	{
		if(rSpriteVertexesRenderOrder[i]->TextureID() == "Background" || rSpriteVertexesRenderOrder[i]->TextureID() == "Atlas_Background")
		{
			rSpriteVertexesRenderOrderPre.push_back(rSpriteVertexesRenderOrder[i]);
		}
		else
		{
			rSpriteVertexesRenderOrderPost.push_back(rSpriteVertexesRenderOrder[i]);
		}
	}
	rSpriteVertexesRenderOrder.Clear();
	
	
	Ascension::Mouse().RegisterInput(m_uMouseLeftButton, "MouseLButton");
	Ascension::Mouse().RegisterInput(m_uMouseRightButton, "MouseRButton");
	Ascension::Mouse().RegisterInput(m_uMouseXY, "MouseXY");
	Ascension::Mouse().RegisterInput(m_uSroll, "MouseWheel");

	Ascension::EventSystem().AddEventListener(this, ET_GUI);
	Ascension::EventSystem().AddEventListener(this, ET_INPUT);

	return true;
}
		
//Game logic
bool 
CTextureAtlasCreatorContext::Update()
{
	m_pAtlasLayers[m_uActiveLayer]->Update();

	return m_bQuit == false;
}

//Optional
void 
CTextureAtlasCreatorContext::Draw()
{
	//do nothing
}
void
CTextureAtlasCreatorContext::Disable()
{
	//do nothing
}
bool 
CTextureAtlasCreatorContext::New(bool bValidCheck)
{
	g_bContinue = true;
	if(bValidCheck)
	{
		for(UINT32 i = 0; i < ALTAS_MAX_LAYERS; ++i)
		{
			if(m_pAtlasLayers[i]->IsValid())
			{
				DialogBox(NULL, MAKEINTRESOURCE( IDD_CONTINUE_Y_N ), reinterpret_cast<HWND>(Ascension::Renderer().GetWindowHandle()), ContinueYN);
				break;
			}
		}
	}
	if(g_bContinue)
	{
		for(UINT32 i = 0; i < ALTAS_MAX_LAYERS; ++i)
		{
			m_pAtlasLayers[i]->Reset();
		}
		m_uActiveLayer = 0;
		m_pAtlasLayers[m_uActiveLayer]->Activate();
		return true;
	}
	return false;
}
void 
CTextureAtlasCreatorContext::AddTexture(ASCString strFile)
{
	ASCString strExtension = "";
	strFile.substrc('.', strExtension);
	if(strExtension == ".png")
	{
		m_pAtlasLayers[m_uActiveLayer]->AddTexture(strFile);
	}
}
void 
CTextureAtlasCreatorContext::LoadTexture()
{
	CHAR8 strOpenName[512] = "";
	OPENFILENAMEA FileName;
	memset(&FileName, 0, sizeof(OPENFILENAMEA));
	FileName.lStructSize = sizeof(OPENFILENAMEA);
	FileName.hwndOwner = reinterpret_cast<HWND>(Ascension::Renderer().GetWindowHandle());
	FileName.hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(NULL));
	FileName.lpstrFilter = NULL;
	FileName.lpstrCustomFilter = NULL;
	FileName.nMaxCustFilter = NULL;
	FileName.lpstrFilter = "PNG Files\0*.png*\0\0";
	FileName.nFilterIndex = 1;
	FileName.lpstrFile = strOpenName;
	FileName.nMaxFile = 512;
	FileName.lpstrFileTitle = NULL;
	FileName.lpstrTitle = "Open File";
	FileName.Flags = OFN_EXPLORER;

	if(TRUE == GetOpenFileNameA(&FileName))
	{
		AddTexture(strOpenName);
	}
}
void 
CTextureAtlasCreatorContext::RemoveTexture()
{
	m_pAtlasLayers[m_uActiveLayer]->RemoveCurrentTexture();
}
void 
CTextureAtlasCreatorContext::SaveAtlas(ASCString strFile)
{
	ASCString strTemp;
	ASCString strAtlasName;
	ASCString strAtlasFileName;
	ASCString strAtlasEditFileName;
	ASCString strTextureName;
	strFile.substrc('\\', strTemp, 1);
	strAtlasName = strFile;
	while(strTemp.length() < strAtlasName.length())
	{
		strAtlasName = strTemp;
		strAtlasName.substrc('\\', strTemp, 1);
	}
	strTemp.prestrc('.', strAtlasName);
	strFile.prestrc('.', strAtlasFileName);
	strFile.prestrc('.', strAtlasEditFileName);
	strFile.prestrc('.', strTextureName);
	strAtlasFileName += ASCString(".ascatl");
	strAtlasEditFileName += ASCString(".ascatledt");
	
	ASCFileIO GameFile;
	ASCFileIO EditFile;
	if(GameFile.Open(strAtlasFileName, IO_OUT) && EditFile.Open(strAtlasEditFileName, IO_OUT))
	{
		GameFile.Clear();
		EditFile.Clear();
		UINT32 uValidCount = 0;
		UINT32 uTextureCount = 0;
		
		GameFile.PrintLine("<General>");
		EditFile.PrintLine("<General>");

		for(UINT32 i = 0; i < ALTAS_MAX_LAYERS; ++i)
		{
			if(m_pAtlasLayers[i]->IsValid())
			{
				uTextureCount += m_pAtlasLayers[i]->TextureCount();
				++uValidCount;
			}
		}
		strTemp = "	<TextureCount>";
		strTemp.AddSmallInt(uValidCount);
		strTemp += ASCString("</TextureCount>");
		
		GameFile.PrintLine(strTemp);
		EditFile.PrintLine(strTemp);

		uValidCount = 0;
		for(UINT32 i = 0; i < ALTAS_MAX_LAYERS; ++i)
		{
			if(m_pAtlasLayers[i]->IsValid())
			{
				strTemp = "	<Texture_";
				strTemp.AddSmallInt(uValidCount);
				strTemp += ASCString(">");
				strTemp += strAtlasName;
				strTemp += ASCString("_");
				strTemp.AddSmallInt(uValidCount);
				strTemp += ASCString(".png</Texture_");
				strTemp.AddSmallInt(uValidCount);
				strTemp += ASCString(">");
		
				GameFile.PrintLine(strTemp);
				EditFile.PrintLine(strTemp);

				++uValidCount;
			}
		}
		strTemp = "	<Count>";
		strTemp.AddSmallInt(uTextureCount);
		strTemp += ASCString("</Count>");
		
		GameFile.PrintLine(strTemp);
		EditFile.PrintLine(strTemp);
		
		GameFile.PrintLine("</General>");
		EditFile.PrintLine("</General>");

		uValidCount = 0;
		uTextureCount = 0;
		for(UINT32 i = 0; i < ALTAS_MAX_LAYERS; ++i)
		{
			if(m_pAtlasLayers[i]->IsValid())
			{
				m_pAtlasLayers[i]->Save(GameFile, EditFile, uTextureCount, uValidCount, strTextureName);
				++uValidCount;
			}
		}
		
		
		GameFile.Close();
		EditFile.Close();
	}
}
void 
CTextureAtlasCreatorContext::Save()
{
	CHAR8 strOpenName[512] = "";
	OPENFILENAMEA FileName;
	memset(&FileName, 0, sizeof(OPENFILENAMEA));
	FileName.lStructSize = sizeof(OPENFILENAMEA);
	FileName.hwndOwner = reinterpret_cast<HWND>(Ascension::Renderer().GetWindowHandle());
	FileName.hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(NULL));
	FileName.lpstrFilter = NULL;
	FileName.lpstrCustomFilter = NULL;
	FileName.nMaxCustFilter = NULL;
	FileName.lpstrFilter = "Ascension Atlas Files\0*.ascatl*\0\0";
	FileName.nFilterIndex = 1;
	FileName.lpstrFile = strOpenName;
	FileName.nMaxFile = 512;
	FileName.lpstrFileTitle = NULL;
	FileName.lpstrTitle = "Save File";
	FileName.Flags = OFN_EXPLORER;

	if(TRUE == GetSaveFileNameA(&FileName))
	{
		SaveAtlas(strOpenName);
	}
}
void 
CTextureAtlasCreatorContext::Load()
{
	CHAR8 strOpenName[512] = "";
	OPENFILENAMEA FileName;
	memset(&FileName, 0, sizeof(OPENFILENAMEA));
	FileName.lStructSize = sizeof(OPENFILENAMEA);
	FileName.hwndOwner = reinterpret_cast<HWND>(Ascension::Renderer().GetWindowHandle());
	FileName.hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(NULL));
	FileName.lpstrFilter = NULL;
	FileName.lpstrCustomFilter = NULL;
	FileName.nMaxCustFilter = NULL;
	FileName.lpstrFilter = "Ascension Atlas Files\0*.ascatl;*.ascatledt*\0\0";
	FileName.nFilterIndex = 2;
	FileName.lpstrFile = strOpenName;
	FileName.nMaxFile = 512;
	FileName.lpstrFileTitle = NULL;
	FileName.lpstrTitle = "Open Atlas";
	FileName.Flags = OFN_EXPLORER;

	if(TRUE == GetOpenFileNameA(&FileName))
	{
		LoadAtlas(strOpenName);
	}
}
void 
CTextureAtlasCreatorContext::LoadAtlas(ASCString strFile)
{
	if(New())
	{
		ASCTexture* pAtlasTextures[ALTAS_MAX_LAYERS];
		for(UINT32 i = 0; i < ALTAS_MAX_LAYERS; ++i)
		{
			pAtlasTextures[i] = NULL;
		}

		ASCString strDirectory;
		ASCString strAtlasTexture;
		ASCString strTemp;

		strFile.poststrc('\\', strDirectory, 1);
	
		ASCINIFile File;
		if(File.Load(strFile))
		{
			UINT32 uLayerCount = File.GetUInt("General", "TextureCount");
			for(UINT32 i = 0; i < uLayerCount; ++i)
			{
				strTemp = "Texture_";
				strTemp.AddSmallInt(i);
				strAtlasTexture = strDirectory;
				strAtlasTexture += File.GetString("General", strTemp);
				pAtlasTextures[i] = Ascension::Renderer().GetTextureManager()->CreateTexture(strAtlasTexture, true);
			}

			ASCString strTextureSection = "";
			UINT32 uAtlasIndex = 0;

			UINT32 uTextureCount = File.GetUInt("General", "Count");
			for(UINT32 i = 0; i < uTextureCount; ++i)
			{
				strTextureSection = "Texture_";
				strTextureSection.AddSmallInt(i);

				uAtlasIndex = File.GetUInt(strTextureSection, "Texture");

				m_pAtlasLayers[uAtlasIndex]->AddTexture(strTextureSection, pAtlasTextures[uAtlasIndex], File);
			}
			
			for(UINT32 i = 1; i < uLayerCount; ++i)
			{
				m_pAtlasLayers[i]->Deactivate();
			}
		}
	}
}

//Events System
void 
CTextureAtlasCreatorContext::OnEvent(ASCIEvent* pEvent)
{
	switch(pEvent->TypeID())
	{
	case ET_GUI:
		{
			UINT32 uEventID = pEvent->ID();
			if(uEventID == m_uMainButtons[EMB_NEW])
			{
				New();
			}
			else if(uEventID == m_uMainButtons[EMB_ADD])
			{
				LoadTexture();
			}
			else if(uEventID == m_uMainButtons[EMB_REMOVE])
			{
				RemoveTexture();
			}
			else if(uEventID == m_uMainButtons[EMB_SAVE])
			{
				Save();
			}
			else if(uEventID == m_uMainButtons[EMB_LOAD])
			{
				Load();
			}
			else if(uEventID == m_uBigButtons[EBB_UPDATE])
			{
				m_pAtlasLayers[m_uActiveLayer]->UpdateCurrentTexture();
			}
			else if(uEventID == m_uBigButtons[EBB_UPDATE_ALL])
			{
				m_pAtlasLayers[m_uActiveLayer]->UpdateAllTextures();
			}
			else
			{
				for(UINT32 i = 0; i < ALTAS_MAX_LAYERS; ++i)
				{
					if(uEventID == m_uLayerButtons[i])
					{
						if(i != m_uActiveLayer)
						{
							m_pAtlasLayers[m_uActiveLayer]->Deactivate();
							m_uActiveLayer = i;
							m_pAtlasLayers[m_uActiveLayer]->Activate();
						}
						break;
					}
				}
			}
			break;
		}
	case ET_INPUT:
		{
			ASCInputEvent* pInputEvent = reinterpret_cast<ASCInputEvent*>(pEvent);
			UINT32 uID = pEvent->ID();
			if(uID == m_uMouseLeftButton)
			{
				m_bMouseDown = pInputEvent->GetState() == KS_Pressed || pInputEvent->GetState() == KS_Held;
			}
			else if(uID == m_uMouseRightButton)
			{
				m_bMouseRightDown = pInputEvent->GetState() == KS_Pressed || pInputEvent->GetState() == KS_Held;
			}
			else if (uID == m_uMouseXY)
			{
				FLOAT32 fX = pInputEvent->GetPos().m_fX;
				FLOAT32 fY = pInputEvent->GetPos().m_fY;
				if(m_bMouseDown)
				{
					if(fX >= ATLAS_CENTER_X - ATLAS_AREA_H_WIDTH && fX <= ATLAS_CENTER_X + ATLAS_AREA_H_WIDTH && fY >= ATLAS_CENTER_Y - ATLAS_AREA_H_HEIGHT && fY <= ATLAS_CENTER_Y + ATLAS_AREA_H_HEIGHT)
					{
						m_pAtlasLayers[m_uActiveLayer]->MoveCurrentTexture(fX - m_fMouseX, fY - m_fMouseY);
					}
				}
				else if( m_bMouseRightDown )
				{
					if(fX >= ATLAS_CENTER_X - ATLAS_AREA_H_WIDTH && fX <= ATLAS_CENTER_X + ATLAS_AREA_H_WIDTH && fY >= ATLAS_CENTER_Y - ATLAS_AREA_H_HEIGHT && fY <= ATLAS_CENTER_Y + ATLAS_AREA_H_HEIGHT)
					{
						m_pAtlasLayers[m_uActiveLayer]->PanAtlas(fX - m_fMouseX, fY - m_fMouseY);
					}
				}
				m_fMouseX = fX;
				m_fMouseY = fY;
			}
			else if (uID == m_uSroll)
			{
				FLOAT32 fWheel = pInputEvent->GetScrollWheel();

				
				if(m_fMouseX >= ATLAS_CENTER_X - ATLAS_AREA_H_WIDTH && m_fMouseX <= ATLAS_CENTER_X + ATLAS_AREA_H_WIDTH && m_fMouseY >= ATLAS_CENTER_Y - ATLAS_AREA_H_HEIGHT && m_fMouseY <= ATLAS_CENTER_Y + ATLAS_AREA_H_HEIGHT)
				{
					m_fMouseScrollWheel += fWheel * 0.005f;
					if(m_fMouseScrollWheel > 0.05f)
					{
						//m_pAtlasLayers[m_uActiveLayer]->Zoom(m_fMouseScrollWheel);
					}
					else
					{
						m_fMouseScrollWheel -= fWheel * 0.005f;
					}
				}
			}
			break;
		}
	default:break;
	}
}


BOOL CALLBACK ContinueYN(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
	wParam;
	lParam;
    switch (message) 
    { 
		case WM_ACTIVATE:
		{
			return TRUE;
		}
        case WM_COMMAND: 
            switch (LOWORD(wParam)) 
            { 
                case IDOK:
					{
					g_bContinue = true;
                    EndDialog(hwndDlg, wParam); 
					return TRUE;
					} 
                case IDCANCEL:
					{
					g_bContinue = false;
                    EndDialog(hwndDlg, wParam); 
                    return TRUE; 
					}
            } 
    } 
    return FALSE; 
} 
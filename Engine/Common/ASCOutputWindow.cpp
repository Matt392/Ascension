#include "ASCGenInc.h"
#include "ASCOutputWindow.h"

#include "..\Ascension\Ascension.h"
#include "ASCTexture.h"
#include "ASCTextureManager.h"
#include "ASCRenderer.h"
#include "ASCShader.h"
#include "ASCShaderManager.h"
#include "ASCVertexBuffer.h"
#include "ASCFont.h"
#include "ASCInputEvent.h"
#include "ASCKeyboardEvent.h"
#include "ASCInputDevice.h"
#include "ASCCMDEvent.h"

ASCOutputWindow::ASCOutputWindow(void)
: m_pTexture ( NULL )
, m_bActive ( false )
, m_uIndex ( 0 )
{
}


ASCOutputWindow::~ASCOutputWindow(void)
{
	m_pTexture = NULL;
}
void 
ASCOutputWindow::Initialise(UINT32 uBufferSize)
{
	m_strInputLine = "";
	m_strPrevMessage = "";
	Ascension::EventSystem().AddEventListener(this, ET_KEYBOARD);
	Ascension::EventSystem().AddEventListener(this, ET_INPUT);
	Ascension::EventSystem().AddEventListener(this, ET_CMD_WND);
	
	Ascension::KeyBoard().RegisterInput(m_uEnter, "Key_Enter");
	Ascension::KeyBoard().RegisterInput(m_uBackspace, "Key_Backspace");
	Ascension::KeyBoard().RegisterInput(m_uUpArrow, "Key_Up");
	Ascension::KeyBoard().RegisterInput(m_uTilde, "Key_Tilde");
	Ascension::KeyBoard().RegisterInput(m_uLeft, "Key_Left");
	Ascension::KeyBoard().RegisterInput(m_uRight, "Key_Right");

	AddCommand("-Help", "Display all registered commands");

	for(UINT32 i = 0; i < uBufferSize; ++i)
	{
		m_lstLines.AddToListStart("");
	}

	m_pFont = Ascension::Renderer().LoadFont("Fonts/CourierNew.fnt");

	ASCString strTextureName = "";
	m_pTexture = Ascension::Renderer().GetTextureManager()->CreateTexture(ASC_OUTPUT_WINDOW_TEXTURE_SIZE_WIDTH, ASC_OUTPUT_WINDOW_TEXTURE_SIZE_HEIGHT, strTextureName);
	if(m_pTexture)
	{
		UINT32* pTexels = NULL;
		UINT32* pOrigTexels = NULL;
		UINT32 uPitch = 0;
		if(m_pTexture->LockTexels(pTexels, uPitch, false))
		{
			pOrigTexels = pTexels;
			for(UINT32 p = 0; p < ASC_OUTPUT_WINDOW_TEXTURE_SIZE_HEIGHT; ++p)
			{
				for(UINT32 i = 0; i < ASC_OUTPUT_WINDOW_TEXTURE_SIZE_WIDTH; ++i)
				{
					pTexels[i] = 0xA10000FF;
				}
				pTexels += uPitch;
			}
			pTexels = pOrigTexels;
			for(UINT32 i = 0; i < ASC_OUTPUT_WINDOW_TEXTURE_SIZE_WIDTH; ++i)
			{
				pTexels[i] = 0xFF000000;
			}

			m_pTexture->UnlockTexels();
		}
		else
		{
			assert_now("Guts, failed to lock texture for output window");
		}
		
		m_pVertDecl = Ascension::Renderer().GetDefaultVertDecl();
		m_pRenderBuffer = Ascension::Renderer().CreateVertexBuffer(6, m_pVertDecl->GetStride());

		FLOAT32 fBottom = (((uBufferSize+2) * m_pFont->GetHeight() + (m_pFont->GetHeight() * 0.2f)) / SC_FLOAT(Ascension::Height()) * 2.0f) - 1.0f;

		SVertex sCorners[4];
		
		// top left
		sCorners[0].m_fX = -1.0f;
		sCorners[0].m_fY = -fBottom;
		sCorners[0].m_fTU = 0.0f;
		sCorners[0].m_fTV = 0.0f;
		sCorners[0].SetColour(0xFFFFFFFF);
		sCorners[0].SetSecondColour(0xFFFFFFFF);
		sCorners[0].m_fZ = 0.0f;
		sCorners[0].m_fW = 1.0f;
		sCorners[0].m_fScaleX = 1.0f;
		sCorners[0].m_fScaleY = 1.0f;
		sCorners[0].m_fPosX = 0.0f;
		sCorners[0].m_fPosX = 0.0f;

		// top right
		sCorners[1].m_fX = 1.0f;
		sCorners[1].m_fY = -fBottom;
		sCorners[1].m_fTU = 1.0f;
		sCorners[1].m_fTV = 0.0f;
		sCorners[1].SetColour(0xFFFFFFFF);
		sCorners[1].SetSecondColour(0xFFFFFFFF);
		sCorners[1].m_fZ = 0.0f;
		sCorners[1].m_fW = 1.0f;
		sCorners[1].m_fScaleX = 1.0f;
		sCorners[1].m_fScaleY = 1.0f;
		sCorners[1].m_fPosX = 0.0f;
		sCorners[1].m_fPosX = 0.0f;

		// bottom right
		sCorners[2].m_fX = 1.0f;
		sCorners[2].m_fY = 1.0f;
		sCorners[2].m_fTU = 1.0f;
		sCorners[2].m_fTV = 1.0f;
		sCorners[2].SetColour(0xFFFFFFFF);
		sCorners[2].SetSecondColour(0xFFFFFFFF);
		sCorners[2].m_fZ = 0.0f;
		sCorners[2].m_fW = 1.0f;
		sCorners[2].m_fScaleX = 1.0f;
		sCorners[2].m_fScaleY = 1.0f;
		sCorners[2].m_fPosX = 0.0f;
		sCorners[2].m_fPosX = 0.0f;

		// bottom left
		sCorners[3].m_fX = -1.0f;
		sCorners[3].m_fY = 1.0f;
		sCorners[3].m_fTU = 0.0f;
		sCorners[3].m_fTV = 1.0f;
		sCorners[3].SetColour(0xFFFFFFFF);
		sCorners[3].SetSecondColour(0xFFFFFFFF);
		sCorners[3].m_fZ = 0.0f;
		sCorners[3].m_fW = 1.0f;
		sCorners[3].m_fScaleX = 1.0f;
		sCorners[3].m_fScaleY = 1.0f;
		sCorners[3].m_fPosX = 0.0f;
		sCorners[3].m_fPosX = 0.0f;
			
		SVertex* pData = reinterpret_cast<SVertex*>(m_pRenderBuffer->Lock());
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
	
		m_pRenderBuffer->UnLock();
	}
	else
	{
		assert_now("Guts, failed to create texture for output window");
	}
}
void 
ASCOutputWindow::AddLine(ASCString& strLine)
{
	m_lstLines.RemoveLast();
	m_lstLines.AddToListStart(strLine);
}
void 
ASCOutputWindow::AddLine(CHAR8* strLine)
{
	m_lstLines.RemoveLast();
	m_lstLines.AddToListStart(strLine);
}
void 
ASCOutputWindow::Switch(bool bVal)
{
	m_bActive = bVal;
}

void 
ASCOutputWindow::Render()
{
	if(m_bActive)
	{
		static ASCShader* pShader = Ascension::Renderer().GetShaderManager()->LoadShader("Shaders/SpriteShaders");

		static ASC_Handle m_TechniqueHandle = pShader->GetNamedHandle("AsIs");

		pShader->SetTechnique(m_TechniqueHandle);
		m_pTexture->Apply( 0 );
		m_pRenderBuffer->SetSource();
		
		if(m_pVertDecl)
		{
			m_pVertDecl->Apply();
		}

		pShader->Begin();
		
		Ascension::Renderer().DrawPrimitives( PT_TriList, 2);

		pShader->End();


		ASCString strLines = m_strInputLine;
		strLines += ASCString("\n\n");

		ASCListNode<ASCString>* pNode = m_lstLines.First();
		for(; NULL != pNode; pNode = pNode->Next())
		{
			strLines += pNode->Value();
			strLines += ASCString("\n");
		}

		m_pFont->Render(0.0f, 0.0f, strLines, 0xFFFF0000);
	}
}
void 
ASCOutputWindow::AddCommand(ASCString& strCommand, ASCString& strDesc)
{
	ASCString strInfo = strCommand;
	strInfo += ASCString("  -  ");
	strInfo += strDesc;
	m_vecCommands.push_back( strInfo );
}
void 
ASCOutputWindow::AddCommand(CHAR8* strCommand, CHAR8* strDesc)
{
	ASCString strInfo = strCommand;
	strInfo += ASCString("  -  ");
	strInfo += ASCString(strDesc);
	m_vecCommands.push_back( strInfo );
}

void 
ASCOutputWindow::OnEvent(ASCIEvent* pEvent)
{
	if(m_bActive)
	{
		switch(pEvent->TypeID())
		{
		case ET_CMD_WND:
			{
				ASCCMDEvent* pCDMEvent = reinterpret_cast<ASCCMDEvent*>(pEvent);
				if(pCDMEvent->GetCMD() == "-Help")
				{
					AddLine("### Avaliable Commands ###");
					for(UINT32 i = 0; i < m_vecCommands.size(); ++i)
					{
						AddLine(m_vecCommands[i]);
					}
					AddLine("### Avaliable Commands ###");
				}
				break;
			}
		case ET_KEYBOARD:
			{
				ASCKeyboardEvent* pKeyboardEvent = reinterpret_cast<ASCKeyboardEvent*>(pEvent);
				m_strInputLine += pKeyboardEvent->GetCharVal();
				break;
			}
		case ET_INPUT:
			{
				ASCInputEvent* pInputEvent = reinterpret_cast<ASCInputEvent*>(pEvent);
				if(pInputEvent->GetState() == KS_Pressed)
				{
					if(pInputEvent->ID() == m_uEnter)
					{
						if(m_strInputLine.length() > 0)
						{
							AddLine(m_strInputLine);
							m_strPrevMessage = m_strInputLine;
							m_strInputLine = "";

							ASCCMDEvent Event;
							Event.SetCMD(&m_strPrevMessage);
							Event.SetEventTypeID(ET_CMD_WND);
							Ascension::EventSystem().AddEvent(Event);
						}
					}
					else if(pInputEvent->ID() == m_uBackspace)
					{
						if(m_strInputLine.length() > 0)
						{
							m_strInputLine.RemoveLast();
						}
					}
					else if(pInputEvent->ID() == m_uUpArrow)
					{
						m_strInputLine = m_strPrevMessage;
					}
					else if(pInputEvent->ID() == m_uTilde)
					{
						m_bActive = !m_bActive;
					}
					else if(pInputEvent->ID() == m_uLeft)
					{
						--m_uIndex;
						if(m_uIndex >= m_vecCommands.size())
						{
							m_uIndex = m_vecCommands.size()-1;
						}
						m_vecCommands[m_uIndex].prestrc(' ', m_strInputLine);
					}
					else if(pInputEvent->ID() == m_uRight)
					{
						++m_uIndex;
						if(m_uIndex >= m_vecCommands.size())
						{
							m_uIndex = 0;
						}
						m_vecCommands[m_uIndex].prestrc(' ', m_strInputLine);
					}
				}
				else if(pInputEvent->GetState() == KS_DoublePressed)
				{
					if(pInputEvent->ID() == m_uBackspace)
					{
						if(m_strInputLine.length() > 0)
						{
							m_strInputLine.RemoveLast();
						}
					}
					else if(pInputEvent->ID() == m_uLeft)
					{
						--m_uIndex;
						if(m_uIndex >= m_vecCommands.size())
						{
							m_uIndex = m_vecCommands.size()-1;
						}
						m_vecCommands[m_uIndex].prestrc(' ', m_strInputLine);
					}
					else if(pInputEvent->ID() == m_uRight)
					{
						++m_uIndex;
						if(m_uIndex >= m_vecCommands.size())
						{
							m_uIndex = 0;
						}
						m_vecCommands[m_uIndex].prestrc(' ', m_strInputLine);
					}
				}
				break;
			}
		default:break;
		}
	}
	else
	{
		switch(pEvent->TypeID())
		{
		case ET_INPUT:
			{
				ASCInputEvent* pInputEvent = reinterpret_cast<ASCInputEvent*>(pEvent);
				if(pInputEvent->GetState() == KS_Pressed)
				{
					if(pInputEvent->ID() == m_uTilde)
					{
						m_bActive = !m_bActive;
					}
				}
				break;
			}
		default:break;
		}
	}
}
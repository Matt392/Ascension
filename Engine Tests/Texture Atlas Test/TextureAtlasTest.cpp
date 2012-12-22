#include "ASCGenInc.h"
#include "TextureAtlasTest.h"

#include "Ascension.h"
#include "ASCInputEvent.h"
#include "ASCInputDevice.h"
#include "ASCSpriteManager.h"
#include "ASCSprite.h"


CTextureAtlasTest::CTextureAtlasTest(void)
: m_uExitButton ( 0 )
, m_bQuit ( false )
{
}


CTextureAtlasTest::~CTextureAtlasTest(void)
{
}

//Creation
bool 
CTextureAtlasTest::Initialise()
{
	Ascension::EventSystem().AddEventListener(this, ET_INPUT);
	
	Ascension::KeyBoard().RegisterInput(m_uExitButton, "Key_Escape");

	ASCSprite* pSprite = NULL;

	pSprite = Ascension::SpriteManager().CreateSpriteFile("Sprites/Alpha.ascspr");
	pSprite->SetPosition(-8.0f, 0.0f);

	pSprite = Ascension::SpriteManager().CreateSpriteFile("Sprites/Beta.ascspr");
	pSprite->SetPosition(8.0f, 0.0f);

	return true;
}
		
//Game logic
bool 
CTextureAtlasTest::Update()
{
	return m_bQuit == false;
}

//Optional
void 
CTextureAtlasTest::Draw()
{
}
void 
CTextureAtlasTest::Disable()
{
}

//Events System
void 
CTextureAtlasTest::OnEvent(ASCIEvent* pEvent)
{
	switch(pEvent->TypeID())
	{
	case ET_GUI:
		{
			break;
		}
	case ET_INPUT:
		{
			if(pEvent->ID() == m_uExitButton)
			{
				ASCInputEvent* pInputEvent = reinterpret_cast<ASCInputEvent*>(pEvent);
				if(pInputEvent->GetState() == KS_Pressed)
				{
					m_bQuit = true;
				}
			}
		}
	default:break;
	}
}

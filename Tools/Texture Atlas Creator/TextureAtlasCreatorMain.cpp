#include "Ascension.h"
#include "ASCFinalShutdownDelay.h"

#include "TextureAtlasCreatorContext.h"

#include <windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SInitParameters InitParam; 
	InitParam.m_strSettings = "Settings.ascini";
	InitParam.m_bCreateMouse = true;
	InitParam.m_bCreateKeyboard = true;
	InitParam.m_pContext = new CTextureAtlasCreatorContext();
	InitParam.m_pLoadingContext = NULL;
	
	Ascension::Initialise(InitParam);
	Ascension::Run();

	Ascension::DeInitialise();
	
	ASCFinalShutdownDelay::SetCanEnd( true );

	return 0;
}
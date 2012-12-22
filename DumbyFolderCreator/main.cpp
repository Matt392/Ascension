#include "Ascension.h"
#include "ASCString.h"
#include "ASCINIManager.h"
#include "ASCMemoryManagement.h"
#include "ASCProfilingSystem.h"
#include "ToolContext.h"
#include "ASCFinalShutdownDelay.h"

#include <windows.h>

using namespace ASC;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//ASCMemoryManagement::Start(3000, false, 10);

	SInitParameters InitParam;
	InitParam.m_bCreateClock = true;
	InitParam.m_bUseProfiling = true;
	InitParam.m_bCreateMouse = true;
	InitParam.m_bCreateKeyboard = true;
	InitParam.m_eRendererType = ERT_DX9;
	InitParam.m_uScreenWidth = 1280;
	InitParam.m_uScreenHeight = 1024;
	InitParam.m_bUseDebugging = false;
	InitParam.m_pContext = new CToolContext();


	Ascension::Initialise(InitParam);

	Ascension::Run();

	Ascension::DeInitialise();
	
	ASCFinalShutdownDelay::SetCanEnd( true );
	//ASCMemoryManagement::End();
	return 0;
}
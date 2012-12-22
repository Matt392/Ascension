#include "ASCGenInc.h"
#include "ASCAssert.h"
#ifdef ASC_IOS
#else
#include <windows.h>
#endif

void 
ASCAssert::AssertMessage(bool bAssert, char* strMsg)
{
	if(false == bAssert)
	{
		ASCString strMessage = strMsg;
		ASCString strTemp = "\n About to exit, Try to debug, Ignore to continue";
		strMessage += strTemp;
#ifdef ASC_IOS
        ASCDebuggingManager::OutputWindowLine("### ASC_ASSERT ### - ", ASCDebugString( strMsg ));
#else
		UINT32 uSelection = MessageBoxA(0, strMessage.c_str(), "Debug assert", MB_ABORTRETRYIGNORE | MB_ICONERROR);
		switch(uSelection)
		{
		case IDABORT:
			{
				ASCString strAbtMessage = "Exit by assert";
				FatalAppExitA(0, strAbtMessage.c_str());
				break;
			}
		case IDRETRY:
			{
#ifdef ASC_X64
#else
				_asm{ int 3 };
#endif
				break;
			}
		case IDIGNORE:
			{
				break;
			}
		default:
			{
				ASCString strAbtMessage = "Unknow result from window";
				FatalAppExitA(0, strAbtMessage.c_str());
				break;
			}
		}
#endif
	}
}
void 
ASCAssert::AssertMessageRelease(bool bAssert, char* strMsg)
{
	if(false == bAssert)
	{
		bAssert = false;
		strMsg = NULL;
	}
}
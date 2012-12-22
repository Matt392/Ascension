#include "ASCGenInc.h"

#include "ASCClock.h"
#include "ASCINIManager.h"
#include "ASCProfilingSystem.h"
#include "ASCEventManager.h"
#include "ASCRenderer.h"
#include "ASCGameContext.h"
#include "ASCSpriteManager.h"
#include "ASCVisualDebugging.h"
#include "ASCKeyboard.h"
#include "ASCMouse.h"
#include "ASCTouchInput.h"
#include "ASCKeyboardObserver.h"
#include "ASCPhysicsScene.h"
#include "ASCAudioManager.h"
#include "ASCGUIManager.h"
#include "ASCCamera.h"
#include "ASCVideoManager.h"
#include "ASCMemoryManagement.h"
#include "ASCStringDebugInfo.h"
#include "ASCFileIO.h"
#include "ASCRemoteLogging.h"
#include "ASCStringRecyleBin.h"
#include "ASCOutputWindow.h"
#include "ASCINIFile.h"

#ifdef ASC_RENDERER_DX9
#include "ASCDX9Renderer.h"
#endif
#ifdef ASC_RENDERER_GLES2
#include "ASCGLES2Renderer.h"
#endif

#include "Ascension.h"

FLOAT32 Ascension::sm_fDelta = 0.0f;
FLOAT32 Ascension::sm_fTimeSacle = 1.0f;
FLOAT32 Ascension::sm_fTimeBetweenRenders = 0.0f;
FLOAT32 Ascension::sm_fDrawsPerFrame = 60.0f;
FLOAT32 Ascension::sm_fRatioY = 0.0f;
FLOAT32 Ascension::sm_fRatioX = 0.0f;

UINT32 Ascension::sm_uWidth = 0;
UINT32 Ascension::sm_uHeight = 0;
UINT32 Ascension::sm_uInputEventCount = 1;

ASCClock* Ascension::sm_pClock = NULL;
ASCINIManager* Ascension::sm_pINIManager = NULL;
ASCDebuggingManager* Ascension::sm_pDebuggingManager = NULL;
//ASCProfilingSystem* Ascension::sm_pProfilingSystem = NULL;
ASCEventManager* Ascension::sm_pEventManager = NULL;
ASCRenderer* Ascension::sm_pRenderer = NULL;
ASCGameContext* Ascension::sm_pContext = NULL;
ASCSpriteManager* Ascension::sm_pSpriteManager = NULL;
ASCVisualDebugging* Ascension::sm_pVisualDebugger = NULL;
ASCKeyboardObserver* Ascension::sm_pKeyboardObserver = NULL;
ASCPhysicsScene* Ascension::sm_pPhysicsScene = NULL;
ASCAudioManager* Ascension::sm_pAudioManager = NULL;
ASCGUIManager* Ascension::sm_pGUIManager = NULL;
ASCCamera* Ascension::sm_pCamera = NULL;
ASCVideoManager* Ascension::sm_pVideoManager = NULL;
ASCDebugInfoListener* Ascension::sm_pDebugInfoListener = NULL;
ASCRemoteLogging* Ascension::sm_pRemoteLogging = NULL;
ASCGameContext* Ascension::sm_pLoadingContext = NULL;
EDevice Ascension::sm_eDevice = ED_COUNT;

#ifdef ASC_WINDOWS
ASCInputDevice* Ascension::sm_pKeyboard = NULL;
ASCInputDevice* Ascension::sm_pMouse = NULL;
#endif
#ifdef ASC_IOS
ASCInputDevice* Ascension::sm_pTouch = NULL;
#endif

#ifdef ASC_DEBUG_TO_FILE
ASCString Ascension::sm_strFileDebug = "";
#endif

bool Ascension::sm_bShowMemoryInfo = true;
bool Ascension::sm_bShowClockInfo = true;
bool Ascension::sm_bShowDrawInfo = true;
bool Ascension::sm_bRunning = true;
bool Ascension::sm_bContextInitialised = false;
bool Ascension::sm_bFirstFrame = true;
bool Ascension::sm_bPaused = false;

//Initialise
bool 
Ascension::Initialise(SInitParameters& rInitParams)
{
    ASCDebuggingManager::OutputWindowLine("Initialise Ascension");
	ASCINIFile INIFile;
	if(INIFile.Load(rInitParams.m_strSettings))
	{
		if(INIFile.GetBool("Logging", "RemoteLogging"))
		{
			sm_pRemoteLogging = new ASCRemoteLogging();
			sm_pRemoteLogging->Begin(INIFile.GetString("Logging", "RemoteLoggingName"));
		}

		sm_bRunning = true;
		sm_fDrawsPerFrame = INIFile.GetFloat("Screen", "DrawFrameRate");
		sm_pDebuggingManager = new ASCDebuggingManager(true);
	
		sm_pClock = new ASCClock(true);
		
		sm_pINIManager = new ASCINIManager();
	
		ASCRandNumGen::Initialise(INIFile.GetUInt("General", "MaxRand"));

		sm_pEventManager = new ASCEventManager();
	
		sm_uWidth = INIFile.GetUInt("Screen", "Width");
		sm_uHeight = INIFile.GetUInt("Screen", "Height");

		SRendererInit sRendInit;
		sRendInit.m_uScreenWidth = sm_uWidth;
		sRendInit.m_uScreenHeight = sm_uHeight;
		sRendInit.m_bWindowMode = INIFile.GetBool("Screen", "Windowed");
		sRendInit.m_strWindowName = INIFile.GetString("General", "WindowName");
		sRendInit.m_hInstance = rInitParams.m_hInstance;
		sRendInit.m_pIOSGLView = rInitParams.m_pIOSGLView;

	#ifdef ASC_RENDERER_DX9
			sm_pRenderer = new ASCDX9Renderer();
			sm_pRenderer->Initialise(sRendInit);
	#endif
	#ifdef ASC_RENDERER_GLES2
			sm_pRenderer = new ASCGLES2Renderer();
			sm_pRenderer->Initialise(sRendInit);
	#endif
	
	#ifdef ASC_WINDOWS
		if(rInitParams.m_bCreateMouse)
		{
			sm_pMouse = new ASCMouse(true, false);
			sm_pMouse->SetFullScreen(sRendInit.m_bWindowMode == false);
		}
		if(rInitParams.m_bCreateKeyboard)
		{
			sm_pKeyboard = new ASCKeyboard();
			sm_pKeyboardObserver = new ASCKeyboardObserver();
			sm_pKeyboardObserver->Init();
		}
	#endif
	#ifdef ASC_IOS
		sm_pTouch = new ASCTouchInput();
	#endif

		sm_pDebuggingManager->Initialise(INIFile.GetUInt("Debugging", "CMDBuffer"));
		sm_pCamera = new ASCCamera();

		sm_pSpriteManager = new ASCSpriteManager();
		sm_pSpriteManager->Initialise();

		SPhyInitParam pParam;
		pParam.m_fGravityX = INIFile.GetFloat("Gravity", "X");
		pParam.m_fGravityY = INIFile.GetFloat("Gravity", "Y");

		sm_pPhysicsScene = new ASCPhysicsScene();
		sm_pPhysicsScene->Init( pParam );

		sm_pAudioManager = new ASCAudioManager();
		sm_pAudioManager->Initialise();

		sm_pGUIManager = new ASCGUIManager();
	#ifdef ASC_IOS
		sm_fRatioX = SC_FLOAT(rInitParams.m_uScreenHeight) / SC_FLOAT(rInitParams.m_uDesignedScreenWidth);
		sm_fRatioY = SC_FLOAT(rInitParams.m_uScreenWidth) / SC_FLOAT(rInitParams.m_uDesignedScreenHeight);
	#else
		sm_fRatioX = SC_FLOAT(sm_uWidth) / INIFile.GetFloat("Screen", "DesignedWidth");
		sm_fRatioY = SC_FLOAT(sm_uHeight) / INIFile.GetFloat("Screen", "DesignedHeight");
	#endif
		sm_pGUIManager->Init(sm_fRatioX, sm_fRatioY);

		sm_pVisualDebugger = new ASCVisualDebugging();
	
		sm_pVideoManager = new ASCVideoManager();

		sm_pRenderer->CalibratePicking();

		//Do this last
		sm_pContext = rInitParams.m_pContext;
		//sm_pContext->Initialise();
		sm_bContextInitialised = false;
    
		sm_pLoadingContext = rInitParams.m_pLoadingContext;
		if(sm_pLoadingContext)
		{
			sm_pLoadingContext->Initialise();
		}
		sm_bFirstFrame = true;

		sm_pDebugInfoListener = new ASCDebugInfoListener();
		sm_pDebugInfoListener->Initialise();
    
		ASCDebuggingManager::OutputWindowLine("Ascension Initialised");
		sm_pDebuggingManager->MaxLinesToDraw(INIFile.GetUInt("Debugging", "MaxDebugLines"));
	
		return true;
	}	

	return false;
}

//Update
void 
Ascension::Run()
{
#ifdef ASC_IOS
	//MSG msg;
	//ZeroMemory( &msg, sizeof( msg) );
    
	//while ( msg.message != WM_QUIT )
	//{
		// Enter the message loop
		//if ( PeekMessage( &msg, NULL, 0, 0 , PM_REMOVE) )
		//{
			//TranslateMessage( &msg );
			//DispatchMessage( &msg );
		//}
		//else
		//{
			//if ( false == Ascension::Update() )
			//{
				//break;
			//}
		//}
	//}
#endif
#ifdef ASC_WINDOWS
	MSG msg;
	ZeroMemory( &msg, sizeof( msg) );

	while ( msg.message != WM_QUIT )
	{
		// Enter the message loop
		if ( PeekMessage( &msg, NULL, 0, 0 , PM_REMOVE) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			if ( false == Ascension::Update() )
			{
				break;
			}
		}
	}
#endif
}
bool 
Ascension::Update()
{
    if(false == sm_bContextInitialised)
    {
        if(false == sm_bFirstFrame || NULL == sm_pLoadingContext)
        {
            ASCDebuggingManager::OutputWindowLine("Initialising Application Context");
            sm_bContextInitialised = true;
            sm_pContext->Initialise();
			ASCStringRecyleBin::CleanUp();
            ASCDebuggingManager::OutputWindowLine("Begining Application");
        }
        else
        {
            sm_bFirstFrame = sm_pLoadingContext->Update();
			if(sm_pClock)
			{
				sm_pClock->Update();
			}            
#ifdef ASC_WINDOWS
            sm_pMouse->Update(Ascension::Delta());
            sm_pKeyboard->Update(Ascension::Delta());
#endif
            
#ifdef ASC_IOS
            sm_pTouch->Update(Ascension::Delta());
#endif
            
			sm_pEventManager->Process();
			sm_pVideoManager->Update();
            sm_pRenderer->ColourZClear(0xFF003900, 1.0f);
            sm_pRenderer->BeginScene();
            
            sm_pSpriteManager->Update();
            sm_pSpriteManager->Render();
            
            sm_pVisualDebugger->Render();
            
            sm_pDebuggingManager->OutputWindow()->Render();
            sm_pRenderer->EndScene();
			if(false == sm_bFirstFrame)
			{
				sm_pLoadingContext->Disable();           
#ifdef ASC_WINDOWS
                sm_pMouse->Update(Ascension::Delta());
                sm_pKeyboard->Update(Ascension::Delta());
#endif
                
#ifdef ASC_IOS
                sm_pTouch->Update(Ascension::Delta());
#endif
			}

        }
        return true;
    }
    
	bool m_bReturn = true;
	//Update

	//Process
	sm_pEventManager->Process();
	
	sm_pGUIManager->Update(Ascension::Delta());

	if(sm_pClock)
	{
		sm_pClock->Update();
	}

#ifdef ASC_WINDOWS
	sm_pMouse->Update(Ascension::Delta());
	sm_pKeyboard->Update(Ascension::Delta());
#endif
    
#ifdef ASC_IOS
    sm_pTouch->Update(Ascension::Delta());
    // Implement touch input update here
#endif

	sm_pPhysicsScene->Update(Ascension::Delta());

	sm_pVideoManager->Update();
	
	//Context Update	
	m_bReturn = sm_pContext->Update();


#ifdef ASC_IOS
    sm_fTimeBetweenRenders = 1.0f / sm_fDrawsPerFrame;
    //Draw
    START_PROFILING_NODE("Draw", "Frame Root", false);
    
    sm_pRenderer->ColourZClear(0xFF003900, 1.0f);
    sm_pRenderer->BeginScene();
    
    sm_pSpriteManager->Update();
    sm_pSpriteManager->Render();
    sm_pContext->Draw();
    
	
    sm_pVisualDebugger->Render();
    
    
    sm_pRenderer->EndScene();
    
    END_PROFILING_NODE("Draw");
#else
	sm_fTimeBetweenRenders -= Ascension::Delta(true);

	if(sm_fTimeBetweenRenders <= 0.0f)
	{
		sm_fTimeBetweenRenders = 1.0f / sm_fDrawsPerFrame;
		//Draw

		sm_pRenderer->ColourZClear(0xFF003900, 1.0f);
		sm_pRenderer->BeginScene();
		
		sm_pSpriteManager->Update();
		sm_pContext->Draw();
		sm_pSpriteManager->Render();
	
		sm_pVisualDebugger->Render();

		
		sm_pDebuggingManager->OutputWindow()->Render();

		if(sm_pDebugInfoListener->DoGUI())
		{
			sm_pGUIManager->DebugBounds();
		}
		sm_pRenderer->EndScene();
	}
	else
	{
		sm_pSpriteManager->Update();
		sm_pRenderer->ClearFontBuffer();
		Ascension::Debugging().ClearLineBuffer();
	}
#endif

	Ascension::DoDebugInfo();

	//if(sm_pProfilingSystem)
	//{
	//	//sm_pProfilingSystem->DisplayInfo();
	//}

	return m_bReturn;
}

//General
void 
Ascension::SetDelta(const FLOAT32 fDelta)
{
	sm_fDelta = fDelta;
	if(sm_fDelta > 2.0f)
	{
		sm_fDelta = 1.0f / 60.0f;
	}
}
void 
Ascension::Pause(bool bPause)
{
	sm_bPaused = bPause;
}
FLOAT32 
Ascension::Delta(bool bIgnorePause)
{
	if(false == sm_bPaused || bIgnorePause)
	{	
		return sm_fDelta * sm_fTimeSacle;
	}
	return 0.0f;
}
UINT32 
Ascension::Width()
{
	return sm_uWidth;
}
UINT32 
Ascension::Height()
{
	return sm_uHeight;
}
FLOAT32 
Ascension::ScaleX()
{
    return sm_fRatioX;
}
FLOAT32 
Ascension::ScaleY()
{
    return sm_fRatioY;
}

void 
Ascension::RegisterCommand(ASCString& strCommand, ASCString& strDesc)
{
	if(sm_pDebuggingManager)
	{
		if(sm_pDebuggingManager->OutputWindow())
		{
			sm_pDebuggingManager->OutputWindow()->AddCommand(strCommand, strDesc);
		}
	}
}
void 
Ascension::RegisterCommand(CHAR8* strCommand, CHAR8* strDesc)
{
	if(sm_pDebuggingManager)
	{
		if(sm_pDebuggingManager->OutputWindow())
		{
			sm_pDebuggingManager->OutputWindow()->AddCommand(strCommand, strDesc);
		}
	}
}

//Systems
ASCRenderer& 
Ascension::Renderer()
{
	assert_msg(sm_pRenderer != NULL, "Guts, No Renderer");
	return *sm_pRenderer;
}
ASCINIManager& 
Ascension::INI()
{
	assert_msg(sm_pINIManager != NULL, "Guts, No INI Manager");
	return *sm_pINIManager;
}
ASCDebuggingManager& 
Ascension::Debugging()
{
	assert_msg(sm_pDebuggingManager != NULL, "Guts, No Debugging Manager");
	return *sm_pDebuggingManager;
}
ASCEventManager& 
Ascension::EventSystem()
{
	assert_msg(sm_pEventManager != NULL, "Guts, No Event Manager");
	return *sm_pEventManager;
}
ASCSpriteManager& 
Ascension::SpriteManager()
{
	assert_msg(sm_pSpriteManager != NULL, "Guts, No Sprite Manager");
	return *sm_pSpriteManager;
}
#ifdef ASC_WINDOWS
ASCInputDevice& 
Ascension::KeyBoard()
{
	assert_msg(sm_pKeyboard != NULL, "Guts, No Keyboard");
	return *sm_pKeyboard;
}
ASCInputDevice& 
Ascension::Mouse()
{
	assert_msg(sm_pMouse != NULL, "Guts, No Mouse");
	return *sm_pMouse;
}
#endif
#ifdef ASC_IOS
ASCInputDevice& 
Ascension::Touch()
{
	assert_msg(sm_pTouch != NULL, "Guts, No Touch");
	return *sm_pTouch;
}
#endif
ASCPhysicsScene& 
Ascension::Physics()
{
	assert_msg(sm_pPhysicsScene != NULL, "Guts, No Physics");
	return *sm_pPhysicsScene;
}
ASCAudioManager& 
Ascension::Audio()
{
	assert_msg(sm_pAudioManager != NULL, "Guts, No Audio Manager");
	return *sm_pAudioManager;
}
ASCGUIManager& 
Ascension::GUI()
{
	assert_msg(sm_pGUIManager != NULL, "Guts, No GUI Manager");
	return *sm_pGUIManager;
}
ASCCamera& 
Ascension::Camera()
{
	assert_msg(sm_pCamera != NULL, "Guts, No Camera");
	return *sm_pCamera;
}
ASCVideoManager& 
Ascension::Video()
{
	assert_msg(sm_pVideoManager != NULL, "Guts, No Video Manager");
	return *sm_pVideoManager;
}
ASCClock& 
Ascension::Clock()
{
	assert_msg(sm_pClock != NULL, "Guts, No Clock");
	return *sm_pClock;
}
ASCGameContext& 
Ascension::Context()
{
	assert_msg(sm_pContext != NULL, "Guts, No Context");
	return *sm_pContext;
}
ASCGameContext& 
Ascension::LoadingContext()
{
	assert_msg(sm_pContext != NULL, "Guts, No Loading Context");
	return *sm_pLoadingContext;
}

//ASCProfilingSystem* 
//Ascension::Profiling()
//{
//	return sm_pProfilingSystem;
//}
UINT32 
Ascension::GetNextInputEventID()
{
	++sm_uInputEventCount;
	return sm_uInputEventCount - 1;
}
bool 
Ascension::Running()
{
	return sm_bRunning;
}
//De-Initialise
void 
Ascension::DeInitialise()
{
    Ascension::Debugging().OutputWindowLine("Beging shutdown");
	sm_bRunning = false;
	SafeDelete( sm_pClock );
	SafeDelete( sm_pINIManager );
	SafeDelete( sm_pDebuggingManager );
	//SafeDelete( sm_pProfilingSystem );
	SafeDelete( sm_pEventManager );
	SafeDelete( sm_pRenderer );
	SafeDelete( sm_pContext );
	SafeDelete( sm_pLoadingContext );
	SafeDelete( sm_pSpriteManager );
	SafeDelete( sm_pVisualDebugger );
	SafeDelete( sm_pKeyboardObserver );
	SafeDelete( sm_pPhysicsScene );
	SafeDelete( sm_pAudioManager );
	SafeDelete( sm_pGUIManager );
	SafeDelete( sm_pCamera );
	SafeDelete( sm_pVideoManager );
	SafeDelete( sm_pDebugInfoListener );
    
#ifdef ASC_WINDOWS
	SafeDelete( sm_pKeyboard );
	SafeDelete( sm_pMouse );
#endif

	SafeDelete( sm_pRemoteLogging );
}
void 
Ascension::DoDebugInfo()
{
	if(sm_pClock && sm_pDebugInfoListener->DoClock())
	{
		sm_pClock->DisplayInfo();
		Ascension::Debugging().AddScreenLine("");
		Ascension::Debugging().AddScreenLine("");
	}

	if(sm_pRenderer && sm_pDebugInfoListener->DoRnderering())
	{
		sm_pRenderer->ShowDrawInfo();
		Ascension::Debugging().AddScreenLine("");
		Ascension::Debugging().AddScreenLine("");
	}
	
	if(sm_pDebugInfoListener->DoMemory())
	{
		ASCMemoryManagement::DisplayMemoryInfo();
		Ascension::Debugging().AddScreenLine("");
		Ascension::Debugging().AddScreenLine("");
	}

	if(sm_pDebugInfoListener->DoStrings())
	{
		ASCStringDebugInfo::DisplayDebugInfo();
		Ascension::Debugging().AddScreenLine("");
		Ascension::Debugging().AddScreenLine("");
	}

	if(sm_pINIManager && sm_pDebugInfoListener->DoINI())
	{
		sm_pINIManager->DisplayDebugInfo();
		Ascension::Debugging().AddScreenLine("");
		Ascension::Debugging().AddScreenLine("");
	}
    /*
    if(sm_pGUIManager)
    {
        sm_pGUIManager->DebugInfo();
		Ascension::Debugging().AddScreenLine("");
		Ascension::Debugging().AddScreenLine("");
    }
     */
}
void 
Ascension::DebugToFile(ASCString strLine)
{
#ifdef ASC_DEBUG_TO_FILE
	sm_strFileDebug += strLine;
	sm_strFileDebug += ASCString("\n");

	ASCFileIO File;
	if(File.Open("DebugReport.txt", IO_OUT))
	{
		File.Clear();
		File.Print(sm_strFileDebug);
		File.Close();
	}
#endif
	strLine;
}

EDevice 
Ascension::Device()
{
    return sm_eDevice;
}
void 
Ascension::Device(EDevice eDevice)
{
    sm_eDevice = eDevice;
}
		
void 
Ascension::RemoteLog(ASCString strPre)
{
	if(NULL != sm_pRemoteLogging)
	{
		sm_pRemoteLogging->Log(strPre);
	}
}
void 
Ascension::RemoteLog(ASCString strPre, UINT32 uVal, ASCString strPost)
{
	if(NULL != sm_pRemoteLogging)
	{
		ASCString strTemp = strPre;
		strTemp += uVal;
		strTemp += strPost;
		sm_pRemoteLogging->Log(strTemp);
	}
}
void 
Ascension::RemoteLog(ASCString strPre, FLOAT32 fVal, ASCString strPost)
{
	if(NULL != sm_pRemoteLogging)
	{
		ASCString strTemp = strPre;
		strTemp += fVal;
		strTemp += strPost;
		sm_pRemoteLogging->Log(strTemp);
	}
}
void 
Ascension::RemoteLog(ASCString strPre, SINT32 iVal, ASCString strPost)
{
	if(NULL != sm_pRemoteLogging)
	{
		ASCString strTemp = strPre;
		strTemp += iVal;
		strTemp += strPost;
		sm_pRemoteLogging->Log(strTemp);
	}
}
void 
Ascension::RemoteLog(ASCString strPre, void* pVal, ASCString strPost)
{
	if(NULL != sm_pRemoteLogging)
	{
		ASCString strTemp = strPre;
		strTemp += pVal;
		strTemp += strPost;
		sm_pRemoteLogging->Log(strTemp);
	}
}
void 
Ascension::RemoteLog(ASCString strPre, ASCString strVal, ASCString strPost)
{
	if(NULL != sm_pRemoteLogging)
	{
		ASCString strTemp = strPre;
		strTemp += strVal;
		strTemp += strPost;
		sm_pRemoteLogging->Log(strTemp);
	}
}
void 
Ascension::TimeScale(FLOAT32 fTimeScale)
{
    sm_fTimeSacle = fTimeScale;
}




#include "ASCCMDEvent.h"


ASCDebugInfoListener::ASCDebugInfoListener()
: m_bMemory ( false )
, m_bClock ( false )
, m_bRnderering ( false )
, m_bStrings ( false )
, m_bINI ( false )
, m_bGUI ( false )
{
	m_bActive = true;
}
ASCDebugInfoListener::~ASCDebugInfoListener()
{
}

void 
ASCDebugInfoListener::Initialise()
{
	Ascension::EventSystem().AddEventListener(this, ET_CMD_WND);

	m_strClock = "-Debug_Clock";
	m_strMemory = "-Debug_Memory";
	m_strRnderering = "-Debug_Renderering";
	m_strStrings = "-Debug_Strings";
	m_strINI = "-Debug_INI";
	m_strGUI = "-Debug_GUI";
	
	Ascension::RegisterCommand(m_strClock.c_str(), "Display clock info");
	Ascension::RegisterCommand(m_strMemory.c_str(), "Display memory info");
	Ascension::RegisterCommand(m_strRnderering.c_str(), "Display renderering info");
	Ascension::RegisterCommand(m_strStrings.c_str(), "Display string info");
	Ascension::RegisterCommand(m_strINI.c_str(), "Display INI info");
	Ascension::RegisterCommand(m_strGUI.c_str(), "Display GUI bounds");
}

void 
ASCDebugInfoListener::OnEvent(ASCIEvent* pEvent)
{
	if(m_bActive)
	{
		switch(pEvent->TypeID())
		{
		case ET_GUI:
			{
				break;
			}
		case ET_CMD_WND:
			{
				ASCCMDEvent* pCMDEvent = reinterpret_cast<ASCCMDEvent*>(pEvent);
				ASCString& rstrCMD = pCMDEvent->GetCMD();
				if(rstrCMD == m_strClock)
				{
					m_bClock = !m_bClock;
				}
				else if(rstrCMD == m_strMemory)
				{
					m_bMemory = !m_bMemory;
				}
				else if(rstrCMD == m_strRnderering)
				{
					m_bRnderering = !m_bRnderering;
				}
				else if(rstrCMD == m_strStrings)
				{
					m_bStrings = !m_bStrings;
				}
				else if(rstrCMD == m_strINI)
				{
					m_bINI = !m_bINI;
				}
				else if(rstrCMD == m_strGUI)
				{
					m_bGUI = !m_bGUI;
				}
				break;
			}
		default:break;
		}
	}
}
bool 
ASCDebugInfoListener::DoClock()
{
	return m_bClock;
}
bool 
ASCDebugInfoListener::DoMemory()
{
	return m_bMemory;
}
bool 
ASCDebugInfoListener::DoRnderering()
{
	return m_bRnderering;
}
bool 
ASCDebugInfoListener::DoStrings()
{
	return m_bStrings;
}
bool 
ASCDebugInfoListener::DoINI()
{
	return m_bINI;
}
bool 
ASCDebugInfoListener::DoGUI()
{
	return m_bGUI;
}
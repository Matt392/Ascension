/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Ascension\Ascension.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCENSION_H__Included_819173460
#define _ASCENSION_H__Included_819173460

#include "ASCGenInc.h"
#include "ASCRandNumGen.h"

namespace ASC
{
	class ASCClock;
	class ASCINIManager;
	class ASCDebuggingManager;
	//class ASCProfilingSystem;
	class ASCEventManager;
	class ASCRenderer;
	class ASCGameContext;
	class ASCSpriteManager;
	class ASCVisualDebugging;
	class ASCInputDevice;
	class ASCKeyboardObserver;
	class ASCPhysicsScene;
	class ASCAudioManager;
	class ASCGUIManager;
	class ASCCamera;
	class ASCVideoManager;
	class ASCRemoteLogging;
    
    enum EDevice
    {
        ED_IPHONE,
        ED_IPAD,
        ED_COUNT,
    };

	struct SInitParameters
	{
		ASCString m_strSettings;

		ASCGameContext* m_pContext;
		ASCGameContext* m_pLoadingContext;
		void* m_hInstance;
        void* m_pIOSGLView;
		
		bool m_bCreateMouse;
		bool m_bCreateKeyboard;


		SInitParameters()
		{
			m_strSettings = "";

			m_pContext = NULL;
            m_pLoadingContext = NULL;
			m_hInstance = NULL;
            m_pIOSGLView = NULL;

			m_bCreateMouse = false;
			m_bCreateKeyboard = false;
		}
		~SInitParameters()
		{
			UINT32 u;
			u = 0;
		}
	};
	class ASCDebugInfoListener : public ASCIEventListener
	{
	public:
		ASCDebugInfoListener();
		~ASCDebugInfoListener();

		void Initialise();

		void OnEvent(ASCIEvent* pEvent);
		
		bool DoClock();
		bool DoMemory();
		bool DoRnderering();
		bool DoStrings();
		bool DoINI();
		bool DoGUI();

	protected:
		ASCString m_strClock;
		ASCString m_strMemory;
		ASCString m_strRnderering;
		ASCString m_strStrings;
		ASCString m_strINI;
		ASCString m_strGUI;
		
		bool m_bClock;
		bool m_bMemory;
		bool m_bRnderering;
		bool m_bStrings;
		bool m_bINI;
		bool m_bGUI;

	};

	class Ascension
	{
	public:
		//Initialise
		static bool Initialise(SInitParameters& rInitParams);

		//Update
		static void Run();
		static bool Update();

		//General
		static void SetDelta(const FLOAT32 fDelta);
		static FLOAT32 Delta(bool bIgnorePause = false);
		static UINT32 Width();
		static UINT32 Height();
        
        static FLOAT32 ScaleX();
        static FLOAT32 ScaleY();

		//Systems
		static ASCRenderer& Renderer();
		static ASCINIManager& INI();
		static ASCDebuggingManager& Debugging();
		static ASCEventManager& EventSystem();
		static ASCSpriteManager& SpriteManager();
		static ASCPhysicsScene& Physics();
		static ASCAudioManager& Audio();
		static ASCGUIManager& GUI();
		static ASCCamera& Camera();
		static ASCVideoManager& Video();
        static ASCClock& Clock();
		static ASCGameContext& Context();
		static ASCGameContext& LoadingContext();

		static void RegisterCommand(ASCString& strCommand, ASCString& strDesc);
		static void RegisterCommand(CHAR8* strCommand, CHAR8* strDesc);
        
//#ifdef ASC_WINDOWS
		static ASCInputDevice& KeyBoard();
		static ASCInputDevice& Mouse();
//#endif
#ifdef ASC_IOS
        static ASCInputDevice& Touch();
#endif
        static EDevice Device();
        static void Device(EDevice eDevice);
        
		//static ASCProfilingSystem* Profiling();

		//Other
		static UINT32 GetNextInputEventID();
		static void DoDebugInfo();

		//De-Initialise
		static void DeInitialise();

		static void DebugToFile(ASCString strLine);
		
		static void RemoteLog(ASCString strPre);
		static void RemoteLog(ASCString strPre, UINT32 uVal, ASCString strPost = "");
		static void RemoteLog(ASCString strPre, FLOAT32 fVal, ASCString strPost = "");
		static void RemoteLog(ASCString strPre, SINT32 iVal, ASCString strPost = "");
		static void RemoteLog(ASCString strPre, void* pVal, ASCString strPost = "");
		static void RemoteLog(ASCString strPre, ASCString strVal, ASCString strPost = "");

		static bool Running();

		static void Pause(bool bPause);
        static void TimeScale(FLOAT32 fTimeScale);

	protected:
		static FLOAT32 sm_fDelta;
		static FLOAT32 sm_fTimeSacle;
		static FLOAT32 sm_fTimeBetweenRenders;
		static FLOAT32 sm_fDrawsPerFrame;
        static FLOAT32 sm_fRatioX;
        static FLOAT32 sm_fRatioY;
		
		static UINT32 sm_uWidth;
		static UINT32 sm_uHeight;
		static UINT32 sm_uInputEventCount;

		static ASCClock* sm_pClock;
		static ASCRenderer* sm_pRenderer;
		static ASCINIManager* sm_pINIManager;
		static ASCDebuggingManager* sm_pDebuggingManager;
		//static ASCProfilingSystem* sm_pProfilingSystem;
		static ASCEventManager* sm_pEventManager;
		static ASCGameContext* sm_pContext;
		static ASCGameContext* sm_pLoadingContext;
		static ASCSpriteManager* sm_pSpriteManager;
		static ASCKeyboardObserver* sm_pKeyboardObserver;
		static ASCPhysicsScene* sm_pPhysicsScene;
		static ASCAudioManager* sm_pAudioManager;
		static ASCGUIManager* sm_pGUIManager;
		static ASCCamera* sm_pCamera;
		static ASCVideoManager* sm_pVideoManager;
		static ASCRemoteLogging* sm_pRemoteLogging;
        
#ifdef ASC_WINDOWS
		static ASCInputDevice* sm_pKeyboard;
		static ASCInputDevice* sm_pMouse;
#endif
#ifdef ASC_IOS
        static ASCInputDevice* sm_pTouch;
#endif
        static EDevice sm_eDevice;

		static ASCVisualDebugging* sm_pVisualDebugger;

		static ASCDebugInfoListener* sm_pDebugInfoListener;

#ifdef ASC_DEBUG_TO_FILE
		static ASCString sm_strFileDebug;
#endif
		
		static bool sm_bShowMemoryInfo;
		static bool sm_bShowClockInfo;
		static bool sm_bShowDrawInfo;
		static bool sm_bRunning;
        static bool sm_bContextInitialised;
        static bool sm_bFirstFrame;
		static bool sm_bPaused;
	};

}

#endif //_ASCENSION_H__Included_819173460


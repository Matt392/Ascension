/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Input\ASCMouse.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCMOUSE_H__Included_1534658825
#define _ASCMOUSE_H__Included_1534658825

#ifdef ASC_WINDOWS

#define DIRECTINPUT_VERSION 0x0800

#include "ASCInputDevice.h"
#include <dinput.h>

namespace ASC
{
	const UINT32 m_kiRealInputCount = 11;

	class ASCMouse : public ASCInputDevice
	{
	public:
		ASCMouse(bool bShowMouse, bool bLockToWindow);
		~ASCMouse(void);

		virtual void Update( FLOAT32 fTimeDelta );

		virtual bool RegisterInput( UINT32& uEventID, ASCString strID );
		
		virtual void SetFullScreen(bool bVal);

		virtual void DisplayDebugInfo();

	protected:
		void ProcessMousePos();
		// Variables
		LPDIRECTINPUTDEVICE8  m_pDIDevice;			//!< Pointer to the Direct Input Device.
		LPDIRECTINPUT8 m_pDI;						//!< Pointer to the instance of Direct Input.
		DIMOUSESTATE2 m_MouseState;					//!< Structure containing mouse state information.

		SInputData m_sKeyData[m_kiInputCount];	//!< Array of key output data.
		UINT32 m_uEventIds[m_kiInputCount];
		bool m_bKeyObserved[m_kiInputCount];		//!< Lookup table for which keys are being observed.

		HWND m_hWnd;								//!< Instance of the game window.
		bool m_bFullScreen;
		bool m_bLockToWindow;

		SDoubleFloat m_dfMousePos;

		FLOAT32 m_fXOffset;
		FLOAT32 m_fYOffset;
	};

}

#endif

#endif //_ASCMOUSE_H__Included_1534658825


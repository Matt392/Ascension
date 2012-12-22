/**
*	@Ascension Engine
 *	@file Ascension 2D\Input\ASCKeyboard.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCKEYBOARD_H__Included_238289858
#define _ASCKEYBOARD_H__Included_238289858

#ifdef ASC_WINDOWS

#define DIRECTINPUT_VERSION 0x0800


#include "ASCInputDevice.h"
#include <dinput.h>

namespace ASC
{	
	static const UINT32 sk_uKeyCodes[sk_uKeyCount] = 
	{	
		DIK_ESCAPE,
		DIK_1,
		DIK_2,
		DIK_3,
		DIK_4,
		DIK_5,
		DIK_6,
		DIK_7,
		DIK_8,
		DIK_9,
		DIK_0,
		DIK_MINUS,
		DIK_EQUALS,
		DIK_BACK,
		DIK_TAB,
		DIK_Q,
		DIK_W,
		DIK_E,
		DIK_R,
		DIK_T,
		DIK_Y,
		DIK_U,
		DIK_I,
		DIK_O,
		DIK_P,
		DIK_LBRACKET,
		DIK_RBRACKET,
		DIK_RETURN,
		DIK_LCONTROL,
		DIK_A,
		DIK_S,
		DIK_D,
		DIK_F,
		DIK_G,
		DIK_H,
		DIK_J,
		DIK_K,
		DIK_L,
		DIK_SEMICOLON,
		DIK_APOSTROPHE,
		DIK_GRAVE,
		DIK_LSHIFT,
		DIK_BACKSLASH,
		DIK_Z,
		DIK_X,
		DIK_C,
		DIK_V,
		DIK_B,
		DIK_N,
		DIK_M,
		DIK_COMMA,
		DIK_PERIOD,
		DIK_SLASH,
		DIK_RSHIFT,
		DIK_MULTIPLY,
		DIK_LMENU,
		DIK_SPACE,
		DIK_CAPITAL,
		DIK_F1,
		DIK_F2,
		DIK_F3,
		DIK_F4,
		DIK_F5,
		DIK_F6,
		DIK_F7,
		DIK_F8,
		DIK_F9,
		DIK_F10,
		DIK_NUMLOCK,
		DIK_SCROLL,
		DIK_NUMPAD7,
		DIK_NUMPAD8,
		DIK_NUMPAD9,
		DIK_SUBTRACT,
		DIK_NUMPAD4,
		DIK_NUMPAD5,
		DIK_NUMPAD6,
		DIK_ADD,
		DIK_NUMPAD1,
		DIK_NUMPAD2,
		DIK_NUMPAD3,
		DIK_NUMPAD0,
		DIK_DECIMAL,
		DIK_OEM_102,
		DIK_F11,
		DIK_F12,
		DIK_F13,
		DIK_F14,
		DIK_F15,
		DIK_KANA,
		DIK_ABNT_C1,
		DIK_CONVERT,
		DIK_NOCONVERT,
		DIK_YEN,
		DIK_ABNT_C2,
		DIK_NUMPADEQUALS,
		DIK_PREVTRACK,
		DIK_AT,
		DIK_COLON,
		DIK_UNDERLINE,
		DIK_KANJI,
		DIK_STOP,
		DIK_AX,
		DIK_UNLABELED,
		DIK_NEXTTRACK,
		DIK_NUMPADENTER,
		DIK_RCONTROL,
		DIK_MUTE,
		DIK_CALCULATOR,
		DIK_PLAYPAUSE,
		DIK_MEDIASTOP,
		DIK_VOLUMEDOWN,
		DIK_VOLUMEUP,
		DIK_WEBHOME,
		DIK_NUMPADCOMMA,
		DIK_DIVIDE,
		DIK_SYSRQ,
		DIK_RMENU,
		DIK_PAUSE,
		DIK_HOME,
		DIK_UP,
		DIK_PRIOR,
		DIK_LEFT,
		DIK_RIGHT,
		DIK_END,
		DIK_DOWN,
		DIK_NEXT,
		DIK_INSERT,
		DIK_DELETE,
		DIK_LWIN,
		DIK_RWIN,
		DIK_APPS,
		DIK_POWER,
		DIK_SLEEP,
		DIK_WAKE,
		DIK_WEBSEARCH,
		DIK_WEBFAVORITES,
		DIK_WEBREFRESH,
		DIK_WEBSTOP,
		DIK_WEBFORWARD,
		DIK_WEBBACK,
		DIK_MYCOMPUTER,
		DIK_MAIL,
		DIK_MEDIASELECT 
	};

	class ASCKeyboard : public ASCInputDevice
	{
	public:
		ASCKeyboard(void);
		~ASCKeyboard(void);

		virtual void Update( FLOAT32 fTimeDelta );

		virtual bool RegisterInput( UINT32& uEventID, ASCString strID );

		virtual void SetFullScreen(bool bVal);

		virtual void DisplayDebugInfo();
		
	protected:
		// Functions
		void CreateLookupTable();

		// Variables
		LPDIRECTINPUTDEVICE8 m_pDIDevice;	//!< Pointer to the Direct Input Device.
		LPDIRECTINPUT8		 m_pDI;			//!< Pointer to the instance of Direct Input.
		char buffer[256];					//!< Char buffer used in copying values.
		bool m_bKeyObserved[sk_uKeyCount];		//!< Lookup table for which keys are being observed.
		bool m_bProcessDoublePress[sk_uKeyCount];
		UINT32 m_uEventIds[sk_uKeyCount];
		FLOAT32 m_fTimeOnState[sk_uKeyCount];
		FLOAT32 m_fTimeSinceLastPress[sk_uKeyCount];
		SInputData	m_sKeyData[sk_uKeyCount];		//!< Array of key output data.
	};

}

#endif

#endif //_ASCKEYBOARD_H__Included_238289858


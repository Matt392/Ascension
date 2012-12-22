/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCInputStructs.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCINPUTSTRUCTS_H__Included_558500470
#define _ASCINPUTSTRUCTS_H__Included_558500470


namespace ASC
{
	static const FLOAT32 sk_mTimeBetweenStates[KS_Count] = {	0.0f,
																0.05f,
																0.05f,
																0.0f,
																0.0f,	};

	static const FLOAT32 sk_fDoublePressGap = 0.2f;

	struct SInputData
	{
		EKeyState KeyState;		//!< Holds the key's current state.
		FLOAT32 FloatVal;		//!< FLOAT32 for holding key output.
		ASCString ID;
		FLOAT32 FloatVal2;		//!< Float for holding another output

		FLOAT32 m_fX;
		FLOAT32 m_fY;
	};

	static const EKeyState KS_TO_NEXT_KS_DOWN[KS_Count] = 
	{
		KS_Pressed,		//!< Key was inactive, is now pressed.
		KS_Held,		//!< Key was pressed, is now held down.
		KS_Held,
		KS_Held,		//!< Key was held, is continuing to be held.
		KS_Inactive		//!< Key was released, is now inactive.
	};

	static const EKeyState KS_TO_NEXT_KS_UP[KS_Count] = 
	{
		KS_Inactive,	//!< Key was released, is now inactive.
		KS_Held,		//!< Key was pressed, is now held.
		KS_Held,		//!< Key was pressed, is now held.
		KS_Released,	//!< Key was held, is now released.
		KS_Inactive		//!< Key was released, is now inactive.
	};

	static const bool KS_TO_BOOL[KS_Count] =
	{
		false,		//!< Key state returns false.
		true,		//!< Key state returns true.
		true,		//!< Key state returns true.
		true,		//!< Key state returns true.
		false		//!< Key state returns false.
	};
	
	static const UINT32 sk_uKeyCount = 144;

	static const UINT32 m_kiInputCount = 13;
    
    static const UINT32 m_kuTouchInputs = 5;
#define ASC_MAX_TOUCH_INPUT 5
}

#endif //_ASCINPUTSTRUCTS_H__Included_558500470


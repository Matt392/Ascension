/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Input\ASCKeyboardObserver.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCKEYBOARDOBSERVER_H__Included_343765170
#define _ASCKEYBOARDOBSERVER_H__Included_343765170

#include "ASCIEventListener.h"

namespace ASC
{
	static ASCString m_strKeyValues[] =
	{	
		"",			//"Key_Escape",
		"1",		//"Key_1",
		"2",		//"Key_2",
		"3",		//"Key_3",
		"4",		//"Key_4",
		"5",		//"Key_5",
		"6",		//"Key_6",
		"7",		//"Key_7",
		"8",		//"Key_8",
		"9",		//"Key_9",
		"0",		//"Key_0",
		"-",		//"Key_Minus",
		"	",		//"Key_Tab",
		"Q",		//"Key_Q",
		"W",		//"Key_W",
		"E",		//"Key_E",
		"R",		//"Key_R",
		"T",		//"Key_T",
		"Y",		//"Key_Y",
		"U",		//"Key_U",
		"I",		//"Key_I",
		"O",		//"Key_O",
		"P",		//"Key_P",
		"[",		//"Key_R_Bracket",
		"]",		//"Key_L_Bracket",
		"\\",		//"Key_Backslash",
		"A",		//"Key_A",
		"S",		//"Key_S",
		"D",		//"Key_D",
		"F",		//"Key_F",
		"G",		//"Key_G",
		"H",		//"Key_H",
		"J",		//"Key_J",
		"K",		//"Key_K",
		"L",		//"Key_L",
		";",		//"Key_Semi_Colon",
		"Z",		//"Key_Z",
		"X",		//"Key_X",
		"C",		//"Key_C",
		"V",		//"Key_V",
		"B",		//"Key_B",
		"N",		//"Key_N",
		"M",		//"Key_M",
		",",		//"Key_Comma",
		".",		//"Key_Period",
		"/",		//"Key_Slash",
		"*",		//"Key_Multiply",
		" ",		//"Key_Space",
		"-",		//"Key_Subract",
		"+",		//"Key_Add",
		".",		//"Key_Decimal",
		"=",		//"Key_NP_Equals",
		"'",		//"Key_Apostrophe",
		"`",		//"Key_Tilde",
		"/",		//"Key_Divide",
		"!",		//"Key_1",
		"@",		//"Key_2",
		"#",		//"Key_3",
		"$",		//"Key_4",
		"%",		//"Key_5",
		"^",		//"Key_6",
		"&",		//"Key_7",
		"*",		//"Key_8",
		"(",		//"Key_9",
		")",		//"Key_0",
		"_",		//"Key_Minus",
		"q",		//"Key_Q",
		"w",		//"Key_W",
		"e",		//"Key_E",
		"r",		//"Key_R",
		"t",		//"Key_T",
		"y",		//"Key_Y",
		"u",		//"Key_U",
		"i",		//"Key_I",
		"o",		//"Key_O",
		"p",		//"Key_P",
		"{",		//"Key_R_Bracket",
		"}",		//"Key_L_Bracket",
		"|",		//"Key_Backslash",
		"a",		//"Key_A",
		"s",		//"Key_S",
		"d",		//"Key_D",
		"f",		//"Key_F",
		"g",		//"Key_G",
		"h",		//"Key_H",
		"j",		//"Key_J",
		"k",		//"Key_K",
		"l",		//"Key_L",
		":",		//"Key_Semi_Colon",
		"z",		//"Key_Z",
		"x",		//"Key_X",
		"c",		//"Key_C",
		"v",		//"Key_V",
		"b",		//"Key_B",
		"n",		//"Key_N",
		"m",		//"Key_M",
		"<",		//"Key_Comma",
		">",		//"Key_Period",
		"?",		//"Key_Slash",
		"\"",		//"Key_Apostrophe",
		"~",		//"Key_Tilde",
	};


	enum EKeyCaseState
	{
		EKCS_LOWER,
		EKCS_UPPER,
		EKCS_SHIFT_LOWER,
		EKCS_SHIFT_UPPER,
		EKCS_COUNT,
	};

	static UINT32 m_uKeyValueIndex[sk_uKeyCount][EKCS_COUNT] =
	{
		{ 0 , 0 , 0 , 0 },		//"Key_Escape",
		{ 1 , 1 , 55 , 55 },		//"Key_1",
		{ 2 , 2 , 56 , 56 },		//"Key_2",
		{ 3 , 3 , 57 , 57 },		//"Key_3",
		{ 4 , 4 , 58 , 58 },		//"Key_4",
		{ 5 , 5 , 59 , 59 },		//"Key_5",
		{ 6 , 6 , 60 , 60 },		//"Key_6",
		{ 7 , 7 , 61 , 61 },		//"Key_7",
		{ 8 , 8 , 62 , 62 },		//"Key_8",
		{ 9 , 9 , 63 , 63 },		//"Key_9",
		{ 10 , 10 , 64 , 64 },		//"Key_0",
		{ 11 , 11 , 65 , 65 },		//"Key_Minus",
		{ 0 , 0 , 0 , 0 },		//"Key_Prev_Track",
		{ 0 , 0 , 0 , 0 },		//"Key_Backspace",
		{ 12 , 12 , 12, 12 },		//"Key_Tab",
		{ 66 , 13 , 13 , 66 },		//"Key_Q",
		{ 67 , 14 , 14 , 67 },		//"Key_W",
		{ 68 , 15 , 15 , 68 },		//"Key_E",
		{ 69 , 16 , 16 , 69 },		//"Key_R",
		{ 70 , 17 , 17 , 70 },		//"Key_T",
		{ 71 , 18 , 18 , 71 },		//"Key_Y",
		{ 72 , 19 , 19 , 72 },		//"Key_U",
		{ 73 , 20 , 20 , 73 },		//"Key_I",
		{ 74 , 21 , 21 , 74 },		//"Key_O",
		{ 75 , 22 , 22 , 75 },		//"Key_P",
		{ 24 , 24 , 77 , 77 },		//"Key_R_Bracket",
		{ 25 , 25 , 78 , 78 },		//"Key_Backslash",
		{ 0 , 0 , 0 , 0 },		//"Key_Enter",
		{ 0 , 0 , 0 , 0 },		//"Key_L_Control",
		{ 79 , 26 , 26 , 26 },		//"Key_A",
		{ 80 , 27 , 27 , 80 },		//"Key_S",
		{ 81 , 28 , 28 , 81 },		//"Key_D",
		{ 82 , 29 , 29 , 82 },		//"Key_F",
		{ 83 , 30 , 30 , 83 },		//"Key_G",
		{ 84 , 31 , 31 , 84 },		//"Key_H",
		{ 85 , 32 , 32 , 85 },		//"Key_J",
		{ 86 , 33 , 33 , 86 },		//"Key_K",
		{ 87 , 34 , 34 , 87 },		//"Key_L",
		{ 35 , 35 , 88 , 88 },		//"Key_Semi_Colon",
		{ 0 , 0 , 0 , 0 },		//"Key_Colon",
		{ 0 , 0 , 0 , 0 },		//"Key_Kanji",
		{ 0 , 0 , 0 , 0 },		//"Key_L_Shift",
		{ 0 , 0 , 0 , 0 },		//"Key_At",
		{ 89 , 36 , 36 , 89 },		//"Key_Z",
		{ 90 , 37 , 37 , 90 },		//"Key_X",
		{ 91 , 38 , 38 , 91 },		//"Key_C",
		{ 92 , 39 , 39 , 92 },		//"Key_V",
		{ 93 , 40 , 40 , 93 },		//"Key_B",
		{ 94 , 41 , 41 , 94 },		//"Key_N",
		{ 95 , 42 , 42 , 95 },		//"Key_M",
		{ 43 , 43 , 96 , 96 },		//"Key_Comma",
		{ 44 , 44 , 97 , 97 },		//"Key_Period",
		{ 45 , 45 , 98 , 98 },		//"Key_Slash",
		{ 0 , 0 , 0 , 0 },		//"Key_R_Shift",
		{ 46 , 46 , 46 , 46 },		//"Key_Multiply",
		{ 0 , 0 , 0 , 0 },		//"Key_L_Menu",
		{ 47 , 47 , 47 , 47 },		//"Key_Space",
		{ 0 , 0 , 0 , 0 },		//"Key_Caps",
		{ 0 , 0 , 0 , 0 },		//"Key_F1",
		{ 0 , 0 , 0 , 0 },		//"Key_F2",
		{ 0 , 0 , 0 , 0 },		//"Key_F3",
		{ 0 , 0 , 0 , 0 },		//"Key_F4",
		{ 0 , 0 , 0 , 0 },		//"Key_F5",
		{ 0 , 0 , 0 , 0 },		//"Key_F6",
		{ 0 , 0 , 0 , 0 },		//"Key_F7",
		{ 0 , 0 , 0 , 0 },		//"Key_F8",
		{ 0 , 0 , 0 , 0 },		//"Key_F9",
		{ 0 , 0 , 0 , 0 },		//"Key_F10",
		{ 0 , 0 , 0 , 0 },		//"Key_Num_Lock",
		{ 0 , 0 , 0 , 0 },		//"Key_Scroll",
		{ 7 , 7 , 7 , 7 },		//"Key_NP_7",
		{ 8 , 8 , 8 , 8 },		//"Key_NP_8",
		{ 9 , 9 , 9 , 9 },		//"Key_NP_9",
		{ 48 , 48 , 48 , 48 },		//"Key_Subract",
		{ 4 , 4 , 4 , 4 },		//"Key_NP_4",
		{ 5 , 5 , 5 , 5 },		//"Key_NP_5",
		{ 6 , 6 , 6 , 6 },		//"Key_NP_6",
		{ 49 , 49 , 49 , 49 },		//"Key_Add",
		{ 1 , 1 , 1 , 1 },		//"Key_NP_1",
		{ 2 , 2 , 2 , 2 },		//"Key_NP_2",
		{ 3 , 3 , 3 , 3 },		//"Key_NP_3",
		{ 10 , 10 , 10 , 10 },		//"Key_NP_0",
		{ 45 , 45 , 45 , 45 },		//"Key_Decimal",
		{ 0 , 0 , 0 , 0 },		//"Key_OEM_102",
		{ 0 , 0 , 0 , 0 },		//"Key_F11",
		{ 0 , 0 , 0 , 0 },		//"Key_F12",
		{ 0 , 0 , 0 , 0 },		//"Key_F13",
		{ 0 , 0 , 0 , 0 },		//"Key_F14",
		{ 0 , 0 , 0 , 0 },		//"Key_F15",
		{ 0 , 0 , 0 , 0 },		//"Key_Kana",
		{ 0 , 0 , 0 , 0 },		//"Key_Abnt_C1",
		{ 0 , 0 , 0 , 0 },		//"Key_Convert",
		{ 0 , 0 , 0 , 0 },		//"Key_NoConvert",
		{ 0 , 0 , 0 , 0 },		//"Key_Yen",
		{ 0 , 0 , 0 , 0 },		//"Key_Abnt_C2",
		{ 0 , 0 , 0 , 0 },		//"Key_NP_Equals",
		{ 51 , 51 , 51 , 51 },		//"Key_Equals",
		{ 23 , 23 , 76 , 76 },		//"Key_L_Bracket",
		{ 52 , 52 , 99 , 99 },		//"Key_Apostrophe",
		{ 0 , 0 , 0 , 0 },		//"Key_Underline",
		{ 0 , 0 , 0 , 0 },		//"Key_Tilde",
		{ 0 , 0 , 0 , 0 },		//"Key_Stop",
		{ 0 , 0 , 0 , 0 },		//"Key_Ax",
		{ 0 , 0 , 0 , 0 },		//"Key_Unlabeled",
		{ 0 , 0 , 0 , 0 },		//"Key_Next_Track",
		{ 0 , 0 , 0 , 0 },		//"Key_NP_Enter",
		{ 0 , 0 , 0 , 0 },		//"Key_R_Control",
		{ 0 , 0 , 0 , 0 },		//"Key_Mute",
		{ 0 , 0 , 0 , 0 },		//"Key_Calculator",
		{ 0 , 0 , 0 , 0 },		//"Key_Play_Pause",
		{ 0 , 0 , 0 , 0 },		//"Key_Media_Stop",
		{ 0 , 0 , 0 , 0 },		//"Key_Volume_Down",
		{ 0 , 0 , 0 , 0 },		//"Key_Volume_Up",
		{ 0 , 0 , 0 , 0 },		//"Key_Web_Home",
		{ 0 , 0 , 0 , 0 },		//"Key_NP_Comma",
		{ 55 , 55 , 55 , 55 },		//"Key_Divide",
		{ 0 , 0 , 0 , 0 },		//"Key_Print_Screen",
		{ 0 , 0 , 0 , 0 },		//"Key_Right_Menu",
		{ 0 , 0 , 0 , 0 },		//"Key_Pause",
		{ 0 , 0 , 0 , 0 },		//"Key_Home",
		{ 0 , 0 , 0 , 0 },		//"Key_Up",
		{ 0 , 0 , 0 , 0 },		//"Key_Page_Up",
		{ 0 , 0 , 0 , 0 },		//"Key_Left",
		{ 0 , 0 , 0 , 0 },		//"Key_Right",
		{ 0 , 0 , 0 , 0 },		//"Key_End",
		{ 0 , 0 , 0 , 0 },		//"Key_Down",
		{ 0 , 0 , 0 , 0 },		//"Key_Page_Down",
		{ 0 , 0 , 0 , 0 },		//"Key_Insert",
		{ 0 , 0 , 0 , 0 },		//"Key_Delete",
		{ 0 , 0 , 0 , 0 },		//"Key_L_Win",
		{ 0 , 0 , 0 , 0 },		//"Key_R_Win",
		{ 0 , 0 , 0 , 0 },		//"Key_Apps",
		{ 0 , 0 , 0 , 0 },		//"Key_Power",
		{ 0 , 0 , 0 , 0 },		//"Key_Sleep",
		{ 0 , 0 , 0 , 0 },		//"Key_Wake",
		{ 0 , 0 , 0 , 0 },		//"Key_Web_Search",
		{ 0 , 0 , 0 , 0 },		//"Key_Web_Favorites",
		{ 0 , 0 , 0 , 0 },		//"Key_Web_Refresh",
		{ 0 , 0 , 0 , 0 },		//"Key_Web_Stop",
		{ 0 , 0 , 0 , 0 },		//"Key_Web_Forward",
		{ 0 , 0 , 0 , 0 },		//"Key_Web_Back",
		{ 0 , 0 , 0 , 0 },		//"Key_My_Computer",
		{ 0 , 0 , 0 , 0 },		//"Key_Mail",
		{ 0 , 0 , 0 , 0 },		//"Key_Media_Select"
	};

	class ASCKeyboardObserver : public ASCIEventListener
	{
	public:
		ASCKeyboardObserver(void);
		~ASCKeyboardObserver(void);
		
		bool Init();

		virtual void OnEvent(ASCIEvent* pEvent);

	protected:

		UINT32 m_uKeys[sk_uKeyCount];

		UINT32 m_uLShift;
		UINT32 m_uRShift;
		UINT32 m_uCaps;

		bool m_bLShift;
		bool m_bRShift;
		bool m_bCaps;
	};

}

#endif //_ASCKEYBOARDOBSERVER_H__Included_343765170


/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCDebuggingManager.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCDEBUGINGMANAGER_H__Included_451057628
#define _ASCDEBUGINGMANAGER_H__Included_451057628

#include "ASCDebugString.h"


namespace ASC
{
	class ASCFont;
	class ASCOutputWindow;

	class ASCDebuggingManager
	{
	public:
		ASCDebuggingManager(bool bUse);
		~ASCDebuggingManager(void);

		void Initialise(UINT32 uBufferSize);

		ASCOutputWindow* OutputWindow();
		
		void AddScreenLine(ASCString strLine);
		void AddScreenLine(ASCString strPre, FLOAT32 fVal, ASCString strPost = "");
		void AddScreenLine(ASCString strPre, UINT32 uVal, ASCString strPost = "");
		void AddScreenLine(ASCString strPre, SINT32 iVal, ASCString strPost = "");
		void AddScreenLine(ASCString strPre, void* pVal, ASCString strPost = "");
		void AddScreenLine(ASCString strPre, ASCString strVal, ASCString strPost = "");

		ASCString* GetLineBuffer();
		void ClearLineBuffer();
		
		static void OutputWindowLine(ASCDebugString strLine);
		static void OutputWindowLine(ASCDebugString strPre, FLOAT32 fVal, ASCDebugString strPost = "\n");
		static void OutputWindowLine(ASCDebugString strPre, ASCDebugString strVal, ASCDebugString strPost = "\n");
		static void OutputWindowLine(ASCDebugString strPre, UINT32 uVal, ASCDebugString strPost = "\n");
		static void OutputWindowLine(ASCDebugString strPre, SINT32 iVal, ASCDebugString strPost = "\n");
		static void OutputWindowLine(ASCDebugString strPre, void* pVal, ASCDebugString strPost = "\n");

		void MaxLinesToDraw(UINT32 uVal);

	protected:
		bool m_bActive;
		
		ASCFont* m_pFont;

		ASCString m_strLines;

		FLOAT32 m_fLineGap;

		UINT32 m_uTextColour;

		UINT32 m_uMaxLinesToDraw;
		UINT32 m_uLineCount;

		UINT32 m_uOffset;
		
		ASCVector<ASCString> strFinal;
		ASCVector<UINT32> uLines;

		ASCOutputWindow* m_pOutputWindow;
	};

}

#endif //_ASCDEBUGINGMANAGER_H__Included_451057628


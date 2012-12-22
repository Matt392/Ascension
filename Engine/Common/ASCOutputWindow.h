/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCOutputWindow.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCOUTPUTWINDOW_H__Included_1237186107
#define _ASCOUTPUTWINDOW_H__Included_1237186107

#include "ASCIEventListener.h"

namespace ASC
{
#define ASC_OUTPUT_WINDOW_TEXTURE_SIZE_WIDTH 16
#define ASC_OUTPUT_WINDOW_TEXTURE_SIZE_HEIGHT 64
	
	class ASCTexture;
	class ASCVertexDeclaration;
	class ASCVertexBuffer;
	class ASCFont;

	class ASCOutputWindow : public ASCIEventListener
	{
	public:
		ASCOutputWindow(void);
		~ASCOutputWindow(void);

		void Initialise(UINT32 uBufferSize);

		void Switch(bool bVal);
		
		void AddLine(ASCString& strLine);
		void AddLine(CHAR8* strLine);
		
		void AddCommand(ASCString& strCommand, ASCString& strDesc);
		void AddCommand(CHAR8* strCommand, CHAR8* strDesc);

		void Render();

		virtual void OnEvent(ASCIEvent* pEvent);

	protected:
		ASCList<ASCString> m_lstLines;
		ASCTexture* m_pTexture;
		ASCVertexDeclaration* m_pVertDecl;
		ASCVertexBuffer* m_pRenderBuffer;
		ASCFont* m_pFont;
		SVertex m_sBacker[6];
		
		ASCString m_strInputLine;
		ASCString m_strPrevMessage;

		ASCVector<ASCString> m_vecCommands;

		bool m_bActive;
		
		UINT32 m_uEnter;
		UINT32 m_uBackspace;
		UINT32 m_uUpArrow;
		UINT32 m_uTilde;
		UINT32 m_uLeft;
		UINT32 m_uRight;

		UINT32 m_uIndex;

	};

}

#endif //_ASCOUTPUTWINDOW_H__Included_1237186107


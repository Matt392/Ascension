/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Printing Press\ASCFont.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCFONT_H__Included_1254560516
#define _ASCFONT_H__Included_1254560516

#define MORE_EFFICENT_FONT_RENDER
#define NON_DRAW_UP_FONT

namespace ASC
{	
	enum EFontJustification
	{
		FJ_Left,		
		FJ_Right,			
		FJ_Centre,		
	};
	struct SCharacter
	{
		char cCharacter;
		SIRect sRect;
		FLOAT32 fPixelsBeforeChar;
		FLOAT32 fPixelsAfterChar;
	};

	static const UINT32 m_kiMaxCharacters = 256;

	class ASCTexture;
	class ASCVertexDeclaration;
#ifdef NON_DRAW_UP_FONT
	class ASCVertexBuffer;
#endif

	class ASCFont
	{
	public:
		ASCFont( ASCString strFont );
		~ASCFont(void);
		
		bool Render( FLOAT32 fX, FLOAT32 fY, ASCString& strMessage, UINT32 iColour = 0xFFFFFFFF, bool bCentered = false, bool bRight = false, UINT32 iColour2 = 0xFFFFFFFF);

		UINT32 GetHeight();

		ASCString ID();

		FLOAT32 Width(ASCString& strString);

	private:
		void ProcessLetterLine( ASCString& strLine, UINT32 iCurCharacter );
		void ProcessImageLine( ASCString& strLine );

		SCharacter& GetCharacter(CHAR8 cChar);

		FLOAT32 GetStringWidth( ASCString& strMessage );
		FLOAT32 GetLineWidth( ASCString& strMessage, UINT32 uStart );

	protected:
		SCharacter* m_pCharacters;
		UINT32 m_iCharacterCount;
		ASCTexture* m_pFontTexture;
		ASCVertexDeclaration* m_pVertDecl;
		ASCVertexBuffer* m_pRenderBuffer;


		
		SINT32 m_uHighest;
		ASCString m_strID;

	};

}

#endif //_ASCFONT_H__Included_1254560516


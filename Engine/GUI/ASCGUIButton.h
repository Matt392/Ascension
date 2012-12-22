/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\GUI\ASCGUIButton.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCGUIBUTTON_H__Included_420006243
#define _ASCGUIBUTTON_H__Included_420006243


#ifdef ASC_NEW_GUI_SYSTEM

#include "ASCGUIElement.h"
#include "ASCGUIButtonElement.h"
#include "ASCGUIVisualElement.h"

#include "ASCTextureAtlas.h"

namespace ASC
{
	class ASCGUIButton : public ASCGUIElement, public ASCGUIVisualElement, public ASCGUIButtonElement
	{
	public:
		ASCGUIButton(void);
		~ASCGUIButton(void);
		
		bool Init(ASCString strNorm, ASCString strOver, ASCString strDown, FLOAT32 fPosX, FLOAT32 fPosY, UINT32 uGUIEventID);
		bool Init(ASCTexture* pTexture, UINT32 uGUIEventID, ASCString& strTechnique);

		void Update(FLOAT32 fPosX, FLOAT32 fPosY, bool bClick, bool bHeld);
		
		void SetSize(FLOAT32 fX, FLOAT32 fY);
		virtual void SetPos(FLOAT32 fX, FLOAT32 fY);
		virtual void IncPosition(FLOAT32 fX, FLOAT32 fY);

		void SetUVs();

		virtual void Disable();
		virtual void Enable();

	protected:
		SAtlasInfo m_sTexureInfo[3];
		UINT32 m_uStageIndex;
		ASCIEvent m_PressedEvent;
	};
}
#else

#include "ASCGUIElement.h"
#include "ASCTextureAtlas.h"
namespace ASC
{
	class ASCGUIButton : public ASCGUIElement
	{
	public:
		ASCGUIButton(void);
		~ASCGUIButton(void);
		
		bool Init(ASCString strNorm, ASCString strOver, ASCString strDown, FLOAT32 fPosX, FLOAT32 fPosY, UINT32 uGUIEventID);
		bool Init(ASCTexture* pTexture, UINT32 uGUIEventID, ASCString& strTechnique);

		void Update(FLOAT32 fPosX, FLOAT32 fPosY, bool bClick, bool bHeld);
		
		void SetSize(FLOAT32 fX, FLOAT32 fY);

		void SetDetectionScale(FLOAT32 fX, FLOAT32 fY);
		void SetDetectionOffset(FLOAT32 fX, FLOAT32 fY);

		void SetUVs();
		virtual void Disable();
		virtual void Enable();

	protected:
		SAtlasInfo m_sTexureInfo[3];

		UINT32 m_uStageIndex;

		ASCIEvent m_PressedEvent;
		
		FLOAT32 m_fDetectScaleX;
		FLOAT32 m_fDetectScaleY;
		
		FLOAT32 m_fDetectOffsetX;
		FLOAT32 m_fDetectOffsetY;
	};

}
#endif
#endif //_ASCGUIBUTTON_H__Included_420006243


#include "ASCGenInc.h"
#include "ASCGUIDebugElement.h"

#include "ASCGUIElement.h"
#include "ASCRenderer.h"
#include "ASCShaderManager.h"
#include "ASCShader.h"
#include "../Ascension/Ascension.h"
#include "ASCGUIManager.h"

ASCGUIDebugElement::ASCGUIDebugElement(void)
{
	Ascension::GUI().AddDebugElement(this);
}


ASCGUIDebugElement::~ASCGUIDebugElement(void)
{
	Ascension::GUI().RemoveDebugElement(this);
}

void 
ASCGUIDebugElement::DrawBounds()
{
	ASCGUIElement* pElement = reinterpret_cast<ASCGUIElement*>(this);
	if(pElement->IsActive())
	{
		FLOAT32 fPosX = pElement->PosX();
		FLOAT32 fPosY = pElement->PosY();
	
		FLOAT32 fHalfWidth = pElement->HalfWidth();
		FLOAT32 fHalfHeight = pElement->HalfHeight();

		static ASCShader* pShader = Ascension::Renderer().GetShaderManager()->LoadShader("Shaders/SpriteShaders");

		static ASC_Handle m_TechniqueHandle = pShader->GetNamedHandle("GUI_Bounds");

		pShader->SetTechnique(m_TechniqueHandle);
	
		SVertex sCorners[4];

		// top left
		sCorners[0].m_fX = ((fPosX-fHalfWidth) / Ascension::Width()) * 2.0f - 1.0f;
		sCorners[0].m_fY = (((fPosY-fHalfHeight) / Ascension::Height()) * 2.0f - 1.0f) * -1.0f;
						 
		// top right		 
		sCorners[1].m_fX = ((fPosX+fHalfWidth) / Ascension::Width()) * 2.0f - 1.0f;
		sCorners[1].m_fY = (((fPosY-fHalfHeight) / Ascension::Height()) * 2.0f - 1.0f) * -1.0f;
						 
		// bottom right		 
		sCorners[2].m_fX = ((fPosX+fHalfWidth) / Ascension::Width()) * 2.0f - 1.0f;
		sCorners[2].m_fY = (((fPosY+fHalfHeight) / Ascension::Height()) * 2.0f - 1.0f) * -1.0f;
						 
		// bottom left		 
		sCorners[3].m_fX = ((fPosX-fHalfWidth) / Ascension::Width()) * 2.0f - 1.0f;
		sCorners[3].m_fY = (((fPosY+fHalfHeight) / Ascension::Height()) * 2.0f - 1.0f) * -1.0f;

		SVertex sDrawVerts[6];
		sDrawVerts[0] = sCorners[0];
		sDrawVerts[1] = sCorners[1];
		sDrawVerts[2] = sCorners[2];
		sDrawVerts[3] = sCorners[0];
		sDrawVerts[4] = sCorners[2];
		sDrawVerts[5] = sCorners[3];

		Ascension::Renderer().ApplyDefaultVertDecl();

		pShader->Begin();

		Ascension::Renderer().DrawPrimitives( PT_TriList, 2, sDrawVerts, sizeof(SVertex));

		pShader->End();
	}
}
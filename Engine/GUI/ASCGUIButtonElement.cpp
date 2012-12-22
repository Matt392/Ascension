#include "ASCGenInc.h"
#include "ASCGUIButtonElement.h"


ASCGUIButtonElement::ASCGUIButtonElement(void)
: m_fDetectScaleX ( 1.0f )
, m_fDetectScaleY ( 1.0f )
, m_fDetectOffsetX ( 0.0f )
, m_fDetectOffsetY ( 0.0f )
{
}


ASCGUIButtonElement::~ASCGUIButtonElement(void)
{
}

bool 
ASCGUIButtonElement::WithinBounds(FLOAT32 fPointerPosX, FLOAT32 fPointerPosY, FLOAT32 fPosX, FLOAT32 fPosY, FLOAT32 fHalfWidth, FLOAT32 fHalfHeight, FLOAT32 fRotation)
{
#ifdef ASC_IOS
	FLOAT32 fHeight;
	if(Ascension::Device() == ED_IPHONE)
	{
		fHeight = Ascension::Width();
	}
	else if(Ascension::Device() == ED_IPAD)
	{
		fHeight = Ascension::Height();
	}
	static const FLOAT32 skfOffset = 13.0f;
	FLOAT32 fLeft = fPosX - (fHalfWidth * m_fDetectScaleX);
	FLOAT32 fRight = fPosX + (fHalfWidth * m_fDetectScaleX);
	FLOAT32 fTop = fHeight - (fPosY + (fHalfHeight * m_fDetectScaleY)) - skfOffset;
	FLOAT32 fBottom = fHeight - (fPosY - (fHalfHeight * m_fDetectScaleY)) - skfOffset;
	//if((fPointerPosX >= fLeft && fPointerPosX <= fRight) && (fPointerPosY >= fTop && fPointerPosY <= fBottom))
#else
	FLOAT32 fLeft = fPosX - (fHalfWidth * m_fDetectScaleX);
	FLOAT32 fRight = fPosX + (fHalfWidth * m_fDetectScaleX);
	FLOAT32 fTop = fPosY - (fHalfHeight * m_fDetectScaleY);
	FLOAT32 fBottom = fPosY + (fHalfHeight * m_fDetectScaleY);
#endif
	FLOAT32 fOffsetPosX = fPosX + m_fDetectOffsetX;
	FLOAT32 fOffsetPosY = fPosY + m_fDetectOffsetY;

	fLeft -= fOffsetPosX;
	fRight -= fOffsetPosX;
	fTop -= fOffsetPosY;
	fBottom -= fOffsetPosY;

	FLOAT32 fTempX = fPointerPosX - fOffsetPosX;
	FLOAT32 fTempY = fPointerPosY - fOffsetPosY;

	FLOAT32 fUseRot = -fRotation;

	fPointerPosX = (fTempX * cos(fUseRot)) - (fTempY * sin(fUseRot));
	fPointerPosY = (fTempY * cos(fUseRot)) + (fTempX * sin(fUseRot));

	if((fPointerPosX >= fLeft && fPointerPosX <= fRight) && (fPointerPosY >= fTop && fPointerPosY <= fBottom))
	{
		return true;
	}
	return false;
}
void 
ASCGUIButtonElement::SetDetectionScale(FLOAT32 fX, FLOAT32 fY)
{
	m_fDetectScaleX = fX;
	m_fDetectScaleY = fY;
}
void 
ASCGUIButtonElement::SetDetectionOffset(FLOAT32 fX, FLOAT32 fY)
{
	m_fDetectOffsetX = fX;
	m_fDetectOffsetY = fY;
}
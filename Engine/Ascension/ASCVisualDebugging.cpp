#include "ASCGenInc.h"
#include "ASCVisualDebugging.h"
#include "ASCFont.h"
#include "ASCProfilingSystem.h"
#include "Ascension.h"


ASCVisualDebugging::ASCVisualDebugging(void)
: m_pFont ( NULL )
, m_uTextColour ( 0xFFFF0000 )
, m_fLineGap ( 0.0f )
, m_uMaxLinesToDraw ( 0 )
, m_uOffset ( 0 )
{
#ifdef ASC_IOS
	m_pFont = new ASCFont("CourierNew.fnt");
#else
	m_pFont = new ASCFont("Fonts/CourierNew.fnt");

#endif
	m_fLineGap = SC_FLOAT(m_pFont->GetHeight());

	m_uMaxLinesToDraw = (Ascension::Height() / SC_UINT(m_fLineGap)) -1;
}


ASCVisualDebugging::~ASCVisualDebugging(void)
{
	SafeDelete( m_pFont );
}
void 
ASCVisualDebugging::Render()
{
	ASCString* pLines = Ascension::Debugging().GetLineBuffer();

#ifdef ASC_IOS
	m_pFont->Render(4.0f, 24.0f, *pLines, m_uTextColour);
#else
	m_pFont->Render(4.0f, 4.0f, *pLines, m_uTextColour);
#endif

	Ascension::Debugging().ClearLineBuffer();
}
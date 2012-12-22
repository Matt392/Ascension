#include "ASCGenInc.h"
#include "ASCKeyboardEvent.h"


ASCKeyboardEvent::ASCKeyboardEvent(void)
{
}


ASCKeyboardEvent::~ASCKeyboardEvent(void)
{
}

void 
ASCKeyboardEvent::SetCharVal(ASCString* strCharacter)
{
	m_pData = strCharacter;
}
ASCString 
ASCKeyboardEvent::GetCharVal()
{
	return *reinterpret_cast<ASCString*>(m_pData);
}
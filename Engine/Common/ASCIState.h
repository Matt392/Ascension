/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCIState.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCISTATE_H__Included_1632829330
#define _ASCISTATE_H__Included_1632829330


namespace ASC
{
	class ASCIState
	{
	public:
		virtual ~ASCIState()
		{
		}

		virtual bool Update() = 0;
		
		virtual bool ActivateState() = 0;
		virtual bool DeactivateState() = 0;
	};

}

#endif //_ASCISTATE_H__Included_1632829330


/**
*	@Danger Balls
 *	@file Ascension 2D\DumbyFolderCreator\ToolContext.h
 *	@brief 
 *	
 *********************************************************************************************/
#ifndef _TOOLCONTEXT_H__Included_459022931
#define _TOOLCONTEXT_H__Included_459022931

#include "ASCGameContext.h"

namespace ASC
{
	class CToolContext : public ASCGameContext
	{
	public:
		CToolContext(void);
		~CToolContext(void);

		//Creation
		bool Initialise();
		
		//Game logic
		bool Update();

		//Optional
		void Draw();
	};

}

#endif //_TOOLCONTEXT_H__Included_459022931


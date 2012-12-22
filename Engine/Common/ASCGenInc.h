/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCGenInc.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCGENINC_H__Included_436604324
#define _ASCGENINC_H__Included_436604324

#include "ASCGenTypes.h"
#include "ASCMacros.h"

namespace ASC
{
	#define ASC_NEW_GUI_SYSTEM
	#define ASC_NEW_INI_SYSTEM

	#ifdef NULL
		#undef NULL
	#endif

	#define NULL (0)

	#define NULL_ECSAPE if(NULL == this) {return;}
	#define NULL_ECSAPE_B if(NULL == this) {return false;}
	#define NULL_ECSAPE_U if(NULL == this) {return -1;}
	#define NULL_ECSAPE_PTR if(NULL == this) {return NULL;}

#ifdef ASC_RELEASE
	#define ASC_MAX_MEM_STORED 1
#else
	#define ASC_MAX_MEM_STORED 75000
#endif

#define ASC_MAX_TRACE_DEPTH 20

	#define ASC_MAX_UINT 4294967295
}

using namespace ASC;


enum EKeyState
{
	KS_Inactive,			//!< The key is inactive.
	KS_Pressed,				//!< The key is currently pressed.
	KS_DoublePressed,		//!< The key is currently double pressed.
	KS_Held,				//!< The key is continuously pressed.
	KS_Released,			//!< The key is no longer pressed.
	KS_Count				//!< Number of states, also used when invalid.
};

#include "ASCString.h"
#include "ASCVector.h"
#include "ASCList.h"
#include "ASCTree.h"
#include "ASCAssert.h"
#include "ASCDebuggingManager.h"
#include "ASCEventManager.h"
#include "ASCInputStructs.h"
#include "ASCVector2.h"
#include "ASCMatrix4.h"
#include "ASCGeneralFunctions.h"

#ifdef ASC_IOS
static ASCString strStringComp = "N3ASC9ASCStringE";
#else
static ASCString strStringComp = "class ASC::ASCString";
#endif

#endif //_ASCGENINC_H__Included_436604324


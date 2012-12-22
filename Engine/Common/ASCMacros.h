/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCMacros.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCMACROS_H__Included_1430714204
#define _ASCMACROS_H__Included_1430714204


template<typename T> void SafeDelete(T*& ptr)
{
	if(ptr)
	{
		delete ptr;
		ptr = 0;
	}
}

template<typename T> void SafeDeleteArray(T*& ptr)
{
	if(ptr)
	{
		delete [] ptr;
		ptr = 0;
	}
}
template<typename T> void SafeRelease(T*& ptr)
{
	if(ptr)
	{
		ptr->Release();
		ptr = 0;
	}
}
template<typename T> void SafeNew(T* ptr)
{
	if(0 == ptr)
	{
		ptr = new T();
	}
}

#define DeleteVector(vec)\
for(UINT32 i = 0; i < vec.size(); ++i)\
{\
	SafeDelete(vec[i]);\
}\
	
namespace ASC
{
	#define SC_UINT(x) static_cast<UINT32>(x)
	#define SC_SINT(x) static_cast<SINT32>(x)
	#define SC_FLOAT(x) static_cast<FLOAT32>(x)
	#define SC_CHAR(x) static_cast<CHAR8>(x)
	#define REINTERP(x,y) reinterpret_cast<x>(y)

	#define Create_Sprite_From_File(name) Ascension::Renderer().GetSpriteManager().CreateSpriteParser(name);
	#define Create_Sprite(name, tech) Ascension::Renderer().GetSpriteManager().CreateSprite(name, tech);

	//#define OUTPUT_WINDOW(message, info) Ascension::Debugging().OutputWindowLine(message, info)
	#define OUTPUT_WINDOW Ascension::Debugging().OutputWindowLine
}

#endif //_ASCMACROS_H__Included_1430714204


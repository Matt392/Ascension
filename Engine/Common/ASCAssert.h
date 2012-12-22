/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCAssert.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCASSERT_H__Included_1438548664
#define _ASCASSERT_H__Included_1438548664


class ASCAssert
{
public:
	static void AssertMessage(bool bAssert, char* strMsg);
	static void AssertMessageRelease(bool bAssert, char* strMsg);
};


#ifdef _DEBUG
#define assert_msg(bAssert, strMsg) ASCAssert::AssertMessage(bAssert, strMsg)
#define assert_msgex(bAssert, strMsg, strMsg2) ASCAssert::AssertMessage(bAssert, (ASCString(strMsg) + strMsg2).c_str())
#else
#ifdef ASC_RELEASE_ASSERTS
#define assert_msg(bAssert, strMsg) ASCAssert::AssertMessage(bAssert, strMsg)
#define assert_msgex(bAssert, strMsg, strMsg2) ASCAssert::AssertMessage(bAssert, (ASCString(strMsg) + strMsg2).c_str())
#else
#ifdef ASC_DEBUG_IOS
#define assert_msg(bAssert, strMsg) ASCAssert::AssertMessage(bAssert, strMsg)
#define assert_msgex(bAssert, strMsg, strMsg2) ASCAssert::AssertMessage(bAssert, (ASCString(strMsg) + strMsg2).c_str())
#else
#define assert_msg(bAssert, strMsg)
#define assert_msgex(bAssert, strMsg, strMsg2)
#endif
#endif
#endif
#define assert_now(msg) assert_msg(false, msg)
#define assert_nowex(msg, msg2) assert_msgex(false, msg, msg2)

#ifndef assert
#define assert(x) assert_msg(x, "Box2D Assert")
#endif

#endif //_ASCASSERT_H__Included_1438548664


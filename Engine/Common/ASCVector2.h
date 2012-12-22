/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASC2DVector.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASC2DVECTOR_H__Included_417426384
#define _ASC2DVECTOR_H__Included_417426384

//#include "..\Box2D_v2.2.0\Box2D\Box2D\Common\b2Math.h"

namespace ASC
{
	struct ASCVector2Values
	{
		FLOAT32 x;
		FLOAT32 y;
	};
	class ASCVector2
	{
	public:
		ASCVector2(void);
		ASCVector2(ASCVector2Values& vec);
		ASCVector2(ASCVector2& vec);
		ASCVector2(FLOAT32 fX, FLOAT32 fY);
		~ASCVector2(void);

		void SetZero();
		void Set(FLOAT32 fX, FLOAT32 fY);
		void SetX(FLOAT32 fX);
		void SetY(FLOAT32 fY);
		void X(FLOAT32 fX);
		void Y(FLOAT32 fY);
		FLOAT32 X();
		FLOAT32 Y();

		ASCVector2 operator -();
		
		void operator = (const ASCVector2Values& v);
		void operator = (const ASCVector2& v);
		bool operator == (const ASCVector2& v);

		void operator += (ASCVector2& v);
		void operator -= (ASCVector2& v);
		void operator *= (FLOAT32 fVal);
		
		ASCVector2 operator + (ASCVector2& v);
		ASCVector2 operator - (ASCVector2& v);
		ASCVector2 operator * (FLOAT32 fVal);

		FLOAT32 Length();
		FLOAT32 LengthSquared();
		FLOAT32 Normalize();
		bool IsValid();

	protected:
		ASCVector2Values m_v2Val;
	};

}

#endif //_ASC2DVECTOR_H__Included_417426384


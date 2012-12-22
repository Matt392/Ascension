#include "ASCGenInc.h"
#ifdef ASC_IOS
#include "b2Math.h"
#else
#include "..\Box2D_v2.2.0\Box2D\Box2D\Common\b2Math.h"
#endif
#include "ASCVector2.h"

#define TO_B2_VEC(x) (*reinterpret_cast<b2Vec2*>(&x))
#define TO_VEC_DATA(x) (*reinterpret_cast<ASCVector2Values*>(&x))

ASCVector2::ASCVector2(void)
{
}

ASCVector2::ASCVector2(ASCVector2Values& vec)
{
	m_v2Val.x = vec.x;
	m_v2Val.y = vec.y;
}
ASCVector2::ASCVector2(ASCVector2& vec)
{
	*this = vec;
}
ASCVector2::ASCVector2(FLOAT32 fX, FLOAT32 fY)
{
	m_v2Val.x = fX;
	m_v2Val.y = fY;
}

ASCVector2::~ASCVector2(void)
{
}

void 
ASCVector2::SetZero()
{
	m_v2Val.x = 0.0f;
	m_v2Val.y = 0.0f;
}
void 
ASCVector2::Set(FLOAT32 fX, FLOAT32 fY)
{
	m_v2Val.x = fX;
	m_v2Val.y = fY;
}
void 
ASCVector2::SetX(FLOAT32 fX)
{
	m_v2Val.x = fX;
}
void 
ASCVector2::SetY(FLOAT32 fY)
{
	m_v2Val.y = fY;
}
void 
ASCVector2::X(FLOAT32 fX)
{
	m_v2Val.x = fX;
}
void 
ASCVector2::Y(FLOAT32 fY)
{
	m_v2Val.y = fY;
}
FLOAT32 
ASCVector2::X()
{
	return m_v2Val.x;
}
FLOAT32 
ASCVector2::Y()
{
	return m_v2Val.y;
}

void 
ASCVector2::operator = (const ASCVector2Values& v)
{
	m_v2Val.x = v.x;
	m_v2Val.y = v.y;
}
void 
ASCVector2::operator = (const ASCVector2& v)
{
	m_v2Val.x = v.m_v2Val.x;
	m_v2Val.y = v.m_v2Val.y;
}bool 
ASCVector2::operator == (const ASCVector2& v)
{
	return (m_v2Val.x == v.m_v2Val.x) && (m_v2Val.y == v.m_v2Val.y);
}

ASCVector2 
ASCVector2::operator -()
{
	ASCVector2 v2Return(m_v2Val);
	b2Vec2 vec = -TO_B2_VEC(v2Return.m_v2Val);
	v2Return = TO_VEC_DATA(vec);
	return v2Return;
}
		
void 
ASCVector2::operator += (ASCVector2& v)
{
	TO_B2_VEC(m_v2Val) += TO_B2_VEC(v.m_v2Val);
}
void 
ASCVector2::operator -= (ASCVector2& v)
{
	TO_B2_VEC(m_v2Val) -= TO_B2_VEC(v.m_v2Val);
}
void 
ASCVector2::operator *= (FLOAT32 fVal)
{
	TO_B2_VEC(m_v2Val) *= fVal;
}
		
ASCVector2 
ASCVector2::operator + (ASCVector2& v)
{
	b2Vec2 v2Temp = TO_B2_VEC(m_v2Val);
	v2Temp += TO_B2_VEC(v.m_v2Val);

	ASCVector2 v2Return(TO_VEC_DATA(v2Temp));
	return v2Return;
}
ASCVector2 
ASCVector2::operator - (ASCVector2& v)
{
	b2Vec2 v2Temp = TO_B2_VEC(m_v2Val);
	v2Temp -= TO_B2_VEC(v.m_v2Val);

	ASCVector2 v2Return(TO_VEC_DATA(v2Temp));
	return v2Return;
}
ASCVector2 
ASCVector2::operator * (FLOAT32 fVal)
{
	b2Vec2 v2Temp = TO_B2_VEC(m_v2Val);
	v2Temp *= fVal;

	ASCVector2 v2Return(TO_VEC_DATA(v2Temp));
	return v2Return;
}

FLOAT32 
ASCVector2::Length() 
{
	return TO_B2_VEC(m_v2Val).Length();
}
FLOAT32 
ASCVector2::LengthSquared() 
{
	return TO_B2_VEC(m_v2Val).LengthSquared();
}
FLOAT32 
ASCVector2::Normalize()
{
	return TO_B2_VEC(m_v2Val).Normalize();
}
bool 
ASCVector2::IsValid() 
{
	return TO_B2_VEC(m_v2Val).IsValid();
}

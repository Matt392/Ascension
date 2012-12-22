/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Physics\ASCPhyBody.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCPHYBODY_H__Included_39078365
#define _ASCPHYBODY_H__Included_39078365

#include "Box2D.h"

namespace ASC
{

	class ASCPhyBody
	{
	public:
		ASCPhyBody(b2Body* pBody);
		~ASCPhyBody(void);
		
		void Release();
		void Activate();

	protected:
		b2Body* m_pBody;

		friend class ASCPhysicsObject;
		friend class ASCSphereEntity;
		friend class ASCBoxEntity;
		friend class ASCEntityGroup;
	};

	class ASCPhyPolygonShape
	{
	public:
		ASCPhyPolygonShape(void)
		{
		}
		~ASCPhyPolygonShape(void)
		{
		}

	protected:
		b2PolygonShape m_Shape;

		friend class ASCBoxEntity;
		friend class ASCEntityGroup;
	};

	class ASCPhySphereShape
	{
	public:
		ASCPhySphereShape(void)
		{
		}
		~ASCPhySphereShape(void)
		{
		}

	protected:
		b2CircleShape m_Shape;
		
		friend class ASCSphereEntity;
		friend class ASCEntityGroup;
	};

}

#endif //_ASCPHYBODY_H__Included_39078365


/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Physics\ASCBoxEntity.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCBOXENTITY_H__Included_1403685740
#define _ASCBOXENTITY_H__Included_1403685740

#include "ASCPhysicsObject.h"

namespace ASC
{
	class ASCPhyPolygonShape;

	class ASCBoxEntity : public ASCPhysicsObject
	{
	public:
		ASCBoxEntity(void);
		~ASCBoxEntity(void);
		
		bool Initialise(ASCString strSpriteName, bool bDynamic = true, ASCString strTechnique = "BasicSprite", SShapeSettings sTemplate = m_ksTemplate);
		bool InitialiseFile(ASCString strFileName);

	protected:
		ASCPhyPolygonShape* m_Shape;
	};

}

#endif //_ASCBOXENTITY_H__Included_1403685740


/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Physics\ASCSphereEntity.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCSPHEREENTITY_H__Included_562972468
#define _ASCSPHEREENTITY_H__Included_562972468

#include "ASCPhysicsObject.h"

namespace ASC
{
	class ASCPhySphereShape;

	class ASCSphereEntity : public ASCPhysicsObject
	{
	public:
		ASCSphereEntity(void);
		~ASCSphereEntity(void);

		bool Initialise(ASCString strSpriteName, bool bDynamic = true, ASCString strTechnique = "BasicSprite", SShapeSettings sTemplate = m_ksTemplate);
		bool InitialiseFile(ASCString strFileName);

	protected:
		ASCPhySphereShape* m_CircleShape;
	};

}

#endif //_ASCSPHEREENTITY_H__Included_562972468


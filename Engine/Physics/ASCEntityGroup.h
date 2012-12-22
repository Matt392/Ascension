/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Physics\ASCEntityGroup.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCENTITYGROUP_H__Included_1289482057
#define _ASCENTITYGROUP_H__Included_1289482057

#include "ASCPhysicsObject.h"

namespace ASC
{
	class ASCEntityGroup : public ASCPhysicsObject
	{
	public:
		ASCEntityGroup(void);
		~ASCEntityGroup(void);

		bool Initialise(ASCString strFile);

		virtual void UpdateToFile(ASCString strFile);

		virtual void SetCollisionFilter(UINT16 uCategory, UINT16 uMask, SINT16 sGroupIndex);

		virtual void Scale(FLOAT32 fVal);

		virtual void Sensor(bool bVal);

		virtual void OffsetY(FLOAT32 fVal);

	protected:
		ASCVector<void*> m_vecFixtures;
		ASCVector<FLOAT32> m_vecRadius;
		ASCVector<FLOAT32> m_vecXOffset;
		ASCVector<FLOAT32> m_vecYOffset;
		
		FLOAT32 m_fScale;
		FLOAT32 m_fOffsetY;
	};

}

#endif //_ASCENTITYGROUP_H__Included_1289482057


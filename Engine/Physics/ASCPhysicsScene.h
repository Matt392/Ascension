/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Physics\ASCPhysicsScene.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCPHYSICSSCENE_H__Included_268084837
#define _ASCPHYSICSSCENE_H__Included_268084837

#include "ASCPhysicsObject.h"

namespace ASC
{
	class ASCPhysicsWorld;
	class ASCPhysicsObject;
	class ASCEntityGroup;

	struct SPhyInitParam
	{
		FLOAT32 m_fGravityX;
		FLOAT32 m_fGravityY;


		SPhyInitParam()
		{
			m_fGravityX = 0.0f;
			m_fGravityY = 98.0f;
		}
	};

	class ASCPhysicsScene
	{
	public:
		ASCPhysicsScene(void);
		~ASCPhysicsScene(void);

		bool Init( SPhyInitParam rInitParams );

		void Update( FLOAT32 fDelta );

		ASCPhysicsWorld* GetWorld();

		void RegisterPhysicsObject(ASCPhysicsObject* pObject);
		void RemovePhysicsObject(ASCPhysicsObject* pObject);
		
		ASCPhysicsObject* CreateBox(ASCString strSpriteName, bool bDynamic = true, ASCString strTechnique = "BasicSprite", SShapeSettings sTemplate = m_ksTemplate);
		ASCPhysicsObject* CreateSphere(ASCString strSpriteName, bool bDynamic = true, ASCString strTechnique = "BasicSprite", SShapeSettings sTemplate = m_ksTemplate);
		
		ASCPhysicsObject* CreatePhyFromFile(ASCString strFileName);

	protected:
		ASCPhysicsWorld* m_pPhysicsWorld;
		UINT32 m_uEmptySlots;
		ASCVector<ASCPhysicsObject*> m_vecPhysicsObjects;
	};

}

#endif //_ASCPHYSICSSCENE_H__Included_268084837


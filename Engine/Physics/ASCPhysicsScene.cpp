#include "ASCGenInc.h"
#include "ASCPhysicsScene.h"
#include "ASCPhysicsWorld.h"
#include "ASCPhysicsObject.h"
#include "ASCEntityGroup.h"
#include "ASCSphereEntity.h"
#include "ASCBoxEntity.h"
#include "../Ascension/Ascension.h"
#include "ASCINIManager.h"

ASCPhysicsScene::ASCPhysicsScene(void)
: m_uEmptySlots ( 0 )
{
}


ASCPhysicsScene::~ASCPhysicsScene(void)
{
	for(UINT32 i = 0; i < m_vecPhysicsObjects.size(); ++i)
	{
		SafeDelete( m_vecPhysicsObjects[i] );
	}
	
	SafeDelete( m_pPhysicsWorld );
}

bool 
ASCPhysicsScene::Init( SPhyInitParam rInitParams )
{
	SPhyWorldInitParam WorldInit;
	WorldInit.m_fGravityX = rInitParams.m_fGravityX;
	WorldInit.m_fGravityY = rInitParams.m_fGravityY;
	m_pPhysicsWorld = new ASCPhysicsWorld();
	m_pPhysicsWorld->Init(WorldInit);

	return false;
}
void 
ASCPhysicsScene::Update( FLOAT32 fDelta )
{
	m_pPhysicsWorld->Update( fDelta );

	for(UINT32 i = 0; i < m_vecPhysicsObjects.size(); ++i)
	{
		if(m_vecPhysicsObjects[i])
		{
			m_vecPhysicsObjects[i]->Update();
		}
	}
}
void 
ASCPhysicsScene::RegisterPhysicsObject(ASCPhysicsObject* pObject)
{
	if(m_uEmptySlots > 0)
	{
		for(UINT32 i = 0; i < m_vecPhysicsObjects.size(); ++i)
		{
			if(NULL == m_vecPhysicsObjects[i])
			{
				m_vecPhysicsObjects[i] = pObject;
				--m_uEmptySlots;
				return;
			}
		}
		m_uEmptySlots = 0;
		m_vecPhysicsObjects.push_back( pObject );
	}
	else
	{
		m_vecPhysicsObjects.push_back( pObject );
	}
}
void 
ASCPhysicsScene::RemovePhysicsObject(ASCPhysicsObject* pObject)
{
	for(UINT32 i = 0; i < m_vecPhysicsObjects.size(); ++i)
	{
		if(pObject == m_vecPhysicsObjects[i])
		{
			m_vecPhysicsObjects[i] = NULL;
			++m_uEmptySlots;
			SafeDelete( pObject );
			break;
		}
	}
}
ASCPhysicsWorld* 
ASCPhysicsScene::GetWorld()
{
	return m_pPhysicsWorld;
}
ASCPhysicsObject* 
ASCPhysicsScene::CreateBox(ASCString strSpriteName, bool bDynamic, ASCString strTechnique, SShapeSettings sTemplate)
{
	assert_now("single box objects are depricated use .ascphy files");
	ASCBoxEntity* pBox = new ASCBoxEntity();
	pBox->Initialise(strSpriteName, bDynamic, strTechnique, sTemplate);
	return pBox;
}
ASCPhysicsObject* 
ASCPhysicsScene::CreateSphere(ASCString strSpriteName, bool bDynamic, ASCString strTechnique, SShapeSettings sTemplate)
{
	assert_now("single sphere objects are depricated use .ascphy files");
	ASCSphereEntity* pSphere = new ASCSphereEntity();
	pSphere->Initialise(strSpriteName, bDynamic, strTechnique, sTemplate);
	return pSphere;
}
ASCPhysicsObject* 
ASCPhysicsScene::CreatePhyFromFile(ASCString strFileName)
{
	ASCString strExtension;
	strFileName.substrc('.', strExtension);
	if(strExtension == ".ascphy")
	{
		assert_now(".ascphy are depricated");
		Ascension::INI().LoadFile(strFileName);
		ASCString strPhyType = Ascension::INI().GetString(strFileName, "General", "Type");

		if( strPhyType == "Box" )
		{
			ASCBoxEntity* pBox = new ASCBoxEntity();
			pBox->InitialiseFile(strFileName);
			return pBox;
		}
		else
		{
			ASCSphereEntity* pSphere = new ASCSphereEntity();
			pSphere->InitialiseFile(strFileName);
			return pSphere;
		}
	}
	else if(strExtension == ".ascphyg")
	{
		ASCEntityGroup* pGroup = new ASCEntityGroup();
		pGroup->Initialise(strFileName);

		return pGroup;
	}
	else
	{
		assert_now("Guts, Unknown file format");
		return NULL;
	}
}
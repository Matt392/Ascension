#include "ASCGenInc.h"
#include "ASCPhysicsObject.h"
#include "Box2D.h"
#include "ASCSprite.h"
#include "ASCPhyBody.h"
#include "ASCMaths.h"
#include "ASCPhysicsScene.h"
#include "ASCSpriteManager.h"

#include "../Ascension/Ascension.h"


ASCPhysicsObject::ASCPhysicsObject(void)
: m_pSprite ( NULL )
, m_pBody ( NULL )
, m_pUserData ( NULL )
, m_fRotation ( 0.0f )
, m_fWidth ( 0.0f )
, m_bLockRotation ( false )
{
	Ascension::Physics().RegisterPhysicsObject(this);
}


ASCPhysicsObject::~ASCPhysicsObject(void)
{
	m_pUserData = NULL;
	m_pBody->Release();
	SafeDelete(m_pBody);
}

void 
ASCPhysicsObject::SetPos(FLOAT32 fX, FLOAT32 fY)
{
	b2Vec2 vPos;
	vPos.x = fX;
	vPos.y = fY;
	m_pBody->m_pBody->SetTransform(vPos, m_fRotation);
}
void 
ASCPhysicsObject::IncPos(FLOAT32 fX, FLOAT32 fY)
{
	b2Vec2 vPos = m_pBody->m_pBody->GetPosition();
	vPos.x += fX;
	vPos.y += fY;
	m_pBody->m_pBody->SetTransform(vPos, m_fRotation);
}
void 
ASCPhysicsObject::SetX(FLOAT32 fX)
{
	b2Vec2 vPos = m_pBody->m_pBody->GetPosition();
	vPos.x = fX;
	m_pBody->m_pBody->SetTransform(vPos, m_fRotation);
}
void 
ASCPhysicsObject::SetY(FLOAT32 fY)
{
	b2Vec2 vPos = m_pBody->m_pBody->GetPosition();
	vPos.y = fY;
	m_pBody->m_pBody->SetTransform(vPos, m_fRotation);
}
void 
ASCPhysicsObject::SetLinearVel(FLOAT32 fX, FLOAT32 fY)
{
	b2Vec2 vPos;
	vPos.x = fX;
	vPos.y = fY;
	m_pBody->m_pBody->SetLinearVelocity(vPos);
}
FLOAT32 
ASCPhysicsObject::GetLinearVelX()
{
	return m_pBody->m_pBody->GetLinearVelocity().x;
}
FLOAT32 
ASCPhysicsObject::GetLinearVelY()
{
	return m_pBody->m_pBody->GetLinearVelocity().y;
}
void 
ASCPhysicsObject::SetRot(FLOAT32 fRot, bool bDegrees)
{
	if(bDegrees)
	{
		fRot = ASCMaths::DegToRad(fRot);
	}
	b2Vec2 vPos = m_pBody->m_pBody->GetPosition();
	m_fRotation = fRot;
	m_pBody->m_pBody->SetTransform(vPos, fRot);
}
void 
ASCPhysicsObject::Update()
{
	if(m_pBody)
	{
		b2Vec2 vPos = m_pBody->m_pBody->GetPosition();
		if(m_bLockRotation)
		{
			m_pBody->m_pBody->SetAngularVelocity(0.0f);
		}
		if(m_pSprite)
		{
			m_pSprite->SetRotation(m_pBody->m_pBody->GetAngle());
			m_pSprite->SetPosition(vPos.x, vPos.y);
		}
	}
}
void 
ASCPhysicsObject::Sensor(bool bVal)
{
	assert_now("Guts, Unsuported");
}
FLOAT32 
ASCPhysicsObject::GetX()
{
	b2Vec2 vPos = m_pBody->m_pBody->GetPosition();
	return vPos.x;
}
FLOAT32 
ASCPhysicsObject::GetY()
{
	b2Vec2 vPos = m_pBody->m_pBody->GetPosition();
	return vPos.y;
}
void 
ASCPhysicsObject::ResetTo(FLOAT32 fX, FLOAT32 fY, FLOAT32 fRot)
{
	b2Vec2 vPos;
	vPos.x = fX;
	vPos.y = fY;
	m_pBody->m_pBody->SetTransform(vPos, fRot);
	m_pBody->m_pBody->SetAwake(false);
	m_pBody->m_pBody->SetAwake(true);
}
ASCSprite* 
ASCPhysicsObject::GetSprite()
{
	return m_pSprite;
}
ASCAnimSprite* 
ASCPhysicsObject::GetAnimSprite()
{
	return m_pAnimSprite;
}
void 
ASCPhysicsObject::SetGravityScale(FLOAT32 fScale)
{
	m_pBody->m_pBody->SetGravityScale(fScale);
}
void 
ASCPhysicsObject::LockRotation(bool bVal)
{
	m_bLockRotation = bVal;
	m_pBody->m_pBody->SetFixedRotation(m_bLockRotation);
}
void 
ASCPhysicsObject::SetUserData(void* pData)
{
	m_pUserData = pData;
}
void* 
ASCPhysicsObject::GetUserData()
{
	return m_pUserData;
}
void 
ASCPhysicsObject::Release()
{
	m_v2StoredPos.Set(GetX(), GetY());
	SetPos(-99999999.0f, -99999999.0f);
	m_pBody->Release();
	if(m_pSprite)
	{
		m_pSprite->Release();
	}
}
void 
ASCPhysicsObject::Activate()
{
	m_pBody->Activate();
	if(m_pSprite)
	{
		m_pSprite->Create();
	}
	SetPos(m_v2StoredPos.X(), m_v2StoredPos.Y());
}
void 
ASCPhysicsObject::Remove()
{
	if(m_pSprite)
	{
		Ascension::SpriteManager().RemoveSprite( m_pSprite );
	}
	m_v2StoredPos.Set(GetX(), GetY());
	SetPos(-99999999.0f, -99999999.0f);
	Ascension::Physics().RemovePhysicsObject(this);
}
		
FLOAT32 
ASCPhysicsObject::DistToLeft()
{
	b2Vec2 bv2Center = m_pBody->m_pBody->GetLocalCenter();
	FLOAT32 fReturn = (m_fWidth * 0.5f) + bv2Center.x;
	if(bv2Center.x > 0.0f || bv2Center.x < 0.0f)
	{
		assert_now("Guts, Check this offset math");
	}
	return fReturn;
}
FLOAT32 
ASCPhysicsObject::DistToRight()
{
	b2Vec2 bv2Center = m_pBody->m_pBody->GetLocalCenter();
	FLOAT32 fReturn = (m_fWidth * 0.5f) + bv2Center.x;
	if(bv2Center.x > 0.0f || bv2Center.x < 0.0f)
	{
		assert_now("Guts, Check this offset math");
	}
	return fReturn;
}
FLOAT32 
ASCPhysicsObject::DistToTop()
{
	b2Vec2 bv2Center = m_pBody->m_pBody->GetLocalCenter();
	FLOAT32 fReturn = (m_fHeight * 0.5f) + bv2Center.y;
	//if(bv2Center.y > 0.0f || bv2Center.y < 0.0f)
	//{
	//	assert_now("Guts, Check this offset math");
	//}
	return fReturn;
}
FLOAT32 
ASCPhysicsObject::DistToBottom()
{
	b2Vec2 bv2Center = m_pBody->m_pBody->GetLocalCenter();
	FLOAT32 fReturn = (m_fHeight * 0.5f) + bv2Center.y;
	//if(bv2Center.y > 0.0f || bv2Center.y < 0.0f)
	//{
	//	assert_now("Guts, Check this offset math");
	//}
	return fReturn;
}
FLOAT32 
ASCPhysicsObject::Radius()
{
	b2Fixture* pFixture = m_pBody->m_pBody->GetFixtureList();
	b2Shape* pShape = pFixture->GetShape();
	return pShape->m_radius;
}
void 
ASCPhysicsObject::UpdateToFile(ASCString strFile)
{
	assert_now("Implement this in sub class");
}
void 
ASCPhysicsObject::SetCollisionFilter(UINT16 uCategory, UINT16 uMask, SINT16 sGroupIndex)
{
	uMask;
	uCategory;
	sGroupIndex;
	assert_now("Implement this in sub class");
}
void 
ASCPhysicsObject::Scale(FLOAT32 fVal)
{
	fVal;
	assert_now("Implement this in sub class");
}
void 
ASCPhysicsObject::OffsetY(FLOAT32 fVal)
{
	fVal;
	assert_now("Implement this in sub class");
}
#include "ASCGenInc.h"
#include "ASCSphereEntity.h"
#include "ASCSpriteManager.h"
#include "ASCSprite.h"
#include "ASCAnimSprite.h"
#include "Box2D.h"
#include "ASCPhyBody.h"
#include "ASCPhysicsScene.h"
#include "ASCPhysicsWorld.h"
#include "../Ascension/Ascension.h"
#include "ASCINIManager.h"

ASCSphereEntity::ASCSphereEntity(void)
{
}


ASCSphereEntity::~ASCSphereEntity(void)
{
	SafeDelete( m_CircleShape );
}

bool 
ASCSphereEntity::Initialise(ASCString strSpriteName, bool bDynamic, ASCString strTechnique, SShapeSettings sTemplate)
{
	ASCString strExtension;
	strSpriteName.substrc('.', strExtension);
	if(strExtension == ".ascspr")
	{
		m_pSprite = Ascension::SpriteManager().CreateSpriteFile(strSpriteName);
	}
	else if (strExtension == ".animspr")
	{
		m_pAnimSprite = Ascension::SpriteManager().CreateAnimSprite( strSpriteName ) ;
		m_pSprite = m_pAnimSprite;
	}
	else
	{
		m_pSprite = Ascension::SpriteManager().CreateSprite(strSpriteName, strTechnique);
	}

	m_CircleShape = new ASCPhySphereShape();
	m_CircleShape->m_Shape.m_radius = m_pSprite->GetWidth() * 0.5f;

	b2FixtureDef fd;

	fd.shape = &m_CircleShape->m_Shape;
	
	fd.density = sTemplate.m_fDensity;
	fd.friction = sTemplate.m_fFriction;
	fd.restitution = sTemplate.m_fRestitution;

	b2BodyDef bodyDef;
	if(bDynamic)
	{
		bodyDef.type = b2_dynamicBody;
	}
	bodyDef.position.Set(0.0f, 0.0f);


	m_pBody = new ASCPhyBody(Ascension::Physics().GetWorld()->GetBox2DWorld().CreateBody(&bodyDef));
	m_pBody->m_pBody->CreateFixture(&fd);

	m_pBody->m_pBody->SetUserData(this);

	return true;
}
bool 
ASCSphereEntity::InitialiseFile(ASCString strFileName)
{
	ASCString strSpriteName = Ascension::INI().GetString(strFileName, "General", "Sprite");
	ASCString strExtension;
	strSpriteName.substrc('.', strExtension);
	if(strExtension == ".ascspr")
	{
		m_pSprite = Ascension::SpriteManager().CreateSpriteFile(strSpriteName);
	}
	else if (strExtension == ".animspr")
	{
		m_pAnimSprite = Ascension::SpriteManager().CreateAnimSprite( strSpriteName ) ;
		m_pSprite = m_pAnimSprite;
	}
	else
	{
		m_pSprite = Ascension::SpriteManager().CreateSprite(strSpriteName);
	}

	m_pSprite->SetOffset(Ascension::INI().GetFloat(strFileName, "General", "OffsetX"), Ascension::INI().GetFloat(strFileName, "General", "OffsetY"));
	
	FLOAT32 fWidth = Ascension::INI().GetFloat(strFileName, "General", "Width");
	FLOAT32 fHeight = Ascension::INI().GetFloat(strFileName, "General", "Height");

    fWidth = fWidth;
    fHeight = fHeight;
    
	m_CircleShape = new ASCPhySphereShape();
	m_CircleShape->m_Shape.m_radius = m_pSprite->GetWidth() * 0.5f;


	b2FixtureDef fd;

	fd.shape = &m_CircleShape->m_Shape;
	
	fd.density = Ascension::INI().GetFloat(strFileName, "General", "Density");
	fd.friction = Ascension::INI().GetFloat(strFileName, "General", "Friction");
	fd.restitution = Ascension::INI().GetFloat(strFileName, "General", "Restitution");
	fd.isSensor = Ascension::INI().GetBool(strFileName, "General", "Sensor");

	b2BodyDef bodyDef;
	if(Ascension::INI().GetBool(strFileName, "General", "Dynamic"))
	{
		bodyDef.type = b2_dynamicBody;
	}
	bodyDef.position.Set(0.0f, 0.0f);


	m_pBody = new ASCPhyBody(Ascension::Physics().GetWorld()->GetBox2DWorld().CreateBody(&bodyDef));
	m_pBody->m_pBody->CreateFixture(&fd);

	m_pBody->m_pBody->SetUserData(this);
	return false;
}
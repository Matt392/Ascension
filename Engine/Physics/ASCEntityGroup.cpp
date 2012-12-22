#include "ASCGenInc.h"
#include "ASCEntityGroup.h"

#include "../Ascension/Ascension.h"
#include "ASCINIFile.h"
#include "ASCPhyBody.h"
#include "ASCPhysicsScene.h"
#include "ASCPhysicsWorld.h"

#include "ASCSprite.h"
#include "ASCAnimSprite.h"
#include "ASCSpriteManager.h"

ASCEntityGroup::ASCEntityGroup(void)
{
}


ASCEntityGroup::~ASCEntityGroup(void)
{
}

bool 
ASCEntityGroup::Initialise(ASCString strFile)
{
	ASCINIFile INIFile;
	if(INIFile.Load( strFile ))
	{
		m_fScale = 1.0f;
		m_fOffsetY = 0.0f;
	
		ASCString strSpriteName = INIFile.GetString( "General", "Sprite");
		ASCString strExtension;
		strSpriteName.substrc('.', strExtension);
		static ASCString s_strSpriteExt = ".ascspr";
		static ASCString s_strAnimSpriteExt = ".animspr";
		if(strExtension == s_strSpriteExt)
		{
			m_pSprite = Ascension::SpriteManager().CreateSpriteFile(strSpriteName);
		}
		else if (strExtension == s_strAnimSpriteExt)
		{
			m_pAnimSprite = Ascension::SpriteManager().CreateAnimSprite( strSpriteName ) ;
			m_pSprite = m_pAnimSprite;
		}
		else
		{
			m_pSprite = NULL;//Ascension::SpriteManager().CreateSprite(strSpriteName);
		}

		b2BodyDef bodyDef;
		if(INIFile.GetBool( "General", "Dynamic"))
		{
			bodyDef.type = b2_dynamicBody;
		}
		bodyDef.position.Set(0.0f, 0.0f);


		m_pBody = new ASCPhyBody(Ascension::Physics().GetWorld()->GetBox2DWorld().CreateBody(&bodyDef));

	
		FLOAT32 fDensity = INIFile.GetFloat( "General", "Density");
		FLOAT32 fFriction = INIFile.GetFloat( "General", "Friction");
		FLOAT32 fRestitution = INIFile.GetFloat( "General", "Restitution");
		bool bIsSensor = INIFile.GetBool( "General", "Sensor");

		UINT32 uSegmentCount = INIFile.GetUInt( "General", "SegmentCount");
		ASCString strSegment;
		ASCString strType;
	
		FLOAT32 m_fFurthestLeft = 0.0f;
		FLOAT32 m_fFurthestRight = 0.0f;
		FLOAT32 m_fFurthestUp = 0.0f;
		FLOAT32 m_fFurthestDown = 0.0f;

		FLOAT32 fDistTemp = 0.0f;

		for(UINT32 i = 0; i < uSegmentCount; ++i)
		{
			ASCPhySphereShape CircleShape;
			ASCPhyPolygonShape BoxShape;
			b2FixtureDef fd;

			strSegment = "Segment_";
			strSegment.AddSmallInt( i );

			strType = INIFile.GetString( strSegment, "Type");
			static ASCString s_strBox = "Box";
			static ASCString s_strCircle = "Circle";
			if(strType == s_strBox)
			{
				FLOAT32 fWidth = INIFile.GetFloat( strSegment, "Width");
				FLOAT32 fHeight = INIFile.GetFloat( strSegment, "Height");
				m_fWidth = fWidth;
				m_fHeight = fHeight;

				b2Vec2 bv2Pos;
				bv2Pos.x = INIFile.GetFloat( strSegment, "OffsetX");
				bv2Pos.y = INIFile.GetFloat( strSegment, "OffsetY");

				FLOAT32 fRotation = INIFile.GetFloat( strSegment, "Rotation");

				BoxShape.m_Shape.SetAsBox(fWidth * 0.5f, fHeight * 0.5f, bv2Pos, fRotation); 
				fd.shape = &BoxShape.m_Shape;
				m_vecRadius.push_back(fHeight * 0.5f);

				m_vecXOffset.push_back(bv2Pos.x);
				m_vecYOffset.push_back(bv2Pos.y);
			
				fDistTemp = (m_fWidth * 0.5f) + bv2Pos.x;
				if(fDistTemp > m_fFurthestLeft)
				{
					m_fFurthestLeft = fDistTemp;
				}
				fDistTemp = (m_fWidth * 0.5f) - bv2Pos.x;
				if(fDistTemp > m_fFurthestRight)
				{
					m_fFurthestRight = fDistTemp;
				}
			
				fDistTemp = (m_fHeight * 0.5f) + bv2Pos.y;
				if(fDistTemp > m_fFurthestUp)
				{
					m_fFurthestUp = fDistTemp;
				}
				fDistTemp = (m_fHeight * 0.5f) - bv2Pos.y;
				if(fDistTemp > m_fFurthestDown)
				{
					m_fFurthestDown = fDistTemp;
				}
			}
			else if(strType == s_strCircle)
			{
				CircleShape.m_Shape.m_radius = INIFile.GetFloat(strSegment, "Radius");
				CircleShape.m_Shape.m_p.Set(INIFile.GetFloat(strSegment, "OffsetX"), INIFile.GetFloat( strSegment, "OffsetY"));
				m_fWidth = CircleShape.m_Shape.m_radius * 2.0f;
				m_fHeight = CircleShape.m_Shape.m_radius * 2.0f;
			
				m_vecRadius.push_back(CircleShape.m_Shape.m_radius);
				m_vecXOffset.push_back(CircleShape.m_Shape.m_p.x);
				m_vecYOffset.push_back(CircleShape.m_Shape.m_p.y);

				fd.shape = &CircleShape.m_Shape;

				fDistTemp = (m_fWidth * 0.5f) + CircleShape.m_Shape.m_p.x;
				if(fDistTemp > m_fFurthestLeft)
				{
					m_fFurthestLeft = fDistTemp;
				}
				fDistTemp = (m_fWidth * 0.5f) - CircleShape.m_Shape.m_p.x;
				if(fDistTemp > m_fFurthestRight)
				{
					m_fFurthestRight = fDistTemp;
				}
			
				fDistTemp = (m_fHeight * 0.5f) + CircleShape.m_Shape.m_p.y;
				if(fDistTemp > m_fFurthestUp)
				{
					m_fFurthestUp = fDistTemp;
				}
				fDistTemp = (m_fHeight * 0.5f) - CircleShape.m_Shape.m_p.y;
				if(fDistTemp > m_fFurthestDown)
				{
					m_fFurthestDown = fDistTemp;
				}
			}
			else
			{
				assert_now("Guts, Unknown phy type");
			}

			fd.density = fDensity;
			fd.friction = fFriction;
			fd.restitution = fRestitution;
			fd.isSensor = bIsSensor;
			m_vecFixtures.push_back( m_pBody->m_pBody->CreateFixture(&fd) );
		}

		m_pBody->m_pBody->SetUserData(this);
	
		m_fWidth = m_fFurthestLeft + m_fFurthestRight;
		m_fHeight = m_fFurthestUp + m_fFurthestDown;

	

		SetCollisionFilter(ASC_PHY_GROUP_0, ASC_PHY_GROUP_0, 0);

		return true;
	}

	assert_now("Guts, Physics file failed to load");
	return false;
}
void 
ASCEntityGroup::UpdateToFile(ASCString strFile)
{
	ASCINIFile INIFile;
	if(INIFile.Load( strFile ))
	{
		for(UINT32 i = 0; i < m_vecFixtures.size(); ++i)
		{
			m_pBody->m_pBody->DestroyFixture(reinterpret_cast<b2Fixture*>(m_vecFixtures[i]));
		}
		m_vecFixtures.Clear();
		m_vecRadius.Clear();
		m_vecXOffset.Clear();
		m_vecYOffset.Clear();
	
		ASCString strSpriteName = INIFile.GetString( "General", "Sprite");
		ASCString strExtension;
		strSpriteName.substrc('.', strExtension);
		static ASCString s_strSpriteExt = ".ascspr";
		static ASCString s_strAnimSpriteExt = ".animspr";
		if(strExtension == s_strSpriteExt)
		{
			m_pSprite->FileReset(strSpriteName);
		}
		else if (strExtension == s_strAnimSpriteExt)
		{
			m_pAnimSprite->FileReset( strSpriteName ) ;
			m_pSprite = m_pAnimSprite;
		}
		else
		{
			m_pSprite = NULL;//Ascension::SpriteManager().CreateSprite(strSpriteName);
		}

		if(INIFile.GetBool( "General", "Dynamic"))
		{
			m_pBody->m_pBody->SetType( b2_dynamicBody );
		}
		else
		{
			m_pBody->m_pBody->SetType( b2_staticBody );
		}
		SetPos(0.0f, 0.0f);
		
	
		FLOAT32 m_fFurthestLeft = 0.0f;
		FLOAT32 m_fFurthestRight = 0.0f;
		FLOAT32 m_fFurthestUp = 0.0f;
		FLOAT32 m_fFurthestDown = 0.0f;

		FLOAT32 fDistTemp = 0.0f;
	
		FLOAT32 fDensity = INIFile.GetFloat( "General", "Density");
		FLOAT32 fFriction = INIFile.GetFloat( "General", "Friction");
		FLOAT32 fRestitution = INIFile.GetFloat( "General", "Restitution");
		bool bIsSensor = INIFile.GetBool( "General", "Sensor");

		UINT32 uSegmentCount = INIFile.GetUInt( "General", "SegmentCount");
		ASCString strSegment;
		ASCString strType;
		for(UINT32 i = 0; i < uSegmentCount; ++i)
		{
			ASCPhySphereShape CircleShape;
			ASCPhyPolygonShape BoxShape;
			b2FixtureDef fd;

			strSegment = "Segment_";
			strSegment.AddSmallInt( i );

			strType = INIFile.GetString( strSegment, "Type");
			static ASCString s_strBox = "Box";
			static ASCString s_strCircle = "Circle";
			if(strType == s_strBox)
			{
				FLOAT32 fWidth = INIFile.GetFloat( strSegment, "Width");
				FLOAT32 fHeight = INIFile.GetFloat( strSegment, "Height");
				m_fWidth = fWidth;
				m_fHeight = fHeight;

				b2Vec2 bv2Pos;
				bv2Pos.x = INIFile.GetFloat( strSegment, "OffsetX");
				bv2Pos.y = INIFile.GetFloat( strSegment, "OffsetY");

				FLOAT32 fRotation = INIFile.GetFloat( strSegment, "Rotation");
			
				m_vecRadius.push_back(fWidth * 0.5f);

				m_vecXOffset.push_back(bv2Pos.x);
				m_vecYOffset.push_back(bv2Pos.y);

				BoxShape.m_Shape.SetAsBox(fWidth * 0.5f, fHeight * 0.5f, bv2Pos, fRotation); 
				fd.shape = &BoxShape.m_Shape;
			
				fDistTemp = (m_fWidth * 0.5f) + bv2Pos.x;
				if(fDistTemp > m_fFurthestLeft)
				{
					m_fFurthestLeft = fDistTemp;
				}
				fDistTemp = (m_fWidth * 0.5f) - bv2Pos.x;
				if(fDistTemp > m_fFurthestRight)
				{
					m_fFurthestRight = fDistTemp;
				}
			
				fDistTemp = (m_fHeight * 0.5f) + bv2Pos.y;
				if(fDistTemp > m_fFurthestUp)
				{
					m_fFurthestUp = fDistTemp;
				}
				fDistTemp = (m_fHeight * 0.5f) - bv2Pos.y;
				if(fDistTemp > m_fFurthestDown)
				{
					m_fFurthestDown = fDistTemp;
				}
			}
			else if(strType == s_strCircle)
			{
				CircleShape.m_Shape.m_radius = INIFile.GetFloat( strSegment, "Radius");
				CircleShape.m_Shape.m_p.Set(INIFile.GetFloat( strSegment, "OffsetX"), INIFile.GetFloat( strSegment, "OffsetY"));
				m_fWidth = CircleShape.m_Shape.m_radius * 2.0f;
				m_fHeight = CircleShape.m_Shape.m_radius * 2.0f;
			
				m_vecRadius.push_back(CircleShape.m_Shape.m_radius);
				m_vecXOffset.push_back(CircleShape.m_Shape.m_p.x);
				m_vecYOffset.push_back(CircleShape.m_Shape.m_p.y);
			
				fd.shape = &CircleShape.m_Shape;
				

				fDistTemp = (m_fWidth * 0.5f) + CircleShape.m_Shape.m_p.x;
				if(fDistTemp > m_fFurthestLeft)
				{
					m_fFurthestLeft = fDistTemp;
				}
				fDistTemp = (m_fWidth * 0.5f) - CircleShape.m_Shape.m_p.x;
				if(fDistTemp > m_fFurthestRight)
				{
					m_fFurthestRight = fDistTemp;
				}
			
				fDistTemp = (m_fHeight * 0.5f) + CircleShape.m_Shape.m_p.y;
				if(fDistTemp > m_fFurthestUp)
				{
					m_fFurthestUp = fDistTemp;
				}
				fDistTemp = (m_fHeight * 0.5f) - CircleShape.m_Shape.m_p.y;
				if(fDistTemp > m_fFurthestDown)
				{
					m_fFurthestDown = fDistTemp;
				}
			}
			else
			{
				assert_now("Guts, Unknown phy type");
			}

			fd.density = fDensity;
			fd.friction = fFriction;
			fd.restitution = fRestitution;
			fd.isSensor = bIsSensor;

			m_vecFixtures.push_back( m_pBody->m_pBody->CreateFixture(&fd) );
		}

		m_pBody->m_pBody->SetUserData(this);
		m_fWidth = m_fFurthestLeft + m_fFurthestRight;
		m_fHeight = m_fFurthestUp + m_fFurthestDown;
		
		SetCollisionFilter(ASC_PHY_GROUP_0, ASC_PHY_GROUP_0, 0);
	}
}
void 
ASCEntityGroup::SetCollisionFilter(UINT16 uCategory, UINT16 uMask, SINT16 sGroupIndex)
{
	for(UINT32 i = 0; i < m_vecFixtures.size(); ++i)
	{
		b2Filter sFilter;
		b2Fixture* pFixture = reinterpret_cast<b2Fixture*>(m_vecFixtures[i]);

		sFilter.categoryBits = uCategory;
		sFilter.maskBits = uMask;
		sFilter.groupIndex = sGroupIndex;

		pFixture->SetFilterData( sFilter );
	}
}
void 
ASCEntityGroup::Sensor(bool bVal)
{
	for(UINT32 i = 0; i < m_vecFixtures.size(); ++i)
	{
		b2Fixture* pFixture = reinterpret_cast<b2Fixture*>(m_vecFixtures[i]);

		pFixture->SetSensor(bVal);
	}
}
void 
ASCEntityGroup::Scale(FLOAT32 fVal)
{
	m_fScale = fVal;
	for(UINT32 i = 0; i < m_vecFixtures.size(); ++i)
	{
		b2Fixture* pFixture = reinterpret_cast<b2Fixture*>(m_vecFixtures[i]);
		if(pFixture->GetType() == b2Shape::e_circle)
		{
			b2CircleShape* pShape = reinterpret_cast<b2CircleShape*>(pFixture->GetShape());
			pShape->m_radius = m_vecRadius[i] * m_fScale;
			pShape->m_p.x = m_vecXOffset[i] * m_fScale;
			pShape->m_p.y = (m_vecYOffset[i] * m_fScale) + m_fOffsetY;
		}
	}
}
void 
ASCEntityGroup::OffsetY(FLOAT32 fVal)
{
	m_fOffsetY = fVal;
	for(UINT32 i = 0; i < m_vecFixtures.size(); ++i)
	{
		b2Fixture* pFixture = reinterpret_cast<b2Fixture*>(m_vecFixtures[i]);
		if(pFixture->GetType() == b2Shape::e_circle)
		{
			b2CircleShape* pShape = reinterpret_cast<b2CircleShape*>(pFixture->GetShape());
			pShape->m_p.y = (m_vecYOffset[i] * m_fScale) + m_fOffsetY;
		}
	}
}
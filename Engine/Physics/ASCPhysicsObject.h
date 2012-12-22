/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Physics\ASCPhysicsObject.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCPHYSICSOBJECT_H__Included_1962035925
#define _ASCPHYSICSOBJECT_H__Included_1962035925


namespace ASC
{
#define ASC_DEFAULT_PHYSICS_GROUP 0x0001
	
#define ASC_PHY_GROUP_0 0x0001
#define ASC_PHY_GROUP_1 0x0002
#define ASC_PHY_GROUP_2 0x0004
#define ASC_PHY_GROUP_3 0x0008
#define ASC_PHY_GROUP_4 0x0010
#define ASC_PHY_GROUP_5 0x0020
#define ASC_PHY_GROUP_6 0x0040
#define ASC_PHY_GROUP_7 0x0080
#define ASC_PHY_GROUP_8 0x0100
#define ASC_PHY_GROUP_9 0x0200
#define ASC_PHY_GROUP_10 0x0400
#define ASC_PHY_GROUP_11 0x0800
#define ASC_PHY_GROUP_12 0x1000
#define ASC_PHY_GROUP_13 0x2000
#define ASC_PHY_GROUP_14 0x4000
#define ASC_PHY_GROUP_15 0x8000
	class ASCSprite;
	class ASCAnimSprite;
	class ASCPhyBody;
	

	struct SShapeSettings
	{
		FLOAT32 m_fFriction;
		FLOAT32 m_fRestitution;
		FLOAT32 m_fDensity;
		SShapeSettings()
		{
			m_fFriction = 0.3f;
			m_fRestitution = 0.0f;
			m_fDensity = 1.0f;
		}
	};
	
	const SShapeSettings m_ksTemplate;

	class ASCPhysicsObject
	{
	public:
		ASCPhysicsObject(void);
		virtual ~ASCPhysicsObject(void);

		void SetPos(FLOAT32 fX, FLOAT32 fY);
		void IncPos(FLOAT32 fX, FLOAT32 fY);
		FLOAT32 GetX();
		FLOAT32 GetY();
		void SetX(FLOAT32 fX);
		void SetY(FLOAT32 fY);
		void SetRot(FLOAT32 fRot, bool bDegrees = true);
		void Update();
		void ResetTo(FLOAT32 fX, FLOAT32 fY, FLOAT32 fRot);
		
		void SetLinearVel(FLOAT32 fX, FLOAT32 fY);
		FLOAT32 GetLinearVelX();
		FLOAT32 GetLinearVelY();

		ASCSprite* GetSprite();
		ASCAnimSprite* GetAnimSprite();

		void SetGravityScale(FLOAT32 fScale);

		void LockRotation(bool bVal);
		
		void SetUserData(void* pData);
		void* GetUserData();

		void Release();
		void Activate();

		void Remove();
		
		FLOAT32 DistToLeft();
		FLOAT32 DistToRight();
		FLOAT32 DistToTop();
		FLOAT32 DistToBottom();

		virtual void UpdateToFile(ASCString strFile);

		virtual void SetCollisionFilter(UINT16 uCategory, UINT16 uMask, SINT16 sGroupIndex);

		FLOAT32 Radius();

		virtual void Scale(FLOAT32 fVal);

		virtual void OffsetY(FLOAT32 fVal);

		virtual void Sensor(bool bVal);

	protected:
		ASCSprite* m_pSprite;
		ASCAnimSprite* m_pAnimSprite;

		ASCPhyBody* m_pBody;

		FLOAT32 m_fRotation;

		bool m_bLockRotation;

		void* m_pUserData;
		
		FLOAT32 m_fWidth;
		FLOAT32 m_fHeight;

		ASCVector2 m_v2StoredPos;
	};

}

#endif //_ASCPHYSICSOBJECT_H__Included_1962035925


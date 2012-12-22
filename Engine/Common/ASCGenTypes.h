/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCGenTypes.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCGENTYPES_H__Included_2000187322
#define _ASCGENTYPES_H__Included_2000187322


typedef float 			FLOAT32;
typedef double 			FLOAT64;

typedef unsigned long long int UINT64;
typedef long long signed int SINT64;

typedef unsigned int 	UINT32;
typedef signed int		SINT32;

typedef unsigned short	UINT16;
typedef signed short	SINT16;

typedef unsigned char	UINT8;
typedef signed char		SINT8;

typedef char CHAR8;
typedef wchar_t CHAR16;

typedef const void* ASC_Handle;

#define ASC_MAX_CHAR 256


struct SDNIRect
{
	SINT32 iLeft;
	SINT32 iRight;
	SINT32 iTop;
	SINT32 iBottom;
};

struct SDoubleFloat
{
	FLOAT32 m_fX;
	FLOAT32 m_fY;
	SDoubleFloat()
	{
		m_fX = 0.0f;
		m_fY = 0.0f;
	}
};

struct SIRect
{
	SINT32	m_iX;		
	SINT32	m_iY;		
	SINT32	m_iWidth;	
	SINT32	m_iHeight;	
	SIRect operator = (SDNIRect rect)
	{
		SIRect ret;
		ret.m_iX = rect.iLeft;
		ret.m_iY = rect.iRight;
		ret.m_iWidth = rect.iTop;
		ret.m_iHeight = rect.iBottom;
		return ret;
	}
};

#ifdef ASC_RENDERER_GLES2
struct SVertex
{
	FLOAT32 m_fX;//1
	FLOAT32	m_fY;//2
	FLOAT32 m_fZ;//3
	FLOAT32 m_fW;//4

	FLOAT32	m_uColour[4];//other
	FLOAT32	m_uColour2[4];
	FLOAT32	m_fTU;//5
	FLOAT32 m_fTV;//6
	
	FLOAT32 m_fPosX;//7
	FLOAT32	m_fPosY;//8
	FLOAT32 m_fRotation;//9
	
	FLOAT32 m_fScaleX;//10
	FLOAT32 m_fScaleY;//11

	SVertex()
	{
		m_uColour[0] = 1.0f;
		m_uColour[1] = 1.0f;
		m_uColour[2] = 1.0f;
		m_uColour[3] = 1.0f;
	
		m_fPosX = 0.0f;
		m_fPosY = 0.0f;
		m_fRotation = 0.0f;
		m_fScaleY = 1.0f;
		m_fScaleX = 1.0f;
	}
	void Init(FLOAT32 fX, FLOAT32 fY, FLOAT32 fZ, UINT32 uColour, FLOAT32 fTU, FLOAT32 fTV)
	{
		SetColour(uColour);
		m_fX = fX;
		m_fY = fY;
		m_fZ = fZ;
		m_fW = 10.0f;
		m_fTU = fTU;
		m_fTV = fTV;
	}
    /*
	void SetColour( UINT32 uCol )
	{
		UINT32* pCol = &uCol;
		CHAR8* cCol = reinterpret_cast<CHAR8*>(pCol);
        SINT32 sTemp = static_cast<SINT32>(cCol[3]);
		m_uColour[0] = static_cast<FLOAT32>(cCol[3]);
        m_uColour[0] /= 255.0f;
		m_uColour[1] = static_cast<FLOAT32>(cCol[2]) / 255.0f;
		m_uColour[2] = static_cast<FLOAT32>(cCol[1]) / 255.0f;
		m_uColour[3] = static_cast<FLOAT32>(cCol[0]) / 255.0f;
	}
     */
	void SetColour( UINT32 uCol )
	{
		UINT32 uA = uCol >> 24;
		UINT32 uR = uCol >> 16 & 0xff;
		UINT32 uG = uCol >> 8 & 0xff;
		UINT32 uB = uCol & 0xff;
        
		m_uColour[0] = static_cast<FLOAT32>(uR) / 255.0f;
		m_uColour[1] = static_cast<FLOAT32>(uG) / 255.0f;
		m_uColour[2] = static_cast<FLOAT32>(uB) / 255.0f;
		m_uColour[3] = static_cast<FLOAT32>(uA) / 255.0f;
	}
	void SetSecondColour( UINT32 uCol )
	{
		UINT32 uA = uCol >> 24;
		UINT32 uR = uCol >> 16 & 0xff;
		UINT32 uG = uCol >> 8 & 0xff;
		UINT32 uB = uCol & 0xff;
        
		m_uColour2[0] = static_cast<FLOAT32>(uR) / 255.0f;
		m_uColour2[1] = static_cast<FLOAT32>(uG) / 255.0f;
		m_uColour2[2] = static_cast<FLOAT32>(uB) / 255.0f;
		m_uColour2[3] = static_cast<FLOAT32>(uA) / 255.0f;
	}
	void Reset()
	{
		m_fZ = -1000.0f;
	}
};
#else
struct SVertex
{
	FLOAT32 m_fX;//1
	FLOAT32	m_fY;//2
	FLOAT32 m_fZ;//3
	FLOAT32 m_fW;//4
	
	UINT32	m_uColour;//other	
	UINT32	m_uColour2;//other	
	FLOAT32	m_fTU;//5
	FLOAT32 m_fTV;//6
	
	FLOAT32 m_fPosX;//7
	FLOAT32	m_fPosY;//8
	FLOAT32 m_fRotation;//9
	
	FLOAT32 m_fScaleX;//10
	FLOAT32 m_fScaleY;//11

	FLOAT32 m_uTextureIndex;//12

	SVertex()
	{
		m_uColour = 0xFFFFFFFF;
		m_uColour2 = 0x00000000;
		
		m_fX = 0.0f;//1
		m_fY = 0.0f;//2
		m_fZ = 0.0f;//3
		m_fW = 1.0f;//4
		
		m_fTU = 0.0f;
		m_fTV = 0.0f;
	
		m_fPosX = 0.0f;
		m_fPosY = 0.0f;
		m_fRotation = 0.0f;
		m_fScaleY = 1.0f;
		m_fScaleX = 1.0f;

		m_uTextureIndex = 0;
	}
	void Init(FLOAT32 fX, FLOAT32 fY, FLOAT32 fZ, UINT32 uColour, FLOAT32 fTU, FLOAT32 fTV)
	{
		m_fX = fX;
		m_fY = fY;
		m_fZ = fZ;
		m_fW = 10.0f;
		m_uColour = uColour;	
		m_fTU = fTU;
		m_fTV = fTV;
	}
	void TextureIndex(UINT32 uIndex)
	{
		m_uTextureIndex = static_cast<FLOAT32>( uIndex);
	}
	void SetColour( UINT32 uCol )
	{
		m_uColour = uCol;
	}
	void SetSecondColour( UINT32 uCol )
	{
		m_uColour2 = uCol;
	}
	void Reset()
	{
		m_fZ = -1000.0f;
	}
};
#endif

struct SFontVertex
{
	FLOAT32 m_fX;
	FLOAT32	m_fY;
	FLOAT32 m_fZ;
	FLOAT32 m_fPad;
	UINT32	m_uColour;	
	FLOAT32	m_fTU;
	FLOAT32 m_fTV; 
	SFontVertex()
	{
		m_fPad = 0.0f;
		m_uColour = 0xFFFFFFFF;
	}
	void Init(FLOAT32 fX, FLOAT32 fY, FLOAT32 fZ, UINT32 uColour, FLOAT32 fTU, FLOAT32 fTV)
	{
		m_fX = fX;
		m_fY = fY;
		m_fZ = fZ;	
		m_uColour = uColour;	
		m_fTU = fTU;
		m_fTV = fTV;
	}
};

#endif //_ASCGENTYPES_H__Included_2000187322


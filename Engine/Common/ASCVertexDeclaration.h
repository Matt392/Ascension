/**
*	@Ascension Engine
 *	@file Ascension 2D\Engine\Common\ASCVertexDeclaration.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASCVERTEXDECLARATION_H__Included_990995041
#define _ASCVERTEXDECLARATION_H__Included_990995041


namespace ASC
{
	enum EType
	{
		ET_FLOAT32 = 0,	//!< A 32Bit floating point value
		ET_FLOAT64,		//!< A 64Bit float
		ET_SINT8,		//!< signed 8 bit integer
		ET_SINT16,		//!< signed 16 bit integer
		ET_SINT32,		//!< signed 32 bit integer
		ET_UINT8,		//!< unsigned 8 bit integer
		ET_UINT16,		//!< unsigned 16 bit integer
		ET_UINT32,		//!< unsigned 32 bit integer
		ET_NONE,		//!< This means there is nothing there
	};	
	enum ESemantic	
	{
		ES_NONE = 0,			//!< This data has no meaning!
		ES_POSITION,			//!< This is a position
		ES_POSITIONT,			//!< This is a transformed position
		ES_NORMAL,				//!< This is a normal.
		ES_COLOUR,				//!< This is a colour.
		ES_TEXTURE,				//!< This is a texture.
		ES_TANGENT,				//!< Tangental normal, for basis vectors.
		ES_BINORMAL,			//!< Binormal normal, for basis vectors.
		ES_BLENDINDICES,
	};
	struct SVertElement
	{
		ESemantic		m_eSemantic	: 7;//!< What does this data represent?
		EType			m_eType		: 7;//!< The type of the data in the stream.
		UINT32			m_uIndex	: 4;//!< What is the index for this element (texture coord set for example).
		UINT32			m_uOffset	: 6;//!< Byte offset from the start of the vertex.
		UINT32			m_uCount	: 4;//!< How many elements are there?
		UINT32			m_uStream	: 4;//!< What stream are we pointing into?
		static SVertElement Init(UINT32 uOffset, ESemantic eSemantic, UINT32 uIndex, EType eType, UINT32 uCount, UINT32 uStream = 0)
		{
			SVertElement sNew;
			sNew.m_eSemantic = eSemantic;
			sNew.m_eType = eType;
			sNew.m_uIndex = uIndex;
			sNew.m_uOffset = uOffset;
			sNew.m_uCount = uCount;
			sNew.m_uStream = uStream;
			return sNew;
		}
	};

	class ASCVertexDeclaration
	{
	public:
		virtual ~ASCVertexDeclaration()
		{
		}
		virtual bool Apply() = 0;
		virtual UINT32 GetStride() = 0;

	protected:
		static const UINT32 m_kiMaxElements = 8;
	};
	
	typedef SVertElement VertElement;
}

#endif //_ASCVERTEXDECLARATION_H__Included_990995041


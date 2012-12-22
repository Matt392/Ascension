/**
*	@Ascension Engine
 *	@file Ascension 2D\Projects\Game Of Death\Game Of Death\Field.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _FIELD_H__Included_303710823
#define _FIELD_H__Included_303710823

#include "GameOfDeathInc.h"

namespace ASC
{

	class ASCGUIStaticImage;
	class ASCTexture;

	class CBase;
	class CGameOfLifePattern;

	struct SField
	{
		UINT32** m_pField;
		UINT32** m_pColour;
		UINT32** m_pNeighbours;
		bool** m_pActivate;
		
		UINT32 m_uWidth;
		UINT32 m_uHeight;

		SField(UINT32 uWidth, UINT32 uHeight)
		{
			m_uWidth = uWidth;
			m_uHeight = uHeight;

			m_pField = new UINT32*[m_uHeight];
			m_pColour = new UINT32*[m_uHeight];
			m_pNeighbours = new UINT32*[m_uHeight];
			
			m_pActivate = new bool*[m_uHeight];

			for(UINT32 y = 0; y < m_uHeight; ++y)
			{
				m_pField[y] = new UINT32[m_uWidth];
				m_pColour[y] = new UINT32[m_uWidth];
				m_pNeighbours[y] = new UINT32[m_uWidth];
				m_pActivate[y] = new bool[m_uWidth];
			
				memset(m_pField[y], 0, sizeof(UINT32)*m_uWidth);
				memset(m_pColour[y], 0, sizeof(UINT32)*m_uWidth);
				memset(m_pNeighbours[y], 0, sizeof(UINT32)*m_uWidth);
				memset(m_pActivate[y], false, sizeof(bool)*m_uWidth);
			}
		}
		~SField()
		{
			for(UINT32 y = 0; y < m_uHeight; ++y)
			{
				SafeDeleteArray( m_pField[y] );
				SafeDeleteArray( m_pColour[y] );
				SafeDeleteArray( m_pNeighbours[y] );
				SafeDeleteArray( m_pActivate[y] );
			}
			SafeDeleteArray( m_pField );
			SafeDeleteArray( m_pColour );
			SafeDeleteArray( m_pNeighbours );
			SafeDeleteArray( m_pActivate );
		}
		void Clear()
		{
			for(UINT32 y = 0; y < m_uHeight; ++y)
			{			
				memset(m_pField[y], 0, sizeof(UINT32)*m_uWidth);
				memset(m_pColour[y], 0, sizeof(UINT32)*m_uWidth);
				memset(m_pNeighbours[y], 0, sizeof(UINT32)*m_uWidth);
				memset(m_pActivate[y], false, sizeof(bool)*m_uWidth);
			}
		}
	};

	class CField
	{
	public:
		CField(void);
		~CField(void);

		bool Initialise();

		bool Update();

		void UpdateRules();
		inline void CheckCell(UINT32 uX, UINT32 uY, UINT32 uTX, UINT32 uTY);
		inline void CheckCellBase(UINT32 uX, UINT32 uY, UINT32 uTX, UINT32 uTY);
		inline void CheckCellPatternImpact(UINT32 uX, UINT32 uY, UINT32 uTX, UINT32 uTY);
		void UpdateField();

		void MoveElements();

		void FillField(CBase* pBase);
		void FillWithAttackPattern(CGameOfLifePattern* pPattern, SCoordinates sCoords);

		UINT32 GetNextID();

		void Activate();
		void Deactivcate();

		UINT32 BaseTargets();
		bool BaseChanged();
		bool AnyChange();

	protected:
		ASCTexture* m_pFieldTexture;
		ASCGUIStaticImage* m_pFieldDisplay;
		
		FLOAT32 m_fInterval;
		FLOAT32 m_fRuleInterval;
		
		UINT32 m_uWidth;
		UINT32 m_uHeight;

		UINT32 m_uNextID;

		UINT32 m_uBaseTargets;
		bool m_bBaseChanged;

		bool m_bAnyChange;
		
		SField* m_sField;
		SField* m_sNewField;
	};

}

#endif //_FIELD_H__Included_303710823


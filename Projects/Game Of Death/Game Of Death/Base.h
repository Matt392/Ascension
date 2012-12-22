/**
*	@Ascension Engine
 *	@file Ascension 2D\Projects\Game Of Death\Game Of Death\Base.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _BASE_H__Included_754290180
#define _BASE_H__Included_754290180


namespace ASC
{
	class CBuilding;

	class CBase
	{
	public:
		CBase(void);
		~CBase(void);

		bool Initialise(bool bTop);

		void DrawOnField(UINT32* pTexels, UINT32 uPitch);

	protected:
		ASCVector<CBuilding*> m_vecBuildings;
	};

}

#endif //_BASE_H__Included_754290180


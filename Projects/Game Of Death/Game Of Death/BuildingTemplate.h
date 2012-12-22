/**
*	@Ascension Engine
 *	@file Ascension 2D\Projects\Game Of Death\Game Of Death\BuildingTemplate.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _BUILDINGTEMPLATE_H__Included_1545068265
#define _BUILDINGTEMPLATE_H__Included_1545068265

#include "GameOfDeathInc.h"

namespace ASC
{
	class CBuildingTemplate
	{
	public:
		CBuildingTemplate(void);
		~CBuildingTemplate(void);

		bool Initialise( ASCString& strFile, UINT32* pLookUp );

		SPatternTemplate& Template();

	protected:
		SPatternTemplate m_sPattern;

	};

}

#endif //_BUILDINGTEMPLATE_H__Included_1545068265


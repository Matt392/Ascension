/**
*	@Ascension Engine
 *	@file Ascension 2D\Projects\Game Of Death\Game Of Death\Templates.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2011 Matt Dalzell. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _TEMPLATES_H__Included_2051077847
#define _TEMPLATES_H__Included_2051077847


namespace ASC
{
	class CBuildingTemplate;

	class CTemplates
	{
	public:
		CTemplates(void);
		~CTemplates(void);

		bool Initialise();

		ASCVector<CBuildingTemplate*>& BuildingTemplates();

	protected:
		ASCVector<CBuildingTemplate*> m_vecBuildingTemplates;
	};

}

#endif //_TEMPLATES_H__Included_2051077847


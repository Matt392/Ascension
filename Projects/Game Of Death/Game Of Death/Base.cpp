#include "ASCGenInc.h"
#include "Base.h"

#include "Ascension.h"
#include "ASCRandNumGen.h"

#include "Building.h"
#include "Templates.h"
#include "BuildingTemplate.h"
#include "GameOfDeathContext.h"

CBase::CBase(void)
{
}


CBase::~CBase(void)
{
	for(UINT32 i = 0; i < m_vecBuildings.size(); ++i)
	{
		SafeDelete( m_vecBuildings[i] );
	}
}

bool 
CBase::Initialise(bool bTop)
{
	ASCVector<CBuildingTemplate*>& vecBuildingTemplates = reinterpret_cast<CGameOfDeathContext*>(&Ascension::Context())->Templates()->BuildingTemplates();

	SCoordinates sStartCoords;
	if(bTop)
	{
		sStartCoords.m_sY = 0;
	}
	else
	{
		sStartCoords.m_sY = FIELD_TEXTURE_HEIGHT;
	}

	UINT32 uBuildingCount = 3;
	UINT32 uTemplateIndex = 0;

	UINT32 uColour = 0xFFFFFFFF;
	CHAR8* pColour = reinterpret_cast<CHAR8*>(&uColour);

	for(UINT32 i = 0; i < uBuildingCount; ++i)
	{
		CHAR8 cColour = static_cast<CHAR8>(ASCRandNumGen::Rand(50, 200));
		pColour[0] = cColour;
		pColour[1] = cColour;
		pColour[2] = cColour;

		uTemplateIndex = ASCRandNumGen::Rand(vecBuildingTemplates.size());

		sStartCoords.m_sX = SC_SINT( ASCRandNumGen::Rand(SC_UINT( FIELD_TEXTURE_WIDTH )) );

		CBuilding* pNewBuilding = new CBuilding();
		if(pNewBuilding->Initialise(uColour, vecBuildingTemplates[uTemplateIndex]->Template(), sStartCoords, bTop))
		{
			m_vecBuildings.push_back( pNewBuilding );
		}
		else
		{
			SafeDelete( pNewBuilding );
		}
	}

	return true;
}

void 
CBase::DrawOnField(UINT32* pTexels, UINT32 uPitch)
{
	for(UINT32 i = 0; i < m_vecBuildings.size(); ++i)
	{
		m_vecBuildings[i]->DrawOnField(pTexels, uPitch);
	}
}

#include "ASCGenInc.h"
#include "Building.h"


CBuilding::CBuilding(void)
{
}


CBuilding::~CBuilding(void)
{
}

bool 
CBuilding::Initialise(UINT32 uColour, SPatternTemplate& Template, SCoordinates sStartPos, bool bTop)
{
	m_uColour = uColour;

	sStartPos.m_sX -= Template.m_uWidth / 2;


	
	SCoordinates sTempCoords;

	for(UINT32 y = 0; y < Template.m_uHeight; ++y)
	{
		if(y > (FIELD_TEXTURE_HEIGHT/3))
		{
			assert_now("Guts, Building too tall");
			break;
		}

		for(UINT32 x = 0; x < Template.m_uWidth; ++x)
		{
			if(Template.m_pPattern[y][x] != 0)
			{
				sTempCoords.m_sX = sStartPos.m_sX + x;
				if(bTop)
				{
					sTempCoords.m_sY = sStartPos.m_sY + (Template.m_uHeight-y-1);
				}
				else
				{
					sTempCoords.m_sY = sStartPos.m_sY - (Template.m_uHeight-y);
				}

				if(sTempCoords.m_sX >= 0 && sTempCoords.m_sX < FIELD_TEXTURE_WIDTH)
				{
					m_uCoordinates.push_back( sTempCoords );
				}
			}
		}
	}

	return true;
}
void 
CBuilding::DrawOnField(UINT32* pTexels, UINT32 uPitch)
{
	for(UINT32 i = 0; i < m_uCoordinates.size(); ++i)
	{
		pTexels[(uPitch * m_uCoordinates[i].m_sY) + m_uCoordinates[i].m_sX] = m_uColour;
	}
}
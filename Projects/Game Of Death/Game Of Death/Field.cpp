#include "ASCGenInc.h"
#include "Field.h"

#include "Ascension.h"
#include "ASCRenderer.h"
#include "ASCTextureManager.h"
#include "ASCTexture.h"

#include "ASCGUIManager.h"
#include "ASCGUIStaticImage.h"

#include "ASCSprite.h"

#include "Base.h"
#include "GameOfLifePattern.h"

CField::CField(void)
: m_pFieldTexture ( NULL )
, m_pFieldDisplay ( NULL )
, m_sField (NULL )
, m_sNewField (NULL )
, m_uNextID ( FIELD_ID_START )
, m_uWidth ( 0 )
, m_uHeight ( 0 )
, m_fInterval ( 0.0f )
, m_fRuleInterval ( 0.0f )
, m_bAnyChange ( false )
{
}


CField::~CField(void)
{
	SafeDelete( m_sField );
	SafeDelete( m_sNewField );
}

bool 
CField::Initialise()
{
	ASCString strTemp = "Field";
	m_pFieldTexture = Ascension::Renderer().GetTextureManager()->CreateTexture(FIELD_TEXTURE_WIDTH, FIELD_TEXTURE_HEIGHT, strTemp);
	strTemp = "Background";

	UINT32 uPitch;
	UINT32* pTexels;
	
	m_uHeight = FIELD_TEXTURE_HEIGHT;
	m_uWidth = FIELD_TEXTURE_WIDTH;
	
	m_sField = new SField(m_uWidth, m_uHeight);
	m_sNewField = new SField(m_uWidth, m_uHeight);

	if(m_pFieldTexture->LockTexels(pTexels, uPitch, false))
	{
		for(UINT32 y = 0; y < FIELD_TEXTURE_HEIGHT; ++y)
		{
			for(UINT32 x = 0; x < FIELD_TEXTURE_WIDTH; ++x)
			{
				pTexels[(uPitch*y)+x] = 0XFF000000;
			}
		}

		m_pFieldTexture->UnlockTexels();

		m_pFieldDisplay = Ascension::GUI().AddGUIStaticImage(m_pFieldTexture, Ascension::Width()*0.5f, Ascension::Height()*0.5f, "GUI_No_Blend");
		m_pFieldDisplay->Sprite()->SetSize(Ascension::Width()*1.0f, Ascension::Height()*1.0f);

		return true;
	}

	return false;
}

bool 
CField::Update()
{
	bool bReturn = false;

	m_fInterval += Ascension::Delta();
	if(m_fInterval >= FIELD_MOVE_UPDATE)
	{
		bReturn = true;
		m_fInterval -= FIELD_MOVE_UPDATE;
		MoveElements();
	}
	m_fRuleInterval += Ascension::Delta();
	if(m_fRuleInterval >= FIELD_RULES_UPDATE)
	{
		//bReturn = true;
		m_fRuleInterval -= FIELD_RULES_UPDATE;
		UpdateRules();
	}

	UINT32 uPitch;
	UINT32* pTexels;
	if(m_pFieldTexture->LockTexels(pTexels, uPitch, false))
	{
		for(UINT32 y = 0; y < m_uHeight; ++y)
		{
			for(UINT32 x = 0; x < m_uWidth; ++x)
			{
				pTexels[(uPitch*y)+x] = m_sField->m_pColour[y][x];
			}
		}

		m_pFieldTexture->UnlockTexels();
	}

	return bReturn;
}
void 
CField::UpdateField()
{
	for(UINT32 y = 0; y < m_uHeight; ++y)
	{
		for(UINT32 x = 0; x < m_uWidth; ++x)
		{
			if(m_sField->m_pField[y][x] != FIELD_BASE)
			{
				if(m_sField->m_pNeighbours[y][x] >= FIELD_KILL_CELL)
				{
					m_sField->m_pField[y][x] = FIELD_EMPTY;
					m_sField->m_pColour[y][x] = 0xFF000000;
				}
				else
				{
					if(m_sField->m_pActivate[y][x] == true)
					{
						if(m_sField->m_pNeighbours[y][x] == 3)
						{
							m_sField->m_pField[y][x] = FIELD_BASE+1;
							m_sField->m_pColour[y][x] = 0xFFFFFFFF;
						}
						else if(m_sField->m_pNeighbours[y][x] == 3 || m_sField->m_pNeighbours[y][x] == 2)
						{
						}
						else
						{
							m_sField->m_pField[y][x] = FIELD_EMPTY;
							m_sField->m_pColour[y][x] = 0xFF000000;
						}
					}
				}
			}
			else if(m_sField->m_pField[y][x] == FIELD_BASE)
			{
				if(m_sField->m_pNeighbours[y][x] >= FIELD_KILL_CELL)
				{
					m_sField->m_pField[y][x] = FIELD_EMPTY;
					m_sField->m_pColour[y][x] = 0xFF000000;
					if(m_uBaseTargets > 0)
					{
						--m_uBaseTargets;
						m_bBaseChanged = true;
					}
				}
			}
		}
	}
}
inline void 
CField::CheckCell(UINT32 uX, UINT32 uY, UINT32 uTX, UINT32 uTY)
{
	if(m_sField->m_pField[uTY][uTX] > FIELD_BASE)
	{
		if(m_sField->m_pField[uTY][uTX] != FIELD_EMPTY  && m_sField->m_pField[uY][uX] != m_sField->m_pField[uTY][uTX])
		{
			if(m_sField->m_pField[uY][uX] == FIELD_EMPTY && m_sField->m_pActivate[uTY][uTX] == false)
			{
				m_sField->m_pActivate[uY][uX] = false;
			}
			else
			{
				m_sField->m_pActivate[uY][uX] = true;
			}
		}
		++m_sField->m_pNeighbours[uY][uX];
	}
}
inline void 
CField::CheckCellPatternImpact(UINT32 uX, UINT32 uY, UINT32 uTX, UINT32 uTY)
{
	if(m_sField->m_pField[uTY][uTX] == FIELD_BASE)
	{
		m_sField->m_pNeighbours[uY][uX] = FIELD_KILL_CELL;
	}
}
inline void 
CField::CheckCellBase(UINT32 uX, UINT32 uY, UINT32 uTX, UINT32 uTY)
{
	if(m_sField->m_pField[uTY][uTX] > FIELD_BASE)
	{
		m_sField->m_pNeighbours[uY][uX] = FIELD_KILL_CELL;
	}
}
void 
CField::UpdateRules()
{
	for(UINT32 y = 0; y < m_uHeight; ++y)
	{
		for(UINT32 x = 0; x < m_uWidth; ++x)
		{
			if(m_sField->m_pField[y][x] != FIELD_BASE)
			{
				m_sField->m_pNeighbours[y][x] = 0;
				if(y > 0)// Top
				{
					CheckCell(x, y, x  , y-1);//Top
					CheckCellPatternImpact(x, y, x  , y-1);//Top
				}
				if(y < m_uHeight-1)// Bottom
				{
					CheckCell(x, y, x  , y+1);//Bottom
					CheckCellPatternImpact(x, y, x  , y+1);//Bottom
				}
				if(x > 0)// Left
				{
					CheckCell(x, y, x-1, y  );//Left
					CheckCellPatternImpact(x, y, x-1, y  );//Left
				}
				if(x < m_uWidth-1)// Right
				{
					CheckCell(x, y, x+1, y  );//Right
					CheckCellPatternImpact(x, y, x+1, y  );//Right
				}
				if(y != 0 && x != 0)// Top Left
				{
					CheckCell(x, y, x-1, y-1);//Top Left
				}
				if(y != 0 && x != m_uWidth-1)// Top Right
				{
					CheckCell(x, y, x+1, y-1);//Top Right
				}
				if(y != m_uHeight-1 && x != 0)// Bottom Left
				{
					CheckCell(x, y, x-1, y+1);//Bottom Left
				}
				if(y != m_uHeight-1 && x != m_uWidth-1)// Bottom Right
				{
					CheckCell(x, y, x+1, y+1);//Bottom Right
				}
			}
			else if( m_sField->m_pField[y][x] == FIELD_BASE )
			{
				m_sField->m_pNeighbours[y][x] = 0;
				if(y > 0)// Top
				{
					CheckCellBase(x, y, x  , y-1);//Top
				}
				if(y < m_uHeight-1)// Bottom
				{
					CheckCellBase(x, y, x  , y+1);//Bottom
				}
				if(x > 0)// Left
				{
					CheckCellBase(x, y, x-1, y  );//Left
				}
				if(x < m_uWidth-1)// Right
				{
					CheckCellBase(x, y, x+1, y  );//Right
				}
				//if(y != 0 && x != 0)// Top Left
				//{
				//	CheckCellBase(x, y, x-1, y-1);//Top Left
				//}
				//if(y != 0 && x != m_uWidth-1)// Top Right
				//{
				//	CheckCellBase(x, y, x+1, y-1);//Top Right
				//}
				//if(y != m_uHeight-1 && x != 0)// Bottom Left
				//{
				//	CheckCellBase(x, y, x-1, y+1);//Bottom Left
				//}
				//if(y != m_uHeight-1 && x != m_uWidth-1)// Bottom Right
				//{
				//	CheckCellBase(x, y, x+1, y+1);//Bottom Right
				//}
			}
		}
	}

	UpdateField();
}
void 
CField::MoveElements()
{
	m_bAnyChange = false;
	UINT32 uMoveOffset = 1;
	for(UINT32 y = 0; y < m_uHeight; ++y)
	{
		for(UINT32 x = 0; x < m_uWidth; ++x)
		{
			if(m_sField->m_pField[y][x] > FIELD_BASE)
			{
				m_bAnyChange = true;
				if(y+uMoveOffset < m_uHeight)
				{
					m_sNewField->m_pField[y+uMoveOffset][x] = m_sField->m_pField[y][x];
					m_sNewField->m_pColour[y+uMoveOffset][x] = m_sField->m_pColour[y][x];
					m_sNewField->m_pNeighbours[y+uMoveOffset][x] = m_sField->m_pNeighbours[y][x];
					m_sNewField->m_pActivate[y+uMoveOffset][x] = m_sField->m_pActivate[y][x];
				}
			}
			else if(m_sField->m_pField[y][x] == FIELD_BASE)
			{
				m_sNewField->m_pField[y][x] = m_sField->m_pField[y][x];
				m_sNewField->m_pColour[y][x] = m_sField->m_pColour[y][x];
				m_sNewField->m_pNeighbours[y][x] = m_sField->m_pNeighbours[y][x];
				m_sNewField->m_pActivate[y][x] = m_sField->m_pActivate[y][x];
			}
		}
	}
	
	for(UINT32 y = 0; y < m_uHeight; ++y)
	{
		for(UINT32 x = 0; x < m_uWidth; ++x)
		{
			m_sField->m_pField[y][x] = m_sNewField->m_pField[y][x];
			m_sField->m_pColour[y][x] = m_sNewField->m_pColour[y][x];
			m_sField->m_pNeighbours[y][x] = m_sNewField->m_pNeighbours[y][x];
			m_sField->m_pActivate[y][x] = m_sNewField->m_pActivate[y][x];
		}
	}
	m_sNewField->Clear();
}
UINT32 
CField::BaseTargets()
{
	return m_uBaseTargets;
}

bool 
CField::AnyChange()
{
	return m_bAnyChange;
}
bool 
CField::BaseChanged()
{
	if(m_bBaseChanged)
	{
		m_bBaseChanged = false;
		return true;
	}

	return false;
}
void 
CField::FillWithAttackPattern(CGameOfLifePattern* pPattern, SCoordinates sCoords)
{
	UINT32 uAliveColour = pPattern->AliveColour();
	UINT32 uPatternWidth = pPattern->Width();
	UINT32 uPatternHeight = pPattern->Height();
	bool** pPatternArray = pPattern->Pattern();
	
	UINT32 uOffestX = uPatternWidth / 4;
	UINT32 uOffestY = 0;

	SINT32 sIndexY = 0;
	SINT32 sIndexX = 0;

	UINT32 uID = GetNextID();

	for(UINT32 y = sCoords.m_sY; y < m_uHeight && y-sCoords.m_sY < uPatternHeight; ++y)
	{
		for(UINT32 x = sCoords.m_sX; x < m_uHeight && x-sCoords.m_sX < uPatternWidth; ++x)
		{
			if(true == pPatternArray[y-sCoords.m_sY][x-sCoords.m_sX])
			{
				sIndexY = y - uOffestY;
				sIndexX = x - uOffestX;
				if((sIndexY >= 0 && sIndexY < SC_SINT( m_uHeight )) && (sIndexX >= 0 && sIndexX < SC_SINT( m_uWidth )))
				{
					m_sField->m_pField[sIndexY][sIndexX] = uID;
					m_sField->m_pColour[sIndexY][sIndexX] = uAliveColour;
					m_sField->m_pActivate[sIndexY][sIndexX] = false;
				}
			}
		}
	}
}
UINT32 
CField::GetNextID()
{
	UINT32 uReturn = m_uNextID;
	++m_uNextID;
	if(m_uNextID >= UINT_MAX)
	{
		assert_now("Guts, Have run out of IDs of patterns");
	}
	return uReturn;
}
void 
CField::FillField(CBase* pBase)
{
	m_uBaseTargets = 0;
	UINT32 uPitch;
	UINT32* pTexels;
	if(m_pFieldTexture->LockTexels(pTexels, uPitch, false))
	{
		for(UINT32 y = 0; y < m_uHeight; ++y)
		{
			for(UINT32 x = 0; x < m_uWidth; ++x)
			{
				pTexels[(uPitch*y)+x] = 0xFF000000;
			}
		}

		pBase->DrawOnField(pTexels, uPitch);

		for(UINT32 y = 0; y < m_uHeight; ++y)
		{
			for(UINT32 x = 0; x < m_uWidth; ++x)
			{
				m_sField->m_pColour[y][x] = pTexels[(uPitch*y)+x];
				if(pTexels[(uPitch*y)+x] == 0xFF000000)
				{
					m_sField->m_pField[y][x] = FIELD_EMPTY;
				}
				else
				{
					m_sField->m_pField[y][x] = FIELD_BASE;
					++m_uBaseTargets;
				}
			}
		}

		m_pFieldTexture->UnlockTexels();
	}
}

void 
CField::Activate()
{
	m_pFieldDisplay->Enable();
}
void 
CField::Deactivcate()
{
	m_pFieldDisplay->Disable();
}
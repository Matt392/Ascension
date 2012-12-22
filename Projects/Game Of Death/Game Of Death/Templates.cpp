#include "ASCGenInc.h"
#include "Templates.h"

#include "BuildingTemplate.h"

#include "ASCFolder.h"

CTemplates::CTemplates(void)
{
}


CTemplates::~CTemplates(void)
{
	for(UINT32 i = 0; i < m_vecBuildingTemplates.size(); ++i)
	{
		SafeDelete(m_vecBuildingTemplates[i]);
	}
}

bool 
CTemplates::Initialise()
{
	ASCFolder Folder;

	if(Folder.Load("Templates/Buildings"))
	{
		UINT32 uLookUp[256];

		memset(uLookUp, 0, sizeof(UINT32)*256);
		
		uLookUp['x'] = 1;
		uLookUp['X'] = 1;

		ASCVector<ASCString>& rFiles = *Folder.Files();

		for(UINT32 i = 0 ; i < rFiles.size(); ++i)
		{
			CBuildingTemplate* pNewBuilding = new CBuildingTemplate();
			if(pNewBuilding->Initialise(rFiles[i], &uLookUp[0]))
			{
				m_vecBuildingTemplates.push_back(pNewBuilding);
			}
		}
	}

	return true;
}
ASCVector<CBuildingTemplate*>& 
CTemplates::BuildingTemplates()
{
	return m_vecBuildingTemplates;
}
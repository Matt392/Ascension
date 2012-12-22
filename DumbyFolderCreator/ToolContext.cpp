#include "ASCGenInc.h"
#include "ToolContext.h"
#include "ASCFolder.h"
#include "ASCTexture.h"
#include "ASCTextureManager.h"
#include "Ascension.h"
#include "ASCRenderer.h"

CToolContext::CToolContext(void)
{
}


CToolContext::~CToolContext(void)
{
}
//Creation
bool 
CToolContext::Initialise()
{
	ASCFolder MainFolder;
	if(MainFolder.Load("../RuntimeFolder"))
	{
		ASCVector<ASCString>& rvecFolders = *MainFolder.Folders();

		for(UINT32 i = 0; i < rvecFolders.size(); ++i)
		{
			ASCString strFolder = "";
			rvecFolders[i].substr(MainFolder.FolderPath().length() + 1, strFolder);
			
			ASCFolder Folder;
			if(Folder.Load(strFolder))
			{
				Folder.SaveDirectoryInfo();
			}
		}
	}
	return true;
}
		
//Game logic
bool 
CToolContext::Update()
{
	return false;
}

//Optional
void 
CToolContext::Draw()
{
}
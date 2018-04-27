#include "guibookmarks.h"
#include "fileobject.h"
#include "stuff.h"

#include "main.h"
extern CMain *Main;

#include "guiimages.h"
extern GuiImages *winImages;

#include "guistart.h"
extern GuiImages *winStart;

GuiBookmarks::GuiBookmarks() : CWindow()
{
	
}


GuiBookmarks::~GuiBookmarks()
{
	clearGuiElements();
}

void GuiBookmarks::Init(){

	std::cout << "GuiBookmarks::Init()" << std::endl;

	panelInfo = new CStackPanel();
	panelInfo->setX(310);
	panelInfo->setY(10);
	panelInfo->setWidth(160);
	panelInfo->setHeight(230);

	listcombobookmarks = new CListboxComboBookmarks();
	listcombobookmarks->setX(10);
	listcombobookmarks->setY(10);
	listcombobookmarks->setWidth(290);
	listcombobookmarks->setHeight(252);

	listcombobookmarks->Init();

	addGuiElement(listcombobookmarks);
	addGuiElement(panelInfo);

}



void GuiBookmarks::KeyClick(){
	if(listcombobookmarks->isComboboxOpen())
	{
		listcombobookmarks->KeyClick();
		enBookmarkChoice retChoice = listcombobookmarks->GetChoice();
		
		if( retChoice == EN_BOOKMARKCHOICE_OPEN )
		{
			if( listcombobookmarks->GetSelectedBookmark() )
			{
				if(winImages->BrowseToFile( listcombobookmarks->GetSelectedBookmark() ))
					Main->ChangeMode(EN_MODE_IMAGES);
			}
		}
		else if( retChoice == EN_BOOKMARKCHOICE_WRITE )
		{
			CFileObject *pFileObj = winImages->GetSelectedFileObject();
			if(pFileObj != NULL)
			{
				CFileObject *fileObjWrite = listcombobookmarks->GetSelectedBookmark();
				if(fileObjWrite != NULL)
				{
					fileObjWrite->setFiletype(pFileObj->getFiletype());
					fileObjWrite->setPath( pFileObj->getPath() );
					fileObjWrite->setFilename( pFileObj->getFilename() );
					fileObjWrite->setPos( pFileObj->getPos() );
// 					fileObjWrite->setRar( pFileObj->getRar() );
// 					fileObjWrite->setArchive( pFileObj->getArchive() );
					fileObjWrite->Debug();
					listcombobookmarks->UpdateLabels();
				}
			}
		}
		else if( retChoice == EN_BOOKMARKCHOICE_DELETE )
		{
			listcombobookmarks->RemoveSelectedBookmark();
		}
		else if( retChoice == EN_BOOKMARKCHOICE_ADDNEW )
		{
			CFileObject *pFileObj = winImages->GetSelectedFileObject();
			if(pFileObj == NULL)
				return;
		
			listcombobookmarks->InsertNewBookmark(pFileObj);
		}
		else if( retChoice == EN_BOOKMARKCHOICE_SAVE )
		{
			CStuff::Save();
		}
		else if( retChoice == EN_BOOKMARKCHOICE_LOAD )
		{
			listcombobookmarks->RemoveAllBookmarks();
			CStuff::Load();
		}
	}
	else
		listcombobookmarks->KeyClick();
}

void GuiBookmarks::KeyCancel(){
//check for selected combobox
	if(listcombobookmarks->isComboboxOpen())
		listcombobookmarks->KeyCancel();
	else
		Main->ChangeMode(EN_MODE_START);
}

void GuiBookmarks::KeyUp(){
	listcombobookmarks->KeyUp();
}

void GuiBookmarks::KeyDown(){
	listcombobookmarks->KeyDown();
}

void GuiBookmarks::KeyLeft(){
	
}

void GuiBookmarks::KeyRight(){
	
}

void GuiBookmarks::KeySquare(){
	
	
}

void GuiBookmarks::KeyTriangle(){
	
}

void GuiBookmarks::KeyL(){
	listcombobookmarks->SetSelectedElement(1);
}

void GuiBookmarks::KeyR(){
	listcombobookmarks->SetSelectedElement(listcombobookmarks->GetBookmarkCount());
}



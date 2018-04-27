#include "guiimages.h"


#include "main.h"
extern CMain *Main;
#include "imageviewer.h"
extern ImageViewer *imageViewer;

GuiImages::GuiImages() : CWindow()
{
	clearGuiElements();
}


GuiImages::~GuiImages()
{
	
}

void GuiImages::Init(){

	labelFileInfo = new CLabelDesigned();
	labelFileInfo->setText("Fileinfo");

	labelFolderInfoFilecount = new CLabelDesigned();
	labelFolderInfoFilecount->setText("x");

	labelFolderInfo = new CLabelDesigned();
	labelFolderInfo->setText("Folderinfo");

	labelCurrentFolder = new CLabelDesigned();
	labelCurrentFolder->setText("x");
	
	panelFolderInfo = new CStackPanel();
	panelFolderInfo->setX(310);
	panelFolderInfo->setY(10);
	panelFolderInfo->setWidth(160);
	panelFolderInfo->setHeight(100);

	panelFileInfo = new CStackPanel();
	panelFileInfo->setX(310);
	panelFileInfo->setY(120);
	panelFileInfo->setWidth(160);
	panelFileInfo->setHeight(120);

	panelCurrentFolder = new CStackPanel();
	panelCurrentFolder->setX(1);
	panelCurrentFolder->setY(251);
	panelCurrentFolder->setWidth(478);
	panelCurrentFolder->setHeight(19);
		
	listboxFilemanager = new CListboxFilebrowser();
	listboxFilemanager->setX(1);
	listboxFilemanager->setY(1);
	listboxFilemanager->setMarginX(10);
	listboxFilemanager->setWidth(290);
	listboxFilemanager->setHeight(245);
	listboxFilemanager->Init();

	panelFileInfo->addGuiElement((CGuiElement*)labelFileInfo);
	panelFolderInfo->addGuiElement((CGuiElement*)labelFolderInfo);
	panelFolderInfo->addGuiElement((CGuiElement*)labelFolderInfoFilecount);

	addGuiElement(listboxFilemanager);
	addGuiElement(panelFolderInfo);
	addGuiElement(panelFileInfo);


	panelCurrentFolder->addGuiElement((CGuiElement*)labelCurrentFolder);
	addGuiElement(panelCurrentFolder);
}



bool GuiImages::BrowseToFile(CFileObject *file){

	if( file == NULL )
		return false;

	bool retVal = listboxFilemanager->BrowseToFile(file);
	UpdateFolderInfo();

	return retVal;
}


void GuiImages::UpdateFolderInfo()
{
	char strTmp[1000];

	short pos = listboxFilemanager->getSelectedFilePosition();
	short count = listboxFilemanager->getFileCount();
	
	sprintf(strTmp, "Pos: %d/%d", pos, count);
	labelFolderInfoFilecount->setTextAndCreateBackground(strTmp);

	labelCurrentFolder->setTextAndCreateBackground(listboxFilemanager->getCurrentFolder());
	
}


void GuiImages::setCurrentFolder(std::string path)
{
	listboxFilemanager->setCurrentFolder(path);
	UpdateFolderInfo();
}


void GuiImages::KeyClick(){
	listboxFilemanager->KeyClick();

	UpdateFolderInfo();
	CFileObject *tmpFileObject = listboxFilemanager->getSelectedFileObject();

	if( tmpFileObject != NULL )
	{
	//	labelFileInfo->setText(tmpFileObject->getCompleteFilename());
		if(tmpFileObject->getFiletype() == 2 )
		{
			//tmpFileObject->Debug();
			
			imageViewer->LoadImage(tmpFileObject);
			Main->ChangeMode(EN_MODE_IMAGEVIEWER);
		
			imageViewer->ImagelistEmpty();

			short poslimit; 
			short pos;
			pos = 1;//listboxFilemanager->getSelectedFilePosition();
			poslimit = listboxFilemanager->getFileCount();
	
			short imagecount = 1;

			while(pos <= poslimit)
			{
				//CFileObject *tmpNewFileObject = new CFileObject();
				tmpFileObject = listboxFilemanager->getSelectedFileObjAtPos(pos);
	
				if( tmpFileObject->getFiletype() == 2)
				{
					CFileObject *tmpNewFileObject = new CFileObject();
					tmpNewFileObject->setFilename(tmpFileObject->getFilename());
					tmpNewFileObject->setPath(tmpFileObject->getPath());
					tmpNewFileObject->setPos(tmpFileObject->getPos());
					tmpNewFileObject->setFiletype(2);
					imageViewer->ImagelistInsert(tmpNewFileObject);
					imagecount++;
				}
				pos++;
				if(pos == listboxFilemanager->getSelectedFilePosition())
					imageViewer->SetImagelistPos(imagecount);
			}
		}
	}
	
}

void GuiImages::KeyCancel(){
	Main->ChangeMode(EN_MODE_START);

}

void GuiImages::KeyUp(){
	listboxFilemanager->KeyUp();
	UpdateFolderInfo();
}

void GuiImages::KeyDown(){
	listboxFilemanager->KeyDown();
	UpdateFolderInfo();
}

void GuiImages::KeyLeft(){
	listboxFilemanager->KeyLeft();
	UpdateFolderInfo();
}

void GuiImages::KeyRight(){
	listboxFilemanager->KeyRight();
	UpdateFolderInfo();
}

void GuiImages::KeyL(){
	listboxFilemanager->SetSelectedElement(1);
	UpdateFolderInfo();
}

void GuiImages::KeyR(){
	listboxFilemanager->SetSelectedElement(listboxFilemanager->getFileCount());
	UpdateFolderInfo();
}




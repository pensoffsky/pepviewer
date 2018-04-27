#include "guiosdimage.h"

#include "main.h"
extern CMain *Main;

#include "guibookmarks.h"
extern GuiBookmarks *winBookmarks;

#include "imageviewer.h"
extern ImageViewer *imageViewer;

#include "guiimages.h"
extern GuiImages *winImages;

GuiOsdImage::GuiOsdImage() : CWindow()
{
	
}


GuiOsdImage::~GuiOsdImage()
{
	clearGuiElements();
}

void GuiOsdImage::Init(){


	labelCmdRotH = new CLabelDesigned();
	labelCmdRotH->setText("RotH");
	labelCmdRotW = new CLabelDesigned();
	labelCmdRotW->setText("RotW");
	labelCmdBookmark = new CLabelDesigned();
	labelCmdBookmark->setText("Bookmark");
	labelCmdOriginal = new CLabelDesigned();
	labelCmdOriginal->setText("Original");

	labelInfoFilename = new CLabelDesigned();
	labelInfoFilename->setText("Filename:");
	labelInfoPath = new CLabelDesigned();
	labelInfoPath->setText("Path:");
// 	labelInfoArchive = new CLabelDesigned();
// 	labelInfoArchive->setText("Archive:");


	std::cout << "GuiOsdImage::Init()" << std::endl;

	panelInfo = new CStackPanel();
	panelInfo->setX(10);
	panelInfo->setY(202);
	panelInfo->setWidth(W-20);
	panelInfo->setHeight(60);

	panelInfo->addGuiElement(labelInfoFilename);
	panelInfo->addGuiElement(labelInfoPath);
// 	panelInfo->addGuiElement(labelInfoArchive);
	

	listbox = new CListbox();
	listbox->setX(10);
	listbox->setY(10);
	listbox->setHeight(90);
	listbox->setWidth(90);
	listbox->addGuiElement((CGuiElement*)labelCmdOriginal);
	listbox->addGuiElement((CGuiElement*)labelCmdRotH);
	listbox->addGuiElement((CGuiElement*)labelCmdRotW);
	listbox->addGuiElement((CGuiElement*)labelCmdBookmark);

	addGuiElement(panelInfo);
	addGuiElement(listbox);
}



void GuiOsdImage::KeyClick(){
	CLabel *tmp = (CLabel*)listbox->getSelectedItem();
	
	if( tmp->getText() == "Original" ){
		std::cout << "Original selected" << std::endl;
		imageViewer->FitToOriginal();
		Main->ChangeMode(EN_MODE_IMAGEVIEWER);
	}
	else if( tmp->getText() == "RotH" ){
// 		imageViewer->LoadImageRaw( imageViewer->ImagelistGet(0) );
		imageViewer->Rotate(90);
		imageViewer->FitToHeight(H);
		Main->ChangeMode(EN_MODE_IMAGEVIEWER);
		std::cout << "RotH selected" << std::endl;
	}
	else if( tmp->getText() == "RotW" ){
		std::cout << "RotW selected" << std::endl;
// 		imageViewer->LoadImageRaw( imageViewer->ImagelistGet(0) );
		imageViewer->Rotate(90);
		imageViewer->FitToWidth(W);
		Main->ChangeMode(EN_MODE_IMAGEVIEWER);
	}
	else if( tmp->getText() == "Bookmark" ){
		std::cout << "Bookmark selected" << std::endl;
		winBookmarks->InsertNewBookmark( imageViewer->ImagelistGet(0) );
		Main->ChangeMode(EN_MODE_IMAGEVIEWER);
	}
}

void GuiOsdImage::KeyCancel(){
	Main->ChangeMode(EN_MODE_IMAGEVIEWER);
}

void GuiOsdImage::KeyUp(){
	listbox->KeyUp();
}

void GuiOsdImage::KeyDown(){
	listbox->KeyDown();
}

void GuiOsdImage::KeyLeft(){
	
}

void GuiOsdImage::KeyRight(){
	
}

void GuiOsdImage::KeyL(){
	//listcombo->SetSelectedElement(1);
}

void GuiOsdImage::KeyR(){
	//listcombo->SetSelectedElement(listcombo->getElementCount());
}

void GuiOsdImage::KeyStart(){
	Main->ChangeMode(EN_MODE_IMAGEVIEWER);	
}

void GuiOsdImage::Draw(SDL_Surface* target){

	labelInfoFilename->setText( "Filename: " + imageViewer->ImagelistGet(0)->getFilename() );
// 	labelInfoArchive->setText( "Archive: " + imageViewer->ImagelistGet(0)->getArchive() );
// 	labelInfoPath->setText( "Path: " + imageViewer->ImagelistGet(0)->getPath() );
	labelInfoPath->setTextAndCreateBackground( "Path: " + winImages->getCurrentFolder() );
	CWindow::Draw(target);
}

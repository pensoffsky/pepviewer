#include "guistart.h"
#include "imageloader.h"

#include "main.h"
extern CMain *Main;

GuiStart::GuiStart() : CWindow()
{
	
}


GuiStart::~GuiStart()
{
	clearGuiElements();
}

void GuiStart::Init(){

	std::cout << "GuiStart Init()" << std::endl;

	logo = NULL;
// 	logo = CImageLoader::LoadResource("graphics/asdf.png");
// 	if(logo)
// 		std::cout << "  GuiStart logo loaded" << std::endl;

	labelImages = new CLabelDesigned();
	labelImages->setText("Images");

// 	labelMP3s = new CLabelDesigned();
// 	labelMP3s->setText("MP3s");
	
	labelBookmarks = new CLabelDesigned();
	labelBookmarks->setText("Bookmarks");

	labelSettings = new CLabelDesigned();
	labelSettings->setText("Settings");

	labelExit = new CLabelDesigned();
	labelExit->setText("Exit");
		
	listbox = new CListbox();
	listbox->setX(90);
	listbox->setY(90);
	listbox->setHeight(60);
	listbox->setWidth(90);
	listbox->addGuiElement((CGuiElement*)labelImages);
	//listbox->addGuiElement((CGuiElement*)labelMP3s);
	listbox->addGuiElement((CGuiElement*)labelBookmarks);
	listbox->addGuiElement((CGuiElement*)labelSettings);
	listbox->addGuiElement((CGuiElement*)labelExit);

	addGuiElement(listbox);
}

void GuiStart::KeyClick(){
	CLabel *tmp = (CLabel*)listbox->getSelectedItem();
	
	if( tmp->getText() == "Images" ){
		std::cout << "images selected" << std::endl;
		Main->ChangeMode(EN_MODE_IMAGES);
	}
	else if( tmp->getText() == "MP3s" ){
		std::cout << "mp3s selected" << std::endl;
	}
	else if( tmp->getText() == "Bookmarks" ){
		std::cout << "bookmarks selected" << std::endl;
		Main->ChangeMode(EN_MODE_BOOKMARKS);
	}
	else if( tmp->getText() == "Settings" ){
		std::cout << "settings selected" << std::endl;
		Main->ChangeMode(EN_MODE_SETTINGS);
	}
	else if( tmp->getText() == "Exit" ){
		std::cout << "exit selected" << std::endl;
		Main->ChangeMode(EN_MODE_EXIT);
	}
}

void GuiStart::KeyCancel(){
	
}

void GuiStart::KeyUp(){
	listbox->KeyUp();
}

void GuiStart::KeyDown(){
	listbox->KeyDown();
}

void GuiStart::KeyLeft(){
	
}

void GuiStart::KeyRight(){
	
}


void GuiStart::Draw(SDL_Surface* target){

	CWindow::Draw(target);
	if(logo)
	{
// 		SDL_Surface *tmp = NULL;
// // 		tmp = sge_transform_surface(logo, SDL_MapRGB(logo->format, 255, 0, 0), 3.0f, logo->w, logo->h, 0);
// 		sge_BlitTransparent(logo, target, 0, 0, 0, 0, logo->w, logo->h,SDL_MapRGB(logo->format, 255, 0, 0), 255);
// 		SDL_FreeSurface(tmp);
// 		sge_Blit (logo, target, 0, 0, 0, 0, logo->w, logo->h);
	}
}



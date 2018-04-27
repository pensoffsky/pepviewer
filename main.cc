#include "main.h" 

#include "label.h"
#include "guielement.h"
#include "stackpanel.h"
#include "stuff.h"
#include "imageviewer.h"

CMain *Main;

//windows

//GuiImageViewer *winImageViewer;

//system
CConfig *config;
GuiImages *winImages;
ImageViewer *imageViewer;
SDL_mutex *lock;
SDL_mutex *lockLoadImage;
GuiBookmarks *winBookmarks;
GuiSettings *winSettings;
GuiOsdImage *winOsdImage;
GuiStart *winStart;

#ifndef PSP
int main(int argc, char *argv[])
#else
extern "C" int SDL_main(int argc, char *argv[])
#endif
{
	Main = new CMain();
	Main->InitSDL();
	Main->InitSystem();
	Main->InitWindows();
	
	//THE LOOP
	Main->Loop();

	delete Main;
	delete config;
	delete winImages;
	delete imageViewer;
	delete winBookmarks;
	delete winSettings;
	delete winOsdImage;
	delete winStart;

	SDL_Quit();
	return 0;
}



CMain::CMain()
{
	mode = 0;
		
	short i;
	for(i=0; i<=22 ; i++)
	{	
		key[i] = 0;
	}
}

CMain::~CMain()
{
	
}

void CMain::GetEvents()
{
//hier werden die eingaben verarbeitet
	while (SDL_PollEvent(&event))
	{ 

		switch (event.type) {

			case SDL_QUIT:
				SDL_Quit();
				break;

//key[]
//richtungen: up=0, down=1, left=2, right=3,
//kn�fe: x=4, kreis(d)=5, dreieck(e)=6, quadrat(s)=7
//obere kn�fe: obenlinks(q)=8, obenrechts(t)=9

			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_UP)
					key[0]= 1;
				else if(event.key.keysym.sym == SDLK_DOWN)
					key[1]= 1; 
				else if(event.key.keysym.sym == SDLK_LEFT)
					key[2]= 1;
				else if(event.key.keysym.sym == SDLK_RIGHT)
					key[3]= 1;
				else if(event.key.keysym.sym == SDLK_x)
				{
					if(config->GetSwapXO() == 0)
						key[4]= 1;
					else
						key[5]= 1;
				}
				else if(event.key.keysym.sym == SDLK_d)
				{
					if(config->GetSwapXO() == 0)
						key[5]= 1;
					else
						key[4]= 1;
				}
				else if(event.key.keysym.sym == SDLK_e)
					key[6]= 1;
				else if(event.key.keysym.sym == SDLK_s)
					key[7]= 1;
				else if(event.key.keysym.sym == SDLK_q)
					key[8]= 1;
				else if(event.key.keysym.sym == SDLK_t)
					key[9]= 1;
				else if(event.key.keysym.sym == SDLK_v)
					key[10]= 1;
				else if(event.key.keysym.sym == SDLK_b)
					key[11]= 1;
					
				else if(event.key.keysym.sym == SDLK_F10)		//play/pause
					key[12]= 1;	
				else if(event.key.keysym.sym == SDLK_F11)		//forward
					key[13]= 1;	
				else if(event.key.keysym.sym == SDLK_F12)		//back
					key[14]= 1;
				else if(event.key.keysym.sym == SDLK_F13)		//volume up
					key[15]= 1; 
				else if(event.key.keysym.sym == SDLK_F14)		//volume down
					key[16]= 1;
				else if(event.key.keysym.sym == SDLK_F15)		//hold
					key[17]= 1;
				break;

        		case SDL_KEYUP:
				if(event.key.keysym.sym == SDLK_UP)
					key[0]= 0;       
				else if(event.key.keysym.sym == SDLK_DOWN)
					key[1]= 0;       
				else if(event.key.keysym.sym == SDLK_LEFT)
					key[2]= 0;
				else if(event.key.keysym.sym == SDLK_RIGHT)
					key[3]= 0;
				else if(event.key.keysym.sym == SDLK_x)
				{
					if(config->GetSwapXO() == 0)
						key[4]= 0;
					else
						key[5]= 0;
				}
				else if(event.key.keysym.sym == SDLK_d)
				{
					if(config->GetSwapXO() == 0)
						key[5]= 0;
					else
						key[4]= 0;
				}
				else if(event.key.keysym.sym == SDLK_e)
					key[6]= 0;
				else if(event.key.keysym.sym == SDLK_s)
					key[7]= 0;
				else if(event.key.keysym.sym == SDLK_q)
					key[8]= 0;
				else if(event.key.keysym.sym == SDLK_t)
					key[9]= 0;
				else if(event.key.keysym.sym == SDLK_v)
					key[10]= 0;
				else if(event.key.keysym.sym == SDLK_b)
					key[11]= 0;
					
				else if(event.key.keysym.sym == SDLK_F10)		//play/pause
					key[12]= 0;
				else if(event.key.keysym.sym == SDLK_F11)		//forward
					key[13]= 0;	
				else if(event.key.keysym.sym == SDLK_F12)		//back
					key[14]= 0;
				else if(event.key.keysym.sym == SDLK_F13)		//volume up
					key[15]= 0; 
				else if(event.key.keysym.sym == SDLK_F14)		//volume down
					key[16]= 0;
				else if(event.key.keysym.sym == SDLK_F15)		//hold
					key[17]= 0;	
				break;

//key[]           
//richtungen: up=0, down=1, left=2, right=3,
//kn�fe: x=4, kreis(d)=5, dreieck(e)=6, quadrat(s)=7
//obere kn�fe: obenlinks(q)=8, obenrechts(t)=9

			case SDL_JOYBUTTONDOWN:
				if(event.jbutton.button == 8)     // up
					key[0]= 1; 
				else if(event.jbutton.button == 6)// down
					key[1]= 1; 
				else if(event.jbutton.button == 7)// left
					key[2]= 1; 
				else if(event.jbutton.button == 9)// right
					key[3]= 1; 
				else if(event.jbutton.button == 2)// X
				{
					if(config->GetSwapXO() == 0)
						key[4]= 1;
					else
						key[5]= 1;
				}
				else if(event.jbutton.button == 1)// circle
				{
					if(config->GetSwapXO() == 0)
						key[5]= 1;
					else
						key[4]= 1;
				}
				else if(event.jbutton.button == 0)// triangel
					key[6]= 1; 
				else if(event.jbutton.button == 3)// square
					key[7]= 1; 
				else if(event.jbutton.button == 4)// LeftTrigger
					key[8]= 1; 
				else if(event.jbutton.button == 5)// RightTrigger
					key[9]= 1; 
				else if(event.jbutton.button == 10)// Select
					key[10]= 1;
				else if(event.jbutton.button == 11)// Start
					key[11]= 1;  
				break;

			case SDL_JOYBUTTONUP:
				if(event.jbutton.button == 8)     // up
					key[0]= 0; 
				else if(event.jbutton.button == 6)// down
					key[1]= 0; 
				else if(event.jbutton.button == 7)// left
					key[2]= 0; 
				else if(event.jbutton.button == 9)// right
					key[3]= 0; 
				else if(event.jbutton.button == 2)// X
				{
					if(config->GetSwapXO() == 0)
						key[4]= 0;
					else
						key[5]= 0;
				}
				else if(event.jbutton.button == 1)// circle
				{
					if(config->GetSwapXO() == 0)
						key[5]= 0;
					else
						key[4]= 0;
				}
				else if(event.jbutton.button == 0)// triangel
					key[6]= 0; 
				else if(event.jbutton.button == 3)// square
					key[7]= 0; 
				else if(event.jbutton.button == 4)// LeftTrigger
					key[8]= 0; 
				else if(event.jbutton.button == 5)// RightTrigger
					key[9]= 0; 
				else if(event.jbutton.button == 10)// Select
					key[10]= 0;
				else if(event.jbutton.button == 11)// Start
					key[11]= 0;
				break;

			default:
				break;
      }
    }

}

//handle the key presses
//keynumber:arrayposition of key
//waitfirst:die schwelle bis zur ersten wiederholung
//waitrepeat:die schwelle für jede weitere wiederholung
short CMain::Keys(short keynumber, short waitfirst, short waitrepeat)
{
	if(key[keynumber] > 1)
		key[keynumber]++;
	
	//the first press
	if(key[keynumber] == 1)
	{
		key[keynumber] = 2;
		return 1;  //the primary press
	}
	
	//the repetition press
	if(key[keynumber] > waitfirst)
	{
		key[keynumber] = waitrepeat;
		return 2;  //the repeated press
	}

	return 0; //no event
}


void CMain::React2Keys()
{
	if(key[0] > 0) {  //UP
		if(GetMode() == EN_MODE_IMAGEVIEWER){
// 			imageViewer->ScrollFromExtern(0,config->GetPadspeed(-1));
			imageViewer->ScrollFromExtern(0,-1);
		}else
		{
			short tmpkey = this->Keys(0, 10, 14);
			if(tmpkey == 1)
				currentWindow->KeyUp();
			else if(tmpkey == 2)
				currentWindow->KeyUp();
		}
	}
	
	if(key[1] > 0) {  //DOWN
		if(GetMode() == EN_MODE_IMAGEVIEWER){
// 			imageViewer->ScrollFromExtern(0,config->GetPadspeed(1));
			imageViewer->ScrollFromExtern(0,1);
		}else
		{
			short tmpkey = this->Keys(1, 10, 14);
			if(tmpkey == 1)
				currentWindow->KeyDown();
			else if(tmpkey == 2)
				currentWindow->KeyDown();
		}
	}
	
	if(key[2] > 0) {  //LEFT
		if(GetMode() == EN_MODE_IMAGEVIEWER){
// 			imageViewer->ScrollFromExtern(config->GetPadspeed(-1),0);
			imageViewer->ScrollFromExtern(-1,0);
		}else
		{
			short tmpkey = this->Keys(2, 10, 14);
			if(tmpkey == 1)
				currentWindow->KeyLeft();
			else if(tmpkey == 2)
				currentWindow->KeyLeft();
		}
	}
	
	if(key[3] > 0) {  //RIGHT
		if(GetMode() == EN_MODE_IMAGEVIEWER){
// 			imageViewer->ScrollFromExtern(config->GetPadspeed(1),0);
			imageViewer->ScrollFromExtern(1,0);
		}else
		{
			short tmpkey = this->Keys(3, 10, 14);
			if(tmpkey == 1)
				currentWindow->KeyRight();
			else if(tmpkey == 2)
				currentWindow->KeyRight();
		}
	}
	
	if(key[4] == 1) {  //X - x
		key[4] = 0;
		if(GetMode() == EN_MODE_IMAGEVIEWER){
			imageViewer->KeyClick();
		}else
		currentWindow->KeyClick();
	}
			
	if(key[5] == 1) {  //Kreis - d =
		key[5] = 0;
		if(GetMode() == EN_MODE_IMAGEVIEWER){
			imageViewer->KeyCancel();
		}else
		currentWindow->KeyCancel();
	}
	
	if(key[6] == 1) {  //Dreieck - e =
		key[6] = 0;
		if(GetMode() == EN_MODE_IMAGEVIEWER){
			imageViewer->KeyTriangle();
		}else
		currentWindow->KeyTriangle();
	}
	
	if(key[7] == 1) {  //Quadrat - s =
		key[7] = 0;
		if(GetMode() == EN_MODE_IMAGEVIEWER){
			imageViewer->KeySquare();
		}else
		currentWindow->KeySquare();
	}

	if(key[8] == 1) {  //LeftTrigger - q =
		key[8] = 0;
		if(GetMode() == EN_MODE_IMAGEVIEWER){
			imageViewer->NextPrev(-1);
		}else
		{
			currentWindow->KeyL();
		}
	}

	if(key[9] == 1) {  //RightTrigger - t =
		key[9] = 0;
		if(GetMode() == EN_MODE_IMAGEVIEWER){
			imageViewer->NextPrev(1);
		}else
		{
			currentWindow->KeyR();
		}
	}

	if(key[10] == 1) {  //Select - v =
		key[10] = 0;
		if(GetMode() == EN_MODE_IMAGEVIEWER){
			imageViewer->KeySelect();
		}else
		{
			currentWindow->KeySelect();
		}
		#ifdef PSP
			CStuff::showramusage();
		#endif
	}

	if(key[11] == 1) {  //Start - b =
		key[11] = 0;
		if(GetMode() == EN_MODE_IMAGEVIEWER){
			imageViewer->KeyStart();
		}else
		{
			currentWindow->KeyStart();
		}
	}

	if(key[12] == 1) {  //remote: play/pause
		key[12] = 0;
		if(GetMode() == EN_MODE_IMAGEVIEWER){
			imageViewer->KeyCancel();
		}else
		currentWindow->KeyCancel();
	}

	if(key[13] == 1) {  //remote: forward
		key[13] = 0;
		if(GetMode() == EN_MODE_IMAGEVIEWER){
			imageViewer->NextPrev(1);
		}else
		{
			currentWindow->KeyClick();
		}
	}

	if(key[14] == 1) {  //remote: back
		key[14] = 0;
		if(GetMode() == EN_MODE_IMAGEVIEWER){
			imageViewer->NextPrev(-1);
		}else
		{
			currentWindow->KeyL();
		}
	}

	if(key[15] > 0) {  //remote: volume up

		if(GetMode() == EN_MODE_IMAGEVIEWER){
// 			imageViewer->ScrollFromExtern(0,config->GetPadspeed(-1));
			imageViewer->ScrollFromExtern(0,-1);
		}else
		{
			short tmpkey = this->Keys(15, 10, 14);
			if(tmpkey == 1)
				currentWindow->KeyUp();
			else if(tmpkey == 2)
				currentWindow->KeyUp();
		}
	}

	if(key[16] > 0) {  //remote: volume down
		if(GetMode() == EN_MODE_IMAGEVIEWER){
// 			imageViewer->ScrollFromExtern(0,config->GetPadspeed(1));
			imageViewer->ScrollFromExtern(0,1);
		}else
		{
			short tmpkey = this->Keys(16, 10, 14);
			if(tmpkey == 1)
				currentWindow->KeyDown();
			else if(tmpkey == 2)
				currentWindow->KeyDown();
		}
	}

	if(key[17] == 1) {  //remote: hold
// 		key[17] = 0;
	}
}


void CMain::React2Analog()
{
	short tmpy = SDL_JoystickGetAxis(joystick, 1) / 1000; //GetAnalogy();
	short tmpx = SDL_JoystickGetAxis(joystick, 0) / 1000; //GetAnalogx();
	
	if( (tmpx > -6 & tmpx < 6) & (tmpy > -6 & tmpy < 6) )
		return;

	if(GetMode() == EN_MODE_IMAGEVIEWER){
		imageViewer->ScrollFromExternStick(tmpx,tmpy);
// 		imageViewer->ScrollFromExtern(0,config->GetStickScrollSpeed(tmpy));
// 		imageViewer->ScrollFromExtern(config->GetStickScrollSpeed(tmpx),0);
	}
}



void CMain::InitSDL()
{
	//die videofunktionen initialisieren
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) == -1) {
		std::cout << "Can't init SDL: " << SDL_GetError() << std::endl;
// 		return 1;
	}

	//was passiert wernn das programm beendet wird
	//atexit(SDL_Quit);

	//den curser deaktivieren
	SDL_ShowCursor(SDL_DISABLE);

	// Set video mode.  Any pixel format should work.
	screen = SDL_SetVideoMode(W, H, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (screen == NULL) {
		printf("Can't set video mode: %s\n", SDL_GetError());
// 		return 1;
	}

	//Open the joystick 
	if(SDL_NumJoysticks()) {
		joystick = SDL_JoystickOpen(0);
	} else {
		std::cout<<"No joystick detected" << std::endl;
	}

	//turn the sge update off
	sge_Update_OFF();

}

void CMain::InitSystem(){
	config = new CConfig();
	config->Init();

	imageViewer = new ImageViewer();
	imageViewer->Init();

	

}

void CMain::InitWindows(){
	winStart = new GuiStart();
	winStart->Init();

	winImages = new GuiImages();
	winImages->Init();

	winSettings = new GuiSettings();
	winSettings->Init();

	winBookmarks = new GuiBookmarks();
	winBookmarks->Init();

	winOsdImage = new GuiOsdImage();
	winOsdImage->Init();

	//winImageViewer = new GuiImageViewer();
	//winImageViewer->Init();


	currentWindow = winStart;

	//load settings
	CStuff::Load();
}



void CMain::ChangeMode(short value) {
	mode = value;
	switch(mode){
		case EN_MODE_START:
			currentWindow = winStart;
			break;

		case EN_MODE_IMAGES:
			currentWindow = winImages;
			break;

		case EN_MODE_SETTINGS:
			currentWindow = winSettings;
			break;

		case EN_MODE_BOOKMARKS:
			currentWindow = winBookmarks;
			break;

		case EN_MODE_IMAGEVIEWER:
//			currentWindow = winImageViewer;
			break;
		
		case EN_MODE_OSDIMAGE:
			currentWindow = winOsdImage;
			break;
	}
}

//testumgebung
void CMain::Loop()
{
	FPSmanager *frames = new FPSmanager();
 	SDL_initFramerate(frames);
 	SDL_setFramerate(frames, 60);

	#ifdef PSP
	switch(config->GetCpuspeed()) 
	{
		case 0:   scePowerSetClockFrequency(222, 222, 111); break;
		case 1:   scePowerSetClockFrequency(333, 333, 166); break;
	}
	#endif


	while(true)
	{	
		GetEvents();
		React2Keys();
		React2Analog();


		if(GetMode() == EN_MODE_IMAGEVIEWER)
		{
			if(imageViewer->GetChanged() == 1)
			{
				sge_ClearSurface(screen, SDL_MapRGB(screen->format, 0, 0, 0));
				imageViewer->Render(screen);
				#ifdef PSP
					sceDisplayWaitVblankStart();
				#endif
				SDL_Flip(screen);
// 	SDL_framerateDelay(frames);
			}
			else
			{
				sge_ClearSurface(screen, SDL_MapRGB(screen->format, 0, 0, 0));
				imageViewer->RenderIdle(screen);
				#ifdef PSP
					sceDisplayWaitVblankStart();
				#endif
				SDL_Flip(screen);
	SDL_framerateDelay(frames);
			}

		}
		else if(GetMode() == EN_MODE_EXIT)
		{
			CStuff::Save();
			break;
		}
		else
		{
			sge_ClearSurface(screen, SDL_MapRGB(screen->format, 0, 0, 0));
			currentWindow->Draw(screen);
			#ifdef PSP
				sceDisplayWaitVblankStart();
			#endif
			SDL_Flip(screen);
	
			SDL_framerateDelay(frames);


		}
	}
	delete frames;

}



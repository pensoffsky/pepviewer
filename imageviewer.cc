#include "imageviewer.h"

#include "imageloader.h"
#include "stuff.h"
#include "config.h"
extern CConfig *config;
#include "guiimages.h"
extern GuiImages *winImages;

extern ImageViewer *imageViewer;
extern SDL_mutex *lock;
extern SDL_mutex *lockLoadImage;

#include "main.h"
extern CMain *Main;

ImageViewer::ImageViewer()
{
	
	x = 0;
	y = 0;
	alpha = 255;
	//size = 1.0f;
	resized_loaded = 0;
	original_loaded = 0;
	ausschnitt.x = 0;
	ausschnitt.y = 0;
	ausschnitt.w = 480;
	ausschnitt.h = 272;
	
	zoomstate = 0;
	zwx = W / 2;
	zwy = H / 2;
	zwwidth = 102;
	zwheight = 120;
	
	zmx = 0;
	zmy = 36;
	zmwidth = 170;
	zmheight = 200;

	zoomfaktor = 1.0f;
	m_zoomActive = false;

	changed = 1;
}

ImageViewer::~ImageViewer()
{

}

void ImageViewer::Init(){
	CStuff::SetThreadInput(EN_THREADINPUT_DONOTHING);
	CStuff::SetThreadStatus(EN_THREADSTATUS_WAITINGFORORDERS);

	lock = SDL_CreateMutex();
	lockLoadImage = SDL_CreateMutex();

#ifdef PSP
	t1 = sceKernelCreateThread("CImage::thread1", ImageViewer::thread1, 0x22, 0x10000, 0, NULL);
	if (t1 >= 0)
	{
		sceKernelStartThread(t1, 0, 0);
	}
#else
	t1 = SDL_CreateThread (thread1, lock);
#endif
}


//setzt das zoomwindow in die mitte
void ImageViewer::ResetZoom()
{

	zwx = (W / 2) - (zwwidth / 2);
	zwy = (H / 2) - (zwheight / 2);

	changed = 1;
}

int ImageViewer::GetOriginalWidth()
{
	if (original_loaded == 1) 
		return original->w;
	else
		return 0;

}

int ImageViewer::GetOriginalHeight()
{
	if (original_loaded == 1) 
		return original->h;
	else
		return 0;

}


//decide what width & height the zoomwindow shall have	
void ImageViewer::ChangeZoomstate()
{
	zoomstate++;

	if(zoomstate == 3)
		zoomstate = 0;

	
	switch(zoomstate) {
		case 0:
			zwwidth = 102;
			zwheight = 120;
			zwx -= 21;
			zwy -= 25;	
			break;

		case 1:
			zwwidth=81;
			zwheight=95;
			zwx += 10;
			zwy += 12;	
			break;

		case 2:
			zwwidth = 60;
			zwheight = 70;
			zwx += 11;
			zwy += 13;
			break;
	}

	changed = 1;
}

void ImageViewer::RenderZoom(SDL_Surface* target, short fastMode)
{
	//	calculate the factor from original to resized
	zoomfaktor = (float)original->w  /  (float)resized->w;

	//	get the current scrolled position of the shown image
	short image_pos_x = ausschnitt.x;
	short image_pos_y =	ausschnitt.y;

	//	calculate the coordiantes on the original	
	float tl_src_x = (zwx + image_pos_x) * zoomfaktor;
	float tl_src_y = (zwy + image_pos_y) * zoomfaktor;

	//blitt the original
	sge_BlitTransparent(resized, target, ausschnitt.x, ausschnitt.y, 0, 0, ausschnitt.w, ausschnitt.h, 0, alpha);

	//	blit the target rect on the screen
	sge_Rect(target, zwx, zwy, (zwx + zwwidth), (zwy + zwheight), SDL_MapRGB(target->format, 0, 0, 255));


// 	Uint32 rmask, gmask, bmask, amask;
//     /* SDL interprets each pixel as a 32-bit number, so our masks must depend
//        on the endianness (byte order) of the machine */
// 	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
// 	rmask = 0xff000000;
// 	gmask = 0x00ff0000;
// 	bmask = 0x0000ff00;
// 	amask = 0x000000ff;
// 	#else
// 	rmask = 0x000000ff;
// 	gmask = 0x0000ff00;
// 	bmask = 0x00ff0000;
// 	amask = 0xff000000;
// 	#endif
	SDL_Surface *tmp, *tmp2;
// 	tmp2 = SDL_CreateRGBSurface(SDL_SWSURFACE, (int)(zwwidth * zoomfaktor), (int)(zwheight * zoomfaktor), 32, rmask, gmask, bmask, amask);

	tmp2 = SDL_AllocSurface(SDL_SWSURFACE,
		        (int)(zwwidth * zoomfaktor), (int)(zwheight * zoomfaktor), 16,
				0x001F, 0x07E0, 0xF800,0);

	SDL_Rect src_rect;
	SDL_Rect magnified_rect;

	src_rect.x = (int)tl_src_x;
	src_rect.y = (int)tl_src_y;
	src_rect.w = (int)(zwwidth * zoomfaktor);
	src_rect.h = (int)(zwheight * zoomfaktor);

	//	copy from original to temp2
	SDL_BlitSurface(original, &src_rect, tmp2, NULL);


	

	//convert & copy to 16 bit srf_temp
	
	tmp = tmp2;
// 	tmp = SDL_DisplayFormat(tmp2);
	//	free the temp2 surface
//  	SDL_FreeSurface(tmp2);
 	tmp2 = NULL; 

	if(fastMode == 1)
	{
	//	scale & aa the temp surface to temp2	(it has to fit the magnify window
		tmp2 = sge_transform_surface(tmp, SDL_MapRGB(tmp->format, 0, 0, 0),0, 
					((float)zmwidth / (zwwidth * zoomfaktor)),
					((float)zmheight / (zwheight * zoomfaktor)), 0);
	}
	else
	{
		tmp2 = sge_transform_surface(tmp, SDL_MapRGB(tmp->format, 0, 0, 0),0, 
				((float)zmwidth / (zwwidth * zoomfaktor)),
				((float)zmheight / (zwheight * zoomfaktor)), SGE_TAA);
	}


	//	free the temp surface
 	SDL_FreeSurface(tmp);
 	tmp = NULL; 


	//	blit the scaled temp2 to the screen (inside the magnify window)
	magnified_rect.x = zmx;
	magnified_rect.y = zmy;
	magnified_rect.w = zmwidth;
	magnified_rect.h = zmheight;
//   	SDL_BlitSurface(tmp2, NULL, target, &magnified_rect);
	
// 	if(zoomstate < 3)
		SDL_BlitSurface(tmp2, NULL, target, &magnified_rect);
// 	else
// 		sge_BlitTransparent(tmp2, target, 0, 0, zmx, zmy, zmwidth, zmheight, -1, 230);
	

	//	free the temp2 surface
	SDL_FreeSurface(tmp2);
	tmp2 = NULL; 


	
	//	blit the viewer rect to the screen
	sge_Rect(target, zmx, zmy, (zmx + zmwidth), (zmy + zmheight), SDL_MapRGB(target->format, 255, 0, 0));


	changed = 0;
}

void ImageViewer::ScrollZoom(short x, short y)
{
	//hier werden zwx und zwy angepasst	
	zwx += x;
	//	check for left border
	if(zwx < 0)
	{	
		zwx = 0;
		this->Scroll(x, 0);
		//scrollimage(x, 0, 0, config, SRF_IMAGE);
	}
					
	//	check for right border
	if( (zwx + zwwidth) > W)
	{
		zwx = W - zwwidth; 
		this->Scroll(x, 0);
		//scrollimage(x, 0, 0, config, SRF_IMAGE);
	}
	
	
	zwy += y;
	
	//	check for top border
	if(zwy < 0)
	{	
		zwy = 0;
		this->Scroll(0, y);
// 		scrollimage(0, y, 0, config, SRF_IMAGE);
	}
						
	//	check for bottom border
	if( (zwy + zwheight) > H)
	{
		zwy = H - zwheight; 
		this->Scroll(0, y);
// 		scrollimage(0, y, 0, config, SRF_IMAGE);
	}
		
		
		


	//	calculate the position of the monitor rect
	//	middle right
	if(	((zwx + (zwwidth / 2)) < (W / 2) + 2 ))
	{

		zmx = W - zmwidth;
		zmy = 36;

	}
	
	//	middle left
	else if(	((zwx + (zwwidth / 2)) > (W / 2)))
	{
		zmx = 0;
		zmy = 36;
	}

	

	changed = 1;
}


	
void ImageViewer::FitToOriginal()
{
	//	check surface
	if(original_loaded == 1) 
	{
		if(resized_loaded == 1)
			SDL_FreeSurface(resized);

		resized = sge_copy_surface(original);
		resized_loaded = 1;

		ausschnitt.x = 0;
		ausschnitt.y = 0;
		changed = 1;
	}
}

void ImageViewer::FitToWidth(short width)
{
	//	check surface
	if(original_loaded == 1) 
	{
	
		float fakw;
	
		//	calculate the resize factor
		fakw = (float)width / (float)original->w;
	
		if(resized_loaded == 1)
			SDL_FreeSurface(resized);
	
		//	resize the surface with AA
		resized = sge_transform_surface(original, SDL_MapRGB(original->format, 0, 0, 0), 0, fakw, fakw, SGE_TAA);
		resized_loaded = 1;

		ausschnitt.x = 0;
		ausschnitt.y = 0;
		changed = 1;

	}
}

void ImageViewer::FitToHeight(short height)
{
	//	check surface
	if(original_loaded == 1) 
	{
	
		float fakh;
	
		//	calculate the resize factor
		fakh = (float)(height) / (float)original->h;
		
		if(resized_loaded == 1)
			SDL_FreeSurface(resized);
	
		//	resize the surface with AA
		resized = sge_transform_surface(original, SDL_MapRGB(original->format, 0, 0, 0), 0, fakh, fakh, SGE_TAA);
		resized_loaded = 1;

		ausschnitt.x = 0;
		ausschnitt.y = 0;
		changed = 1;

	}
}

void ImageViewer::Rotate(short grad)
{
	//	check surface
	if(original_loaded == 1) 
	{
		if(resized_loaded == 1)
			SDL_FreeSurface(resized);

		//	rotate
		resized = sge_transform_surface(original, SDL_MapRGB(original->format, 0, 0, 0), (float)grad, 1.0f, 1.0f, 0);
		resized_loaded = 1;	

		SDL_FreeSurface(original);
		
		original  = sge_copy_surface(resized);
		changed = 1;
	}
}


void ImageViewer::NextPrev(short x)
{
	m_zoomActive = false;

	CFileObject *tmpFileObject = NULL;
// SDL_mutexP(lock);
	if(x > 0)
		tmpFileObject = ImagelistNext();
	else if(x == 0)
		tmpFileObject = ImagelistGet(0);
	else if(x < 0)
		tmpFileObject = ImagelistPrevious();
// SDL_mutexV(lock);

	tmpFileObject->Debug();

	//char filenameList[700];
	std::string filename = tmpFileObject->getCompleteFilename();
	//filenameList = tmpFileObject->getCompleteFilename().c_str();
	//std::cout << "filename aus der liste: " << filename << std::endl;

	if(x == 0) //erstes bild
	{
		std::cout << std::endl <<"load direct: " << filename << std::endl;
		this->LoadImage(tmpFileObject);
		
	}
	else if(x > 0)
	{
		std::cout << std::endl <<"   ImageViewer::NextPrev(), x>0" << std::endl;

		if(config->GetPrecaching() > 0)
		{
			std::cout << std::endl <<"	ImageViewer::NextPrev(), precaching active" << std::endl;
			if(CStuff::GetThreadStatus() == EN_THREADSTATUS_LOADING_NEXTIMAGE)
			{
				CStuff::SetThreadInput(EN_THREADINPUT_ABORT_LOADING);
				std::cout << std::endl <<"	ImageViewer::NextPrev(), while(true) start" << std::endl;
				while(true)
				{
					if(tmpFileObject->image_loaded == 1)
						break;
			
			
					if(CStuff::GetThreadStatus() == EN_THREADSTATUS_WAITINGFORORDERS)
						break;
			
					SDL_Delay(50);
				}
				std::cout << std::endl <<"	ImageViewer::NextPrev(), while(true) end" << std::endl;

				
			}

			if(tmpFileObject->image_loaded == 1)
			{
				std::cout << "nextimage vom thread holen!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11" << std::endl << std::endl;
				UnloadImage();
				
				original = tmpFileObject->image;
				original_loaded = 1;
				tmpFileObject->image_loaded = 0;
				tmpFileObject->image = NULL;
	
// 				SDL_mutexP(lock);
				resized = sge_copy_surface(original);
				resized_loaded = 1;
	
				DoImageChanges();
// 				SDL_mutexV(lock);
				ausschnitt.x = 0;
				ausschnitt.y = 0;
				SetInitialPosition();
				changed = 1;
			}
			else //thread did not load the next image
			{
				std::cout << std::endl <<"			load next direct,(th): " << filename << std::endl;
				CStuff::SetThreadInput(EN_THREADINPUT_ABORT_LOADING);
				while(CStuff::GetThreadStatus() == EN_THREADSTATUS_LOADING_NEXTIMAGE)
				{
					SDL_Delay(30);
				}
// 				SDL_mutexP(lockLoadImage);
					this->LoadImage(tmpFileObject);
// 				SDL_mutexV(lockLoadImage);
			}

			

			
		}
		else //if(config->GetPrecaching() > 0)
		{
		//	CStuff::SetThreadInput(EN_THREADINPUT_ABORT_LOADING);
			this->LoadImage(tmpFileObject);
			std::cout << std::endl <<"load next direct: " << filename << std::endl;
		}
		
/*
		if(tmpFileObject->image_loaded == 1)
		{
			std::cout << "nextimage vom thread holen!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11" << std::endl << std::endl;
			UnloadImage();
			
// 			original  = Main->Convertto16bit(tmpFileObject->image);
			original = tmpFileObject->image;
 			original_loaded = 1;
			tmpFileObject->image_loaded = 0;
			tmpFileObject->image = NULL;

// 			SDL_mutexV(lock);

			resized = sge_copy_surface(original);
 			resized_loaded = 1;
 
 			DoImageChanges();
			ausschnitt.x = 0;
			ausschnitt.y = 0;
			SetInitialPosition();
			changed = 1;

		}
		else
		{
			CStuff::SetThreadInput(EN_THREADINPUT_ABORT_LOADING);
				while(CStuff::GetThreadStatus() == EN_THREADSTATUS_LOADING_NEXTIMAGE)
				{
					SDL_Delay(30);
				}
			SDL_mutexP(lockLoadImage);
				this->LoadImage(tmpFileObject);
			SDL_mutexV(lockLoadImage);
			std::cout << std::endl <<"load next direct: " << filename << std::endl;
		}
// 		SDL_mutexV(lock);
*/
	}
	else if(x < 0)
	{
		if(config->GetPrecaching() > 0)
		{
			CStuff::SetThreadInput(EN_THREADINPUT_ABORT_LOADING);
			while(CStuff::GetThreadStatus() == EN_THREADSTATUS_LOADING_NEXTIMAGE)
			{
				SDL_Delay(30);
			}
		}
		
// 		SDL_mutexP(lockLoadImage);
			this->LoadImage(tmpFileObject);
// 		SDL_mutexV(lockLoadImage);
		std::cout << std::endl <<"load previous direct: " << filename << std::endl;
	}

	if(config->GetPrecaching() > 0)
		CStuff::SetThreadInput(EN_THREADINPUT_LOAD_NEXTIMAGE);

}

void ImageViewer::ScrollFromExternStick(short x, short y)
{
	if(m_zoomActive)
	{
		ScrollZoom(config->GetZoomStickspeed(x), config->GetZoomStickspeed(y));
	}
	else
	{
		Scroll(config->GetStickScrollSpeed(x), config->GetStickScrollSpeed(y));
	}

}


void ImageViewer::ScrollFromExtern(short x, short y)
{

	//info wir bekommen hier nur die direction also z.b. 0,-1 fuer up
	if(m_zoomActive)
	{
		
		ScrollZoom(config->GetZoomPadspeed(x), config->GetZoomPadspeed(y));
	}
	else
	{
		Scroll(config->GetPadspeed(x), config->GetPadspeed(y));
// 		Scroll(x, y);
	}

}

void ImageViewer::Scroll(short x, short y)
{
	if(resized_loaded == 1) 
	{
		changed = 1;

		//wenn der x-wert ver�dert wird (LINKS-RECHTS)
		if((x != 0) & (resized->w > W))
		{  
			//den x-wert zum x-auschnitts-wert addieren
			ausschnitt.x = ausschnitt.x + x;
			
			//wenn das bild schon ganz links ist
			if(ausschnitt.x < 0)  
				ausschnitt.x = 0;
			
			//wenn das bild ganz rechts ist  
			if(ausschnitt.x + ausschnitt.w > resized->w)
				ausschnitt.x = resized->w - ausschnitt.w;  
		}
			
		//wenn der y-wert ver�dert wird  (HOCH-RUNTER)
		if((y != 0) & (resized->h > H))
		{  
			//y addieren
			ausschnitt.y = ausschnitt.y + y;
			
			//prft auf den anschlag oben
			if(ausschnitt.y < 0)  
				ausschnitt.y = 0;
			
			//prft auf den anschlag unten
			if(ausschnitt.y + ausschnitt.h > resized->h)
				ausschnitt.y = resized->h - ausschnitt.h;
		}
	}
}


int ImageViewer::LoadImage(CFileObject* fileObj)
{
//	std::cout << "LoadImage:" << filename << std::endl;

	if( this->LoadImageRaw(fileObj) == 1 )
	{
		return 1;
	}
	DoImageChanges();

	ausschnitt.x = 0;
	ausschnitt.y = 0;
	SetInitialPosition();

	return 0;
}

int ImageViewer::LoadImageRaw(CFileObject* fileObj)
{
SDL_mutexP(lockLoadImage);

	if(original_loaded == 1)
	{	
		SDL_FreeSurface(original);
		original_loaded = 0;
	}
	
	if(resized_loaded == 1)
	{	
		SDL_FreeSurface(resized);
		resized_loaded = 0;
	}

	#ifdef PSP
		scePowerSetClockFrequency(333, 333, 166);
	#endif

	if( CStuff::IsJpeg(fileObj->getCompleteFilename().c_str()) == 1 )
		original = CImageLoader::LoadJpeg(fileObj, 0);
	else if ( CStuff::IsPng(fileObj->getCompleteFilename().c_str()) == 1 )
		original = CImageLoader::LoadPng(fileObj, 0);
	else
		original = IMG_Load(fileObj->getCompleteFilename().c_str());
	
	#ifdef PSP
 		switch(config->GetCpuspeed()) 
 		{
 			case 0:   scePowerSetClockFrequency(222, 222, 111); break;
 			case 1:   scePowerSetClockFrequency(333, 333, 166); break;
 		}
	#endif

	if(original == NULL)
	{
		std::cout << "LoadImage failed " << fileObj->getCompleteFilename() << std::endl;
		SDL_mutexV(lockLoadImage);
		return 1;
	}
	else
	{
		original_loaded = 1;
		//std::cout << "LoadImage OK!" << std::endl;
	}
	
	//set the resolution limit 1500x1500 pixel
	//show the logo & error message
	if((original->w * original->h) > (1500 * 1500))
	{
// 		SDL_FreeSurface(original);
// 		original_loaded = 0;

//		original = sge_copy_surface(Main->GetSurfaceBackground());
//		original_loaded = 1;

		//print error message
//		stringRGBA(original, 10, 50, "The image", 200, 200, 0, 255);
// 		stringRGBA(original, 10, 65, filename.c_str(), 200, 200, 0, 255);
// 		stringRGBA(original, 10, 80, "has more than 2,25MPixel!" , 200, 200, 0, 255);
// 		stringRGBA(original, 10, 95, "             (1500x1500)" , 200, 200, 0, 255);
// 		stringRGBA(original, 10, 110, "Just go to the next image!" , 200, 200, 0, 255);

		resized = sge_copy_surface(original);
		resized_loaded = 1;

	}
	else
	{
		if( true/*(this->IsJpeg(filename) != 1) &  (this->IsPng(filename) != 1)*/ )  //no conversion necessary
		{
			//ACHTUNG, nur deaktiviert weil aus jpeg eh schon ein 16bit surface kommt
			if((original->w * original->h) < (1000 * 1000))
			{
				//create a 32bit surface
				SDL_Surface *tmp32;
				tmp32 = CImageLoader::CreateSurface(1, 2);
	
				//convert to 32bit and copy to tmp
				SDL_Surface *tmp;
				tmp = SDL_ConvertSurface(original, tmp32->format, SDL_SWSURFACE);
				SDL_FreeSurface(original);
				original_loaded = 0;
	
				//convert to 16bit and copy to srf_dst
				original = SDL_DisplayFormat(tmp);
				original_loaded = 1;
			
				SDL_FreeSurface(tmp);
				SDL_FreeSurface(tmp32);
			}
			else
			{
				SDL_Surface *tmp;
				tmp = SDL_DisplayFormat(original);
				SDL_FreeSurface(original);
				original_loaded = 0;
	
				original = SDL_DisplayFormat(tmp);
				SDL_FreeSurface(tmp);
				original_loaded = 1;
			}
		}
	
		resized = sge_copy_surface(original);
		resized_loaded = 1;
	}

	ausschnitt.x = 0;
	ausschnitt.y = 0;
	changed = 1;
	
	SDL_mutexV(lockLoadImage);
	return 0;
}


void ImageViewer::DoImageChanges()
{
	switch(config->GetRotation())
	{
		case 1:	//90
			this->Rotate(90);
			break;

		case 2:	//180
			this->Rotate(180);
			break;

		case 3:	//270
			this->Rotate(270);
			break;
	}

	switch(config->GetFitto())
	{
		case 1:	//width
			this->FitToWidth(W);
			break;

		case 2:	//height
			this->FitToHeight(H);
			break;

		case 3:	//doublepage
			this->FitToWidth(960);
			break;

		case 4:	//width + offset
			this->FitToWidth(W + config->GetFittoOffset());
			break;

		case 5:	//height + offset
			this->FitToHeight(H + config->GetFittoOffset());
			break;
	}

	// Y-Center
	if( config->GetTopcenterbottom() == 1)
		this->Scroll( 0, ((resized->h / 2) - (H / 2)));

	// Y-Bottom
	if( config->GetTopcenterbottom() == 2)
		this->Scroll( 0, (resized->h - H ));

	// X-Center
	if( config->GetLeftcenterright() == 1)
		this->Scroll(((resized->w / 2) - (W / 2)), 0);

	// X-Right
	if( config->GetLeftcenterright() == 2)
		this->Scroll((resized->w - W ), 0);


	// X-Offset
	switch(config->GetXoffset())
		{
			case 1:
				this->Scroll( 10, 0);
				break;
	
			case 2:
				this->Scroll( 20, 0);
				break;

			case 3:
				this->Scroll( 30, 0);
				break;

			case 4:
				this->Scroll( 40, 0);
				break;

			case 5:
				this->Scroll( 50, 0);
				break;

			case 6:
				this->Scroll( -10, 0);
				break;

			case 7:
				this->Scroll( -20, 0);
				break;

			case 8:
				this->Scroll( -30, 0);
				break;

			case 9:
				this->Scroll( -40, 0);
				break;

			case 10:
				this->Scroll( -50, 0);
				break;
		}

	// Y-Offset
	switch(config->GetYoffset())
		{
			case 1:
				this->Scroll(0, 10);
				break;
	
			case 2:
				this->Scroll(0, 20);
				break;

			case 3:
				this->Scroll(0, 30);
				break;

			case 4:
				this->Scroll(0, 40);
				break;

			case 5:
				this->Scroll(0, 50);
				break;

			case 6:
				this->Scroll(0, -10);
				break;

			case 7:
				this->Scroll(0, -20);
				break;

			case 8:
				this->Scroll(0, -30);
				break;

			case 9:
				this->Scroll(0, -40);
				break;

			case 10:
				this->Scroll(0, -50);
				break;
		}
}

void ImageViewer::SetInitialPosition()
{
	// Y-Center
	if( config->GetTopcenterbottom() == 1)
		this->Scroll( 0, ((resized->h / 2) - (H / 2)));

	// Y-Bottom
	if( config->GetTopcenterbottom() == 2)
		this->Scroll( 0, (resized->h - H ));

	// X-Center
	if( config->GetLeftcenterright() == 1)
		this->Scroll(((resized->w / 2) - (W / 2)), 0);

	// X-Right
	if( config->GetLeftcenterright() == 2)
		this->Scroll((resized->w - W ), 0);


	// X-Offset
	switch(config->GetXoffset())
		{
			case 1:
				this->Scroll( 10, 0);
				break;
	
			case 2:
				this->Scroll( 20, 0);
				break;

			case 3:
				this->Scroll( 30, 0);
				break;

			case 4:
				this->Scroll( 40, 0);
				break;

			case 5:
				this->Scroll( 50, 0);
				break;

			case 6:
				this->Scroll( -10, 0);
				break;

			case 7:
				this->Scroll( -20, 0);
				break;

			case 8:
				this->Scroll( -30, 0);
				break;

			case 9:
				this->Scroll( -40, 0);
				break;

			case 10:
				this->Scroll( -50, 0);
				break;
		}

	// Y-Offset
	switch(config->GetYoffset())
		{
			case 1:
				this->Scroll(0, 10);
				break;
	
			case 2:
				this->Scroll(0, 20);
				break;

			case 3:
				this->Scroll(0, 30);
				break;

			case 4:
				this->Scroll(0, 40);
				break;

			case 5:
				this->Scroll(0, 50);
				break;

			case 6:
				this->Scroll(0, -10);
				break;

			case 7:
				this->Scroll(0, -20);
				break;

			case 8:
				this->Scroll(0, -30);
				break;

			case 9:
				this->Scroll(0, -40);
				break;

			case 10:
				this->Scroll(0, -50);
				break;
		}
}

void ImageViewer::UnloadImage()
{
	if(original_loaded == 1)
	{	
		SDL_FreeSurface(original);
		original = NULL;
		original_loaded = 0;
	}
	
	if(resized_loaded == 1)
	{	
		SDL_FreeSurface(resized);
		resized = NULL;
		resized_loaded = 0;
	}


// 	if( GetThreadStatus() != EN_THREADSTATUS_LOADING_NEXTIMAGE)
// 	{
// 		UnloadNextImage();
// 	}
	
}


void ImageViewer::UnloadNextImage()
{
// 	for(int i=0; i<3; i++) {
// 		if(nextImage_loaded[i] == 1)
// 		{	
// 			SDL_FreeSurface(nextImage[i]);
// 			nextImage_loaded[i] = 0;
// 			nextImage[i] = NULL;
// 		}
// 	}
}


void ImageViewer::UnloadImageWait()
{
	while( CStuff::GetThreadStatus() != EN_THREADSTATUS_WAITINGFORORDERS)
		SDL_Delay(10);
	UnloadImage();
	
}

/*
SDL_Surface* ImageViewer::CreateSurface(short w, short h)
{
        Uint32 rmask, gmask, bmask, amask;
        / * SDL interprets each pixel as a 32-bit number, so our masks must depend
        on the endianness (byte order) of the machine * /

        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
        #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
        #endif

        SDL_Surface *tmp;

        //create a 32bit surface
        tmp = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, rmask, gmask, bmask, amask);
        return tmp;

}

*/

#ifdef PSP
int ImageViewer::thread1(SceSize args, void *argp)
#else
int ImageViewer::thread1 (void *p)
#endif
{
	std::cout << "thread: started" << std::endl;
 	short thnextpos=1;
	short precachinglevel = 0;
	while( CStuff::GetThreadInput() != EN_THREADINPUT_QUIT )
	{
		switch(CStuff::GetThreadInput())
		{
			case EN_THREADINPUT_ABORT_LOADING:
				CStuff::SetThreadInput(EN_THREADINPUT_DONOTHING);
				CStuff::SetThreadStatus(EN_THREADSTATUS_WAITINGFORORDERS);
				break;
				
		
			case EN_THREADINPUT_LOAD_NEXTIMAGE:
				CStuff::SetThreadInput(EN_THREADINPUT_DONOTHING);
				CStuff::SetThreadStatus(EN_THREADSTATUS_LOADING_NEXTIMAGE);

				
				precachinglevel = config->GetPrecaching();
				
// SDL_mutexP(lock);
				imageViewer->ImagelistUnloadImagesBeforePosition(); //alles bis position leeren
				imageViewer->ImagelistUnloadImagesFromPosition(precachinglevel); //alles nach position+3 leeren
// SDL_mutexV(lock);
				thnextpos = 1;
				while(thnextpos <= precachinglevel & CStuff::GetThreadInput() == EN_THREADINPUT_DONOTHING)
				{

					CFileObject *tmpFileObject;
//  				SDL_mutexP(lock);
					tmpFileObject = imageViewer->ImagelistGet(thnextpos);
// 				SDL_mutexV(lock);
	
					if(tmpFileObject->image_loaded == 0)
					{
// 						Image->ThreadLoadImage(0);
						
						//char filename[700];
						std::string filename = tmpFileObject->getCompleteFilename();
						std::cout << "thread: load: " << filename << " - thnextpos" << thnextpos << std::endl;
						#ifdef PSP
						CStuff::showramusage();
						#endif
						
						SDL_Surface *tmpSurface = NULL;
	//changed thread parameter!!!
				SDL_mutexP(lockLoadImage);
						if( CStuff::IsJpeg(filename) == 1 )
							tmpSurface = CImageLoader::LoadJpeg(tmpFileObject, 0);
						else if ( CStuff::IsPng(filename) == 1 )
							tmpSurface = CImageLoader::LoadPng(tmpFileObject, 0);
				SDL_mutexV(lockLoadImage);
// 						else
// 							tmpSurface = IMG_Load(filename);
	
						if(tmpSurface != NULL)
						{
							if(tmpFileObject){
								tmpFileObject->image = tmpSurface;
								tmpFileObject->image_loaded = 1;
							}
							else
								SDL_FreeSurface(tmpSurface);
						}
	
						std::cout << "thread: loaded: " << std::endl;
						#ifdef PSP
						CStuff::showramusage();
						#endif
					}
					thnextpos++;
				}

				CStuff::SetThreadStatus(EN_THREADSTATUS_WAITINGFORORDERS);
				break;


			case EN_THREADINPUT_LOAD_FUTUREIMAGE: //maybe use this for thumbnail loading?
				CStuff::SetThreadInput(EN_THREADINPUT_DONOTHING);
				CStuff::SetThreadStatus(EN_THREADSTATUS_LOADING_FUTUREIMAGE);
				std::cout << std::endl <<"loading for the future of ..." << std::endl;
// 				Image->ThreadLoadImage(1);
				
				CStuff::SetThreadStatus(EN_THREADSTATUS_WAITINGFORORDERS);
				break;
		}
		SDL_Delay(20);
	}

	std::cout << "thread: finished" << std::endl;

	return 0;
}

void ImageViewer::ThreadLoadImage( short position )
{
	std::cout << "thread: clear nextImage" << std::endl;
	#ifdef PSP
	//CStuff::showramusage();
	#endif


// // // 	if(Image->nextImage_loaded[position] == 1){
// // // 		SDL_FreeSurface(Image->nextImage[position]);
// // // 		Image->nextImage[position] = NULL;
// // // 		Image->nextImage_loaded[position] = 0;	
// // // 	}
// // // 	SDL_Surface *tmpSurface = NULL;
// // // 
// // // 	std::cout << "thread: get filename" << std::endl;
// // // 	#ifdef PSP
// // // 	Main->showramusage();
// // // 	#endif
// // // 
// // // 	char filename[350];
// // // 	Filebrowser->GetNextFilename(filename, position);
// // // 
// // // 	std::cout << "thread: load: " << filename << std::endl;
// // // 	#ifdef PSP
// // // 	Main->showramusage();
// // // 	#endif
// // // 
// // // 	if( Image->IsJpeg(filename) == 1 )
// // // 		tmpSurface = Main->LoadJpeg(filename, 1);
// // // 	else if ( Image->IsPng(filename) == 1 )
// // // 		tmpSurface = Main->LoadPng(filename);
// // // 	else
// // // 		tmpSurface = IMG_Load(filename);
// // // 
// // // 
// // // 	if(tmpSurface != NULL)
// // // 	{
// // // 		Image->nextImage[position] = tmpSurface;
// // // 		Image->nextImage_loaded[position] = 1;
// // // 	}

	std::cout << "thread: loaded" << std::endl;
	#ifdef PSP
	//CStuff::showramusage();
	#endif
}





void ImageViewer::ImagelistInsert(CFileObject *value)
{ 
	Imagelist.push_back( value );
}


CFileObject* ImageViewer::ImagelistNext()
{ 
	imagelistPos++;
	if(imagelistPos < 1)
		imagelistPos = 1;
	if(imagelistPos > Imagelist.size())
		imagelistPos = Imagelist.size();
	return ImagelistGet(0);
}

CFileObject* ImageViewer::ImagelistPrevious()
{ 
	imagelistPos--;
	if(imagelistPos < 1)
		imagelistPos = 1;
	if(imagelistPos > Imagelist.size())
		imagelistPos = Imagelist.size();

	return ImagelistGet(0);
}


CFileObject* ImageViewer::ImagelistGet(short x)
{ 
	//std::cout << "start ImagelistGet - listsize(): "<< Imagelist.size() << std::endl;
	unsigned short i = 1;

	short searchPos = imagelistPos + x;
	if(searchPos > (short)Imagelist.size())
		searchPos = Imagelist.size();

	if(searchPos < 1 )
		searchPos = 1;

// 	std::cout << "searchpos: " << searchPos << std::endl;

	for( ImagelistIt = Imagelist.begin(); ImagelistIt != Imagelist.end(); ImagelistIt++ )
	{
		CFileObject *tmpFileObject = *ImagelistIt;
		if(i == searchPos)
		{	
// 			std::cout << "fileobject found at pos: " << i << std::endl;
			return tmpFileObject;
		}
		i++;
	}
	//std::cout << "end-imagelistget, nothing found" << Imagelist.size() <<std::endl << std::endl;
	return NULL;
}


void ImageViewer::ImagelistEmpty()
{ 
	std::cout << "ImageViewer::ImagelistEmpty(), start empty" << std::endl;

	for( ImagelistIt = Imagelist.begin(); ImagelistIt != Imagelist.end(); ImagelistIt++ )
	{
		CFileObject *tmpFileObject = *ImagelistIt;
		if(tmpFileObject->image_loaded == 1)
			SDL_FreeSurface(tmpFileObject->image);

		delete tmpFileObject;
	}
	Imagelist.erase( Imagelist.begin(), Imagelist.end() );
	std::cout << "ImageViewer::ImagelistEmpty(), end-empty" << Imagelist.size() <<std::endl << std::endl;
}


void ImageViewer::ImagelistDebug()
{ 
	std::cout << "start debug" << std::endl;
	short i = 0;
	for( ImagelistIt = Imagelist.begin(); ImagelistIt != Imagelist.end(); ImagelistIt++ )
	{
		CFileObject *tmp = *ImagelistIt;
		if(i == imagelistPos)
			std::cout << "gewähltes file:     ";
		tmp->Debug();
		i++;
	}
	std::cout << "end-empty" << Imagelist.size() <<std::endl << std::endl;
}

void ImageViewer::ImagelistUnloadImages()
{
	std::cout << "start unloadimages" << std::endl;
SDL_mutexP(lock);
	for( ImagelistIt = Imagelist.begin(); ImagelistIt != Imagelist.end(); ImagelistIt++ )
	{
		CFileObject *tmpFileObject = *ImagelistIt;
		if(tmpFileObject->image_loaded == 1){
			SDL_FreeSurface(tmpFileObject->image);
			tmpFileObject->image_loaded = 0;
		}
	}
SDL_mutexV(lock);
	std::cout << "end-unloadimages" << Imagelist.size() <<std::endl << std::endl;
}

void ImageViewer::ImagelistUnloadImagesBeforePosition()
{ 
	std::cout << "start unloadimagesbeforeposition" << std::endl;
SDL_mutexP(lock);
	int i=1;
	for( ImagelistIt = Imagelist.begin(); ImagelistIt != Imagelist.end(); ImagelistIt++ )
	{
		if(i <= imagelistPos) {
			CFileObject *tmpFileObject = *ImagelistIt;
			if(tmpFileObject->image_loaded == 1){
				SDL_FreeSurface(tmpFileObject->image);
				tmpFileObject->image_loaded = 0;
			}
		}
		i++;
	}
SDL_mutexV(lock);
	std::cout << "end-unloadimagesbeforeposition" << Imagelist.size() <<std::endl << std::endl;
}

void ImageViewer::ImagelistUnloadImagesFromPosition(short offset)
{ 
	std::cout << "start unloadimagesFromPosition" << std::endl;
SDL_mutexP(lock);
	int i=1;
	for( ImagelistIt = Imagelist.begin(); ImagelistIt != Imagelist.end(); ImagelistIt++ )
	{
		if(i > imagelistPos + offset) {
			CFileObject *tmpFileObject = *ImagelistIt;
			if(tmpFileObject->image_loaded == 1){
				SDL_FreeSurface(tmpFileObject->image);
				tmpFileObject->image_loaded = 0;
			}
		}
		i++;
	}
SDL_mutexV(lock);
	std::cout << "end-unloadimagesFromPosition" << Imagelist.size() <<std::endl << std::endl;
}



void ImageViewer::Render(SDL_Surface* target){
	
	if(m_zoomActive)
	{
		RenderZoom(target, 1);
	}
	else
	{
		changed = 0;
		if(original_loaded == 1)
		{
			//sge_BlitTransparent(resized, target, ausschnitt.x, ausschnitt.y, 0, 0, ausschnitt.w, ausschnitt.h, SDL_MapRGB(resized->format, 255, 0, 0), alpha);
			sge_BlitTransparent(resized, target, ausschnitt.x, ausschnitt.y, 0, 0, ausschnitt.w, ausschnitt.h, 0, alpha);	
		}
	}
}

void ImageViewer::RenderIdle(SDL_Surface* target){
	
	if(m_zoomActive)
	{
		RenderZoom(target, 0);
	}
	else
	{
		Render(target);
	}
}



void ImageViewer::ZoomImageReset(){

// 	m_sZoomImage = 0;
}

void ImageViewer::ZoomImageOut(){

// 	m_sZoomImage += 10;
	if(resized_loaded != 1 )
		return;

// 	if((resized->w - config->GetZoomStep()) <= 0);
// 		return;

	short tmpx = ausschnitt.x;
	short tmpy = ausschnitt.y;

	FitToWidth(resized->w - config->GetZoomStep());

	if(resized_loaded != 1)
		return;

	if((resized->w > W) || (resized->h > H))
	{
		ausschnitt.x = tmpx;
		ausschnitt.y = tmpy;
		Scroll(+1, +1);
	}

}

void ImageViewer::ZoomImageIn(){

	if(resized_loaded != 1 )
		return;

// 	if(resized->w == original->w);
// 		return;

	short tmpx = ausschnitt.x;
	short tmpy = ausschnitt.y;


	if( (resized->w + config->GetZoomStep()) > original->w)
		FitToWidth(original->w);
	else
		FitToWidth(resized->w + config->GetZoomStep());

	if(resized_loaded != 1)
		return;

	if((resized->w > W) || (resized->h > H))
	{
		ausschnitt.x = tmpx;
		ausschnitt.y = tmpy;
		Scroll(-1, -1);
	}	
}





void ImageViewer::KeyClick(){
	ZoomImageIn();
}

void ImageViewer::KeyCancel(){
	if(m_zoomActive)
		m_zoomActive = false;
	else
	{
 		CStuff::SetThreadInput(EN_THREADINPUT_ABORT_LOADING);	
		UnloadImage();
		CFileObject* tmpFileObj = ImagelistGet(0);
		std::cout << "ImageViewer::KeyCancel()" << std::endl;
		tmpFileObj->Debug();
		winImages->BrowseToFile(tmpFileObj);
		ImagelistEmpty();
		Main->ChangeMode(EN_MODE_IMAGES);
	}
}

void ImageViewer::KeyUp(){

}

void ImageViewer::KeyDown(){

}

void ImageViewer::KeyLeft(){

}

void ImageViewer::KeyRight(){

}

void ImageViewer::KeyStart(){
	Main->ChangeMode(EN_MODE_OSDIMAGE);	
}

void ImageViewer::KeySelect(){

}

void ImageViewer::KeySquare(){
	if(m_zoomActive)
		ChangeZoomstate();
	else
		m_zoomActive = true;
	
}

void ImageViewer::KeyTriangle(){
	ZoomImageOut();
}

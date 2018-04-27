#pragma once
#include "includes.h"

#include "window.h"

class ImageViewer
{
	protected:
		short x, y, alpha;
		float size;
		short resized_loaded, original_loaded;
		short changed;

		short zoomstate, zwwidth, zwheight, zwx, zwy, zmwidth, zmheight, zmx, zmy;
		float zoomfaktor;
		bool m_zoomActive;

		short m_sZoomImage;

// 		SDL_Thread *t1;

		#ifdef PSP
		SceUID t1; // = sceKernelCreateThread("sound_thread", CImage::thread1, 0x18, 0x10000, 0, NULL);
		static int thread1(SceSize args, void *argp);
		#else
		SDL_Thread *t1;
		static int thread1 (void *p);
		#endif
		

// 		SDL_Surface *nextImage[3];
// 		short nextImage_loaded[3];
		

		SDL_Surface *resized, *original;
		SDL_Rect ausschnitt;

		std::list<CFileObject*> Imagelist;
		std::list<CFileObject*>::iterator ImagelistIt;
		unsigned short imagelistPos;


	public:

		ImageViewer();
		~ImageViewer();

		void Init();

		void KeyClick();
		void KeyCancel();
		void KeyUp();
		void KeyDown();
		void KeyLeft();
		void KeyRight();
		void KeyR(){};
		void KeyL(){};
		void KeySquare();
		void KeyTriangle();
		void KeyStart();
		void KeySelect();


		void ImagelistInsert(CFileObject *value);
		void ImagelistEmpty();
		void ImagelistDebug();
		void SetImagelistPos(short pos){imagelistPos = pos;}
		short GetImagelistPos(){return imagelistPos;}
		CFileObject* ImagelistGet(short x);
		CFileObject* ImagelistNext();
		CFileObject* ImagelistPrevious();
		void ImagelistUnloadImages();
		void ImagelistUnloadImagesBeforePosition();
		void ImagelistUnloadImagesFromPosition(short offset);


		void ThreadLoadImage( short position );

		void pixelstuff();

		//!sets the first position of the ausschnitt after the menu
		void SetInitialPosition();
		void DoImageChanges();

		void SetXY(short valuex, short valuey) {x=valuex; y=valuey;}
		void SetAlpha(short value) {alpha=value;}
		
		short GetChanged() {return changed;}
		void SetChanged() {changed = 1;}
		void ResetChanged() {changed = 0;}
		//float GetSize() {return size;}
		int GetOriginalWidth();
		int GetOriginalHeight();
		
		//!copy the original image into the resized one
		void FitToOriginal();
		//!resize the image to param:width
		void FitToWidth(short width);
		//!resize the image to the height of the psp-screen
		void FitToHeight(short height);
		//!rotate the image
		void Rotate(short grad);

		//!load the previous or next image with the offset param:x
		void NextPrev(short x);
		//!move the ausschnitt inside the resized image
		void Scroll(short x, short y);

		void ScrollFromExtern(short x, short y);
		void ScrollFromExternStick(short x, short y);

		/*!
		*try to load an image from param:filename.
		*free the surfaces. check for resolution limit, convert to 16bit,
		*rotate, resize and offset scroll.
		*/
		int  LoadImage(CFileObject* fileObj);
		//!load image without resizeing
		int  LoadImageRaw(CFileObject* fileObj);
		//!free the two surfaces
		void UnloadImage();
		void UnloadNextImage();
		void UnloadImageWait();
		
		void Render(SDL_Surface* target);
		void RenderIdle(SDL_Surface* target);

		void ZoomImageIn();
		void ZoomImageOut();
		void ZoomImageReset();

		void ChangeZoomstate();
		void ResetZoom();
		void RenderZoom(SDL_Surface* target, short fastMode);
		void ScrollZoom(short x, short y);

};


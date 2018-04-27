#pragma once
#include "includes.h"
#include "fileobject.h"

extern "C" {
	
	#include <jpeglib.h>
	#include <png.h>
}
#include <setjmp.h>


struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;


class CImageLoader{
	protected:
		static void my_error_exit(j_common_ptr cinfo);

		


		static void png_read_data(png_structp ctx, png_bytep area, png_size_t size);

		static SDL_Surface* LoadPngImpl(SDL_RWops *src, short thread);

		static SDL_Surface* LoadJpegImpl(SDL_RWops *src, short thread);

		static SDL_Surface* IMG_LoadJPG_RW(SDL_RWops *src);
	
	public:
		CImageLoader();
		~CImageLoader();

		static SDL_Surface* LoadPng(CFileObject* fileObj, short thread);

		static SDL_Surface* LoadJpeg(CFileObject* fileObj, short thread);

		static SDL_Surface* LoadResource(std::string filename);

		static SDL_Surface* CreateSurface(short w, short h);

		static void SecureSurface(SDL_Surface* original);

};


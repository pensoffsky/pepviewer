#include "imageloader.h"
#include "stuff.h"

CImageLoader::CImageLoader()
{
}


CImageLoader::~CImageLoader()
{
}





#define INPUT_BUFFER_SIZE	4096
typedef struct {
	struct jpeg_source_mgr pub;

	SDL_RWops *ctx;
	Uint8 buffer[INPUT_BUFFER_SIZE];
} my_source_mgr;

/*
 * Initialize source --- called by jpeg_read_header
 * before any data is actually read.
 */
static void init_source (j_decompress_ptr cinfo)
{
	/* We don't actually need to do anything */
	return;
}

/*
 * Fill the input buffer --- called whenever buffer is emptied.
 */
static int fill_input_buffer (j_decompress_ptr cinfo)
{
	my_source_mgr * src = (my_source_mgr *) cinfo->src;
	int nbytes;

	nbytes = SDL_RWread(src->ctx, src->buffer, 1, INPUT_BUFFER_SIZE);
	if (nbytes <= 0) {
		/* Insert a fake EOI marker */
		src->buffer[0] = (Uint8) 0xFF;
		src->buffer[1] = (Uint8) JPEG_EOI;
		nbytes = 2;
	}
	src->pub.next_input_byte = src->buffer;
	src->pub.bytes_in_buffer = nbytes;

	return TRUE;
}


/*
 * Skip data --- used to skip over a potentially large amount of
 * uninteresting data (such as an APPn marker).
 *
 * Writers of suspendable-input applications must note that skip_input_data
 * is not granted the right to give a suspension return.  If the skip extends
 * beyond the data currently in the buffer, the buffer can be marked empty so
 * that the next read will cause a fill_input_buffer call that can suspend.
 * Arranging for additional bytes to be discarded before reloading the input
 * buffer is the application writer's problem.
 */
static void skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
	my_source_mgr * src = (my_source_mgr *) cinfo->src;

	/* Just a dumb implementation for now.	Could use fseek() except
	 * it doesn't work on pipes.  Not clear that being smart is worth
	 * any trouble anyway --- large skips are infrequent.
	 */
	if (num_bytes > 0) {
		while (num_bytes > (long) src->pub.bytes_in_buffer) {
			num_bytes -= (long) src->pub.bytes_in_buffer;
			(void) src->pub.fill_input_buffer(cinfo);
			/* note we assume that fill_input_buffer will never
			 * return FALSE, so suspension need not be handled.
			 */
		}
		src->pub.next_input_byte += (size_t) num_bytes;
		src->pub.bytes_in_buffer -= (size_t) num_bytes;
	}
}

/*
 * Terminate source --- called by jpeg_finish_decompress
 * after all data has been read.
 */
static void term_source (j_decompress_ptr cinfo)
{
	/* We don't actually need to do anything */
	return;
}

/*
 * Prepare for input from a stdio stream.
 * The caller must have already opened the stream, and is responsible
 * for closing it after finishing decompression.
 */
static void jpeg_SDL_RW_src (j_decompress_ptr cinfo, SDL_RWops *ctx)
{
  my_source_mgr *src;

  /* The source object and input buffer are made permanent so that a series
   * of JPEG images can be read from the same file by calling jpeg_stdio_src
   * only before the first one.  (If we discarded the buffer at the end of
   * one image, we'd likely lose the start of the next one.)
   * This makes it unsafe to use this manager and a different source
   * manager serially with the same JPEG object.  Caveat programmer.
   */
  if (cinfo->src == NULL) {	/* first time for this JPEG object? */
    cinfo->src = (struct jpeg_source_mgr *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
				  sizeof(my_source_mgr));
    src = (my_source_mgr *) cinfo->src;
  }

  src = (my_source_mgr *) cinfo->src;
  src->pub.init_source = init_source;
  src->pub.fill_input_buffer = fill_input_buffer;
  src->pub.skip_input_data = skip_input_data;
  src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
  src->pub.term_source = term_source;
  src->ctx = ctx;
  src->pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
  src->pub.next_input_byte = NULL; /* until buffer loaded */
}



//for jpeg stuff
//METHODDEF(void) my_error_exit (j_common_ptr cinfo)
void  CImageLoader::my_error_exit (j_common_ptr cinfo)
{
	/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
	my_error_ptr myerr = (my_error_ptr) cinfo->err;

	/* Always display the message. */
	/* We could postpone this until after returning, if we chose. */
	(*cinfo->err->output_message) (cinfo);

	/* Return control to the setjmp point */
	longjmp(myerr->setjmp_buffer, 1);
}

/* Load a PNG type image from an SDL datasource */
void CImageLoader::png_read_data(png_structp ctx, png_bytep area, png_size_t size)
{
	SDL_RWops *src;
	src = (SDL_RWops *)png_get_io_ptr(ctx);
	SDL_RWread(src, area, size, 1);
}


SDL_Surface* CImageLoader::LoadPngImpl(SDL_RWops *src, short thread)
{
	#ifdef DEBUG
	cout << "LoadPngImpl" << endl;
	#endif
	int start;
	const char *error;
	SDL_Surface* surface;
	png_structp png_ptr;
	png_infop info_ptr;
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;
	Uint32 Rmask;
	Uint32 Gmask;
	Uint32 Bmask;
	Uint32 Amask;
	SDL_Palette *palette;
	png_bytep *volatile row_pointers;
	int i;
	volatile int ckey = -1;
	png_color_16 *transv;

// 	int row;
	float rf = 1.0;
//	SDL_Surface *tmp = NULL;
	short parts = 1;

	SDL_Surface* transportSurface = NULL;
	short surfaceHeight;

	short line = 0;
	short nextYPos = 0;
	short surfaceIndex = 0;



	if ( !src ) {
		/* The error message has been set in SDL_RWFromFile */
		return NULL;
	}
	start = SDL_RWtell(src);


	/* Initialize the data we will clean up when we're done */
	error = NULL;
	png_ptr = NULL; info_ptr = NULL; row_pointers = NULL; surface = NULL;

	/* Create the PNG loading context structure */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
					  NULL,NULL,NULL);
	if (png_ptr == NULL){
		error = "Couldn't allocate memory for PNG file or incompatible PNG dll";
		goto done;
	}

	 /* Allocate/initialize the memory for image information.  REQUIRED. */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		error = "Couldn't create image information for PNG file";
		goto done;
	}

	/* Set error handling if you are using setjmp/longjmp method (this is
	 * the normal method of doing things with libpng).  REQUIRED unless you
	 * set up your own error handlers in png_create_read_struct() earlier.
	 */
	if ( setjmp(png_ptr->jmpbuf) ) {
		error = "Error reading the PNG file.";
		goto done;
	}

	/* Set up the input control */
	png_set_read_fn(png_ptr, src, png_read_data);

	/* Read PNG header info */
	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,
			&color_type, &interlace_type, NULL, NULL);

	//cout << "w: " << width << " h: " << height << " b:" << bit_depth << endl;
	//png_set_sCAL( png_ptr, info_ptr, 1, 0.5, 0.5);

	/* tell libpng to strip 16 bit/color files down to 8 bits/color */
	png_set_strip_16(png_ptr) ;

	/* Extract multiple pixels with bit depths of 1, 2, and 4 from a single
	 * byte into separate bytes (useful for paletted and grayscale images).
	 */
	png_set_packing(png_ptr);

	/* scale greyscale values to the range 0..255 */
	if(color_type == PNG_COLOR_TYPE_GRAY)
		png_set_expand(png_ptr);

	/* For images with a single "transparent colour", set colour key;
	   if more than one index has transparency, or if partially transparent
	   entries exist, use full alpha channel */
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
	        int num_trans;
		Uint8 *trans;
		png_get_tRNS(png_ptr, info_ptr, &trans, &num_trans,
			     &transv);
		if(color_type == PNG_COLOR_TYPE_PALETTE) {
		    /* Check if all tRNS entries are opaque except one */
		    int i, t = -1;
		    for(i = 0; i < num_trans; i++)
			if(trans[i] == 0) {
			    if(t >= 0)
				break;
			    t = i;
			} else if(trans[i] != 255)
			    break;
		    if(i == num_trans) {
			/* exactly one transparent index */
			ckey = t;
		    } else {
			/* more than one transparent index, or translucency */
			png_set_expand(png_ptr);
		    }
		} else
		    ckey = 0; /* actual value will be set later */
	}

	if ( color_type == PNG_COLOR_TYPE_GRAY_ALPHA )
		png_set_gray_to_rgb(png_ptr);

	png_read_update_info(png_ptr, info_ptr);

	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,
			&color_type, &interlace_type, NULL, NULL);
	std::cout << "w: " << width << " h: " << height << " b:" << bit_depth << std::endl;





	/* Allocate the SDL surface to hold the image */
	Rmask = Gmask = Bmask = Amask = 0 ; 
	if ( color_type != PNG_COLOR_TYPE_PALETTE ) {
		if ( SDL_BYTEORDER == SDL_LIL_ENDIAN ) {
			Rmask = 0x000000FF;
			Gmask = 0x0000FF00;
			Bmask = 0x00FF0000;
			Amask = (info_ptr->channels == 4) ? 0xFF000000 : 0;
		} else {
		        int s = (info_ptr->channels == 4) ? 0 : 8;
			Rmask = 0xFF000000 >> s;
			Gmask = 0x00FF0000 >> s;
			Bmask = 0x0000FF00 >> s;
			Amask = 0x000000FF >> s;
		}
	}


//change the target size
	#ifdef DEBUG
	cout << "debug: before calc:" <<endl;
	#endif //DEBUG

	rf = float(1300*1300) / float(width*height);
	if(rf < 1.0)
	{
		rf = sqrt( rf );
		parts = (height*width)/(512*512);
		surfaceHeight = height / parts;

		#ifdef DEBUG
		cout << "tmprf: " << rf << endl;
		cout << "parts: " << parts << endl;
		#endif //DEBUG
/*
		surface = SDL_AllocSurface(SDL_SWSURFACE, (int)(width*rf), (int)(height*rf)-(int)(parts/2),
				bit_depth*info_ptr->channels, Rmask,Gmask,Bmask,Amask);*/
		surface = SDL_AllocSurface(SDL_SWSURFACE,
		        (int)(width*rf), (int)(height*rf)-(int)(parts/2), 16,
				0x001F, 0x07E0, 0xF800,0);
		if ( surface == NULL ) {
			error = "Out of memory";
			goto done;
		}


// 		transportSurface = SDL_AllocSurface(SDL_SWSURFACE, width, surfaceHeight,
// 								bit_depth*info_ptr->channels,
// 								Rmask,Gmask,Bmask,Amask);

		transportSurface = SDL_AllocSurface(SDL_SWSURFACE,
		        width, surfaceHeight, 16,
				0x001F, 0x07E0, 0xF800,0);


		SDL_Surface *tmpline;
 		tmpline = SDL_AllocSurface(SDL_SWSURFACE, width, 1,
			bit_depth*info_ptr->channels, Rmask,Gmask,Bmask,Amask);
 
		palette = tmpline->format->palette;
		if ( palette ) {
			if(color_type == PNG_COLOR_TYPE_GRAY) {
				palette->ncolors = 256;
				for(i = 0; i < 256; i++) {
					palette->colors[i].r = i;
					palette->colors[i].g = i;
					palette->colors[i].b = i;
				}
			} else if (info_ptr->num_palette > 0 ) {
				palette->ncolors = info_ptr->num_palette; 
				for( i=0; i<info_ptr->num_palette; ++i ) {
					palette->colors[i].b = info_ptr->palette[i].blue;
					palette->colors[i].g = info_ptr->palette[i].green;
					palette->colors[i].r = info_ptr->palette[i].red;
				}
			}
		}

		SDL_Rect dst;
		dst.x = 0; dst.y = 0;
		dst.w = width;
		dst.h = 1;


		/* Create the array of pointers to image data */
		row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
		if ( (row_pointers == NULL) ) {
			error = "Out of memory";
			goto done;
		}
	
	
		/* Read the image a single row at a time */
		for (png_uint_32 y = 0; y < height; y++)
		{
			//row_pointers[y] = (png_bytep)(Uint8 *)surface->pixels + y*surface->pitch;
			row_pointers[y] = (png_bytep)(Uint8 *)tmpline->pixels;// + line*transportSurface->pitch;
			png_read_rows(png_ptr, &row_pointers[y], png_bytepp_NULL, 1);

			SDL_BlitSurface(tmpline, NULL, transportSurface, &dst);
			dst.y++;
	
			line++;
			if(line > surfaceHeight-1)
			{
				if(thread == 1)
				{
					if( CStuff::GetThreadInput() == EN_THREADINPUT_ABORT_LOADING )	
					{
						std::cout << "pngload-Got Abort Signal" << std::endl;
						SDL_FreeSurface(surface);
						surface = NULL;
						break;
					}
				}
	
				sge_transform(transportSurface, 
					surface, 0.0, rf, rf, 0, 0, 0, 
					nextYPos, 0);
	
	
				nextYPos += (int)(surfaceHeight*rf);

// 				short outlines = outRect.h -1;
// 				nextYPos += outlines;
// 				nextYPos--;
				surfaceIndex++;
				//cout << "nextYPos: " << nextYPos <<  endl;
				dst.y = line = 0;
			}
		}
	
		SDL_FreeSurface(transportSurface);
		SDL_FreeSurface(tmpline);
		
	}
	else
	{
		#ifdef DEBUG
		cout << "png no resize" << endl;
		#endif

		surface = SDL_AllocSurface(SDL_SWSURFACE,
		        width, height, 16,
				0x001F, 0x07E0, 0xF800,0);
// 		surface = SDL_AllocSurface(SDL_SWSURFACE, width, height,
// 			bit_depth*info_ptr->channels, Rmask,Gmask,Bmask,Amask);
		if ( surface == NULL ) {
			error = "Out of memory";
			goto done;
		}
	

		SDL_Surface *tmpline;
 		tmpline = SDL_AllocSurface(SDL_SWSURFACE, width, 1,
			bit_depth*info_ptr->channels, Rmask,Gmask,Bmask,Amask);
 
		palette = tmpline->format->palette;
		if ( palette ) {
			if(color_type == PNG_COLOR_TYPE_GRAY) {
				palette->ncolors = 256;
				for(i = 0; i < 256; i++) {
					palette->colors[i].r = i;
					palette->colors[i].g = i;
					palette->colors[i].b = i;
				}
			} else if (info_ptr->num_palette > 0 ) {
				palette->ncolors = info_ptr->num_palette; 
				for( i=0; i<info_ptr->num_palette; ++i ) {
					palette->colors[i].b = info_ptr->palette[i].blue;
					palette->colors[i].g = info_ptr->palette[i].green;
					palette->colors[i].r = info_ptr->palette[i].red;
				}
			}
		}

		SDL_Rect dst;
		dst.x = 0; dst.y = 0;
		dst.w = width;
		dst.h = 1;



		/* Create the array of pointers to image data */
		row_pointers = (png_bytep*) malloc(sizeof(png_bytep)*height);
		if ( (row_pointers == NULL) ) {
			error = "Out of memory";
			goto done;
		}
// 		for (row = 0; row < (int)height; row++) {
// 			row_pointers[row] = (png_bytep)
// 					(Uint8 *)surface->pixels + row*surface->pitch;
// 		}
	
		/* Read the entire image in one go */
// 		png_read_image(png_ptr, row_pointers);
	

		/* Read the image a single row at a time */
		for (png_uint_32 y = 0; y < height; y++)
		{
			//row_pointers[y] = (png_bytep)(Uint8 *)surface->pixels + y*surface->pitch;
			row_pointers[y] = (png_bytep)(Uint8 *)tmpline->pixels;// + line*transportSurface->pitch;
			png_read_rows(png_ptr, &row_pointers[y], png_bytepp_NULL, 1);
	
			SDL_BlitSurface(tmpline, NULL, surface, &dst);
 			dst.y++;

			if(thread == 1 & y%30 == 0)
			{
				if( CStuff::GetThreadInput() == EN_THREADINPUT_ABORT_LOADING )	
				{
					std::cout << "pngload-Got Abort Signal" << std::endl;
					SDL_FreeSurface(surface);
					surface = NULL;

					break;
				}
			}

		}

		SDL_FreeSurface(tmpline);

	}



	/* and we're done!  (png_read_end() can be omitted if no processing of
	 * post-IDAT text/time/etc. is desired)
	 * In some cases it can't read PNG's created by some popular programs (ACDSEE),
	 * we do not want to process comments, so we omit png_read_end

	lib.png_read_end(png_ptr, info_ptr);
	*/

	/* Load the palette, if any */
// 	palette = surface->format->palette;
// 	if ( palette ) {
// 	    if(color_type == PNG_COLOR_TYPE_GRAY) {
// 		palette->ncolors = 256;
// 		for(i = 0; i < 256; i++) {
// 		    palette->colors[i].r = i;
// 		    palette->colors[i].g = i;
// 		    palette->colors[i].b = i;
// 		}
// 	    } else if (info_ptr->num_palette > 0 ) {
// 		palette->ncolors = info_ptr->num_palette; 
// 		for( i=0; i<info_ptr->num_palette; ++i ) {
// 		    palette->colors[i].b = info_ptr->palette[i].blue;
// 		    palette->colors[i].g = info_ptr->palette[i].green;
// 		    palette->colors[i].r = info_ptr->palette[i].red;
// 		}
// 	    }
// 	}

done:	/* Clean up and return */
	if ( png_ptr ) {
		png_destroy_read_struct(&png_ptr,
		                        info_ptr ? &info_ptr : (png_infopp)0,
								(png_infopp)0);
	}
	if ( row_pointers ) {
		free(row_pointers);
	}
	if ( error ) {
		SDL_RWseek(src, start, SEEK_SET);
		if ( surface ) {
			SDL_FreeSurface(surface);
			surface = NULL;
		}
		//IMG_QuitPNG();
		IMG_SetError(error);
	} else {
		//IMG_QuitPNG();
	}

	return(surface); 
}


SDL_Surface* CImageLoader::LoadJpeg(CFileObject* fileObj, short thread)
{
// 	SDL_RWops *src = SDL_RWFromFile(fileObj->getCompleteFilename().c_str(), "rb");
// 	SDL_Surface *retSurface = LoadJpegImpl(src, thread);
// 	SDL_RWclose(src);
// 	return (retSurface);
	SDL_RWops *src = NULL;
	SDL_Surface *retSurface = NULL;

	src = SDL_RWFromFile(fileObj->getCompleteFilename().c_str(), "rb");
	retSurface = LoadJpegImpl(src, thread);
	SDL_RWclose(src);

	return (retSurface);
}


SDL_Surface* CImageLoader::LoadPng(CFileObject* fileObj, short thread)
{
	SDL_RWops *src = NULL;
	SDL_Surface *retSurface = NULL;


	src = SDL_RWFromFile(fileObj->getCompleteFilename().c_str(), "rb");
	retSurface = LoadPngImpl(src, thread);
	SDL_RWclose(src);

	return (retSurface);
}



SDL_Surface* CImageLoader::LoadJpegImpl(SDL_RWops *src, short thread)
{
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
//	FILE * infile;
	JSAMPROW rowptr[1];

	SDL_Surface* transportSurface = NULL;
	SDL_Surface* surface = NULL;

// 	if ((infile = fopen(fileObj->getCompleteFilename().c_str(), "rb")) == NULL) 
// 	{
// 		return NULL;
// 	}
	
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	
	if (setjmp(jerr.setjmp_buffer)) 
	{
		jpeg_destroy_decompress(&cinfo);
// 		fclose(infile);
		return NULL;
	}


	//int start = SDL_RWtell(src);


	jpeg_create_decompress(&cinfo);

//insert stuff for src
	jpeg_SDL_RW_src(&cinfo, src);
		

// 	jpeg_stdio_src(&cinfo, infile);




	(void) jpeg_read_header(&cinfo, TRUE);

	float rf = 1.0;
// 	SDL_Surface *tmp = NULL;
	short parts = 1;

	if(cinfo.num_components == 4) {
		/* Set 32-bit Raw output */
		cinfo.out_color_space = JCS_CMYK;
		cinfo.quantize_colors = FALSE;
		jpeg_calc_output_dimensions(&cinfo);

		//SDL_Surface tmp
		/* Allocate an output surface to hold the image */
/*		tmp = SDL_AllocSurface(SDL_SWSURFACE,
		        cinfo.output_width, cinfo.output_height, 32,
			#if SDL_BYTEORDER == SDL_LIL_ENDIAN
				0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
			#else
				0x0000FF00, 0x00FF0000, 0xFF000000, 0x000000FF);
			#endif
*/
	}
	else
	{
		/* Set 24-bit RGB output */
		cinfo.out_color_space = JCS_RGB;
		cinfo.quantize_colors = FALSE;

//  		cinfo.scale_num   = 1;
//  		cinfo.scale_denom = 1;
  		cinfo.dct_method = JDCT_FASTEST;
  		cinfo.do_fancy_upsampling = FALSE;

		jpeg_calc_output_dimensions(&cinfo);

		#ifdef DEBUG
		cout << "debug: image_width: " << cinfo.image_width << endl;
		#endif
		
// 		tmp = SDL_AllocSurface(SDL_SWSURFACE,
// 		        (int)(cinfo.output_width*rf), (int)(cinfo.output_height*rf), 24,
// 				#if SDL_BYTEORDER == SDL_LIL_ENDIAN
// 		                   0x0000FF, 0x00FF00, 0xFF0000,
// 				#else
// 		                   0xFF0000, 0x00FF00, 0x0000FF,
// 				#endif
// 		                   0);
	}
	
	#ifdef DEBUG
	#ifdef PSP
	cout << "jpeg begin: " ;
	showramusage();
	#endif
	#endif //DEBUG

	rf = sqrt( float(1300*1300) / float(cinfo.output_width*cinfo.output_height) );
	if(rf > 1.0)
		rf = 1.0;

	if(rf < 1.0)
	{
		//rf = sqrt( rf );
		parts = (cinfo.output_height*cinfo.output_width)/(512*512);
		if(parts < 1)
			parts = 1;

		short surfaceHeight = cinfo.output_height / parts;

		
// 		surface = SDL_AllocSurface(SDL_SWSURFACE,
// 		        (int)(cinfo.output_width*rf), (int)(cinfo.output_height*rf), 24,
// 				#if SDL_BYTEORDER == SDL_LIL_ENDIAN
// 		                   0x0000FF, 0x00FF00, 0xFF0000,
// 				#else
// 		                   0xFF0000, 0x00FF00, 0x0000FF,
// 				#endif
// 		                   0);

		surface = SDL_AllocSurface(SDL_SWSURFACE,
		        (int)(cinfo.output_width*rf), (int)(cinfo.output_height*rf)-(int)(parts/2), 16,
				0x001F, 0x07E0, 0xF800,0);

		if ( surface == NULL ) {
			std::cout << "Loadjpg: not enough ram to create surface" << std::endl;
		}

// 		sge_FloodFill(surface,0, 0, 255, 255, 255);

		#ifdef DEBUG
		#ifdef PSP
		cout << "jpeg surface16bit created" ;
		showramusage();
		#endif
		#endif


// 		transportSurface = SDL_AllocSurface(SDL_SWSURFACE,
// 			cinfo.output_width, surfaceHeight, 24,
// 			#if SDL_BYTEORDER == SDL_LIL_ENDIAN
// 				0x0000FF, 0x00FF00, 0xFF0000,
// 			#else
// 				0xFF0000, 0x00FF00, 0x0000FF,
// 			#endif
// 				0);

		transportSurface = SDL_AllocSurface(SDL_SWSURFACE,
		        cinfo.output_width, surfaceHeight, 16,
				0x001F, 0x07E0, 0xF800,0);

		#ifdef DEBUG
		#ifdef PSP
		cout << "jpeg transportsurface created" ;
		showramusage();
		#endif
		#endif


		SDL_Surface *tmp24;
 		tmp24 = SDL_AllocSurface(SDL_SWSURFACE,
		        cinfo.output_width, 1, 24,
				#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		                   0x0000FF, 0x00FF00, 0xFF0000,
				#else
		                   0xFF0000, 0x00FF00, 0x0000FF,
				#endif
		                   0);
		SDL_Rect dst;
		dst.x = 0; dst.y = 0;
		dst.w = cinfo.output_width;
		dst.h = 1;


		short line = 0;
		short nextYPos = 0;
		short surfaceIndex = 0;
	
		(void) jpeg_start_decompress(&cinfo);

		#ifdef DEBUG
		#ifdef PSP
		cout << "jpeg surfaces created: " ;
		showramusage();
		#endif
		#endif
	
		/* Decompress the image */
		while ( cinfo.output_scanline < cinfo.output_height ) 
		{

			rowptr[0] = (JSAMPROW)(Uint8 *)tmp24->pixels;
			jpeg_read_scanlines(&cinfo, rowptr, (JDIMENSION) 1);
			//blit the line
			SDL_BlitSurface(tmp24, NULL, transportSurface, &dst);
			dst.y++;
			
// 			rowptr[0] = (JSAMPROW)(Uint8 *)transportSurface->pixels + line * transportSurface->pitch;
// 			//rowptr[0] = (JSAMPROW)(Uint8 *)surface->pixels + cinfo.output_scanline * surface->pitch;
// 			jpeg_read_scanlines(&cinfo, rowptr, (JDIMENSION) 1);
			line++;
			if(line > surfaceHeight-1)
			{
				if(thread == 1)
				{
					if( CStuff::GetThreadInput() == EN_THREADINPUT_ABORT_LOADING )	
					{
						std::cout << "jpegload-Got Abort Signal" << std::endl;
						SDL_FreeSurface(surface);
						surface = NULL;
// // 						Image->SetThreadInput(EN_THREADINPUT_DONOTHING);
						break;
					}
				}
				#ifdef DEBUG
				cout << "line: " << line << "  surfaceHeight: " << surfaceHeight << endl;
				cout << "nextYPos: " << nextYPos <<  endl;
				#endif
				
// 				if(surfaceIndex > 1)
// 					nextYPos--;
				
// 				SDL_Rect test; test.x = 0; test.y = nextYPos;
// 				SDL_BlitSurface(transportSurface, NULL, surface, &test);
 				SDL_Rect outRect = sge_transform(transportSurface, 
 							surface, 0.0, rf, rf, 0, 0, 0, 
 							nextYPos, 0);
	
				nextYPos += (int)(surfaceHeight*rf); 
	/*
				short outlines = outRect.h;// -1;
				nextYPos += outlines;*/


// 				nextYPos--; //nextYPos--;
				surfaceIndex++;

				#ifdef DEBUG
				cout << "nextYPos: " << nextYPos <<  endl;
// 				cout << "outRect.h: " << outRect.h << endl;
				#endif
				line = 0;
				dst.y = 0;
			}
			
		}

		SDL_FreeSurface(transportSurface);
		SDL_FreeSurface(tmp24);
		
	}
	else
	{
		
		surface = SDL_AllocSurface(SDL_SWSURFACE,
		        cinfo.output_width, cinfo.output_height, 16,
				0x001F, 0x07E0, 0xF800,0);
// 		surface = SDL_AllocSurface(SDL_SWSURFACE,
// 		        (int)(cinfo.output_width*rf), (int)(cinfo.output_height*rf), 24,
// 				#if SDL_BYTEORDER == SDL_LIL_ENDIAN
// 		                   0x0000FF, 0x00FF00, 0xFF0000,
// 				#else
// 		                   0xFF0000, 0x00FF00, 0x0000FF,
// 				#endif
// 		                   0);
		if ( surface == NULL ) {
			std::cout << "LoadPngImpl: not enough ram to crate surface" << std::endl;
		}
		
		#ifdef DEBUG
		#ifdef PSP
		cout << "jpeg-noresize surface16bit created" ;
		showramusage();
		#endif
		#endif

		(void) jpeg_start_decompress(&cinfo);

		#ifdef DEBUG
		#ifdef PSP
		cout << "jpeg-noresize decompress started" ;
		showramusage();
		#endif
		#endif

		SDL_Surface *tmp24;
 		tmp24 = SDL_AllocSurface(SDL_SWSURFACE,
		        cinfo.output_width, 1, 24,
				#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		                   0x0000FF, 0x00FF00, 0xFF0000,
				#else
		                   0xFF0000, 0x00FF00, 0x0000FF,
				#endif
		                   0);

		#ifdef DEBUG
		#ifdef PSP
		cout << "jpeg-noresize surface24bit created w:" << cinfo.output_width;
		showramusage();
		#endif
		#endif
// 
		SDL_Rect dst;
		dst.x = 0; dst.y = 0;
		dst.w = (int)(cinfo.output_width*rf);
		dst.h = 1;

		if(thread == 0) 
		{
			/* Decompress the image */
			while ( cinfo.output_scanline < cinfo.output_height ) 
			{
//  				rowptr[0] = (JSAMPROW)(Uint8 *)surface->pixels + cinfo.output_scanline * surface->pitch;
				rowptr[0] = (JSAMPROW)(Uint8 *)tmp24->pixels;
				jpeg_read_scanlines(&cinfo, rowptr, (JDIMENSION) 1);
				//blit the line
				SDL_BlitSurface(tmp24, NULL, surface, &dst);
 				dst.y++;
			}

			#ifdef DEBUG
			#ifdef PSP
			cout << "jpeg-noresize after scanline" ;
			showramusage();
			#endif
			#endif

		}
		else //vom thread aus aufgerufen
		{
			while ( cinfo.output_scanline < cinfo.output_height ) 
			{
				if(cinfo.output_scanline % 100 == 0)
				{
					if( CStuff::GetThreadInput() == EN_THREADINPUT_ABORT_LOADING )	
					{
						std::cout << "jpegload-Got Abort Signal" << std::endl;
						SDL_FreeSurface(surface);
						surface = NULL;
// 						Image->SetThreadInput(EN_THREADINPUT_DONOTHING);
						break;
					}

// 					sceKernelDelayThread(100);
//  					SDL_Delay(10);
				}
// 				rowptr[0] = (JSAMPROW)(Uint8 *)surface->pixels + cinfo.output_scanline * surface->pitch;
// 				jpeg_read_scanlines(&cinfo, rowptr, (JDIMENSION) 1);
				rowptr[0] = (JSAMPROW)(Uint8 *)tmp24->pixels;
				jpeg_read_scanlines(&cinfo, rowptr, (JDIMENSION) 1);
				//blit the line
				SDL_BlitSurface(tmp24, NULL, surface, &dst);
 				dst.y++;
			}
		}

		SDL_FreeSurface(tmp24);
	}


	#ifdef DEBUG
	#ifdef PSP
	cout << "jpeg end: " << endl;
	showramusage();
	#endif
	#endif

	(void) jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
//	fclose(infile);
	return (surface);
}



SDL_Surface* CImageLoader::LoadResource(std::string filename)
{
	SDL_RWops *src = NULL;
	SDL_Surface *retSurface = NULL;

	src = SDL_RWFromFile(filename.c_str(), "rb");
	retSurface = LoadPngImpl(src, 0);
	CImageLoader::SecureSurface(retSurface);
	SDL_RWclose(src);

	return (retSurface);
}


SDL_Surface* CImageLoader::CreateSurface(short w, short h)
{
        Uint32 rmask, gmask, bmask, amask;
        /* SDL interprets each pixel as a 32-bit number, so our masks must depend
        on the endianness (byte order) of the machine */

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


void CImageLoader::SecureSurface(SDL_Surface* original)
{
	if(original == NULL)
		return;	
	if((original->w * original->h) < (1000 * 1000))
	{
		//create a 32bit surface
		SDL_Surface *tmp32;
		tmp32 = CImageLoader::CreateSurface(1, 2);

		//convert to 32bit and copy to tmp
		SDL_Surface *tmp;
		tmp = SDL_ConvertSurface(original, tmp32->format, SDL_SWSURFACE);
		SDL_FreeSurface(original);

		//convert to 16bit and copy to srf_dst
		original = SDL_DisplayFormat(tmp);
	
		SDL_FreeSurface(tmp);
		SDL_FreeSurface(tmp32);
	}
	else
	{
		SDL_Surface *tmp;
		tmp = SDL_DisplayFormat(original);
		SDL_FreeSurface(original);

		original = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
	}
}


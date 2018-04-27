#pragma once

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/sge.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_framerate.h>

#include <string>
#include <iostream>
#include <list>

#ifdef PSP
	#include <malloc.h> //only debug special
	#include <pspdisplay.h>
	#include <psppower.h>
	#include <pspkernel.h>
#endif

#define W 480
#define H 272

struct ColorAlpha
{
	short red;
	short green;
	short blue;
	short alpha;
};

enum  enMode
{
EN_MODE_START=0
, EN_MODE_IMAGES
, EN_MODE_MP3S
, EN_MODE_BOOKMARKS
, EN_MODE_SETTINGS
, EN_MODE_IMAGEVIEWER
, EN_MODE_EXIT
, EN_MODE_OSDIMAGE
};

enum  enThreadInput
{
EN_THREADINPUT_DONOTHING=0
, EN_THREADINPUT_LOAD_NEXTIMAGE
, EN_THREADINPUT_LOAD_FUTUREIMAGE
, EN_THREADINPUT_ABORT_LOADING
, EN_THREADINPUT_QUIT
};

enum  enThreadStatus
{
EN_THREADSTATUS_WAITINGFORORDERS=0
, EN_THREADSTATUS_LOADING_NEXTIMAGE
, EN_THREADSTATUS_LOADING_FUTUREIMAGE
, EN_THREADSTATUS_QUITTING
};


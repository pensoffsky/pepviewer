#pragma once

#include "includes.h"
#include "listbox.h"
#include "combobox.h"
#include "listboxcombo.h"
#include "guistart.h"
#include "guiimages.h"
#include "guisettings.h"
#include "guibookmarks.h"
#include "guiosdimage.h"
#include "window.h"

#include "config.h"


class CMain
{
	protected:
		SDL_Surface *screen;
		SDL_Event event;
		SDL_Joystick *joystick;
		short key[22];

		CListboxCombo *listbox;
		short mode;

		CWindow *currentWindow;
		
		
	public:
		CMain();
		virtual ~CMain();

		void InitSDL();
		void InitSystem();
		void InitWindows();

		void ChangeMode(short value);
		short GetMode(){return mode;}

		void GetEvents();
		short Keys(short keynumber, short waitfirst, short waitrepeat);
		void React2Keys();
		void React2Analog();

		void Loop();
};


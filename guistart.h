#pragma once

#include "window.h"


class GuiStart : public CWindow
{
	protected:
		CListbox *listbox;
		CLabel *labelImages;
		CLabel *labelMP3s;
		CLabel *labelBookmarks;
		CLabel *labelSettings;
		CLabel *labelExit;

		SDL_Surface *logo;

	public:
		GuiStart();
		~GuiStart();

		virtual void Init();

		virtual void KeyClick();
		virtual void KeyCancel();
		virtual void KeyUp();
		virtual void KeyDown();
		virtual void KeyLeft();
		virtual void KeyRight();

		virtual void Draw(SDL_Surface* target);
};



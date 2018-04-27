#pragma once

#include "window.h"


class GuiOsdImage : public CWindow
{
	protected:
		CStackPanel *panelInfo;
		CListbox *listbox;
		CLabel *labelCmdOriginal;
		CLabel *labelCmdRotH;
		CLabel *labelCmdRotW;
		CLabel *labelCmdBookmark;

		CLabelDesigned *labelInfoFilename;
		CLabelDesigned *labelInfoPath;
// 		CLabelDesigned *labelInfoArchive;

	public:
		GuiOsdImage();
		~GuiOsdImage();

		virtual void Init();

		virtual void KeyClick();
		virtual void KeyCancel();
		virtual void KeyUp();
		virtual void KeyDown();
		virtual void KeyLeft();
		virtual void KeyRight();
		virtual void KeyL();
		virtual void KeyR();
		virtual void KeyStart();

		virtual void Draw(SDL_Surface* target);
};



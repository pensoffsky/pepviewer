#pragma once

#include "window.h"


class GuiSettings : public CWindow
{
	protected:
		CListboxCombo *listcombo;
		CStackPanel *panelInfo;

	public:
		GuiSettings();
		~GuiSettings();

		void UpdateSettings();

		virtual void Init();

		virtual void KeyClick();
		virtual void KeyCancel();
		virtual void KeyUp();
		virtual void KeyDown();
		virtual void KeyLeft();
		virtual void KeyRight();
		virtual void KeyL();
		virtual void KeyR();
};



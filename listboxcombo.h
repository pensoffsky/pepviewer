#pragma once

#include "listbox.h"
#include "combobox.h"

class CListboxCombo : public CListbox
{
	protected:
		CCombobox *selectedCombox;
	
	public:
		CListboxCombo();
		~CListboxCombo();

		virtual bool isComboboxOpen();

		virtual void KeyClick();
		virtual void KeyCancel();
		virtual void KeyUp();
		virtual void KeyDown();
		virtual void KeyLeft(){};
		virtual void KeyRight(){};

		virtual void Draw(SDL_Surface* target);
};



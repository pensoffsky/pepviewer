#pragma once

#include "guielement.h"
#include "label.h"
#include "listbox.h"
#include "labeldesigned.h"

class CCombobox : public CLabelDesigned
{
	protected:
		CListbox listbox;
		bool listboxVisible;
		std::string textBase;
	
	public:
		CCombobox();
		~CCombobox();

		virtual void addGuiElement(CGuiElement* element);
		virtual CGuiElement* getSelectedItem();
		
		virtual void setListboxVisible(bool value);
		virtual bool getListboxVisible(){return listboxVisible;}

		virtual void resetPosition();

		virtual CListbox* getListbox(){return &listbox;}
		virtual void SetSelectedElement(short value){listbox.SetSelectedElement(value);}

		virtual void Draw(SDL_Surface* target);

		virtual void setText(std::string value);
		virtual void updateText();
		
		virtual void KeyClick(){};
		virtual void KeyCancel(){};
		virtual void KeyUp();
		virtual void KeyDown();
		virtual void KeyLeft(){};
		virtual void KeyRight(){};
};



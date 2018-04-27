#pragma once
#include "stackpanel.h"
#include "container.h"

class CListbox : public CStackPanel
{

	protected:
		short listtop;
		short maxVisibleElements;
		short selectedElement;
	
		virtual void changeElementColors();
		virtual void MoveUpDown(short y);

		virtual void setElementsVisibility();

	public:
		CListbox();
		~CListbox();

		virtual void addGuiElement(CGuiElement *element);
		virtual void setHeight(short value);

		virtual CGuiElement* getSelectedItem();
		virtual short getSelectedElement(){return selectedElement;}
		virtual short getElementCount(){return listElements.size();}
		virtual void SetSelectedElement(short value);
		virtual CGuiElement* GetItem(short pos);


		virtual void KeyClick(){};
		virtual void KeyCancel(){};
		virtual void KeyUp();
		virtual void KeyDown();
		virtual void KeyLeft();
		virtual void KeyRight();

		virtual void Draw(SDL_Surface* target);
};





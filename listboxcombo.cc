#include "listboxcombo.h"

CListboxCombo::CListboxCombo() : CListbox()
{
	selectedCombox = NULL;
}


CListboxCombo::~CListboxCombo()
{
	clearGuiElements();
}

bool CListboxCombo::isComboboxOpen(){
	if(selectedCombox != NULL){
		return true;
	}
	return false;
}

void CListboxCombo::KeyClick(){
	if(listElements.size() < 1)
		return;

	if(selectedCombox == NULL)
	{
		//KeyCancel();
	
		short i = 1;
		for( itElements = listElements.begin(); itElements != listElements.end(); itElements++ )
		{
			CGuiElement *tmpGuiElement = *itElements;
			if(i  == selectedElement){
				selectedCombox = (CCombobox*)tmpGuiElement;
				selectedCombox->setListboxVisible(true);
			}
			i++;
		}
	}
	else
	{
		KeyCancel();
	}
	
}

void CListboxCombo::KeyCancel(){
	if(selectedCombox != NULL)
	{
		selectedCombox->setListboxVisible(false);
		selectedCombox = NULL;
	}
}

void CListboxCombo::KeyUp(){
	if(selectedCombox == NULL)
		CListbox::KeyUp();
	else
		selectedCombox->KeyUp();
}

void CListboxCombo::KeyDown(){
	if(selectedCombox == NULL)
		CListbox::KeyDown();
	else
		selectedCombox->KeyDown();
}


void CListboxCombo::Draw(SDL_Surface* target){
	if(visible == true){
		CListbox::Draw(target);

		
		if(selectedCombox != NULL){
			selectedCombox->Draw(target);
		}
	}
}


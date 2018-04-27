#include "combobox.h"

CCombobox::CCombobox(): CLabelDesigned()
{
	listboxVisible = false;
}


CCombobox::~CCombobox()
{
	listbox.clearGuiElements();
}


void CCombobox::addGuiElement(CGuiElement* element){
	listbox.addGuiElement(element);	
}

CGuiElement* CCombobox::getSelectedItem(){
	return listbox.getSelectedItem();
}

void CCombobox::KeyUp(){
	if(listboxVisible)
		listbox.KeyUp();
}

void CCombobox::KeyDown(){
	if(listboxVisible)
		listbox.KeyDown();
}


void CCombobox::setListboxVisible(bool value)
{
	listboxVisible = value;
	

// 	if(guiel != NULL)
// 	{
// 		CLabel *la = (CLabel*)guiel;
// 		CLabel::setText(textBase + la->getText());
// 	}
}


void CCombobox::resetPosition()
{
	listbox.SetSelectedElement(1);
}


void CCombobox::setText(std::string value){
	CLabelDesigned::setText(value);
	textBase = value;
}

void CCombobox::updateText(){
	CGuiElement *guiel = listbox.getSelectedItem();
	if(guiel != NULL)
	{
		CLabelDesigned *la = (CLabelDesigned*)guiel;
		CLabelDesigned::setTextAndCreateBackground(textBase + la->getText());
	}
}


void CCombobox::Draw(SDL_Surface* target){
	if(visible == true){
		CLabelDesigned::Draw(target);

		
		if(listboxVisible == true){
			listbox.Draw(target);
		}
	}
}


#include "listbox.h"
#include "label.h"

CListbox::CListbox() : CStackPanel()
{
	selectedElement = 1;
	maxVisibleElements = 1;
	listtop = 1;
}


CListbox::~CListbox()
{
}

void CListbox::addGuiElement(CGuiElement *element){
	CStackPanel::addGuiElement(element);
	maxVisibleElements = (this->getHeight() - (2*marginY)) / element->getHeight();	
	changeElementColors();
}



void CListbox::changeElementColors()
{
	short i = 1;
	
	for( itElements = listElements.begin(); itElements != listElements.end(); itElements++ )
	{
		CLabel *tmpLabel = (CLabel*)*itElements;

		if(i  == selectedElement){
			//tmpLabel->setColor(255,100,100,255);
			tmpLabel->setFocus(true);
			tmpLabel->setScrollable(true);
		}else
		{
			//tmpLabel->setColor(100,100,100, 255);
			tmpLabel->setFocus(false);
			tmpLabel->setScrollable(false);
		}
// 		tmpFileObject->Debug();
// 		//delete tmpFileObject;
// 		//listFilename.erase( listIter );
		i++;
	}
	
	//only the visible elements use surfaces	
	setElementsVisibility();
}


void CListbox::setElementsVisibility()
{
	short scrollpos = 1;	

	for( itElements = listElements.begin(); itElements != listElements.end(); itElements++ )
	{
		CGuiElement *tmpGuiElement = *itElements;
		if( (scrollpos >= listtop) & (scrollpos < listtop + maxVisibleElements) ){
			
			tmpGuiElement->setVisible(true);
		}
		else
		{
			tmpGuiElement->setVisible(false);
		}
		scrollpos++;
	}
}


CGuiElement* CListbox::getSelectedItem(){
	if( listElements.size() > 1 ){

		short i = 1;
		for( itElements = listElements.begin(); itElements != listElements.end(); itElements++ )
		{
			CGuiElement *tmpGuiElement = *itElements;
			if(i  == selectedElement){
				return tmpGuiElement;
			}
			i++;
		}
	}
	return NULL;
}

CGuiElement* CListbox::GetItem(short pos){
	if( listElements.size() > 1 ){

		short i = 1;
		for( itElements = listElements.begin(); itElements != listElements.end(); itElements++ )
		{
			CGuiElement *tmpGuiElement = *itElements;
			if(i  == pos){
				return tmpGuiElement;
			}
			i++;
		}
	}
	return NULL;
}

void CListbox::setHeight(short value){
	CContainer::setHeight(value);
	maxVisibleElements = (value - (2*marginY)) / 10;
	std::cout << "maxvisibleelements: " << maxVisibleElements << std::endl;
	
}


void CListbox::SetSelectedElement(short value){
	MoveUpDown( value - selectedElement );
	changeElementColors();
}


void CListbox::MoveUpDown(short y)
{
	selectedElement = selectedElement + y;
	
	// wenn verzx bei der letzten datei angekommen ist, stoppen
	if(selectedElement >= (short)listElements.size())
		selectedElement = listElements.size();// - 1;
	
	//x darf nich negativ werden
	if(selectedElement < 2)
		selectedElement = 1;
	
	
	// 	wenn markierung ganz unten
	if((selectedElement) > ( listtop + maxVisibleElements) -1)
		listtop = selectedElement - maxVisibleElements + 1;
				
	//	wenn markierung ganz oben
	if((selectedElement) <= listtop )
		listtop = selectedElement;
}


void CListbox::KeyDown()
{
	if( listElements.size() > 1 ){
		MoveUpDown(1);
// 		if( selectedElement < (short)listElements.size() )
// 		{
// 			selectedElement++;
			changeElementColors();
// 		}
	}
// 	std::cout<< "CListbox::KeyDown() - selectedElement: " << selectedElement << std::endl;
}


void CListbox::KeyUp()
{
	if( listElements.size() > 1 ){
		MoveUpDown(-1);
// 		if( selectedElement > 1 )
// 		{
// 			selectedElement--;
			changeElementColors();
// 		}
	}
// 	std::cout<< "CListbox::KeyUp() - selectedElement: " << selectedElement << std::endl;
}

void CListbox::KeyLeft()
{
	if( listElements.size() > 1 ){
		MoveUpDown(-5);
// 		if( selectedElement > 1 )
// 		{
// 			selectedElement--;
			changeElementColors();
// 		}
	}

}

void CListbox::KeyRight()
{
	if( listElements.size() > 1 ){
		MoveUpDown(5);
// 		if( selectedElement > 1 )
// 		{
// 			selectedElement--;
			changeElementColors();
// 		}
	}

}


void CListbox::Draw(SDL_Surface* target){
	if(visible == true){
		sge_RectAlpha(target, x, y, x+width, y+height, colors.red, colors.green, colors.blue, colors.alpha);
	
		short i = 0;
		short scrollpos = 1;	

		for( itElements = listElements.begin(); itElements != listElements.end(); itElements++ )
		{
			if( (scrollpos >= listtop) & (scrollpos < listtop + maxVisibleElements) ){
				CGuiElement *tmpGuiElement = *itElements;
				tmpGuiElement->setX(this->getX() + marginX);
				tmpGuiElement->setY(marginY + (this->getY() + (i * tmpGuiElement->getHeight()) ));
				tmpGuiElement->Draw(target);
				i++;
			}
			scrollpos++;
		}
	}
}

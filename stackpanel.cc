#include "stackpanel.h"

CStackPanel::CStackPanel()
{
// 	std::cout<< "CStackPanel()" << std::endl;
	marginX = marginY = 5;
}

CStackPanel::~CStackPanel()
{
// 	std::cout<< "~CStackPanel()" << std::endl;	
}

void CStackPanel::addGuiElement(CGuiElement *element){
	element->setX(0);
	element->setY(0);
	element->setWidth(this->getWidth() - (2 * marginX));
	
	listElements.push_back( element );
}


void CStackPanel::Draw(SDL_Surface* target){
	if(visible == true){
		//stringRGBA(target, x, y, text.c_str() , colors.red, colors.green, colors.blue, colors.alpha);
		sge_RectAlpha(target, x, y, x+width, y+height, colors.red, colors.green, colors.blue, colors.alpha);
	
		short i = 0;
	
		for( itElements = listElements.begin(); itElements != listElements.end(); itElements++ )
		{
			CGuiElement *tmpGuiElement = *itElements;
			tmpGuiElement->setX(this->getX() + marginX);
			tmpGuiElement->setY(marginY + (this->getY() + (i * tmpGuiElement->getHeight()) ));
			tmpGuiElement->Draw(target);
			i++;
		}
	}
}


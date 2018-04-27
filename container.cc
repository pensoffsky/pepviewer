#include "container.h"

CContainer::CContainer()
{
// 	std::cout<< "CContainer()" << std::endl;
	width=100;
	height=100;
}

CContainer::~CContainer()
{
// 	std::cout<< "~CContainer()" << std::endl;
	clearGuiElements();	
}

void CContainer::addGuiElement(CGuiElement *element){
	listElements.push_back( element );
}

void CContainer::clearGuiElements(){
	for( itElements = listElements.begin(); itElements != listElements.end(); itElements++ )
		{
			CGuiElement *tmpGuiElement = *itElements;
			delete tmpGuiElement;
			
		}
	listElements.clear();
//	std::cout<< "clearGuiElements() size: " << listElements.size() << std::endl;
}


void CContainer::Draw(SDL_Surface* target){
	if(visible == true){
		for( itElements = listElements.begin(); itElements != listElements.end(); itElements++ )
		{
			CGuiElement *tmpGuiElement = *itElements;
			tmpGuiElement->Draw(target);
		}
	}
}


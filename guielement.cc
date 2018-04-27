#include "guielement.h"

CGuiElement::CGuiElement()
{
	//std::cout<< "cguielement()" << std::endl;
	x = y = width = height = 0;
	visible = true;
	enabled = true;
	focus = false;
	colors.red = 255;
	colors.green = 255;
	colors.blue = 255;
	colors.alpha = 255;
	focusColors.red = 255;
	focusColors.green = 0;
	focusColors.blue = 0;
	focusColors.alpha = 255;
}

CGuiElement::~CGuiElement()
{
	//std::cout<< "~cguielement()" << std::endl;
}


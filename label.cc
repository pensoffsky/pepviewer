#include "label.h"

CLabel::CLabel()
{
	this->setHeight(10);
	scrollCounter = 0;
	maxVisibleChars = 0;
	setText("-");
	bScrollText = false;
}

CLabel::~CLabel()
{
	
}


void CLabel::setWidth(short value)
{
	CGuiElement::setWidth(value);
	maxVisibleChars = CalcMaxVisibleChars();
}

short CLabel::CalcMaxVisibleChars()
{
	short tmp = (short)(width/8)-1;
// 	std::cout<<"maxvisiblechars: " << tmp << std::endl;
	return tmp;
}


void CLabel::Draw(SDL_Surface* target){
	std::string tmpstr;
	if(visible == true){
		if( bScrollText &  (maxVisibleChars < (short)text.size()) ){
			//std::cout<< "scroll width" << std::endl;
			 tmpstr = textScroll;
			scrollCounter++;
			if(scrollCounter > 5){
				scrollCounter = 0;
				textScroll = textScroll[textScroll.size()-1] + textScroll;
				textScroll.erase(textScroll.size()-1);
				
				tmpstr = textScroll;
				
			}
			tmpstr.erase((maxVisibleChars));
			//if(getFocus())
			//	stringRGBA(target, x, y, tmpstr.c_str() , focusColors.red, focusColors.green, focusColors.blue, focusColors.alpha);
			//else
			//	stringRGBA(target, x, y, tmpstr.c_str() , colors.red, colors.green, colors.blue, colors.alpha);
		}
		else
		{	
			 tmpstr = text;
			if( maxVisibleChars < (short)text.size() )
				tmpstr.erase((maxVisibleChars));
			//stringRGBA(target, x, y, tmpstr.c_str() , colors.red, colors.green, colors.blue, colors.alpha);
		}

		if(getFocus())
				stringRGBA(target, x, y, tmpstr.c_str() , focusColors.red, focusColors.green, focusColors.blue, focusColors.alpha);
			else
				stringRGBA(target, x, y, tmpstr.c_str() , colors.red, colors.green, colors.blue, colors.alpha);
	}
}


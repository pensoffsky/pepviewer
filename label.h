#pragma once
#include "includes.h"

#include "guielement.h"

//#include <string>
//using namespace std;


class CLabel : public CGuiElement
{

	protected:
		std::string text;
		std::string textScroll;
		bool bScrollText;

		short scrollCounter;
		short maxVisibleChars;
		
		
	public:
		CLabel();
		virtual ~CLabel();

		virtual void setText(std::string value){textScroll = text = value;}
		virtual std::string getText(){return text;}

		virtual void setWidth(short value);
		virtual short CalcMaxVisibleChars();

		virtual void setScrollable(bool value){bScrollText = value;}
		virtual bool getScrollable(){return bScrollText;}

		virtual void Draw(SDL_Surface* target);

};


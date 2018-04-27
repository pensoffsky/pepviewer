#pragma once
#include "includes.h"

class CGuiElement{

	protected:
		short x, y, width, height;
		bool visible, enabled, focus;
		ColorAlpha colors, focusColors;

	public:

		CGuiElement();
		virtual ~CGuiElement();

		virtual void setX(short value){x = value;}
		virtual void setY(short value){y = value;}
		virtual void setWidth(short value){width = value;}
		virtual void setHeight(short value){height = value;}
		virtual void setVisible(bool value){visible = value;}
		virtual void setEnabled(bool value){enabled = value;}
		virtual void setFocus(bool value){focus = value;}
		virtual void setColor(short r, short g, short b, short a){
			colors.red = r;
			colors.green = g;
			colors.blue = b;
			colors.alpha = a;
		};
		virtual void setColor(ColorAlpha value){
			colors = value;
		};

		virtual short getX(){return x;}
		virtual short getY(){return y;}
		virtual short getWidth(){return width;}
		virtual short getHeight(){return height;}
		virtual bool getVisible(){return visible;}
		virtual bool getEnabled(){return enabled;}
		virtual bool getFocus(){return focus;}

		virtual void KeyClick(){};
		virtual void KeyCancel(){};
		virtual void KeyUp(){};
		virtual void KeyDown(){};
		virtual void KeyLeft(){};
		virtual void KeyRight(){};
		virtual void KeyR(){};
		virtual void KeyL(){};
		virtual void KeySquare(){};
		virtual void KeyTriangle(){};
		virtual void KeyStart(){};
		virtual void KeySelect(){};

		virtual void Draw(SDL_Surface* target){};
};


#pragma once
#include "includes.h"

#include "container.h"
#include "guielement.h"




class CStackPanel : public CContainer
{

	protected:
		short marginX, marginY;

	public:
		CStackPanel();
		virtual ~CStackPanel();

		virtual void addGuiElement(CGuiElement *element);

		virtual void setMarginX(short value){marginX = value;}
		virtual void setMarginY(short value){marginY = value;}

		virtual void Draw(SDL_Surface* target);
};


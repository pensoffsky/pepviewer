#pragma once
#include "includes.h"

#include "guielement.h"


class CContainer : public CGuiElement
{

	protected:
		std::list<CGuiElement*> listElements;
		std::list<CGuiElement*>::iterator itElements;

		

	public:
		CContainer();
		virtual ~CContainer();

		virtual void addGuiElement(CGuiElement *element);
		virtual void clearGuiElements();
		virtual void Draw(SDL_Surface* target);
};


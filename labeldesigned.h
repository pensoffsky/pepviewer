#pragma once
#include "includes.h"

#include "label.h"

class CLabelDesigned : public CLabel
{
public:
	CLabelDesigned();
	~CLabelDesigned();

	virtual void setText(std::string value);

	virtual void setTextAndCreateBackground(std::string value);
	virtual void clearBackground();

	virtual void setWidth(short value);
	virtual void setVisible(bool value);

	virtual void Draw(SDL_Surface* target);

protected:
	SDL_Surface* backgroundFocus;
	SDL_Surface* backgroundInactive;
	virtual void createBackground();

	virtual std::string reduceString(std::string input);
};



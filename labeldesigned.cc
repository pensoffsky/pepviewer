#include "labeldesigned.h"

#include "imageloader.h"

CLabelDesigned::CLabelDesigned()
 : CLabel()
{
	height = 11;
	backgroundFocus = NULL;
	backgroundInactive = NULL;
}


CLabelDesigned::~CLabelDesigned()
{
// 	std::cout << "~CLabelDesigned()" << std::endl;
	if(backgroundFocus){
		SDL_FreeSurface(backgroundFocus);
		backgroundFocus = NULL;
	}

	if(backgroundInactive){
		SDL_FreeSurface(backgroundInactive);
		backgroundInactive = NULL;
	}
}


void CLabelDesigned::createBackground()
{
	if(backgroundFocus){
		SDL_FreeSurface(backgroundFocus);
		backgroundFocus = NULL;
	}
	SDL_Surface* tmp = NULL;
	tmp = CImageLoader::CreateSurface(width, height);
	if(!tmp)
		return;
	
	backgroundFocus = SDL_DisplayFormat(tmp);

	SDL_FreeSurface(tmp);
	tmp = NULL;

	sge_ClearSurface(backgroundFocus, SDL_MapRGB(backgroundFocus->format, 100, 100, 100));

	//grafiken draufblitten

	//blit text drauf
	std::string tmpstr = textScroll;
// 	if( maxVisibleChars < (short)text.size() )
// 		tmpstr.erase((maxVisibleChars));

	stringRGBA(backgroundFocus, 0, 2, tmpstr.c_str() , colors.red, colors.green, colors.blue, colors.alpha);



///

	if(backgroundInactive){
		SDL_FreeSurface(backgroundInactive);
		backgroundInactive = NULL;
	}

	tmp = CImageLoader::CreateSurface(width, height);
	if(!tmp)
		return;
	
	backgroundInactive = SDL_DisplayFormat(tmp);

	SDL_FreeSurface(tmp);
	tmp = NULL;

	sge_ClearSurface(backgroundInactive, SDL_MapRGB(backgroundInactive->format, 255, 0, 0));

	//grafiken draufblitten

	//blit text drauf
// 	tmpstr = textScroll;
// 	if( maxVisibleChars < (short)text.size() )
// 		tmpstr.erase((maxVisibleChars));

	stringRGBA(backgroundInactive, 0, 2, tmpstr.c_str() , colors.red, colors.green, colors.blue, colors.alpha);
}


void CLabelDesigned::clearBackground()
{
// 	std::cout << "CLabelDesigned::clearBackground()" << std::endl;

	if(backgroundFocus){
		SDL_FreeSurface(backgroundFocus);
		backgroundFocus = NULL;
	}
	if(backgroundInactive){
		SDL_FreeSurface(backgroundInactive);
		backgroundInactive = NULL;
	}

}

void CLabelDesigned::setVisible(bool value)
{
	if(!value)
// 		createBackground();
// 	else
		clearBackground();
	CLabel::setVisible(value);
}


std::string CLabelDesigned::reduceString(std::string input)
{
	if( maxVisibleChars >= (short)input.size() )
		return input;

	//fixme: wenn maxvisiblechars ziemlich klein ist, dann schmiert er ab
	if( maxVisibleChars > 0 ){
// 		std::cout << "input: " << input << " maxVisibleChars: " << maxVisibleChars << std::endl;
		std::string tmpstr = input.substr(0, 5);
// 		std::cout << "tmpstr: " << tmpstr << std::endl;
		tmpstr = tmpstr + "...";
		tmpstr = tmpstr + input.substr( (input.size() - maxVisibleChars+7) , input.size());
		return tmpstr;
	}
	return input;
}

void CLabelDesigned::setTextAndCreateBackground(std::string value)
{
	CLabel::setText(value);
	textScroll = reduceString(text);
// 	textScroll = reduceString(text);
// 	if(width > 0)
 		this->createBackground();

}

void CLabelDesigned::setText(std::string value)
{
	CLabel::setText(value);
	textScroll = reduceString(text);
// 	if(width > 0)
// 		this->createBackground();

}

void CLabelDesigned::setWidth(short value)
{
	CLabel::setWidth(value);
	textScroll = reduceString(text);
// 	if(width > 0)
// 		this->createBackground();
	
}



void CLabelDesigned::Draw(SDL_Surface* target)
{
	if(getVisible())
	{
		if(backgroundInactive == NULL || backgroundFocus == NULL)
			createBackground();
	
		if(getFocus())
			sge_BlitTransparent(backgroundFocus, target, 0, 0, x, y, backgroundFocus->w, backgroundFocus->h,SDL_MapRGB(backgroundFocus->format, 255, 0, 0), 255);
		else
		{
			sge_BlitTransparent(backgroundInactive, target, 0, 0, x, y, backgroundInactive->w, backgroundInactive->h,SDL_MapRGB(backgroundInactive->format, 255, 0, 0), 255);
			
		}
	}
}


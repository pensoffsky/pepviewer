#include "fileobject.h"

CFileObject::CFileObject()
{
	filetype = 0;
	originalPosition = 0;
	image_loaded = 0;
	filename = "";
	path = "";
}


CFileObject::~CFileObject()
{
//if(image_loaded == 1) 
//		SDL_FreeSurface(image);
}


std::string CFileObject::getCompleteFilename()
{
	if(path == "/")
		return path + filename;
	else
		return path + "/" + filename;
}

std::string CFileObject::getFilename()
{
	return filename;
}


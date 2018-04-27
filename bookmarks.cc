#include "bookmarks.h"


/** Default constructor **/
CBookmarks::CBookmarks()
{
	//count = 14;

}

/** destructor **/
CBookmarks::~CBookmarks()
{

}


//load all the graphics for the menu
void CBookmarks::Init()
{

	std::cout << "CBookmarks::Init()" << std::endl;

}

void CBookmarks::ListInsert(CFileObject *value)
{ 
	list.push_back( value );
}



void CBookmarks::ListEmpty()
{ 
	std::cout << "CBookmarks::ListEmpty, start empty" << std::endl;

	for( listIt = list.begin(); listIt != list.end(); listIt++ )
	{
		CFileObject *tmpFileObject = *listIt;
		if(tmpFileObject->image_loaded == 1)
			SDL_FreeSurface(tmpFileObject->image);

		delete tmpFileObject;
	}
	list.erase( list.begin(), list.end() );
	std::cout << "CBookmarks::ListEmpty, end-empty" << list.size() <<std::endl << std::endl;
}


void CBookmarks::ListDebug()
{ 
	std::cout << "start debug" << std::endl;
	short i = 0;
	for( listIt = list.begin(); listIt != list.end(); listIt++ )
	{
		CFileObject *tmp = *listIt;
// 		if(i == listPos)
// 			std::cout << "gewähltes file:     ";
		tmp->Debug();
		i++;
	}
	std::cout << "end-debug" << list.size() <<std::endl << std::endl;
}





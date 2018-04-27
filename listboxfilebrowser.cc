#include "listboxfilebrowser.h"

CListboxFilebrowser::CListboxFilebrowser() : CListbox()
{
	currentFolder = "/";
}


CListboxFilebrowser::~CListboxFilebrowser()
{
	delete filebrowser;
	clearGuiElements();
}


void CListboxFilebrowser::Init()
{
	filebrowser = new CFilebrowser();
	filebrowser->ReadFolder(currentFolder);

	UpdateLabels();
	
}


void CListboxFilebrowser::UpdateLabels()
{
	clearGuiElements();

	std::list<CFileObject*>* listFileObjects;
	std::list<CFileObject*>::iterator itFileObjects;

	listFileObjects = filebrowser->GetListFiles();

	for( itFileObjects = listFileObjects->begin(); itFileObjects != listFileObjects->end(); itFileObjects++ )
	{
		CFileObject* tmpFileObject = *itFileObjects;
		CLabel *tmpLabel = new CLabelDesigned();
		tmpLabel->setText( tmpFileObject->getFilename() );
		switch(tmpFileObject->getFiletype()){
			case 0:	tmpLabel->setColor(255, 255, 255, 255);
				break;
			case 1:	tmpLabel->setColor(0, 255, 0, 255);
				break;
			case 2:	tmpLabel->setColor(0, 255, 255, 255);
				break;
			case 3:	tmpLabel->setColor(100, 100, 255, 255);
				break;

		}
		addGuiElement( (CGuiElement*) tmpLabel);
	}
	
	changeElementColors();
}

//todo, add rar support
bool CListboxFilebrowser::BrowseToFile(CFileObject* file){
	if( file == NULL )
		return false;

	if( !filebrowser->ChangeDir(file->getPath() ) )
		return false;
	
	currentFolder = file->getPath();
	selectedElement = 1;
	listtop = 1;
	UpdateLabels();

	SetSelectedElement( file->getPos() );
	return true;
}


CFileObject* CListboxFilebrowser::getSelectedFileObject()
{
	short i = 1;
	for( itElements = listElements.begin(); itElements != listElements.end(); itElements++ )
	{
		if(i  == selectedElement){
			break;
		}

		i++;
	}

	std::list<CFileObject*>* listFileObjects;
	std::list<CFileObject*>::iterator itFileObjects;

	listFileObjects = filebrowser->GetListFiles();

	for( itFileObjects = listFileObjects->begin(); itFileObjects != listFileObjects->end(); itFileObjects++ )
	{
		if(i == 1){
			CFileObject* tmpFileObject = *itFileObjects;
			//std::cout << "selectedFile: ";
			//tmpFileObject->Debug();
			return tmpFileObject;
		}
		i--;
	}

	return NULL;
	
}


short CListboxFilebrowser::getFileCount()
{
	
	std::list<CFileObject*>* listFileObjects;
// 	std::list<CFileObject*>::iterator itFileObjects;

// 	int i = 1;

	listFileObjects = filebrowser->GetListFiles();

	return listFileObjects->size();
//todo (nur size????)
// 	for( itFileObjects = listFileObjects->begin(); itFileObjects != listFileObjects->end(); itFileObjects++ )
// 	{
// 		i++;
// 	}
// 	return i;
}

short CListboxFilebrowser::getSelectedFilePosition()
{
	short i = 1;
	for( itElements = listElements.begin(); itElements != listElements.end(); itElements++ )
	{
		if(i  == selectedElement){
			break;
		}

		i++;
	}

	std::list<CFileObject*>* listFileObjects;
	std::list<CFileObject*>::iterator itFileObjects;

	listFileObjects = filebrowser->GetListFiles();
	short pos = 1;

	for( itFileObjects = listFileObjects->begin(); itFileObjects != listFileObjects->end(); itFileObjects++ )
	{
		
		if(i == 1){
			return pos;
		}
		pos++;
		i--;
	}

	return -1;
	
}


CFileObject* CListboxFilebrowser::getSelectedFileObjAtPos(short pos)
{
	std::list<CFileObject*>* listFileObjects;
	std::list<CFileObject*>::iterator itFileObjects;

	listFileObjects = filebrowser->GetListFiles();

	for( itFileObjects = listFileObjects->begin(); itFileObjects != listFileObjects->end(); itFileObjects++ )
	{
		if(pos == 1){
			CFileObject* tmpFileObject = *itFileObjects;
			//std::cout << "selectedFile: ";
			//tmpFileObject->Debug();
			return tmpFileObject;
		}
		pos--;
	}
	return NULL;
	
}


void CListboxFilebrowser::KeyClick(){

	CFileObject *tmpFileObject = getSelectedFileObject();

	if( tmpFileObject != NULL ){

		if(tmpFileObject->getFiletype() == 4)
		{
			if( filebrowser->ReadFolder(currentFolder) )
			{
				if(listlastPos.size() > 0)
				{
					UpdateLabels();
					itlastPos = listlastPos.begin();
					std::cout << "  stack, pop: " << *itlastPos << std::endl;
					SetSelectedElement(*itlastPos);
					listlastPos.pop_front();
				}
				UpdateLabels();
			}
			else if(filebrowser->ReadFolder("/"))
			{
				currentFolder = "/";
				if(listlastPos.size() > 0)
				{
					UpdateLabels();
					itlastPos = listlastPos.begin();
					std::cout << "  stack, pop: " << *itlastPos << std::endl;
					SetSelectedElement(*itlastPos);
					listlastPos.pop_front();
				}
				UpdateLabels();
			}
			
		}
		else if(tmpFileObject->getFiletype() == 1 | tmpFileObject->getFilename() == "..")
		{
			bool up = false; 
			if(tmpFileObject->getFilename() == "..")
				up = true;
		
			if( filebrowser->ChangeDir(tmpFileObject->getCompleteFilename()) )
			{
				UpdateLabels();
				if(up)
				{
					if(listlastPos.size() > 0)
					{
						itlastPos = listlastPos.begin();
						std::cout << "  stack, pop: " << *itlastPos << std::endl;
						SetSelectedElement(*itlastPos);
						listlastPos.pop_front();
						
					}
				}
				else
				{
					listlastPos.push_front(selectedElement);
					std::cout << "  stack, push: " << selectedElement << std::endl;
					selectedElement = 1;
					listtop = 1;
				}
				
				currentFolder = getSelectedFileObject()->getPath();
				std::cout << "setting currentFolder: " << currentFolder << std::endl;
				
			}
			
			UpdateLabels();
		}
	}
}



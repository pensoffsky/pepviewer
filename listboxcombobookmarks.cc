#include "listboxcombobookmarks.h"


CListboxComboBookmarks::CListboxComboBookmarks() : CListboxCombo()
{
	selectedCombox = NULL;
	m_choice = EN_BOOKMARKCHOICE_OPEN;
}


CListboxComboBookmarks::~CListboxComboBookmarks()
{
	RemoveAllBookmarks();
	clearGuiElements();
}

bool CListboxComboBookmarks::isComboboxOpen(){
	if(selectedCombox != NULL){
		return true;
	}
	return false;
}

void CListboxComboBookmarks::Init(){
	CCombobox *bm = new CCombobox();
	bm->setX(300);
	bm->setWidth(50);
	bm->setText("Menu");
	bm->setScrollable(false);
	bm->getListbox()->setX(315);
	bm->getListbox()->setY(15);
	bm->getListbox()->setWidth(150);
	bm->getListbox()->setHeight(150);
	
	CLabel *la1 = new CLabelDesigned();
	CLabel *la2 = new CLabelDesigned();
	CLabel *la3 = new CLabelDesigned();
	la1->setText("Add New BM");
	la2->setText("Save");
	la3->setText("Load");

	bm->addGuiElement( (CGuiElement*)la1);
	bm->addGuiElement( (CGuiElement*)la2);
	bm->addGuiElement( (CGuiElement*)la3);

	addGuiElement((CGuiElement*) bm);

	//nothing in here
	CFileObject *nothing = new CFileObject();
	nothing->setFilename("Menu");
	listBookmarks.push_back( nothing );

}

void CListboxComboBookmarks::InsertNewBookmark(CFileObject* fileObj){

	//eigene file combobox

	CCombobox *bm = new CCombobox();
	bm->setX(300);
// 	bm->setWidth(50);

	bm->setScrollable(false);
	bm->getListbox()->setX(315);
	bm->getListbox()->setY(15);
	bm->getListbox()->setWidth(150);
	bm->getListbox()->setHeight(150);
	
	CLabel *la1 = new CLabelDesigned();
	CLabel *la2 = new CLabelDesigned();
	CLabel *la3 = new CLabelDesigned();
	la1->setText("Open");
	la2->setText("Write");
	la3->setText("Delete");
	
	bm->addGuiElement( (CGuiElement*)la1);
	bm->addGuiElement( (CGuiElement*)la2);
	bm->addGuiElement( (CGuiElement*)la3);

	addGuiElement((CGuiElement*) bm);
	bm->setText(fileObj->getCompleteFilename());

	CFileObject *newFileObj = new CFileObject();
	newFileObj->setFiletype(fileObj->getFiletype());
	newFileObj->setPath( fileObj->getPath() );
	newFileObj->setFilename( fileObj->getFilename() );
	newFileObj->setPos( fileObj->getPos() );
// 	newFileObj->setRar( fileObj->getRar() );
// 	newFileObj->setArchive( fileObj->getArchive() );


	listBookmarks.push_back( newFileObj );
}


enBookmarkChoice CListboxComboBookmarks::GetChoice(){

	return m_choice;
}

CFileObject* CListboxComboBookmarks::GetSelectedBookmark(){
	if(listBookmarks.size() < 1)
		return NULL;

	short i = 1;
	for( itListBookmarks = listBookmarks.begin(); itListBookmarks != listBookmarks.end(); itListBookmarks++ )
	{
		CFileObject *tmp = *itListBookmarks;
		if(i  == selectedElement){
			return tmp;
		}
		i++;
	}
	return NULL;
}

void CListboxComboBookmarks::RemoveAllBookmarks(){
	for( itListBookmarks = listBookmarks.begin(); itListBookmarks != listBookmarks.end(); itListBookmarks++ )
	{
		CFileObject *tmp = *itListBookmarks;
		delete tmp;
	}
	listBookmarks.clear();

	for( itElements = listElements.begin(); itElements != listElements.end(); itElements++ )
	{
		CGuiElement *tmpGuiElement = *itElements;
		delete tmpGuiElement;
	}
	listElements.clear();

	CCombobox *bm = new CCombobox();
	bm->setX(300);
	bm->setWidth(50);
	bm->setText("Menu");
	bm->setScrollable(false);
	bm->getListbox()->setX(315);
	bm->getListbox()->setY(15);
	bm->getListbox()->setWidth(150);
	bm->getListbox()->setHeight(150);
	
	CLabel *la1 = new CLabelDesigned();
	CLabel *la2 = new CLabelDesigned();
	CLabel *la3 = new CLabelDesigned();
	la1->setText("Add New BM");
	la2->setText("Save");
	la3->setText("Load");

	bm->addGuiElement( (CGuiElement*)la1);
	bm->addGuiElement( (CGuiElement*)la2);
	bm->addGuiElement( (CGuiElement*)la3);

	addGuiElement((CGuiElement*) bm);

	//nothing in here
	CFileObject *nothing = new CFileObject();
	nothing->setFilename("Menu");
	listBookmarks.push_back( nothing );

	selectedElement = 1;
	listtop = 1;
}


bool CListboxComboBookmarks::RemoveSelectedBookmark(){
	if(listBookmarks.size() < 1)
		return false;

	if(listBookmarks.size() == 1)
		;

	//071104 select the element above after deletion
	short tmpSelectedElement = selectedElement;
//	KeyUp();

	short i = 1;
	for( itListBookmarks = listBookmarks.begin(); itListBookmarks != listBookmarks.end(); itListBookmarks++ )
	{
		CFileObject *tmp = *itListBookmarks;
		if(i  == tmpSelectedElement){
			listBookmarks.erase(itListBookmarks);
			delete tmp;
			//CListbox::KeyUp();
			break;
		}
		i++;
	}

	i = 1;
	for( itElements = listElements.begin(); itElements != listElements.end(); itElements++ )
	{
		CGuiElement *tmpGuiElement = *itElements;
		if(i  == tmpSelectedElement){
			listElements.erase(itElements);
			delete tmpGuiElement;
			
			break;
		}
		i++;
	}

	
// 	selectedElement = 1;
// 	listtop = 1;
	


	std::cout << listElements.size() << " - " << listBookmarks.size() << std::endl;
CListbox::KeyUp();
	UpdateLabels();
	return true;
}

CFileObject* CListboxComboBookmarks::GetBookmark(short pos){
	if(listBookmarks.size() < 1)
		return NULL;

	short i = 1;
	for( itListBookmarks = listBookmarks.begin(); itListBookmarks != listBookmarks.end(); itListBookmarks++ )
	{
		CFileObject *tmp = *itListBookmarks;
		if(i  == pos){
			return tmp;
		}
		i++;
	}
	return NULL;
}


void CListboxComboBookmarks::UpdateLabels(){
	short i = 1;
	for( itElements = listElements.begin(); itElements != listElements.end(); itElements++ )
	{
		CGuiElement *tmpGuiElement = *itElements;
		
		CCombobox *combox = (CCombobox*)tmpGuiElement;
		CFileObject *pFileobj = GetBookmark(i);
		if(pFileobj != NULL)
			combox->setText(pFileobj->getCompleteFilename());
		i++;
	}
	changeElementColors();

}

void CListboxComboBookmarks::KeyClick(){
	if(listElements.size() < 1)
		return;

	if(selectedCombox == NULL)
	{
		short i = 1;
		for( itElements = listElements.begin(); itElements != listElements.end(); itElements++ )
		{
			CGuiElement *tmpGuiElement = *itElements;
			if(i  == selectedElement){
				selectedCombox = (CCombobox*)tmpGuiElement;
				selectedCombox->setListboxVisible(true);
			}
			i++;
		}
	}
	else
	{
		CLabel *selected = (CLabel*)selectedCombox->getSelectedItem();
		std::cout << "CListboxComboBookmarks::KeyClick(), selected:" << selected->getText() << std::endl;
		
		m_choice = EN_BOOKMARKCHOICE_NOTHING;
		if( selected->getText() == "Open" )
			m_choice = EN_BOOKMARKCHOICE_OPEN;
		else if( selected->getText() == "Delete" )
			m_choice = EN_BOOKMARKCHOICE_DELETE;
		else if( selected->getText() == "Write" )
			m_choice = EN_BOOKMARKCHOICE_WRITE;
		else if( selected->getText() == "Add New BM" )
			m_choice = EN_BOOKMARKCHOICE_ADDNEW;
		else if( selected->getText() == "Save" )
			m_choice = EN_BOOKMARKCHOICE_SAVE;
		else if( selected->getText() == "Load" )
			m_choice = EN_BOOKMARKCHOICE_LOAD;

		KeyCancel();
	}
}

void CListboxComboBookmarks::KeyCancel(){
	if(selectedCombox != NULL)
	{
		selectedCombox->setListboxVisible(false);
		selectedCombox->resetPosition();
		selectedCombox = NULL;
	}
}

void CListboxComboBookmarks::KeyUp(){
	if(selectedCombox == NULL)
		CListbox::KeyUp();
	else
		selectedCombox->KeyUp();
}

void CListboxComboBookmarks::KeyDown(){
	if(selectedCombox == NULL)
		CListbox::KeyDown();
	else
		selectedCombox->KeyDown();
}


void CListboxComboBookmarks::Draw(SDL_Surface* target){
	if(visible == true){
		CListbox::Draw(target);

		
		if(selectedCombox != NULL){
			selectedCombox->Draw(target);
		}
	}
}


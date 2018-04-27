#pragma once

#include "listboxcombo.h"
#include "fileobject.h"


enum  enBookmarkChoice
{
  EN_BOOKMARKCHOICE_OPEN=0
, EN_BOOKMARKCHOICE_DELETE
, EN_BOOKMARKCHOICE_WRITE
, EN_BOOKMARKCHOICE_ADDNEW
, EN_BOOKMARKCHOICE_SAVE
, EN_BOOKMARKCHOICE_LOAD
, EN_BOOKMARKCHOICE_NOTHING
};



class CListboxComboBookmarks : public CListboxCombo
{
	protected:
		std::list<CFileObject*> listBookmarks;
		std::list<CFileObject*>::iterator itListBookmarks;

		enBookmarkChoice m_choice;

	public:
		CListboxComboBookmarks();
		~CListboxComboBookmarks();

		virtual bool isComboboxOpen();
		virtual void Init();

		virtual void InsertNewBookmark(CFileObject* fileObj);
		virtual enBookmarkChoice GetChoice();
		virtual CFileObject* GetSelectedBookmark();
		virtual bool RemoveSelectedBookmark();
		virtual CFileObject* GetBookmark(short pos);
		virtual void UpdateLabels();
		virtual short GetBookmarkCount(){return listBookmarks.size();}

		virtual void KeyClick();
		virtual void KeyCancel();
		virtual void KeyUp();
		virtual void KeyDown();
		virtual void KeyLeft(){};
		virtual void KeyRight(){};

		virtual void Draw(SDL_Surface* target);
};



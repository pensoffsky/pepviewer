#pragma once

#include "window.h"


class GuiBookmarks : public CWindow
{
	protected:
		CListboxComboBookmarks *listcombobookmarks;
		CStackPanel *panelInfo;

	public:
		GuiBookmarks();
		~GuiBookmarks();

		virtual void Init();

		//interface for save/load
		virtual short GetBookmarkCount(){return listcombobookmarks->GetBookmarkCount();}
		virtual CFileObject* GetBookmark(short pos){return listcombobookmarks->GetBookmark(pos);};
		virtual void InsertNewBookmark(CFileObject* fileObj){listcombobookmarks->InsertNewBookmark(fileObj);}


		virtual void KeyClick();
		virtual void KeyCancel();
		virtual void KeyUp();
		virtual void KeyDown();
		virtual void KeyLeft();
		virtual void KeyRight();
		virtual void KeySquare();
		virtual void KeyTriangle();
		virtual void KeyL();
		virtual void KeyR();
};



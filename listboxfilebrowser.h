#pragma once

#include "listbox.h"
#include "filebrowser.h"
#include "label.h"
#include "labeldesigned.h"


class CListboxFilebrowser : public CListbox
{
	protected:
		CFilebrowser *filebrowser;

		std::string currentFolder;
		virtual void UpdateLabels();

		std::list<CLabel*> listlabels;
		std::list<CLabel*>::iterator itLabels;

		std::list<short> listlastPos;
		std::list<short>::iterator itlastPos;

	public:
		CListboxFilebrowser();
		~CListboxFilebrowser();

		virtual void Init();

		virtual bool BrowseToFile(CFileObject* file);

		virtual CFileObject* getSelectedFileObject();
		virtual short getFileCount();
		virtual short getSelectedFilePosition();
		virtual CFileObject* getSelectedFileObjAtPos(short pos);

		virtual std::string getCurrentFolder(){return currentFolder;}
		virtual void setCurrentFolder(std::string path){currentFolder = path;}

		virtual void KeyClick();
// 		virtual void KeyCancel();
// 		virtual void KeyUp();
// 		virtual void KeyDown();
// 		virtual void KeyLeft();
// 		virtual void KeyRight();
};


#pragma once

#include "window.h"



class GuiImages : public CWindow
{
	protected:
		CListboxFilebrowser *listboxFilemanager;
		CStackPanel *panelFolderInfo;
		CStackPanel *panelFileInfo;
		CStackPanel *panelCurrentFolder;
		
		CLabelDesigned *labelFolderInfo;
		CLabelDesigned *labelFolderInfoFilecount;
		CLabelDesigned *labelFileInfo;
		CLabelDesigned *labelCurrentFolder;

		virtual void UpdateFolderInfo();

	public:
		GuiImages();
		~GuiImages();

		virtual CFileObject* GetSelectedFileObject(){return listboxFilemanager->getSelectedFileObject();}
		
		virtual std::string getCurrentFolder(){return listboxFilemanager->getCurrentFolder();}
		virtual void setCurrentFolder(std::string path);

		virtual bool BrowseToFile(CFileObject *file);

		virtual void Init();

		virtual void KeyClick();
		virtual void KeyCancel();
		virtual void KeyUp();
		virtual void KeyDown();
		virtual void KeyLeft();
		virtual void KeyRight();

		virtual void KeyR();
		virtual void KeyL();

};



#pragma once
#include "includes.h"

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>


#include "fileobject.h"

class CFilebrowser{
	protected:
		short dindex;			//anzahl der files in der dateiliste
		short verzx;			//die aktuelle position im ordner

// 		short currentSelection;
		
		void SortFilenames();
		CFileObject* RemoveSmallestFileObj();

		void CreateFilename(short position);
		
		short CheckSuffix(std::string filename);
		bool ReadFiletypes();
		void ClearlistFiles();

		std::list<CFileObject*> listFiles;
		std::list<CFileObject*>::iterator itFiles;

	public:
		CFilebrowser();
		~CFilebrowser();

		bool ReadRarFile(std::string filename);
		bool ChangeDir(std::string path);
		bool ReadFolder(std::string pathToFolder);

		std::list<CFileObject*>* GetListFiles(){return &listFiles;}
		std::list<CFileObject*>::iterator* GetListFilesIterator(){return &itFiles;}

		

};





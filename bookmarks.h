#pragma once
#include "includes.h"
#include "window.h"

class CBookmarks
{
	private:

		short count;

		std::list<CFileObject*> list;
		std::list<CFileObject*>::iterator listIt;

	public:
		CBookmarks();
		~CBookmarks();
		void Init();

		void ListInsert(CFileObject *value);
		void ListEmpty();
		void ListDebug();
};


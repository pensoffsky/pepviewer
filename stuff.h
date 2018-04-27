#pragma once
#include "includes.h"
#include "config.h"
#include "fileobject.h"


class CStuff{

	protected:
		static short threadInput, threadStatus;
	public:

		CStuff();
		virtual ~CStuff();

		static void Init();

		static void SetThreadStatus( short status );
		static short GetThreadStatus();
		
		static void SetThreadInput( short input );
		static short GetThreadInput();

		static int IsPng(std::string filename);
		static int IsJpeg(std::string filename);

		static void Save();
		static void Load();

		static void WriteFileObject(FILE *file, CFileObject* pFileObj);
		static CFileObject* ReadFileObject(FILE *file);

		static void showramusage();
};


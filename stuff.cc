#include "stuff.h"
extern SDL_mutex *lock;
extern CConfig *config;
#include "guibookmarks.h"
extern GuiBookmarks *winBookmarks;
#include "fileobject.h"
#include "guiimages.h"
extern GuiImages *winImages;
#include "guisettings.h"
extern GuiSettings *winSettings;

short CStuff::threadInput;
short CStuff::threadStatus;

CStuff::CStuff()
{
	
}

CStuff::~CStuff()
{
}



void CStuff::SetThreadStatus( short status )
{
	SDL_mutexP(lock);
	CStuff::threadStatus = status;
 	SDL_mutexV(lock);
}

short CStuff::GetThreadStatus()
{
	short tmp;
	SDL_mutexP(lock);
 	tmp = CStuff::threadStatus;
 	SDL_mutexV(lock);
	return tmp;
}

void CStuff::SetThreadInput( short input )
{
	SDL_mutexP(lock);
	CStuff::threadInput = input;
 	SDL_mutexV(lock);
}

short CStuff::GetThreadInput()
{
	short tmp;
	SDL_mutexP(lock);
 	tmp = CStuff::threadInput;
 	SDL_mutexV(lock);
	return tmp;
}


int CStuff::IsJpeg(std::string filename)
{
	//check if the image is a jpeg or something else
	const char *ext = strrchr(filename.c_str(), '.');
 	if(!ext)
 		return 0;
	char jpegList[4][7] = { {".jpg\0"},{".JPG\0"},{".jpeg\0"},{".JPEG\0"} };
	short jpegsx = 3;
	while(0 <= jpegsx)
	{
//  		printf("vergleichex %s mit %s\n", ext, jpegList[jpegsx]);
		if(strcmp(ext, jpegList[jpegsx]) == 0)
			return 1;
		jpegsx--;
	}
	return 0;
}

int CStuff::IsPng(std::string filename)
{
	//check if the image is a jpeg or something else
	const char *ext = strrchr(filename.c_str(), '.');
 	if(!ext)
 		return 0;
	char jpegList[2][7] = { {".png\0"},{".PNG\0"} };
	short jpegsx = 2;
	while(0 <= jpegsx)
	{
//  		printf("vergleichex %s mit %s\n", ext, jpegList[jpegsx]);
		if(strcmp(ext, jpegList[jpegsx]) == 0)
			return 1;
		jpegsx--;
	}
	return 0;
}



void CStuff::WriteFileObject(FILE *file, CFileObject* pFileObj){
	//fprintf(file, "%s\n", pFileObj->getFilename().c_str()); file, pFileObj->getFilename().size()
	fputs(pFileObj->getFilename().c_str(), file);
	fprintf(file, "\n");
	fputs(pFileObj->getPath().c_str(), file);
	fprintf(file, "\n");
	
	//archive	
// 	std::string strTmp = "x";
// 	if(pFileObj->getArchive() != "")
// 		strTmp = pFileObj->getArchive();


// 	fputs(strTmp.c_str(), file);
	//fputs(pFileObj->getArchive().c_str(), file);
	fprintf(file, "\n");
	
	fprintf(file, "%d\n", pFileObj->getFiletype());
	fprintf(file, "%d\n", pFileObj->getPos());
	
// 	short tmp = 0;
// 	if(pFileObj->getRar())
// 		tmp = 1;
// 
// 	fprintf(file, "%d\n", tmp);
	
}


CFileObject* CStuff::ReadFileObject(FILE *file){
	CFileObject *pFileObj = new CFileObject();
	char *strTmp = new char[5000];
	int tmp;

	//fscanf(file, "%s\n", strTmp);
	for(short i=0; i<5000; i++)
	strTmp[i] = '\0';
	fgets( strTmp, 5000, file);
	for(int i=0; i<4999; i++)
	{
		if(strTmp[i] == '\n'){
			strTmp[i] = '\0';
			break;
		}
	}
	pFileObj->setFilename(strTmp);

	for(short i=0; i<5000; i++)
		strTmp[i] = '\0';
	fgets( strTmp, 5000, file);
	for(int i=0; i<4999; i++)
	{
		if(strTmp[i] == '\n'){
			strTmp[i] = '\0';
			break;
		}
	}
	pFileObj->setPath(strTmp);

/*	//archive
	for(short i=0; i<5000; i++)
		strTmp[i] = '\0';
	fgets( strTmp, 5000, file);
	for(int i=0; i<4999; i++)
	{
		if(strTmp[i] == '\n'){
			strTmp[i] = '\0';
			break;
		}
	}
	//if(strTmp != "x")
		pFileObj->setArchive(strTmp);
*/


	fscanf(file, "%d\n", &tmp);
	pFileObj->setFiletype(tmp);

	fscanf(file, "%d\n", &tmp);
	pFileObj->setPos(tmp);


/*
	fscanf(file, "%d\n", &tmp);
	if(tmp == 0)
		pFileObj->setRar(false);
	else
		pFileObj->setRar(true);
*/
	
	//pFileObj->Debug();

	delete strTmp;

// 	fprintf(file, "%s\n", pFileObj->getPath().c_str());
// 	fprintf(file, "%d\n", pFileObj->getFiletype());
// 	fprintf(file, "%d\n", pFileObj->getPos());
	return pFileObj;
}





void CStuff::Save()
{
	FILE *datei;

	//open the file
	datei = fopen("save.txt", "w+t");

	if(datei == NULL)
		return;

	int i;
	//write settings
	for(i=0; i<=config->GetCount(); i++)
	{
		fprintf(datei, "%d\n", config->GetSettings(i));
	}

	//write bookmarks
	fprintf(datei, "%d\n", winBookmarks->GetBookmarkCount()-1);
	for(i=2; i<=winBookmarks->GetBookmarkCount(); i++)
	{
		CFileObject *pFileObj = winBookmarks->GetBookmark(i);
		CStuff::WriteFileObject(datei, pFileObj);
	}

	//write current file
	CStuff::WriteFileObject(datei, winImages->GetSelectedFileObject());
	
	//write the current folder (fix for startup with rar)
	fputs(winImages->getCurrentFolder().c_str(), datei);
	fprintf(datei, "\n");

	fclose(datei);
}


void CStuff::Load()
{
	FILE *datei;

	datei = fopen("save.txt", "rt");
	//wennn die datei nicht ge�fnet werden kann dann return 3
	if(datei == NULL)
	{
// 		printf("Load(): could not find save.txt\n");
// 		Filebrowser->SetCurrentPosition(0);
// 		Filebrowser->ReadDir();
// 		Filebrowser->MoveUpDown(0);
		return;
	}

	int i;
	int tmp;
	CFileObject* pFileObj;
	//load settings
	for(i=0; i<=config->GetCount(); i++)
	{
		fscanf(datei, "%d\n", &tmp);
		config->SetSettings(i, tmp);
		//fprintf(datei, ",");
	}
	winSettings->UpdateSettings();

	//load bookmarks
	fscanf(datei, "%d\n", &tmp);
	for(i=0; i<tmp; i++)
	{
		pFileObj = CStuff::ReadFileObject(datei);
		winBookmarks->InsertNewBookmark(pFileObj);
		if(pFileObj)
			delete pFileObj;
	}

	//load currentfile
	pFileObj = CStuff::ReadFileObject(datei);
	pFileObj->Debug();
	winImages->BrowseToFile(pFileObj);	
	if(pFileObj)
		delete pFileObj;



	//read the current folder (fix for startup with rar)
	char *strTmp = new char[5000];
	for(short i=0; i<5000; i++)
		strTmp[i] = '\0';
	fgets( strTmp, 5000, datei);
	for(int i=0; i<4999; i++)
	{
		if(strTmp[i] == '\n'){
			strTmp[i] = '\0';
			break;
		}
	}
	if(strlen(strTmp) > 0)
		winImages->setCurrentFolder(strTmp);
	else
		winImages->setCurrentFolder("/");
	
	delete strTmp;


	fclose(datei);
}



#ifdef PSP
PSP_HEAP_SIZE_KB(20*1024);
//debug, zeig den freien ram an
void CStuff::showramusage()
{
	struct mallinfo mi;
	char tempstring[100];
	mi = mallinfo();
	sprintf(tempstring, "Ram %dbytes", (20*1024*1024) - mi.arena + mi.fordblks);
	std::cout << tempstring << std::endl;
}
#endif


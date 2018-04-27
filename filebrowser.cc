#include "filebrowser.h"


CFilebrowser::CFilebrowser()
{

}


CFilebrowser::~CFilebrowser()
{
	ClearlistFiles();
}

void CFilebrowser::ClearlistFiles(){

	if(listFiles.size() < 1)
		return;

	for( itFiles = listFiles.begin(); itFiles != listFiles.end(); itFiles++ )
 	{
 		CFileObject *tmpFileObject = *itFiles;
 		//tmpFileObject->Debug();
 		delete tmpFileObject;
 		//listFiles.erase( itFiles );
 	}

	listFiles.clear();
}

/*
bool CFilebrowser::ReadRarFile(std::string filename)
{
	std::cout << "rar, start" << std::endl;
	char *fn = make_string(filename.c_str());

	std::cout << "rar, fn" << fn << std::endl;
	void *rar_file = rar_open(fn,RAR_OM_LIST);
	if(!rar_file){
		std::cout << "rar, file open problem" << std::endl;
		if(fn)
			delete fn;
		return false;
	}
	struct RARHeaderData *hd = (RARHeaderData*)malloc(sizeof(struct RARHeaderData));
	if(!hd) {
		std::cout << "rar, out of memory" << std::endl;
		if(fn)
			delete fn;
		return false;
	}
	memset(hd,0,sizeof(struct RARHeaderData));
	int status = 0;
	std::cout << "rar, size" << sizeof(struct RARHeaderData) << std::endl;


	ClearlistFiles();

	CFileObject *tmpFileObject = new CFileObject();
	tmpFileObject->setFilename("..");
	tmpFileObject->setFiletype(4);
	tmpFileObject->setPos(1);
	tmpFileObject->setArchive(filename);
	tmpFileObject->setRar(true);
	listFiles.push_back( tmpFileObject );

	short i = 2;

	while(!(status = rar_get_header(rar_file,hd))) 
	{

		CFileObject *tmpFileObject = new CFileObject();
		tmpFileObject->setFilename(hd->FileName);
		tmpFileObject->setPath("");
		tmpFileObject->setArchive(filename);
		tmpFileObject->setRar(true);
		tmpFileObject->setFiletype(-1);
		tmpFileObject->setPos(i);
	//	tmpFileObject->setArchive(filename);
	//	tmpFileObject->setRar(true);
		listFiles.push_back( tmpFileObject );
		i++;

		//char *block = hd->FileName;
// 		std::cout << "Filename: " << hd->FileName << std::endl;
// 		std::cout << "Flags: " << hd->Flags << std::endl;
// 		std::cout << "PackSize: " << hd->PackSize << std::endl;
// 		std::cout << "UnpSize: " << hd->UnpSize << std::endl;
// 		std::cout << "UnpVer: " << hd->UnpVer << std::endl;
// 		std::cout << "Method: " << hd->Method << std::endl;
// 		std::cout << "FileAttr: " << hd->FileAttr << std::endl;
		if( ((hd->Flags & 0xE0)^0xE0) == 0)
			tmpFileObject->setFiletype(1);
// 		std::cout << std::endl;

		rar_next_file(rar_file);
	}
	rar_close(rar_file);
	if(hd)
		delete hd;
	if(fn)
		delete fn;

	SortFilenames();
	
	for( itFiles = listFiles.begin(); itFiles != listFiles.end(); itFiles++ )
 	{
		CFileObject *tmpFileObject = *itFiles;
		if(tmpFileObject->getFiletype() != 1 && tmpFileObject->getFiletype() != 4)
		{
			tmpFileObject->setFiletype(this->CheckSuffix(tmpFileObject->getCompleteFilename().c_str()));
		}

	}

	return true;

}
*/


bool CFilebrowser::ChangeDir(std::string path)
{
	/////////////////////////////
	//    change the folder    //
	/////////////////////////////
	
// 	CreateFilename(verzx);
// 	strcpy(averz, adatei);
	
	std::string b("/..");

	std::cout << "changedir start, path: " << path  << " size: " << path.size() << std::endl;
	int i = -1;
	i = path.find(b);
	std::cout << "changedir , i: " << i << std::endl;

	if(i > 0)
	{
		//i = path.find_last_of("/");
		path.erase(i);
		std::cout << "changedir erase, path: " << path << std::endl;
		
		i = path.find_last_of("/");

		std::cout << "changedir , i2: " << i << std::endl;
		if(i == 0)
			path = "/";
		else{
			path.erase(i);
		}
	} 

	if(path == "/..")
		path = "/";
	
	/*
	char averz[path.size()];
	strcpy(averz, path.c_str());
	//cout << "changedir averz = " << averz << endl;

	//wenn xaverz auf eine .-endung zeigt    
	if((averz[strlen(averz) - 1]) == '.')
	{   
		//beim hinteren / trennen    
		char *ptr;
		ptr = strrchr(averz, '/');
		*ptr = '\0';
		
		//das erste / kurzzeitig aendern
		averz[0] = '.';
		
		//wenn noch ein / vorkommt wir am hinteren getrennt
		if((strchr(averz , '/')) > 0) {
			ptr = strrchr(averz, '/');
			*ptr = '\0';
			//das ver�derte / wiederherstellen 
			averz[0] = '/';
		} else {
			//Man will ins rootverzeichnis
			strcpy(averz,"/");
		}
	}

	return ReadFolder(averz);
*/
	std::cout << "changedir end, path: " << path << std::endl;
return ReadFolder(path);
// 	verzx = 0;
// 	listtop = 0;
// 	changed = 1;
}


bool CFilebrowser::ReadFolder(std::string pathToFolder)
{
	DIR *dir;
	struct dirent *dirzeiger;
	std::cout << "ReadFolder();" << std::endl;
	
	if( ( dir=opendir(pathToFolder.c_str()) ) == NULL) {
		std::cout << "pathToFolder = " << pathToFolder << std::endl;
		std::cout << "readfolder() fehler bei opendir()" << std::endl;
		return false;
	}

	ClearlistFiles();

	std::cout << "ClearlistFiles();" << std::endl;
	int i = 1;
	//das ge?fnete verzeichnis auslesen
	while(((dirzeiger=readdir(dir)) != NULL)) 
	{

		if( (strcmp((*dirzeiger).d_name, ".\0" ) != 0) ) {

			CFileObject *tmpFileObject = new CFileObject();
			tmpFileObject->setFilename((*dirzeiger).d_name);
			tmpFileObject->setPath(pathToFolder);
			tmpFileObject->setFiletype(-1);
			tmpFileObject->setPos(i);
			listFiles.push_back( tmpFileObject );
			i++;
		}
	} 
	
	std::cout << "read"<< std::endl;

	SortFilenames();


	//close the dir   
	if(closedir(dir) == -1)
		std::cout<<"verzlesen: Fehler bei closedir" << std::endl;
	
	ReadFiletypes();

	std::cout << "ReadFiletypes();" << std::endl;
//debug ausgabe
 /*	for( itFiles = listFiles.begin(); itFiles != listFiles.end(); itFiles++ )
 	{
 		//cout << *listIter << endl;
 		CFileObject *tmpFileObject = *itFiles;
 		tmpFileObject->Debug();
 		//delete tmpFileObject;
 		//listFilename.erase( listIter );
 	}
*/
	return true;
}



bool CFilebrowser::ReadFiletypes(){
	struct stat narf;

	if(listFiles.size() < 1)
		return false;

	for( itFiles = listFiles.begin(); itFiles != listFiles.end(); itFiles++ )
 	{
		CFileObject *tmpFileObject = *itFiles;

		stat(tmpFileObject->getCompleteFilename().c_str(), &narf);
		if(S_ISDIR(narf.st_mode) != 0)
		{
			tmpFileObject->setFiletype(1); //dir
		}
		else
		{
			tmpFileObject->setFiletype(this->CheckSuffix(tmpFileObject->getCompleteFilename().c_str()));
		}

	}
	return true;
}


//1=dir, 2=image, 3=mp3, 0=unsupported
short CFilebrowser::CheckSuffix(std::string filename)
{
	short i = 0;
	char *ext = strrchr(filename.c_str(), '.');
	if(!ext)
		return 0;

	char whitelist[16][7] = { { ".jpg\0"},
						{ ".JPG\0"},
						{ ".jpeg\0"},
						{ ".JPEG\0"},
						{ ".bmp\0"},
						{ ".BMP\0"},
						{ ".png\0"}, 
						{ ".PNG\0"},
						{ ".gif\0"},
						{ ".GIF\0"},
						{ ".iff\0"},
						{ ".IFF\0"},
						{ ".pcx\0"},
						{ ".PCX\0"},
						{ ".tga\0"},
						{ ".TGA\0"}};

	short whx = 16;
	while(i < whx)
	{
		if(strcmp(ext, whitelist[i]) == 0)
			return 2;
		i++;
	}

	if(strcmp(ext, ".mp3\0") == 0)
			return 3;
	if(strcmp(ext, ".MP3\0") == 0)
			return 3;
	
	if(strcmp(ext, ".rar\0") == 0)
			return 5;
	if(strcmp(ext, ".RAR\0") == 0)
			return 5;

	//printf("Unsupported Fileformat\n");
	return 0;
}



void CFilebrowser::SortFilenames(){
	listFiles.sort(SortCFileObjects());

	int i = 1; 
	for( itFiles = listFiles.begin(); itFiles != listFiles.end(); itFiles++ )
 	{
 		CFileObject *tmpFileObject = *itFiles;
 		tmpFileObject->setPos( i );
		i++;
 	}
}




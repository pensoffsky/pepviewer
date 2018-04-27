#pragma once
#include "includes.h"


class CFileObject{
	protected:
		std::string filename;
		std::string path;

		short originalPosition;
		short filetype;


	public:
		CFileObject();
		~CFileObject();

		void setFilename(std::string value){ filename = value; }
		void setPath(std::string value){ path = value; }
		void setFiletype(short value){ filetype = value;}
		void setPos(short value){originalPosition = value;}
		short getPos(){return originalPosition;}
		short getFiletype(){ return filetype;}
		
		std::string getPath(){return path;}
		std::string getCompleteFilename();
		std::string getFilename();

	

 		SDL_Surface *image;
	// 	SDL_Surface *thumbnail;
		short image_loaded;
	// 	short thumbnail_loaded;

		void Debug(){std::cout << filename << " - " << path << " - " << filetype << " pos:" << originalPosition << std::endl;}
};


struct SortCFileObjects : public std::binary_function<CFileObject*, CFileObject*, bool>
{
	bool operator()(CFileObject* left, CFileObject* right) const
	{     //return true if left is logically less then right for given comparison

		std::string leftFilename = left->getFilename();
		std::string rightFilename = right->getFilename();

		if(leftFilename < rightFilename)
			return true;
		else
			return false;
		//if(left->data < right->data)
// 			return true;
// 		else
// 			return false;
	};
};


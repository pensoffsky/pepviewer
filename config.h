#pragma once
#include "includes.h"

#define COUNT 16

enum  enConfigs
{
EN_CONFIGS_CPUSPEED=0
, EN_CONFIGS_PRECACHING
, EN_CONFIGS_FITTO
, EN_CONFIGS_FITTOOFFSET
, EN_CONFIGS_ROTATION
, EN_CONFIGS_REVERSELR
, EN_CONFIGS_PADSPEED
, EN_CONFIGS_STICKSPEED
, EN_CONFIGS_ZOOM_PADSPEED
, EN_CONFIGS_ZOOM_STICKSPEED
, EN_CONFIGS_TOPCENTERBOTTOM
, EN_CONFIGS_LEFTCENTERRIGHT
, EN_CONFIGS_XOFFSET
, EN_CONFIGS_YOFFSET
// , EN_CONFIGS_SOUNDREMOTE
, EN_CONFIGS_SWAPXO
, EN_CONFIGS_ZOOMSTEP
};

class CConfig
{
	private:
//		short count;


		//!the actual setting
		short settings[COUNT];
		//!how many choices are there for this category
		short settingscount[COUNT];
		//!name of the option category
		char settingslist[COUNT][25];
		//!name of the choices inside each category
		char settingsvalues[COUNT][11][15];
		
	public:
		CConfig();
		~CConfig();
		void Init();

		std::string GetSettingsNames(short value){return settingslist[value];}
		std::string GetSettingsChoices(short name, short choice){return settingsvalues[name][choice];}
		inline short GetSettingsCount(short value){return settingscount[value];}
		inline short GetCount(){return COUNT;}

		short GetPadspeed(short direction);
		inline short GetStickspeed() {return settings[EN_CONFIGS_STICKSPEED];}
		short GetStickScrollSpeed(short strength);

		short GetZoomPadspeed(short direction);
		short GetZoomStickspeed(short strength);

		inline short GetFitto() {return settings[EN_CONFIGS_FITTO];}
		short GetFittoOffset();
		inline short GetRotation() {return settings[EN_CONFIGS_ROTATION];}
		inline short GetTopcenterbottom() {return settings[EN_CONFIGS_TOPCENTERBOTTOM];}
		inline short GetLeftcenterright() {return settings[EN_CONFIGS_LEFTCENTERRIGHT];}
		inline short GetXoffset() {return settings[EN_CONFIGS_XOFFSET];}
		inline short GetYoffset() {return settings[EN_CONFIGS_YOFFSET];}
		inline short GetReverselr() {return settings[EN_CONFIGS_REVERSELR];}
// 		inline short GetSoundremote() {return settings[EN_CONFIGS_SOUNDREMOTE];}
		inline short GetCpuspeed() {return settings[EN_CONFIGS_CPUSPEED];}
		inline short GetPrecaching() {return settings[EN_CONFIGS_PRECACHING];}
		inline short GetSwapXO() {return settings[EN_CONFIGS_SWAPXO];}
		

		short GetZoomStep();
		

		//Interface for Save()
		short GetSettings(short x) {return settings[x];}
		void  SetSettings(short x, short value) {settings[x] = value;}
};


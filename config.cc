#include "config.h"


/** Default constructor **/
CConfig::CConfig()
{
	//count = 14;

}

/** destructor **/
CConfig::~CConfig()
{

}


//load all the graphics for the menu
void CConfig::Init()
{

	std::cout << "CConfig::Init()" << std::endl;

	/////////////////////////////
	//   Reset the Settings    //
	/////////////////////////////
	short i = 0;
	for(i=0; i<COUNT; i++)
	{
		settings[i] = 0;
	}


	strcpy(settingslist[EN_CONFIGS_PADSPEED], "Image Padspeed:       ");
	strcpy(settingslist[EN_CONFIGS_STICKSPEED], "Image Analogspeed:    ");
	strcpy(settingslist[EN_CONFIGS_ZOOM_PADSPEED], "Zoom Padspeed:        ");
	strcpy(settingslist[EN_CONFIGS_ZOOM_STICKSPEED], "Zoom Analogspeed:     ");
	strcpy(settingslist[EN_CONFIGS_FITTO], "Fit to:               ");
	strcpy(settingslist[EN_CONFIGS_FITTOOFFSET],"Fit to offset:        ");
	strcpy(settingslist[EN_CONFIGS_ROTATION], "Rotation:             ");
	strcpy(settingslist[EN_CONFIGS_TOPCENTERBOTTOM], "Top/Center/Bottom:    ");
	strcpy(settingslist[EN_CONFIGS_LEFTCENTERRIGHT], "Left/Center/Right:    ");
	strcpy(settingslist[EN_CONFIGS_XOFFSET], "X-Offset:             ");
	strcpy(settingslist[EN_CONFIGS_YOFFSET], "Y-Offset:             ");
	strcpy(settingslist[EN_CONFIGS_REVERSELR], "Reverse L & R:        ");
// 	strcpy(settingslist[EN_CONFIGS_SOUNDREMOTE], "Sound Remote:         ");
	strcpy(settingslist[EN_CONFIGS_CPUSPEED],"CPU-Speed:            ");
	strcpy(settingslist[EN_CONFIGS_PRECACHING],"Precaching:           ");
	strcpy(settingslist[EN_CONFIGS_SWAPXO],"Swap X & O:           ");
	strcpy(settingslist[EN_CONFIGS_ZOOMSTEP],"Zoom Step:            ");

	strcpy(settingsvalues[EN_CONFIGS_ZOOMSTEP][0], "20");
	strcpy(settingsvalues[EN_CONFIGS_ZOOMSTEP][1], "50");
	strcpy(settingsvalues[EN_CONFIGS_ZOOMSTEP][2], "80");
	strcpy(settingsvalues[EN_CONFIGS_ZOOMSTEP][3], "110");
	strcpy(settingsvalues[EN_CONFIGS_ZOOMSTEP][4], "140");
	settingscount[EN_CONFIGS_ZOOMSTEP] = 5;

	strcpy(settingsvalues[EN_CONFIGS_SWAPXO][0], "False");
	strcpy(settingsvalues[EN_CONFIGS_SWAPXO][1], "True");
	settingscount[EN_CONFIGS_SWAPXO] = 2;

	strcpy(settingsvalues[EN_CONFIGS_ZOOM_PADSPEED][0], "Slow");
	strcpy(settingsvalues[EN_CONFIGS_ZOOM_PADSPEED][1], "Middle");
	strcpy(settingsvalues[EN_CONFIGS_ZOOM_PADSPEED][2], "Fast");
	settingscount[EN_CONFIGS_ZOOM_PADSPEED] = 3;

	strcpy(settingsvalues[EN_CONFIGS_ZOOM_STICKSPEED][0], "Slow");
	strcpy(settingsvalues[EN_CONFIGS_ZOOM_STICKSPEED][1], "Middle");
	strcpy(settingsvalues[EN_CONFIGS_ZOOM_STICKSPEED][2], "Fast");
	strcpy(settingsvalues[EN_CONFIGS_ZOOM_STICKSPEED][3], "Faster");
	settingscount[EN_CONFIGS_ZOOM_STICKSPEED] = 4;
	

	strcpy(settingsvalues[EN_CONFIGS_PADSPEED][0], "Slow");
	strcpy(settingsvalues[EN_CONFIGS_PADSPEED][1], "Middle");
	strcpy(settingsvalues[EN_CONFIGS_PADSPEED][2], "Fast");
	strcpy(settingsvalues[EN_CONFIGS_PADSPEED][3], "Faster");
	settingscount[EN_CONFIGS_PADSPEED] =  4;
	
	strcpy(settingsvalues[EN_CONFIGS_STICKSPEED][0], "Slow");
	strcpy(settingsvalues[EN_CONFIGS_STICKSPEED][1], "Middle");
	strcpy(settingsvalues[EN_CONFIGS_STICKSPEED][2], "Fast");
	strcpy(settingsvalues[EN_CONFIGS_STICKSPEED][3], "Faster");
	settingscount[EN_CONFIGS_STICKSPEED] =  4;

	strcpy(settingsvalues[EN_CONFIGS_FITTO][0], "None");
	strcpy(settingsvalues[EN_CONFIGS_FITTO][1], "Width");
	strcpy(settingsvalues[EN_CONFIGS_FITTO][2], "Height");
	strcpy(settingsvalues[EN_CONFIGS_FITTO][3], "Double Page");
	strcpy(settingsvalues[EN_CONFIGS_FITTO][4], "Width+Off.");
	strcpy(settingsvalues[EN_CONFIGS_FITTO][5], "Height+Off.");
	settingscount[EN_CONFIGS_FITTO] =  6;
	
	strcpy(settingsvalues[EN_CONFIGS_FITTOOFFSET][0], "-120");
	strcpy(settingsvalues[EN_CONFIGS_FITTOOFFSET][1], "-90");
	strcpy(settingsvalues[EN_CONFIGS_FITTOOFFSET][2], "-60");
	strcpy(settingsvalues[EN_CONFIGS_FITTOOFFSET][3], "-30");
	strcpy(settingsvalues[EN_CONFIGS_FITTOOFFSET][4], "30");
	strcpy(settingsvalues[EN_CONFIGS_FITTOOFFSET][5], "60");
	strcpy(settingsvalues[EN_CONFIGS_FITTOOFFSET][6], "90");
	strcpy(settingsvalues[EN_CONFIGS_FITTOOFFSET][7], "120");
	strcpy(settingsvalues[EN_CONFIGS_FITTOOFFSET][8], "150");
	settingscount[EN_CONFIGS_FITTOOFFSET] =  9;

	strcpy(settingsvalues[EN_CONFIGS_ROTATION][0], "0");
	strcpy(settingsvalues[EN_CONFIGS_ROTATION][1], "90");
	strcpy(settingsvalues[EN_CONFIGS_ROTATION][2], "180");
	strcpy(settingsvalues[EN_CONFIGS_ROTATION][3], "270");
	settingscount[EN_CONFIGS_ROTATION] =  4;

	strcpy(settingsvalues[EN_CONFIGS_TOPCENTERBOTTOM][0], "Top");
	strcpy(settingsvalues[EN_CONFIGS_TOPCENTERBOTTOM][1], "Center");
	strcpy(settingsvalues[EN_CONFIGS_TOPCENTERBOTTOM][2], "Bottom");
	settingscount[EN_CONFIGS_TOPCENTERBOTTOM] =  3;

	strcpy(settingsvalues[EN_CONFIGS_LEFTCENTERRIGHT][0], "Left");
	strcpy(settingsvalues[EN_CONFIGS_LEFTCENTERRIGHT][1], "Center");
	strcpy(settingsvalues[EN_CONFIGS_LEFTCENTERRIGHT][2], "Right");
	settingscount[EN_CONFIGS_LEFTCENTERRIGHT] =  3;

	strcpy(settingsvalues[EN_CONFIGS_XOFFSET][0], "0");
	strcpy(settingsvalues[EN_CONFIGS_XOFFSET][1], "20");
	strcpy(settingsvalues[EN_CONFIGS_XOFFSET][2], "40");
	strcpy(settingsvalues[EN_CONFIGS_XOFFSET][3], "-20");
	strcpy(settingsvalues[EN_CONFIGS_XOFFSET][4], "-40");
	settingscount[EN_CONFIGS_XOFFSET] =  5;

	strcpy(settingsvalues[EN_CONFIGS_YOFFSET][0], "0");
	strcpy(settingsvalues[EN_CONFIGS_YOFFSET][1], "20");
	strcpy(settingsvalues[EN_CONFIGS_YOFFSET][2], "40");
	strcpy(settingsvalues[EN_CONFIGS_YOFFSET][3], "-20");
	strcpy(settingsvalues[EN_CONFIGS_YOFFSET][4], "-40");
	settingscount[EN_CONFIGS_YOFFSET] =  5;

	strcpy(settingsvalues[EN_CONFIGS_REVERSELR][0], "Normal");
	strcpy(settingsvalues[EN_CONFIGS_REVERSELR][1], "Reversed");
	settingscount[EN_CONFIGS_REVERSELR] =  2;

// 	strcpy(settingsvalues[EN_CONFIGS_SOUNDREMOTE][0], "Scrolling");
// 	strcpy(settingsvalues[EN_CONFIGS_SOUNDREMOTE][1], "Audio");
// 	settingscount[EN_CONFIGS_SOUNDREMOTE] =  2;

	strcpy(settingsvalues[EN_CONFIGS_CPUSPEED][0], "222");
	strcpy(settingsvalues[EN_CONFIGS_CPUSPEED][1], "333");
	settingscount[EN_CONFIGS_CPUSPEED] = 2;

	strcpy(settingsvalues[EN_CONFIGS_PRECACHING][0], "Off");
	strcpy(settingsvalues[EN_CONFIGS_PRECACHING][1], "1");
	strcpy(settingsvalues[EN_CONFIGS_PRECACHING][2], "2");
	strcpy(settingsvalues[EN_CONFIGS_PRECACHING][3], "3");
	strcpy(settingsvalues[EN_CONFIGS_PRECACHING][4], "4");
	strcpy(settingsvalues[EN_CONFIGS_PRECACHING][5], "5");
	strcpy(settingsvalues[EN_CONFIGS_PRECACHING][6], "6");
	settingscount[EN_CONFIGS_PRECACHING] = 7;
}


short CConfig::GetPadspeed(short direction)
{
	if(direction == 0)
		return 0;

	short speed = 4;
	
	if(settings[EN_CONFIGS_PADSPEED] == 0)
		speed = 3;
	else if(settings[EN_CONFIGS_PADSPEED] == 1)
		speed = 7;
	else if(settings[EN_CONFIGS_PADSPEED] == 2)
		speed = 11;
	else if(settings[EN_CONFIGS_PADSPEED] == 3)
		speed = 15;
	
	if(direction < 0)
	 speed = speed * (-1);
		
	return speed;
}

short CConfig::GetZoomPadspeed(short direction)
{
	if(direction == 0)
		return 0;

	short speed = 4;
	
	if(settings[EN_CONFIGS_ZOOM_PADSPEED] == 0)
		speed = 4;
	else if(settings[EN_CONFIGS_ZOOM_PADSPEED] == 1)
		speed = 8;
	else if(settings[EN_CONFIGS_ZOOM_PADSPEED] == 2)
		speed = 12;
	
	if(direction < 0)
	 speed = speed * (-1);
		
	return speed;
}


short CConfig::GetStickScrollSpeed(short strength)
{
	short speed = 0;

	short tmpStrength = strength;

	if(strength < 0)
		tmpStrength *= -1;
	
	if(tmpStrength > 32)
		speed = 3;
	else if(tmpStrength > 29)
		speed = 3;
	else if(tmpStrength > 26)
		speed = 2;
	else if(tmpStrength > 23)
		speed = 2;
	else if(tmpStrength > 20)
		speed = 2;
	else if(tmpStrength > 17)
		speed = 1;
	else if(tmpStrength > 14)
		speed = 1;
	else if(tmpStrength > 11)
		speed = 1;
	else if(tmpStrength > 6)
		speed = 1;

	if(settings[EN_CONFIGS_STICKSPEED] == 0)
		speed = (short)(speed * 1);
	else if(settings[EN_CONFIGS_STICKSPEED] == 1)
		speed = (short)(speed * 1.6);
	else if(settings[EN_CONFIGS_STICKSPEED] == 2)
		speed = (short)(speed * 2.4);
	else if(settings[EN_CONFIGS_STICKSPEED] == 3)
		speed = (short)(speed * 3.2);

	
	if(strength < 0)
	 speed = speed * (-1);
		
	return speed;
}


short CConfig::GetZoomStickspeed(short strength)
{
	short speed = 0;

	short tmpStrength = strength;

	if(strength < 0)
		tmpStrength *= -1;
	
	if(tmpStrength > 32)
		speed = 3;
	else if(tmpStrength > 29)
		speed = 3;
	else if(tmpStrength > 26)
		speed = 2;
	else if(tmpStrength > 23)
		speed = 2;
	else if(tmpStrength > 20)
		speed = 2;
	else if(tmpStrength > 17)
		speed = 1;
	else if(tmpStrength > 14)
		speed = 1;
	else if(tmpStrength > 11)
		speed = 1;
	else if(tmpStrength > 6)
		speed = 1;

	if(settings[EN_CONFIGS_ZOOM_STICKSPEED] == 0)
		speed = (short)(speed * 1);
	else if(settings[EN_CONFIGS_ZOOM_STICKSPEED] == 1)
		speed = (short)(speed * 2);
	else if(settings[EN_CONFIGS_ZOOM_STICKSPEED] == 2)
		speed = (short)(speed * 3);
	else if(settings[EN_CONFIGS_ZOOM_STICKSPEED] == 3)
			speed = (short)(speed * 4);
	if(strength < 0)
	 speed = speed * (-1);
		
	return speed;
}


short CConfig::GetZoomStep()
{
	short speed = 0;
	
	if(settings[EN_CONFIGS_ZOOMSTEP] == 0)
		speed = 20;
	else if(settings[EN_CONFIGS_ZOOMSTEP] == 1)
		speed = 50;
	else if(settings[EN_CONFIGS_ZOOMSTEP] == 2)
		speed = 80;
	else if(settings[EN_CONFIGS_ZOOMSTEP] == 3)
		speed = 110;
	else if(settings[EN_CONFIGS_ZOOMSTEP] == 4)
		speed = 140;
		
	return speed;
}

short CConfig::GetFittoOffset()
{
	short speed = 0;
	
	if(settings[EN_CONFIGS_FITTOOFFSET] == 0)
		speed = -120;
	else if(settings[EN_CONFIGS_FITTOOFFSET] == 2)
		speed = -90;
	else if(settings[EN_CONFIGS_FITTOOFFSET] == 2)
		speed = -60;
	else if(settings[EN_CONFIGS_FITTOOFFSET] == 3)
		speed = -30;
	else if(settings[EN_CONFIGS_FITTOOFFSET] == 4)
		speed = 30;
	else if(settings[EN_CONFIGS_FITTOOFFSET] == 5)
		speed = 60;
	else if(settings[EN_CONFIGS_FITTOOFFSET] == 6)
		speed = 90;
	else if(settings[EN_CONFIGS_FITTOOFFSET] == 7)
		speed = 120;
	else if(settings[EN_CONFIGS_FITTOOFFSET] == 8)
		speed = 150;
		
	return speed;
}


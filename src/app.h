#ifndef APP_H
#define APP_H

#include "olcPixelGameEngine.h"

class App : public olc::PixelGameEngine
{
public:
	App()
	{
		sAppName = "Parkour Run";
	}

public:
	
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
	bool OnUserDestroy() override;
};

#endif
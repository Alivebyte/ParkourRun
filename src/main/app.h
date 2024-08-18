#ifndef APP_H
#define APP_H

#include "olcPixelGameEngine.h"

class App : public olc::PixelGameEngine
{
public:
	App(): playerSprite(nullptr), backgroundSprite(nullptr), pack(nullptr)
	{
		sAppName = "Parkour Run";
	}

public:
	
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
	bool OnUserDestroy() override;

	olc::Sprite* playerSprite;
	olc::Sprite* backgroundSprite;
	olc::vf2d playerPos;
	olc::ResourcePack* pack;
};

#endif
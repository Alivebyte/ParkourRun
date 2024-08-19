#ifndef APP_H
#define APP_H


#include "olcPixelGameEngine.h"
#include "extensions/olcPGEX_Graphics2D.h"
#include "extensions/olcPGEX_SplashScreen.h"
#include "extensions/olcPGEX_TransformedView.h"
#include "utilities/olcUTIL_Camera2D.h"
#include "olcPGEX_ResourceManager.h"
#include "olcPGEX_Animator2D.h"

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

	olc::Sprite* pipeTurnSprite;
	olc::Sprite* pipeVerticalSprite;
	olc::Sprite* pipeHorizontalSprite;
	olc::SplashScreen* splash;
	olc::utils::Camera2D camera;
	olc::TileTransformedView tv;
};

#endif
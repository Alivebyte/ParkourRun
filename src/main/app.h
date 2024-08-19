#ifndef APP_H
#define APP_H


#include "olcPixelGameEngine.h"
#include "extensions/olcPGEX_Graphics2D.h"
#include "extensions/olcPGEX_SplashScreen.h"
#include "extensions/olcPGEX_TransformedView.h"
#include "utilities/olcUTIL_Camera2D.h"
#include "resource_manager.h"
#include "olcPGEX_Animator2D.h"

class App : public olc::PixelGameEngine
{
public:
	App(): pack(nullptr)
	{
		sAppName = "Parkour Run";
	}

public:
	
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
	bool OnUserDestroy() override;

	//olc::Sprite* playerSprite;
	olc::Decal* playerDecal;
	//olc::Sprite* backgroundSprite;
	olc::Decal* backgroundDecal;
	olc::vf2d playerPos;
	olc::ResourcePack* pack;

	GameResourceManager rm;
	
	olc::SplashScreen* splash;
	olc::utils::Camera2D camera;
	olc::TileTransformedView tv;
	olc::vi2d vTileSize;
};

#endif
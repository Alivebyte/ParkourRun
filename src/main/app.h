#ifndef APP_H
#define APP_H


#include "olcPixelGameEngine.h"
#include "extensions/olcPGEX_Graphics2D.h"
#include "extensions/olcPGEX_SplashScreen.h"
#include "extensions/olcPGEX_TransformedView.h"
#include "utilities/olcUTIL_Camera2D.h"
#include "olcPGEX_Raycast2D.h"
#include "resource_manager.h"
#include "animator.h"
#include "tiles.h"

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


	void Render(float fElapsedTime);
	void HandleCollision(float fElapasedTime);
	void CameraMovement(float fElapsedTime);

	Player* m_pPlayer;
	olc::Decal* backgroundDecal;
	olc::vi2d levelSize;
	olc::ResourcePack* pack;

	GameResourceManager rm;
	
	olc::SplashScreen* splash;
	olc::utils::Camera2D camera;
	olc::TileTransformedView tv;
	olc::vi2d vTileSize;
	olcPGEX_RayCast2D rayCaster2D;

	Tile *tiles;
};

#endif
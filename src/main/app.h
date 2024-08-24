#ifndef APP_H
#define APP_H

#pragma once

#include "olcPixelGameEngine.h"
#include "extensions/olcPGEX_Graphics2D.h"
#include "extensions/olcPGEX_SplashScreen.h"
#include "extensions/olcPGEX_TransformedView.h"
#include "utilities/olcUTIL_Camera2D.h"
#include "olcPGEX_Raycast2D.h"
#include "resource_manager.h"
#include "animator.h"
#include "tiles.h"
#include "level_manager.h"
#include "level.h"



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


	void Render(float fElapsedTime);
	void HandleCollision(float fElapasedTime);
	void CameraMovement(float fElapsedTime);
	void SetEditMode(bool bEMode);

private:
	Player* m_pPlayer;
	olc::Decal* backgroundDecal;
	olc::vi2d levelSize;
	//olc::ResourcePack* pack;


	
	olc::SplashScreen* splash;
	olc::utils::Camera2D camera;
	olc::TileTransformedView tv;
	olc::vi2d vTileSize;
	olcPGEX_RayCast2D rayCaster2D;

	LevelManager lm;

	//Tile *tiles;

private:
	bool m_bEditMode;
};



#endif
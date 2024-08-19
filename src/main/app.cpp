#include "app.h"

bool App::OnUserCreate()
{
	//splash = new olc::SplashScreen();

	pack = new olc::ResourcePack();

	vTileSize = { 32,32 }; 
	pack->LoadPack("content.pack", "TEST_KEY");

	tv = olc::TileTransformedView(GetScreenSize(), vTileSize);
	//playerSprite = new olc::Sprite("content/sprites/player.png", pack);
	//playerDecal = new olc::Decal(playerSprite);
	//backgroundSprite = new olc::Sprite("content/sprites/background_test.png",pack);
	//backgroundDecal = new olc::Decal(backgroundSprite);
	
	backgroundDecal = rm.RM_Sprite("content/sprites/background_test.png", pack);//pipeHorizontalSprite = new olc::Sprite("content/sprites/world_objects/industrial_pipe_horizontal.png",pack);
	playerDecal = rm.RM_Sprite("content/sprites/player.png", pack);
	//pipeVerticalSprite = new olc::Sprite("content/sprites/world_objects/industrial_pipe_vertical.png",pack);

	//pipeTurnSprite = new olc::Sprite("content/sprites/world_objects/industrial_pipe_turn90.png",pack);
	auto backgroundSize = backgroundDecal->sprite->Size() / vTileSize;
	// Generate world
	tiles = new Tile[backgroundSize.y*backgroundSize.x];
	for (int y = 0; y < backgroundSize.y; y++)
	{
		for (int x = 0; x < backgroundSize.x; x++)
		{
			if (x == 0 || y == 0 || x == 23)
			{
				//tiles->at(y*24+x) = Tile("content/sprites/world_objects/industrial_pipe_vertical.png", &rm, pack);
				//tiles->at(y* 24 + x).SetType(10);
				tiles[y * backgroundSize.x + x].SetSprite("content/sprites/world_objects/industrial_pipe_vertical.png", &rm, pack);
				tiles[y * backgroundSize.x + x].SetType(10);
			}
			else
			{
				tiles[y*backgroundSize.x+x].SetType(0);
			}
		}
	}

	//playerPos.y = ScreenHeight() - 250.0f;
	//printf("Background size: %d %d\n", .x / vTileSize, backgroundDecal->sprite->Size().y / vTileSize);

	camera = olc::utils::Camera2D(GetScreenSize() / vTileSize, playerPos);

	camera.SetTarget(playerPos);
	camera.SetMode(olc::utils::Camera2D::Mode::Simple);
	camera.SetWorldBoundary({ 0,0 }, backgroundDecal->sprite->Size() / vTileSize);
	camera.EnableWorldBoundary(true);


	return true;
};

bool App::OnUserUpdate(float fElapsedTime)
{
	// called once per frame
	Clear(olc::CYAN);

	// Handle collision & simple physics
	//playerPos.y += 9.8f * fElapsedTime;
	// Handle controls
	if (GetKey(olc::Key::W).bHeld)
	{
		playerPos.y -= 1.0f * 2.0f * fElapsedTime;
	}

	if (GetKey(olc::Key::S).bHeld)
	{
		playerPos.y += 1.0f * 2.0f * fElapsedTime;
	}

	if (GetKey(olc::Key::A).bHeld)
	{
		playerPos.x -= 1.0f * 2.0f * fElapsedTime;
	}

	if (GetKey(olc::Key::D).bHeld)
	{
		playerPos.x += 1.0f * 2.0f * fElapsedTime;
	}

	if (playerPos.x < 0.0f) playerPos.x = 0.0f;
	if (playerPos.y < 0.0f) playerPos.y = 0.0f;
	
	camera.SetMode(olc::utils::Camera2D::Mode::LazyFollow);

	bool bOnScreen = camera.Update(fElapsedTime);



	tv.SetWorldOffset(camera.GetViewPosition());

	// Render background
	SetPixelMode(olc::Pixel::NORMAL);

	tv.DrawDecal({ 0,0 }, backgroundDecal);


	// Render world
	SetPixelMode(olc::Pixel::ALPHA);

	auto backgroundSize = backgroundDecal->sprite->Size() / vTileSize;
	
	for (int y = 0; y < backgroundSize.y; y++)
	{
		for (int x = 0; x < backgroundSize.x; x++)
		{
			switch (tiles[y * backgroundSize.x + x].GetType())
			{
			case 0:
				break;
			case 10:
				tv.DrawDecal(olc::vi2d( x,y ), tiles[y * backgroundSize.x + x].GetSprite());
			}
		}
	}
	//tv.DrawSprite({ 0,0 }, pipeHorizontalSprite, { 1.0f, 1.0f});
	//tv.DrawSprite({ 32,0 }, pipeTurnSprite, {1.0f,1.0f}, olc::Sprite::Flip::HORIZ);
	//tv.DrawSprite({ 32,32 }, pipeVerticalSprite, {1.0f,1.0f}, olc::Sprite::Flip::HORIZ);

	// Render player
	tv.DrawDecal(playerPos, playerDecal, {1,1});

	SetPixelMode(olc::Pixel::NORMAL);

	

	return true;
}

bool App::OnUserDestroy()
{
	return true;
}
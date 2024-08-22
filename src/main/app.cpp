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
	//pipeVerticalSprite = new olc::Sprite("content/sprites/world_objects/industrial_pipe_vertical.png",pack)
	
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
				tiles[y * backgroundSize.x + x].SetType(TILE_COLLIDE);
			}
			else
			{
				tiles[y*backgroundSize.x+x].SetType(TILE_AIR);
			}
		}
	}

	//float fAngle = float(rand()) / float(RAND_MAX) * 2.0f * 3.14159f;
	//fAngle = -0.4f;
	playerVel = { 0.0f, 0.0f };
	playerPos = { 4.0f, 10.0f };
	playerSpeed = 3.0f;
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

	

	auto backgroundSize = backgroundDecal->sprite->Size() / vTileSize;

	auto GetTile = [&](int x, int y)
		{
			if (x >= 0 && x < backgroundSize.x && y >= 0 && y < backgroundSize.y)
				return tiles[y * backgroundSize.x + x];
			else
				return Tile("content/sprites/test_wall.png", &rm, pack);
		};
	// Debug rendering

	for (int y = 0; y < backgroundSize.y; y++)
	{
		for (int x = 0; x < backgroundSize.x; x++)
		{
			switch (tiles[y * backgroundSize.x + x].GetType())
			{
			case 0:
				break;
			case 10:
				tv.FillRect(olc::vi2d( x,y ), {1,1}, olc::GREEN);
			}
		}
	}
	
	playerVel = { 0.0f ,0.0f };

	// Handle user input
	if (GetKey(olc::Key::W).bHeld)
	{
		playerVel = { 0.0f, -2.0f };
	} //else playerDir = { 0.0f, 0.0f };
	if (GetKey(olc::Key::S).bHeld)
	{
		playerVel = { 0.0f, 2.0f };
	} //else playerDir = { 0.0f, 0.0f };
	if (GetKey(olc::Key::A).bHeld)
	{
		playerVel = { -2.0f, 0.0f };

	} //else playerDir = { 0.0f, 0.0f };
	if (GetKey(olc::Key::D).bHeld)
	{
		playerVel = { 2.0f, 0.0f };
	}
	
	// Handle collision & simple physics
	// Gravity physics

	olc::vf2d potentialPlayerPos = playerPos + playerVel * fElapsedTime;
	
	// Check for collision
	if (playerVel.x <= 0) // Moving Left
	{
		if (GetTile(potentialPlayerPos.x + 0.0f, playerPos.y + 0.0f).GetType() != TILE_AIR || GetTile(potentialPlayerPos.x + 0.0f, playerPos.y + 0.9f).GetType() != TILE_AIR)
		{
			potentialPlayerPos.x = (int)potentialPlayerPos.x + 1;
			playerVel.x = 0;
		}
	}
	if (playerVel.x > 0) // Moving Right
	{
		if (GetTile(potentialPlayerPos.x + 1.0f, playerPos.y + 0.0f).GetType() != TILE_AIR || GetTile(potentialPlayerPos.x + 1.0f, playerPos.y + 0.9f).GetType() != TILE_AIR)
		{
			potentialPlayerPos.x = (int)potentialPlayerPos.x;
			playerVel.x = 0;
		}
	}

	bOnGround = false;
	if (playerVel.y <= 0) // Moving Up
	{
		if (GetTile(potentialPlayerPos.x + 0.0f, potentialPlayerPos.y).GetType() !=  TILE_AIR || GetTile(potentialPlayerPos.x + 0.9f, potentialPlayerPos.y).GetType() != TILE_AIR)
		{
			potentialPlayerPos.y = (int)potentialPlayerPos.y + 1;
			playerVel.y = 0;
		}
	}
	else if(playerVel.y > 0) // Moving Down
	{
		if (GetTile(potentialPlayerPos.x + 0.0f, potentialPlayerPos.y + 1.0f).GetType() != TILE_AIR || GetTile(potentialPlayerPos.x + 0.9f, potentialPlayerPos.y + 1.0f).GetType() != TILE_AIR)
		{
			potentialPlayerPos.y = (int)potentialPlayerPos.y;
			playerVel.y = 0;
			bOnGround = true; // Player has a solid surface underfoot
		}
	}

	playerPos = potentialPlayerPos;

	
	
	//if (bHasHitTile) return true;

	// Clamp player position on the left and upward side
	if (playerPos.x < 0.0f) playerPos.x = 0.0f;
	if (playerPos.y < 0.0f) playerPos.y = 0.0f;
	
	//playerPos += playerVel * playerSpeed * fElapsedTime;
	
	// Handle player camera movement
	camera.SetMode(olc::utils::Camera2D::Mode::LazyFollow);
	bool bOnScreen = camera.Update(fElapsedTime);
	tv.SetWorldOffset(camera.GetViewPosition());


	//playerPos += playerDir * playerSpeed * fElapsedTime;
	// Render background

	//tv.DrawDecal({ 0,0 }, backgroundDecal);

	// Render world
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

	//Render player
	tv.DrawDecal({ playerPos.x, playerPos.y}, playerDecal, { 1,1 });
	

	//SetPixelMode(olc::Pixel::NORMAL);

	

	return true;
}

bool App::OnUserDestroy()
{
	return true;
}
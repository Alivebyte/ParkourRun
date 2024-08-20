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
			if (x == 0 || y == 0 || x == 23 || y == 16)
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
	playerDir = { 0.0f, 0.0f };
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

	olc::vf2d vPotentialPlayerPos = playerPos + playerDir * playerSpeed * fElapsedTime;

	olc::vf2d vTilePlayerRadialDims = { 16.0f / vTileSize.x, 16.0f / vTileSize.y };

	auto TestResolveCollisionPoint = [&](const olc::vf2d& point)
	{
		olc::vi2d vTestPoint = vPotentialPlayerPos + vTilePlayerRadialDims * point;
		tv.FillRect(vTestPoint, {1,1}, olc::DARK_BLUE);
		tv.DrawRect({ playerPos.x - 0.25f, playerPos.y - 0.25f}, vTilePlayerRadialDims, olc::DARK_MAGENTA);

		auto& tile = tiles[vTestPoint.y * backgroundSize.x + vTestPoint.x];
		if (tile.GetType() == TILE_AIR)
		{
			// Do Nothing, no collision
			return false;
		}
		else
		{
				// Player has collided with a tile
			bool bTileHit = tile.GetType() == TILE_COLLIDE;
				
			if (bTileHit)
			{
				tv.FillRect(vTestPoint, { 1,1 }, olc::RED);
				////Collision response
			if (point.x == 0.0f)
				playerDir.y *= -1.0f;
			if (point.y == 0.0f)
				playerDir.x *= -1.0f;
			}
				
			
			return bTileHit;
		}
	};

	
	
	// Handle collision & simple physics
	//playerPos.y += 9.8f * fElapsedTime;
	
	

	
	bool bHasHitTile = false;
	bHasHitTile |= TestResolveCollisionPoint(olc::vf2d(0, -1));
	bHasHitTile |= TestResolveCollisionPoint(olc::vf2d(0, +1));
	bHasHitTile |= TestResolveCollisionPoint(olc::vf2d(-1, 0));
	bHasHitTile |= TestResolveCollisionPoint(olc::vf2d(+1, 0));
	
	
	//if (bHasHitTile) return true;

	//if (playerPos.x < 0.0f) playerPos.x = 0.0f;
	//if (playerPos.y < 0.0f) playerPos.y = 0.0f;
	// Handle controls
	if (GetKey(olc::Key::W).bHeld)
	{
		if(!bHasHitTile)
			playerDir = { 0.0f, -1.0f };
	} //else playerDir = { 0.0f, 0.0f };
	if (GetKey(olc::Key::S).bHeld) 
	{
		if (!bHasHitTile)
			playerDir = { 0.0f, 1.0f };
	} //else playerDir = { 0.0f, 0.0f };
	if (GetKey(olc::Key::A).bHeld)
	{
		if (!bHasHitTile)
			playerDir = { -1.0f, 0.0f };

	} //else playerDir = { 0.0f, 0.0f };
	if (GetKey(olc::Key::D).bHeld)
	{ 
		if (!bHasHitTile)
			playerDir = { 1.0f, 0.0f };
		
	}
	
	if (GetKey(olc::Key::W).bReleased || GetKey(olc::Key::A).bReleased || GetKey(olc::Key::S).bReleased || GetKey(olc::Key::D).bReleased)
	{
		playerDir = { 0.0f, 0.0f };
	}
	
	playerPos += playerDir * playerSpeed * fElapsedTime;

	camera.SetMode(olc::utils::Camera2D::Mode::LazyFollow);

	bool bOnScreen = camera.Update(fElapsedTime);



	tv.SetWorldOffset(camera.GetViewPosition());


	//playerPos += playerDir * playerSpeed * fElapsedTime;
	// Render background

	tv.DrawDecal({ 0,0 }, backgroundDecal);


	// Render world
	//SetPixelMode(olc::Pixel::ALPHA);

	
	
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

	// Render "tile map", by getting visible tiles
	/*olc::vi2d vTileTL = tv.GetTopLeftTile().max({ 0,0 });
	olc::vi2d vTileBR = tv.GetBottomRightTile().min(backgroundSize);
	olc::vi2d vTile;*/
	// Then looping through them and drawing them
	/*for (vTile.y = vTileTL.y; vTile.y < vTileBR.y; vTile.y++)
		for (vTile.x = vTileTL.x; vTile.x < vTileBR.x; vTile.x++)
		{
			int idx = vTile.y * backgroundSize.x + vTile.x;

			if (tiles[idx].GetType() == TILE_AIR)
				break;

			if (tiles[idx].GetType() == TILE_COLLIDE)
				tv.DrawDecal(vTile, tiles[idx].GetSprite());
		}*/
	//tv.DrawSprite({ 0,0 }, pipeHorizontalSprite, { 1.0f, 1.0f});
	//tv.DrawSprite({ 32,0 }, pipeTurnSprite, {1.0f,1.0f}, olc::Sprite::Flip::HORIZ);
	//tv.DrawSprite({ 32,32 }, pipeVerticalSprite, {1.0f,1.0f}, olc::Sprite::Flip::HORIZ);

	//Render player
	tv.DrawDecal({ playerPos.x - 0.5f , playerPos.y - 0.5f}, playerDecal, { 1,1 });
	

	//SetPixelMode(olc::Pixel::NORMAL);

	

	return true;
}

bool App::OnUserDestroy()
{
	return true;
}
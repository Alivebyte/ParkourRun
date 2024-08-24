#include "app.h"
#include "level.h"

GameResourceManager g_RM;

GameResourceManager* GetGameResourceManager()
{
	return &g_RM;
}

bool App::OnUserCreate()
{

	//// Testing out level creation
	//srand(rand() % 255);
	//TileInfo tinfo;
	//tinfo.flip = olc::Sprite::Flip::HORIZ;
	//tinfo.m_iColType = TILE_NOCOLLIDE;
	//tinfo.m_spriteName = "content/sprites/world_objects/test_wall.png";
	//tinfo.m_viPos = { rand() % 43, rand() % 22 };

	//auto ld = level.GetLevelData();
	//const int size = 10;
	////ld->m_sLevelName = "level1";
	//ld->playerSpawnPoint = { rand() % 43, rand() % 22 };
	////ld->m_TilesInfo = new TileInfo[size];
	g_RM.InitializeResourcePack();

	g_RM.LoadResourcePack("content.pack", "TEST_KEY");
	//for (int i = 0; i < size; i++)
	//{
	//	ld->m_TilesInfo.push_back(tinfo);
	//}

	//lm.SaveLevelFile("content/levels/level1.map", &level);

	lm.LoadLevelFile("content/levels/level1.map");
	//splash = new olc::SplashScreen();

	//pack = new olc::ResourcePack();

	vTileSize = { 32,32 }; 
	//pack->LoadPack("content.pack", "TEST_KEY");

	tv = olc::TileTransformedView(GetScreenSize(), vTileSize);
	
	//playerSprite = new olc::Sprite("content/sprites/player.png", pack);
	//playerDecal = new olc::Decal(playerSprite);
	//backgroundSprite = new olc::Sprite("content/sprites/background_test.png",pack);
	//backgroundDecal = new olc::Decal(backgroundSprite);
	
	backgroundDecal = g_RM.RM_Sprite("content/sprites/background_test.png");//pipeHorizontalSprite = new olc::Sprite("content/sprites/world_objects/industrial_pipe_horizontal.png",pack);
	//playerDecal = rm.RM_Sprite("content/sprites/player.png",pack);
	//pipeVerticalSprite = new olc::Sprite("content/sprites/world_objects/industrial_pipe_vertical.png",pack)
	

	//animator.ScaleAnimation("Idle", { 2.0f, 2.0f });
	//pipeTurnSprite = new olc::Sprite("content/sprites/world_objects/industrial_pipe_turn90.png",pack);
	levelSize = backgroundDecal->sprite->Size() / vTileSize;

	// Generate world
	tiles = new Tile[levelSize.y*levelSize.x];
	for (int y = 0; y < levelSize.y; y++)
	{
		for (int x = 0; x < levelSize.x; x++)
		{
			if (x == 0 || y == 0 || y == 22 || (x == 23 && y >= 18))
			{
				//tiles->at(y*24+x) = Tile("content/sprites/world_objects/industrial_pipe_vertical.png", &rm, pack);
				//tiles->at(y* 24 + x).SetType(10);
				tiles[y * levelSize.x + x].SetSprite("content/sprites/world_objects/industrial_pipe_vertical.png", &g_RM);
				tiles[y * levelSize.x + x].SetCollisionType(TILE_COLLIDE);
			}
			else
			{
				tiles[y*levelSize.x+x].SetCollisionType(TILE_NOCOLLIDE);
			}
		}
	}


	// Create player
	m_pPlayer = new Player("content/sprites/player.png", &g_RM, olc::vf2d(4.0f, 10.0f));

	// Initialize animations
	m_pPlayer->InitializeAnimations();
	m_pPlayer->SetTouchedWall(false);
	/*playerVel = { 0.0f, 0.0f };
	playerSpawnPoint = { 4.0f, 10.0f };
	playerPos = playerSpawnPoint;
	playerSpeed = 3.0f;*/
	olc::vf2d *playerPos = m_pPlayer->GetPlayerPosition();
	
	//playerPos.y = ScreenHeight() - 250.0f;
	//printf("Background size: %d %d\n", .x / vTileSize, backgroundDecal->sprite->Size().y / vTileSize);

	camera = olc::utils::Camera2D(GetScreenSize() / vTileSize, *playerPos);

	camera.SetTarget(*playerPos);
	camera.SetMode(olc::utils::Camera2D::Mode::Simple);
	camera.SetWorldBoundary({ 0,0 }, backgroundDecal->sprite->Size() / vTileSize);
	camera.EnableWorldBoundary(true);


	return true;
};

bool App::OnUserUpdate(float fElapsedTime)
{
	// called once per frame
	Clear(olc::CYAN);

	// Handle user input
	m_pPlayer->HandleUserInput(this, fElapsedTime);

	// Handle collision
	HandleCollision(fElapsedTime);
	
	// Handle player camera movement
	CameraMovement(fElapsedTime);

	// Do Render
	Render(fElapsedTime);

	return true;
}

void App::Render(float fElapsedTime)
{
	Level *level = lm.GetLevel();
	// Render background
	tv.DrawDecal({ 0,0 }, backgroundDecal);

	// Render world
	for (int y = 0; y < levelSize.y; y++)
	{
		for (int x = 0; x < levelSize.x; x++)
		{
			/*if (level)
			{
				
			}*/
			switch (tiles[y * levelSize.x + x].GetCollisionType())
			{
			case TILE_NOCOLLIDE:
				break;
			case TILE_COLLIDE:
				tv.DrawDecal(olc::vi2d(x, y), tiles[y * levelSize.x + x].GetSprite());
			}

		}
	}

	auto& animator = *m_pPlayer->GetAnimator();

	//Render player
	if (!animator.GetAnim("Idle")->bIsPlaying)
		animator.Play("Idle");

	animator.UpdateAnimations(fElapsedTime);

	animator.DrawAnimationFrame(*m_pPlayer->GetPlayerPosition(), 0.0f, &tv);
}

void App::CameraMovement(float fElapsedTime)
{
	camera.SetMode(olc::utils::Camera2D::Mode::LazyFollow);
	bool bOnScreen = camera.Update(fElapsedTime);
	tv.SetWorldOffset(camera.GetViewPosition());
}

void App::HandleCollision(float fElapsedTime)
{
	auto GetTile = [&](int x, int y)
		{
			if (x >= 0 && x < levelSize.x && y >= 0 && y < levelSize.y)
				return tiles[y * levelSize.x + x];
			else
				return Tile("content/sprites/test_wall.png", &g_RM);
		};


	olc::vf2d playerPos = *m_pPlayer->GetPlayerPosition();
	olc::vf2d playerVel = *m_pPlayer->GetPlayerVelocity();
	olc::vf2d playerSpawnPoint = *m_pPlayer->GetPlayerSpawnPosition();

	// Clamp velocities

	if (playerVel.x > 50.0f)
	{
		playerVel.x = 50.0f;
		//m_pPlayer->SetPlayerVelocity(playerVel);
	}
	if (playerVel.x < -50.0f)
	{
		playerVel.x = -50.0f;
		//m_pPlayer->SetPlayerVelocity(playerVel);
	}
	if (playerVel.y > 100.0f)
	{
		playerVel.y = 100.0f;
		//m_pPlayer->SetPlayerVelocity(playerVel);
	}
	if (playerVel.y < -100.0f)
	{
		playerVel.y = -100.0f;
		//m_pPlayer->SetPlayerVelocity(playerVel);
	}
	// Clamp player position on the left and upward side
	if (playerPos.x < 0.0f) playerPos.x = 0.0f;
	if (playerPos.y < 0.0f) playerPos.y = 0.0f;
	//m_pPlayer->SetPlayerPosition(playerPos);
	// Handle player falling off the level or finishing level and respawning
	if (playerPos.y > levelSize.y)
	{
		playerPos = playerSpawnPoint;
		playerVel = { 0.0f, 0.0f };
		//m_pPlayer->SetPlayerPosition(playerPos);
		//m_pPlayer->SetPlayerVelocity(playerVel);
	}
	if (playerPos.x > levelSize.x + 1)
	{
		playerPos = playerSpawnPoint;
		playerVel = { 0.0f,0.0f };
		//m_pPlayer->SetPlayerPosition(playerPos);
		//m_pPlayer->SetPlayerVelocity(playerVel);
	}


	// Handle collision & simple physics

	// Gravity physics
	playerVel.y += 20.0f * fElapsedTime;
	m_pPlayer->SetPlayerVelocity(playerVel);
	//std::cout << "Player velocity: " << playerVel.x << " " << playerVel.y << std::endl;
	//std::cout << "Player position: " << playerPos.x << " " << playerPos.y << std::endl;
	// Drag
	if (m_pPlayer->IsOnGround())
	{
		playerVel.x += -4.0f * playerVel.x * fElapsedTime;
		if (fabs(playerVel.x) < 0.01f)
			playerVel.x = 0.0f;
	}


	olc::vf2d potentialPlayerPos = playerPos + playerVel * fElapsedTime;
	tv.FillRect(potentialPlayerPos, { 1,1 }, olc::DARK_MAGENTA);
	// Check for collision

	/*bTouchWall = false;*/
	m_pPlayer->SetTouchedWall(false);
	if (playerVel.x <= 0) // Moving Left
	{
		if (GetTile(potentialPlayerPos.x + 0.0f, playerPos.y + 0.0f).GetCollisionType() != TILE_NOCOLLIDE || GetTile(potentialPlayerPos.x + 0.0f, playerPos.y + 0.9f).GetCollisionType() != TILE_NOCOLLIDE)
		{
			potentialPlayerPos.x = (int)potentialPlayerPos.x + 1;
			playerVel.x = 0;
			m_pPlayer->SetPlayerVelocity(playerVel);
		}
	}
	if (playerVel.x > 0) // Moving Right
	{
		if (GetTile(potentialPlayerPos.x + 1.0f, playerPos.y + 0.0f).GetCollisionType() != TILE_NOCOLLIDE || GetTile(potentialPlayerPos.x + 1.0f, playerPos.y + 0.9f).GetCollisionType() != TILE_NOCOLLIDE)
		{
			potentialPlayerPos.x = (int)potentialPlayerPos.x;
			playerVel.x = 0;
			m_pPlayer->SetPlayerVelocity(playerVel);
		}
	}

	/*bOnGround = false;*/
	m_pPlayer->SetOnGround(false);
	if (playerVel.y <= 0) // Moving Up
	{
		if (GetTile(potentialPlayerPos.x + 0.0f, potentialPlayerPos.y).GetCollisionType() != TILE_NOCOLLIDE || GetTile(potentialPlayerPos.x + 0.9f, potentialPlayerPos.y).GetCollisionType() != TILE_NOCOLLIDE)
		{
			potentialPlayerPos.y = (int)potentialPlayerPos.y + 1;
			playerVel.y = 0;
			m_pPlayer->SetPlayerVelocity(playerVel);
		}
	}
	else if (playerVel.y > 0) // Moving Down
	{
		if (GetTile(potentialPlayerPos.x + 0.0f, potentialPlayerPos.y + 1.0f).GetCollisionType() != TILE_NOCOLLIDE || GetTile(potentialPlayerPos.x + 0.9f, potentialPlayerPos.y + 1.0f).GetCollisionType() != TILE_NOCOLLIDE)
		{
			potentialPlayerPos.y = (int)potentialPlayerPos.y;
			playerVel.y = 0;
			//bOnGround = true; // Player has a solid surface underfoot
			m_pPlayer->SetOnGround(true);
			m_pPlayer->SetPlayerVelocity(playerVel);
		}
	}

	// Check for touching wall
	if (GetTile((int)potentialPlayerPos.x, playerPos.y + 0.0f).GetCollisionType() != TILE_NOCOLLIDE || GetTile((int)potentialPlayerPos.x, playerPos.y + 0.9f).GetCollisionType() != TILE_NOCOLLIDE)
	{
		//bTouchWall = true;
		m_pPlayer->SetTouchedWall(true);
	}

	if (GetTile(potentialPlayerPos.x + 1.0f, playerPos.y + 0.0f).GetCollisionType() != TILE_NOCOLLIDE || GetTile(potentialPlayerPos.x + 1.0f, playerPos.y + 0.9f).GetCollisionType() != TILE_NOCOLLIDE)
	{
		//bTouchWall = true;
		m_pPlayer->SetTouchedWall(true);
	}

	playerPos = potentialPlayerPos;
	m_pPlayer->SetPlayerPosition(playerPos);
}

void App::SetEditMode(bool EMode)
{
	m_bEditMode = EMode;
}

bool App::OnUserDestroy()
{
	return true;
}
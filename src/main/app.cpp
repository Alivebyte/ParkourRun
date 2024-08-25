#include "app.h"
#include "level.h"



bool App::OnUserCreate()
{
	m_bEditMode = true;
	// Initializing resource pack
	g_RM.InitializeResourcePack();

	// Loading resource pack
	g_RM.LoadResourcePack("content.pack", "TEST_KEY");
	
	vTileSize = { 32,32 };
	//splash = new olc::SplashScreen();
	backgroundDecal = g_RM.RM_Sprite("content/sprites/background.png");
	//pack = new olc::ResourcePack();
	levelSize = backgroundDecal->sprite->Size() / vTileSize;
	lm.GetLevel()->SetLevelSize(levelSize);

	
	//pack->LoadPack("content.pack", "TEST_KEY");
	// Load level file
	if (!lm.LoadLevelFile("content/levels/level1.map"))
	{
		std::cout << "Couldn't load level! The game will now exit";
		return false;
	}
	tv = olc::TileTransformedView(GetScreenSize(), vTileSize);
	
	if (!m_bEditMode)
	{
		// Create player
		m_pPlayer = new Player("content/sprites/player.png", &g_RM, lm.GetLevel()->GetLevelData()->playerSpawnPoint);

		// Initialize animations
		m_pPlayer->InitializeAnimations();
		m_pPlayer->SetTouchedWall(false);


		olc::vf2d* playerPos = m_pPlayer->GetPlayerPosition();

		//playerPos.y = ScreenHeight() - 250.0f;
		//printf("Background size: %d %d\n", .x / vTileSize, backgroundDecal->sprite->Size().y / vTileSize);

		camera = olc::utils::Camera2D(GetScreenSize() / vTileSize, *playerPos);

		camera.SetTarget(*playerPos);
		camera.SetMode(olc::utils::Camera2D::Mode::Simple);
		camera.SetWorldBoundary({ 0,0 }, backgroundDecal->sprite->Size() / vTileSize);
		camera.EnableWorldBoundary(true);
	}
	else // Editor mode, enable camera for zooming and panning
	{
		camera = olc::utils::Camera2D(GetScreenSize() / vTileSize);
		camera.SetMode(olc::utils::Camera2D::Mode::Simple);
	}


	return true;
};

bool App::OnUserUpdate(float fElapsedTime)
{
	// called once per frame
	Clear(olc::CYAN);

	if (!m_bEditMode)
	{
		// Handle user input
		m_pPlayer->HandleUserInput(this, fElapsedTime);

		// Handle collision
		HandleCollision(fElapsedTime);
	}
	// Handle player camera movement
	CameraMovement(fElapsedTime, m_bEditMode);

	if (m_bEditMode)
	{
		HandleEditMode(fElapsedTime);
	}
	// Do Render
	Render(fElapsedTime);

	return true;
}

void App::Render(float fElapsedTime)
{
	Level* level = lm.GetLevel();
	// Render background
	tv.DrawDecal({ 0,0 }, backgroundDecal);

	// Render level
	if (level)
	{
		auto ld = level->GetLevelData();


		if (ld)
		{
			auto tiles = level->GetTiles();
			if (tiles)
			{
				for (int y = 0; y < levelSize.y; y++)
				{
					for (int x = 0; x < levelSize.x; x++)
					{
						auto tile = tiles[y * levelSize.x + x];
						if (tile.GetSprite())
						{
							if (!m_bEditMode)
							{
								tv.DrawDecal(tile.GetPosition(), tile.GetSprite());
							}
							else
							{
								// Draw non-collided types as tinted magenta 
								if (tile.GetCollisionType() == TILE_NOCOLLIDE)
								{
									tv.DrawDecal(tile.GetPosition(), tile.GetSprite(), {1.0f, 1.0f}, olc::MAGENTA);
								}
								else
								{
									tv.DrawDecal(tile.GetPosition(), tile.GetSprite());
								}
							}
						}
					}
				}
			}


			if (!m_bEditMode || !m_pPlayer)
			{
				auto& animator = *m_pPlayer->GetAnimator();

				//Render player
				if (!animator.GetAnim("Idle")->bIsPlaying)
					animator.Play("Idle");

				animator.UpdateAnimations(fElapsedTime);

				animator.DrawAnimationFrame(*m_pPlayer->GetPlayerPosition(), 0.0f, &tv);
			}
			else
			{
				
				tv.DrawDecal(ld->playerSpawnPoint, g_RM.RM_Sprite("content/sprites/world_objects/spawnpoint.png"));

				DrawStringDecal({ 0,0 }, "EDTTOR MODE", olc::WHITE, { 2.0f, 2.0f });
			}
		}
	}
}

void App::HandleEditMode(float fElapsedTime)
{
	Level *level = lm.GetLevel();

	if (level)
	{
		Tile* tiles = level->GetTiles();
		LevelData *ld = level->GetLevelData();

		olc::vi2d screen_mouse_pos = GetMousePos();
		olc::vi2d world_pos = tv.ScreenToWorld(screen_mouse_pos);

		static std::string selected_sprite = "content/sprites/world_objects/test_wall.png";
		static olc::Sprite::Flip flip;

		std::cout << "Screen pos:" << screen_mouse_pos << std::endl;
		std::cout << "World pos:" << world_pos << std::endl;

		world_pos.x = std::clamp(world_pos.x, 0, levelSize.x);
		world_pos.y = std::clamp(world_pos.y, 0, levelSize.y);

		// Sprite selection
		if (GetKey(olc::T).bPressed)
		{
			selected_sprite = "content/sprites/world_objects/industrial_pipe_horizontal.png";
		}
		else if (GetKey(olc::R).bPressed)
		{
			selected_sprite = "content/sprites/world_objects/industrial_pipe_turn90.png";
		}
		else if (GetKey(olc::V).bPressed)
		{
			selected_sprite = "content/sprites/world_objects/industrial_pipe_vertical.png";
		}
		else if (GetKey(olc::B).bPressed)
		{
			selected_sprite = "content/sprites/world_objects/tile_wall1.png";
		}
		else if (GetKey(olc::N).bPressed)
		{
			selected_sprite = "content/sprites/world_objects/tile_wall2.png";
		}
		else if (GetKey(olc::M).bPressed)
		{
			selected_sprite = "content/sprites/world_objects/tile_wall3.png";
		}
		else if (GetKey(olc::D).bPressed)
		{
			selected_sprite = "content/sprites/world_objects/tile_wall4.png";
		}
		else if (GetKey(olc::H).bPressed)
		{
			selected_sprite = "content/sprites/world_objects/tile_wall5.png";
		}
		else if (GetKey(olc::J).bPressed)
		{
			selected_sprite = "content/sprites/world_objects/tile_wall6.png";
		}

		// Add tile at cursor position
		if (GetMouse(0).bHeld)
		{
		
			//if ((world_pos.x > levelSize.x || world_pos.x < 0) || (world_pos.y > levelSize.y || world_pos.y < 0)) world_pos = world_pos;
			if (tiles)
			{
				tiles[world_pos.y * levelSize.x + world_pos.x].SetSprite(selected_sprite, &g_RM);
				//tiles[world_pos.y * levelSize.x + world_pos.x].GetSprite()->sprite..
				tiles[world_pos.y * levelSize.x + world_pos.x].SetCollisionType(TILE_COLLIDE);
			}
		}

		// Remove tile at cursor position
		if (GetMouse(1).bHeld)
		{
			

			if (tiles)
			{
				tiles[world_pos.y * levelSize.x + world_pos.x].Reset();
			}
		}

		// Save level
		if (GetKey(olc::S).bPressed)
		{
			ld->m_TilesInfo.clear();

			for (int y = 0; y < levelSize.y; y++)
			{
				for (int x = 0; x < levelSize.x; x++)
				{
					if (tiles)
					{
						auto tile = tiles[y * levelSize.x + x];
						if (tile.GetSprite())
						{
							TileInfo tinfo;
							tinfo.flip = (olc::Sprite::Flip)tile.GetSpriteFlip();
							tinfo.m_iColType =(TILE_COLLISION_TYPES)tile.GetCollisionType();
							tinfo.m_spriteName = tile.GetSpriteName();
							tinfo.m_viPos = tile.GetPosition();

							ld->m_TilesInfo.push_back(tinfo);
						}
					}
				}
			}

			lm.SaveLevelFile("content/levels/level1.map");
		}

		// Set player spawn position
		if(GetKey(olc::P).bPressed)
		{
			ld->playerSpawnPoint = world_pos;
		}

		// Toggle collision state
		if (GetKey(olc::C).bPressed)
		{
			if (tiles)
			{
				tiles[world_pos.y * levelSize.x + world_pos.x].SetCollisionType(
					tiles[world_pos.y * levelSize.x + world_pos.x].GetCollisionType() == TILE_NOCOLLIDE ? TILE_COLLIDE : TILE_NOCOLLIDE
				);
			}
		}

		
	}
}

void App::CameraMovement(float fElapsedTime, bool bEditMode)
{
	if (!bEditMode)
	{
		camera.SetMode(olc::utils::Camera2D::Mode::LazyFollow);
		bool bOnScreen = camera.Update(fElapsedTime);
		tv.SetWorldOffset(camera.GetViewPosition());
	}
	else
	{
		tv.HandlePanAndZoom();
	}
}

void App::HandleCollision(float fElapsedTime)
{
	auto GetTile = [&](int x, int y, LevelManager lm)
		{
			auto tiles = lm.GetLevel()->GetTiles();
			if (x >= 0 && x < levelSize.x && y >= 0 && y < levelSize.y)
				return tiles[y * levelSize.x + x];
			else
				return Tile();
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
		if (GetTile(potentialPlayerPos.x + 0.0f, playerPos.y + 0.0f, lm).GetCollisionType() != TILE_NOCOLLIDE || GetTile(potentialPlayerPos.x + 0.0f, playerPos.y + 0.9f,lm).GetCollisionType() != TILE_NOCOLLIDE)
		{
			potentialPlayerPos.x = (int)potentialPlayerPos.x + 1;
			playerVel.x = 0;
			m_pPlayer->SetPlayerVelocity(playerVel);
		}
	}
	if (playerVel.x > 0) // Moving Right
	{
		if (GetTile(potentialPlayerPos.x + 1.0f, playerPos.y + 0.0f,lm).GetCollisionType() != TILE_NOCOLLIDE || GetTile(potentialPlayerPos.x + 1.0f, playerPos.y + 0.9f,lm).GetCollisionType() != TILE_NOCOLLIDE)
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
		if (GetTile(potentialPlayerPos.x + 0.0f, potentialPlayerPos.y,lm).GetCollisionType() != TILE_NOCOLLIDE || GetTile(potentialPlayerPos.x + 0.9f, potentialPlayerPos.y,lm).GetCollisionType() != TILE_NOCOLLIDE)
		{
			potentialPlayerPos.y = (int)potentialPlayerPos.y + 1;
			playerVel.y = 0;
			m_pPlayer->SetPlayerVelocity(playerVel);
		}
	}
	else if (playerVel.y > 0) // Moving Down
	{
		if (GetTile(potentialPlayerPos.x + 0.0f, potentialPlayerPos.y + 1.0f,lm).GetCollisionType() != TILE_NOCOLLIDE || GetTile(potentialPlayerPos.x + 0.9f, potentialPlayerPos.y + 1.0f,lm).GetCollisionType() != TILE_NOCOLLIDE)
		{
			potentialPlayerPos.y = (int)potentialPlayerPos.y;
			playerVel.y = 0;
			//bOnGround = true; // Player has a solid surface underfoot
			m_pPlayer->SetOnGround(true);
			m_pPlayer->SetPlayerVelocity(playerVel);
		}
	}

	// Check for touching wall
	if (GetTile((int)potentialPlayerPos.x, playerPos.y + 0.0f,lm).GetCollisionType() != TILE_NOCOLLIDE || GetTile((int)potentialPlayerPos.x, playerPos.y + 0.9f,lm).GetCollisionType() != TILE_NOCOLLIDE)
	{
		//bTouchWall = true;
		m_pPlayer->SetTouchedWall(true);
	}

	if (GetTile(potentialPlayerPos.x + 1.0f, playerPos.y + 0.0f,lm).GetCollisionType() != TILE_NOCOLLIDE || GetTile(potentialPlayerPos.x + 1.0f, playerPos.y + 0.9f,lm).GetCollisionType() != TILE_NOCOLLIDE)
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
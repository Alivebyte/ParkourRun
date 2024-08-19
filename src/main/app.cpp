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

	//playerPos.y = ScreenHeight() - 250.0f;

	camera = olc::utils::Camera2D(GetScreenSize() / vTileSize, playerPos);

	camera.SetTarget(playerPos);
	camera.SetMode(olc::utils::Camera2D::Mode::Simple);
	camera.SetWorldBoundary({ 0,0 }, {80,75});
	camera.EnableWorldBoundary(true);


	return true;
};

bool App::OnUserUpdate(float fElapsedTime)
{
	// called once per frame
	Clear(olc::CYAN);

	// Handle collision & simple physics

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
	
	camera.SetMode(olc::utils::Camera2D::Mode::LazyFollow);

	bool bOnScreen = camera.Update(fElapsedTime);



	tv.SetWorldOffset(camera.GetViewPosition());

	// Render background
	SetPixelMode(olc::Pixel::NORMAL);

	tv.DrawDecal({ 0,0 }, backgroundDecal, { 1.0f, 1.0f });


	// Render world
	SetPixelMode(olc::Pixel::ALPHA);

	

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
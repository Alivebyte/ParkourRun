#include "app.h"

bool App::OnUserCreate()
{
	tv = olc::TileTransformedView(GetScreenSize(), { 32,32 });

	splash = new olc::SplashScreen();

	pack = new olc::ResourcePack();

	pack->LoadPack("content.pack", "TEST_KEY");

	playerSprite = new olc::Sprite("content/sprites/player.png", pack);
	backgroundSprite = new olc::Sprite("content/sprites/background_test.png",pack);

	pipeHorizontalSprite = new olc::Sprite("content/sprites/world_objects/industrial_pipe_horizontal.png",pack);

	pipeVerticalSprite = new olc::Sprite("content/sprites/world_objects/industrial_pipe_vertical.png",pack);

	pipeTurnSprite = new olc::Sprite("content/sprites/world_objects/industrial_pipe_turn90.png",pack);

	//playerPos.y = ScreenHeight() - 250.0f;

	camera = olc::utils::Camera2D(GetScreenSize() / olc::vi2d(32, 32), playerPos);

	camera.SetTarget(playerPos);
	camera.SetMode(olc::utils::Camera2D::Mode::Simple);
	camera.SetWorldBoundary({ 0,0 }, backgroundSprite->Size());
	//camera.EnableWorldBoundary(true);


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

	tv.DrawSprite({ 0,0 }, backgroundSprite, { 1.0f, 1.0f });


	// Render world
	SetPixelMode(olc::Pixel::ALPHA);

	

	tv.DrawSprite({ 0,0 }, pipeHorizontalSprite, { 1.0f, 1.0f});
	tv.DrawSprite({ 32,0 }, pipeTurnSprite, {1.0f,1.0f}, olc::Sprite::Flip::HORIZ);
	tv.DrawSprite({ 32,32 }, pipeVerticalSprite, {1.0f,1.0f}, olc::Sprite::Flip::HORIZ);

	// Render player
	tv.DrawSprite(playerPos, playerSprite, {1,1});

	SetPixelMode(olc::Pixel::NORMAL);

	

	return true;
}

bool App::OnUserDestroy()
{
	return true;
}
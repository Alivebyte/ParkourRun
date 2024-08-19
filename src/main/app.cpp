#include "app.h"

bool App::OnUserCreate()
{
	splash = new olc::SplashScreen();

	pack = new olc::ResourcePack();

	pack->LoadPack("content.pack", "TEST_KEY");

	playerSprite = new olc::Sprite("content/sprites/player.png", pack);
	backgroundSprite = new olc::Sprite("content/sprites/background_test.png",pack);

	pipeHorizontalSprite = new olc::Sprite("content/sprites/world_objects/industrial_pipe_horizontal.png",pack);

	pipeVerticalSprite = new olc::Sprite("content/sprites/world_objects/industrial_pipe_vertical.png",pack);

	pipeTurnSprite = new olc::Sprite("content/sprites/world_objects/industrial_pipe_turn90.png",pack);

	playerPos.y = ScreenHeight() - 250.0f;

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
		playerPos.y -= 1.0f;// *fElapsedTime;
	}

	if (GetKey(olc::Key::S).bHeld)
	{
		playerPos.y += 1.0f;// *fElapsedTime;
	}

	if (GetKey(olc::Key::A).bHeld)
	{
		playerPos.x -= 1.0f;// *fElapsedTime;
	}

	if (GetKey(olc::Key::D).bHeld)
	{
		playerPos.x += 1.0f;// *fElapsedTime;
	}
	

	// Render background
	SetPixelMode(olc::Pixel::NORMAL);

	DrawPartialSprite({ 0,0 }, backgroundSprite, { (int)playerPos.x + 20, (int)playerPos.y - 250}, { ScreenWidth(), ScreenHeight()}, 1);


	// Render world
	SetPixelMode(olc::Pixel::ALPHA);

	

	DrawSprite({ 0,0 }, pipeHorizontalSprite, 1);
	DrawSprite({ 32,0 }, pipeTurnSprite, 1, olc::Sprite::Flip::HORIZ);
	DrawSprite({ 32,32 }, pipeVerticalSprite, 1, olc::Sprite::Flip::HORIZ);

	// Render player
	DrawSprite((olc::vi2d)playerPos, playerSprite, 2);

	SetPixelMode(olc::Pixel::NORMAL);

	

	return true;
}

bool App::OnUserDestroy()
{
	return true;
}
#include "app.h"

bool App::OnUserCreate()
{
	pack = new olc::ResourcePack();

	pack->LoadPack("content.pack", "TEST_KEY");

	playerSprite = new olc::Sprite("content/sprites/player.png", pack);
	backgroundSprite = new olc::Sprite("content/sprites/background_test.png",pack);

	playerPos.y = ScreenHeight() - 250.0f;

	return true;
};

bool App::OnUserUpdate(float fElapsedTime)
{
	// called once per frame
	Clear(olc::CYAN);

	/*for (int x = 0; x < ScreenWidth(); x++)
		for (int y = 0; y < ScreenHeight(); y++)
			Draw(x, y, olc::Pixel(rand() % 256, rand() % 256, rand() % 256));*/
	
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

	SetPixelMode(olc::Pixel::NORMAL);

	DrawPartialSprite({ 0,0 }, backgroundSprite, { (int)playerPos.x + 20, (int)playerPos.y - 250}, { ScreenWidth(), ScreenHeight()}, 1);

	// Stoopid rendah
	SetPixelMode(olc::Pixel::ALPHA);

	DrawSprite((olc::vi2d)playerPos, playerSprite, 2);

	SetPixelMode(olc::Pixel::NORMAL);

	return true;
}

bool App::OnUserDestroy()
{
	return true;
}
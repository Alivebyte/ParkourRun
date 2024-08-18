#include "app.h"

bool App::OnUserCreate()
{
	return true;
};

bool App::OnUserUpdate(float fElapsedTime)
{
	// called once per frame
	for (int x = 0; x < ScreenWidth(); x++)
		for (int y = 0; y < ScreenHeight(); y++)
			Draw(x, y, olc::Pixel(rand() % 256, rand() % 256, rand() % 256));
	return true;
}

bool App::OnUserDestroy()
{
	return true;
}
#define OLC_PGE_APPLICATION
#define OLC_PGEX_SPLASHSCREEN
#include "app.h"

int main()
{
	App game;

	if (game.Construct(800, 600, 1, 1))
		game.Start();

	return 0;
}
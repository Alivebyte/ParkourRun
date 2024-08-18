#define OLC_PGE_APPLICATION
#include "app.h"

int main()
{
	App game;

	if (game.Construct(800, 600, 1, 1))
		game.Start();

	return 0;
}
#define OLC_PGE_APPLICATION
#define OLC_PGEX_SPLASHSCREEN
#define OLC_PGEX_TRANSFORMEDVIEW
#define OLC_PGEX_RESOURCE_MANAGER_IMPLEMENTATION
#define ANIMATOR_IMPLEMENTATION
#define OLC_PGEX_RAYCAST2D_IMPLEMENTATION
#include "app.h"

int main()
{
	App game;

	if (game.Construct(800, 600, 1, 1))
		game.Start();

	return 0;
}
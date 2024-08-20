#include "olcPixelGameEngine.h"
#include "resource_manager.h"

class olc::Decal;

typedef enum
{
	TILE_AIR = 0,
	TILE_NOCOLLIDE = 5,
	TILE_COLLIDE = 10
} TILE_TYPES;

class Tile
{
public:
	Tile();
	Tile(std::string filename, GameResourceManager* rm, olc::ResourcePack* pack);

	olc::Decal* GetSprite();
	void SetSprite(std::string filename, GameResourceManager* rm, olc::ResourcePack* pack);

	void SetType(TILE_TYPES type);
	int GetType();

private:
	olc::Decal* m_TileSprite;
	int m_Type;
};
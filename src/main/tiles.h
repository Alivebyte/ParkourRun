#include "olcPixelGameEngine.h"
#include "resource_manager.h"
#include "animator.h"

class olc::Decal;

typedef enum
{
	TILE_NOCOLLIDE = 0,
	TILE_COLLIDE = 10
} TILE_COLLISION_TYPES;

class Tile
{
public:
	Tile();
	Tile(std::string filename, GameResourceManager* rm, olc::ResourcePack* pack);
	~Tile();

	olc::Decal* GetSprite();
	void SetSprite(std::string filename, GameResourceManager* rm, olc::ResourcePack* pack);

	void SetCollisionType(TILE_COLLISION_TYPES type);
	int GetCollisionType();

private:

	GameResourceManager* m_pRM;
	std::string m_sSprite_name;
	olc::Decal* m_pTileSprite;
	int m_iColType;
};
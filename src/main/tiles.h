#include "olcPixelGameEngine.h"
#include "resource_manager.h"

class olc::Decal;

class Tile
{
public:
	Tile();
	Tile(std::string filename, GameResourceManager* rm, olc::ResourcePack* pack);

	olc::Decal* GetSprite();
	void SetSprite(std::string filename, GameResourceManager* rm, olc::ResourcePack* pack);

	void SetType(int type);
	int GetType();

private:
	olc::Decal* m_TileSprite;
	int m_Type;
};

// Tile with collisions
class CollisionTile : public Tile
{
public:
	CollisionTile();

	bool TestCollision();
};
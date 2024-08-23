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

	std::string GetSpriteName() { return m_sSprite_name; };

	void SetCollisionType(TILE_COLLISION_TYPES type);
	int GetCollisionType();

protected:
	GameResourceManager* m_pRM;
	GameAnimator m_Animator;
	std::string m_sSprite_name;
	olc::Decal* m_pTileSprite;
	int m_iColType;
};
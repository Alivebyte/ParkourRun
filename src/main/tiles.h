#include "olcPixelGameEngine.h"
#include "resource_manager.h"
#include "animator.h"

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
	olc::vi2d GetPosition() { return m_viPos; };

	void SetCollisionType(TILE_COLLISION_TYPES type);
	int GetCollisionType();

	GameAnimator* GetAnimator() { return &m_Animator; };

protected:
	GameResourceManager* m_pRM;
	GameAnimator m_Animator;
	std::string m_sSprite_name;
	olc::Decal* m_pTileSprite;
	int m_iColType;
private:
	olc::vi2d m_viPos;
};

class Player : public Tile
{

	typedef Tile BaseClass;

public:
	Player();
	Player(std::string filename, GameResourceManager* rm, olc::ResourcePack* pack, olc::vf2d spawn_pos);

	void InitializeAnimations(olc::ResourcePack* pack);


	void SetPlayerPosition(olc::vf2d position) { m_vfPlayerPos = position; };
	void SetPlayerPosition(float x, float y) { m_vfPlayerPos.x = x; m_vfPlayerPos.y = y; };
	olc::vf2d* GetPlayerPosition() { return &m_vfPlayerPos; };

	void SetPlayerSpawnPosition(olc::vf2d position) { m_vfPlayerSpawnPoint = position; };
	void SetPlayerSpawnPosition(float x, float y) { m_vfPlayerSpawnPoint.x = x; m_vfPlayerSpawnPoint.y = y; };
	olc::vf2d* GetPlayerSpawnPosition() { return &m_vfPlayerSpawnPoint; };

	void SetPlayerVelocity(olc::vf2d velocity) { m_vfPlayerVel = velocity; };
	void SetPlayerVelocity(float x, float y) { m_vfPlayerVel.x = x; m_vfPlayerVel.y = y; };
	olc::vf2d* GetPlayerVelocity() { return &m_vfPlayerVel; };

	bool IsOnGround() { return m_bIsOnGround; };
	bool HasTouchedWall() { return m_bIsOnGround; };

	void SetOnGround(bool onGround = false) { m_bIsOnGround = onGround; };
	void SetTouchedWall(bool touchWall = false) { m_bTouchWall = touchWall; };

	void HandleUserInput(olc::PixelGameEngine* pge, float fElapsedTime);

	olc::vf2d m_vfPlayerPos;
	olc::vf2d m_vfPlayerSpawnPoint;
	olc::vf2d m_vfPlayerVel;

private:

	bool m_bIsOnGround;
	bool m_bTouchWall;
};

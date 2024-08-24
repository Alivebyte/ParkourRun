#include "olcPixelGameEngine.h"
#include "tiles.h"

Tile::Tile() : m_pTileSprite(nullptr), m_iColType(TILE_NOCOLLIDE), m_pRM(nullptr)
{
	//std::cout << "Tile created\n";
}

Tile::~Tile()
{
	if (m_pRM)
	{
		if (m_pTileSprite)
		{
			if (!m_sSprite_name.empty())
			{
				m_pRM->RM_FreeSpriteData(m_sSprite_name);
				m_pTileSprite = nullptr;
			}
			else
			{
				delete m_pTileSprite;
				m_pTileSprite = nullptr;
			}
		}
	}
}

Tile::Tile(std::string filename, GameResourceManager* rm, olc::Sprite::Flip flip) : m_iColType(TILE_NOCOLLIDE), m_flip(olc::Sprite::Flip::NONE)
{
	m_sSprite_name = filename;
	m_pTileSprite = rm->RM_Sprite(filename);
	m_pRM = rm;
}

void Tile::SetSprite(std::string filename, GameResourceManager* rm)
{
	m_sSprite_name = filename;
	m_pTileSprite = rm->RM_Sprite(filename);
	m_pRM = rm;
}

olc::Decal* Tile::GetSprite()
{
	return m_pTileSprite;
}

int Tile::GetCollisionType()
{
	return m_iColType;
}

void Tile::SetCollisionType(TILE_COLLISION_TYPES type)
{
	m_iColType = type;
}

Player::Player()
{

}

Player::Player(std::string filename, GameResourceManager* rm, olc::vf2d spawn_pos)
{
	//BaseClass::Tile(filename, rm, pack);
	m_pTileSprite = rm->RM_Sprite(filename);

	m_sSprite_name = filename;
	m_pRM = rm;
	m_vfPlayerPos = spawn_pos;
	m_vfPlayerSpawnPoint = spawn_pos;
}

void Player::InitializeAnimations()
{
	// Idle
	m_Animator.AddAnimation("Idle", 2, 4, m_pRM->RM_Sprite(m_sSprite_name), {0.0f,0.0f}, {32.0f,32.0f}, {0.0f,0.0f}, {0.0f, 0.0f}, true);
	// Running
	// Climbing
	// Jump
}

void Player::HandleUserInput(olc::PixelGameEngine* pge, float fElapsedTime)
{
	if (pge->GetKey(olc::Key::W).bHeld)
	{
		if (HasTouchedWall())
			m_vfPlayerVel.y += -30.0f * fElapsedTime;
	} //else playerDir = { 0.0f, 0.0f };
	if (pge->GetKey(olc::Key::S).bHeld)
	{
		m_vfPlayerVel.y = 2.0f;
	} //else playerDir = { 0.0f, 0.0f };
	if (pge->GetKey(olc::Key::A).bHeld)
	{
		m_vfPlayerVel.x += (m_bIsOnGround ? -50.0f : -1.0f) * fElapsedTime;

	} //else playerDir = { 0.0f, 0.0f };
	if (pge->GetKey(olc::Key::D).bHeld)
	{
		m_vfPlayerVel.x += (m_bIsOnGround ? 50.0f : 1.0f) * fElapsedTime;
	}
	if (pge->GetKey(olc::Key::SPACE).bPressed)
	{
		if (m_vfPlayerVel.y == 0)
		{
			m_vfPlayerVel.y = -12.0f;
		}
	}
}
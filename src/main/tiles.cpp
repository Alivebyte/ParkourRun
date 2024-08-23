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

Tile::Tile(std::string filename, GameResourceManager* rm, olc::ResourcePack* pack) : m_iColType(0)
{
	m_sSprite_name = filename;
	m_pTileSprite = rm->RM_Sprite(filename, pack);
	m_pRM = rm;
}

void Tile::SetSprite(std::string filename, GameResourceManager* rm, olc::ResourcePack* pack)
{
	m_sSprite_name = filename;
	m_pTileSprite = rm->RM_Sprite(filename, pack);
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
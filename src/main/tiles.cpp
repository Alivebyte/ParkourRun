#include "olcPixelGameEngine.h"
#include "tiles.h"

Tile::Tile() : m_TileSprite(nullptr), m_Type(0), m_rm(nullptr)
{
	std::cout << "Tile created\n";
}

Tile::~Tile()
{
	if (m_rm)
	{
		if (m_TileSprite)
		{
			if (!m_sprite_name.empty())
			{
				m_rm->RM_FreeSpriteData(m_sprite_name);
				m_TileSprite = nullptr;
			}
			else
			{
				delete m_TileSprite;
				m_TileSprite = nullptr;
			}
		}
	}
}

Tile::Tile(std::string filename, GameResourceManager* rm, olc::ResourcePack* pack) : m_Type(0)
{
	m_sprite_name = filename;
	m_TileSprite = rm->RM_Sprite(filename, pack);
	m_rm = rm;
}

void Tile::SetSprite(std::string filename, GameResourceManager* rm, olc::ResourcePack* pack)
{
	m_sprite_name = filename;
	m_TileSprite = rm->RM_Sprite(filename, pack);
	m_rm = rm;
}

olc::Decal* Tile::GetSprite()
{
	return m_TileSprite;
}

int Tile::GetType()
{
	return m_Type;
}

void Tile::SetType(TILE_TYPES type)
{
	m_Type = type;
}
#include "olcPixelGameEngine.h"
#include "tiles.h"

Tile::Tile() : m_TileSprite(nullptr), m_Type(0)
{
	std::cout << "Tile created\n";
}

Tile::Tile(std::string filename, GameResourceManager* rm, olc::ResourcePack* pack) : m_Type(0)
{
	m_TileSprite = rm->RM_Sprite(filename, pack);
}

void Tile::SetSprite(std::string filename, GameResourceManager* rm, olc::ResourcePack* pack)
{
	m_TileSprite = rm->RM_Sprite(filename, pack);
}

olc::Decal* Tile::GetSprite()
{
	return m_TileSprite;
}

int Tile::GetType()
{
	return m_Type;
}

void Tile::SetType(int type)
{
	m_Type = type;
}
#include "level.h"
#include "app.h"

Level::Level()
{

}

Level::~Level()
{

}

void Level::Create()
{
	int nNumTiles = m_ld.m_TilesInfo.size();
	for (int i = 0; i < nNumTiles; i++)
	{
		TileInfo tinfo = m_ld.m_TilesInfo[i];
		//Tile tile = Tile(tinfo.m_spriteName, GetGlobalResourceManager(), );
	}

	OnLevelCreate();
}

void Level::OnLevelCreate()
{

}

void Level::OnLevelLoad()
{

}
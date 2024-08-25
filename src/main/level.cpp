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
	tiles = new Tile[m_levelSize.x * m_levelSize.y];
	int nNumTiles = m_ld.m_TilesInfo.size();
	for (int i = 0; i < nNumTiles; i++)
	{
		TileInfo tinfo = m_ld.m_TilesInfo[i];
		//Tile tile = Tile(tinfo.m_spriteName, GetGameResourceManager());
		//tile.SetPosition(tinfo.m_viPos);

		for (int y = 0; y < m_levelSize.y; y++)
		{
			for (int x = 0; x < m_levelSize.x; x++)
			{
				if (x == tinfo.m_viPos.x && y == tinfo.m_viPos.y)
				{
					//tiles->at(y*24+x) = Tile("content/sprites/world_objects/industrial_pipe_vertical.png", &rm, pack);
					//tiles->at(y* 24 + x).SetType(10);
					tiles[y * m_levelSize.x + x].SetSprite(tinfo.m_spriteName, &g_RM);
					tiles[y * m_levelSize.x + x].SetCollisionType(tinfo.m_iColType);
					//tiles[y * m_levelSize.x + x].SetPosition(tinfo.m_viPos);
				}
				tiles[y * m_levelSize.x + x].SetPosition(olc::vi2d(x,y));
			}
		}

		//tiles.push_back(tile);
	}
	

	OnLevelCreate();
}

void Level::OnLevelCreate()
{
	
}

//void Level::OnLevelLoad()
//{
//
//}
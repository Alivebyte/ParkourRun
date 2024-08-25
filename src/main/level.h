#pragma once

#include "olcPixelGameEngine.h"
#include "tiles.h"

struct TileInfo
{
	std::string m_spriteName;
	olc::vi2d m_viPos;
	TILE_COLLISION_TYPES m_iColType;
	olc::Sprite::Flip flip;
};

struct LevelData
{
public:
	olc::vi2d playerSpawnPoint;
	std::vector<TileInfo> m_TilesInfo;
};

class Level
{
private:
	LevelData m_ld;
	Tile* tiles;
	olc::vi2d m_levelSize;

public:
	Level();
	~Level();
	//void Load();
	void Create();
	virtual void OnLevelCreate();
	//virtual void OnLevelLoad();

	olc::vi2d* GetLevelSize() { return &m_levelSize; };
	void SetLevelSize(olc::vi2d levelSize) { m_levelSize = levelSize; };


	LevelData* GetLevelData() { return &m_ld; };
	Tile* GetTiles() { return tiles; };
	
};
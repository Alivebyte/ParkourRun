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
	TileInfo* m_TilesInfo;
};

class Level
{
private:
	LevelData m_ld;
public:
	Level();
	~Level();
	void Load();
	void Create();
	void OnLevelCreate();
	void OnLevelLoad();

	LevelData* GetLevelData() { return &m_ld; };


	
};
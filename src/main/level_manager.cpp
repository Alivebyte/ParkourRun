#include "level_manager.h"
#include "utilities/olcUTIL_DataFile.h"

LevelManager::LevelManager()
{

}

bool LevelManager::LoadLevelFile(std::string filename)
{
	auto ld = m_Level.GetLevelData();

	olc::utils::datafile df;

	if (olc::utils::datafile::Read(df, filename))
	{

		auto& level_node = df["level_node"];
		auto& player_spawnPoint = level_node["player_spawnpoint"];
		ld->playerSpawnPoint.x = player_spawnPoint["X"].GetInt();
		ld->playerSpawnPoint.y = player_spawnPoint["Y"].GetInt();

		
		int nTileInfoCount = level_node["tile_info_count"].GetInt();

		
		for (int i = 0; i < nTileInfoCount; i++)
		{
			TileInfo tinfo;
			
			auto& TileInfoNode = level_node["tile_info[" + std::to_string(i) + "]"];

			tinfo.m_spriteName = TileInfoNode["SpriteFile"].GetString();
			auto& tilePos = TileInfoNode["TilePosition"];
			tinfo.m_viPos.x = tilePos["X"].GetInt();
			tinfo.m_viPos.y = tilePos["Y"].GetInt();
			tinfo.flip = (olc::Sprite::Flip)TileInfoNode["Flip"].GetInt();
			tinfo.m_iColType = (TILE_COLLISION_TYPES)TileInfoNode["CollisionType"].GetInt();

			ld->m_TilesInfo.push_back(tinfo);
		}

		m_Level.Create();
	}
	else return false;

	return true;
}

bool LevelManager::SaveLevelFile(std::string filename)
{
	
	auto ld = m_Level.GetLevelData();
	if (ld)
	{
		olc::utils::datafile df;

		auto& level_node = df["level_node"];
		auto& player_spawnPoint = level_node["player_spawnpoint"];
		player_spawnPoint["X"].SetInt(ld->playerSpawnPoint.x);
		player_spawnPoint["Y"].SetInt(ld->playerSpawnPoint.y);
		
		if (!ld->m_TilesInfo.empty())
		{
			level_node["tile_info_count"].SetInt(ld->m_TilesInfo.size());

			for (int i = 0; i < ld->m_TilesInfo.size(); i++)
			{
				auto tinfo = ld->m_TilesInfo[i];
				auto& TileInfoNode = level_node["tile_info["+std::to_string(i)+"]"];
				TileInfoNode["SpriteFile"].SetString(tinfo.m_spriteName);
				auto& tilePos = TileInfoNode["TilePosition"];
				tilePos["X"].SetInt(tinfo.m_viPos.x);
				tilePos["Y"].SetInt(tinfo.m_viPos.y);

				TileInfoNode["Flip"].SetInt(tinfo.flip);
				TileInfoNode["CollisionType"].SetInt(tinfo.m_iColType);
			}
		}
		olc::utils::datafile::Write(df, filename);
	}

	return true;
}
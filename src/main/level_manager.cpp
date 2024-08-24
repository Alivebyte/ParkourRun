#include "level_manager.h"
#include "utilities/olcUTIL_DataFile.h"

LevelManager::LevelManager()
{

}

bool LevelManager::LoadLevelFile(std::string filename)
{
	olc::utils::datafile datafile;

	return true;
}

bool LevelManager::SaveLevel(std::string filename, Level* level)
{
	//FILE* fstream = fopen(filename.c_str(), "wb+");

	//if (fstream == NULL)
	//{
	//	printf("Couldn't open level file %s!\n", filename.c_str());
	//	return false;
	//}
	//
	//if (level)
	//{
	//	LevelData* ld = level->GetLevelData();

	//	char ldstr[1024] = "";
	//	char end[] = "\2";
	//	char next[] = "\n";
	//	//snprintf()

	//	if (ld)
	//	{
	//		std::strcat(ldstr, std::to_string(ld->playerSpawnPoint.x).c_str());
	//		strcat(ldstr, end);
	//		std::strcat(ldstr, std::to_string(ld->playerSpawnPoint.x).c_str());
	//		strcat(ldstr, next);

	//		if (ld->m_TilesInfo)
	//		{
	//			for (int i = 0; i < 10; i++)
	//			{
	//				auto& tinfo = ld->m_TilesInfo[i];
	//				strcat(ldstr, tinfo.m_spriteName.c_str());
	//				strcat(ldstr, end);
	//				strcat(ldstr, std::to_string(tinfo.m_viPos.x).c_str());
	//				strcat(ldstr, end);
	//				strcat(ldstr, std::to_string(tinfo.m_viPos.y).c_str());
	//				strcat(ldstr, end);
	//				strcat(ldstr, std::to_string(tinfo.flip).c_str());
	//				strcat(ldstr, end);
	//				strcat(ldstr, std::to_string(tinfo.m_iColType).c_str());
	//				strcat(ldstr, next);
	//			}
	//		}

	//		fputs(ldstr, fstream);
	//	}
	//}

	//fclose(fstream);

	// Datafile Test
	if (level)
	{
		auto ld = level->GetLevelData();
		if (ld)
		{
			olc::utils::datafile df;

			auto& level_node = df["level_node"];
			auto& player_spawnPoint = level_node["player_spawnpoint"];
			player_spawnPoint["X"].SetInt(ld->playerSpawnPoint.x);
			player_spawnPoint["Y"].SetInt(ld->playerSpawnPoint.y);

			if (!ld->m_TilesInfo.empty())
			{
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
	}

	return true;
}
#include "resource_manager.h"

//#ifdef OLC_PGEX_RESOURCE_MANAGER_IMPLEMENTATION
#undef OLC_PGEX_RESOURCE_MANAGER_IMPLEMENTATION
bool GameResourceManager::i_NewSpriteResource(spriteResource& sprRes, const std::string& sprFileName)
{
	sprRes.spr = new olc::Sprite(sprFileName, m_pack.get());
	sprRes.dec = new olc::Decal(sprRes.spr);
	sprRes.fileName = sprFileName;
	if (sprRes.ID == -1)
		sprRes.ID = resSprites.size() > 0 ? (resSprites.back().ID) + 1 : 0;

	resSprites.push_back(sprRes);


	if (sprRes.spr->pColData.size() > 0)
		return true;

	return false;
}

bool GameResourceManager::i_UnloadSpriteData(spriteResource& sprRes)
{
	if (!sprRes.spr->pColData.empty())
	{
		sprRes.spr->pColData.clear();
		sprRes.spr->pColData = std::vector<olc::Pixel>{};

		return true;
	}

	return false;
}

olc::Decal* GameResourceManager::RM_Sprite(const std::string& spriteFileName)
{
	strError = "";

	for (auto& s : resSprites)
		if (s.fileName == spriteFileName)
			return s.dec;

	spriteResource resCurrentSprite{};
	if (i_NewSpriteResource(resCurrentSprite, spriteFileName))
		return resCurrentSprite.dec;

	strError = "ERROR: RM_Sprite - Sprite data was empty...";
	return resCurrentSprite.dec;
}

olc::Decal* GameResourceManager::RM_Sprite(const int fileNameID, const std::string& spriteFileName)
{
	strError = "";

	if (spriteFileName != "")
	{
		for (auto& s : resSprites)
			if (s.fileName == spriteFileName)
			{
				strError = "ERROR: RM_Sprite - Tried to add existing sprite to new ID, existing ID used";
				return s.dec;
			}

		if (resSprites.size() > 0)
			strError = fileNameID <= resSprites.back().ID ? "ERROR: RM_Sprite - fileNameID duplicate or created out of order" : "";
	}

	for (auto& s : resSprites)
		if (s.ID == fileNameID)
			return s.dec;

	spriteResource resCurrentSprite{};
	resCurrentSprite.ID = fileNameID;

	if (i_NewSpriteResource(resCurrentSprite, spriteFileName))
		return resCurrentSprite.dec;

	strError = "ERROR: RM_Sprite - Sprite data was empty...";
	return resCurrentSprite.dec;
}

void GameResourceManager::InitializeResourcePack()
{
	m_pack = std::make_unique<olc::ResourcePack>();
}

void GameResourceManager::LoadResourcePack(const std::string sFile, const std::string sKey)
{
	if (m_pack.get())
	{
		m_pack->LoadPack(sFile, sKey);
	}
}
//#endif
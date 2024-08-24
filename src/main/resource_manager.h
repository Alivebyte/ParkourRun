#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
// Simple override to make use of packs
#include "olcPGEX_ResourceManager.h"

class GameResourceManager : public olcPGEX_ResourceManager
{
private:

	std::unique_ptr<olc::ResourcePack> m_pack;

	struct spriteResource
	{
		olc::Sprite* spr = nullptr;
		olc::Decal* dec = nullptr;
		std::string fileName = "";
		int ID = -1;
	};

	std::vector<spriteResource> resSprites;
	bool i_NewSpriteResource(spriteResource& sprRes, const std::string& sprFileName);
	bool i_UnloadSpriteData(spriteResource& sprRes);
public:
	std::string strError = "";																// Should always be "", if not - you have an error (check the console log)

	olc::Decal* RM_Sprite(const std::string& spriteFileName);								// Use a File Name to create a new Sprite Resource and return its Decal, or if it exists - return that Decal Instance
	olc::Decal* RM_Sprite(const int fileNameID, const std::string& spriteFileName = "");	// Use an ID to create a new Sprite Resource and return its Decal, or if it exists - return that Decal Instance

	olc::ResourcePack* GetResourcePack() { return m_pack.get(); };
	void InitializeResourcePack();
	void LoadResourcePack(const std::string sFile, const std::string sKey);
	//void RM_FreeSpriteData(const std::string& spriteFileName); 								// Locate a Sprite Resource by File Name and delete its Sprite Data (Will invalidate existing Sprite References, use with caution)
	//void RM_FreeSpriteData(const int fileNameID);
};
#endif
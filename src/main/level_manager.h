#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include "olcPixelGameEngine.h"
#include "level.h"

class LevelManager
{
public:
	LevelManager();
	bool LoadLevelFile(std::string filename);
	bool SaveLevelFile(std::string filename);
	Level* GetLevel() { return &m_Level; };
private:
	Level m_Level;
};


#endif
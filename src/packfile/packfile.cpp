#include "packfile.h"
#include <iostream>

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		olc::ResourcePack* pack = new olc::ResourcePack;

		if (pack)
		{
			pack->AddFile("content/sprites/background_test.png");
			pack->AddFile("content/sprites/player.png");

			pack->SavePack(argv[1], argv[2]);

			std::cout << "Content succesfully packed!" << std::endl;
		}
	}

	return 0;
}
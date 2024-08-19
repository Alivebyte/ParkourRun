#include "packfile.h"
#include <iostream>
//#include <fstream>


int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		olc::ResourcePack* pack = new olc::ResourcePack;
		char strbuf[256];


		if (pack)
		{
			/*pack->AddFile("content/sprites/background_test.png");
			pack->AddFile("content/sprites/player.png");
			*/

			FILE* packlist = fopen(argv[2],"r");

			if (packlist == NULL)
			{
				std::cout << "Couldn't open list file!";
				return 1;
			}

			while (fgets(strbuf, sizeof(strbuf),packlist))
			{
				if (strbuf[strlen(strbuf) - 1] == '\n')
				{
					strbuf[strlen(strbuf) - 1] = '\0';
				}
				std::cout << "Packing file " << strbuf << std::endl;

				pack->AddFile(strbuf);
			}
			

			fclose(packlist);

			pack->SavePack(argv[1], argv[3]);

			std::cout << "Content succesfully packed!" << std::endl;
		}
	}
	else
	{
		std::cout << "Usage: packfile.exe <pack name> <pack list> <pack key>";
	}

	return 0;
}
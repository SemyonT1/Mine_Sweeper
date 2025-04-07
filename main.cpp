#include "GameGUI.h"

int main()
{
	soundPlay("music", "sounds/Music.flac", true);

	try
	{
		mainMenu();
	}

	catch (const tgui::Exception& e)
	{
		std::cout << "Failed to load widgets: " << e.what() << std::endl;
		return 1;
	}

	mainWindowDisplay(1024, 768, "Minesweeper");

	return 0;
}
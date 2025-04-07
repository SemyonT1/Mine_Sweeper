#include "GameState.h"

bool clickedOnMine(false);
bool won(false);


int grid[50][50];
char visible[50][50];




void initVisibleToHash(int height, int width)
{
	won = false;
	
	for (int i = 1; i <= height; i++)
		for (int j = 1; j <= width; j++)
			visible[i][j] = '#';
}

void randomiseMineCoordinates(std::set < std::pair<int, int> >& minesCoordinates,
	int numOfMines, int height, int width)
{
	minesCoordinates.clear();
	srand(time(NULL));
	while (minesCoordinates.size() < numOfMines)
	{
		int y = rand() % height + 1;
		int x = rand() % width + 1;
		minesCoordinates.insert(std::make_pair(y, x));
	}
}

void setValueForTile(int r, int c)
{
	int cnt = 0;
	for (int row = r - 1; row <= r + 1; row++)
	{
		for (int col = c - 1; col <= c + 1; col++)
		{
			if (grid[row][col] == Mines_Flag)
				cnt++;
		}
	}
	grid[r][c] = cnt;
}

void setValuesForGrid(int height, int width)
{
	for (int row = 1; row <= height; row++)
	{
		for (int col = 1; col <= width; col++)
		{
			if (grid[row][col] != Mines_Flag)
				setValueForTile(row, col);
		}
	}
}

void placeMines(std::set < std::pair<int, int> >& minesCoordinates)
{
	
	memset(grid, 0, sizeof(grid));
	for (std::set< std::pair<int, int> >::iterator it = minesCoordinates.begin(); it != minesCoordinates.end(); it++)
		grid[it->first][it->second] = Mines_Flag;
}

void showMinesIfLost(int height, int width)
{
	for (int i = 1; i <= height; i++)
		for (int j = 1; j <= width; j++)
			if (grid[i][j] == Mines_Flag && visible[i][j] == 'F')
				visible[i][j] = '%';
			else if (grid[i][j] == Mines_Flag)
				visible[i][j] = '+';
}

void printVisible(int height, int width)
{
	std::cout << "    ";
	for (int i = 1; i <= width; i++)
		std::cout << static_cast<char>(i + 64) << ' ';
	std::cout << std::endl << "    ";

	for (int i = 1; i <= height; i++)
	{
		std::cout << static_cast<char>(i + 64) << " | "; 		
		for (int j = 1; j <= width; j++)
		{
			std::cout << visible[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}

void openTiles(int y, int x, int height, int width)
{
	//DFS
	if (y > height || y == 0 || x > width || x == 0 || visible[y][x] != '#')
		return;

	if (grid[y][x] != 0)
	{
		visible[y][x] = (char)(grid[y][x] + '0');
		return;
	}

	visible[y][x] = (char)(grid[y][x] + '0');

	openTiles(y + 1, x, height, width); //Север
	openTiles(y - 1, x, height, width); //Юг
	openTiles(y, x + 1, height, width); //Восток
	openTiles(y, x - 1, height, width); //Запад

	openTiles(y - 1, x - 1, height, width); //Юго-запад
	openTiles(y + 1, x + 1, height, width);	//Северо-восток
	openTiles(y - 1, x + 1, height, width);	//Юго-восток
	openTiles(y + 1, x - 1, height, width);	//Северо-запад
}

void clickTile(int height, int width, int y, int x, char operation)
{

	y++, x++; 
	if (operation == 'O')
	{
		if (grid[y][x] == Mines_Flag && visible[y][x] != 'F')
		{
			showMinesIfLost(height, width);
			visible[y][x] = '@'; //взорванная мина
			clickedOnMine = true;
		}


		else
			openTiles(y, x, height, width);

		printVisible(height, width);
	}

	else if (operation == 'F')
	{
		if (visible[y][x] == '#' && checkFlags())
			visible[y][x] = 'F';

		else if (visible[y][x] == 'F')
			visible[y][x] = '?';

		else if (visible[y][x] == '?')
			visible[y][x] = '#';

		printVisible(height, width);
	}
	else if (operation == '?')
	{
		if (visible[y][x] == '#')
			visible[y][x] = '?';

		else if (visible[y][x] == '?')
			visible[y][x] = '#';

		printVisible(height, width);
	}
}

char endGame(int height, int width, int numOfMines)
{
	int cntr(0), flagCntr(0);
	bool emptyAreVisible(true);

	for (int i = 1; i <= height; i++)
		for (int j = 1; j <= width; j++)
		{
			if (visible[i][j] == 'F' && grid[i][j] == Mines_Flag)
				cntr++;
			if (visible[i][j] == 'F')
				flagCntr++;
		}

	for (int i = 1; i <= height; i++)
		for (int j = 1; j <= width; j++)
		{
			if ((grid[i][j] != Mines_Flag && visible[i][j] == '#') || (visible[i][j] == 'F' && grid[i][j] != Mines_Flag))
				emptyAreVisible = false;
		}


	
	bool nFlagsCheck = (cntr == numOfMines && flagCntr == numOfMines);
	if (nFlagsCheck || emptyAreVisible)
	{
		
		won = true;
		return 'W'; 
	}


	if (clickedOnMine)
	{
		return 'L'; 
	}

	return 'N'; 

}

void getVisible(char arr[50][50])
{
	for (int i = 0; i < 50; i++)
		for (int j = 0; j < 50; j++)
			arr[i][j] = visible[i][j];
}

bool getClickedONMine()
{
	return clickedOnMine;
}

bool getWin()
{
	return won;
}
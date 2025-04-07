#pragma once 
#include "GameGUI.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <cstring>
#include <iomanip>
#include <chrono> 

const int Mines_Flag = 9; 



void initVisibleToHash(int height, int width);
void randomiseMineCoordinates(std::set < std::pair<int, int> >& minesCoordinates,
	int numOfMines, int height, int width);
void setValueForTile(int r, int c);
void setValuesForGrid(int height, int width);
void placeMines(std::set < std::pair<int, int> >& minesCoordinates);
void showMinesIfLost(int height, int width);
void printVisible(int height, int width);
void openTiles(int y, int x, int height, int width);
void clickTile(int height, int width, int y, int x, char operation);
void getVisible(char arr[50][50]);
char endGame(int height, int width, int numOfMines);
bool getClickedONMine();
bool getWin();

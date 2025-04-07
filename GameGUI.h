#pragma once

#include <TGUI/TGUI.hpp>
#include<sstream>
#include<SFML/Audio.hpp>
#include"GameState.h"

void mainWindowDisplay(float W_width, float W_height, std::string W_name);
void mainMenu();
void levelMenu();
void levelSelect(int level);
void launchLevel(int width, int height, int numOfmines);
void disableWidgets();
void backButton();
void soundPlay(const std::string& type, const std::string& path, bool isLoop);
void store(int i, int j, int width, int height, char operation);
void finishedLevel(int width, int height, bool won);
void time_Display();
bool checkFlags();

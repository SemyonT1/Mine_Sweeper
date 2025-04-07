#include "GameGUI.h"




sf::RenderWindow* window;
tgui::Gui gui;
auto theme = tgui::Theme::create("art/Black.txt");
auto windowWidth = tgui::bindWidth(gui);     
auto windowHeight = tgui::bindHeight(gui);   
std::vector<tgui::Widget::Ptr> Menu_Widgets;



tgui::Label::Ptr time_;


sf::SoundBuffer buffer;
sf::Music music;
sf::Sound sound;
std::set < std::pair<int, int> > minesCoordinates;


tgui::Button::Ptr** button;


tgui::Texture bar, Hbar, Cbar;
tgui::Texture tile, Htile, Ctile;
tgui::Texture tilenums[9];
tgui::Texture mine, Emine, flaggedMine, flag, QMark;
tgui::Texture timer;
char my_visible[50][50];


bool launched_Level;   
int Gwidth = 8;   
int Gheight = 8;  
int GMinesnum = 10;
int flgcntr = 0;
int maxFlags = 0;
std::string gameplay_info;
bool iswon, finishedCurrentLevel = false;      



std::chrono::steady_clock::time_point clock_begin;
std::chrono::steady_clock::duration time_span;
long long second_cnt;



void mainWindowDisplay(float W_width, float W_height, std::string W_name)
{
	
	window = new sf::RenderWindow(sf::VideoMode(W_width, W_height), W_name, sf::Style::Titlebar | sf::Style::Close);

	gui.setWindow(*window); 
	//Main loop
	while (window->isOpen())
	{
		if (launched_Level && !finishedCurrentLevel)
		{
			if (maxFlags < 0) maxFlags = 0;
			if (flgcntr < 0) flgcntr = 0;
			auto clock_end = std::chrono::steady_clock::now();
			auto tMilliseconds = std::chrono::duration_cast <std::chrono::milliseconds> (clock_end - clock_begin).count(); //Counting milliseconds
			second_cnt = tMilliseconds / 1000;
			gameplay_info = std::to_string(flgcntr) + "/" + std::to_string(maxFlags);
			time_->setText("\n\n\n" + gameplay_info + "\n\n\n\n         " + std::to_string(second_cnt));
		}
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();
			if (launched_Level)
			{
				//обработка правой кнопки мыши
				for (int i = 0; i < Gheight; i++)
				{
					for (int j = 0; j < Gwidth; j++)
					{

						if (button[i][j]->mouseOnWidget(event.mouseButton.x, event.mouseButton.y) && button[i][j]->isEnabled())
						{
							if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
							{
								clickTile(Gheight, Gwidth, i, j, 'F');
								getVisible(my_visible);
								if (flgcntr >= 0)
								{
									if (my_visible[i + 1][j + 1] == 'F')
									{
										button[i][j]->getRenderer()->setNormalTexture(flag);
										button[i][j]->getRenderer()->setHoverTexture(flag);
										endGame(Gheight, Gwidth, GMinesnum);
										button[i][j]->disconnectAll();
										flgcntr--;
									}
									else if (my_visible[i + 1][j + 1] == '?')
									{
										button[i][j]->getRenderer()->setNormalTexture(QMark);
										button[i][j]->getRenderer()->setHoverTexture(QMark);
										button[i][j]->disconnectAll();
										flgcntr++;
									}
									else
									{
										button[i][j]->getRenderer()->setNormalTexture(tile);
										button[i][j]->getRenderer()->setHoverTexture(Htile);
										button[i][j]->connect("pressed", soundPlay, "sound", "sounds/click.flac", false);
										button[i][j]->connect("pressed", store, i, j, Gwidth, Gheight, 'O');
										button[i][j]->connect("pressed", endGame, Gheight, Gwidth, GMinesnum);
									}
								}
								
							}
						}
					}

				}
			}

			if (getWin() && !iswon)
			{
				finishedLevel(Gwidth, Gheight, true);
				soundPlay("sound", "sounds/win.flac", false);
				iswon = true;
			}

			gui.handleEvent(event); 
		}
		window->clear();
		gui.draw(); 
		window->display();
	}
}

void soundPlay(const std::string& type, const std::string& path, bool isLoop)
{
	
	if (type == "Sound" || type == "sound")
	{
		if (!buffer.loadFromFile(path))
		{
			std::cout << "ERROR Playing sound \n";
		}

		sound.setBuffer(buffer);
		sound.play();
		sound.setLoop(isLoop);
	}

	else if (type == "Music" || type == "music")
	{
		if (!music.openFromFile(path))
			std::cerr << "Error \n";
		music.play();
		music.setLoop(isLoop);
	}
}


void mainMenu()
{
	
	bar.load("art/activebar.png");
	Hbar.load("art/hbar.png");
	Cbar.load("art/closedbar.png");
	tile.load("art/closedtile.png");
	Htile.load("art/hoveredtile.png");
	Ctile.load("art/emptytile.png");

	for (int i = 0; i < 9; i++)
	{
		std::string temp = ".png";
		tilenums[i].load("art/" + std::to_string(i) + temp);
	}

	mine.load("art/mine.png");
	Emine.load("art/explodedmine.png");
	flaggedMine.load("art/xmine.png");
	flag.load("art/flag.png");
	QMark.load("art/qmark.png");
	timer.load("art/mainmenuitems/timer.png");

	//background
	tgui::Picture::Ptr Bpicture = tgui::Picture::create("art/BackGround.png");
	Bpicture->setSize(tgui::bindMax(1024, windowWidth), tgui::bindMax(768, windowHeight));
	gui.add(Bpicture);

	//menu Background
	tgui::Picture::Ptr M_picture = tgui::Picture::create("art/emptymenu.png");
	M_picture->setSize(windowWidth - 250, windowHeight - 150);
	M_picture->setPosition(windowWidth / 7.6, windowHeight / 7);
	M_picture->showWithEffect(tgui::ShowAnimationType::Fade, sf::milliseconds(800));
	Menu_Widgets.insert(Menu_Widgets.end(), M_picture);
	gui.add(M_picture);

	//logo
	tgui::Picture::Ptr logo = tgui::Picture::create("art/logo.png");
	logo->setSize(windowWidth / 3, windowHeight / 7);
	logo->setPosition(windowWidth / 3, 90);
	logo->showWithEffect(tgui::ShowAnimationType::Fade, sf::milliseconds(800));
	Menu_Widgets.insert(Menu_Widgets.end(), logo);
	gui.add(logo);

	//checkbox
	tgui::CheckBox::Ptr c = tgui::CheckBox::create();
	c->setSize(40, 40);
	c->setPosition(20, 20);
	c->getRenderer()->setForegroundColor("Yellow");
	c->setText("Music");
	c->setFont("art/arcadeclassic.TTF");
	if (music.getStatus() == music.Stopped)            c->uncheck();
	else if (music.getStatus() == music.Playing)         c->check();
	c->connect("checked", [&]() { music.play(); });
	c->connect("unchecked", [&]() { music.stop(); });
	Menu_Widgets.insert(Menu_Widgets.end(), c);
	gui.add(c);

	//buttons
	tgui::Button::Ptr M_buttons[2];
	tgui::Texture txts[2];
	tgui::Texture Htxts[2];
	std::string temp = ".png";

	float v = 530;

	for (int i = 0; i < 2; i++)
	{
		txts[i].load("art/mainmenuitems/m" + std::to_string(i) + temp);
		Htxts[i].load("art/mainmenuitems/Hm" + std::to_string(i) + temp);
		M_buttons[i] = tgui::Button::create();
		M_buttons[i]->getRenderer()->setNormalTexture(txts[i]);
		M_buttons[i]->getRenderer()->setHoverTexture(Htxts[i]);
		M_buttons[i]->setFont("art/arcadeclassic.TTF");
		M_buttons[i]->setSize(500, 100);
		M_buttons[i]->setPosition(windowWidth / 3.8, windowHeight - v);
		v = v - 120;
		M_buttons[i]->showWithEffect(tgui::ShowAnimationType::Fade, sf::milliseconds(800));
		Menu_Widgets.insert(Menu_Widgets.end(), M_buttons[i]);
		gui.add(M_buttons[i]);
		M_buttons[i]->connect("pressed", soundPlay, "sound", "sounds/click.flac", false);
		M_buttons[i]->connect("pressed", disableWidgets);
	}
	M_buttons[0]->connect("pressed", levelMenu);
	M_buttons[1]->connect("pressed", [&]() { window->close(); });


}

void levelMenu()
{

	//logo
	tgui::Picture::Ptr logo = tgui::Picture::create("art/logo.png");
	logo->setSize(windowWidth / 2, windowHeight / 6);
	logo->setPosition(windowWidth / 2, 0);
	Menu_Widgets.insert(Menu_Widgets.end(), logo);
	gui.add(logo);

	//difficulty buttons
	tgui::Button::Ptr button[4];
	//Start position for buttons
	float s = 760;

	int level = 0;
	std::string temp = ".png";
	tgui::Texture dif[4];
	tgui::Texture Hdif[4];

	for (int i = 0; i < 4; i++)
	{
		dif[i].load("art/mainmenuitems/d" + std::to_string(i) + temp);
		Hdif[i].load("art/mainmenuitems/Hd" + std::to_string(i) + temp);
		button[i] = tgui::Button::create();
		button[i]->getRenderer()->setNormalTexture(dif[i]);
		button[i]->getRenderer()->setHoverTexture(Hdif[i]);
		button[i]->setSize(windowWidth / 3, windowHeight / 6);
		button[i]->setPosition(windowWidth / 6, windowHeight - s);
		button[i]->showWithEffect(tgui::ShowAnimationType::SlideFromLeft, sf::milliseconds(800));
		gui.add(button[i]);
		s -= 150;
		Menu_Widgets.insert(Menu_Widgets.end(), button[i]);
		button[i]->connect("pressed", soundPlay, "sound", "sounds/click.flac", false);
		button[i]->connect("pressed", disableWidgets);
		button[i]->connect("pressed", levelSelect, level + 1);



		level++;

	}
	level = 0;

	
	backButton();
}

void levelSelect(int level)
{

	switch (level)
	{
	case 1:
		//Easy
		launchLevel(8, 8, 10);
		break;
	case 2:
		// Medium
		launchLevel(10, 10, 20);
		break;

	case 3:
		//Hard
		launchLevel(15, 15, 45);
		break;

	case 4:
		// Brutal
		launchLevel(20, 20, 80);
		break;

	default:
		break;
	}
}

void launchLevel(int width, int height, int numOfmines)
{
	maxFlags = numOfmines;

	initVisibleToHash(height, width);
	randomiseMineCoordinates(minesCoordinates, numOfmines, height, width);
	placeMines(minesCoordinates);
	setValuesForGrid(height, width);
	iswon = false, finishedCurrentLevel = false;
	flgcntr = maxFlags;
	clock_begin = std::chrono::steady_clock::now(); //start time

	float v = 0;
	float H = 0;
	button = new tgui::Button::Ptr * [height];

	for (int i = 0; i < height; i++)
	{
		button[i] = new tgui::Button::Ptr[width];
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			button[i][j] = tgui::Button::create();
			button[i][j]->getRenderer()->setNormalTexture(tile);
			button[i][j]->getRenderer()->setHoverTexture(Htile);
			button[i][j]->getRenderer()->setDownTexture(Ctile);
			button[i][j]->setSize((windowWidth - 200) / (width), (windowHeight) / (height));
			button[i][j]->setPosition(windowWidth - windowWidth + H, windowHeight - windowHeight + v);
			button[i][j]->connect("pressed", soundPlay, "sound", "sounds/click.flac", false);
			button[i][j]->connect("pressed", store, i, j, width, height, 'O');
			button[i][j]->connect("pressed", endGame, height, width, numOfmines);
			button[i][j]->showWithEffect(tgui::ShowAnimationType::Fade, sf::milliseconds(800));
			gui.add(button[i][j]);
			Menu_Widgets.insert(Menu_Widgets.end(), button[i][j]);
			H += 824 / width;
		}

		v += 768 / height;
		H = 0;

	}

	Gwidth = width;
	Gheight = height;
	GMinesnum = numOfmines;
	launched_Level = true;
	time_Display();
	backButton();
}


void disableWidgets()
{


	for (int i = 0; i < Menu_Widgets.size(); i++)
	{
		Menu_Widgets[i]->hide();
		gui.remove(Menu_Widgets[i]);
	}
	Menu_Widgets.clear();

}

void backButton()
{
	
	tgui::Button::Ptr back_Button = tgui::Button::create();
	back_Button->getRenderer()->setNormalTexture(bar);
	back_Button->getRenderer()->setHoverTexture(Hbar);
	back_Button->getRenderer()->setDownTexture(Cbar);
	back_Button->setSize(windowWidth / 6, windowHeight / 12);
	back_Button->setText("MainMenu");
	back_Button->getRenderer()->setTextColor("White");
	back_Button->setFont("art/arcadeclassic.TTF");
	back_Button->setPosition(windowWidth - windowWidth / 6, windowHeight - windowHeight / 12);
	Menu_Widgets.insert(Menu_Widgets.end(), back_Button);
	back_Button->connect("pressed", soundPlay, "sound", "sounds/click.flac", false);
	back_Button->connect("pressed", disableWidgets);
	back_Button->connect("pressed", mainMenu);
	back_Button->showWithEffect(tgui::ShowAnimationType::Fade, sf::milliseconds(800));
	gui.add(back_Button);
}


void store(int i, int j, int width, int height, char operation)
{

	clickTile(height, width, i, j, operation);
	getVisible(my_visible);
	for (int k = 0; k < height; k++)
	{
		for (int l = 0; l < width; l++)
		{
			if (my_visible[k + 1][l + 1] != '#')
			{
				if (my_visible[k + 1][l + 1] != '@' && my_visible[k + 1][l + 1] != '+' && my_visible[k + 1][l + 1] != 'F' && my_visible[k + 1][l + 1] != '%')
				{
					button[k][l]->getRenderer()->setNormalTexture(tilenums[my_visible[k + 1][l + 1] - '0']);
					button[k][l]->disable();
				}
				else if (my_visible[k + 1][l + 1] == '@')
				{
					button[k][l]->getRenderer()->setNormalTexture(Emine);
					soundPlay("sound", "sounds/explode.flac", false);
					finishedLevel(width, height, false);
				}
				else if (my_visible[k + 1][l + 1] == '+')
				{
					button[k][l]->getRenderer()->setNormalTexture(mine);
					button[k][l]->disable();
				}
				else if (my_visible[k + 1][l + 1] == '%')
				{
					button[k][l]->getRenderer()->setNormalTexture(flaggedMine);
					button[k][l]->disable();
				}

			}
			else if (my_visible[k + 1][l + 1] == '#')
			{
				button[k][l]->getRenderer()->setNormalTexture(tile);
				button[k][l]->getRenderer()->setHoverTexture(Htile);

			}

		}
	}
}

void finishedLevel(int width, int height, bool won)
{
	

	tgui::Label::Ptr txt;
	tgui::Button::Ptr again;


	txt = tgui::Label::create();
	txt->setSize(400, 400);
	txt->setTextSize(72);
	txt->setPosition(850, 200);
	txt->setFont("art/arcadeclassic.TTF");
	txt->setTextColor("White");


	if (won)
	{
		txt->setText("You \n \n Win!");
	}
	else
	{
		txt->setText("You \n \n Lose!");
	}

	Menu_Widgets.insert(Menu_Widgets.end(), txt);
	gui.add(txt);

	
	again = tgui::Button::create();
	again->getRenderer()->setNormalTexture(bar);
	again->getRenderer()->setHoverTexture(Hbar);
	again->getRenderer()->setDownTexture(Cbar);
	again->setSize(windowWidth / 6, windowHeight / 12);
	again->setText("PlayAgain!");
	again->getRenderer()->setTextColor("White");
	again->setFont("art/arcadeclassic.TTF");
	again->setPosition(windowWidth - windowWidth / 6, windowHeight - windowHeight / 4);
	Menu_Widgets.insert(Menu_Widgets.end(), again);
	again->connect("pressed", soundPlay, "sound", "sounds/click.flac", false);
	again->connect("pressed", [&]() { music.play(); });
	again->connect("pressed", disableWidgets);
	again->connect("pressed", levelMenu);
	gui.add(again);

	music.pause();
	finishedCurrentLevel = true;

	
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			button[i][j]->disable();
		}
	}
}







void time_Display()
{
	

	time_ = tgui::Label::create();
	time_->setSize(400, 400);
	time_->setTextSize(15);
	time_->setPosition(835, 10);
	time_->setTextStyle(sf::Text::Style::Bold);
	time_->setTextColor("White");
	Menu_Widgets.insert(Menu_Widgets.end(), time_);
	gui.add(time_);

	tgui::Picture::Ptr flg = tgui::Picture::create(flag);
	flg->setSize(40, 40);
	flg->setPosition(880, 50);
	Menu_Widgets.insert(Menu_Widgets.end(), flg);
	gui.add(flg);

	tgui::Picture::Ptr time = tgui::Picture::create(timer);
	time->setSize(50, 25);
	time->setPosition(877, 123);
	Menu_Widgets.insert(Menu_Widgets.end(), time);
	gui.add(time);
}
bool checkFlags()
{
	return flgcntr > 0;
}
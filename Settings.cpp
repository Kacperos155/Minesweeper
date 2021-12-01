#include "Minesweeper.hpp"
using namespace sf;

Difficulty_Level::Difficulty_Level()
	:width(0), height(0), bombs(0), auto_first_move(true) {}

Difficulty_Level::Difficulty_Level(int _width, int _height, int _bombs, bool afm)
	:width(_width), height(_height), bombs(_bombs)
	, auto_first_move(afm) {}

Difficulty_Level & Difficulty_Level::operator*=(const Difficulty_Level X)
{
	if (X == *this)
		return *this;
	width = X.width;
	height = X.height;
	bombs = X.bombs;
	return *this;
}

bool Difficulty_Level::operator==(const Difficulty_Level &X) const
{
	if (width == X.width && height == X.height && bombs == X.bombs)
		return true;
	return false;
}

Settings::Settings(unsigned int x, unsigned int y, Config &C)
	: INI(&C), background(static_cast<float>(x))
	, Easy(10, 10, 14), Medium(20, 20, 16), Hard(40, 20, 21)
	, Stats_Saver("stats.bin")
{
	setView(static_cast<float>(x), static_cast<float>(y));
	size = INI->getSize();
	Change = Difficulty_Level(INI->getDifficult());
	Custom = Difficulty_Level(15, 15, 15);

	//Statystyki
	if (!Stats_Saver.Is_Good())
	{
		std::ofstream("stats.bin", std::ios::trunc);
		stats = vector<char>(12, 9);
		Stats_Saver.ImportBuffer(stats, 12, "stats.txt");
		Stats_Saver.SaveToBinary();
	}
	Stats_Saver.LoadFromBinary();
	stats = Stats_Saver.ExportBuffer();

	//Interface
	createText("10x10 14%", 260, 55, 200, 20, 18);
	createText("Best Time XX:XX", 260, 75, 200, 20, 16);
	createText("20x20 16%", 260, 110, 200, 20, 18);
	createText("Best Time XX:XX", 260, 130, 200, 20, 16);
	createText("40x20 21%", 260, 165, 200, 20, 18);
	createText("Best Time XX:XX", 260, 185, 200, 20, 16);

	createButton("Easy", 50, 50, 230, 60);			//0
	createButton("Medium", 50, 115, 230, 50);		//1
	createButton("Hard", 50, 170, 230, 50);			//2
	createLine(55, 225, 370, 0);

	createButton("Save", 40, 400, 280, 40);			//3
	createButton("Cancel", 320, 400, 120, 40);		//4

	createText("Field Size", 60, 230, 130, 22, 22);
	createButton("20", 60, 260, 50, 40);				//5
	createButton("40", 140, 260, 50, 40);			//6
	
	createText("Auto First Move", 270, 230, 130, 22, 22);
	createButton("On", 270, 260, 50, 40);			//7
	createButton("Off", 350, 260, 50, 40);			//8
	createLine(55, 305, 370, 0);

	createButton("Custom", 50, 310, 230, 50);		//9
	createText("W", 285, 310, 0, 0, 22);
	createButton("<", 320, 310, 25, 28);				//10
	createText("15", 345, 310, 55, 0, 22);	//N9
	createButton(">", 400, 310, 25, 28);				//11

	createText("H", 285, 340, 0, 0, 22);
	createButton("<", 320, 340, 25, 28);				//12
	createText("15", 345, 340, 55, 0, 22);	//N11
	createButton(">", 400, 340, 25, 28);				//13

	createText("B%", 285, 370, 0, 0, 22);
	createButton("<", 320, 370, 25, 28);				//14
	createText("15", 345, 370, 55, 0, 22);	//N13
	createButton(">", 400, 370, 25, 28);				//15

	//createButton("Center Window", 50, 362, 230, 36);		//16
	createText("Saper v0.9", 50, 443, 0, 0, 18);
	createText("by Kacper Zielinski", 280, 443, 0, 0, 18);
	
	//Przelaczanie
	if (Change == Easy)
		switchSelection(1, 0);
	else if (Change == Medium)
		switchSelection(1, 1);
	else if (Change == Hard)
		switchSelection(1, 2);
	else
		switchSelection(1, 9);
	if (size == 20)
		switchSelection(1, 5);
	else if (size == 40)
		switchSelection(1, 6);
	if (Change.auto_first_move == true)
		switchSelection(1, 7);
	else
		switchSelection(1, 8);

	if(Stats_Saver.Is_Good())
		fillStats();
}

void Settings::Draw(sf::RenderWindow &Window)
{
	Window.setSize(window_size);
	Window.setView(_View);
	Window.draw(background);
	for (auto&& Line : Lines)
		Window.draw(Line);
	for (auto&& Button : Buttons)
		Button.Draw(Window);
	for (auto&& Text : Texts)
		Window.draw(Text);
}

void Settings::Click(sf::RenderWindow &Window)
{
	Vector2f Coordinates = Window.mapPixelToCoords(Mouse::getPosition(Window), _View);
	if(Mouse::isButtonPressed(Mouse::Button::Left))
		for (size_t i = 0; i < Buttons.size(); ++i)
		{
			if (Buttons[i].isClicked(Coordinates))
			{
				switch (i)
				{
				case 0:
					Change *= Easy;
					switchSelection(1, 0);
					switchSelection(0, 1);
					switchSelection(0, 2);
					switchSelection(0, 9);
					break;
				case 1:
					Change *= Medium;
					switchSelection(0, 0);
					switchSelection(1, 1);
					switchSelection(0, 2);
					switchSelection(0, 9);
					break;
				case 2:
					Change *= Hard;
					switchSelection(0, 0);
					switchSelection(0, 1);
					switchSelection(1, 2);
					switchSelection(0, 9);
					break;
				case 3:
					INI->newFile(Change, size);
					_Communication.input(Communication::Statement::CLOSE);
					break;
				case 4:
					Change = Difficulty_Level(INI->getDifficult());
					_Communication.input(Communication::Statement::CLOSE);
					break;
				case 5:
					size = 20;
					switchSelection(1, 5);
					switchSelection(0, 6);
					break;
				case 6:
					size = 40;
					switchSelection(0, 5);
					switchSelection(1, 6);
					break;
				case 7:
					Change.auto_first_move = true;
					switchSelection(1, 7);
					switchSelection(0, 8);
					break;
				case 8:
					Change.auto_first_move = false;
					switchSelection(0, 7);
					switchSelection(1, 8);
					break;
				case 9:
					Change *= Custom;
					switchSelection(0, 0);
					switchSelection(0, 1);
					switchSelection(0, 2);
					switchSelection(1, 9);
					break;
				case 10:
					if (Custom.width > 10)
						--Custom.width;
					Texts[9].setString(std::to_string(Custom.width));
					switchSelection(0, 9);
					break;
				case 11:
					if (Custom.width < 50)
						++Custom.width;
					Texts[9].setString(std::to_string(Custom.width));
					switchSelection(0, 9);
					break;
				case 12:
					if (Custom.height > 10)
						--Custom.height;
					Texts[11].setString(std::to_string(Custom.height));
					switchSelection(0, 9);
					break;
				case 13:
					if (Custom.height < 25)
						++Custom.height;
					Texts[11].setString(std::to_string(Custom.height));
					switchSelection(0, 9);
					break;
				case 14:
					if (Custom.bombs > 1)
						--Custom.bombs;
					Texts[13].setString(std::to_string(Custom.bombs));
					switchSelection(0, 9);
					break;
				case 15:
					if (Custom.bombs < 100)
						++Custom.bombs;
					Texts[13].setString(std::to_string(Custom.bombs));
					switchSelection(0, 9);
					break;
				case 16:
					_Communication.input(Communication::Statement::CENTER_WINDOW);
					break;
				default:
					break;
				}
			}
		}
}

void Settings::saveTime(std::size_t s)
{
	std::size_t pointer = -1;
	if (INI->getDifficult() == Easy)
		pointer = 0;
	else if (INI->getDifficult() == Medium)
		pointer = 4;
	else if (INI->getDifficult() == Hard)
		pointer = 8;

	//Comparsion
	std::string temp = "9999";
	if(pointer >= 0 && pointer <= 8)
	{
		for (std::size_t i = 0; i < 4; ++i)
		{
			temp[i] = stats[i + pointer] + '0';
		}
	}
	std::size_t old_time = 6000;
	if (temp.length() == 4)
	{
		old_time = std::stoi(temp.substr(0, 2)) * 60;
		old_time += std::stoi(temp.substr(2, 2));
	}

	if(old_time > s)
	{
		//Saving
		if (pointer >= 0 && pointer <= 8)
		{
			std::size_t minutes=0, seconds=0;
			minutes = _Communication.getTime() / 60;
			seconds = _Communication.getTime() % 60;
			if (std::to_string(minutes).length() == 1)
			{
				stats[0 + pointer] = '0' - '0';
				stats[1 + pointer] = std::to_string(minutes)[0] - '0';
			}
			else
			{
				stats[0 + pointer] = std::to_string(minutes)[0] - '0';
				stats[1 + pointer] = std::to_string(minutes)[1] - '0';
			}
			if (std::to_string(seconds).length() == 1)
			{
				stats[2 + pointer] = '0' - '0';
				stats[3 + pointer] = std::to_string(seconds)[0] - '0';
			}
			else
			{
				stats[2 + pointer] = std::to_string(seconds)[0] - '0';
				stats[3 + pointer] = std::to_string(seconds)[1] - '0';
			}
		}
		fillStats();
	}
}

Settings::~Settings()
{
	std::ofstream("stats.bin", std::ios::trunc);
	Stats_Saver.ImportBuffer(stats, 12, "stats.txt");
	Stats_Saver.SaveToBinary();
}

void Settings::switchSelection(bool on, std::size_t pos)
{
	if(pos<Buttons.size())
	{
		if (on)
			Buttons[pos].setColor(Color(220,222,220));
		else
			Buttons[pos].setColor(Color::White);
	}
}

void Settings::createLine(float x, float y, float w, float h)
{
	VertexArray Line(sf::Lines, 2);
	Button_Shape TC;
	Line[0].color = TC.Dark_Grey;
	Line[1].color = TC.Dark_Grey;
	Line[0].position = Vector2f(x, y);
	Line[1].position = Vector2f(x + w, y + h);
	Lines.push_back(Line);
}

void Settings::createButton(string text, float x, float y, float w, float h)
{
	Button_Text _Button(text, x, y, w, h);
	Buttons.push_back(_Button);
}

void Settings::createText(string text, float x, float y, float w, float h, unsigned int size)
{
	Text _Text;
	_Text.setFont(Arial);
	_Text.setFillColor(Color(0, 0, 0));
	_Text.setCharacterSize(size);
	_Text.setString(text);
	Vector2f _Text_Center;
	_Text_Center.x = static_cast<float>(static_cast<int>(_Text.getLocalBounds().width / 2));
	_Text_Center.y = static_cast<float>(static_cast<int>(_Text.getLocalBounds().height / 2));
	_Text.setOrigin(_Text_Center);
	if (w == 0)
		_Text_Center.x = static_cast<int>(_Text.getLocalBounds().width / 2) + x;
	else 
		_Text_Center.x = static_cast<int>(w / 2) + x;
	if (h == 0)
		_Text_Center.y = static_cast<int>(_Text.getLocalBounds().height / 2) + y;
	else
		_Text_Center.y = static_cast<int>(h / 2) + y;
	_Text.setPosition(_Text_Center);
	Texts.push_back(_Text);
}

void Settings::fillStats()
{
	std::string best_time = "Best Time XX:XX";
	std::string temp = best_time;
	for (int i = 0; i < 3; ++i)
	{
		std::string temp = best_time;
		temp[10] = stats[0 + (i * 4)] + '0';
		temp[11] = stats[1 + (i * 4)] + '0';
		temp[13] = stats[2 + (i * 4)] + '0';
		temp[14] = stats[3 + (i * 4)] + '0';
		if (Texts.size() >= 5)
			Texts[1 + (i * 2)].setString(temp);
	}
}

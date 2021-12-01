#include "Minesweeper.hpp"
using namespace sf;

GUI::GUI(float _x, float _y, float _s)
	: x(_x), y(_y), s(_s)
	, Face_Button(Tx_Face_Button, (_x - _s) / 2.0F, (_s * 1.5F - _s) / 2.0F, 40, 40)
{
	setView(static_cast<float>(x), static_cast<float>(y));
	y = s * 1.5F;
	float interspace = y / 12.0F;
	game_over = false;
	show_gui_hint = false;
	seconds = 0;
	Vector2f Skala;
	Skala.x = Skala.y = y / 60.0F;
	//Tlo
	background.setFillColor(Color(229, 229, 229));
	background.setOutlineColor(Color(190, 190, 190));
	background.setOutlineThickness(interspace);
	background.setPosition(interspace, interspace);
	background.setSize(Vector2f(x - interspace * 2, y - interspace * 2));
	//Buzka
	Face_Button.backlightTexture(40, 40);
	Face_Button.scale(Skala);
	//Licznik Bomb
	for (int i = 0; i < 3; ++i)
	{
		Bombs_Counter[i].setScale(Skala);
		Bombs_Counter[i].setTexture(Tx_Digit);
		Bombs_Counter[i].setTextureRect(IntRect(0, 0, 30, 50));
		Bombs_Counter[i].setPosition(y / 2 * i + interspace, interspace);
	}
	//Zegar
	float clock_position=x-interspace;
	//for (int i = 0; i < 5; ++i)
	for (size_t i = 0; i < 5; ++i)
	{
		Time_Counter[i].setScale(Skala);
		Time_Counter[i].setTexture(Tx_Digit);
		if (i == 2)
		{
			clock_position -= 8 * Skala.x;
			Time_Counter[i].setTextureRect(numberTexture(12));
		}
		else
		{
			clock_position -= 30 * Skala.x;
			Time_Counter[i].setTextureRect(numberTexture(0));
		}
		Time_Counter[i].setPosition(clock_position, interspace);
	}
	Time_Counter[4].setTextureRect(numberTexture(10));
	//Podpowiedzi
	for (size_t i = 0; i < 2; ++i)
	{
		Gui_Hints[i].setScale(Skala);
		Gui_Hints[i].setTexture(Tx_Face_Button);
		Gui_Hints[i].setTextureRect(IntRect(80, i * 40, 20, 40));
		Gui_Hints[i].setPosition(Face_Button().getPosition().x-(y/3)+(y*i), Face_Button().getPosition().y);
	}


}

void GUI::Draw(sf::RenderWindow &Window, int bombs_number)
{
	countBombs(bombs_number);
	Draw(Window);
}

void GUI::Draw(sf::RenderWindow &Window)
{
	Window.setView(_View);
	Window.draw(background);
	if (Game_Clock.getElapsedTime().asSeconds() > 1.0F)
		countTime();
	Face_Button.Draw(Window);
	for (auto&& _Licznik : Bombs_Counter)
		Window.draw(_Licznik);
	for (auto&& _Licznik_Czasu : Time_Counter)
		Window.draw(_Licznik_Czasu);

	if (Face_Button.isClicked(Mouse::getPosition(Window)))
		for (auto&& _Podpowiedzi : Gui_Hints)
			Window.draw(_Podpowiedzi);
}

void GUI::setView(float width, float height)
{
	Window_Mode::setView(width, height);
	_View.setSize(sf::Vector2f(x, y));
	_View.setCenter(x / 2, y / 2);
	_View.setViewport(FloatRect(0, 0, 1, static_cast<float>(y / window_size.y)));
}

void GUI::countBombs(int bombs_number)
{
	std::string number = "   ";
	if (bombs_number < 0)
	{
		number = "-  ";
		bombs_number *= -1;
		if (bombs_number > 99)
			bombs_number = 99;
	}
	if (bombs_number > 999)
		bombs_number = 999;
	std::string temp = std::to_string(bombs_number);
	int difference = 3 - temp.length();
	for (int i = difference; i < 3; ++i)
		number[i] = temp[i-difference];
	if (game_over)
		number = "   ";
	for (int i = 0; i < 3; ++i)
	{
		if (number[i] == ' ')
			Bombs_Counter[i].setTextureRect(numberTexture(10));
		else if (number[i] == '-')
			Bombs_Counter[i].setTextureRect(numberTexture(11));
		else
			Bombs_Counter[i].setTextureRect(numberTexture(number[i]-48));
	}
}

void GUI::Click(sf::RenderWindow &Window)
{
	Vector2f Coordinates = Window.mapPixelToCoords(Mouse::getPosition(Window), _View);
	if (Face_Button.isClicked(Coordinates))
	{
		if (Mouse::isButtonPressed(Mouse::Left))
			_Communication.input(Communication::Statement::NEW_GAME);
		if (Mouse::isButtonPressed(Mouse::Right))
			_Communication.input(Communication::Statement::SETTINGS);
	}
}

IntRect GUI::numberTexture(short number)
{
	IntRect texture_square;
	texture_square.width = 30;
	texture_square.height = 50;
	if (number >= 0 && number < 10)
	{
		texture_square.top = 0;
		texture_square.left = (number + 1) * 30;
		if (number >= 5)
		{
			texture_square.top = 50;
			texture_square.left = (number - 5) * 30;
		}
	}
	else if (number >= 10) // 10 = "", 11 = "-", (12,13) = ":"
	{
		if (number % 2 == 0)
		{
			texture_square.top = 0;
			texture_square.left = 0;
		}
		else
		{
			texture_square.top = 50;
			texture_square.left = 150;
		}
		if (number >= 12)
		{
			texture_square.width = 8;
			texture_square.left = 180;
		}
	}
	return texture_square;
}

void GUI::countTime()
{
	if (!game_over)
	{
		seconds = _Communication.getTime();
		seconds += static_cast<int>(Game_Clock.restart().asSeconds());
		_Communication.sendTime(seconds);
	}
	//Miganie dwukropka
	if (seconds % 2 == 0)
		Time_Counter[2].setTextureRect(numberTexture(12));
	else
		Time_Counter[2].setTextureRect(numberTexture(13));
	std::string str_seconds = std::to_string(seconds % 60);
	std::string str_minutes = std::to_string(seconds / 60);
	//Sekundy
	if (str_seconds.length() == 1)
	{
		str_seconds += (char)0;
		str_seconds[1] = str_seconds[0];
		str_seconds[0] = '0';
	}
	//Minuty
	if (str_minutes.length() == 1)
	{
		str_minutes += (char)0;
		str_minutes[1] = str_minutes[0];
		str_minutes[0] = '0' - 1;
	}
	//Przekroczenie limitu
	if (seconds >= 6000)
	{
		str_minutes[0] = str_seconds[0] = '9' + 1;
		str_minutes[1] = str_seconds[1] = '9' + 1;
	}
	Time_Counter[0].setTextureRect(numberTexture(str_seconds[1] - 48));
	Time_Counter[1].setTextureRect(numberTexture(str_seconds[0] - 48));
	Time_Counter[3].setTextureRect(numberTexture(str_minutes[1] - 48));
	Time_Counter[4].setTextureRect(numberTexture(str_minutes[0] - 48));
}

void GUI::gameOver(bool victory)
{
	game_over = true;
	if (victory)
		Face_Button.setTexture(40, 0);
	if (!victory)
		Face_Button.setTexture(0, 40);
	_Communication.sendTime(seconds);
}

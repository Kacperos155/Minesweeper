#include "Minesweeper.hpp"
using namespace sf;

Minesweeper_Map::Minesweeper_Map()
	:_GUI(0, 0, 0), _width(0), _height(0), _size(0) {}

Minesweeper_Map::Minesweeper_Map(Difficulty_Level D, unsigned int size, unsigned int seed)
	: _GUI(static_cast<float>(D.width*size), static_cast<float>(size*(static_cast<float>(D.height) + 1.5F)), static_cast<float>(size))
	, _width(D.width), _height(D.height), _size(size), map_discovered(false)
{
	setView(static_cast<float>(_width*_size), static_cast<float>(_size *(_height + 1.5F)));
	bombs_number = static_cast<int>(_width * _height * static_cast<float>(D.bombs) / 100.0F);
	//Tworzenie pol
	for (size_t y = 0; y<_height; ++y)
	{
		for (size_t x = 0; x<_width; ++x)
		{
			Field p(x*_size, y*_size, _size);
			Fields.push_back(p);
		}
	}
	for (size_t i = 0; i < Fields.size(); ++i)
		findNeighboringFields(i);
	//Zaminowanie
	if (seed == 0)
		game_seed = randomNumberGenerator(0, INT_MAX, 0);
	else
		game_seed = seed;
	setBombsOnMap(bombs_number);
	Game_Clock.restart();
}

void Minesweeper_Map::Draw(sf::RenderWindow &Window)
{
	_GUI.Draw(Window, bombs_number);
	Window.setSize(window_size);
	Window.setView(_View);
	for (auto&& _Field : Fields)
		Window.draw(_Field);
}

void Minesweeper_Map::Click(sf::RenderWindow &Window)
{
	_GUI.Click(Window);
	Vector2f Coordinates = Window.mapPixelToCoords(Mouse::getPosition(Window), _View);
	if (Coordinates.y > 0)
	{
		unsigned int field_id = static_cast<int>(Coordinates.y / _size)*_width;
		field_id += static_cast<int>(Coordinates.x / _size);
		if (Mouse::isButtonPressed(Mouse::Right) && !map_discovered)
			bombs_number += Fields[field_id].setFlag();
		else if (Mouse::isButtonPressed(Mouse::Left))
			revealField(field_id);
	}
}

std::string Minesweeper_Map::getSeed() const
{
	return std::to_string(game_seed);
}

void Minesweeper_Map::setView(float width, float height)
{
	float GUI_Height = _size * 1.5F;
	Window_Mode::setView(width, height);
	height = static_cast<float>(_height * _size);
	_View.setSize(sf::Vector2f(width, height));
	_View.setCenter(width / 2, height / 2);
	_View.setViewport(FloatRect(0, GUI_Height / window_size.y, 1, (window_size.y - GUI_Height) / window_size.y));
}

void Minesweeper_Map::setBombsOnMap(unsigned int bombs_number)
{
	int setted_bombs = 0, i = 0;
	while (setted_bombs != bombs_number)
	{
		auto j = randomNumberGenerator(0, Fields.size() - 1, game_seed + i);
		if (Fields[j].setBomb())
			++setted_bombs;
		++i;
	}
	//Uzupelnienie wartosci pol
	for (auto&& _Pole : Fields)
	{
		auto neighbors = _Pole.getNeighborhood();
		int bombs = 0;
		for (auto&& j : neighbors)
			bombs += Fields[j].isBomb();
		_Pole.setNumberOfBombs(bombs);
	}
}

void Minesweeper_Map::gameOver(bool victory)
{
	map_discovered = true;
	_GUI.gameOver(victory);
	for (auto&& _Field : Fields)
		_Field.reaveal(map_discovered);
	if (victory)
		_Communication.input(Communication::Statement::VICTORY);
}

void Minesweeper_Map::WaveOfReveals(std::queue<int> &reveals_queue)
{
	int id = reveals_queue.front();
	reveals_queue.pop();
	auto neighbors = Fields[id].getNeighborhood();
	for (auto&& i : neighbors)
	{
		if (Fields[i].isEmpty(false))
			reveals_queue.push(i);
		Fields[i].reaveal(map_discovered);
	}	
}

void Minesweeper_Map::findNeighboringFields(unsigned int field_id)
{
	std::vector<int> neighbors;
	if (field_id >= _width)
	{
		if (field_id%_width != 0)
			neighbors.push_back(field_id - _width - 1);
		neighbors.push_back(field_id - _width);
		if (field_id%_width != _width - 1)
			neighbors.push_back(field_id - _width + 1);
	}
	if (field_id%_width != 0)
		neighbors.push_back(field_id - 1);
	if (field_id%_width != _width - 1)
		neighbors.push_back(field_id + 1);
	if (field_id < _width*_height - _width)
	{
		if (field_id%_width != 0)
			neighbors.push_back(field_id + _width - 1);
		neighbors.push_back(field_id + _width);
		if (field_id%_width != _width - 1)
			neighbors.push_back(field_id + _width + 1);
	}
	Fields[field_id].setNeighborhood(neighbors);
}

void Minesweeper_Map::revealField(unsigned int field_id)
{
	if (!Fields[field_id].reaveal(map_discovered) && !map_discovered)
		gameOver(false);
	//Fala odkryc
	if (Fields[field_id].isEmpty(true))
	{
		std::queue<int> reveals_queue;
		reveals_queue.push(field_id);
		while (reveals_queue.size())
			WaveOfReveals(reveals_queue);
	}
	// Sprawdzanie wygranej
	else if (Fields[field_id].reaveal(map_discovered) && !map_discovered)
	{
		bool isWon = true;
		for (auto&& _Field : Fields)
		{
			if (!_Field.isSolved())
			{
				isWon = false;
				break;
			}
		}
		if (isWon)
			gameOver(true);
	}
}

void Minesweeper_Map::firstMove()
{
	for (size_t i = 0; i < Fields.size(); ++i)
	{
		if (Fields[i].isEmpty(false))
		{
			revealField(i);
			break;
		}
	}
}

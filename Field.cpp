#include "Minesweeper.hpp"
using namespace sf;

Field::Field(int ax, int ay, int ar)
	: x(static_cast<float>(ax))
	, y(static_cast<float>(ay))
	, s(static_cast<float>(ar))
{
	float skala = s / 40;
	Field_Condition = _Field_Condition::Covered;
	Square.setPosition(x, y);
	Square.setSize(Vector2f(s, s));
	Square.setTexture(Tx_Field);
	Square.setTextureRect(fieldTexture());
	number_of_bombs.setFont(Arial);
	number_of_bombs.setPosition(x + s / 4, y);
	number_of_bombs.setCharacterSize(static_cast<unsigned int>(15 * s / 20));
	number_of_bombs.setFillColor(Color(0, 0, 0));
}

void Field::draw(sf::RenderTarget &Window, sf::RenderStates states) const
{
	Window.draw(Square);
	if (Field_Condition == _Field_Condition::Revealed)
		Window.draw(number_of_bombs);
}

short Field::setFlag()
{
	short change = 0;
	switch (Field_Condition)
	{
	case _Field_Condition::Covered:
		Field_Condition = _Field_Condition::Flag;
		break;
	case _Field_Condition::Bomb:
		Field_Condition = _Field_Condition::Marked_Bomb;
		break;
	case _Field_Condition::Flag:
		Field_Condition = _Field_Condition::Covered;
		break;
	case _Field_Condition::Marked_Bomb:
		Field_Condition = _Field_Condition::Bomb;
		break;
	default:
		break;
	}
	if (Field_Condition == _Field_Condition::Flag || Field_Condition == _Field_Condition::Marked_Bomb)
		change = -1;
	else if (Field_Condition == _Field_Condition::Covered || Field_Condition == _Field_Condition::Bomb)
		change = 1;
	Square.setTextureRect(fieldTexture());
	return change;
}

bool Field::reaveal(bool map_discovered)
{
	if (map_discovered)
	{
		if (Field_Condition == _Field_Condition::Bomb || Field_Condition == _Field_Condition::Marked_Bomb)
			Field_Condition = _Field_Condition::Revealed_Bomb;
		if (Field_Condition == _Field_Condition::Covered)
			Field_Condition = _Field_Condition::Revealed;
		Square.setTextureRect(fieldTexture());
	}
	else if (Field_Condition == _Field_Condition::Bomb)
		return false;
	else if (Field_Condition == _Field_Condition::Covered)
	{
		Field_Condition = _Field_Condition::Revealed;
		Square.setTextureRect(fieldTexture());
	}
	return true;
}

bool Field::setBomb()
{
	if (Field_Condition != _Field_Condition::Bomb)
	{
		Field_Condition = _Field_Condition::Bomb;
		return true;
	}
	return false;
}

bool Field::isBomb() const
{
	if (Field_Condition == _Field_Condition::Bomb || Field_Condition == _Field_Condition::Marked_Bomb)
		return true;
	return false;
}

bool Field::isSolved() const
{
	if (Field_Condition == _Field_Condition::Flag || Field_Condition == _Field_Condition::Covered)
		return false;
	return true;
}

bool Field::isEmpty(bool revealed) const
{
	if (number_of_bombs.getString().getSize() == 0)
	{
		if (revealed && Field_Condition == _Field_Condition::Revealed)
			return true;
		else if (Field_Condition == _Field_Condition::Flag || Field_Condition == _Field_Condition::Marked_Bomb)
			return false;
		else if (!revealed && Field_Condition != _Field_Condition::Revealed && Field_Condition != _Field_Condition::Bomb)
			return true;
	}
	return false;
}

void Field::setNumberOfBombs(int l)
{
	std::string text = "";
	if (l != 0 && Field_Condition != _Field_Condition::Bomb)
		text = std::to_string(l);
	number_of_bombs.setString(text);
	switch (text[0])
	{
	case '1':
		number_of_bombs.setFillColor(Color(50, 50, 255));
		break;
	case '2':
		number_of_bombs.setFillColor(Color(0, 155, 0));
		break;
	case '3':
		number_of_bombs.setFillColor(Color(180, 180, 0));
		break;
	case '4':
		number_of_bombs.setFillColor(Color(255, 0, 0));
		break;
	default:
		break;
	}
}

sf::IntRect Field::fieldTexture()
{
	IntRect texture_square;
	texture_square.top = 0;
	texture_square.height = texture_square.width = 32;

	if (Field_Condition == _Field_Condition::Covered || Field_Condition == _Field_Condition::Bomb )
	{
		texture_square.left = 96;
		Square.changeBackground(Color::White);
	}
	else if (Field_Condition == _Field_Condition::Revealed)
	{
		texture_square.left = 0;
		Square.changeBackground(Square.Light_Grey);
	}
	else if (Field_Condition == _Field_Condition::Flag || Field_Condition == _Field_Condition::Marked_Bomb)
	{
		texture_square.left = 32;
		Square.changeBackground(Color::White);
	}
	else if (Field_Condition == _Field_Condition::Revealed_Bomb)
	{
		texture_square.left = 64;
		Square.changeBackground(Color::White);
	}
	return texture_square;
}

void Field::setNeighborhood(std::vector<int> _neighbors)
{
	neighboring_fields = _neighbors;
}

std::vector<int> Field::getNeighborhood() const
{
	return neighboring_fields;
}

// FIELD_SHAPE
Field_Shape::Field_Shape(float _size, float _spacing)
	:Dark_Grey(197, 197, 197), Light_Grey(229, 229, 229)
{
	Size.x = Size.y = _size;
	spacing = _spacing;

	_Shape.setPrimitiveType(sf::Quads);
	_Shape.resize(48);
	Background = Light_Grey;

	setColor(Color::White);
	setSize(Size, spacing);
}

void Field_Shape::setColor(sf::Color _New_Color)
{
	Mix_Color = _New_Color;
	Color Mixed_LG = Light_Grey * Mix_Color;
	Color Mixed_DG = Dark_Grey * Mix_Color;
	Color Mixed_BG = Background * Mix_Color;
	for (int i = 0; i < 4; ++i)
		_Shape[i].color = Mixed_DG;
	for (int i = 4; i < 8; ++i)
		_Shape[i].color = Mixed_LG;
	for (int i = 8; i < 12; ++i)
		_Shape[i].color = Mixed_DG;
	for (int i = 12; i < 16; ++i)
		_Shape[i].color = Mixed_BG;
	for (int i = 16; i < 48; ++i)
		_Shape[i].color = Mixed_DG;
}

void Field_Shape::setSize(sf::Vector2f _Size, float _spacing)
{
	Size = _Size;
	float move = 0;
	float spacing = _spacing * (Size.y / 40.0F);
	if (spacing < 1.0F)
		spacing = 1.0F;

	//Frame
	for (int i = 0; i < 16; i += 4)
	{
		_Shape[i + 0].position = Vector2f(move, move);
		_Shape[i + 1].position = Vector2f(Size.x - move, move);
		_Shape[i + 2].position = Vector2f(Size.x - move, Size.y - move);
		_Shape[i + 3].position = Vector2f(move, Size.y - move);
		if ((i == 0 || i == 8) && spacing >= 2.0F)
			move += spacing / 2;
		else
			move += spacing;
	}
	//Frame points
	for (int i = 16; i < 32; i += 4)
	{
		float x = spacing / 2, mx = spacing / 2;
		float y = spacing / 2, my = spacing / 2;
		if (i >= 28 || (i >= 20 && i < 23))
		{
			x = Size.x - spacing / 2;
			mx *= -1;
		}
		if (i >= 24)
		{
			y = Size.y - spacing / 2;
			my *= -1;
		}

		_Shape[i + 0].position = Vector2f(x, y);
		_Shape[i + 1].position = Vector2f(x + mx, y);
		_Shape[i + 2].position = Vector2f(x + mx, y + my);
		_Shape[i + 3].position = Vector2f(x, y + my);
	}
	for (int i = 32; i < 48; i += 4)
	{
		float x = spacing, mx = spacing / 2;
		float y = spacing, my = spacing / 2;
		if (i >= 44 || (i >= 36 && i < 39))
		{
			x = Size.x - spacing;
			mx *= -1;
		}
		if (i >= 40)
		{
			y = Size.y - spacing;
			my *= -1;
		}

		_Shape[i + 0].position = Vector2f(x, y);
		_Shape[i + 1].position = Vector2f(x + mx, y);
		_Shape[i + 2].position = Vector2f(x + mx, y + my);
		_Shape[i + 3].position = Vector2f(x, y + my);
	}
}

void Field_Shape::changeBackground(const sf::Color color)
{
	Background = color;
	setColor(Color::White);
}

void Field_Shape::updateTexCoords()
{
	float left = static_cast<float>(TextureRect.left);
	float right = left + TextureRect.width;
	float top = static_cast<float>(TextureRect.top);
	float bottom = top + TextureRect.height;
	int i = 12;
	_Shape[i + 0].texCoords = Vector2f(left, top);
	_Shape[i + 1].texCoords = Vector2f(right, top);
	_Shape[i + 2].texCoords = Vector2f(right, bottom);
	_Shape[i + 3].texCoords = Vector2f(left, bottom);
}
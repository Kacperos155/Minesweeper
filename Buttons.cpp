#include "Buttons.hpp"
#include <iostream>
using namespace sf;

// VERTEX_SHAPE
sf::VertexArray Vertex_Shape::operator()() const
{
	return sf::VertexArray();
}

sf::Color Vertex_Shape::getColor() const
{
	return Mix_Color;
}

sf::Vector2f Vertex_Shape::getSize() const
{
	return Size;
}

float Vertex_Shape::getSpacing() const
{
	return spacing;
}

sf::Texture * Vertex_Shape::getTexture() const
{
	return _Texture;
}

sf::IntRect Vertex_Shape::getTextureRect() const
{
	return TextureRect;
}

void Vertex_Shape::setTexture(sf::Texture &_texture)
{
	_Texture = &_texture;
	setTextureRect(sf::IntRect(0, 0, _Texture->getSize().x, _Texture->getSize().y));
}

void Vertex_Shape::setTextureRect(sf::IntRect _TextureRect)
{
	TextureRect = _TextureRect;
	updateTexCoords();
}

void Vertex_Shape::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = _Texture;
	target.draw(_Shape, states);
}

//	BUTTON_SHAPE
Button_Shape::Button_Shape()
	:Dark_Grey(197, 197, 197), Light_Grey(229, 229, 229)
{
	_Shape.setPrimitiveType(sf::Quads);
	_Shape.resize(36);
	spacing = 3.0F;
	circularity = true;
}

Button_Shape::Button_Shape(float width, float height, float _spacing)
	:Dark_Grey(197, 197, 197), Light_Grey(229, 229, 229)
{
	Size.x = width;
	Size.y = height;
	spacing = _spacing;
	circularity = true;

	_Shape.setPrimitiveType(sf::Quads);
	_Shape.resize(36);

	setColor(sf::Color(255, 255, 255, 255));
	setSize(Size, spacing);
}

bool Button_Shape::getCircularity() const
{
	return circularity;
}

void Button_Shape::setColor(sf::Color _New_Color)
{
	Mix_Color = _New_Color;
	Color Mixed_LG = Light_Grey * Mix_Color;
	Color Mixed_DG = Dark_Grey * Mix_Color;
	for (int i = 0; i < 8; ++i)		//Frame
		_Shape[i].color = Mixed_DG;
	for (int i = 8; i < 12; ++i)	//Interior
		_Shape[i].color = Mixed_LG; 
	for (int i = 12; i < 16; ++i)	//Internal frame
		_Shape[i].color = Mixed_DG * Color(255, 255, 255, 75);
	for (int i = 16; i < 20; ++i)	//Interior
		_Shape[i].color = Mixed_LG;
	for (int i = 20; i < 36; ++i)	//Frame points
		_Shape[i].color = Mixed_DG;
}

void Button_Shape::setSize(sf::Vector2f Size, float _spacing)
{
	spacing = _spacing;
	float move = spacing;
	//Frame
	for (int i = 0; i < 8; i += 4)
	{
		float x = 0, y = spacing;
		if (i < 4)
		{
			x = spacing;
			y = 0;
		}
		if (!circularity)
			x = y = 0;
		_Shape[i + 0].position = Vector2f(x, y);
		_Shape[i + 1].position = Vector2f(Size.x - x, y);
		_Shape[i + 2].position = Vector2f(Size.x - x, Size.y - y);
		_Shape[i + 3].position = Vector2f(x, Size.y - y);
	}
	//Interior
	for (int i = 8; i < 20; i += 4)
	{
		_Shape[i + 0].position = Vector2f(move, move);
		_Shape[i + 1].position = Vector2f(Size.x - move, move);
		_Shape[i + 2].position = Vector2f(Size.x - move, Size.y - move);
		_Shape[i + 3].position = Vector2f(move, Size.y - move);
		move += spacing;
	}
	//Frame points
	for (int i = 20; i < 36; i += 4)
	{
		float x = spacing, mx = spacing;
		float y = spacing, my = spacing;
		if (i >=32 || (i >= 24 && i < 27))
		{
			x = Size.x - spacing;
			mx = -spacing;
		}
		if (i >= 28)
		{
			y = Size.y - spacing;
			my = -spacing;
		}

		_Shape[i + 0].position = Vector2f(x, y);
		_Shape[i + 1].position = Vector2f(x + mx, y);
		_Shape[i + 2].position = Vector2f(x + mx, y + my);
		_Shape[i + 3].position = Vector2f(x, y + my);
	}
}

void Button_Shape::setCircularity(bool _circularity)
{
	circularity = _circularity;
	setSize(Size, spacing);
}

void Button_Shape::updateTexCoords()
{
	float left = static_cast<float>(TextureRect.left);
	float right = left + TextureRect.width;
	float top = static_cast<float>(TextureRect.top);
	float bottom = top + TextureRect.height;
	int i = 16;
	_Shape[i + 0].texCoords = Vector2f(left, top);
	_Shape[i + 1].texCoords = Vector2f(right, top);
	_Shape[i + 2].texCoords = Vector2f(right, bottom);
	_Shape[i + 3].texCoords = Vector2f(left, bottom);
}

//	Button_Base
void Button_Base::setSize(float x, float y, float w, float h)
{
	Size.left = x;
	Size.top = y;
	Size.width = w;
	Size.height = h;
	backlight = false;
}

sf::Color Button_Base::getColor() const
{
	return Mix_Color;
}

bool Button_Base::isClicked(Vector2f M) const
{
	return Size.contains(M.x, M.y);
}

bool Button_Base::isClicked(Vector2i M) const
{
	return Size.contains(static_cast<float>(M.x), static_cast<float>(M.y));
}

bool Button_Base::isClicked(float x, float y) const
{
	return Size.contains(x, y);
}

//	Button_Graphic
Button_Graphic::Button_Graphic()
{
	backlight = false;
}

Button_Graphic::Button_Graphic(sf::Texture &_Texture, float x, float y, float w, float h)
{
	Button_Base::setSize(x, y, w, h);
	backlight_texture.width = normal_texture.width = static_cast<int>(w);
	backlight_texture.height = normal_texture.height = static_cast<int>(h);
	normal_texture.left = normal_texture.top = 0;
	backlight_texture.left = backlight_texture.top = 0;
	Shape.setPosition(x, y);
	Shape.setTexture(_Texture);
	Shape.setTextureRect(normal_texture);
}

sf::Sprite Button_Graphic::operator()() const
{
	return Shape;
}

void Button_Graphic::setColor(sf::Color _Mix_Color)
{
	Mix_Color = _Mix_Color;
	backlight = true;
}

void Button_Graphic::Draw(sf::RenderWindow &Window)
{
	if (isClicked(Mouse::getPosition(Window)))
	{
		backlight = true;
		Shape.setTextureRect(backlight_texture);
		Shape.setColor(Color(230, 230, 230));
		if (Mouse::isButtonPressed(Mouse::Left))
			Shape.setColor(Color(130, 130, 130));
	}
	else if (backlight)
	{
		backlight = false;
		Shape.setTextureRect(normal_texture);
		Shape.setColor(Color(255, 255, 255));
	}
	Window.draw(Shape);
}

void Button_Graphic::scale(float x, float y)
{
	Shape.setScale(x, y);
	Size.width = Size.width*x;
	Size.height = Size.height*y;
}

void Button_Graphic::scale(Vector2f _scale)
{
	Shape.setScale(_scale);
	Size.width = Size.width*_scale.x;
	Size.height = Size.height*_scale.y;
}

void Button_Graphic::setTexture(int x, int y)
{
	normal_texture.left = x;
	normal_texture.top = y;
	Shape.setTextureRect(normal_texture);
}

void Button_Graphic::backlightTexture(int x, int y)
{
	backlight_texture.left = x;
	backlight_texture.top = y;
}

//	Button_Text
Button_Text::Button_Text()
	:Shape(0.0F, 0.0F, 0.0F)
{
	backlight = false;
}

Button_Text::Button_Text(string text, float x, float y, float w, float h)
	:Shape(w, h)
{
	Button_Base::setSize(x, y, w, h);
	_Text.setString(text);
	_Text.setFont(Arial);
	_Text.setCharacterSize(static_cast<unsigned>(h - 2));
	_Text.setFillColor(Color(0, 0, 0));
	adjustText();

	Shape.setColor(Color(255, 255, 255));
	Shape.setPosition(static_cast<float>(x), static_cast<float>(y));
}

sf::VertexArray Button_Text::operator()() const
{
	return Shape();
}

void Button_Text::setColor(sf::Color _Mix_Color)
{
	Mix_Color = _Mix_Color;
	backlight = true;
}

void Button_Text::Draw(sf::RenderWindow &Window)
{
	if (isClicked(Mouse::getPosition(Window)))
	{
		backlight = true;
		Shape.setColor(Color(230, 230, 230) * Mix_Color);
		if (Mouse::isButtonPressed(Mouse::Left))
			Shape.setColor(Color(130, 130, 130) * Mix_Color);
	}
	else if (backlight)
	{
		backlight = false;
		Shape.setColor(Color::White * Mix_Color);
	}
	Window.draw(Shape);
	Window.draw(_Text);
}

void Button_Text::adjustText()
{
	Vector2f _Text_Center;
	float temp;
	while (_Text.getLocalBounds().width >= Size.width - 3.0F * Shape.getSpacing())
	{
		unsigned int x = _Text.getCharacterSize();
		_Text.setCharacterSize(x-1);
	}
	while (_Text.getLocalBounds().height >= Size.height - 6.0F * Shape.getSpacing())
	{
		unsigned int x = _Text.getCharacterSize();
		_Text.setCharacterSize(x-1);
	}
	temp = _Text.getLocalBounds().left + _Text.getLocalBounds().width;
	_Text_Center.x = static_cast<float>(static_cast<int>(temp / 2.0F));
	temp = _Text.getLocalBounds().top + _Text.getLocalBounds().height;
	_Text_Center.y = static_cast<float>(static_cast<int>(temp / 2.0F));
	_Text.setOrigin(_Text_Center);

	_Text_Center.x = static_cast<int>(Size.width / 2.0F) + Size.left;
	_Text_Center.y = Size.top + static_cast<int>(Size.height / 2.5F);
	_Text.setPosition(_Text_Center);
}

void Button_Text::scale(float x, float y)
{
	scale(Vector2f(x, y));
}

void Button_Text::scale(sf::Vector2f _scale)
{
	Size.width = Size.width * _scale.x;
	Size.height = Size.height * _scale.y;
	_scale.x *= Shape.getSize().x;
	_scale.y *= Shape.getSize().y;
	Shape.setSize(_scale, Shape.getSpacing());
	adjustText();
}

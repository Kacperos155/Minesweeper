//By Kacper Zielinski
#pragma once
#ifndef SzK_Button
#define SzK_Button
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
using std::string;

//Font
extern sf::Font Arial;
extern std::vector<char> Arial_Buffer;

class Vertex_Shape :public sf::Drawable, public sf::Transformable
{
public:
	sf::VertexArray operator()() const;
	sf::Color getColor() const;
	sf::Vector2f getSize() const;
	float getSpacing() const;
	sf::Texture* getTexture() const;
	sf::IntRect getTextureRect() const;
	void setTexture(sf::Texture &texture);
	void setTextureRect(sf::IntRect TextureRect);
	virtual void setColor(sf::Color Mix_Color) = 0;
	virtual void setSize(sf::Vector2f Size, float spacing = 3.0F) = 0;
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void updateTexCoords() = 0;
	sf::VertexArray _Shape;
	sf::Vector2f Size;
	sf::Color Mix_Color;
	sf::Texture* _Texture = nullptr;
	sf::IntRect TextureRect;
	float spacing;
};

class Button_Shape :public Vertex_Shape
{
public:
	Button_Shape();
	Button_Shape(float width, float height, float spacing = 3.0F);
	bool getCircularity() const;
	void setColor(sf::Color Mix_Color) override;
	void setSize(sf::Vector2f Size, float spacing = 3.0F) override;
	void setCircularity(bool circularity);
	const sf::Color Dark_Grey;
	const sf::Color Light_Grey;
private:
	void updateTexCoords() override;
	bool circularity;
};

class Button_Base
{
public:
	void setSize(float x, float y, float w, float h);
	sf::Color getColor() const;
	virtual void setColor(sf::Color Mix_Color) = 0;
	virtual void Draw(sf::RenderWindow &Window) = 0;
	virtual void scale(float x, float y) = 0;
	virtual void scale(sf::Vector2f _scale) = 0;
	bool isClicked(sf::Vector2f M) const;
	bool isClicked(sf::Vector2i M) const;
	bool isClicked(float x, float y) const;
protected:
	sf::Rect<float> Size;
	sf::Color Mix_Color = sf::Color(255,255,255);
	bool backlight = false;
};

class Button_Graphic :public Button_Base
{
public:
	Button_Graphic();
	Button_Graphic(sf::Texture &_Texture, float x, float y, float w, float h);
	sf::Sprite operator()() const;
	void setColor(sf::Color Mix_Color) override;
	void Draw(sf::RenderWindow &Window) override;
	void scale(float x, float y) override;
	void scale(sf::Vector2f _scale) override;
	void setTexture(int x, int y);
	void backlightTexture(int x, int y);
private:
	sf::Sprite Shape;
	sf::IntRect backlight_texture;
	sf::IntRect normal_texture;
};

class Button_Text :public Button_Base
{
public:
	Button_Text();
	Button_Text(string text, float x, float y, float w, float h);
	sf::VertexArray operator()() const;
	void setColor(sf::Color Mix_Color) override;
	void adjustText();
	void Draw(sf::RenderWindow &Okno) override;
	void scale(float x, float y) override;
	void scale(sf::Vector2f _scale) override;
private:
	sf::Text _Text;
	Button_Shape Shape;
};

#endif SzK_Button

//By Kacper Zielinski
#pragma once
#ifndef Saper
#define Saper
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <time.h>
#include <fstream>
#include <string>
#include <queue>
#include <memory>
#include <utility>
#include "Buttons.hpp"
#include "Binary Converter.hpp"
using std::cout;
using std::vector;
using std::string;
//Zegar Gry
extern sf::Clock Game_Clock;
//Textury
extern sf::Image Icon;
extern sf::Texture Tx_Field, Tx_Digit, Tx_Face_Button;
//Silnik Losujacy
int randomNumberGenerator(int bottom_range, int upper_range, int seed);
//Menadzer Zasobow
void ResourceLoader(std::string binary_file_location);
void LoadingError(std::string binary_file_location);

struct Difficulty_Level
{
	Difficulty_Level();
	Difficulty_Level(int width, int height, int bombs, bool afm=true);
	Difficulty_Level& operator*=(const Difficulty_Level X);
	bool operator==(const Difficulty_Level& X) const;

	unsigned int width, height, bombs;
	bool auto_first_move;
};

class Window_Mode
{
public:
	virtual void Draw(sf::RenderWindow &Window) = 0;
	virtual void Click(sf::RenderWindow &Window) = 0;
	sf::Vector2u getWindowSize() const;
	virtual ~Window_Mode();
protected:
	virtual void setView(float width, float height);
	sf::Vector2u window_size;
	sf::View _View;
};

class Field_Shape :public Vertex_Shape
{
public:
	Field_Shape(float size = 40.0F, float spacing = 2.0F);
	virtual void setColor(sf::Color _New_Color) override;
	virtual void setSize(sf::Vector2f _Size, float _spacing = 2.0F) override;
	void changeBackground(const sf::Color color);
	const sf::Color Dark_Grey;
	const sf::Color Light_Grey;
private:
	void updateTexCoords() override;
	sf::Color Background;
};

class Field :public sf::Drawable
{
public:
	Field(int x, int y, int s);
	virtual void draw(sf::RenderTarget &Window, sf::RenderStates states) const override;
	short setFlag();
	bool reaveal(bool map_discovered);
	bool setBomb();
	bool isBomb() const;
	bool isSolved() const;
	bool isEmpty(bool revealed) const;
	void setNumberOfBombs(int x);
	void setNeighborhood(vector<int> neighbors);
	vector<int> getNeighborhood() const;
private:
	float x, y, s;
	enum class _Field_Condition { Covered, Flag, Bomb, Marked_Bomb, Revealed, Revealed_Bomb }Field_Condition;
	Field_Shape Square;
	sf::Text number_of_bombs;
	sf::IntRect fieldTexture();
	vector<int> neighboring_fields;
};

class GUI :public Window_Mode
{
public:
	GUI(float gx, float gy, float s);
	void Draw(sf::RenderWindow &Okno, int bombs_number);
	void Click(sf::RenderWindow &Okno) override;
	void gameOver(bool victory);
private:
	float x, y, s;
	int seconds;
	bool game_over;
	bool show_gui_hint;
	sf::RectangleShape background;
	sf::Sprite Bombs_Counter[3];
	sf::Sprite Time_Counter[5];
	sf::Sprite Gui_Hints[2];
	void Draw(sf::RenderWindow &Window) override;
	void setView(float width, float height) override;
	void countBombs(int bombs_number);
	void countTime();
	sf::IntRect numberTexture(short number);
	Button_Graphic Face_Button;
};

class Minesweeper_Map :public Window_Mode
{
public:
	Minesweeper_Map();
	Minesweeper_Map(Difficulty_Level D, unsigned int size, unsigned int seed);
	void Draw(sf::RenderWindow &Window) override;
	void Click(sf::RenderWindow &Window) override;
	string getSeed() const;
	void firstMove();
private:
	unsigned int _width, _height, _size;
	GUI _GUI;
	vector<Field> Fields;
	unsigned int bombs_number;
	unsigned int game_seed;
	bool map_discovered;
	void setView(float width, float height) override;
	void setBombsOnMap(unsigned int bombs_number);
	void gameOver(bool victory);
	void WaveOfReveals(std::queue<int> &reveals_queue);
	void findNeighboringFields(unsigned int field_id);
	void revealField(unsigned int field_id);
};

class Config
{
public:
	Config(string file_name);
	Difficulty_Level getDifficult() const;
	unsigned int getSize() const;
	int getSeed() const;
	void newFile(const Difficulty_Level& X, const int& Size);
	void loadingData();
private:
	unsigned int size, seed;
	Difficulty_Level Difficulty;
	std::fstream config_file;
	string file_name;
	string text;
	string getText();
	int getInt();
	void createFile();
};

class Communication
{
public:
	enum class Statement { NORMAL, NEW_GAME, SETTINGS, CLOSE, VICTORY, CENTER_WINDOW };
	Communication();
	void input(Statement x);
	Statement output();
	void sendTime(int seconds);
	int getTime() const;
	void backupTime();
	void restoreTime();
	void resetTime();
private:
	Statement _Statement;
	std::size_t seconds;
	std::size_t seconds_backup;
};

class Settings :public Window_Mode
{
public:
	Settings(unsigned int x, unsigned int y, Config &INI);
	void Draw(sf::RenderWindow &Okno) override;
	void Click(sf::RenderWindow &Okno) override;
	const Difficulty_Level Easy, Medium, Hard;
	void saveTime(std::size_t s);
	~Settings();
private:
	int size;
	vector<char> stats;
	void switchSelection(bool on, std::size_t pos);
	void createLine(float x, float y, float w, float h);
	void createButton(string text, float x, float y, float w, float h);
	void createText(string text, float x, float y, float w, float h, unsigned int size);
	void fillStats();
	vector<Button_Text> Buttons;
	vector<sf::Text> Texts;
	vector<sf::VertexArray> Lines;
	Field_Shape background;
	Config *INI;
	Difficulty_Level Custom, Change;
	Binary_Converter Stats_Saver;
};

extern Communication _Communication;
//Tworzenie Planszy
void New_Game(std::unique_ptr<Minesweeper_Map> &NP, Config &INI);
#endif

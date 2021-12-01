#include "Minesweeper.hpp"
using namespace std;

Config::Config(string _file_name)
	:Difficulty(20, 20, 14), size(40), seed(0)
{
	file_name = _file_name;
	config_file.open(file_name, ios::in);
	if (!config_file.good())
		createFile();
	loadingData();
}

Difficulty_Level Config::getDifficult() const
{
	return Difficulty;
}

unsigned int Config::getSize() const
{
	return size;
}

int Config::getSeed() const
{
	return seed;
}

void Config::newFile(const Difficulty_Level& X, const int& _size)
{
	size = _size;
	Difficulty = X;
	createFile();
	loadingData();
}

void Config::loadingData()
{
	config_file.close();
	config_file.open(file_name, ios::in);
	while (!config_file.eof())
	{
		getline(config_file, text);
		if (text[0] != ':')
			continue;
		else
		{
			switch (text[1])
			{
			case 'W':
				Difficulty.width = getInt();
				break;
			case 'H':
				Difficulty.height = getInt();
				break;
			case 'S':
				size = getInt();
				break;
			case 'B':
				Difficulty.bombs = getInt();
				break;
			case 'f':
				Difficulty.auto_first_move = getInt();
				break;
			case 's':
				seed = getInt();
				break;
			default:
				break;
			}
		}
	}
	config_file.close();
}

string Config::getText()
{
	size_t begin;
	begin = text.find("-> ");
	begin += 3;
	string s_data = "";
	for (size_t i = begin; i < text.length(); ++i)
		s_data += text[i];
	return s_data;
}

int Config::getInt()
{
	return stoi(getText());
}

void Config::createFile()
{
	config_file.close();
	config_file.open(file_name, ios::out);
	//Tresc configu
	config_file << "#MINESWEEPER - CONFIG \n";
	config_file << "\n";
	config_file << "# W = Width of the game board\n";
	config_file << ":W -> " << Difficulty.width << "\n";
	config_file << "# H = Height of the game board\n";
	config_file << ":H -> " << Difficulty.height << "\n";
	config_file << "# B% = Percent of bombs\n";
	config_file << ":B% -> " << Difficulty.bombs << "\n";
	config_file << "# S = Size of single field\n";
	config_file << ":S -> " << size << "\n";
	config_file << "# f = Auto first move\n";
	config_file << ":f -> " << Difficulty.auto_first_move << "\n";
	config_file << "# seed = Game board seed\n";
	config_file << "# Set 0 to auto generate\n";
	config_file << ":seed -> " << seed << "\n";
	config_file.close();
}

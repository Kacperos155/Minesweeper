#include "Minesweeper.hpp"
#include <Windows.h>

Window_Mode::~Window_Mode() {}

sf::Vector2u Window_Mode::getWindowSize() const
{
	return window_size;
}

void Window_Mode::setView(float width, float height)
{
	window_size = sf::Vector2u(static_cast<unsigned int>(width), static_cast<unsigned int>(height));
	_View.setViewport(sf::FloatRect(0, 0, 1.0F, 1.0F));
	_View.setSize(sf::Vector2f(width, height));
	_View.setCenter(width / 2, height / 2);
}

void New_Game(std::unique_ptr<Minesweeper_Map> &NP, Config & INI)
{
	auto NN = std::make_unique<Minesweeper_Map>(INI.getDifficult(), INI.getSize(), INI.getSeed());
	NP = std::move(NN);
	NN = nullptr;
	//Pierwszy ruch
	if(INI.getDifficult().auto_first_move)
		NP->firstMove();
	//Wygladzanie
	bool smooth = (INI.getSize() >= 40) ? 0 : 1;
	Tx_Face_Button.setSmooth(smooth);
	Tx_Digit.setSmooth(smooth);
	Tx_Field.setSmooth(smooth);
}

void ResourceLoader(std::string binary_file_location)
{
	Binary_Converter BC(binary_file_location);
	if(!BC.Is_Good())
		LoadingError(binary_file_location);
	/*
	---Lista Zasobow---
	Font: !(Potrzebuje buffera)!
		Arial (arial.ttf)
	Texture:
		Tx_Field (Pole.png)
		Tx_Digit (Cyfrowe Cyfry.png)
		Tx_Face_Button (Buzka.png)
	Image:
		Icon (Ikona.png)
	*/
	const int resource_number = 5;
	string resource_name = "";
	vector<char> resource;

	for (int i = 0; i < resource_number; ++i)
	{
		BC.LoadFromBinary();
		resource_name = BC.GetName();
		resource = BC.ExportBuffer();
		if (resource_name == "arial.ttf")
		{
			Arial_Buffer = std::move(resource);
			Arial.loadFromMemory(&Arial_Buffer[0], Arial_Buffer.size());
		}
		else if (resource_name == "Pole.png")
			Tx_Field.loadFromMemory(&resource[0], resource.size());
		else if (resource_name == "Cyfrowe Cyfry.png")
			Tx_Digit.loadFromMemory(&resource[0], resource.size());
		else if (resource_name == "Buzka.png")
			Tx_Face_Button.loadFromMemory(&resource[0], resource.size());
		else if (resource_name == "Ikona.png")
			Icon.loadFromMemory(&resource[0], resource.size());
	}
	resource.clear();
}

void LoadingError(std::string binary_file_location)
{
	const LPCWSTR text = L"B³¹d wczytywania!";
	std::wstring temp = L"Nie znaleziono pliku zasobów!\nSprawdŸ czy w lokacji gry znajduje siê plik \"";
	std::wstring temp2(binary_file_location.begin(), binary_file_location.end());
	temp += temp2;
	temp += L"\"!";
	LPCWSTR title = temp.c_str();
	MessageBoxW(NULL, title, text, MB_OK | MB_ICONERROR);
	exit(1);
}

Communication::Communication()
{
	_Statement = Statement::NORMAL;
	seconds = 0;
	seconds_backup = 0;
}

void Communication::input(Statement x)
{
	_Statement = x;
}

Communication::Statement Communication::output()
{
	Statement x = _Statement;
	_Statement = Statement::NORMAL;
	return x;
}

void Communication::sendTime(int _seconds)
{
	seconds = _seconds;
}

int Communication::getTime() const
{
	return seconds;
}

void Communication::backupTime()
{
	seconds_backup = seconds;
}

void Communication::restoreTime()
{
	seconds = seconds_backup;
}

void Communication::resetTime()
{
	seconds = seconds_backup = 0;
}

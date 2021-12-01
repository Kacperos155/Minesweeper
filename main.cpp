#include "Minesweeper.hpp"
#include <thread>
using namespace sf;
RenderWindow Game_Window;
Event Eventy;
Clock Game_Clock;
Communication _Communication;
Font Arial;
std::vector<char> Arial_Buffer;
Image Icon;
Texture Tx_Field, Tx_Digit, Tx_Face_Button;

int main()
{
	//Menadzer Zasobow
	std::thread Resources(ResourceLoader,"Saper.bin");
	srand(static_cast<unsigned int>(time(NULL)));
	Config _Config("config.ini");
	//Okno
	VideoMode Desktop = VideoMode::getDesktopMode();
	Vector2i Center;
	{
		Difficulty_Level _Difficulty = _Config.getDifficult();
		int size = _Config.getSize();
		unsigned int W_W = _Difficulty.width*size;
		unsigned int W_H = static_cast<unsigned int>(size * (_Difficulty.height + 1.5F));
		Game_Window.create(VideoMode(W_W, W_H), "Minesweeper", Style::Close);
	}
	Window_Mode* _Window_Mode = nullptr;
	auto _Game = std::make_unique<Minesweeper_Map>();
	New_Game(_Game, _Config);
	_Window_Mode = _Game.get();
	Resources.join();
	auto _Settings = std::make_unique<Settings>(480, 480, _Config);
	Game_Window.setIcon(32, 32, Icon.getPixelsPtr());
	Game_Window.setFramerateLimit(60);

	while (Game_Window.isOpen())
	{
		while (Game_Window.pollEvent(Eventy))
		{
			switch (Eventy.type)
			{
			case Event::Closed:
				Game_Window.close();
				_Window_Mode = _Game.get();
				_Communication.input(Communication::Statement::CLOSE);
				break;
			case Event::KeyPressed:
				if (Eventy.key.code == Keyboard::Escape)
					_Communication.input(Communication::Statement::CLOSE);
				if (Eventy.key.code == Keyboard::R)
					_Communication.input(Communication::Statement::NEW_GAME);
				break;
			case Event::MouseButtonPressed:
				if(_Window_Mode != nullptr)
					_Window_Mode->Click(Game_Window);
			default:
				break;
			}
		}

		//Renderowanie Klatki
		Game_Window.clear();
		if (_Window_Mode != nullptr)
			_Window_Mode->Draw(Game_Window);
		Game_Window.display();

		//Obsluga wyjscia planszy
		switch (_Communication.output())
		{
		case Communication::Statement::NEW_GAME: //Nowy seed
			New_Game(_Game, _Config);
			_Window_Mode = _Game.get();
			Game_Window.setTitle("Saper - " + _Game->getSeed());
			_Communication.resetTime();
			break;
		case Communication::Statement::SETTINGS: //Ustawienia
			_Window_Mode = _Settings.get();
			_Communication.backupTime();
			break;
		case Communication::Statement::CLOSE: //Zamykanie/Powrot
			if (_Window_Mode == _Game.get())
			{
				return 0;
			}
			else if (_Window_Mode == _Settings.get())
			{
				_Window_Mode = _Game.get();
				_Communication.restoreTime();
				Game_Clock.restart();
			}
			break;
		case Communication::Statement::VICTORY:
			_Settings->saveTime(_Communication.getTime());
			break;
		case Communication::Statement::CENTER_WINDOW: //Centrowanie Okna
			;
			Center.x = (Desktop.width / 2) - (_Game->getWindowSize().x / 2);
			Center.y = (Desktop.height / 2) - (_Game->getWindowSize().y / 2);
			Game_Window.setPosition(Center);
			break;
		default:
			break;
		}
	}
	return 0;
}

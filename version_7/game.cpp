#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>

#include <iostream>

#include "lib/Client.hpp"

class Game {
private:
	sf::RenderWindow _window;
	bool _main_menu, _playing;
	Client _client;
	
private:
	void handleKeyRelease(sf::Keyboard::Key key)
	{
		std::cout << (_main_menu ? "MENU" : "GAME") << std::endl;
		sf::Vector2u who, whom;
		sf::IpAddress ip;
		std::string msg;
		
		switch (key) {
			case sf::Keyboard::I: // Info
				if (_playing) {
					std::cout << "|| I: info" << std::endl;
					std::cout << "|| Enter: nextPhase" << std::endl;
					std::cout << "|| A: attack" << std::endl;
					std::cout << "|| F: feed" << std::endl;
					std::cout << "|| D: display" << std::endl;
					std::cout << "|| M: message" << std::endl;
					std::cout << "|| X: exit" << std::endl;
				} else if (_main_menu) {
					std::cout << "|| I: info" << std::endl;
					std::cout << "|| C: connnect" << std::endl;
					std::cout << "|| H: host" << std::endl;
					std::cout << "|| X: exit" << std::endl;
				}
				break;
			
			case sf::Keyboard::Enter: // nestPhase
				std::cout << "Next game phase" << std::endl;
				_client.nextPhase();
				std::cout << _client.getPhase() << std::endl;
				break;
			
			case sf::Keyboard::A: // Attack
				if (_main_menu) break;
				if (_client.getPhase() != gm::ATTACK) {
					std::cout << "It's not your turn to attack!" << std::endl;
					break;
				}
				std::cout << "Attack: Who [y][x] Whom [y][x]" << std::endl;
				std::cin >> who >> whom;
				_client.attack(who, whom);
				break;
				
			case sf::Keyboard::F: // Feed
				if (_main_menu) break;
				if (_client.getPhase() != gm::FEED) {
					std::cout << "It's not your turn to feed!" << std::endl;
					break;
				}
				std::cout << "Feed: Whom [y][x]" << std::endl;
				std::cin >> whom;
				_client.feed(whom);
				break;
				
			case sf::Keyboard::C: // Connect
				if (_playing) break;
				std::cout << "Connect: IpAddress" << std::endl;
				std::cin >> ip;
				if (!_client.connect(ip)) {
					std::cout << "Connection failed" << std::endl;
					break;
				}
				_main_menu = false;
				_playing = true;
				break;
				
			case sf::Keyboard::H: // Host
				if (_playing) break;
				std::cout << "Host" << std::endl;
				if (!_client.host(5, 5)) {
					std::cout << "Connection failed" << std::endl;
					break;
				}
				_main_menu = false;
				_playing = true;
				break;
				
			case sf::Keyboard::D: // Display
				if (_main_menu) break;
				std::cout << std::endl << std::endl << std::endl << std::endl;
				
				std::cout << "Playing as: " << _client.whoAmI() << std::endl << std::endl;
				
				std::cout << "Field:" << std::endl << _client.field() << std::endl << std::endl;
				
				std::cout << "Players:"  << std::endl;
				for (auto it: _client.players())
					std::cout << " | " << it << std::endl;
				std::cout << std::endl;
				
				std::cout << _client.whoIsPlaying() << " is playing" << std::endl << std::endl;
				
				std::cout << "Message history:"  << std::endl;
				for (auto it: _client.msg_queue())
					std::cout << ">>> " << it << std::endl;
				std::cout << std::endl;
				
				std::cout << std::endl << std::endl << std::endl << std::endl;
				break;
				
			case sf::Keyboard::M: // Message
				if (_main_menu) break;
				std::cout << "Enter message (no spaces!)" << std::endl;
				std::cin >> msg;
				_client.msg_send(msg);
				break;
				
			case sf::Keyboard::X: // eXit
				if (_playing) {
					_client.disconnect();
					std::cout << "disconnected" << std::endl;
					_playing = false;
					_main_menu = true;
				} else if(_main_menu)
					_window.close();
				break;
				
			default:
				std::cout << key << std::endl;
				break;
		}
	}
	
	void processEvents(void)
	{
		sf::Event event;
		while (_window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::KeyPressed:
				handleKeyRelease(event.key.code);
				break;
				
			case sf::Event::Closed:
				_window.close();
				break;
				
			default:
				break;
			}
		}
	}
	
	void update(void)
	{
		// I dunno
	}
	
	void render(void)
	{
		if (_main_menu) {
			// Draw menu
		} else if (_playing) { 
			// Draw field
		}
	}
	
public:
	Game(void)
		: _window(sf::VideoMode(100, 100), "TBSGame", sf::Style::Close | sf::Style::Resize)
		, _client("XFireFall")
		, _main_menu(true)
		, _playing(false)
	{ }
	
	void run(void)
	{
		sf::Time TPF = sf::seconds(1.f / 60.f); // time per frame

		sf::Clock clock;
		sf::Time timeSinceLastUpdate = sf::Time::Zero;
		while (_window.isOpen()) {
			timeSinceLastUpdate += clock.restart();
			for (timeSinceLastUpdate += clock.restart(); timeSinceLastUpdate > TPF; timeSinceLastUpdate -= TPF) {
				processEvents();
				update();
			}
			render();
		}
	}
};

int main()
{
	Game game;
	game.run();
	return 0;
}
 

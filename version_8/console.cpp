#include <iostream>

#include "Lib/backend/Client.hpp"

int main(int argc, char* argv[])
{
	if (argc > 3) {
		std::cerr << "Call format: " << argv[0] << " [nickname=_console_player] [IpAddres=127.0.0.1]" << std::endl;
		return 1;
	}
	
	std::string nickname{(argc > 1) ? argv[1] : "_console_player"};
	sf::IpAddress ip{(argc > 2) ? argv[2] : "127.0.0.1"};
	
	Client client(nickname);
	if (argc == 3) {
		if (!client.connect(ip))
			return 1;
	} else {
		if (!client.host(4, 4))
			return 1;
	}
	
	while (1) {
		std::cout << "Wait for your turn..." << std::endl;
		while (client.getPhase() == gm::WAIT) {
			sf::sleep(sf::milliseconds(1000));
		}
		
		int cmd;
		bool busy;
		sf::Vector2u who, whom;
		std::string msg;
		
		/*====| ATTACK |====*/
		
		busy = true;
		while (busy) {
			std::cout << "Enter command:" << std::endl;
			std::cout << " 0 = disconnect" << std::endl;
			std::cout << " 1 = attack..." << std::endl;
			std::cout << " 2 = stop attacking" << std::endl;
			std::cout << " 3 = show field" << std::endl;
			std::cout << " 4 = show players" << std::endl;
			std::cout << " 5 = send message" << std::endl;
			std::cin >> cmd;
			if (!client.isConnected()) break;
			
			switch (cmd) {
				case 0:
					std::cout << "Client shutdown" << std::endl;
					client.disconnect();
					return 0;
					
				case 1:
					std::cout << "Enter arguments:" << std::endl;
					std::cout << " Who: y x" << std::endl;
					std::cin >> who;
					if (!client.isConnected()) break;
					
					std::cout << " Whom: y x" << std::endl;
					std::cin >> whom;
					if (!client.isConnected()) break;
					
					client.attack(who, whom);
					break;
				
				case 2:
					client.nextPhase();
					busy = false;
					break;
					
				case 3:
					std::cout << client.field() << std::endl;
					break;
					
				case 4:
					for (auto it: client.players())
						std::cout << " >" << it << std::endl;
					break;
					
				case 5:
					std::cout << "Enter message:" << std::endl;
					std::cin >> msg;
					client.msg_send(msg);
					break;
					
				default:
					std::cerr << "Unknown command (" << cmd << ')'<< std::endl;
					break;
			}
		}
		
		/*====|  FEED  |====*/
		
		busy = true;
		while (busy) {
			std::cout << "Enter command:" << std::endl;
			std::cout << " 0 = disconnect" << std::endl;
			std::cout << " 1 = feed..." << std::endl;
			std::cout << " 2 = end turn" << std::endl;
			std::cout << " 3 = show field" << std::endl;
			std::cout << " 4 = show players" << std::endl;
			std::cout << " 5 = send message" << std::endl;
			std::cin >> cmd;
			if (!client.isConnected()) break;
			
			switch (cmd) {
				case 0:
					std::cout << "Client shutdown" << std::endl;
					client.disconnect();
					return 0;
					
				case 1:
					std::cout << "Enter arguments:" << std::endl;
					std::cout << " Whom: y x" << std::endl;
					std::cin >> whom;
					if (!client.isConnected()) break;
					
					client.feed(whom);
					break;
				
				case 2:
					client.nextPhase();
					busy = false;
					break;
					
				case 3:
					std::cout << client.field() << std::endl;
					break;
					
				case 4:
					for (auto it: client.players())
						std::cout << " >" << it << std::endl;
					break;
					
				case 5:
					std::cout << "Enter message:" << std::endl;
					std::cin >> msg;
					client.msg_send(msg);
					break;
					
				default:
					std::cerr << "Unknown command (" << cmd << ')'<< std::endl;
					break;
			}
		}
	}
	
	std::clog << "Game shutdown" << std::endl;
	return 0;
} 

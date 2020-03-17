#include "../Client.hpp"

int main()
{
	sf::TcpSocket *socket = new sf::TcpSocket;
	if (sf::Socket::Done != socket->connect("127.0.0.1", 5990)) {
		std::cout << "connection failed" << std::endl;
		return 1;
	}
	std::cout << "connected" << std::endl;
	
	Client cli(socket);
	
	TBSGame::Field field = cli.init("XFireFall");
	
	while (1) {
		
		std::cout << "Waiting for push" << std::endl;
		
		for (Client::UPDresult res = Client::UPDresult::NothingNew; res != Client::UPDresult::ACKrecieved; res = cli.update_and_getAck(field)) {
			if (res == Client::UPDresult::Updated)
				std::cout << field << std::endl;
		}
		std::cout << "Ack received" << std::endl;
		
		while (1) {
			sf::Vector2u v1, v2;
			
			std::cout << field << std::endl;
			
			std::cout << "Enter whom to fight OR '0' to go start feeding:" << std::endl;
			
			std::cout << "From:" << std::endl;
			std::cin >> v1.y;
			if (v1.y == 0) {
				cli.ack();
				std::cout << "Ack sent" << std::endl;
				break;
			}
			std::cin >> v1.x;
			
			std::cout << "Where:" << std::endl;
			std::cin >> v2.y >> v2.x;
			
			if (cli.fight(field, v1, v2))
				std::cout << "Feed success" << std::endl;
			else
				std::cout << "Feed FAILED" << std::endl;
		}
		
		while (1) {
			sf::Vector2u v;
			
			std::cout << field << std::endl;
			
			std::cout << "Enter whom to feed OR '0' to go finish your turn:" << std::endl;
			
			std::cout << "Whom:" << std::endl;
			std::cin >> v.y;
			if (v.y == 0) {
				cli.ack();
				std::cout << "Ack sent" << std::endl;
				break;
			}
			std::cin >> v.x;
			
			if (cli.feed(field, v))
				std::cout << "Feed success" << std::endl;
			else
				std::cout << "Feed FAILED" << std::endl;
		}
	}
	return 0; 
}

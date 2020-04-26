#include "Client.hpp"


int main()
{
	sf::TcpSocket *socket = new sf::TcpSocket;
	if (!socket->connect("127.0.0.1", 5990))
		std::cout << "disconnected" << std::endl;
	std::cout << "connected" << std::endl;
	
	Client cli(socket);
	
	field_t field(10);
	for (int i = 0; i < 10; ++i)
		field[i].resize(10);
	
	while (1) {
		
		std::cout << "Waiting for push" << std::endl;
		
		for (uint16_t res = 0; res != 2; res = cli.update_and_getAck(field)) {
			if (res == 1)
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
			
			cli.fight(field, v1, v2);
			std::cout << "Fight sent" << std::endl;
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
			
			cli.feed(field, v);
			std::cout << "Feed sent" << std::endl;
		}
		
		
		
		
		/*std::cout << "enter whom to feed:" << std::endl;
		std::cin >> x >> y;
		sf::Vector2u v3(x,y);
		cli.send_feed(v3);
		
		std::cout << "waiting for server..." << std::endl;
		while(!cli.isUnread())
			;
		std::pair<int, std::string> mail = cli.get_mail();
		std::cout << "Received: '" << (std::string) mail.second << "'" << std::endl;
		std::cout << " from " << (int) mail.first  << "st player" << std::endl;*/
	}
	return 0;
}

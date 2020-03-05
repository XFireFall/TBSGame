#include <iostream>
#include <SFML/Network.hpp>

enum CMD { READY, MOVE, ADD, _CMD_count };

int main()
{
	sf::TcpSocket socket;
	socket.connect("127.0.0.1", 5990);

	while (1) {
		sf::Packet packet;
		std::string message;
		std::cin >> message;
		packet << message;
		socket.send(packet);
	}
	return 0;
}

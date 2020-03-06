#include <iostream>
#include <SFML/Network.hpp>

int main()
{
	sf::TcpSocket socket;
	socket.connect("127.0.0.1", 5990);

	while (1) {
		sf::Packet packet;
		socket.receive(packet);
		std::string message;
		packet >> message;
		std::cout << message << std::endl;
	}
	return 0;
}

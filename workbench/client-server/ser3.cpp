#include <iostream>
#include <vector>
#include <queue>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#include "Client.hpp"

int main()
{
	sf::TcpListener listener;
	while(sf::Socket::Done != listener.listen(5990))
		;
	std::cout << "Listerer binded to port " << 5990 << " successfully" << std::endl;
	
	std::vector<Client *> clients;
	sf::TcpSocket *p_socket = new sf::TcpSocket;
	while (1) {
		if (sf::Socket::Done == listener.accept(*p_socket)) {
			std::cout << "New client connected: " << p_socket->getRemoteAddress() << std::endl;
			
			clients.push_back(new Client(p_socket));
			
			listener.setBlocking(false);
			p_socket = new sf::TcpSocket;
		}
		
		/*if (clients[0]->isUnread()) {
			sf::Packet packet;
			std::string message;
			clients[0]->receive(packet);
			packet >> message;
			std::cout << message << std::endl;
		}*/
		
		for (int i = 0; i < clients.size(); ++i) {
			if (!clients[i]->isConnected()) {
				delete clients[i];
				clients.erase(clients.begin() + i);
				i--;
				std::cout << "Disconnected :^(" << std::endl;
			}
		}
		
		std::string message;
		std::cin >> message;
		sf::Packet packet;
		packet << message;
		for (auto it: clients) {
			it->send(packet);
		}
	}
	return 0;
}

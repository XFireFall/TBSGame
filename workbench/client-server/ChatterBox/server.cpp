#include "ChatterBox.hpp"

enum { ACK = 0, NOPE, FIGHT, FEED, MSG, UPD};

typedef struct { bool isCell; uint8_t owner, size, capacity; } cell_t;
typedef std::vector<std::vector<cell_t>> field_t;

std::ostream& operator<<(std::ostream& os, cell_t c)
{
	return os << (int) c.size;
}

std::ostream& operator<<(std::ostream& os, field_t const& field)
{
	int c = 1;
	for (auto v: field) {
		c = 1 - c;
		if (c)
			std::cout << ' ';
		for (auto i: v)
			std::cout << i << ' ';
		std::cout << std::endl;
	}
	return os;
}

void operator++(cell_t& cell)
{
	cell.size++;
}

void operator--(cell_t& cell)
{
	cell.size--;
}

std::ostream& operator<<(std::ostream& os, sf::Vector2u v)
{
	return os << (uint16_t) v.x << " " << (uint16_t) v.y;
}

/*
 * 0 = OK/ACK
 * 1 = FAIL/NO
 * 2 = FIGHT
 * 3 = FEED
 * 4 = UPD
 */

int main()
{
	field_t field(10);
	for (int i = 0; i < 10; ++i)
		field[i].resize(10);
	
	sf::TcpListener listener;
	while(sf::Socket::Done != listener.listen(5990))
		;
	std::cout << "Listerer binded to port " << 5990 << " successfully" << std::endl;
	
	std::vector<ChatterBox *> clients;
	sf::TcpSocket *new_socket = new sf::TcpSocket;
	
	uint16_t _curr_client = 0;
	while (1) {
		if (sf::Socket::Done == listener.accept(*new_socket)) {
			std::cout << "New client connected: " << new_socket->getRemoteAddress() << std::endl;
			
			clients.push_back(new ChatterBox(new_socket));
			
			listener.setBlocking(false);
			new_socket = new sf::TcpSocket;
		}
		
		bool check;
		sf::Packet packet;
		packet << (uint16_t) ACK;
		
		std::cout << "Send acknowledgement?" << std::endl;
		std::cin >> check;
		clients[_curr_client]->send(packet); // push
		std::cout << "Acknowledgement sent" << std::endl;
		
		while (1) {
			std::cout << field << std::endl;
			
			while (!clients[_curr_client]->isUnread())
				;
			clients[_curr_client]->receive(packet);
			std::cout << "Packet received" << std::endl;
			
			uint16_t msgtype;
			uint16_t sx,sy,dx,dy;
			packet >> msgtype >> sx >> sy >> dx >> dy;
			if (msgtype == ACK) {
				std::cout << "- this is acknowledgement" << std::endl;
				break;
			}
			if (msgtype == FIGHT) {
				std::cout << "- this is fight" << std::endl;
				--field[sy][sx];
				++field[dy][dx];
				sf::Packet p;
				p << (uint16_t) ACK;
				clients[_curr_client]->send(p); // approve
			}
		}
		
		while (1) {
			std::cout << field << std::endl;
			
			while (!clients[_curr_client]->isUnread())
				;
			clients[_curr_client]->receive(packet);
			std::cout << "Packet received" << std::endl;
			
			uint16_t msgtype;
			uint16_t x, y;
			packet >> msgtype >> x >> y;
			if (msgtype == ACK) {
				std::cout << "- this is cknowledgement" << std::endl;
				break;
			}
			if (msgtype == FEED) {
				std::cout << "i this is feed" << std::endl;
				++field[y][x];
				
				sf::Packet p;
				p << (uint16_t) ACK;
				clients[_curr_client]->send(p); // approve
				
				
				p = sf::Packet();
				p << (uint16_t) UPD << (uint16_t) x << (uint16_t) y << (uint16_t) field[y][x].size;
				for (int i = 0; i < clients.size(); ++i) {
					if (i == _curr_client)
						continue;
					clients[i]->send(p);
				}
			}
		}
		
		for (int i = 0; i < clients.size(); ++i) {
			if (!clients[i]->isConnected()) {
				delete clients[i];
				clients.erase(clients.begin() + i);
				i--;
				std::cout << "Disconnected :^(" << std::endl;
			}
		}
		
		if (sf::Socket::Done == listener.accept(*new_socket)) {
			std::cout << "New client connected: " << new_socket->getRemoteAddress() << std::endl;
			
			clients.push_back(new ChatterBox(new_socket));
			
			listener.setBlocking(false);
			new_socket = new sf::TcpSocket;
		}
		
		++_curr_client;
		if (_curr_client >= clients.size())
			_curr_client = 0;
	}
	
	std::cout << "Disconnecting..." << std::endl;
	for (int i = 0; i < clients.size(); ++i) {
		delete clients[i];
		std::cout << i << std::endl;
	}
	delete new_socket;
	return 0;
}

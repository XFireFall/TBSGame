#include "ChatterBox.hpp"

enum { ACK = 0, NOPE, FIGHT, FEED, MSG, UPD };

typedef struct { uint8_t owner, size, capacity; bool isCell() { return capacity != 0; } } cell_t;
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

//========================================================================

class Client: private ChatterBox {
private:
	bool yourTurn;
	
	
public:
	Client(sf::TcpSocket *socket_new)
		: ChatterBox(socket_new)
	{ }
	
	bool isConnected(void) const { return ChatterBox::isConnected(); }
	bool isMyTurn(void) const { return true; }
	
	void send_mail(std::string const& msg, uint16_t player_id = 0);
	std::pair<uint16_t, std::string> get_mail(void);
	bool isUnread(void) { return ChatterBox::isUnread(); }
	
	void send_fight(sf::Vector2u& attacker, sf::Vector2u& defender);
	void send_feed(sf::Vector2u& eater);
	void send_ack(void) {}
	
	void update(field_t& field) {}
	
	void fight(field_t& field, sf::Vector2u& attacker, sf::Vector2u& defender);
	void feed(field_t& field, sf::Vector2u& eater);
	void ack(void) {
		sf::Packet packet;
		packet << ACK;
		ChatterBox::send(packet);
	}
	void getAck(void)
	{
		sf::Packet packet;
		ChatterBox::receive(packet);
	}
	void getAck_waiting(void)
	{
		while(!isUnread())
			;
		getAck();
	}
	
	/* 0 => no update, no ACK; 1 => update, no ACK; 2 => ACK */
	uint16_t update_and_getAck(field_t& field)
	{
		if (!isUnread())
			return 0;
		while (isUnread()) {
			sf::Packet packet;
			uint16_t x, y;
			uint16_t value;
			uint16_t msgtype;
			
			ChatterBox::receive(packet);
			packet >> msgtype >> x >> y >> value;
			
			if (msgtype == ACK)
				return 2;
			if (msgtype == UPD) {
// 				std::cout << "UPD: " << pos.y << " & " << pos.x << std::endl;
				field[y][x].size = value;
			}
		}
		return 1;
	}
}; 

//========================================================================

void Client::send_mail(std::string const& msg, uint16_t player_id)
{
	sf::Packet packet;
	packet << (uint16_t) MSG << (uint16_t) player_id << msg;
	ChatterBox::send(packet);
}

std::pair<uint16_t, std::string> Client::get_mail(void)
{
	while (!ChatterBox::isUnread())
		;
	
	sf::Packet packet;
	ChatterBox::receive(packet);
	
	std::string msg;
	uint8_t player_id;
	uint8_t dummy;
	
	packet >> dummy >> player_id >> msg;
	return std::pair<int, std::string>(player_id, msg);
}




void Client::fight(field_t& field, sf::Vector2u& attacker, sf::Vector2u& defender)
{
	sf::Packet packet;
	packet << (uint16_t) FIGHT << (uint16_t) attacker.x << (uint16_t) attacker.y << (uint16_t) defender.x << (uint16_t) defender.y;
	ChatterBox::send(packet);
	
	while (!isUnread())
		;
	
	packet = sf::Packet();
	ChatterBox::receive(packet);
	uint16_t answer;
	packet >> answer;
	if (answer == ACK) {
		std::cout << "approved" << std::endl;
		--field[attacker.y][attacker.x];
		++field[defender.y][defender.x];
	} else {
		std::cout << "blocked" << std::endl;
	}
}

void Client::feed(field_t& field, sf::Vector2u& eater)
{
	sf::Packet packet;
	packet << (uint16_t) FEED << (uint16_t) eater.x << (uint16_t) eater.y;
	ChatterBox::send(packet);
	
	while (!isUnread())
		;
	
	packet = sf::Packet();
	ChatterBox::receive(packet);
	uint16_t answer;
	packet >> answer;
	if (answer == ACK) {
		std::cout << "approved" << std::endl;
		++field[eater.y][eater.x];
	} else {
		std::cout << "blocked" << std::endl;
	}
}

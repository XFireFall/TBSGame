#include "Client.hpp"

//========================================================================

Client::Client(sf::TcpSocket *socket_new)
	: ChatterBox(socket_new)
	, player_id(0)
{ }

TBSGame::Field Client::init(std::string const& nickname)
{
	sf::Packet packet;
	uint16_t count;
	TBSGame::Field field(1, 1);
	ChatterBox::receive_waiting(packet);
	
	packet >> player_id >> field >> count;
	for (uint16_t i = 0; i < count; ++i) {
		uint16_t id;
		std::string name;
		packet >> id >> name;
		std::cout << name << "(Player #" << id << std::endl;
	}
	
	packet = sf::Packet();
	packet << nickname;
	ChatterBox::send(packet);
	return field;
}


bool Client::fight(TBSGame::Field& field, sf::Vector2u& attacker, sf::Vector2u& defender)
{
	{
		if (!field.isValid(attacker)
			|| !field.isValid(defender)
		) return false;
		std::cout << "valid: OK" << std::endl;
		
		if ((abs(attacker.x - defender.x) > 1)
			|| (abs(attacker.y - defender.y) > 1)
		) return false;
		std::cout << "3x3: OK" << std::endl;
		
		if (attacker.y != defender.y) {
			if (((attacker.y % 2 == 0) && (defender.x > attacker.x))
				|| ((attacker.y % 2 == 1) && (defender.x < attacker.x))
			) return false;
			std::cout << "pos: y!=: OK" << std::endl;
		} else {
			if (attacker.x == defender.x)
				return false;
			std::cout << "pos: y==, x!=: OK" << std::endl;
		}
		
		if (field[attacker].owner != player_id)
			return false;
		std::cout << "owner_1: OK=" << player_id << std::endl;
		
		if (field[attacker].owner == field[defender].owner)
			return false;
		std::cout << "owner_2: OK" << std::endl;
		
		if (field[attacker].size == 0)
			return false;
		std::cout << "size: OK" << std::endl;
	}
	
	sf::Packet packet;
	packet << TBSGame::MsgType::FIGHT << attacker << defender;
	ChatterBox::send(packet);
	
	ChatterBox::receive_waiting(packet);
	TBSGame::MsgType mtype;
	packet >> mtype;
	if (mtype != TBSGame::MsgType::ACK)
		return false;
	
	sf::Vector2u v;
	TBSGame::Cell cell;
	for (int i = 0; i < 2; ++i) {
		ChatterBox::receive_waiting(packet);
		packet >> mtype >> v >> cell;
		field[v] = cell;
	}
	return true;
}

bool Client::feed(TBSGame::Field& field, sf::Vector2u& eater)
{
	{
		if (!field.isValid(eater))
			return false;
		
		if (field[eater].owner != player_id)
			return false;
		
		if ((field[eater].size == 0)
			|| (field[eater].size == field[eater].capacity)
		) return false;
	}
	
	sf::Packet packet;
	packet << TBSGame::MsgType::FEED << eater;
	ChatterBox::send(packet);
	
	ChatterBox::receive_waiting(packet);
	TBSGame::MsgType mtype;
	packet >> mtype;
	
	if (mtype != TBSGame::MsgType::ACK)
		return false;
	
	sf::Vector2u v;
	TBSGame::Cell cell;
	ChatterBox::receive_waiting(packet);
	packet >> mtype >> v >> cell;
	field[v] = cell;
	return true;
}

void Client::ack(void) {
	sf::Packet packet;
	packet << TBSGame::MsgType::ACK;
	ChatterBox::send(packet);
}

Client::UPDresult Client::update_and_getAck(TBSGame::Field& field)
{
	if (!isUnread())
		return UPDresult::NothingNew;
	
	sf::Packet packet;
	sf::Vector2u v;
	TBSGame::Cell cell;
	TBSGame::MsgType msgtype;
	
	while (isUnread()) {
		ChatterBox::receive(packet);
		packet >> msgtype >> v >> cell;
		std::cout << "mtype=" << msgtype << std::endl;
		
		if (msgtype == TBSGame::MsgType::ACK)
			return UPDresult::ACKrecieved;
		
		if (msgtype == TBSGame::MsgType::UPD)
			field[v] = cell;
	}
	return UPDresult::Updated;
}

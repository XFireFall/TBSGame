#pragma once

#include "GamePrimitives.hpp"
#include "ChatterBox.hpp"

//========================================================================

class Client: public ChatterBox {
private:
	uint16_t player_id;

public:
	enum class UPDresult { NothingNew = 0, Updated, ACKrecieved };
	
public:
	Client(sf::TcpSocket *socket_new);
	
	TBSGame::Field init(std::string const& nickname);
	
	bool fight(TBSGame::Field& field, sf::Vector2u& attacker, sf::Vector2u& defender);
	bool feed(TBSGame::Field& field, sf::Vector2u& eater);
	void ack(void);
	
	UPDresult update_and_getAck(TBSGame::Field& field);
}; 

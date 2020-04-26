#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

class ChatterBox {
private:
	sf::TcpSocket *_socket;
	bool _connected;
	
	struct RTmodule {
		sf::Thread thread;
		std::queue<sf::Packet> queue;
		sf::Mutex mutex;
		
		RTmodule(void(ChatterBox::*function)(), ChatterBox *object);
	} _rx, _tx;
	
private:
	void rcv_queue(void);
	void snd_queue(void);
	
public:
	ChatterBox(sf::TcpSocket *socket_new);
	~ChatterBox(void);
	
	void send(sf::Packet const &packet);
	void receive(sf::Packet &packet);
	void receive_waiting(sf::Packet &packet);
	
	bool isUnread(void) const;
	bool isConnected(void) const;
};

//================================================================

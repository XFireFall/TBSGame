#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

class Client {
private:
	unsigned int player_id;
	std::string nickname;
	
	sf::TcpSocket *socket;
	bool connected;
	
	sf::Thread thread_rcv, thread_snd;
	std::queue<sf::Packet> queue_rcv, queue_snd;
	sf::Mutex mutex_rcv, mutex_snd;
	
private:
	void rcv_queue(void);
	void snd_queue(void);
	
public:
	Client(sf::TcpSocket *socket_new, unsigned int player_id_new);
	~Client(void);
	
	void send(sf::Packet const &packet);
	void receive(sf::Packet &packet);
	
	bool isUnread(void) const;
	bool isConnected(void) const;
};

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
	
	sf::Thread thread_rcv;
	std::queue<sf::Packet> queue_rcv;
	sf::Mutex mutex_rcv;
	bool unread;
	
	sf::Thread thread_snd;
	std::queue<sf::Packet> queue_snd;
	sf::Mutex mutex_snd;
	
private:
	void rcv_queue(void);
	void snd_queue(void);
	
public:
	Client(sf::TcpSocket *socket_new);
	~Client(void);
	
	void send(sf::Packet const &packet);
	void receive(sf::Packet &packet);
	sf::Packet receive_2(void);
	
	bool isUnread(void) { return unread; }
	bool isConnected(void) { return connected; }
};

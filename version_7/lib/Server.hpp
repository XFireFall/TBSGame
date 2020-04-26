#pragma once

#include <iostream>
#include <vector>
#include <queue>

#include "prim/Prim_common.hpp"
#include "prim/Prim_game.hpp"
#include "prim/Prim_network.hpp"


class Server {
private:
	struct Client {
	private:
		static std::priority_queue<uint16_t, std::vector<uint16_t>, std::greater<uint16_t>> _free_ids;
		static int _count;
		
	public:
		sf::TcpSocket *_socket;
		gm::player_data _pd;
		
		Client(sf::TcpSocket *socket);
		~Client(void);
		
		void init(gm::Field const& field, std::vector<Client*> const& clients);
	};
	
private:
	gm::Field _field;
	sf::Thread _thread;
	sf::TcpListener _listener;
	sf::SocketSelector _selector;
	std::vector<Client*> _clients;
	uint16_t _current_pos;
	
private:
	void broadcast(sf::Packet& p, int except = -1);
	void update_cell(sf::Vector2u const& v, int except = -1);
	void next_turn(void);
	
	void process_packet(uint16_t pos, sf::Packet& p);
	
	sf::TcpSocket* new_player(void);
	void remove_player(uint16_t pos);
	
	void thread_routine(void);
	
public:
	enum ex { ListenerDisconnected };
	
	Server(int height, int width);
	~Server(void);
	
	bool busy(void) const;
};

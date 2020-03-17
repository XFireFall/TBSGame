#include "../ChatterBox.hpp"
#include "../GamePrimitives.hpp"

int main()
{
	TBSGame::Field field(10, 10);
	
	sf::TcpListener listener;
	while(sf::Socket::Done != listener.listen(5990))
		;
	std::cout << "Listerer binded to port " << 5990 << " successfully" << std::endl;
	
	std::vector<ChatterBox *> clients;
	std::vector<std::string> nicknames;
	std::vector<uint16_t> player_ids;
	sf::TcpSocket *new_socket = new sf::TcpSocket;
	std::queue<uint16_t> free_ids;
	
	uint16_t _curr_client = 0;
	while (1) {
		while (sf::Socket::Done == listener.accept(*new_socket)) {
			
			uint16_t id = 0;
			if (!free_ids.empty()) {
				id = free_ids.front();
				free_ids.pop();
			} else
				id = clients.size() + 1;
			
			std::cout << "New client #" << id << " connected: " << new_socket->getRemoteAddress() << std::endl;
			
			clients.push_back(new ChatterBox(new_socket));
			player_ids.push_back(id);
			
			for (int i = 1, nfound = 1; nfound && (i < field.getHeight()); ++i)
				for (int j = 1; j < field.getWidth(); ++j) {
					if (field[i][j] == 0) {
						std::cout << "Nest: (" << i << ", " << j << ")" << std::endl;
						field[i][j] = TBSGame::Cell(1, id);
						nfound = 0;
						break;
					} else {
						std::cout << "Bad: (" << i << ", " << j << ") " << field[i][j] << std::endl;
					}
				}
			
			sf::Packet packet;
			packet << id << field << (uint16_t) clients.size() - 1;
			for (int i = 0; i < clients.size() - 1; ++i)
				packet << player_ids[i] << nicknames[i];
			clients[clients.size() - 1]->send(packet);
			
			std::cout << "Waiting for nickname..." << std::endl;
			
			clients[clients.size() - 1]->receive_waiting(packet);
			std::string nick;
			packet >> nick;
			nicknames.push_back(nick);
			std::cout << "Name: " << nick << std::endl;
			
			listener.setBlocking(false);
			new_socket = new sf::TcpSocket;
		}
		
		bool check;
		sf::Packet packet;
		packet << TBSGame::MsgType::ACK;
		
		std::cout << "Send acknowledgement?" << std::endl;
		std::cin >> check;
		clients[_curr_client]->send(packet);
		std::cout << "Acknowledgement sent" << std::endl;
		
		while (1) {
			TBSGame::MsgType mtype;
			sf::Vector2u src, dest;
			
			std::cout << field << std::endl;
			
			std::cout << "Waiting for a packet..." << std::endl;
			clients[_curr_client]->receive_waiting(packet);
			std::cout << "Packet received" << std::endl;
			packet >> mtype >> src >> dest;
			
			if (mtype == TBSGame::MsgType::ACK) {
				std::cout << "- this is acknowledgement" << std::endl;
				break;
			}
			if (mtype == TBSGame::MsgType::FIGHT) {
				std::cout << "- this is fight" << std::endl;
				--field[src];
				if (field[dest] == 0)
					field[dest].owner = player_ids[_curr_client];
				++field[dest];
				
				
				sf::Packet p;
				p << TBSGame::MsgType::ACK;
				clients[_curr_client]->send(p);
				
				p = sf::Packet();
				p << TBSGame::MsgType::UPD << src << field[src];
				for (int i = 0; i < clients.size(); ++i) {
					clients[i]->send(p);
				}
				
				p = sf::Packet();
				p << TBSGame::MsgType::UPD << dest << field[dest];
				for (int i = 0; i < clients.size(); ++i) {
					clients[i]->send(p);
				}
			}
		}
		
		std::cout << "Now for feeding" << std::endl;
		
		while (1) {
			TBSGame::MsgType mtype;
			sf::Vector2u v;
			
			std::cout << field << std::endl;
			
			std::cout << "Waiting for a packet..." << std::endl;
			clients[_curr_client]->receive_waiting(packet);
			std::cout << "Packet received" << std::endl;
			packet >> mtype >> v;
			
			if (mtype == TBSGame::MsgType::ACK) {
				std::cout << "- this is cknowledgement" << std::endl;
				break;
			}
			if (mtype == TBSGame::MsgType::FEED) {
				std::cout << "- this is feed" << std::endl;
				++field[v];
				
				sf::Packet p;
				p << TBSGame::MsgType::ACK;
				clients[_curr_client]->send(p);
				
				p = sf::Packet();
				p << TBSGame::MsgType::UPD << v << field[v];
				for (int i = 0; i < clients.size(); ++i) {
					clients[i]->send(p);
				}
			}
		}
		
		for (int i = 0; i < clients.size(); ++i) {
			if (!clients[i]->isConnected()) {
				free_ids.push(player_ids[i]);
				delete clients[i];
				clients.erase(clients.begin() + i);
				player_ids.erase(player_ids.begin() + i);
				nicknames.erase(nicknames.begin() + i);
				i--;
				std::cout << "Disconnected :^(" << std::endl;
			}
		}
		
		if (sf::Socket::Done == listener.accept(*new_socket)) {
			uint16_t id = 0;
			if (!free_ids.empty()) {
				id = free_ids.front();
				free_ids.pop();
			} else
				id = clients.size() + 1;
			
			std::cout << "New client #" << id << " connected: " << new_socket->getRemoteAddress() << std::endl;
			
			clients.push_back(new ChatterBox(new_socket));
			player_ids.push_back(id);
			
			for (int i = 1, nfound = 1; nfound && (i < field.getHeight()); ++i)
				for (int j = 1; j < field.getWidth(); ++j) {
					if (field[i][j] == 0) {
						std::cout << "Nest: (" << i << ", " << j << ")" << std::endl;
						field[i][j] = TBSGame::Cell(1, id);
						nfound = 0;
						break;
					} else {
						std::cout << "Bad: (" << i << ", " << j << ") " << field[i][j] << std::endl;
					}
				}
			
			sf::Packet packet;
			packet << id << field << (uint16_t) clients.size() - 1;
			for (int i = 0; i < clients.size() - 1; ++i)
				packet << player_ids[i] << nicknames[i];
			clients[clients.size() - 1]->send(packet);
			
			std::cout << "Waiting for nickname..." << std::endl;
			
			clients[clients.size() - 1]->receive_waiting(packet);
			std::string nick;
			packet >> nick;
			nicknames.push_back(nick);
			std::cout << "Name: " << nick << std::endl;
			
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
 

#include "Server.hpp"

Server::Client::Client(sf::TcpSocket *socket)
	: _socket(socket)
{
	if (_free_ids.size() != 0) {
		_pd.id = _free_ids.top();
		_free_ids.pop();
	} else
		_pd.id = _count + 1;
	++_count;
}

//==============================

Server::Client::~Client(void)
{
	--_count;
	_free_ids.push(_pd.id);
	delete _socket;
}

//==============================

void Server::Client::init(gm::Field const& field, std::vector<Client*> const& clients)
{
	sf::Packet p;
	
	_socket->receive(p);
	p >> _pd.nickname;
	
	p = sf::Packet();
	p << _pd.id << field << (int) clients.size();
	for (auto it: clients)
		p << it->_pd;
	_socket->send(p);
	
	std::clog << _pd <<" initialized" << std::endl;
}

//==============================

void Server::broadcast(sf::Packet& p, int except)
{
	for (int i = 0; i < _clients.size(); ++i)
		if (i != except)
			_clients[i]->_socket->send(p);
}

//==============================

void Server::update_cell(sf::Vector2u const& v, int except)
{
	sf::Packet p;
	p << net::msg_t::UPD << net::upd_t::CELL_UPDATE << v << _field[v];
	broadcast(p, except);
}

//==============================

void Server::next_turn(void)
{
	++_current_pos;
	if (_current_pos >= _clients.size())
		_current_pos = 0;
	
	sf::Packet p;
	p << net::msg_t::TURN << _clients[_current_pos]->_pd;
	broadcast(p);
}

//==============================

void Server::process_packet(uint16_t pos, sf::Packet& p)
{
	net::msg_t mtype;
	sf::Vector2u who, whom;
	
	p >> mtype;
	switch (mtype) {
		case net::msg_t::ATTACK:
			p >> who >> whom;
			_field.attack(who, whom);
			update_cell(who);
			update_cell(whom);
			std::clog << "Server sent broadcast message" << std::endl;
			break;
			
		case net::msg_t::FEED:
			p >> whom;
			_field.feed(whom);
			update_cell(whom);
			std::clog << "Server sent broadcast message" << std::endl;
			break;
			
		case net::msg_t::TURN:
			next_turn();
			break;
			
		default:
			std::clog << "ERROR: unknown message structure with type " << mtype << std::endl;
			break;
	}
}

//==============================

sf::TcpSocket* Server::new_player(void)
{
	sf::TcpSocket *new_socket = new sf::TcpSocket;
	if (sf::TcpListener::Done != _listener.accept(*new_socket)) {
		std::cerr << "Connection failed" << std::endl;
		delete new_socket;
		return nullptr;
	}
	
	Client* cli = new Client{new_socket};
	
	sf::Vector2u nest = _field.nest(cli->_pd.id);
	
	cli->init(_field, _clients);
	
	sf::Packet p;
	p << net::msg_t::UPD << net::upd_t::CONN_PLAYER << cli->_pd;
	broadcast(p);
	
	update_cell(nest);
	
	_clients.push_back(cli);
	
	p = sf::Packet{};
	p << net::msg_t::TURN << _clients[_current_pos]->_pd;
	new_socket->send(p);
	
	std::clog << cli->_pd << " connected" << std::endl;
	return new_socket;
}

//==============================

void Server::remove_player(uint16_t pos)
{
	sf::Packet p;
	p << net::msg_t::UPD << net::upd_t::DISC_PLAYER << _clients[pos]->_pd;
	broadcast(p, pos);
	
	std::vector<sf::Vector2u> discarded = _field.discard(_clients[pos]->_pd.id);
	for (auto it: discarded)
		update_cell(it, pos);
	
	delete _clients[pos];
	_clients.erase(_clients.begin() + pos);
}

//==============================

void Server::thread_routine(void)
{
	while (busy()) {
		std::clog << "Server waiting for incoming messages" << std::endl;
		_selector.wait();
		
		if (_selector.isReady(_listener)) {
			if (sf::TcpSocket *sock = new_player())
				_selector.add(*sock);
		} else {
			for (int i = 0; i < _clients.size(); ++i) {
				sf::TcpSocket& socket = *_clients[i]->_socket;
				if (!_selector.isReady(socket))
					continue;
				
				sf::Packet p;
				if (sf::Socket::Done != socket.receive(p)) {
					std::clog << _clients[i]->_pd << " disconnected" << std::endl;
					_selector.remove(socket);
					remove_player(i);
					if (_current_pos == i)
						next_turn();
					i--;
				} else {
					std::clog << "Incoming message from " << _clients[i]->_pd << std::endl;
					process_packet(i, p);
				}
			}
		}
	}
}

//==============================

Server::Server(int height, int width)
	: _field(height, width)
	, _thread(&Server::thread_routine, this)
	, _current_pos(0)
{
	if (sf::Socket::Done != _listener.listen(net::gameport))
		throw ListenerDisconnected;
	
	_selector.add(_listener);
	
	std::clog << "Server waiting for the first player" << std::endl;
	while (1)
		if (sf::TcpSocket *sock = new_player()) {
			_selector.add(*sock);
			break;
		}
	
	_thread.launch();
}

//==============================

Server::~Server(void)
{
	_selector.clear();
	
	while (_clients.size() > 0) {
		delete _clients[0];
		_clients.erase(_clients.begin());
	}
}

//==============================

bool Server::busy(void) const
{
	return _clients.size() != 0;
}

//==============================

typeof(Server::Client::_free_ids) Server::Client::_free_ids;
typeof(Server::Client::_count) Server::Client::_count = 0;

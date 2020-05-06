#include "Client.hpp"

#include "../primitives/Prim_network.hpp"
#include "../primitives/Prim_system.hpp"

void Client::thread_receive_routine(void)
{
	net::msg_t mtype;
	net::upd_t utype;
	gm::player_data player;
	sf::Vector2u v;
	gm::Cell cell;
	std::string msg;
	bool isWinner;
	
	sf::SocketSelector selector;
	selector.add(static_cast<sf::TcpSocket&>(*this));
	
	while (1) {
		if (clog_enabled) std::clog << "Client waiting for messages from server" << std::endl;
		do {
			selector.wait(sf::milliseconds(1000));
		} while (!selector.isReady(static_cast<sf::TcpSocket&>(*this)) && isConnected());
		
		sf::Packet p;
		if (sf::Socket::Done != receive(p))
			return;
		
		p >> mtype;
		switch (mtype) {
			case net::msg_t::UPD:
				p >> utype;
				switch (utype) {
					case net::upd_t::DISC_PLAYER:
						p >> player;
						for (int i = 0; i < _players.size(); ++i)
							if (_players[i].id == player.id) {
								_players.erase(_players.begin() + i);
								break;
							}
						if (clog_enabled) std::clog << player << " disconnected" << std::endl;
						break;
						
					case net::upd_t::CONN_PLAYER:
						p >> player;
						_players.push_back(player);
						if (clog_enabled) std::clog << player << " connected" << std::endl;
						break;
						
					case net::upd_t::CELL_UPDATE:
						p >> v >> cell;
						_field[v] = cell;
						if (clog_enabled) std::clog << "Cell updated: field" << v << "=" << cell << std::endl;
						
						if (clog_enabled) std::clog << _field << std::endl;
						break;
						
					default:
						if (cerr_enabled) std::cerr << "Unexpected update type " << utype << std::endl;
						break;
				} // switch utype
				break;
				
			case net::msg_t::GMOVER:
				p >> isWinner;
				_phase = (isWinner ? gm::WIN : gm::LOSE);
				if (clog_enabled) std::clog << (isWinner ? "You won!" : "You lose!") << std::endl;
				if (isWinner)
					_msg_queue.push_back("You win!");
				else
					_msg_queue.push_back("You lose!");
				
				break;
				
			case net::msg_t::TURN:
				p >> player;
				if (clog_enabled) std::clog << "It's " << player << "'s turn" << std::endl;
				_current_player = player;
				
				_msg_queue.push_back(std::string("It's ") + player.nickname + "'s turn");
				if (_current_player.id == _pd.id) {
					if (_phase != gm::LOSE) {
						if (clog_enabled) std::clog << " - It's your turn" << std::endl;
						_msg_queue.push_back("Now attack other cells!");
						_phase = gm::Phase::ATTACK;
					} else {
						p = sf::Packet{};
						p << net::msg_t::TURN;
						send(p); // ATTACK -> FEED -> WAIT
					}
				}
				break;
				
			case net::msg_t::MSG:
				p >> msg;
				_msg_queue.push_back(msg);
				std::cout << msg << std::endl;
				break;
				
			default:
				if (cerr_enabled) std::cerr << "Unexpected message type " << mtype << std::endl;
				break;
		} // switch mtype
	} // while 1
}

//================================

void Client::check_state(void) const
{
	if (!isConnected()) {
		std::cerr << "X: disc" << std::endl;
		throw DISCONNECTED;
	}
	if (_phase == gm::WIN) {
		std::cerr << "X: win" << std::endl;
		throw GMOVR_WIN;
	}
	if (_phase == gm::LOSE) {
		std::cerr << "X: lose" << std::endl;
		throw GMOVR_LOSE;
	}
}

//================================

Client::Client(std::string const& nickname)
	: _thread_receive(&Client::thread_receive_routine, this)
	, _pd(nickname)
	, _phase(gm::WAIT)
	, _food(0)
	, clog_enabled(false)
	, cerr_enabled(true)
{
	
}

//================================

Client::~Client(void)
{
	disconnect();
}

//================================

void Client::setNickname(std::string const& nickname)
{
	_pd.nickname = nickname;
}

//================================

bool Client::connect(sf::IpAddress const& ip)
{
	disconnect();
	
	if (sf::TcpSocket::Done != sf::TcpSocket::connect(ip, net::gameport)) {
		if (cerr_enabled) std::cerr << "Connection failed: " << ip << std::endl;
		return false;
	}
	if (clog_enabled) std::clog << "Connection success: " << ip << std::endl;
	
	/*================*/
	
	sf::Packet p;
	p << _pd.nickname;
	send(p);
	
	int players_count;
	receive(p);
	p >> _pd.id >> _field >> players_count;
	_players.resize(players_count);
	for (auto& it: _players)
		p >> it;
	
	if (clog_enabled) std::clog << "Initialization success" << std::endl;
	
	/*================*/
	
	_thread_receive.launch();
	return true;
}

//================================

bool Client::host(int height, int width)
{
	sys::Mutex mtx(sys::Mutex::CREAT);
	
	switch (::fork()) {
		case -1:
			mtx.change_mode(sys::Mutex::DELETE);
			return false;
			
		case 0:
			execlp("./host", "./host", std::to_string(height).c_str(), std::to_string(width).c_str(), "0", NULL);
			mtx.change_mode(sys::Mutex::DELETE);
			exit(1);
			
		default:
			break;
	}
	
	mtx.lock();
	mtx.unlock();
	
	if (clog_enabled) std::clog << "Host success" << std::endl;
	
	return connect("127.0.0.1");
}

//================================

void Client::disconnect(void)
{
	if (!isConnected())
		return;
	
	sf::TcpSocket::disconnect();
	_thread_receive.wait();
	_phase = gm::WAIT;
	_food = 0;
	_msg_queue.resize(0);
	_players.resize(0);
	
	if (clog_enabled) std::clog << "Disconnection success" << std::endl;
}

//================================

bool Client::isConnected(void) const
{
	return getRemotePort() != 0;
}

//================================

bool Client::attack(sf::Vector2u const& who, sf::Vector2u const& whom)
{
	check_state();
	
	if ((_phase != gm::ATTACK) || (!_field.may_attack(_pd.id, who, whom)))
		return false;
	
	sf::Packet p;
	p << net::msg_t::ATTACK << who << whom;
	send(p);
	return true;
}

//================================

bool Client::feed(sf::Vector2u const& whom)
{
	check_state();
	
	if ((_phase != gm::FEED) || !_field.may_feed(_pd.id, whom) || (_food == 0))
		return false;
	
	sf::Packet p;
	p << net::msg_t::FEED << whom;
	send(p);
	--_food;
	_msg_queue.push_back("Food left: " + std::to_string(_food));
	return true;
}

//================================

void Client::msg_send(std::string const& msg)
{
	check_state();
	
	sf::Packet p;
	p << net::msg_t::MSG << msg;
	send(p);
}

//================================

uint16_t Client::food_left(void) const
{
	check_state();
	
	return _food;
}

//================================

gm::Phase Client::nextPhase(void)
{
	check_state();
	
	if (_phase == gm::WAIT) {
		_msg_queue.push_back("Wait for your turn!");
		return _phase;
	}
	
	if (_phase == gm::ATTACK) {
		_phase = gm::FEED;
		_food = _field.count(_pd.id);
		_msg_queue.push_back("Now feed your cells!");
	} else if (_phase == gm::FEED) {
		_phase = gm::WAIT;
		_food = 0;
		
		sf::Packet p;
		p << net::msg_t::TURN;
		send(p);
		_msg_queue.push_back("Now wait for your turn!");
	}
	return _phase;
}

//================================

gm::Phase Client::getPhase(void) const
{
	check_state();
	
	return _phase;
}

//================================

gm::player_data Client::whoAmI(void) const
{
	check_state();
	
	return _pd;
}

gm::player_data Client::whoIsPlaying(void) const
{
	check_state();
	
	return _current_player;
}

//================================

gm::Field const& Client::field(void) const
{
// 	check_state();
	
	return _field;
}

//================================

std::vector<gm::player_data> const& Client::players(void) const
{
// 	check_state();
	
	return _players;
}

//================================

std::vector<std::string> const& Client::msg_queue(void) const
{
// 	check_state();
	
	return _msg_queue;
}

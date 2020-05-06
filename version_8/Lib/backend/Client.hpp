#pragma once

#include <iostream>
#include <vector>
#include <queue>

#include "../primitives/Prim_game.hpp"
#include "../primitives/Prim_network.hpp"


class Client: private sf::TcpSocket {
private:
	sf::Thread _thread_receive;
	gm::Field _field;
	std::vector<gm::player_data> _players;
	std::vector<std::string> _msg_queue;
	gm::player_data _pd, _current_player;
	gm::Phase _phase;
	uint16_t _food;
	
	bool clog_enabled, cerr_enabled;
	
private:
	void thread_receive_routine(void);
	void check_state(void) const;
	
public:
	enum ex { DISCONNECTED, GMOVR_WIN, GMOVR_LOSE };
	
public:
	Client(std::string const& nickname = "Player");
	~Client(void);
	
	/* Rename yourself*/
	void setNickname(std::string const& nickname);
	
	/* Initiating joining to a game server */
	bool connect(sf::IpAddress const& ip);
	
	/* Make own server & connect to it */
	bool host(int height, int width);
	
	/* Disconnect from the current server
	 * WARNING: server may not stop though */
	void disconnect(void);
	
	/* Checks if the connection is stable */
	bool isConnected(void) const;
	
public: // WARNING: may throw Client::ex
	/* Attacking command: if it is
	 * unavailable, returns false and does nothing */
	bool attack(sf::Vector2u const& who, sf::Vector2u const& whom);
	
	/* Feed command: if it is
	 * unavailable, returns false and does nothing */
	bool feed(sf::Vector2u const& whom);
	
	/* Send message via common chat */
	void msg_send(std::string const& msg);
	
	/* Checks how many food is left
	 * to feed cells
	 * When called not at FEED phase returns 0 */
	uint16_t food_left(void) const;
	
	/* Attempts to turn to next game phase
	 * Makes no sense when the phase is WAIT */
	gm::Phase nextPhase(void);
	
	/* Returns current game phase
	 * Use it to learn if it's your turn to play */
	gm::Phase getPhase(void) const;
	
	/* Tells you who you are */
	gm::player_data whoAmI(void) const;
	
	/* Indicates who's current turn is */
	gm::player_data whoIsPlaying(void) const;
	
	/* Current field picture to be printed */
	gm::Field const& field(void) const;
	
	/* Current enemies list to be printed */
	std::vector<gm::player_data> const& players(void) const;
	
	/* Get message history */
	std::vector<std::string> const& msg_queue(void) const;
};

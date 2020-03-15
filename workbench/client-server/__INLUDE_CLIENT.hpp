
typedef struct { bool isCell; uint8_t owner, size, capacity; } cell_t;
typedef std::vector<std::vector<cell_t>> field_t;

class Client {
public:
	Client(void)
	{ }
	
	bool isConnected(void) const {
		return true;
	}
	
	void isMyTurn(void) const {
		return true;
	}
	
	void send_mail(std::string const& msg, uint8_t player_id = 0) {}
	std::pair<uint8_t, std::string> std::string get_mail(void) {}
	
	void send_fight(sf::Vector2u& attacker, sf::Vector2u& defender) {}
	void send_feed((sf::Vector2u& eater) {}
	void send_ack(void) {}
	
	void update(field_t *field) {}
};

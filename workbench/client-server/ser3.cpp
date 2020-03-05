#include <iostream>
#include <vector>
#include <queue>
#include <SFML/Network.hpp>

class Client {
private:
	unsigned int player_id;
	std::string nickname;
	sf::TcpSocket *socket;
	sf::Thread thread_rcv, thread_snd;
	std::queue<sf::Packet> msq_rcv, msq_snd;
	bool isSending;
	
private:
	void rcv_queue(void);
	void snd_queue(void);
	
public:
	Client(sf::TcpSocket *socket_new);
	~Client(void);
	
	void send(sf::Packet const &msg);
	sf::Packet& receive(void);
};

Client::Client(sf::TcpSocket *socket_new)
	: socket(socket_new)
	, thread_rcv(&Client::rcv_queue, this)
	, thread_snd(&Client::snd_queue, this)
{
	socket->setBlocking(true);
	thread_rcv.launch();
}

Client::~Client(void)
{
	thread_rcv.terminate();
	thread_snd.terminate();
}

void Client::rcv_queue(void)
{
	sf::Packet packet;
	while (1) {
		std::cout << "Waiting for a packet" << std::endl;
		socket->receive(packet);
		msq_rcv.push(packet);
		
		std::string message;
		packet >> message;
		std::cout << message << std::endl;
	}
}

void Client::snd_queue(void)
{
	sf::Packet packet;
	while (!msq_snd.empty()) {
		socket->send(msq_snd.front());
		msq_snd.pop();
	}
	isSending = false;
}

int main()
{
	sf::TcpListener listener;
	while(sf::Socket::Done != listener.listen(5990))
		;
	std::cout << "Listerer binded to port " << 5990 << " successfully" << std::endl;
	
	std::vector<Client *> clients;
	sf::TcpSocket *p_socket = new sf::TcpSocket;
	while (1) {
		if (sf::Socket::Done == listener.accept(*p_socket)) {
			std::cout << "New client connected: " << p_socket->getRemoteAddress() << std::endl;
			
			clients.push_back(new Client(p_socket));
			
			listener.setBlocking(false);
			p_socket = new sf::TcpSocket;
		}
	}
	return 0;
}

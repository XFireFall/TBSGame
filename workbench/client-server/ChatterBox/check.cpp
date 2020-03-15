#include <iostream>

class ChatterBox {
private:
	sf::TcpSocket *socket;
	bool connected;
	  
	sf::Thread thread_rcv, thread_snd;
	std::queue<sf::Packet> queue_rcv, queue_snd;
	sf::Mutex mutex_rcv, mutex_snd;
	
private:
	void rcv_queue(void);
	void snd_queue(void);
	
public:
	ChatterBox(sf::TcpSocket *socket_new);
	~ChatterBox(void);
	
	void send(sf::Packet const &packet);
	void receive(sf::Packet &packet);
	
	bool isUnread(void) const;
	bool isConnected(void) const;
};

//================================================================

ChatterBox::ChatterBox(sf::TcpSocket *socket_new, unsigned int player_id_new)
	: socket(socket_new)
	, connected(true)
	, thread_rcv(&ChatterBox::rcv_queue, this)
	, thread_snd(&ChatterBox::snd_queue, this)
{
	socket->setBlocking(true);
	thread_rcv.launch();
}

ChatterBox::~ChatterBox(void)
{
	socket->disconnect();
	socket->setBlocking(false);
	thread_rcv.wait();
	thread_snd.wait();
	delete socket;
}

//===============================================================

/*
 * Thread function that constantly scans socket for incoming packets
 * The received packet is stored in the appropriate queue and the appropriate flag is updated
 */
void Client::rcv_queue(void)
{
	sf::Packet packet;
	while (1) {
		if (sf::Socket::Done != socket->receive(packet)) {
			connected = false;
			return;
		}
		
		sf::Lock lock(mutex_rcv);
		queue_rcv.push(packet);
	}
}

/*
 * Returns an appropriate packet from the incoming queue
 * It may block for a little if there's a packet being received
 * If the queue is empty the packet is left untouched, so check if there are incoming packets before calling
 */
void ChatterBox::receive(sf::Packet &packet)
{
	sf::Lock lock(mutex_rcv);
	if (queue_rcv.empty())
		return;
	packet = queue_rcv.front();
	queue_rcv.pop();
}

//===============================================================

void ChatterBox::snd_queue(void)
{
	while (!queue_snd.empty()) {
		if (sf::Socket::Done != socket->send(queue_snd.front())) {
			connected = false;
			return;
		}
		
		sf::Lock lock(mutex_snd);
		queue_snd.pop();
	}
}

void ChatterBox::send(sf::Packet const &packet)
{
	sf::Lock lock(mutex_snd);
	queue_snd.push(packet);
	if (1 == queue_snd.size())
		thread_snd.launch();
}

//===============================================================

bool ChatterBox::isUnread(void) const { return !queue_rcv.empty(); }
bool ChatterBox::isConnected(void) const { return connected; }

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
			
			clients.push_back(new Client(p_socket, clients.size()));
			
			listener.setBlocking(false);
			p_socket = new sf::TcpSocket;
		}
		
		/*if (clients[0]->isUnread()) {
			sf::Packet packet;
			std::string message;
			clients[0]->receive(packet);
			packet >> message;
			std::cout << message << std::endl;
		}*/
		
		for (int i = 0; i < clients.size(); ++i) {
			if (!clients[i]->isConnected()) {
				delete clients[i];
				clients.erase(clients.begin() + i);
				i--;
				std::cout << "Disconnected :^(" << std::endl;
			}
		}
		
break;
		
		std::string message;
		std::cin >> message;
		sf::Packet packet;
		packet << message;
		for (auto it: clients) {
			it->send(packet);
		}
	}
	
	
	std::cout << "Disconnecting..." << std::endl;
	for (int i = 0; i < clients.size(); ++i) {
		delete clients[i];
		std::cout << i << std::endl;
	}
	delete p_socket;
	return 0;
}

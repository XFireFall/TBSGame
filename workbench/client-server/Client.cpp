#include "Client.hpp"

Client::Client(sf::TcpSocket *socket_new)
	: socket(socket_new)
	, thread_rcv(&Client::rcv_queue, this)
	, thread_snd(&Client::snd_queue, this)
	, connected(true)
{
	socket->setBlocking(true);
	thread_rcv.launch();
}

Client::~Client(void)
{
	thread_rcv.terminate();
	thread_snd.terminate();
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
// 		std::cout << "THRD: Waiting for a packet" << std::endl;
		
		if (sf::Socket::Disconnected == socket->receive(packet)) {
			connected = false;
			return;
		}
		
		sf::Lock lock(mutex_rcv);
		queue_rcv.push(packet);
		unread = true;
		
		
// 		std::string message;
// 		packet >> message;
// 		std::cout << "THRD: New message: " << message << std::endl;
	}
}

/*
 * Returns an appropriate packet from the incoming queue
 * It may block for a little if there's a packet being received
 * If the queue is empty the exception is thrown, so check if there are incoming packets before calling this
 */
void Client::receive(sf::Packet &packet)
{
	sf::Lock lock(mutex_rcv);
	packet = queue_rcv.front();
	queue_rcv.pop();
	unread = queue_rcv.empty();
}

sf::Packet Client::receive_2(void)
{
	sf::Lock lock(mutex_rcv);
	sf::Packet packet = queue_rcv.front();
	queue_rcv.pop();
	unread = queue_rcv.empty();
	return packet;
}

//===============================================================

void Client::snd_queue(void)
{
	while (!queue_snd.empty()) {
		if (sf::Socket::Disconnected == socket->send(queue_snd.front())) {
			connected = false;
			return;
		}
		
		sf::Lock lock(mutex_snd);
		queue_snd.pop();
	}
}

void Client::send(sf::Packet const &packet)
{
	sf::Lock lock(mutex_snd);
	queue_snd.push(packet);
	if (1 == queue_snd.size())
		thread_snd.launch();
}

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
 * If the queue is empty the exception is thrown, so check if there are incoming packets before calling this
 */
void Client::receive(sf::Packet &packet)
{
	sf::Lock lock(mutex_rcv);
	packet = queue_rcv.front();
	queue_rcv.pop();
}

//===============================================================

void Client::snd_queue(void)
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

void Client::send(sf::Packet const &packet)
{
	sf::Lock lock(mutex_snd);
	queue_snd.push(packet);
	if (1 == queue_snd.size()) {
		thread_snd.launch();
	}
}

//===============================================================

bool Client::isUnread(void) const
{
	return !queue_rcv.empty();
}

bool Client::isConnected(void) const
{
	return connected;
}

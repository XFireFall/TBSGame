#include "ChatterBox.hpp"

ChatterBox::RTmodule::RTmodule(void(ChatterBox::*function)(), ChatterBox *object)
	: thread(function, object)
{ }

//================================================================

ChatterBox::ChatterBox(sf::TcpSocket *socket_new)
	: _socket(socket_new)
	, _connected(true)
	, _rx(&ChatterBox::rcv_queue, this)
	, _tx(&ChatterBox::snd_queue, this)
{
	_socket->setBlocking(true);
	_rx.thread.launch();
}

ChatterBox::~ChatterBox(void)
{
	_socket->disconnect();
	_socket->setBlocking(false);
	_rx.thread.wait();
	_tx.thread.wait();
	delete _socket;
}

//===============================================================

/*
 * Thread function that constantly scans socket for incoming packets
 * The received packet is stored in the appropriate queue and the appropriate flag is updated
 */
void ChatterBox::rcv_queue(void)
{
	sf::Packet packet;
	while (1) {
		if (sf::Socket::Done != _socket->receive(packet)) {
			_connected = false;
			return;
		}
		
		sf::Lock lock(_rx.mutex);
		_rx.queue.push(packet);
	}
}

/*
 * Returns an appropriate packet from the incoming queue
 * It may block for a little if there's a packet being received
 * If the queue is empty the packet is left untouched, so check if there are incoming packets before calling
 */
void ChatterBox::receive(sf::Packet &packet)
{
	sf::Lock lock(_rx.mutex);
	if (_rx.queue.empty())
		return;
	packet = _rx.queue.front();
	_rx.queue.pop();
}

void ChatterBox::receive_waiting(sf::Packet &packet)
{
	while(!isUnread())
		;
	receive(packet);
}
//===============================================================

void ChatterBox::snd_queue(void)
{
	while (!_tx.queue.empty()) {
		if (sf::Socket::Done != _socket->send(_tx.queue.front())) {
			_connected = false;
			return;
		}
		
		sf::Lock lock(_tx.mutex);
		_tx.queue.pop();
	}
}

void ChatterBox::send(sf::Packet const &packet)
{
	sf::Lock lock(_tx.mutex);
	_tx.queue.push(packet);
	if (1 == _tx.queue.size())
		_tx.thread.launch();
}

//===============================================================

bool ChatterBox::isUnread(void) const { return !_rx.queue.empty(); }
bool ChatterBox::isConnected(void) const { return _connected; }
 

#include "lib/prim/Prim_system.hpp"
#include "lib/Server.hpp"

#include <iostream>
#include <queue>


int main(int argc, char* argv[])
{
	if (argc != 3 && argc != 4) {
		std::cerr << "Call format: " << argv[0] << " [height] [width]" << std::endl;
		return 1;
	}
	
	int width = std::atoi(argv[1]);
	int height = std::atoi(argv[2]);
	
	if (width <= 0 || height <= 0) {
		std::cerr << "Wrong field size specified" << std::endl;
		return 1;
	}
	
	sys::Mutex mtx((argc == 3) ? sys::Mutex::BOTH : sys::Mutex::DELETE);
	
	Server server(height, width);
	
	if (argc == 4) {
		mtx.unlock();
		mtx.lock();
	}
	
	while (server.busy()) {
		sf::sleep(sf::milliseconds(2000));
	}
	
	std::clog << "Server shutdown" << std::endl;
	return 0;
}

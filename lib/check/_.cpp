#include "GamePrimitives.hpp"
#include "ChatterBox.hpp"
#include "Client.hpp"
#include <iostream>

int main()
{
	std::cout << "All OK" << std::endl;
	
	TBSGame::Field field(3,4);
	field[3][0] = 4;
	std::cout << field << std::endl;
	
	TBSGame::Cell a((uint16_t) 4), b((uint16_t) 5);
	std::cout << (a < 5) << std::endl;
	
	{
		sf::Packet p;
		sf::Vector2u vec(3,0);
		std::cout << vec << std::endl;
		p << vec;
		vec = sf::Vector2u();
		p >> vec;
		std::cout << vec << std::endl;
		
		std::cout << field[vec] << std::endl;
	}
	
	{
		sf::Packet p;
		TBSGame::MsgType mtype = TBSGame::MsgType::FEED;
		std::cout << mtype << std::endl;
		p << mtype;
		mtype = TBSGame::MsgType::ACK;
		p >> mtype;
		std::cout << mtype << std::endl;
	}
	
	return 0;
}

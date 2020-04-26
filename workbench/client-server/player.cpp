#include <iostream>
#include <SFML/Network.hpp>

struct cell {
	unsigned int capacity = 6;
	unsigned int size = 0;
	unsigned int owner = 0;
	bool isCell = true;
};

struct vec2 {
	int x, y;
	vec2(int x_new, int y_new)
		: x(x_new)
		, y(y_new)
	{}
};

inline vec2 LU(vec2 p)
{
	return {p.x - ((p.y % 2) ? 0 : 1), p.y - 1};
}

int main()
{
	//sf::TcpSocket socket;
	//socket.connect("127.0.0.1", 5990);
	
	int const width = 10;
	int const height = 5;
	
	cell **field = new cell*[height];
	for (int i = 0; i < height; ++i)
		field[i] = new cell[width];
	
	vec2 p{3, 2};
	field[p.y][p.x].owner = 1;
	p = LU(p);
	field[p.y][p.x].owner = 2;
	p = LU(p);
	field[p.y][p.x].owner = 3;
	/*p = R(p);
	field[p.first][p.second].owner = 2;
	p = RD(p);
	field[p.first][p.second].owner = 2;*/
	
	
	
	for (int i = 0; i < height; ++i) {
		if (i % 2)
			std::cout << "   ";
		for (int j = 0; j < width; ++j) {
// 			std::cout << "|" << i << " " << j << "| ";
			if (field[i][j].owner)
				std::cout << "| " << field[i][j].owner << " | ";
			else
				std::cout << "|   | ";
		}
		std::cout << std::endl;
	}
	
	for (int i = 0; i < height; ++i)
		delete[] field[i];
	delete[] field;
	
	return 0;

	/*while (1) {
		sf::Packet packet;
		socket.receive(packet);
		std::string message;
		packet >> message;
		std::cout << message << std::endl;
	}*/
	
	return 0;
}

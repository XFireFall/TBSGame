#include "GamePrimitives.hpp"

//================================================================
//========| MsgType |========
//================================================================

namespace TBSGame {
	sf::Packet& operator<<(sf::Packet& packet, TBSGame::MsgType const& mtype)
	{
		return packet << (int) mtype;
	}
	
	sf::Packet& operator>>(sf::Packet& packet, TBSGame::MsgType& mtype)
	{
		return packet >> (int&) mtype;
	}
	
	std::ostream& operator<<(std::ostream& os, TBSGame::MsgType const& mtype)
	{
		std::string res;
		switch (mtype) {
			case MsgType::ACK:   res = "<ack>"; break;
			case MsgType::NOPE:  res = "<nope>"; break;
			case MsgType::FIGHT: res = "<fight>"; break;
			case MsgType::FEED:  res = "<feed>"; break;
			case MsgType::MAIL:  res = "<mail>"; break;
			case MsgType::UPD:   res = "<update>"; break;
			default: res = "<? ? ?>"; break;
		}
		return os << res;
	}
}

//================================================================
//========| Cell |========
//================================================================

namespace TBSGame {
	Cell::Cell(uint16_t size_new, uint16_t owner_new, uint16_t capacity_new)
		: owner(owner_new)
		, capacity(capacity_new)
		, size((size_new < 0) ? 0 : (size_new > capacity) ? capacity : size_new)
	{ }
	
	bool Cell::isCell(void) const { return capacity != 0; }

	bool Cell::operator++(void)
	{
		if (size == capacity)
			return false;
		++size;
		return true;
	}
	
	bool Cell::operator--(void)
	{
		if (size == 0)
			return false;
		--size;
		if (size == 0) {
			std::cout << "owner := 0" << std::endl;
			owner = 0;
		}
		return true;
	}
	
	bool Cell::operator++(int) { return operator++(); }
	bool Cell::operator--(int) { return operator--(); }
	
	bool Cell::operator >(Cell const& cell) const { return size  > cell.size; }
	bool Cell::operator>=(Cell const& cell) const { return size >= cell.size; }
	bool Cell::operator==(Cell const& cell) const { return size == cell.size; }
	bool Cell::operator!=(Cell const& cell) const { return size != cell.size; }
	bool Cell::operator<=(Cell const& cell) const { return size <= cell.size; }
	bool Cell::operator <(Cell const& cell) const { return size  < cell.size; }
	
	sf::Packet& operator<<(sf::Packet& packet, TBSGame::Cell const& cell)
	{
		return packet << cell.owner << cell.capacity << cell.size;
	}
	
	sf::Packet& operator>>(sf::Packet& packet, TBSGame::Cell& cell)
	{
		return packet >> cell.owner >> cell.capacity >> cell.size;
	}
	
	std::ostream& operator<<(std::ostream& os, TBSGame::Cell const& cell)
	{
		return os << "{#" << cell.owner << ',' << cell.size << '/' << cell.capacity << '}';
	}
}

//================================================================
//========| Field |========
//================================================================

namespace TBSGame {
	Field::Field(uint16_t width_new, uint16_t height_new)
		: _width(width_new)
		, _height(height_new)
		, _data(height_new)
	{
		for (auto& it: _data)
			it.resize(_width);
	}
	
	uint16_t Field::getWidth(void) const { return _width; }
	uint16_t Field::getHeight(void) const {return _height; }
	bool Field::isValid(sf::Vector2u const& vec) const { return (vec.x < _width) && (vec.y < _height); }
	
	std::vector<Cell> const& Field::operator[](uint16_t i) const { return _data[i]; }
	std::vector<Cell>& Field::operator[](uint16_t i) { return _data[i]; }
	Cell const& Field::operator[](sf::Vector2u const& vec) const { return _data[vec.y][vec.x]; }
	Cell& Field::operator[](sf::Vector2u const& vec) { return _data[vec.y][vec.x]; }
	
	sf::Packet& operator>>(sf::Packet& packet, TBSGame::Field& field)
	{
		uint16_t height = 0, width = 0;
		packet >> height >> width;
		field = TBSGame::Field(height, width);
		for (uint16_t i = 0; i < height; ++i)
			for (uint16_t j = 0; j < width; ++j)
				packet >> field[i][j];
		return packet;
	}
	
	sf::Packet& operator<<(sf::Packet& packet, TBSGame::Field const& field)
	{
		packet << field.getHeight() << field.getWidth();
		for (uint16_t i = 0; i < field.getHeight(); ++i)
			for (uint16_t j = 0; j < field.getWidth(); ++j)
				packet << field[i][j];
		return packet;
	}
	
	std::ostream& operator<<(std::ostream& os, TBSGame::Field const& field)
	{
		os << '[' << field._height << 'x' << field._width << ']' << std::endl;
		bool flag = false;
		for (auto vec: field._data) {
			if (flag)
				os << "    ";
			for (auto cell: vec)
				os << cell << ' ';
			os << std::endl;
			flag = !flag;
		}
		return os;
	}
}

//================================================================
//========| SFML I/O |========
//================================================================

sf::Packet& operator<<(sf::Packet& packet, sf::Vector2u const& vec)
{
	return packet << (uint16_t) vec.x << (uint16_t) vec.y;
}

sf::Packet& operator>>(sf::Packet& packet, sf::Vector2u& vec)
{
	return packet >> (uint16_t&) vec.x >> (uint16_t&) vec.y;
}

std::ostream& operator<<(std::ostream& os, sf::Vector2u const& vec)
{
	return os << (uint16_t) vec.x << ' ' << (uint16_t) vec.y;
}

std::istream& operator>>(std::istream& is, sf::Vector2u& vec)
{
	return is >> vec.x >> vec.y;
}

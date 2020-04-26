#pragma once

#include <vector>
#include <cstdint>
#include <iostream>
#include <SFML/Network.hpp>

namespace TBSGame {
	enum class MsgType { ACK = 0, NOPE, FIGHT, FEED, MAIL, UPD };
	
	sf::Packet& operator<<(sf::Packet& packet, TBSGame::MsgType const& mtype);
	sf::Packet& operator>>(sf::Packet& packet, TBSGame::MsgType& mtype);
	std::ostream& operator<<(std::ostream& os, TBSGame::MsgType const& mtype);
	
	//================================
	
	struct Cell {
	private:
		static uint16_t const _default_capacity = 8;
		
	public:
		uint16_t owner;
		uint16_t capacity;
		uint16_t size;
		
		Cell(uint16_t size_new = 0, uint16_t owner_new = 0, uint16_t capacity_new = _default_capacity);
		
		bool isCell(void) const;
		bool operator++(void);
		bool operator++(int);
		bool operator--(void);
		bool operator--(int);
		
		bool operator >(Cell const& cell) const;
		bool operator>=(Cell const& cell) const;
		bool operator==(Cell const& cell) const;
		bool operator!=(Cell const& cell) const;
		bool operator<=(Cell const& cell) const;
		bool operator <(Cell const& cell) const;
		
		friend std::ostream& operator<<(std::ostream& os, TBSGame::Cell const& cell);
	};
	
	sf::Packet& operator<<(sf::Packet& packet, TBSGame::Cell const& cell);
	sf::Packet& operator>>(sf::Packet& packet, TBSGame::Cell& cell);
	std::ostream& operator<<(std::ostream& os, TBSGame::Cell const& cell);
	
	//================================
	
	class Field {
	private:
		uint16_t _width, _height;
		std::vector<std::vector<Cell>> _data;
	
	public:
		Field(uint16_t width_new, uint16_t height_new);
		
		uint16_t getWidth(void) const;
		uint16_t getHeight(void) const;
		bool isValid(sf::Vector2u const& vec) const;
		
		std::vector<Cell> const& operator[](uint16_t i) const;
		std::vector<Cell>& operator[](uint16_t i);
		Cell const& operator[](sf::Vector2u const& vec) const;
		Cell& operator[](sf::Vector2u const& vec);
		
		friend std::ostream& operator<<(std::ostream& os, TBSGame::Field const& field);
	};
	
	sf::Packet& operator>>(sf::Packet& packet, TBSGame::Field& field);
	sf::Packet& operator<<(sf::Packet& packet, TBSGame::Field const& field);
	std::ostream& operator<<(std::ostream& os, TBSGame::Field const& field);
}

//================================================================

sf::Packet& operator<<(sf::Packet& packet, sf::Vector2u const& vec);
sf::Packet& operator>>(sf::Packet& packet, sf::Vector2u& vec);
std::ostream& operator<<(std::ostream& os, sf::Vector2u const& vec);
std::istream& operator>>(std::istream& is, sf::Vector2u& vec);

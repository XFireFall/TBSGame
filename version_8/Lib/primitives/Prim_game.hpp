#pragma once

#include <iostream>
#include <vector>

#include "Prim_common.hpp"

//====================================================================================
//==============================| Minors |============================================

namespace gm {
	/* Phase of playing:
	 *  ATTACK -> FEED -> WAIT
	 * Player may attack only
	 * with the server's permission
	 */
	enum Phase { ATTACK, FEED, WAIT, LOSE, WIN };
	
	std::ostream& operator<<(std::ostream& os, gm::Phase const& ph);
}

//====================================================================================
//==============================| struct player_data |================================

namespace gm {
	struct player_data {
	public:
		/* ID that is given by server
		 * when player connectsto a game session
		 * Zero is a poison value */
		uint16_t id;
		
		/* Nickname which players
		 * choose at their wish */
		std::string nickname;
		
	public:
		/* Default _nickname is "Player" */
		player_data(std::string const& nickname_new = "Player");
	};
	
	template <typename OSTREAM> OSTREAM& operator<<(OSTREAM& os, gm::player_data const& pd) { return os << pd.id << pd.nickname; }
	template <typename ISTREAM> ISTREAM& operator>>(ISTREAM& is, gm::player_data      & pd) { return is >> pd.id >> pd.nickname; }
	template <> std::ostream& operator<<(std::ostream& os, gm::player_data const& pd);
}

//====================================================================================
//==============================| struct Cell |=======================================

namespace gm {
	struct Cell {
	public:
		static int const _default_capacity = 8;
		
		/* Current size of the cell
		 * from zero to _capacity */
		uint16_t _size;
		
		/* ID of the player
		 * who owns the cell */
		uint16_t _owner;
		
		/* Maximum value of _size
		 * for the cell */
		uint16_t _capacity;
		
	public:
		/* A cell may easily be created
		 * with no parameters */
		Cell(uint16_t size = 0, uint16_t owner = 0, uint16_t capacity = _default_capacity);
		
		/* Checks if _size is less than _capacity */
		bool empty(void) const;
		
		/* Checks if the owner of the cell
		 * is player with given ID */
		bool belongsTo(uint16_t owner) const;
		
		/* Sets _size and _owner to zero
		 * as nobody owns an empty cell */
		void discard(void);
		
		/* Sets _size to 2 and
		 * _owner what is specified */
		void init(uint16_t owner);
	};
	
	template <typename OSTREAM> OSTREAM& operator<<(OSTREAM& os, gm::Cell const& cell) { return os << cell._size << cell._owner << cell._capacity; }
	template <typename ISTREAM> ISTREAM& operator>>(ISTREAM& is, gm::Cell      & cell) { return is >> cell._size >> cell._owner >> cell._capacity; }
	template <> std::ostream& operator<<(std::ostream& os, gm::Cell const& cell);
}

//====================================================================================
//==============================| struct Field |======================================

namespace gm {
	using Field_parent = std::vector<std::vector<Cell>>;
	
	class Field: private Field_parent {
	public:
		/* Consider values to be actual integers */
		Field(int width, int height);
		
		/* Creates field with zero size */
		Field(void) = default;
		
		/* Methods for receving
		 * access to cells */
		Cell      & at(sf::Vector2u const& v);
		Cell const& at(sf::Vector2u const& v) const;
		Cell      & operator[](sf::Vector2u const& v);
		Cell const& operator[](sf::Vector2u const& v) const;
		
		/* Methods for receving
		 * size of the field */
		int height(void) const;
		int width(void) const;
		
		/* Returns total size of cells that are owned
		 * by the player with the specified ID */
		int count(uint16_t owner) const;
		
		/* Attack operation: let Cell{.size=A, .capacity=B, .owner=C} be A/B:C
		 *  [6/8:1]->[3/8:2] => [1/8:1]--[2/8:1]
		 *  [6/8:1]->[8/8:2] => [1/8:1]--[3/8:2]
		 *  [6/8:1]->[5/8:2] => [1/8:1]--[0/8:0]
		 * The attack operation happens any time the method is called */
		void attack(sf::Vector2u const& who, sf::Vector2u const& whom);
		
		/* Checks if attack operation is available */
		bool may_attack(uint16_t owner, sf::Vector2u const& v_who, sf::Vector2u const& v_whom) const;
		
		/* Feed operation: let Cell{.size=A, .capacity=B, .owner=C} be [A/B:C]
		 *  [6/8:1] => [7/8:1]
		 *  [8/8:1] => [8/8:1]
		 * The feed operation happens any time the method is called */
		void feed(sf::Vector2u const& whom);
		
		/* Checks if feed operation is available */
		bool may_feed(uint16_t owner, sf::Vector2u const& v_whom) const;
		
		/* Checks if the specified position
		 * is within field bounds */
		bool belongs(sf::Vector2u const& a) const;
		
		/* Checks if the entire field is owner's */
		bool belongsTo(uint16_t owner) const;
		
		/* Checks if the specified positions
		 * are neighbours - or if one can be
		 * reached from another in one step */
		bool reachable(sf::Vector2u const& a, sf::Vector2u const& b) const;
		
		/* Sets new size of the field 
		 * previous data is deleted */
		void resize(int height, int width);
		
		/* Discards every cell that
		 * was owned by the player with specified ID
		 * (See Cell:discard) */
		std::vector<sf::Vector2u> discard(uint16_t owner);
		
		/* Initializes (See Cell::init) exactly one cell
		 * to be owned by the player with specified ID
		 * TODO WARNING: the behaviour when there's no place is undefined */
		sf::Vector2u nest(uint16_t owner);
		
		template <typename OSTREAM> friend OSTREAM& operator<<(OSTREAM& os, gm::Field const& f);
		template <typename ISTREAM> friend ISTREAM& operator>>(ISTREAM& is, gm::Field      & f);
	};
	
	template <typename OSTREAM> OSTREAM& operator<<(OSTREAM& os, gm::Field const& f) { os << f.height() << f.width(); for (auto  ln: f) for (auto  cell: ln) os << cell; return os; }
	template <typename ISTREAM> ISTREAM& operator>>(ISTREAM& is, gm::Field      & f) { int height, width; is >> height >> width; f.resize(height, width); for (auto& ln: f) for (auto& cell: ln) is >> cell; return is; }
	template <> std::ostream& operator<<(std::ostream& os, gm::Field const& field);
}

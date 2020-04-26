#include "Primitives_game.hpp"

using namespace gm;

//====================================================================================
//==============================| Minors |============================================



//====================================================================================
//==============================| struct player_data |================================

player_data::player_data(std::string const& nickname_new)
	: nickname(nickname_new)
{ }

template <>
std::ostream& gm::operator<<(std::ostream& os, gm::player_data const& pd)
{
	 return os << "Player" << pd.id << '(' << pd.nickname << ')';
}


//====================================================================================
//==============================| struct Cell |=======================================

Cell::Cell(uint16_t size, uint16_t owner, uint16_t capacity)
	: _owner(owner)
	, _capacity(capacity)
	, _size((size > capacity) ? capacity : size)
{ }

//==============================

bool Cell::empty(void) const
{
	return _size == 0;
}

//==============================

bool Cell::belongsTo(uint16_t owner) const
{
	return _owner == owner;
}

//==============================

void Cell::discard(void)
{
	_size = 0;
	_owner = 0;
}

//==============================

void Cell::init(uint16_t owner)
{
	_size = 1;
	_owner = owner;
}

//==============================

template <>
std::ostream& gm::operator<<(std::ostream& os, gm::Cell const& cell)
{
	return os << '[' << cell._size << '/' << cell._capacity << ':' << cell._owner << ']';
}

//====================================================================================
//==============================| struct Field |======================================

Field::Field(int width, int height)
	: Field_parent(height)
{
	for (auto& it: *this)
		it.resize(width);
}

//==============================

Cell      & Field::at        (sf::Vector2u const& v)       { return Field_parent::at(v.y)[v.x]; }
Cell const& Field::at        (sf::Vector2u const& v) const { return Field_parent::at(v.y)[v.x]; }
Cell      & Field::operator[](sf::Vector2u const& v)       { return at(v); }
Cell const& Field::operator[](sf::Vector2u const& v) const { return at(v); }

//==============================

int Field::height(void) const { return size(); }
int Field::width(void)  const { return Field_parent::operator[](0).size(); }

//==============================

int Field::count(uint16_t owner) const
{
	int res = 0;
	for (auto ln: *this)
		for (auto cell: ln)
			if (cell.belongsTo(owner))
				res += cell._size;
	return res;
}

//==============================

void Field::attack(sf::Vector2u const& who, sf::Vector2u const& whom)
{
	uint16_t mass = at(who)._size - 1;
	at(who)._size = 1;
	
	if (at(whom)._owner == 0) {
		at(whom)._size = mass;
		at(whom)._owner = at(who)._owner;
	} else {
		int newsize = (int)at(whom)._size - mass;
		if (newsize == 0) {
			at(whom).discard();
		} else if (newsize > 0) {
			at(whom)._size = newsize;
		} else {
			at(whom)._size = -newsize;
			at(whom)._owner = at(who)._owner;
		}
	}
}

//==============================

bool Field::may_attack(uint16_t owner, sf::Vector2u const& v_who, sf::Vector2u const& v_whom) const
{
	if (!belongs(v_who) || !belongs(v_whom))
		return false;
	
	Cell who = at(v_who), whom = at(v_whom);
	bool res = true;
	res &= (reachable(v_who, v_whom));
	res &= (who.belongsTo(owner));
	res &= (!whom.belongsTo(owner));
	res &= (who._size > 1);
	return res;
}

//==============================

void Field::feed(sf::Vector2u const& whom)
{
	at(whom)._size++;
}

//==============================

bool Field::may_feed(uint16_t owner, sf::Vector2u const& v_whom) const
{
	if (!belongs(v_whom))
		return false;
	
	Cell whom = at(v_whom);
	bool res = true;
	res &= (whom.belongsTo(owner));
	res &= (whom._size < whom._capacity);
	return res;
}

//==============================

bool Field::belongs(sf::Vector2u const& a) const
{
	return (a.y < height()) && (a.x < width());
}

//==============================

bool Field::reachable(sf::Vector2u const& a, sf::Vector2u const& b) const
{
	if (!belongs(a) || !belongs(b) || (a == b))
		return false;
	
	if (std::abs((int)b.x - (int)a.x) > 1 || std::abs((int)b.x - (int)a.x) > 1)
		return false;
	
	bool res = false;
	if (a.y == b.y)
		res = true;
	else {
		if (a.x == b.x)
			res = true;
		else
			res = (a.x == b.x + ((a.y % 2 == 0) ? 1 : -1));
	}
	return res;
}

//==============================

void Field::resize(int height, int width)
{
	Field_parent::resize(height);
	for (auto& it: *this)
		it.resize(width);
	
	sf::Vector2u v{0, 0};
	for ( ; v.y < height; ++v.y)
		for ( ; v.x < width; ++v.x)
			at(v) = Cell{};
}

//==============================

std::vector<sf::Vector2u> Field::discard(uint16_t owner)
{
	std::vector<sf::Vector2u> res;
	sf::Vector2u v{0, 0};
	for ( ; v.y < height(); ++v.y)
		for ( ; v.x < width(); ++v.x)
			if (at(v).belongsTo(owner)) {
				at(v).discard();
				res.push_back(v);
			}
	return res;
}

//==============================

sf::Vector2u Field::nest(uint16_t owner)
{
	sf::Vector2u v{0, 0};
	for ( ; v.y < height(); ++v.y)
		for ( ; v.x < width(); ++v.x)
			if (at(v).empty()) {
				at(v).init(owner);
				return v;
			}
	return v;
}

//==============================

template <>
std::ostream& gm::operator<<(std::ostream& os, gm::Field const& field)
{
	int c = 1;
	for (auto ln: field) {
		if (c = 1 - c)
			os << "    ";
		for (auto cell: ln) {
			os << cell << " ";
		}
		os << std::endl;
	}
	return os;
}

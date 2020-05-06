#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <iostream>

template<typename OSTREAM, typename T> OSTREAM&      operator<<(OSTREAM&      os, sf::Vector2<T> const& v) { return os << v.y << v.x; }
template<typename ISTREAM, typename T> ISTREAM&      operator>>(ISTREAM&      is, sf::Vector2<T>      & v) { return is >> v.y >> v.x; }
template<typename T>                   std::ostream& operator<<(std::ostream& os, sf::Vector2<T> const& v) { return os << '{' << v.x << ',' << v.y << '}'; }

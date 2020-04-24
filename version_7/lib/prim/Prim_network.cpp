#include "Prim_network.hpp"

using namespace net;

//====================================================================================
//==============================| Minors |============================================



//====================================================================================
//==============================| enum msg_t |======================================

template <>
std::ostream& net::operator<<(std::ostream& os, msg_t const& mtype)
{
	std::string res;
	switch (mtype) {
		case msg_t::TURN:   res = "<turn>";    break;
		case msg_t::ATTACK: res = "<attack>";  break;
		case msg_t::FEED:   res = "<feed>";    break;
		case msg_t::UPD:    res = "<update>";  break;
		case msg_t::MSG:    res = "<message>"; break;
		default: res = "<? ? ?>"; break;
	}
	return os << res;
}

//====================================================================================
//==============================| enum upd_t |======================================

template <>
std::ostream& net::operator<<(std::ostream& os, upd_t const& utype)
{
	std::string res;
	switch (utype) {
		case upd_t::DISC_PLAYER: res = "<disc>"; break;
		case upd_t::CONN_PLAYER: res = "<conn>"; break;
		case upd_t::CELL_UPDATE: res = "<cell>"; break;
		default: res = "<? ? ?>"; break;
	}
	return os << res;
}

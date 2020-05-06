#pragma once

#include <SFML/Network.hpp>

#include <vector>
#include <iostream>

#include "Prim_common.hpp"

//====================================================================================
//==============================| Minors |============================================

namespace net {
	/* Network port has been chosen here */
	unsigned short const gameport = 5990;
}

//====================================================================================
//==============================| enum msg_t |======================================

namespace net {
	enum class msg_t {
		TURN,	// client: FEED over;          Next come: none
				// server: Who starts playing; Next cone: [ID] [nickname]
		ATTACK,	// Dealing attack operation;   Next come: [pos from] [pos to]
		FEED,	// Dealing feed operation;     Next come: [pos where]
		UPD,	// (See upd_t);                Next come: [upd_t] ...
		MSG,	// Common chat message;        Next come: [msg]
		GMOVER	// Game over;                  Next come: [isWin]
	};

	template <typename OSTREAM> OSTREAM& operator<<(OSTREAM& os, msg_t const& mtype) { return os << (int ) mtype; }
	template <typename ISTREAM> ISTREAM& operator>>(ISTREAM& is, msg_t      & mtype) { return is >> (int&) mtype; }
	template <> std::ostream& operator<<(std::ostream& os, net::msg_t const& mtype);
}

//====================================================================================
//==============================| enum upd_t |======================================

namespace net {
	/* Commands that are sent by server
	 * and telling about field updates
	 * caused by other players
	 * Should be processed immediately */
	enum class upd_t {
		DISC_PLAYER,	// Player disconnection; Next come: [ID] [nickname]
		CONN_PLAYER,	// New player connected; Next come: [ID] [nickname]
		CELL_UPDATE,	// Update a cell;        Next come: [pos] [cell]
	};

	template <typename OSTREAM> OSTREAM& operator<<(OSTREAM& os, upd_t const& utype) { return os << (int ) utype; }
	template <typename ISTREAM> ISTREAM& operator>>(ISTREAM& is, upd_t      & utype) { return is >> (int&) utype; }
	template <> std::ostream& operator<<(std::ostream& os, upd_t const& utype);
}

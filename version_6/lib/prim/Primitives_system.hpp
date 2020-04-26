#pragma once

#include <SFML/System.hpp>

#include <sys/types.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>

#include "Primitives_common.hpp"

//====================================================================================
//==============================| Minors |============================================

namespace sys {
	/* Semophore access key has been chosen here */
	key_t const gamekey = 5990;
}

//====================================================================================
//==============================| System Mutex |======================================

namespace sys {
	/* Like ordinary mutexes
	 * but through the Linux system */
	class Mutex {
	public:
		enum Mode { CREAT, DELETE, BOTH };
		enum ex { EXIST };
		
	private:
		int _semid;
		Mode _mode;
		
	public:
		Mutex(Mode mode);
		
		~Mutex(void);
		
		bool lock(void) const;
		bool unlock(void) const;
		
		void change_mode(Mode mode);
	};
}

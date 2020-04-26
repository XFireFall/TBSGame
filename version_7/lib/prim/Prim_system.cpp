#include "Prim_system.hpp"

using namespace sys;

#include <sys/types.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>

//====================================================================================
//==============================| Minors |============================================



//====================================================================================
//==============================| System Mutex |======================================

Mutex::Mutex(Mode mode)
	: _mode(mode)
	, _semid(semget(gamekey, 1, (mode == CREAT || mode == BOTH) ? (IPC_CREAT | IPC_EXCL) : 0))
{
	if (_semid < 0)
		throw EEXIST;
}

Mutex::~Mutex(void)
{
	if (_mode == DELETE || _mode == BOTH)
		semctl(_semid, 0, IPC_RMID);
}

bool Mutex::lock(void) const
{
	struct sembuf sop = {0, -1, 0};
	return semop(_semid, &sop, 1) ? true : false;
}

bool Mutex::unlock(void) const
{
	sleep(1);
	struct sembuf sop = {0, -1, 0};
	return semop(_semid, &sop, 1) ? true : false;
}

void Mutex::change_mode(Mode mode)
{
	_mode = mode;
}

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

/**
* @brief spawn a new player entity
* @return NULL on error, or a pointer to the spawned player entity
*/
Entity *player_new_entity();

#endif
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "entity.h"

/**
* @brief spawn a new player entity
* @return NULL on error, or a pointer to the spawned player entity
*/
Entity* enemy_new_entity();

Entity* enemy_load(const char *filename, GFC_Vector2D position);

#endif
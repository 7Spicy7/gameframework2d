#ifndef __COLLECTIBLES_H__
#define __COLLECTIBLES_H__

#include "simple_json.h"

#include "entity.h"
#include "gfc_text.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

void collectibles_init(const char* filename);

SJson *get_collectible_def_by_name(const char* name);

Entity *collectible_new(const char *name);

void collectible_free(Entity *collectible);

void collectibles_close();

#endif
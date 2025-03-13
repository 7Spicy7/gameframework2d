#ifndef __COLLECTIBLES_H__
#define __COLLECTIBLES_H__

#include "simple_json.h"

#include "gfc_text.h"
#include "gf2d_sprite.h"

typedef struct
{
	int count;
	Sprite *sprite;
	GFC_TextLine name;
}Collectible;

void collectibles_init(const char* filename);

SJson *get_collectible_def_by_name(const char* name);

Collectible *collectible_new(const char *name);

void collectible_free(Collectible *collectible);

void collectibles_close();

#endif
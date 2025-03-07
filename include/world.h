#ifndef __WORLD_H__
#define __WORLD_H__

#include "gf2d_sprite.h"

typedef struct
{
	Sprite *background; /**<background image for the world*/
	Sprite *tileset; /**<the sprite with the tiles on it*/
	Uint8  *tileMap; /**<the tiles (the tiles!)*/
	Uint32  tileHeight;
	Uint32  tileWidth;
}World;

World* world_test_new();

World *world_new();

void world_free(World *world);

void world_draw(World *world);

#endif
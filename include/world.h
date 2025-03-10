#ifndef __WORLD_H__
#define __WORLD_H__

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"

typedef struct
{
	Sprite *background; /**<background image for the world*/
	Sprite *tileLayer; /**<prerendered tiles??? the power of technology*/
	Sprite *tileset; /**<the sprite with the tiles on it*/
	Uint8  *tileMap; /**<the tiles (the tiles!)*/
	Uint32  tileHeight; /**<tiles high for the map*/
	Uint32  tileWidth; /**<tiles wide for the map*/
}World;

World *world_load(const char* filename);

void world_tile_layer_build(World *world);

World *world_test_new();

World *world_new(Uint32 width, Uint32 height);

void world_free(World *world);

void world_draw(World *world);

#endif
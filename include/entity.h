#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "simple_json.h"
#include "gfc_types.h"
#include "gfc_shape.h"
#include "gfc_text.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

typedef struct Entity_S
{
	Uint8		 _inuse; /**<memory management flag*/
	GFC_TextLine name; /**<name of the entity for debugging*/
	char		*spriteName; /**<for referencing sprites*/
	GFC_Rect	 bounds; /**<where the sprite is*/
	Sprite		 *sprite; /**<graphical representation of the entity*/
	float		 frame; /**<for drawing the sprite*/
	float        speedMax;
	GFC_Vector2D position; /**<where to draw it*/
	GFC_Vector2D velocity; /**<how we are moving*/
	GFC_Vector2D acceleration; /**<how we are moving*/
	GFC_Vector2D offset;
	void (*think)(struct Entity_S *self); /**<function for decision-making*/
	void (*update)(struct Entity_S *self); /**<function for decision-doing*/
	void (*free)(struct Entity_S *self); /**<cleaning up when not in use*/
	void *data;
	int team;
	// ENEMY SPECIFIC
	int directionx; /**<for enemies, tells what direction they're going in horizontally. -1 is left, 0 is not moving, 1 is right*/
	int directiony; /**<like above but for vertical movement. -1 is left, 0 is not moving, 1 is down*/
	int speed;
	// COLLECTIBLE SPECIFIC
	int count; /**<for collectibles, how many can stack (if any)*/
	SJson * info; /**<holds the information about the collectible*/
}Entity;

/**
* @brief initialize the entity sub entity_system_init
* @param maxEnts uper limmit for how many entities can exist at once
*/
void entity_system_init(Uint32 maxEnts);

/**
* @brief clean up all active entities
* @param don't clean up this entity
*/
void entity_clear_all(Entity *ignore);

/**
* @brief draw all inuse entities, if they have a sprite
*/
void entity_system_draw_all();

/**
* @brief get a new empty entity to work with
* @return NULL if out of entities, or a blank entity otherwise
*/
Entity *entity_new();

void entity_draw(Entity *self);

/**
* @brief run the think function for all active entities
*/
void entity_system_think_all();

/**
* @brief run the update function for all active entities
*/
void entity_system_update_all();

/**
* @brief clean up an entity and free up its spot for future use
*/
void entity_free(Entity *self);

void entity_system_close();

void entity_configure_from_file(Entity* self, const char *filename);

void entity_configure(Entity *self, SJson *json);

int entity_collision_check(Entity* self, Entity* other);

#endif
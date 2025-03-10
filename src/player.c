#include "simple_logger.h"
#include "gfc_input.h"
#include "player.h"

void player_think(Entity *self);
void player_update(Entity *self);
void player_free(Entity *self);

Entity *player_new_entity()
{
	gfc_input_init("config/inputs.cfg");
	Entity *self;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new player entity");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all("images/orbobaseidleloop.png", 128, 128, 16, 0);
	self->frame = 0;
	self->position = gfc_vector2d(0,0);
	self->think = player_think;
	self->update = player_update;
	self->free = player_free;
	return self;
}

void player_think(Entity *self)
{
	gfc_input_update();
	GFC_Vector2D dir = { 0 };
	Sint32 mx, my;
	if (!self)return;
	SDL_GetMouseState(&mx,&my);
	if (gfc_input_command_down("right"))dir.x = 1;
	if (gfc_input_command_down("left"))dir.x = -1;
	if (gfc_input_command_down("down"))dir.y = 1;
	if (gfc_input_command_down("up"))dir.y = -1;
	gfc_vector2d_normalize(&dir);
	gfc_vector2d_scale(self->velocity, dir, 2);
}

void player_update(Entity *self)
{
	if (!self)return;
	self->frame += 0.1;
	if (self->frame >= 16)self->frame = 0;
	gfc_vector2d_add(self->position, self->position, self->velocity);
}

void player_free(Entity *self)
{
	if (!self)return;
}
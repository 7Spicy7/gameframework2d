#include "simple_logger.h"
#include "gfc_input.h"
#include "gfc_list.h"
#include "enemy.h"

void enemy_think(Entity* self);
void enemy_update(Entity* self);
void enemy_free(Entity* self);

Entity* enemy_new_entity()
{
	GFC_Rect bounds;
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new enemy entity");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all("images/lilbug.png", 128, 128, 1, 0);
	self->frame = 0;
	self->position = gfc_vector2d(728, 64);
	self->think = enemy_think;
	self->update = enemy_update;
	self->team = 4;
	bounds = gfc_rect(self->position.x, self->position.y, self->position.x + 128, self->position.y + 128);
	self->bounds = bounds;
	self->free = enemy_free;
	return self;
}

void enemy_think(Entity* self)
{
	GFC_Vector2D dir = { 0 };
	Sint32 mx, my;
	if (!self)return;
	SDL_GetMouseState(&mx, &my);
	if (self->position.x < mx)dir.x = 1;
	if (self->position.x > mx)dir.x = -1;
	if (self->position.y < my)dir.y = 1;
	if (self->position.y > my)dir.y = -1;
	gfc_vector2d_normalize(&dir);
	gfc_vector2d_scale(self->velocity, dir, 1);
}

void enemy_update(Entity* self)
{
	if (!self)return;
	self->frame += 0.1;
	if (self->frame >= 1)self->frame = 0;
	gfc_vector2d_add(self->position, self->position, self->velocity);
	self->bounds = gfc_rect(self->bounds.x + self->position.x, self->bounds.y + self->position.y, self->bounds.w + self->position.x, self->bounds.h + self->position.y);
}

void enemy_free(Entity* self)
{
	if (!self)return;
}
#include "simple_logger.h"
#include "gfc_input.h"
#include "gfc_list.h"
#include "world.h"
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
	self->sprite = gf2d_sprite_load_all("images/swoopybug.png", 128, 128, 1, 0);
	self->frame = 0;
	self->position = gfc_vector2d(8, 64);
	self->think = enemy_think;
	self->update = enemy_update;
	self->team = 4;
	bounds = gfc_rect(self->position.x, self->position.y, 128, 128);
	self->bounds = bounds;
	self->free = enemy_free;
	self->directionx = -1;
	self->directiony = 0;
	self->speed = 5;
	return self;
}

Entity* enemy_load(const char *filename, GFC_Vector2D position)
{
	Entity* self;
	SJson* json = NULL;
	int ox = 0, oy = 0, bw = 0, bh = 0;
	const char* sprite;
	int frame_w = 0, frame_h = 0;
	int frames_per_line = 0;
	int speed = 0;
	int dx = 0, dy = 0;
	if (!filename)
	{
		slog("no filename given");
		return NULL;
	}
	self = entity_new();
	if (!self) {
		slog("failed to spawn new enemy (load ver)");
		return NULL;
	}
	json = sj_load(filename);
	self->spriteName = sj_object_get_value_as_string(json, "name");
	slog("sprite name is %s", self->spriteName);
	sprite = sj_object_get_value_as_string(json, "sprite");
	slog("sprite named %s", sprite);
	sj_object_get_value_as_int(json, "sprite_w", &frame_w);
	sj_object_get_value_as_int(json, "sprite_h", &frame_h);
	sj_object_get_value_as_int(json, "spriteFPL", &frames_per_line);
	self->sprite = gf2d_sprite_load_all(sprite, frame_w, frame_h, frames_per_line, 0);
	slog("self->sprite made");
	sj_object_get_value_as_int(json, "offset_x", &ox);
	sj_object_get_value_as_int(json, "offset_y", &oy);
	sj_object_get_value_as_int(json, "bounds_w", &bw);
	sj_object_get_value_as_int(json, "bounds_h", &bh);
	sj_object_get_value_as_int(json, "dx", &dx);
	sj_object_get_value_as_int(json, "dy", &dy);
	slog("bounds of %s are %i, %i, %i, %i", self->spriteName, ox, oy, bw, bh);
	self->position = position;
	self->offset = gfc_vector2d(ox, oy);
	self->bounds = gfc_rect(self->position.x + ox, self->position.y + oy, bw, bh);
	self->frame = 0;
	self->think = enemy_think;
	self->update = enemy_update;
	self->team = 4;
	sj_object_get_int(json, "speed", &speed);
	self->speed = speed;
	slog("self->speed is %i", self->speed);
	self->free = enemy_free;
	self->directionx = dx;
	self->directiony = dy;
	return self;
}

void enemy_think(Entity* self)
{
	World* world = get_active_world();
	GFC_Vector2D dir = { 0 };
	if (!self)return;
	if (world_shape_check(world, gfc_shape_from_rect(self->bounds))) self->directionx = -self->directionx;
	if (self->directionx == 1)dir.x = 1;
	if (self->directionx == -1)dir.x = -1;
	if (self->directiony == 1)dir.y = 1;
	if (self->directiony == -1)dir.y = -1;
	gfc_vector2d_normalize(&dir);
	gfc_vector2d_scale(self->velocity, dir, self->speed);
}

void enemy_update(Entity* self)
{
	if (!self)return;
	self->frame += 0.1;
	if (self->frame >= 1)self->frame = 0;
	gfc_vector2d_add(self->position, self->position, self->velocity);
	self->bounds = gfc_rect(self->position.x + self->offset.x, self->position.y + self->offset.y, self->bounds.w, self->bounds.h);
}

void enemy_free(Entity* self)
{
	if (!self)return;
}
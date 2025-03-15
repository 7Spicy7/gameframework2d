#include "simple_logger.h"
#include "gfc_input.h"
#include "camera.h"
#include "world.h"
#include "player.h"
#include "inventory.h"

typedef struct
{
	Inventory inventory;
}PlayerData;

void player_think(Entity *self);
void player_update(Entity *self);
void player_free(Entity *self);

Entity *player_new_entity()
{
	gfc_input_init("config/inputs.cfg");
	Entity *self;
	PlayerData *data;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new player entity");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all("images/orbobaseidleloop.png", 128, 128, 16, 0);
	self->frame = 0;
	self->position = gfc_vector2d(64,64);
	self->think = player_think;
	self->update = player_update;
	self->free = player_free;
	self->team = 2;
	self->bounds = gfc_rect(self->position.x + 31, self->position.y + 82, 64, 46);
	data = gfc_allocate_array(sizeof(PlayerData),1);
	if (data)
	{
		self->data = data;
		inventory_init(&data->inventory);
	}
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
	World* world = get_active_world();
	GFC_Vector2D currentPos, newPos;
	GFC_Rect currentBounds, newBounds;
	if (!self)return;
	currentPos = self->position;
	currentBounds = self->bounds;
	self->frame += 0.1;
	if (self->frame >= 16)self->frame = 0;
	gfc_vector2d_add(newPos, self->position, self->velocity);
	newBounds = gfc_rect(self->position.x + 31, self->position.y + 82, self->bounds.w, self->bounds.h);
	if (world_shape_check(world, gfc_shape_from_rect(newBounds)))
	{
		GFC_Vector2D tile = tile_get(world, gfc_shape_from_rect(newBounds));
		GFC_Rect bounds1, bounds2;
		bounds1 = self->bounds;
		bounds2 = gfc_rect(tile.x*world->tileWidth, tile.y*world->tileHeight, world->tileWidth, world->tileHeight);
		if ((bounds1.x < bounds2.x + bounds2.w) && self->velocity.x < 0) {
			self->bounds = gfc_rect(bounds2.x + bounds2.w + 2, bounds1.y, self->bounds.w, self->bounds.h);
			self->position = gfc_vector2d(self->bounds.x - 31, self->bounds.y - 82);
		} else if (bounds1.x + bounds1.w > bounds2.x) {
			self->bounds = gfc_rect(bounds2.x - (bounds1.w + 10), bounds1.y, self->bounds.w, self->bounds.h);
			self->position = gfc_vector2d(self->bounds.x - 31, self->bounds.y - 82);
		}
	} else {
		self->position = newPos;
		self->bounds = newBounds;
	}
	camera_center_on(self->position);
}

void player_free(Entity *self)
{
	PlayerData *data;
	if ((!self)||(!self->data)) return;
	data = (PlayerData *)self->data;
	inventory_cleanup(&data->inventory);
	free(data);
}
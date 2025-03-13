#include "simple_logger.h"
#include "simple_json.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "camera.h"
#include "world.h"

void world_tile_layer_build(World* world) {
	if (!world) return;
	if (!world->tileset) return;
	if (world->tileLayer) gf2d_sprite_free(world->tileLayer); 

	int i, j;
	Uint32 index;
	GFC_Vector2D position = { 0 };
	Uint32 frame;

	world->tileLayer = gf2d_sprite_new();
	if (!world->tileLayer) slog("impeccable failure, my good man, the tile layer didn't get made");
	world->tileLayer->surface = gf2d_graphics_create_surface(((int)world->tileWidth * world->tileset->frame_w),((int)world->tileHeight * world->tileset->frame_h));

	if (!world->tileLayer->surface) {
		slog("Failed to make the tile layer surface");
		return;
	}
	world->tileLayer->frame_w = world->tileWidth * world->tileset->frame_w;
	world->tileLayer->frame_h = world->tileHeight * world->tileset->frame_h;

	for (j = 0; j < world->tileHeight; j++) {
		for (i = 0; i < world->tileWidth; i++) {
			index = i + (j * world->tileWidth);  
			if (world->tileMap[index] == 0) continue; 
			position.x = i * world->tileset->frame_w;
			position.y = j * world->tileset->frame_h;

			frame = world->tileMap[index] - 1;

			gf2d_sprite_draw_to_surface(
				world->tileset,
				position,
				NULL,
				NULL,
				frame,
				world->tileLayer->surface);
		}
	}
	world->tileLayer->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), world->tileLayer->surface);
	if (!world->tileLayer->texture) {
		slog("Failed to convert world tile layer to a texture");
		return;
	}
}

World *world_load(const char *filename)
{
	World *world = NULL;
	SJson *json = NULL;
	SJson *wjson = NULL;
	SJson *vertical = NULL, *horizontal = NULL;
	SJson *item;
	int tile;
	int w = 0, h = 0;
	int i, j;
	const char *tileset;
	const char *background;
	int frame_w, frame_h;
	int frames_per_line;
	if (!filename)
	{
		slog("no file provided for world loading");
		return NULL;
	}
	json = sj_load(filename);
	if (!json)
	{
		slog("failed to load world file %s", filename);
		return NULL;
	}
	wjson = sj_object_get_value(json, "world");
	if (!wjson)
	{
		slog("%s is missing a 'world' object", filename);
		sj_free(json);
		return NULL;
	}
	vertical = sj_object_get_value(wjson, "tileMap");
	if (!vertical)
	{
		slog("%s is missing a 'tileMap' object", filename);
		sj_free(json);
		return NULL;
	}
	h = sj_array_get_count(vertical);
	horizontal = sj_array_get_nth(vertical, 0);
	w = sj_array_get_count(horizontal);
	world = world_new(w, h);
	if (!world)
	{
		slog("failed to make new world from file %s", filename);
		sj_free(json);
		return NULL;
	}
	for (j = 0; j < h; j++)
	{
		horizontal = sj_array_get_nth(vertical, j);
		if (!horizontal) continue; // admittedly should be checked but i digress
		for (i = 0; i < w; i++)
		{
			item = sj_array_get_nth(horizontal, i);
			if (!item) continue;
			tile = 0;
			sj_get_integer_value(item, &tile);
			world->tileMap[i + (j * w)] = tile;
		}
	}
	tileset = sj_object_get_value_as_string(wjson, "tileset");
	background = sj_object_get_value_as_string(wjson, "background");
	sj_object_get_value_as_int(wjson, "frame_w", &frame_w);
	sj_object_get_value_as_int(wjson, "frame_h", &frame_h);
	sj_object_get_value_as_int(wjson, "frames_per_line", &frames_per_line);
	world->background = gf2d_sprite_load_image(background);
	world->tileset = gf2d_sprite_load_all(tileset, frame_w, frame_h, frames_per_line, 1);
	world_tile_layer_build(world);
	sj_free(json);
	return world;
}

World *world_test_new()
{
	int i;
	int width = 38, height = 23;
	World *world;
	world = world_new(width, height);
	if (!world)
	{
		slog("failed to allocate a new world");
		return NULL;
	}
	world->background = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
	world->tileset = gf2d_sprite_load_all("images/tileset1.png", 32, 32, 16, 1);
	for (i = 0; i < width; i++)
	{
		world->tileMap[i] = 1;
		world->tileMap[i + ((height - 1) * width)] = 1;
	}
	for (i = 0; i < height; i++)
	{
		world->tileMap[i*width] = 1;
		world->tileMap[i * width + (width - 1)] = 1;
	}
	world_tile_layer_build(world);
	return world;
}

World *world_new(Uint32 width, Uint32 height)
{
	World *world;
	if ((!width) || (!height))
	{
		slog("cannot make a world with zero width/height");
		return NULL;
	}
	world = gfc_allocate_array(sizeof(World), 1);
	if (!world)
	{
		slog("failed to allocate a new world");
		return NULL;
	}
	world->tileMap = gfc_allocate_array(sizeof(Uint8), height * width);
	world->tileHeight = height;
	world->tileWidth = width;
	return world;
}

void world_free(World* world)
{
	if (!world) return;
	gf2d_sprite_free(world->background);
	gf2d_sprite_free(world->tileset);
	gf2d_sprite_free(world->tileLayer);
	free(world->tileMap);
	free(world);
}

void world_draw(World* world)
{
	GFC_Vector2D offset;
	if (!world)
	{
		slog("worldn't");
		return;
	}
	offset = camera_get_offset();
	gf2d_sprite_draw_image(world->background, gfc_vector2d(0, 0));
	gf2d_sprite_draw_image(world->tileLayer, offset);
}

void world_setup_camera(World *world)
{
	if (!world) return;
	if ((!world->tileLayer)||(!world->tileLayer->surface))
	{
		slog("no tile layer set for world");
		return;
	}
	camera_set_bounds(gfc_rect(0,0,world->tileLayer->surface->w, world->tileLayer->surface->h));
	camera_apply_bounds();
	camera_enable_binding(1);
}

Uint8 world_get_tile_at(World *world, GFC_Vector2D position)
{
	if ((!world)||(!world->tileMap)) return 0;
	return world->tileMap[(Uint32)position.y * world->tileWidth + (Uint32)position.x];

}

int world_shape_check(World* world, GFC_Shape shape)
{
	if ((!world) || (!world->tileset)) return 0;
	int i, j; 
	Uint8 tileIndex;
	GFC_Rect rect = { 0 };
	GFC_Shape test;
	rect.w = world->tileset->frame_w;
	rect.h = world->tileset->frame_h;
	for (j = 0; j < world->tileMap; j++)
	{
		for (i = 0; i < world->tileMap[j]; i++)
		{
			tileIndex = world_get_tile_at(world, gfc_vector2d(i, j));
			if (!tileIndex) continue;
			rect.x = (i * rect.w);
			rect.y = (j * rect.h);
			test = gfc_shape_from_rect(rect);
			if (gfc_shape_overlap(test, shape)) return 1;
		}
	}
	return 0;
}

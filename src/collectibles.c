#include "simple_logger.h"
#include "simple_json.h"
#include "gfc_config.h"
#include "gfc_text.h"
#include "gfc_vector.h"
#include "gfc_shape.h"
#include "collectibles.h"

SJson *_collectibleJson = { 0 };
SJson *_collectibleDefs = { 0 };

void collectibles_init(const char* filename)
{
	if (!filename)
	{
		slog("no file name");
		return;
	}
	_collectibleJson = sj_load(filename);
	if (!_collectibleJson)
	{
		slog("json failed to load");
		return;
	}
	_collectibleDefs = sj_object_get_value(_collectibleJson, "collectibles");
	if (!_collectibleDefs)
	{
		slog("json has no 'collectibles'");
		sj_free(_collectibleJson);
		return;
	}

	atexit(collectibles_close);
}

void collectibles_close()
{
	if (_collectibleJson)
	{
		sj_free(_collectibleJson);
	}
	_collectibleJson = NULL;
	_collectibleDefs = NULL;
}

SJson* get_collectible_def_by_name(const char* name)
{
	int i, c;
	SJson *collectible;
	const char *collectibleName = NULL;
	if (!name) return NULL;
	if (!_collectibleDefs)
	{
		slog("no collectible definitions");
		return NULL;
	}
	c = sj_array_get_count(_collectibleDefs);
	for (i = 0; i < c; i++)
	{
		collectible = sj_array_get_nth(_collectibleDefs, i);
		if (!collectible) continue;
		collectibleName = sj_object_get_value_as_string(collectible, "name");
		if (!collectibleName) continue;
		if ((gfc_strlcmp(name, collectibleName)) == 0)
		{
			return collectible;
		}
	}
	slog("no collectible with name %s found", name);
	return NULL;
}

Entity *collectible_new(const char* name)
{
	Entity *collectible;
	SJson *collectibleDef;
	collectibleDef = get_collectible_def_by_name(name);
	if (!collectibleDef) return NULL;
	collectible = gfc_allocate_array(sizeof(Entity),1);
	if (!collectible) return NULL;
	gfc_line_cpy(collectible->name, name);
	collectible->count = 1;
	return collectible;
}

void collectible_free(Entity *collectible)
{
	if (!collectible) return;
	gf2d_sprite_free(collectible->sprite);
}

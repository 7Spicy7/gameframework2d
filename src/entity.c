#include "simple_logger.h"
#include "gfc_config.h"
#include "gf2d_graphics.h"
#include "entity.h"

typedef struct 
{
	Uint32 entity_max;
	Entity *entity_list;
}EntitySystem;

static EntitySystem _entity_system = { 0 };

void entity_system_init(Uint32 maxEnts)
{
	if (_entity_system.entity_list)
	{
		slog("cannot have two instances of an entity manager, one is already active");
		return;
	}
	if (!maxEnts)
	{
		slog("cannot allocate 0 entities");
		return;
	}
	_entity_system.entity_list = gfc_allocate_array(sizeof(Entity), maxEnts);
	if (!_entity_system.entity_list)
	{
		slog("failed to allocate global entity list");
		return;
	}
	_entity_system.entity_max = maxEnts;
	atexit(entity_system_close);
}

void entity_configure_from_file(Entity *self, const char *filename)
{
	SJson *json;

}

void entity_configure(Entity *self, SJson *json)
{
	GFC_Vector4D bounds = { 0 };
	GFC_Vector2D frameSize = { 0 };
	Uint32 framesPerLine = 0;
	const char *sprite = NULL;
	if ((!self) || (!json))return;
	sprite = sj_object_get_string(json, "sprite");
	if (sprite)
	{
		sj_object_get_vector2d(json, "spriteSize", &frameSize);
		sj_object_get_uint32(json, "spriteFPL", &framesPerLine);
		self->sprite = gf2d_sprite_load_all(
			sprite,
			(Uint32)frameSize.x,
			(Uint32)frameSize.y,
			framesPerLine,
			0
		);
	}
	sj_object_get_float(json, "speedMax", &self->speedMax);
	sprite = sj_object_get_string(json, "name");
	if (sprite)gfc_line_cpy(self->name, sprite);
	sj_object_get_vector4d(json,"bounds",&bounds);

}

void entity_system_close()
{
	entity_clear_all(NULL);
	if (_entity_system.entity_list)
	{
		free(_entity_system.entity_list);
	}
	memset(&_entity_system, 0, sizeof(EntitySystem));
	slog("entity system closed");
}

void entity_clear_all(Entity* ignore)
{
	int i;
	for (i = 0; i < _entity_system.entity_max; i++)
	{
		if (!_entity_system.entity_list[i]._inuse)
		{
			continue;
		}
		if (&_entity_system.entity_list[i] == ignore)
		{
			continue;
		}
		entity_free(&_entity_system.entity_list[i]);
	}
}

void entity_system_draw_all()
{
	int i;
	for (i = 0; i < _entity_system.entity_max; i++)
	{
		if (!_entity_system.entity_list[i]._inuse)continue;
		entity_draw(&_entity_system.entity_list[i]);
	}
}

void entity_think(Entity *self)
{
	if (!self)return;
	if (self->think)
	{
		self->think(self);
	}
}

void entity_system_think_all()
{
	int i;
	for (i = 0; i < _entity_system.entity_max; i++)
	{
		if (!_entity_system.entity_list[i]._inuse)continue;
		entity_think(&_entity_system.entity_list[i]);
	}

}void entity_update(Entity* self)
{
	if (!self)return;
	if (self->update)
	{
		self->update(self);
	}
}

void entity_system_update_all()
{
	int i;
	for (i = 0; i < _entity_system.entity_max; i++)
	{
		if (!_entity_system.entity_list[i]._inuse)continue;
		entity_update(&_entity_system.entity_list[i]);
	}
}


Entity *entity_new()
{
	int i;
	for (i = 0; i < _entity_system.entity_max; i++)
	{
		if (_entity_system.entity_list[i]._inuse)continue;
		memset(&_entity_system.entity_list[i], 0, sizeof(Entity));
		_entity_system.entity_list[i]._inuse = 1;
		return &_entity_system.entity_list[i];
	}
	slog("no more available entities");
	return NULL;
}

void entity_free(Entity *self)
{
	if (!self)return;
	if (self->sprite)
	{
		gf2d_sprite_free(self->sprite);
	}
	if (self->free)
	{
		self->free(self->data);
	}
}

void entity_draw(Entity *self)
{
	if (!self)return;
	if (!self->sprite)return;
	gf2d_sprite_draw(
		self->sprite,
		self->position,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(Uint32)self->frame);
}

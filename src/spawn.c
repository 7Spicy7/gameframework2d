#include "simple_logger.h"
#include "enemy.h"

typedef struct
{
	const char* name;
	const char* defFile;
	Entity* spawn(GFC_Vector2D position, const char* defFile);
}SpawnPair;

static SpawnPair spawn_list[] = 
{
	{
		"monster_bug",
		"defs/bug.def"
		enemy_new_entity
	},
	{0}
}

Entity* spawn_entity(const char* name, GFC_Vector2D position)
{
	int i;
	if (!name)
	{
		slog("Spawn Failed: No name provided");
		return NULL;
	}
	for (i = 0; spawn_list[i].name != NULL; i++)
	{
		if (gfc_strlcmp(spawn_list[i].name, name) == 0)
		{
			if (spawn_list[i].spawn)
			{
				return spawn_list[i].spawn(position, spawn_list[i].defFile)
			}
		}
	}
	return NULL;
}

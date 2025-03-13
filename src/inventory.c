#include "simple_logger.h"

#include "inventory.h"

void inventory_init(Inventory* inventory)
{
	if (!inventory) return;
	inventory->collectibleList = gfc_list_new();
}

void inventory_cleanup(Inventory* inventory)
{
	if (!inventory) return;
	gfc_list_foreach(inventory->collectibleList,(gfc_work_func*)collectible_free);
	gfc_list_delete(inventory->collectibleList);
}

void inventory_add_item(Inventory* inventory, const char* name)
{
	Collectible *collectible;
	if ((!inventory) || (!name)) return;
	collectible = inventory_get_collectible_by_name(inventory, name);
	if (collectible)
	{
		collectible->count++;
		return;
	}
	collectible = collectible_new(name);
	if (!collectible) return;
	gfc_list_append(inventory->collectibleList, collectible);
}

Collectible *inventory_get_collectible_by_name(Inventory* inventory, const char* name)
{
	Collectible *collectible;
	int i, c;
	if (!inventory) return NULL;
	c = gfc_list_get_count(inventory->collectibleList);
	for (i = 0; i < c; i++)
	{
		collectible = gfc_list_get_nth(inventory->collectibleList, i);
		if (!collectible) continue;
		if (gfc_strlcmp(collectible->name, name) == 0)
		{
			return collectible;
		}
	}

	return NULL;
}

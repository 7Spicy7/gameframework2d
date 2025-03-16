#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include "gfc_list.h"
#include "collectibles.h"

#define MAX_COLLECTIBLES 128

typedef enum
{
	CI_Coin,
	CI_TopHat,
	CI_MAX
}CollectibleIndex;


typedef struct
{
	GFC_List *collectibleList;
}Inventory;

void inventory_init(Inventory *inventory);

void inventory_cleanup(Inventory *inventory);

void inventory_add_item(Inventory *inventory, const char *name);

Entity *inventory_get_collectible_by_name(Inventory *inventory, const char *name);

#endif
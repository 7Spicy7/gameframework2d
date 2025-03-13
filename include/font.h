#ifndef __FONT_H__
#define __FONT_H__

#include "gfc_text.h"
#include "gfc_color.h"

typedef enum
{
	FS_small,
	FS_medium,
	FS_large,
	FS_MAX
}FontStyles;

void font_close();

void font_init();

void font_draw_test(const char* text, FontStyles style, GFC_Color color, GFC_Vector2D position);

#endif
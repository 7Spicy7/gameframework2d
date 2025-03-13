#include <SDL_ttf.h>
#include "simple_logger.h"
#include "SDL_surface.h"

#include "gfc_list.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "font.h"

typedef struct
{
	GFC_List *fonts;
}FontManager;

static FontManager font_manager = { 0 };

void font_close()
{
	TTF_Font* font;
	int i, c;
	c = gfc_list_get_count(font_manager.fonts);
	for (i = 0; i < c; i++)
	{
		font = gfc_list_get_nth(font_manager.fonts, i);
		if (!font) continue;
		TTF_CloseFont(font);
	}
	gfc_list_delete(font_manager.fonts);
	memset(&font_manager, 0, sizeof(FontManager));
	TTF_Quit();
}

void font_init()
{
	TTF_Font* font;
	int i;
	if (TTF_Init() != 0)
	{
		slog("failed to initialize TTF");
		return;
	}
	font_manager.fonts = gfc_list_new();
	for (i = 0; i < FS_MAX; i++)
	{
		font = TTF_OpenFont("fonts/QuirkyRobot.ttf", 10 + (i*7));
		if (!font)
		{
			slog("failed to open fonts");
			continue;
		}
		gfc_list_append(font_manager.fonts, font);
	}
	atexit(font_close);
}

void font_draw_test(const char* text, FontStyles style, GFC_Color color, GFC_Vector2D position)
{
	TTF_Font* font;
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Color fg;
	SDL_Rect rect;
	font = gfc_list_get_nth(font_manager.fonts, style);
	if (!font)
	{
		slog("failed to render text");
		return;
	}
	fg = gfc_color_to_sdl(color);
	surface = TTF_RenderUTF8_Blended_Wrapped(font, text, fg, 0);
	if (!surface)
	{
		slog("error with surface");
		return;
	}
	surface = gf2d_graphics_screen_convert(&surface);
	if (!surface)
	{
		slog("error with surface (the second)");
		return;
	}
	texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surface);
	if (!texture)
	{
		SDL_FreeSurface(surface);
		slog("error with texture");
		return;
	}
	rect.x = position.x;
	rect.y = position.y;
	rect.w = surface->w;
	rect.h = surface->h;
	SDL_RenderCopy(gf2d_graphics_get_renderer(), texture, NULL, &rect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

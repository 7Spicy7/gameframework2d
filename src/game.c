#include <SDL.h>
#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "camera.h"
#include "entity.h"
#include "player.h"
#include "enemy.h"
#include "collectibles.h"
#include "world.h"
#include "font.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Entity *player;
    Entity *lilbug;

    World *world;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    GFC_Color mouseGFC_Color = gfc_color8(0,200,205,200);
    
    /*program initializtion*/
    init_logger("gf2d.log",0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        gfc_vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    entity_system_init(1024);
    font_init();
    SDL_ShowCursor(SDL_DISABLE);
    camera_set_size(gfc_vector2d(1200,720));

    
    /*demo setup*/
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);
    player = player_new_entity();
    lilbug = enemy_new_entity();
    world = world_load("levels/testLevel.level");
    world_setup_camera(world);
    slog("passed the world");
    slog("press [escape] to quit");
    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;

        entity_system_think_all();
       
        if (entity_collision_check(player, lilbug))
        {
            lilbug->sprite = gf2d_sprite_load_all("images/lilbug_owie.png", 128, 128, 1, 0);
        } else {
            lilbug->sprite = gf2d_sprite_load_all("images/lilbug.png", 128, 128, 1, 0);
        }
        entity_system_update_all();

        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            world_draw(world);

            entity_system_draw_all();
            
            font_draw_test("Press ESC to exit\nHow funky!",FS_large, GFC_COLOR_WHITE, gfc_vector2d(10, 10));
            //UI elements last
            gf2d_sprite_draw(
                mouse,
                gfc_vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseGFC_Color,
                (int)mf);

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    entity_free(player);
    world_free(world);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/

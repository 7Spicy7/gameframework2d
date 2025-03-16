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
    Entity *swoopybug;
    Entity* lilbug;
    Entity* beefybug;
    Entity* brutebug;
    Entity* bubblecrab;
    Entity* thepit;
    Entity* geyser;
    Entity* stalagmites;
    Entity* stalactite;
    Entity* rollingstone;
    Entity* tophat;
    World *world;
    
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
    collectibles_init("defs/collectibles.def");
    SDL_ShowCursor(SDL_DISABLE);
    camera_set_size(gfc_vector2d(1200,720));
    
    
    /*demo setup*/
    player = player_new_entity();
    tophat = collectible_new("tophat");
    tophat->position = gfc_vector2d(500, 200);
    swoopybug = enemy_load("defs/swoopybug.def", gfc_vector2d(728, 64));
    lilbug = enemy_load("defs/lilbug.def", gfc_vector2d(1264, 258));
    beefybug = enemy_load("defs/beefybug.def", gfc_vector2d(1264, 256));
    brutebug = enemy_load("defs/brutebug.def", gfc_vector2d(1264, 260));
    bubblecrab = enemy_load("defs/bubblecrab.def", gfc_vector2d(1200, 611));
    thepit = enemy_load("defs/thepit.def", gfc_vector2d(32, 768));
    geyser = enemy_load("defs/geyser.def", gfc_vector2d(352,384));
    stalagmites = enemy_load("defs/stalagmites.def", gfc_vector2d(640, 576));
    stalactite = enemy_load("defs/stalactite.def", gfc_vector2d(1600,33));
    rollingstone = enemy_load("defs/rollingstone.def", gfc_vector2d(1632, 607));
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

        entity_system_think_all();
       
        if (entity_collision_check(player, swoopybug))
        {
            swoopybug->sprite = gf2d_sprite_load_all("images/swoopybug_owie.png", 128, 128, 1, 0);
        } else {
            swoopybug->sprite = gf2d_sprite_load_all("images/swoopybug.png", 128, 128, 1, 0);
        }
        if (entity_collision_check(player, lilbug))
        {
            lilbug->sprite = gf2d_sprite_load_all("images/lilbug_owie.png", 128, 128, 1, 0);
        } else {
            lilbug->sprite = gf2d_sprite_load_all("images/lilbug.png", 128, 128, 1, 0);
        }
        if (entity_collision_check(player, beefybug))
        {
            beefybug->sprite = gf2d_sprite_load_all("images/beefybug_owie.png", 128, 128, 1, 0);
        }
        else {
            beefybug->sprite = gf2d_sprite_load_all("images/beefybug.png", 128, 128, 1, 0);
        }
        if (entity_collision_check(player, brutebug))
        {
            brutebug->sprite = gf2d_sprite_load_all("images/brutebug_owie.png", 128, 128, 1, 0);
        }
        else {
            brutebug->sprite = gf2d_sprite_load_all("images/brutebug.png", 128, 128, 1, 0);
        }
        if (entity_collision_check(player, bubblecrab))
        {
            bubblecrab->sprite = gf2d_sprite_load_all("images/bubblecrab_owie.png", 128, 128, 1, 0);
        }
        else {
            bubblecrab->sprite = gf2d_sprite_load_all("images/bubblecrab.png", 128, 128, 1, 0);
        }
        if (entity_collision_check(player, thepit))
        {
            player->sprite = gf2d_sprite_load_all("images/orbobaseidleloop_owie.png", 128, 128, 16, 0);
        }
        else {
            player->sprite = gf2d_sprite_load_all("images/orbobaseidleloop.png", 128, 128, 16, 0);
        }
        if (entity_collision_check(player, geyser))
        {
            player->sprite = gf2d_sprite_load_all("images/orbobaseidleloop_owie.png", 128, 128, 16, 0);
            player->velocity.y -= 3;
        }
        else {
            player->sprite = gf2d_sprite_load_all("images/orbobaseidleloop.png", 128, 128, 16, 0);
        }
        if (entity_collision_check(player, stalagmites))
        {
            player->sprite = gf2d_sprite_load_all("images/orbobaseidleloop_owie.png", 128, 128, 16, 0);
        }
        else {
            player->sprite = gf2d_sprite_load_all("images/orbobaseidleloop.png", 128, 128, 16, 0);
        }
        if (entity_collision_check(player, stalactite))
        {
            player->sprite = gf2d_sprite_load_all("images/orbobaseidleloop_owie.png", 128, 128, 16, 0);
        }
        else {
            player->sprite = gf2d_sprite_load_all("images/orbobaseidleloop.png", 128, 128, 16, 0);
        }
        if (entity_closeness_check_x(stalactite, player, 15)) 
        {
            stalactite->directiony = 1;
        }
        if (entity_collision_check(player, rollingstone))
        {
            player->sprite = gf2d_sprite_load_all("images/orbobaseidleloop_owie.png", 128, 128, 16, 0);
        }
        else {
            player->sprite = gf2d_sprite_load_all("images/orbobaseidleloop.png", 128, 128, 16, 0);
        }
        entity_system_update_all();

        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            world_draw(world);

            entity_system_draw_all();
            
            font_draw_test("Press ESC to exit\nHow funky!",FS_large, GFC_COLOR_WHITE, gfc_vector2d(10, 10));
            //UI elements last

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    entity_free(player);
    entity_free(swoopybug);
    entity_free(lilbug);
    world_free(world);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/

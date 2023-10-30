#include "common.h"

#include "defs.h"
#include "draw.h"
#include "structs.h"
#include "entities.h"
#include "util.h"
#include <stdlib.h>

inline void draw_entity_of_type(Entity_Type TYPE, char DEBUG);
inline void draw_debug_rect(Entity *e);

inline Entity* get_bullet_inactive(void);
inline void handle_death_bullet(Entity *BULLET);
inline void fire_pistol(Entity E);

inline void spawn_enemy(Entity *E);
inline void handle_death_enemy(Entity *ENEMY);
inline Entity* get_enemy_inactive(void);

inline void spawn_pickup_coin(Entity E);
inline Entity* get_pickup_coin_inactive(void);
inline void handle_death_pickup_coin(Entity *COIN);

inline void spawn_pickup_hp(Entity E);
inline Entity* get_pickup_hp_inactive(void);
inline void handle_death_pickup_hp(Entity *HPP);

Entity player;
char player_facing_right = true;
float player_reload = .10;
float current_reload = 0;

int player_score = 0;

char player_take_dmg = false;
float p_take_dmg_interval = .5f;
float p_take_dmg_timer = 0;

float enemy_spawn_interval = 1;
float enemy_spawn_timer = 3;

void init_entities(void)
{
    Entity e;
    stage.entity_count = 0;
    player_facing_right = true;
    player_reload = .10;
    current_reload = 0;

    player_score = 0;

    player_take_dmg = false;
    p_take_dmg_interval = .5f;
    p_take_dmg_timer = 0;

    enemy_spawn_interval = .5;
    enemy_spawn_timer = 3;

    { // Player
        player.type = ENT_PLAYER;
        player.active = true;
        player.hp = 3;
        player.rect.x = get_scr_width_scaled() / 2;
        player.rect.y = get_scr_height_scaled() / 2 + 115;

        player.sprite.src.x = 0;
        player.sprite.src.y = 0;
        player.sprite.src.w = 21;
        player.sprite.src.h = 28;

        player.rect.w = player.sprite.src.w;
        player.rect.h = player.sprite.src.h;

        player.sprite.tex = load_texture("assets/player.png");

        player.hitbox = player.rect;

        stage.entities_pool[0] = player;
        stage.entity_count++;
    }
    { // Background
        e.type = ENT_BACKGROUND;
        e.active = true;
        e.rect.x = get_scr_width_scaled() / 2;
        e.rect.y = get_scr_height_scaled() / 2;

        e.sprite.src.x = 0;
        e.sprite.src.y = 0;
        e.sprite.src.w = 320;
        e.sprite.src.h = 180;

        e.rect.w = e.sprite.src.w;
        e.rect.h = e.sprite.src.h;

        e.sprite.tex = load_texture("assets/background.png");

        e.hitbox = e.rect;

        stage.entities_pool[stage.entity_count] = e;
        stage.entity_count++;
    }
    { // Tiles
        e.type = ENT_TILE;
        e.active = true;
        e.rect.x = get_scr_width_scaled() / 2;
        e.rect.y = get_scr_height_scaled() / 2;

        e.sprite.src.x = 0;
        e.sprite.src.y = 0;
        e.sprite.src.w = 320;
        e.sprite.src.h = 180;

        e.rect.w = e.sprite.src.w;
        e.rect.h = e.sprite.src.h;

        e.sprite.tex = load_texture("assets/tiles.png");

        e.hitbox = e.rect;

        stage.entities_pool[stage.entity_count] = e;
        stage.entity_count++;
    }
    { // Tiles hitbox
        e.type = ENT_TILE_HITBOX;
        e.active = true;
        e.rect.x = 10;
        e.rect.y = get_scr_height_scaled() / 2 + 100;

        e.sprite.src.x = 0;
        e.sprite.src.y = 0;
        e.sprite.src.w = 320;
        e.sprite.src.h = 180;

        e.rect.w = 25;
        e.rect.h = 50;

        e.sprite.tex = load_texture("assets/tiles.png");

        e.hitbox = e.rect;

        stage.entities_pool[stage.entity_count] = e;
        stage.entity_count++;

        e.type = ENT_TILE_HITBOX;
        e.active = true;
        e.rect.x = get_scr_width_scaled() - 10;
        e.rect.y = get_scr_height_scaled() / 2 + 100;

        e.sprite.src.x = 0;
        e.sprite.src.y = 0;
        e.sprite.src.w = 320;
        e.sprite.src.h = 180;

        e.rect.w = 25;
        e.rect.h = 50;

        e.sprite.tex = load_texture("assets/tiles.png");

        e.hitbox = e.rect;

        stage.entities_pool[stage.entity_count] = e;
        stage.entity_count++;
    }
    { // Enemies 
        int i = 0;
        for(i = 0; i < 48; i++)
        {
            e.id = stage.entity_count;
            e.type =  ENT_ENEMY;
            e.active = false;
            e.hp = 1;
            e.dmg = 1;
            e.rect.x = get_scr_width_scaled() / 2 + 350;
            e.rect.y = get_scr_height_scaled() / 2 + 150;

            e.sprite.src.x = 0;
            e.sprite.src.y = 0;
            e.sprite.src.w = 10;
            e.sprite.src.h = 10;

            e.rect.w = e.sprite.src.w;
            e.rect.h = e.sprite.src.h;

            e.sprite.tex = load_texture("assets/enemy_robot.png");

            e.hitbox = e.rect;

            stage.entities_pool[stage.entity_count] = e;
            stage.entity_count++;
        }
    }
    { // Player Pistol
        e.type = ENT_P_PISTOL,
        e.active = true;
        e.rect.x = get_scr_width_scaled() / 2 + 20;
        e.rect.y = get_scr_height_scaled() / 2 + 135;

        e.sprite.src.x = 0;
        e.sprite.src.y = 0;
        e.sprite.src.w = 15;
        e.sprite.src.h = 9;

        e.rect.w = e.sprite.src.w;
        e.rect.h = e.sprite.src.h;

        e.sprite.tex = load_texture("assets/player_pistol.png");

        e.hitbox = e.rect;

        stage.entities_pool[stage.entity_count] = e;
        stage.entity_count++;
    }
    { // Barrel
        e.type = ENT_BARREL,
        e.active = true;
        e.rect.x = get_scr_width_scaled() / 2 + 120;
        e.rect.y = get_scr_height_scaled() / 2 + 130;

        e.sprite.src.x = 0;
        e.sprite.src.y = 0;
        e.sprite.src.w = 15;
        e.sprite.src.h = 15;

        e.rect.w = e.sprite.src.w;
        e.rect.h = e.sprite.src.h;

        e.sprite.tex = load_texture("assets/barrels.png");

        e.hitbox = e.rect;

        stage.entities_pool[stage.entity_count] = e;
        stage.entity_count++;
    }
    { // UI Player HP
        e.type = ENT_UI_P_HEALTH,
        e.active = true;
        e.rect.x = get_scr_width_scaled() / 2 - 20;
        e.rect.y = get_scr_height_scaled() / 2 + 270;

        e.sprite.src.x = 45;
        e.sprite.src.y = 13;
        e.sprite.src.w = 33;
        e.sprite.src.h = 13;

        e.rect.w = e.sprite.src.w;
        e.rect.h = e.sprite.src.h;

        e.sprite.tex = load_texture("assets/ui_elements.png");

        e.hitbox = e.rect;

        stage.entities_pool[stage.entity_count] = e;
        stage.entity_count++;
    }
    { // UI Player Ammo
        e.type = ENT_UI_P_BULLETS,
        e.active = true;
        e.rect.x = get_scr_width_scaled() / 2 + 140;
        e.rect.y = get_scr_height_scaled() / 2 + 270;

        e.sprite.src.x = 12;
        e.sprite.src.y = 0;
        e.sprite.src.w = 33;
        e.sprite.src.h = 13;

        e.rect.w = e.sprite.src.w;
        e.rect.h = e.sprite.src.h;

        e.sprite.tex = load_texture("assets/ui_elements.png");

        e.hitbox = e.rect;

        stage.entities_pool[stage.entity_count] = e;
        stage.entity_count++;
    }
    { // UI Player Score
        e.type = ENT_UI_P_SCORE,
        e.active = true;
        e.rect.x = get_scr_width_scaled() / 2 - 70;
        e.rect.y = get_scr_height_scaled() / 2 - 300;

        e.sprite.src.x = 0;
        e.sprite.src.y = 0;
        e.sprite.src.w = 8;
        e.sprite.src.h = 8;

        e.rect.w = e.sprite.src.w;
        e.rect.h = e.sprite.src.h;

        e.sprite.tex = load_texture("assets/coin.png");

        e.hitbox = e.rect;

        stage.entities_pool[stage.entity_count] = e;
        stage.entity_count++;
    }
    { // Pickup HP
        for(int i = 0; i < 10; i++)
        {
            e.type = ENT_PICKUP_HP,
            e.active = false;
            e.rect.x = get_scr_width_scaled() / 2 - 140;
            e.rect.y = get_scr_height_scaled() / 2 + 120;

            e.sprite.src.x = 0;
            e.sprite.src.y = 13;
            e.sprite.src.w = 12;
            e.sprite.src.h = 13;

            e.rect.w = e.sprite.src.w;
            e.rect.h = e.sprite.src.h;

            e.sprite.tex = load_texture("assets/ui_elements.png");

            e.hitbox = e.rect;

            stage.entities_pool[stage.entity_count] = e;
            stage.entity_count++;
        }
    }
    { // Pickup Power Up
        e.type = ENT_PICKUP_POWERUP,
        e.active = true;
        e.rect.x = get_scr_width_scaled() / 2 - 200;
        e.rect.y = get_scr_height_scaled() / 2 + 140;

        e.sprite.src.x = 2;
        e.sprite.src.y = 0;
        e.sprite.src.w = 9;
        e.sprite.src.h = 13;

        e.rect.w = e.sprite.src.w;
        e.rect.h = e.sprite.src.h;

        e.sprite.tex = load_texture("assets/ui_elements.png");

        e.hitbox = e.rect;

        stage.entities_pool[stage.entity_count] = e;
        stage.entity_count++;
    }
    { // Pickup Coin
        for(int i = 0; i < 36; i++)
        {
            e.type = ENT_PICKUP_COIN,
            e.active = false;
            e.rect.x = get_scr_width_scaled() / 2 - 140;
            e.rect.y = get_scr_height_scaled() / 2 + 120;

            e.sprite.src.x = 0;
            e.sprite.src.y = 0;
            e.sprite.src.w = 8;
            e.sprite.src.h = 8;

            e.rect.w = e.sprite.src.w;
            e.rect.h = e.sprite.src.h;

            e.sprite.tex = load_texture("assets/coin.png");

            e.hitbox = e.rect;

            stage.entities_pool[stage.entity_count] = e;
            stage.entity_count++;
        }
    }
    { // Player bullets
        int i = 0;
        for(i = 0; i < 32; i++)
        {
            e.id = rand();
            e.type = ENT_P_BULLET,
            e.active = false;
            e.dmg = 1;
            e.rect.x = get_scr_width_scaled() / 2;
            e.rect.y = get_scr_height_scaled() / 2;

            e.sprite.src.x = 0;
            e.sprite.src.y = 1;
            e.sprite.src.w = 9;
            e.sprite.src.h = 4;

            e.rect.w = e.sprite.src.w;
            e.rect.h = e.sprite.src.h;

            e.sprite.tex = load_texture("assets/bullets.png");

            e.hitbox = e.rect;

            stage.entities_pool[stage.entity_count] = e;
            stage.entity_count++;
        }
    }

    SDL_Log("Total Entities initialised: %i", stage.entity_count);
}

void update_entities(void)
{
    Entity *e;
    Entity *player = &stage.entities_pool[0];

    current_reload += (game.dt / 1000.f);
    enemy_spawn_timer += (game.dt / 1000.f);

    if(player_take_dmg)
    {
        p_take_dmg_timer += (game.dt / 1000.f);
    }

    // Update entities by type
    int i = 0;
    for(i = 0; i < stage.entity_count; i++)
    {
        e = &stage.entities_pool[i];

        switch(e->type)
        {
            case ENT_PLAYER:
                {
                    if(e->active == true)
                    {
                        e->vel.x = e->vel.y = 0;

                        if(game.keyboard[SDL_SCANCODE_A])
                        {
                            e->vel.x = -PLAYER_VELOCITY;
                            player_facing_right = false;
                        }
                        if(game.keyboard[SDL_SCANCODE_D])
                        {
                            e->vel.x = PLAYER_VELOCITY;
                            player_facing_right = true;
                        }

                
                        e->rect.x += e->vel.x * game.dt;
                        e->rect.y += e->vel.y * game.dt;
                        //SDL_Log("Player x: %i, Player y: %i", e->rect.x, e->rect.y);
                    }
                    break;
                }
            case ENT_P_PISTOL:
                {
                    if(e->active == true)
                    {
                        if(player_facing_right)
                        {
                            e->rect.x = player->rect.x + 15;
                            e->rect.y = player->rect.y + 20;
                        }
                        else if(!player_facing_right)
                        {
                            e->rect.x = player->rect.x - 15;
                            e->rect.y = player->rect.y + 20;

                        }

                        if(game.keyboard[SDL_SCANCODE_SPACE])
                        {
                            if(current_reload > player_reload)
                            {
                                fire_pistol(*e);

                                current_reload = 0;
                            }
                        }
                    }

                    break;
                }
            case ENT_P_SHOTGUN:
                {
                    if(e->active == true)
                    {
                        e->rect.x = player->rect.x + 15;
                        e->rect.y = player->rect.y + 20;
                    }
                    break;
                }
            case ENT_P_MACHINEGUN:
                {
                    if(e->active == true)
                    {
                        e->rect.x = player->rect.x + 15;
                        e->rect.y = player->rect.y + 20;
                    }
                    break;
                }
            case ENT_P_BULLET:
                {
                    if(e->active == true)
                    {
                        e->rect.x += e->vel.x * game.dt;
                        //e->rect.y += e->vel.y * game.dt;

                        if(e->rect.x > get_scr_width_scaled() + 100)
                        {
                            handle_death_bullet(e);
                        }
                        if(e->rect.x < - 100)
                        {
                            handle_death_bullet(e);
                        }
                        //5 52 
                       
                        for(int eID = 5; eID < 53; eID++)
                        {
                            SDL_Rect bul, enem;
                            bul = e->rect;
                            bul.w *= SCREEN_SCALE;
                            bul.h *= SCREEN_SCALE;
                            bul.x -= (bul.w / 2);
                            bul.y -= (bul.h / 2);

                            enem = stage.entities_pool[eID].rect;
                            enem.w *= SCREEN_SCALE;
                            enem.h *= SCREEN_SCALE;
                            enem.x -= (enem.w / 2);
                            enem.y -= (enem.h / 2);
                            
                            if(stage.entities_pool[eID].active == true)
                            {
                                if(SDL_HasIntersection(&bul, &enem))
                                {
                                    //TODO: take into account dmg and hp
                                    handle_death_bullet(e);
                                    handle_death_enemy(&stage.entities_pool[eID]);
                                };
                            }
                        }
                    }
                    break;
                }
            case ENT_UI_P_HEALTH:
                {
                    break;
                }
            case ENT_UI_P_BULLETS:
                {
                    break;
                }
            case ENT_UI_P_SCORE:
                {
                    break;
                }
            case ENT_BACKGROUND:
                {
                    break;
                }
            case ENT_TILE:
                {
                    break;
                }
            case ENT_TILE_HITBOX:
                {
                    if(e->active)
                    {
                        SDL_Rect p_r, t_r;
                        p_r = player->rect;
                        p_r.w *= SCREEN_SCALE;
                        p_r.h *= SCREEN_SCALE;
                        p_r.x -= (p_r.w / 2);
                        p_r.y -= (p_r.h / 2);

                        t_r = e->rect;
                        t_r.w *= SCREEN_SCALE;
                        t_r.h *= SCREEN_SCALE;
                        t_r.x -= (t_r.w / 2);
                        t_r.y -= (t_r.h / 2);

                        if(SDL_HasIntersection(&p_r, &t_r))
                        {
                            if(p_r.x > t_r.x)
                            {
                                player->rect.x = 100;
                            }
                            if(p_r.x < t_r.x)
                            {
                                player->rect.x = t_r.x - 48;
                            }
                        };
                    }
                    break;
                }
            case ENT_BARREL:
                {
                    break;
                }
            case ENT_PICKUP_HP:
                {
                    if(e->active)
                    {
                        SDL_Rect p_r, t_r;
                        p_r = player->rect;
                        p_r.w *= SCREEN_SCALE;
                        p_r.h *= SCREEN_SCALE;
                        p_r.x -= (p_r.w / 2);
                        p_r.y -= (p_r.h / 2);

                        t_r = e->rect;
                        t_r.w *= SCREEN_SCALE;
                        t_r.h *= SCREEN_SCALE;
                        t_r.x -= (t_r.w / 2);
                        t_r.y -= (t_r.h / 2);

                        if(SDL_HasIntersection(&p_r, &t_r))
                        {
                            e->active = false;
                            player->hp++;
                            SDL_Log("POWERUP: Player HP: %i", player->hp);
                        };
                    }
                    break;
                }
            case ENT_PICKUP_POWERUP:
                {
                    break;
                }
            case ENT_PICKUP_COIN:
                {
                    if(e->active)
                    {
                        SDL_Rect p_r, t_r;
                        p_r = player->rect;
                        p_r.w *= SCREEN_SCALE;
                        p_r.h *= SCREEN_SCALE;
                        p_r.x -= (p_r.w / 2);
                        p_r.y -= (p_r.h / 2);

                        t_r = e->rect;
                        t_r.w *= SCREEN_SCALE;
                        t_r.h *= SCREEN_SCALE;
                        t_r.x -= (t_r.w / 2);
                        t_r.y -= (t_r.h / 2);

                        if(SDL_HasIntersection(&p_r, &t_r))
                        {
                            e->active = false;
                            player_score++;
                        };
                    }
                    break;
                }
            case ENT_ENEMY:
                {
                    if(e->active == false AND enemy_spawn_timer > enemy_spawn_interval)
                    {
                        spawn_enemy(e);
                        //SDL_Log("Spawn enemy");
                        enemy_spawn_timer = 0;
                    }

                    if(e->active == true)
                    {
                        e->vel.x = e->vel.y = 0;
                            
                        if(e->rect.x < player->rect.x)
                        {
                            e->vel.x = ENEMY_VELOCITY;
                        }
                        if(e->rect.x > player->rect.x)
                        {
                            e->vel.x = -ENEMY_VELOCITY;
                        }

                        e->rect.x += e->vel.x * game.dt;
                        e->rect.y += e->vel.y * game.dt;
                    }

                    SDL_Rect p_r, t_r;
                    p_r = player->rect;
                    p_r.w *= SCREEN_SCALE;
                    p_r.h *= SCREEN_SCALE;
                    p_r.x -= (p_r.w / 2);
                    p_r.y -= (p_r.h / 2);

                    t_r = e->rect;
                    t_r.w *= SCREEN_SCALE;
                    t_r.h *= SCREEN_SCALE;
                    t_r.x -= (t_r.w / 2);
                    t_r.y -= (t_r.h / 2);

                    if(SDL_HasIntersection(&p_r, &t_r))
                    {
                        player_take_dmg = true;

                        if(player_take_dmg == true AND p_take_dmg_timer > p_take_dmg_interval)
                        {
                            player->hp--;
                            player_take_dmg = false;
                            p_take_dmg_timer = 0;
                            SDL_Log("ENEMY DEAL DMG: Player HP: %i", player->hp);
                        }
                    };
                    
                    break;
                }
            case NUM_ENTITIES:
                {
                    break;
                }
            default:
                break;
        }
    }    

    if(player->hp <= 0)
    {
        game_state = GAME_OVER;
    }
}

void draw_entities(void)
{
    draw_entity_of_type(ENT_BACKGROUND, false);
    draw_entity_of_type(ENT_TILE, false);

    draw_entity_of_type(ENT_TILE_HITBOX, true);

    draw_entity_of_type(ENT_BARREL, true);

    draw_entity_of_type(ENT_PLAYER, true);
    draw_entity_of_type(ENT_P_PISTOL, true);

    draw_entity_of_type(ENT_ENEMY, true);

    draw_entity_of_type(ENT_PICKUP_HP, true);
    draw_entity_of_type(ENT_PICKUP_POWERUP, true);
    draw_entity_of_type(ENT_PICKUP_COIN, true);

    draw_entity_of_type(ENT_P_BULLET, true);

    draw_entity_of_type(ENT_UI_P_HEALTH, false);
    draw_entity_of_type(ENT_UI_P_BULLETS, false);
    draw_entity_of_type(ENT_UI_P_SCORE, false);

    char score_buffer[32];
    sprintf(score_buffer, "%i", player_score);
    SDL_Rect dest = { (get_scr_width_scaled() / 2) - 40, 40, 0, 0};
    render_text(score_buffer, dest, 2.f);
}

inline void draw_entity_of_type(Entity_Type TYPE, char DEBUG)
{
    Entity *e;
    int i = 0;
    for(i = 0; i < stage.entity_count; i++)
    {
        e = &stage.entities_pool[i];
       
        if(e->active == false)
            continue;

        if(e->type == TYPE)
        {
            if(TYPE == ENT_TILE_HITBOX)
            {
                draw_debug_rect(e);
            }
            else if(TYPE == ENT_PLAYER 
                    OR TYPE == ENT_P_PISTOL 
                    OR TYPE == ENT_P_SHOTGUN 
                    OR TYPE == ENT_P_MACHINEGUN)
            {
                if(player_facing_right == true)
                {
                    blit_from_sheet_and_flip(e->sprite.tex, e->rect, e->sprite.src, 0, SCREEN_SCALE, 1, SDL_FLIP_NONE);
                }
                else if (player_facing_right == false)
                {
                    blit_from_sheet_and_flip(e->sprite.tex, e->rect, e->sprite.src, 0, SCREEN_SCALE, 1, SDL_FLIP_HORIZONTAL);
                }
            }
            else
            {
                blit_from_sheet(e->sprite.tex, e->rect, e->sprite.src, 0, SCREEN_SCALE, 1);
            }


            if(DEBUG == true)
            {
                draw_debug_rect(e);
            }
        }
    }
}

inline void draw_debug_rect(Entity *e)
{
    SDL_Rect drect;
    drect = e->rect;
    drect.w *= SCREEN_SCALE;
    drect.h *= SCREEN_SCALE;
    drect.x -= (drect.w / 2);
    drect.y -= (drect.h / 2);
    SDL_SetRenderDrawColor(game.renderer, 200, 0, 0, 255);
    SDL_RenderDrawRect(game.renderer, &drect);
}

inline void fire_pistol(Entity E)
{
    Entity *b = get_bullet_inactive();
    
    b->rect.x = E.rect.x + 25;
    b->rect.y = E.rect.y;

    b->active = true;
    
    if(player_facing_right)
    {
        b->vel.x = BULLET_VELOCITY;
    }
    else 
    {
        b->vel.x = -BULLET_VELOCITY;
    }
}

inline Entity* get_bullet_inactive(void)
{
    Entity *b = NULL;
    int i = 0;
    for(i = 0; i < stage.entity_count; i++)
    {
        b = &stage.entities_pool[i];
        if(b->type == ENT_P_BULLET AND b->active == false)
        {
            return(b);
        }
    }

    SDL_Log("Could not find an inactive bullet");
    return(b);
}

inline void handle_death_bullet(Entity *BULLET)
{
    BULLET->active = false;
}

inline void spawn_pickup_hp(Entity E)
{
    Entity *hp = get_pickup_hp_inactive();
    
    hp->rect.x = E.rect.x;
    hp->rect.y = E.rect.y;

    hp->active = true;
}

inline Entity* get_pickup_hp_inactive(void)
{
    Entity *hp = NULL;
    int i = 0;
    for(i = 0; i < stage.entity_count; i++)
    {
        hp = &stage.entities_pool[i];
        if(hp->type == ENT_PICKUP_HP AND hp->active == false)
        {
            return(hp);
        }
    }

    SDL_Log("Could not find an inactive HP pickup");
    return(hp);
}

inline void handle_death_pickup_hp(Entity *HPP)
{
    HPP->active = false;
}

inline void spawn_pickup_coin(Entity E)
{
    Entity *coin = get_pickup_coin_inactive();
    
    coin->rect.x = E.rect.x;
    coin->rect.y = E.rect.y;

    coin->active = true;
}

inline Entity* get_pickup_coin_inactive(void)
{
    Entity *coin = NULL;
    int i = 0;
    for(i = 0; i < stage.entity_count; i++)
    {
        coin = &stage.entities_pool[i];
        if(coin->type == ENT_PICKUP_COIN AND coin->active == false)
        {
            return(coin);
        }
    }

    SDL_Log("Could not find an inactive HP pickup");
    return(coin);
}

inline void handle_death_pickup_coin(Entity *COIN)
{
    COIN->active = false;
}


inline void spawn_enemy(Entity *E)
{
    int spawn_point = (rand() % 2);

    if(spawn_point == 0)
    {
        E->rect.x = get_scr_width_scaled() / 2 - 525;
    }
    else if (spawn_point == 1)
    {
        E->rect.x = get_scr_width_scaled() / 2 + 475;
    }
    E->rect.y = get_scr_height_scaled() / 2 + 150;

    E->active = true;
}

inline Entity* get_enemy_inactive(void)
{
    Entity *e = NULL;
    int i = 0;
    for(i = 0; i < stage.entity_count; i++)
    {
        e = &stage.entities_pool[i];
        if(e->type == ENT_ENEMY AND e->active == false)
        {
            return(e);
        }
    }

    SDL_Log("Could not find an inactive enemy");
    return(e);
}

inline void handle_death_enemy(Entity *ENEMY)
{
    ENEMY->active = false;

    int drop = (rand() % 7);
    // 0/1 - no drop, 2/3/4 - coin, 5 - hp, 6 - ammo pack
    switch(drop)
    {
        case 0:
        case 1:
        {
            SDL_Log("DROP: None");
            break;
        }

        case 2:
        case 3:
        case 4:
        {
            SDL_Log("DROP: Coin");
            spawn_pickup_coin(*ENEMY);
            break;
        }

        case 5:
        {
            SDL_Log("DROP: HP Pack");
            spawn_pickup_hp(*ENEMY);
            break;
        }

        case 6:
        {
            SDL_Log("DROP: Ammo Pack");
            break;
        }
        
        default:
        {
            SDL_Log("DROP: Shouldnt happen - %i", drop);
            break;
        }
    }
}

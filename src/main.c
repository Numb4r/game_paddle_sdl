#include <stdio.h>
#include <SDL2/SDL.h>
#include "./constans.h"
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int game_is_running = FALSE;
int last_frame_time = 0;

struct ball
{
    float x;
    float y;
    float width;
    float height;
    float vel_x;
    float vel_y;
} ball, paddle;

int initialize_window(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL. \n");
        return FALSE;
    }
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS);
    if (!window)
    {
        fprintf(stderr, "Error creating SDL window.\n");
        return FALSE;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        fprintf(stderr, "Error creating SDL Renderer. \n");
        return FALSE;
    }
    return TRUE;
}

void process_input()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        game_is_running = FALSE;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            game_is_running = FALSE;
        if (event.key.keysym.sym == SDLK_LEFT)
            paddle.vel_x = -400;
        if (event.key.keysym.sym == SDLK_RIGHT)
            paddle.vel_x = +400;
        break;
    case SDL_KEYUP:
        if (event.key.keysym.sym == SDLK_LEFT)
            paddle.vel_x = 0;
        if (event.key.keysym.sym == SDLK_RIGHT)
            paddle.vel_x = 0;
        break;
    }
}
void update()
{
    // while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME)); CPU issues,dont use while for delay
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
    {
        SDL_Delay(time_to_wait);
    }
    //Get a delta time factor conveted to seconds to be used to update my  objects
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
    last_frame_time = SDL_GetTicks();

    ball.x += ball.vel_x * delta_time;
    ball.y += ball.vel_y * delta_time;
    paddle.x += paddle.vel_x * delta_time;
    paddle.y += paddle.vel_y * delta_time;

    if (ball.x <= 0 || ball.x + ball.width >= WINDOW_WIDTH)
        ball.vel_x *= -1;

    if (ball.y < 0)
        ball.vel_y *= -1;

    if (ball.y + ball.height >= paddle.y && ball.x + ball.width >= paddle.x && ball.x <= paddle.x + paddle.width)
        ball.vel_y *= -1;

    if (paddle.x <= 0)
        paddle.x = 0;
    if (paddle.x >= WINDOW_WIDTH - paddle.width)
        paddle.x = WINDOW_WIDTH - paddle.width;

    if (ball.y + ball.height > WINDOW_HEIGHT)
    {
        ball.x = WINDOW_WIDTH / 2;
        ball.y = 0;
    }
}
void render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    //TODO: Draw a rectangle
    SDL_Rect ball_rect = {
        (int)ball.x,
        (int)ball.y,
        (int)ball.width,
        (int)ball.height};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ball_rect);

    SDL_Rect paddle_rect = {
        (int)paddle.x,
        (int)paddle.y,
        (int)paddle.width,
        (int)paddle.height};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddle_rect);
    SDL_RenderPresent(renderer); //buffer swap
}
void setup(void)
{
    ball.x = 20;
    ball.y = 20;
    ball.width = 15;
    ball.height = 15;
    ball.vel_x = 300;
    ball.vel_y = 300;

    paddle.width = 100;
    paddle.height = 20;
    paddle.x = (WINDOW_WIDTH / 2) - (paddle.width / 2);
    paddle.y = WINDOW_HEIGHT - 40;
    paddle.vel_y = 0;
    paddle.vel_x = 0;
}
void destroy_window()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
int main()
{
    game_is_running = initialize_window();
    setup();
    while (game_is_running)
    {
        process_input();
        update();
        render();
    }
    destroy_window();
    return 0;
}
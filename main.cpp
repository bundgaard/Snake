#include <algorithm>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <random>

constexpr int WIDTH = 640;
constexpr int HEIGHT = 480;

int main()
{

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    std::random_device rnd;
    std::mt19937 mt_rand(rnd());

    auto in_between = [mt_rand = std::ref<std::mt19937>(mt_rand)](int min, int max)
    {
        auto distribution = std::uniform_int_distribution<>(min, max);
        return distribution(mt_rand.get());
    };

    auto random_width = std::uniform_int_distribution(0, 480);


    auto window = SDL_CreateWindow("Snake",
                                   SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   WIDTH, HEIGHT, 0);
    auto renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
    auto font = TTF_OpenFont(
            "/opt/fonts/font.otf", 36);
    const char *text = "SNAKE";
    auto helloWorldSurface = TTF_RenderText_Blended(font, text, SDL_Color{255, 255, 0, 255});
    auto snakeWord = SDL_CreateTextureFromSurface(renderer, helloWorldSurface);
    int fontWidth;
    int fontHeight;

    SDL_FreeSurface(helloWorldSurface);
    TTF_SizeText(font, text, &fontWidth, &fontHeight);
    SDL_Rect fontRect = {10, 10, fontWidth, fontHeight};

    int x = WIDTH / 2, y = HEIGHT / 2, x_dir = 0, y_dir = 0;

    bool quit = false;
    SDL_Event event;
    SDL_Rect snake_head = {.x = x, .y = y, .w = 10, .h = 10};
    std::vector<SDL_Rect> snake;
    int snake_length = 1;
    int col = WIDTH / snake_head.w;
    int row = HEIGHT / snake_head.h;
    printf("grid %d,%d\n", col, row);

    SDL_Rect food_rect = {
            .x = in_between(0, col - 1) * 10,
            .y = in_between(0, row - 1),
            .w = 10,
            .h = 10};
    std::cout << "col " << col << std::endl;
    std::cout << "row " << row << std::endl;

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            auto type = event.type;
            if (type == SDL_QUIT)
            {
                quit = true;
                break;
            }

            if (type == SDL_KEYUP)
            {
                if (event.key.keysym.sym == SDLK_LEFT)
                {
                    if (x_dir == 1)
                    {
                        break;
                    }
                    x_dir = -1;
                    y_dir = 0;
                }
                if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    if (x_dir == -1)
                    {
                        break;
                    }
                    x_dir = 1;
                    y_dir = 0;
                }
                if (event.key.keysym.sym == SDLK_UP)
                {
                    if (y_dir == 1)
                    {
                        break;
                    }
                    x_dir = 0;
                    y_dir = -1;
                }
                if (event.key.keysym.sym == SDLK_DOWN)
                {
                    if (y_dir == -1)
                    {
                        break;
                    }
                    x_dir = 0;
                    y_dir = 1;
                }
            }
        }


        snake_head.x += x_dir * snake_head.w;
        snake_head.y += y_dir * snake_head.h;

        snake.push_back(snake_head);

        if (snake.size() > snake_length)
        {
            snake.erase(snake.begin());
        }


        if (snake_head.x < 0)
        {
            snake_head.x = 0;
        }
        if (snake_head.x > WIDTH - snake_head.w)
        {
            snake_head.x = WIDTH - snake_head.w;
        }

        if (snake_head.y < 0)
        {
            snake_head.y = 0;
        }
        if (snake_head.y > HEIGHT - snake_head.h)
        {
            snake_head.y = HEIGHT - snake_head.h;
        }

        // EAT

        if (
                snake_head.x < food_rect.x + food_rect.w &&
                snake_head.x + snake_head.w > food_rect.x &&
                snake_head.y < food_rect.y + food_rect.h &&
                snake_head.y + snake_head.h > food_rect.y
                )
        {
            food_rect.x = in_between(0, col - 1) * snake_head.w;
            food_rect.y = in_between(0, row - 1) * snake_head.h;
#ifdef DEBUG
            printf("food @ %d,%d\n", food_rect.x, food_rect.y);
#endif
            snake_length++;
        }


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
#ifdef DEBUG
        SDL_SetRenderDrawColor(renderer, 255,0,0,255);
        SDL_Rect border = {0,0,WIDTH, HEIGHT};
        SDL_RenderDrawRect(renderer, &border);
#endif

        if (x_dir == 0 && y_dir == 0)
        { SDL_RenderCopy(renderer, snakeWord, nullptr, &fontRect); }


        // FOOD
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &food_rect);


        for (const auto &part: snake)
        {

            SDL_SetRenderDrawColor(renderer, 255, 182, 193, 255);
            SDL_RenderFillRect(renderer, &part);

        }


        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / 30);
    }


    SDL_DestroyTexture(snakeWord);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
    return 0;
}

//
// Created by dbundgaard on 2022-07-04.
//

#ifndef SNAKE_SNAKE_H
#define SNAKE_SNAKE_H
#include <SDL2/SDL.h>

class Snake
{
    SDL_Rect head;

public:
    void Update(SDL_Renderer *);
    void Draw(SDL_Renderer *);
};


#endif //SNAKE_SNAKE_H

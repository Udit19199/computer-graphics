#include <SDL2/SDL.h>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

// Clipping rectangle
const int x_min = 200;
const int y_min = 150;
const int x_max = 600;
const int y_max = 450;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

void putpixel(int x, int y, SDL_Color color) {
    // Only plot if inside the clipping window
    if (x >= x_min && x <= x_max && y >= y_min && y <= y_max) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_RenderDrawPoint(renderer, x, y);
    }
}

// Simple DDA line drawing with clipping at the pixel level
void drawLineClipped(int x1, int y1, int x2, int y2, SDL_Color color) {
    int dx = x2 - x1;
    int dy = y2 - y1;

    int steps = std::max(abs(dx), abs(dy));

    float x_inc = dx / (float)steps;
    float y_inc = dy / (float)steps;

    float x = x1;
    float y = y1;

    for (int i = 0; i <= steps; i++) {
        putpixel((int)x, (int)y, color);
        x += x_inc;
        y += y_inc;
    }
}

void drawRect() {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect rect = {x_min, y_min, x_max - x_min, y_max - y_min};
    SDL_RenderDrawRect(renderer, &rect);
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("Basic Line Clipping",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    drawRect();

    // Lines to test
    drawLineClipped(100, 100, 700, 500, {255, 0, 0, 255});   // partly outside
    drawLineClipped(250, 200, 550, 400, {0, 0, 255, 255});   // fully inside
    drawLineClipped(50, 500, 750, 50,  {255, 255, 0, 255});  // crossing

    SDL_RenderPresent(renderer);

    SDL_Event e;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


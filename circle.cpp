#include <SDL2/SDL.h>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

void putpixel(int x, int y) {
    SDL_Rect rect = { x, y, 2, 2 }; // pixel size = 2x2
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red pixels
    SDL_RenderFillRect(renderer, &rect);
}

// Midpoint Circle Algorithm (Bresenham’s Circle)
void drawCircleBresenham(int xc, int yc, int r) {
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while (y >= x) {
        // draw 8 symmetric points
        putpixel(xc + x, yc + y);
        putpixel(xc - x, yc + y);
        putpixel(xc + x, yc - y);
        putpixel(xc - x, yc - y);
        putpixel(xc + y, yc + x);
        putpixel(xc - y, yc + x);
        putpixel(xc + y, yc - x);
        putpixel(xc - y, yc - x);

        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("Bresenham Circle Drawing",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Example circle
    drawCircleBresenham(400, 300, 200);

    SDL_RenderPresent(renderer);

    // Event loop
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


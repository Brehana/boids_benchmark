#include "visualizer.h"
#include "boids.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace
{
    constexpr SDL_Color BACKGROUND_COLOR{204, 246, 178, 255};
    constexpr SDL_Color BOID_COLOR{48, 98, 48, 255};
    constexpr SDL_Color TEXT_COLOR{9, 25, 0, 255};
    constexpr const char* WINDOW_TITLE = "Boids Visualization";
    constexpr const char* FONT_PATH = "../assets/fonts/DejaVuSans.ttf";
    constexpr int FONT_SIZE = 18;
    constexpr int TARGET_FRAME_DELAY_MS = 16; // ~60 FPS pacing

    bool render_text(SDL_Renderer* renderer,
                     TTF_Font* font,
                     const std::string& text,
                     int x,
                     int y)
    {

        SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), TEXT_COLOR);
        if (!surface)
        {
            return false;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture)
        {
            SDL_FreeSurface(surface);
            return false;
        }

        SDL_Rect dst{x, y, surface->w, surface->h};
        SDL_FreeSurface(surface);

        SDL_RenderCopy(renderer, texture, nullptr, &dst);
        SDL_DestroyTexture(texture);
        return true;
    }

    void draw_boids(SDL_Renderer* renderer,
                    const std::vector<Boid>& boids,
                    int window_width,
                    int window_height)
    {
        
        SDL_SetRenderDrawColor(renderer, BOID_COLOR.r, BOID_COLOR.g, BOID_COLOR.b, BOID_COLOR.a);

        for (const auto& b : boids)
        {
            int x = static_cast<int>((b.x / 1000.0f) * window_width);
            int y = static_cast<int>((b.y / 1000.0f) * window_height);

            SDL_Rect rect{x, y, 3, 3};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

int run_visualizer(const VisualizerConfig& config)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    if (TTF_Init() != 0)
    {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        config.window_width,
        config.window_height,
        SDL_WINDOW_SHOWN);

    if (!window)
    {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer)
    {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (!font)
    {
        std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << "\n";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    std::vector<Boid> current = create_boids(config.boid_count);
    std::vector<Boid> next(config.boid_count);

    bool running = true;
    SDL_Event event;

    using clock = std::chrono::steady_clock;
    auto last_fps_update = clock::now();
    int frames = 0;
    double displayed_fps = 0.0;

    while (running)
    {
        auto frame_start = clock::now();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        if (config.thread_count <= 1)
        {
            update_boids_single(current, next);
        }
        else
        {
            update_boids_multithreaded(current, next, config.thread_count);
        }

        current.swap(next);

        SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
        SDL_RenderClear(renderer);

        draw_boids(renderer, current, config.window_width, config.window_height);

        frames++;
        auto now = clock::now();
        std::chrono::duration<double> fps_elapsed = now - last_fps_update;
        if (fps_elapsed.count() >= 1.0)
        {
            displayed_fps = static_cast<double>(frames) / fps_elapsed.count();
            frames = 0;
            last_fps_update = now;
        }

        std::ostringstream overlay;
        overlay.precision(1);
        overlay << std::fixed
                << "Threads: " << config.thread_count
                << "   FPS: " << displayed_fps;

        if (!render_text(renderer, font, overlay.str(), 10, 10))
        {
            std::cerr << "Failed to render overlay text\n";
        }

        SDL_RenderPresent(renderer);

        auto frame_end = clock::now();
        auto frame_ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start).count();

        if (frame_ms < TARGET_FRAME_DELAY_MS)
        {
            SDL_Delay(static_cast<Uint32>(TARGET_FRAME_DELAY_MS - frame_ms));
        }
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
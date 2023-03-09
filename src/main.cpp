#include "SDL.h"

constexpr int SCREEN_WIDTH = 320;
constexpr int SCREEN_HEIGHT = 240;

constexpr int WINDOW_WIDTH = SCREEN_WIDTH * 4;
constexpr int WINDOW_HEIGHT = SCREEN_HEIGHT * 4;

int main()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *window = SDL_CreateWindow("Invaders Formation Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
  SDL_Event event;
  bool running = true;
  float lastTime = static_cast<float>(SDL_GetTicks());

  while (running)
  {
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_QUIT:
        running = false;
      case SDL_KEYDOWN:
      {
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        {
          running = false;
        }
      }
      break;
      default:
        break;
      }
    }

    float currentTime = static_cast<float>(SDL_GetTicks());
    float deltaTime = (currentTime - lastTime) * 0.001f;
    lastTime = currentTime;

    // update
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(renderer);
    // render
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
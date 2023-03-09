#include "SDL.h"

constexpr int SCREEN_WIDTH = 320;
constexpr int SCREEN_HEIGHT = 240;

constexpr int WINDOW_WIDTH = SCREEN_WIDTH * 4;
constexpr int WINDOW_HEIGHT = SCREEN_HEIGHT * 4;

constexpr int INVADER_SPACING = 4;
constexpr int NUM_INVADERS_ACROSS = 10;
constexpr int NUM_INVADERS_DOWN = 4;
constexpr int NUM_INVADERS = NUM_INVADERS_ACROSS * NUM_INVADERS_DOWN;
constexpr int INVADER_WIDTH = 16;
constexpr int INVADER_HEIGHT = 16;

class Invader
{
public:
  bool alive{false};
  float xPos;
  float yPos;
  float xVel;
  float yVel;
  SDL_Color color{0, 0xff, 0, 0xff};
  SDL_Rect rect{-100, -100, INVADER_WIDTH, INVADER_HEIGHT};
  void update(float deltaTime)
  {
    if (!alive)
    {
      return;
    }
    xPos += xVel * deltaTime;
    yPos += yVel * deltaTime;
    rect.x = static_cast<int>(xPos);
    rect.y = static_cast<int>(yPos);
  }
  void draw(SDL_Renderer *renderer)
  {
    if (!alive)
    {
      return;
    }
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
  }
};

class Invaders
{
  float time{0};
  float nextMovement{0.7};
  Invader *invaders{nullptr};

public:
  Invaders()
  {
    invaders = new Invader[NUM_INVADERS];
  }
  ~Invaders()
  {
    delete[] invaders;
  }
  void getIntoFormation()
  {
    int xGap = INVADER_WIDTH + INVADER_SPACING;
    int yGap = INVADER_HEIGHT + INVADER_SPACING;
    for (int i = 0; i < NUM_INVADERS; i++)
    {
      Invader &obj = invaders[i];
      // move to column
      obj.xPos = static_cast<int>((i % NUM_INVADERS_ACROSS) * xGap);
      // move to row
      obj.yPos = static_cast<int>((i / NUM_INVADERS_ACROSS) * yGap);
      // move to center x form
      obj.xPos += (SCREEN_WIDTH - (xGap * NUM_INVADERS_ACROSS)) / 2;
      // move down an invader height
      // obj.yPos += (SCREEN_HEIGHT - (yGap * NUM_INVADERS_DOWN)) / 2;
      // move up an invader height
      obj.yPos += INVADER_HEIGHT;

      obj.alive = true;
    }
  }

  void update(float deltaTime)
  {
    time += deltaTime;
    if (time >= nextMovement)
    {
      time -= nextMovement;
      for (int i = 0; i < NUM_INVADERS; i++)
      {
        Invader &obj = invaders[i];
        obj.update(deltaTime);
      }
    }
  }
  void draw(SDL_Renderer *renderer)
  {
    for (int i = 0; i < NUM_INVADERS; i++)
    {
      Invader &obj = invaders[i];
      obj.draw(renderer);
    }
  }
};

int main()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *window = SDL_CreateWindow("Invaders Formation Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
  SDL_Event event;
  bool running = true;
  float lastTime = static_cast<float>(SDL_GetTicks());

  Invaders invaders;

  invaders.getIntoFormation();

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
    invaders.update(deltaTime);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(renderer);
    // render
    invaders.draw(renderer);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
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
  float xPos{0};
  float yPos{0};
  float xVel{0};
  float yVel{0};
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
  float nextMovement{0.25};
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

      obj.xVel = 32;

      obj.alive = true;
    }
  }

  Invader *findInvaderAtPosition(int x, int y)
  {
    for (int i = 0; i < NUM_INVADERS; i++)
    {
      Invader &obj = invaders[i];
      if (!obj.alive)
      {
        continue;
      }
      bool a = x >= obj.rect.x;
      bool b = x <= obj.rect.x + obj.rect.w;
      bool c = y >= obj.rect.y;
      bool d = y <= obj.rect.y + obj.rect.h;

      if (a && b && c && d)
      {
        return &obj;
      }
    }
    return nullptr;
  }

  Invader *findFirstInvaderAliveInRow(int row)
  {
    for (int j = 0; j < NUM_INVADERS_ACROSS; j++)
    {
      Invader &obj = invaders[j + (row * NUM_INVADERS_ACROSS)];
      if (obj.alive)
      {
        return &obj;
        break;
      }
    }
    return nullptr;
  }

  Invader *findLeftMostInvaderAliveInRow(int row)
  {
    return findFirstInvaderAliveInRow(row);
  }

  Invader *findRightMostInvaderAliveInRow(int row)
  {
    for (int j = NUM_INVADERS_ACROSS - 1; j >= 0; j--)
    {
      Invader &obj = invaders[j + (row * NUM_INVADERS_ACROSS)];
      if (obj.alive)
      {
        return &obj;
        break;
      }
    }
    return nullptr;
  }

  void dropAndReverseDirectionOfInvaders()
  {
    for (int j = 0; j < NUM_INVADERS; j++)
    {
      Invader &obj = invaders[j];
      if (obj.alive)
      {
        obj.xVel = -obj.xVel;
        obj.yPos += INVADER_HEIGHT;
        // kill the invader when it hits the bottom of the screen
        if (obj.yPos > SCREEN_HEIGHT)
        {
          obj.alive = false;
        }
      }
    }
  }

  void update(float deltaTime)
  {
    // for each row of invaders
    for (int i = 0; i < NUM_INVADERS_DOWN; i++)
    {
      // find the first invader in the row that is alive
      Invader *ptr = findFirstInvaderAliveInRow(i);
      if (ptr == nullptr)
      {
        // no invaders alive in this row
        // go to next row
        continue;
      }

      // what direction is the row moving?
      Invader &leader = *ptr;
      if (leader.xVel < 0)
      {
        // moving left
        // find the leftmost invader of all invaders in this row
        Invader &leftmost = *findLeftMostInvaderAliveInRow(i);
        // if the x position is <= left edge of screen,
        if (leftmost.xPos <= 0)
        {
          // move all invaders in row down the height of an invader
          // reverse the x velocity for all invaders in this row
          dropAndReverseDirectionOfInvaders();
        }
      }
      else if (leader.xVel > 0)
      {
        // moving right
        // find the rightmost invader of all invaders in this row
        Invader &rightmost = *findRightMostInvaderAliveInRow(i);
        // if the x position + width of invader is >= right edge of screen,
        if (rightmost.xPos + INVADER_WIDTH >= SCREEN_WIDTH)
        {
          // reverse the x velocity for all invaders in this row
          // move all invaders in row down the height of an invader
          dropAndReverseDirectionOfInvaders();
        }
      }
    }

    for (int i = 0; i < NUM_INVADERS; i++)
    {
      Invader &obj = invaders[i];
      obj.update(deltaTime);
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
      case SDL_MOUSEBUTTONDOWN:
      {
        // testing: click on an invader to kill it
        Invader *ptr = invaders.findInvaderAtPosition(event.button.x, event.button.y);
        if (ptr != nullptr)
        {
          Invader &obj = *ptr;
          obj.alive = false;
        }
      }
      break;
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
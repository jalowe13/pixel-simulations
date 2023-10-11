#include "Application.h"

// Upgrade to C++ 11

// Pixels
void Pixel::render(SDL_Renderer *renderer) {
  if (life <= 0) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  } else {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  }
  SDL_RenderDrawPoint(renderer, x, y);
}
void Pixel::update() {
  if (life > 0) {
    if (sway >= 60) {
      l = rand() % 3;
      sway = 0;
    }
    sway++;
    int f = rand() % 2;
    switch (f) {
    case (0):
      if ((x + l) < SCREEN_WIDTH) {
        x = x + l;
      }
      break;
    case (1):
      if ((x - l) >= 0) {
        x = x - l;
      }
      break;
    }
    life--;
  } else {
    if (clean > 0) {
      clean--;
    } else {
      if (y != SCREEN_HEIGHT - 1) {
        y++;
      }
    }
  }
}

#ifdef _WIN32
#else
uint32_t linux_tick() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ((ts.tv_sec * 1000) + (ts.tv_nsec / 1000000));
}
#endif

void Application::setRefreshRate() {
  DEVMODE dm;
  dm.dmSize = sizeof(dm);
  EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
  this->fps = dm.dmDisplayFrequency;
  std::cout << "Refresh Rate: " << this->fps << std::endl;
}

Application::Application() {
  std::cout << "-----Application Created\n";
  gameRunning = true;
  frameCount = 0;
  for (int i = 0; i < SCREEN_WIDTH; i++) {
    for (int j = 0; j < SCREEN_HEIGHT; j++) {
      screen[i][j] = Pixel();
    }
  }
// Windows and Linux Definitions
#ifdef _WIN32
  startTime = GetTickCount();
  endTime = GetTickCount();
  setRefreshRate();
#else
  startTime = linux_tick();
  endTime = linux_tick();
#endif
  timeDifference = 0;
  frameAverage = 0;
}

Application::~Application() { std::cout << "-----Application Destroyed\n"; }

bool Application::init() {
  try {
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
      std::cout << "-----SDL Init Done" << std::endl;
      window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

      if (!window) {
        throw "Window creation failed!";
      }

      std::cout << "-----Window Created" << std::endl;

      renderer =
          SDL_CreateRenderer(window, -1,
                             SDL_RENDERER_PRESENTVSYNC |
                                 SDL_RENDERER_ACCELERATED); // Create renderer
      SDL_SetRenderDrawColor(renderer, 0, 0, 0,
                             0); // Set clear color to white

      if (!renderer) {
        throw "Renderer creation failed.";
      }

      std::cout << "-----Renderer Created" << std::endl;

      // Loading texture memory
      SDL_Texture *temp_tex = NULL;

      gameRunning = true;
    } else {
      gameRunning = false;
    }

    return true;
  } catch (const char *error) {
    std::cout << error << std::endl;
    return false;
  }
}

// Other Application Functions
void Application::handleEvents() {
  SDL_Event event;
  SDL_PollEvent(&event);
  switch (event.type) {
  case SDL_MOUSEBUTTONDOWN: {
    // Handle mouse button down
    int x, y;
    Uint32 mouseCords = SDL_GetMouseState(&x, &y);
    std::cout << "Mouse cords " << x << "," << y << std::endl;
    // std::cout << "Click!\n";
    break;
  }
  case SDL_QUIT:
    gameRunning = false;
    break;
  case SDL_KEYDOWN:
    switch (event.key.keysym.sym) {}
    break;
  case SDL_KEYUP:
    switch (event.key.keysym.sym) {
    case SDLK_w: {
      break;
    }
    case SDLK_a: {
      break;
    }
    case SDLK_s: {
      break;
    }
    case SDLK_d: {
      break;
    }
    // Debug Mode Toggle
    case SDLK_BACKQUOTE: {
      debugMode = (debugMode) ? false : true;
      std::cout << "Debug Mode: " << debugMode << std::endl;
      break;
    }
    case SDLK_KP_0: {
      break;
    }
    case SDLK_KP_1: {
      break;
    }
    case SDLK_KP_2: {
    } break;
    }
  }
}

void Application::update() // Update Logic
{
  // Logic --> Physics --> Render
  checkPixelCollisions();
}

void Application::checkPixelCollisions() {
  for (auto &p1 : pixels) { // O(N) collision checking
    int old_x = p1.getX();
    int old_y = p1.getY();

    p1.update(); // Update Pixel Position x and y

    int x = p1.getX();
    int y = p1.getY();

    // Screen bounds check
    if (((0 <= x) && (x < SCREEN_WIDTH) && (0 <= y) && (y < SCREEN_HEIGHT)) &&
        ((0 <= old_x) && (old_x < SCREEN_WIDTH) && (0 <= old_y) &&
         (old_y < SCREEN_HEIGHT))) {
      screen[old_x][old_y].setEmpty(true);
      if (screen[x][y].checkEmpty()) { // Check if new position is empty
        screen[x][y] = p1;
      } else { // If there is a pixel there set both lifes to 0 (COLLIDE)
        p1.life = 0;
        screen[x][y].life = 0;
      }
      if ((p1.getLife() == 0) && !p1.getDone()) { // If pixel done
        p1.setDone(true);
        green++;
        red--;
      }
    }
  }
}

void Application::render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer); // Clear Screen
  // C++ Unique Pointers, References and Ownership
  if (pixels.size() < max_pixels) {
    Pixel pixel((rand() % (SCREEN_WIDTH + 1)), (rand() % (SCREEN_HEIGHT + 1)));
    red++;
    pixels.push_back(pixel);
  }
  std::cout << "Red[" << red << "] Green[" << green << "]\r";

  for (auto &p : pixels) {
    p.render(renderer);
  }
  // std::cout << "Try\n";
  SDL_RenderPresent(renderer);
  // std::cout << "Pre\n";
}

void Application::clean() {
  SDL_DestroyWindow(window); // destroy the window
  SDL_Quit();                // quit and delete all SDL
}

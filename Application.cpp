#include "Application.h"

// Upgrade to C++ 11

// Pixels
std::random_device Pixel::rd;
std::default_random_engine Pixel::generator(Pixel::rd());
std::bernoulli_distribution Pixel::distribution(0.31);
Pixel::Pixel(int posX, int posY) {
  x = posX;
  y = posY;
  isEmpty = false;
  leader = distribution(generator);
  color = leader ? SDL_Color{255, 255, 0, 255} : SDL_Color{255, 0, 0, 255};
}
void Pixel::render(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawPoint(renderer, x, y);
}
int Pixel::detectScare() {
  int x, y;
  Uint32 mouseCords = SDL_GetMouseState(&x, &y);

  // Mouse Left of Pixel
  if (((x - this->x) > 0) && ((x - this->x) <= 20) &&
      (abs(y - this->y) <= 20)) {
    return 0;
  }
  // Mouse Below Pixel
  if (((y - this->y) > 0) && ((y - this->y) <= 20) &&
      (abs(x - this->x) <= 20)) {
    return 3;
  }
  // Mouse Right of Pixel
  if (((this->x - x) > 0) && ((this->x - x) <= 20) &&
      (abs(y - this->y) <= 20)) {
    return 1;
  }
  // Mouse Above Pixel
  if (((this->y - y) > 0) && ((this->y - y) <= 20) &&
      (abs(x - this->x) <= 20)) {
    return 2;
  }
  return 4;
}
void Pixel::moveDirection(int i) {
  switch (i) {
  case (0): // Move right
    if ((x + l) < SCREEN_WIDTH) {
      x = x + l;
    }
    break;
  case (1): // Move left
    if ((x - l) >= 0) {
      x = x - l;
    }
    break;
  case (2): // Move down
    if ((y + l) < SCREEN_HEIGHT) {
      y = y + l;
    }
    break;
  case (3): // Move up
    if ((y - l) >= 0) {
      y = y - l;
    }
    break;
  case (4): // Random
    moveDirection(rand() % 4);
  }
}
void Pixel::update() {
  if (life <= 0) {
    color = {0, 255, 0, 255}; // Green
  }
  if (life > 0) {
    if (sway >= 60) {
      l = (rand() % 2) + 1;
      sway = 0;
    }
    sway++;
    int f = detectScare();
    // moveDirection(f);
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

int Physics::boid_seperation(Pixel *p, std::vector<Pixel> *pixels) {
  // std::cout << "Seperate\n";
  return 0;
}
int Physics::boid_alignment(Pixel *p, std::vector<Pixel> *pixels) {
  // std::cout << "Alignment\n";
  return 0;
}
int Physics::boid_cohesion(Pixel *p, std::vector<Pixel> *pixels) {
  // std::cout << "Cohesion\n";
  if (p->isLeader()) {
    // if (leader_position[0] == 0 && leader_position[1] == 0) {
    leader_position[0] = p->getX();
    leader_position[1] = p->getY();
    num_leaders++;
    // }
  } else if (p->getLife() > 0) {
    if ((leader_position[0] - 1) > p->getX()) {
      p->setX(p->getX() + 1);
    } else {
      p->setX(p->getX() - 1);
    }
    if ((leader_position[1] - 1) > p->getY()) {
      p->setY(p->getY() + 1);
    } else {
      p->setY(p->getY() - 1);
    }
  }
  if (num_leaders > 0) {
    // leader_position[0] = leader_position[0] / num_leaders;
    // leader_position[1] = leader_position[1] / num_leaders;
    // std::cout << leader_position[0] << "," << leader_position[1] <<
    // std::endl;
  }
  return 0;
}
void Physics::boid_update(Pixel *p, std::vector<Pixel> *pixels) {
  boid_seperation(p, pixels);
  boid_alignment(p, pixels);
  boid_cohesion(p, pixels);
};

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
      window = SDL_CreateWindow(
          windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

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

      std::cout << "-----Start Physics Engine\n";
      phys_eng = new Physics();

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
  case SDL_MOUSEMOTION:
    SDL_GetMouseState(&xMouse, &yMouse);
    // std::cout << "x" << xMouse << "y" << yMouse << std::endl;
    break;
  case SDL_MOUSEBUTTONDOWN: {
    mouseDown = true;
    break;
  }
  case SDL_MOUSEBUTTONUP:
    mouseDown = false;
    break;
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
    case SDLK_c: {
      collisionMode = collisionMode ? false : true;
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
  spawnPixel();
  for (auto &p : pixels) {
    p.detectScare();
    phys_eng->boid_update(&p, &pixels);
  }
  checkPixelCollisions();
}

void Application::spawnPixel() {
  if (mouseDown) {
    int x, y;
    Uint32 mouseCords = SDL_GetMouseState(&x, &y);
    Pixel pixel(x, y);
    pixel.setLeader(false); // Left clicked pixels are not leaders
    pixel.setColor(SDL_Color{255, 0, 0, 255});
    pixels.push_back(pixel);
    red++;
    total++;
  } else if (pixels.size() < max_pixels) {
    Pixel pixel((rand() % (SCREEN_WIDTH + 1)), (rand() % (SCREEN_HEIGHT + 1)));
    pixels.push_back(pixel);
    red++;
    total++;
  }
}

bool Application::checkBounds(int x, int y) {
  return ((0 <= x) && (x < SCREEN_WIDTH) && (0 <= y) && (y < SCREEN_HEIGHT));
}

void Application::checkPixelCollisions() {
  for (auto &p1 : pixels) { // O(N) collision checking
    int old_x = p1.getX();
    int old_y = p1.getY();

    p1.update(); // Update Pixel Position x and y

    int x = p1.getX();
    int y = p1.getY();

    // Screen bounds check
    if (checkBounds(x, y) && checkBounds(old_x, old_y)) {
      screen[old_x][old_y].setEmpty(true);
      if (screen[x][y].checkEmpty()) { // Check if new position is empty
        screen[x][y] = p1;
      } else { // If there is a pixel there set both lifes to 0 (COLLIDE)
        if (collisionMode) {
          p1.life = 0;
          screen[x][y].life = 0;
        }
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
  std::cout << "Collision Mode:" << collisionMode << " Red[" << red
            << "] Green[" << green << "]"
            << "Total[" << total << "]\r";

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

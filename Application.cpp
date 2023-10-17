#include "Application.h"

// Upgrade to C++ 11

// Pixels
std::random_device Pixel::rd;
std::default_random_engine Pixel::generator(Pixel::rd());
std::bernoulli_distribution Pixel::distribution(0.01);
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
    return 2;
  }
  // Mouse Below Pixel
  if (((y - this->y) > 0) && ((y - this->y) <= 20) &&
      (abs(x - this->x) <= 20)) {
    return 1;
  }
  // Mouse Right of Pixel
  if (((this->x - x) > 0) && ((this->x - x) <= 20) &&
      (abs(y - this->y) <= 20)) {
    return 3;
  }
  // Mouse Above Pixel
  if (((this->y - y) > 0) && ((this->y - y) <= 20) &&
      (abs(x - this->x) <= 20)) {
    return 0;
  }
  return 4;
}

bool Pixel::directionMoveablePureCheck(
    Pixel (&screen)[SCREEN_WIDTH][SCREEN_HEIGHT], int direction) {
  switch (direction) {
  case (0): // North
    if ((0 <= x) && (x < SCREEN_WIDTH) && (0 <= (y - 1)) &&
        ((y - 1) < SCREEN_HEIGHT)) {
      return true;
    }
    break;
  case (1): // South
    if ((0 <= x) && (x < SCREEN_WIDTH) && (0 <= (y + 1)) &&
        ((y + 1) < SCREEN_HEIGHT)) {
      return true;
    }
    break;
  case (2): // East
    if ((0 <= (x - 1)) && ((x - 1) < SCREEN_WIDTH) && (0 <= (y)) &&
        ((y) < SCREEN_HEIGHT)) {
      return true;
    }
    break;
  case (3): // West
    if ((0 <= (x + 1)) && ((x + 1) < SCREEN_WIDTH) && (0 <= (y)) &&
        ((y) < SCREEN_HEIGHT)) {
      return true;
    }
    break;
  }
  return false;
}

int Pixel::directionsMoveable(Pixel (&screen)[SCREEN_WIDTH][SCREEN_HEIGHT]) {
  std::deque<std::pair<int, bool>> directions;
  // Check directions NSEW
  int scaredDir = detectScare();
  if ((0 <= x) && (x < SCREEN_WIDTH) && (0 <= (y - 1)) &&
      ((y - 1) < SCREEN_HEIGHT)) {
    const std::pair<int, bool> north(0, ((screen[x][y - 1]).checkEmpty()));
    if (scaredDir != 0) {
      directions.push_back(north);
    }
  }
  if ((0 <= x) && (x < SCREEN_WIDTH) && (0 <= (y + 1)) &&
      ((y + 1) < SCREEN_HEIGHT)) {
    const std::pair<int, bool> south(1, ((screen[x][y + 1]).checkEmpty()));
    if (scaredDir != 1) {
      directions.push_back(south);
    }
  }
  if ((0 <= (x - 1)) && ((x - 1) < SCREEN_WIDTH) && (0 <= (y)) &&
      ((y) < SCREEN_HEIGHT)) {
    const std::pair<int, bool> east(2, ((screen[x - 1][y]).checkEmpty()));
    if (scaredDir != 2) {
      directions.push_back(east);
    }
  }
  if ((0 <= (x + 1)) && ((x + 1) < SCREEN_WIDTH) && (0 <= (y)) &&
      ((y) < SCREEN_HEIGHT)) {
    const std::pair<int, bool> west(3, ((screen[x + 1][y]).checkEmpty()));
    if (scaredDir != 3) {
      directions.push_back(west);
    }
  }

  std::vector<int> true_directions;
  for (auto &d : directions) {
    if (d.second) {
      true_directions.push_back(d.first);
    }
  }
  if (true_directions.size() == 0) { // If the pixel cannot move
    return 4;                        // Code for no moving
  }
  const int result = true_directions[rand() % true_directions.size()];
  return result;
}

bool Pixel::moveDirection(int i) {
  switch (i) {
  case (0): // Move up
    if ((y - 1) >= 0) {
      y = y - 1;
      return true;
    }
    break;
  case (1): // Move down
    if ((y + 1) < SCREEN_HEIGHT) {
      y = y + 1;
      return true;
    }
    break;
  case (2): // Move left
    if ((x - 1) >= 0) {
      x = x - 1;
      return true;
    }
    break;
  case (3): // Move right
    if ((x + 1) < SCREEN_WIDTH) {
      x = x + 1;
      return true;
    }
    break;
  case (4): // Cant move (Do nothing)
    break;
  }
  return false;
}

void Pixel::update() {
  if (life <= 0) {
    color = {0, 255, 0, 255}; // Green
  }
  if (life > 0) {
    life--;
  } else {
    if (clean > 0) {
      clean--;
      cleanup--;
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
int Physics::boid_cohesion(Pixel *p, std::vector<Pixel> *pixels,
                           Pixel (&screen)[SCREEN_WIDTH][SCREEN_HEIGHT]) {
  // std::cout << "Cohesion\n";
  if (p->getLife() > 0) {
    if ((leader_position[0]) > p->getX() + 1) { // If leader is on Right
      if (p->directionMoveablePureCheck(screen, 3)) {
        p->setX(p->getX() + 1); // Move 1 to leader
      }
    } else if ((leader_position[0]) < p->getX() - 1) { // If leader is on Left
      if (p->directionMoveablePureCheck(screen, 2)) {
        p->setX(p->getX() - 1); // Move 1 to leader
      }
    }
    if ((leader_position[0]) > p->getY() + 1) { // If leader is Below
      if (p->directionMoveablePureCheck(screen, 1)) {
        p->setY(p->getY() + 1); // Move 1 to leader
      }
    } else if ((leader_position[0]) < p->getX() - 1) { // If leader is Above
      if (p->directionMoveablePureCheck(screen, 0)) {
        p->setY(p->getY() - 1); // Move 1 to leader
      }
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
void Physics::boid_update(Pixel *p, std::vector<Pixel> *pixels,
                          Pixel (&screen)[SCREEN_WIDTH][SCREEN_HEIGHT]) {
  // boid_seperation(p, pixels);
  // boid_alignment(p, pixels);
  boid_cohesion(p, pixels, screen);
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
bool Application::checkBounds(int x, int y) {
  return ((0 <= x) && (x < SCREEN_WIDTH) && (0 <= y) && (y < SCREEN_HEIGHT));
}
void Application::update() // Update Logic
{
  // Logic --> Physics --> Render
  spawnPixel(phys_eng);
  for (auto &p : pixels) {
    bool boundCheck = checkBounds(p.getX(), p.getY());
    bool belowBoundCheck = checkBounds(p.getX(), p.getY() + 1);
    Pixel *p_2 = &screen[p.getX()][p.getY() + 1];
    if (!p.isDone()) {
      checkPixelCollisions(&p);
      if (!p.isLeader()) {
        // phys_eng->boid_update(&p, &pixels, screen);
      } else {
        if (!p.isDone()) {
          phys_eng->setLeaderPosX(p.getX());
          phys_eng->setLeaderPosY(p.getY());
        }
      }
      p.update();
    }
    // Else if still in bounds on my pixel and next pixel
    else if (boundCheck && belowBoundCheck) {
      if (!p.landed) {
        if (p_2->isDone()) {
          screen[p.getX()][p.getY()] = p;
          p.landed = true;
          // std::cout << "Landed on top" << p.getX() << std::endl;
        } else {
          p.fall();
        }
      }
    }

    // Cant check next pixel, must be on the ground condition
    if ((p.getY() == (SCREEN_HEIGHT - 1)) && !p.landed && p.isDone()) {
      screen[p.getX()][p.getY()] = p;
      p.landed = true;
      // std::cout << "Landed" << p.getX() << std::endl;
    }
  }
}

void Application::spawnPixel(Physics *phys_eng) {
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
    if (phys_eng->getLeaders() == 0) {
      pixel.setLeader(true);
      pixel.setColor(SDL_Color{255, 255, 0, 255});
      phys_eng->addPixelLeader(pixel);
    }
    red++;
    total++;
    if (pixel.isLeader()) {
      phys_eng->incLeaders();
    }
    pixels.push_back(pixel);
  }
}

void Application::checkPixelCollisions(Pixel *p1) {
  int old_x = p1->getX();
  int old_y = p1->getY();
  p1->moveDirection(p1->directionsMoveable(screen));
  int x = p1->getX();
  int y = p1->getY();
  // std::cout << "[" << old_x << "," << old_y << "]"
  //           << "[" << x << "," << y << "]" << std::endl;

  // Screen bounds check
  if (checkBounds(x, y) && checkBounds(old_x, old_y)) {
    screen[old_x][old_y].setEmpty(true);
    if (screen[x][y].checkEmpty()) { // Check if new position is empty
      screen[x][y] = *p1;
    }
    if ((p1->getLife() == 0) && !p1->isDone()) { // If pixel done
      p1->setDone(true);
      green++;
      red--;
    }
  }
}

void Application::render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer); // Clear Screen
  // C++ Unique Pointers, References and Ownership
  std::cout << "Red[" << red << "] Green[" << green << "]"
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

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

// Version Number
#define VERSION_MAJOR 0
#define VERSION_MINOR 4
#define VERSION_PATCH
#define VERSION_ALT
#define STR_HELPER(x) #x // convert to fit window title
#define STR(x) STR_HELPER(x)

// Screen Definitions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <fstream>
#include <iostream>
#include <json/json.h>
#include <memory>
#include <vector>

// PreProcessor Declerations
#ifdef _WIN32
#include <windows.h>
#else
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#endif

// Some forward declerations for pointers
class Player;
class Enemy;
class Terrain;
class Physics;

class Pixel {
public:
  bool isEmpty;
  int life = rand() % 8640;
  int clean = 60;
  int x, y;
  int l = rand() % 2;
  int sway = 0;
  bool done = false;
  Pixel() { isEmpty = true; }
  Pixel(int posX, int posY) {
    x = posX;
    y = posY;
    isEmpty = false;
  }
  void setX(int newX) { x = newX; }
  void setY(int newY) { y = newY; }
  void setDone(bool newDone) { done = newDone; }
  void setEmpty(bool newEmpty) { isEmpty = newEmpty; }
  int getX() { return x; }
  int getY() { return y; }
  int getLife() { return life; }
  bool getDone() { return done; }
  bool checkEmpty() { return isEmpty; }
  void render(SDL_Renderer *renderer);
  void update();
};

class Application {
public:
  const char *windowTitle =
      "C23 Engine: Pixel Simulations v." STR(VERSION_MAJOR) "." STR(
          VERSION_MINOR) STR(VERSION_PATCH) STR(VERSION_ALT) " FPS:";
  Application();
  ~Application();

  // Refresh rate
  void setRefreshRate();
  // Windows and Surfaces
  SDL_Window *getWindow() { return window; }
  SDL_Surface *getSurface() { return surface; }
  SDL_Renderer *getRenderer() { return renderer; }
  const int getScreenWidth() { return SCREEN_WIDTH; }
  const int getScreenHeight() { return SCREEN_HEIGHT; }
  void setWindow(SDL_Window *new_window) { window = new_window; }
  void setSurface(SDL_Surface *new_surface) { surface = new_surface; }
  void setRenderer(SDL_Renderer *new_renderer) { renderer = new_renderer; }

  // Check collisions between pixels on this frame
  void checkPixelCollisions();

  // Events
  bool init();
  void handleEvents();
  void update();
  void render();
  void clean();
  bool running() { return gameRunning; }

  // Getters
  // const std::vector<Pixel> &getPixels() const { return pixels; }

  // Text Events
  void createText(const char *text, int x, int y);
  void createTexture(const char *filename, float x, float y);

  // Pixel Stats
  int red = 0;
  int green = 0;
  const int max_pixels = 2023;

  // Constants for reference

  bool debugMode = false;

  int textureWidth = 1920;
  int textureHeight = 32;
  int frameWidth, frameHeight;

  int fps = 60;
  int current_fps = 0;
  int frame_time = 0;

private:
  // Pointers
  SDL_Window *window = NULL;         // Pointer to Window
  SDL_Renderer *renderer = NULL;     // Render graphics
  SDL_Surface *surface = NULL;       // Pointer to Surface in Window
  SDL_Surface *sf_background = NULL; // Background pointer
  SDL_Surface *surface_temp = NULL;

  Terrain *terrain_gen = nullptr; // Terrain Generation

  // Screen
  Pixel screen[SCREEN_WIDTH][SCREEN_HEIGHT];

  // Pixels
  std::vector<Pixel> pixels;

  // Rectangles
  SDL_Rect *titleRect; // Title Location rectangle

  // Physics Engine
  Physics *phys_eng = nullptr; // Pointer to Physics Engine

  // Textures
  SDL_Texture *ground = nullptr;
  SDL_Texture *air = nullptr;

  SDL_Texture *fancy_text = nullptr;

  // Temporary pointers for enemy and player instantiation
  Player *player = NULL;
  Enemy *enemy = NULL;

  // Game Variables
  bool gameRunning;
  int frameCount;
  int startTime;
  int endTime;
  int timeDifference;
  float frameAverage;
};

#endif

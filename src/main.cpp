#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
SDL_Surface *loadSurface();
bool loadMedia();
void loop();
void close();

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gHelloWorld = NULL;

bool init() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL failed to initialise! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (gWindow == NULL) {
    printf("Unable to create window! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags)) {
    printf("Image flags failed to initialise: SDL_image Error: %s\n", IMG_GetError());
    return false;
  }

  gScreenSurface = SDL_GetWindowSurface(gWindow);
  return true;
}

SDL_Surface *loadSurface(std::string path) {
  SDL_Surface *loadedSurface = IMG_Load(path.c_str());
  if (loadedSurface == NULL) {
    printf("Failed to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    return NULL;
  }

  SDL_Surface *optimisedImage = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
  if (optimisedImage == NULL) {
    printf("Unable to optimise image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    SDL_FreeSurface(loadedSurface);
  }

  return optimisedImage;
}

bool loadMedia() {
  gHelloWorld = loadSurface("assets/hi.png");
  return true;
}

void loop() {
  bool running = true;
  SDL_Event e;

  while (running) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        running = false;
      }
    }

    SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
    SDL_UpdateWindowSurface(gWindow);
  }
}

void close() {
  SDL_FreeSurface(gHelloWorld);
  gHelloWorld = NULL;

  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  SDL_Quit();
}

int main(int argc, char *args[]) {
  if (!init()) {
    printf("Initialisation failed!\n");
    return 0;
  }

  if (!loadMedia()) {
    printf("Load media failed!\n");
    return 0;
  }

  loop();
  close();
  return 0;
}

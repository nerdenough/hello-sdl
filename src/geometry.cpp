#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
SDL_Texture *loadTexture(std::string path);
bool loadMedia();
void loop();
void close();

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

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

  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
  if (gRenderer == NULL) {
    printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags)) {
    printf("SDL_image could not initialise: SDL_image Error: %s\n", IMG_GetError());
    return false;
  }

  return true;
}

bool loadMedia() {
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

    // Clear screen, render texture, and update the screen
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
    SDL_RenderPresent(gRenderer);
  }
}

void close() {
  SDL_DestroyRenderer(gRenderer);
  gRenderer = NULL;

  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  IMG_Quit();
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

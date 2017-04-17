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
SDL_Texture *gTexture = NULL;

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

SDL_Texture *loadTexture(std::string path) {
  SDL_Surface *loadedSurface = IMG_Load(path.c_str());
  if (loadedSurface == NULL) {
    printf("Failed to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    return NULL;
  }

  SDL_Texture *newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
  if (newTexture == NULL) {
    printf("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
  }

  SDL_FreeSurface(loadedSurface);
  return newTexture;
}

bool loadMedia() {
  gTexture = loadTexture("assets/default.png");
  if (gTexture == NULL) {
    printf("Error loading default image\n");
    return false;
  }

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

    SDL_RenderClear(gRenderer);

    SDL_Rect topLeftViewport;
    topLeftViewport.x = 0;
    topLeftViewport.y = 0;
    topLeftViewport.w = SCREEN_WIDTH / 2;
    topLeftViewport.h = SCREEN_HEIGHT / 2;
    SDL_RenderSetViewport(gRenderer, &topLeftViewport);
    SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

    SDL_Rect topRightViewport;
    topRightViewport.x = SCREEN_WIDTH / 2;
    topRightViewport.y = 0;
    topRightViewport.w = SCREEN_WIDTH / 2;
    topRightViewport.h = SCREEN_HEIGHT / 2;
    SDL_RenderSetViewport(gRenderer, &topRightViewport);
    SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

    SDL_Rect bottomViewport;
    bottomViewport.x = 0;
    bottomViewport.y = SCREEN_HEIGHT / 2;
    bottomViewport.w = SCREEN_WIDTH;
    bottomViewport.h = SCREEN_HEIGHT / 2;
    SDL_RenderSetViewport(gRenderer, &bottomViewport);
    SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

    SDL_RenderPresent(gRenderer);
  }
}

void close() {
  SDL_DestroyTexture(gTexture);
  gTexture = NULL;

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

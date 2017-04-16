#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum KeyPressSurfaces {
  KEY_PRESS_SURFACE_DEFAULT,
  KEY_PRESS_SURFACE_UP,
  KEY_PRESS_SURFACE_DOWN,
  KEY_PRESS_SURFACE_LEFT,
  KEY_PRESS_SURFACE_RIGHT,
  KEY_PRESS_SURFACE_TOTAL
};

bool init();
SDL_Surface *loadSurface(std::string path);
bool loadMedia();
void loop();
void close();

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Surface *gCurrentSurface = NULL;

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
  gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("assets/default.png");
  if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL) {
    printf("Failed to load default image!\n");
    return false;
  }

  gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("assets/up.png");
  if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL) {
    printf("Failed to load up image!\n");
    return false;
  }

  gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("assets/down.png");
  if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL) {
    printf("Failed to load down image!\n");
    return false;
  }

  gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("assets/left.png");
  if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL) {
    printf("Failed to load left image!\n");
    return false;
  }

  gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("assets/right.png");
  if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL) {
    printf("Failed to load right image!\n");
    return false;
  }

  return true;
}

void loop() {
  bool running = true;
  SDL_Event e;
  gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

  while (running) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        running = false;
      } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
          case SDLK_w:
          case SDLK_UP:
          gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
          break;

          case SDLK_s:
          case SDLK_DOWN:
          gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
          break;

          case SDLK_a:
          case SDLK_LEFT:
          gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
          break;

          case SDLK_d:
          case SDLK_RIGHT:
          gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
          break;

          default:
          gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
          break;
        }
      }
    }

    SDL_Rect stretchRect;
    stretchRect.x = 0;
    stretchRect.y = 0;
    stretchRect.w = SCREEN_WIDTH;
    stretchRect.h = SCREEN_HEIGHT;

    SDL_BlitScaled(gCurrentSurface, NULL, gScreenSurface, &stretchRect);
    SDL_UpdateWindowSurface(gWindow);
  }
}

void close() {
  SDL_FreeSurface(gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT]);
  gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = NULL;

  SDL_FreeSurface(gKeyPressSurfaces[KEY_PRESS_SURFACE_UP]);
  gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = NULL;

  SDL_FreeSurface(gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN]);
  gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = NULL;

  SDL_FreeSurface(gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT]);
  gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = NULL;

  SDL_FreeSurface(gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT]);
  gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = NULL;

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

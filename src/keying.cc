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

class LTexture {
  public:
    LTexture();
    ~LTexture();
    bool loadFromFile(std::string path);
    void free();
    void render(int x, int y);
    int getWidth();
    int getHeight();

  private:
    SDL_Texture *mTexture;
    int mWidth;
    int mHeight;
};

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
LTexture gFooTexture;
LTexture gBackgroundTexture;

LTexture::LTexture() {
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
}

LTexture::~LTexture() {
  free();
}

bool LTexture::loadFromFile(std::string path) {
  free();

  SDL_Surface *loadedSurface = IMG_Load(path.c_str());
  if (loadedSurface == NULL) {
    printf("Error loading surface: %s\n", IMG_GetError());
    return false;
  }

  SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0, 0xFF)); // magenta
  mTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
  if (mTexture == NULL) {
    printf("Error creating texture: %s\n", SDL_GetError());
    return false;
  }

  mWidth = loadedSurface->w;
  mHeight = loadedSurface->h;
  SDL_FreeSurface(loadedSurface);

  return true;
}

void LTexture::free() {
  if (mTexture == NULL) {
    SDL_DestroyTexture(mTexture);
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
  }
}

void LTexture::render(int x, int y) {
  SDL_Rect renderQuad = { x, y, mWidth, mHeight };
  SDL_RenderCopy(gRenderer, mTexture, NULL, &renderQuad);
}

int LTexture::getWidth() {
  return mWidth;
}

int LTexture::getHeight() {
  return mHeight;
}

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
  if (!gFooTexture.loadFromFile("assets/foo.png")) {
    printf("Failed to load assets/foo.png!\n");
    return false;
  }

  if (!gBackgroundTexture.loadFromFile("assets/background.png")) {
    printf("Failed to load assets/background.png!\n");
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

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    gBackgroundTexture.render(0, 0);
    gFooTexture.render(240, 190);

    SDL_RenderPresent(gRenderer);
  }
}

void close() {
  gFooTexture.free();
  gBackgroundTexture.free();

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

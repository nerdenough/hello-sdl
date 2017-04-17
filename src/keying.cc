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
    void setColour(Uint8 red, Uint8 green, Uint8 blue);
    void render(int x, int y, SDL_Rect *clip = NULL);
    int getWidth();
    int getHeight();

  private:
    SDL_Texture *mTexture;
    int mWidth;
    int mHeight;
};

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Rect gSpriteClips[4];
LTexture gSpriteTexture;

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

void LTexture::setColour(Uint8 red, Uint8 green, Uint8 blue) {
  SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::render(int x, int y, SDL_Rect *clip) {
  SDL_Rect renderQuad = { x, y, mWidth, mHeight };

  if (clip != NULL) {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }

  SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
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
  if (!gSpriteTexture.loadFromFile("assets/circles.png")) {
    printf("Failed to load assets/circle.png!\n");
    return false;
  }

  gSpriteClips[0].x = 0;
  gSpriteClips[0].y = 0;
  gSpriteClips[0].w = 100;
  gSpriteClips[0].h = 100;

  gSpriteClips[1].x = 100;
  gSpriteClips[1].y = 0;
  gSpriteClips[1].w = 100;
  gSpriteClips[1].h = 100;

  gSpriteClips[2].x = 0;
  gSpriteClips[2].y = 100;
  gSpriteClips[2].w = 100;
  gSpriteClips[2].h = 100;

  gSpriteClips[3].x = 100;
  gSpriteClips[3].y = 100;
  gSpriteClips[3].w = 100;
  gSpriteClips[3].h = 100;

  return true;
}

void loop() {
  bool running = true;
  SDL_Event e;
  Uint8 red = 255;
  Uint8 green = 255;
  Uint8 blue = 255;

  while (running) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        running = false;
      } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
          case SDLK_q:
            red += 32;
            break;
          case SDLK_w:
            green += 32;
            break;
          case SDLK_e:
            blue += 32;
            break;
          case SDLK_a:
            red -= 32;
            break;
          case SDLK_s:
            green -= 32;
            break;
          case SDLK_d:
            blue -= 32;
            break;
        }
      }
    }

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(gRenderer);

    gSpriteTexture.setColour(red, green, blue);

    gSpriteTexture.render(0, 0, &gSpriteClips[0]);
    gSpriteTexture.render(SCREEN_WIDTH - gSpriteClips[1].w, 0, &gSpriteClips[1]);
    gSpriteTexture.render(0, SCREEN_HEIGHT - gSpriteClips[2].h, &gSpriteClips[2]);
    gSpriteTexture.render(SCREEN_WIDTH - gSpriteClips[3].w, SCREEN_HEIGHT - gSpriteClips[3].h, &gSpriteClips[3]);

    SDL_RenderPresent(gRenderer);
  }
}

void close() {
  gSpriteTexture.free();

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

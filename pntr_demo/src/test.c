#define PNTR_ENABLE_DEFAULT_FONT
#define PNTR_PIXELFORMAT_RGBA
#define PNTR_IMPLEMENTATION
#include "pntr.h"
#include <stdlib.h>
#include <time.h>
#include "utils.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

static FILE* fb;
static pntr_image* screen;
static pntr_font* defaultFont;
static char* fpsText;
static int frameCount = 0;

// ddraw some random circles
void drawCircles(){
  static int x[3] = {0,0,0};
  static int y[3] = {0,0,0};

  // only update every 20 frames
  if (frameCount % 20 == 0) {
    x[0] = rando() % SCREEN_WIDTH + 40;
    x[1] = rando() % SCREEN_WIDTH+ 40;
    x[2] = rando() % SCREEN_WIDTH+ 40;
    y[0] = rando() % SCREEN_HEIGHT+ 40;
    y[1] = rando() % SCREEN_HEIGHT+ 40;
    y[2] = rando() % SCREEN_HEIGHT+ 40;
  }
  pntr_draw_circle_fill(screen, x[0], y[0], 80, PNTR_BLUE);
  pntr_draw_circle_fill(screen, x[1], y[1], 80, PNTR_GREEN);
  pntr_draw_circle_fill(screen, x[2], y[2], 80, PNTR_RED);
}

// draw the FPS-counter
void drawFps(){
  static time_t previousTime = 0;
  static time_t currentTime = 0;
  static int fps = 0;

  frameCount++;
  currentTime = time(NULL);
  if ( currentTime - previousTime >= 1.0 ) {
    previousTime = currentTime;
    fps = frameCount;
    frameCount = 0;
  }
  itoa(fps, fpsText, 10);
  pntr_draw_text(screen, defaultFont, fpsText, 5, 5, PNTR_BLACK);
}

// this will be called by host when it's ready to update
__attribute__((export_name("update")))
void update() {
  pntr_clear_background(screen, PNTR_RAYWHITE);
  drawCircles();
  drawFps();
  pntr_draw_text(screen, defaultFont, "If you can see this, it's working!", 180, SCREEN_HEIGHT/2, PNTR_BLACK);

  // write screen to framebuffer
  if (fb != NULL) {
    fwrite(screen->data, sizeof(char), SCREEN_WIDTH*SCREEN_HEIGHT*4, fb);
  }
}

int main() {
  fpsText = malloc(6);
  defaultFont = pntr_load_font_default();

  printf("Hi, from C.\n");
  screen = pntr_new_image(SCREEN_WIDTH, SCREEN_HEIGHT);

  fb = fopen("/dev/fb0", "wb");
  if (fb == NULL) {
    fprintf(stderr, "Error opening framebuffer\n");
    return 1;
  }
  
  return 0;
}
